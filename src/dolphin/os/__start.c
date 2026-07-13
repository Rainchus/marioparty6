#include <dolphin.h>
#include <dolphin/os.h>

#include <dolphin/os/__os.h>

/*
 * Clean-C startup subset. The __start and __init_registers assembly routines
 * remain owned by the target fallback.
 */

#define PAD3_BUTTON_ADDR 0x800030E4
#define OS_RESET_RESTART 0
#define FALSE 0
#define TRUE 1

u16 Pad3Button AT_ADDRESS(PAD3_BUTTON_ADDR);
static u8 Debug_BBA ATTRIBUTE_ALIGN(4) = 0;

__declspec(weak) void InitMetroTRK_BBA() {}

typedef struct __rom_copy_info {
    char* rom;
    char* addr;
    unsigned int size;
} __rom_copy_info;

__declspec(section ".init") extern __rom_copy_info _rom_copy_info[];

typedef struct __bss_init_info {
    char* addr;
    unsigned int size;
} __bss_init_info;

__declspec(section ".init") extern __bss_init_info _bss_init_info[];
__declspec(section ".init") extern void __flush_cache(void* address, unsigned int size);

__declspec(section ".init") static void __check_pad3(void);
__declspec(section ".init") static void __set_debug_bba(void);
__declspec(section ".init") static u8 __get_debug_bba(void);

static void __check_pad3(void) {
	if ((Pad3Button & 0xEEF) == 0xEEF) {
		OSResetSystem(OS_RESET_RESTART, 0, FALSE);
	}
}

static void __set_debug_bba(void) {
    Debug_BBA = 1;
}

static u8 __get_debug_bba(void) {
    return Debug_BBA;
}

__declspec(section ".init") static inline void __copy_rom_section(void* dst, const void* src, unsigned long size) {
	if (size && dst != src) {
		memcpy(dst, src, size);
		__flush_cache(dst, size);
	}
}

__declspec(section ".init") static inline void __init_bss_section(void* dst, unsigned long size) {
	if (size) {
		memset(dst, 0, size);
	}
}

#pragma scheduling off
__declspec(section ".init") static void __init_data(void) {
    __rom_copy_info* dci;
    __bss_init_info* bii;

    dci = _rom_copy_info;
    while (TRUE) {
        if (dci->size == 0)
            break;

        __copy_rom_section(dci->addr, dci->rom, dci->size);
        dci++;
    }

    bii = _bss_init_info;
    while (TRUE) {
        if (bii->size == 0)
            break;

        __init_bss_section(bii->addr, bii->size);
        bii++;
    }
}
