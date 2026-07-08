/* Generated target-specific clean C seed-expansion probes for strict proof only. */

typedef unsigned char u8;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;
typedef int s32;
typedef float f32;
typedef void *mp6_ptr;
register u8 *mp6_sda_base __asm__("r13");

__attribute__((section(".text.ser_field_accessor_main_dol_3752_8014ab14"), used, noinline))
u32 ser_field_accessor_main_dol_3752_8014ab14(void)
{
    return *(u32 *)(mp6_sda_base + -27136);
}

__attribute__((section(".text.ser_sda_accessor_store_main_dol_2_80003140"), used, noinline))
void ser_sda_accessor_store_main_dol_2_80003140(void)
{
    *(volatile u8 *)(mp6_sda_base + -28840) = (u8)0x1;
}

__attribute__((section(".text.ser_sda_accessor_store_main_dol_2_80003140__r0_fixed"), used, noinline))
void ser_sda_accessor_store_main_dol_2_80003140__r0_fixed(void)
{
    register u32 value __asm__("r0") = (u32)0x1;
    __asm__ volatile("" : "+r"(value));
    *(volatile u8 *)(mp6_sda_base + -28840) = (u8)value;
}

__attribute__((section(".text.ser_field_accessor_main_dol_464_8002d4e0"), used, noinline))
f32 ser_field_accessor_main_dol_464_8002d4e0(s32 index)
{
    u8 *base = *(u8 * volatile *)(mp6_sda_base + -29976);
    s32 row_offset = (s16)index * 308;
    return *(f32 *)(base + row_offset + 100);
}

__attribute__((section(".text.ser_field_accessor_main_dol_464_8002d4e0__direct_expr"), used, noinline))
f32 ser_field_accessor_main_dol_464_8002d4e0__direct_expr(s32 index)
{
    return *(f32 *)(*(u8 **)(mp6_sda_base + -29976) + ((s16)index * 308) + 100);
}

__attribute__((section(".text.ser_field_accessor_main_dol_464_8002d4e0__r31_frame"), used, noinline))
f32 ser_field_accessor_main_dol_464_8002d4e0__r31_frame(s32 index)
{
    u8 *base = *(u8 * volatile *)(mp6_sda_base + -29976);
    s32 row_offset = (s16)index * 308;
    register u8 *slot __asm__("r31") = base + row_offset;
    return *(f32 *)(slot + 100);
}

__attribute__((section(".text.ser_field_accessor_main_dol_464_8002d4e0__r0_r31_frame"), used, noinline))
f32 ser_field_accessor_main_dol_464_8002d4e0__r0_r31_frame(s32 index)
{
    u8 *base = *(u8 * volatile *)(mp6_sda_base + -29976);
    register s32 idx __asm__("r0") = (s16)index;
    __asm__ volatile("" : "+r"(idx));
    register u8 *slot __asm__("r31") = base + (idx * 308);
    __asm__ volatile("" : "+r"(slot));
    return *(f32 *)(slot + 100);
}

__attribute__((section(".text.ser_sda_accessor_store_main_dol_34_80006fc8"), used, noinline))
void ser_sda_accessor_store_main_dol_34_80006fc8(void)
{
    *(volatile u32 *)(mp6_sda_base + -30404) = (u32)0x1;
}

__attribute__((section(".text.ser_sda_accessor_store_main_dol_34_80006fc8__r0_fixed"), used, noinline))
void ser_sda_accessor_store_main_dol_34_80006fc8__r0_fixed(void)
{
    register u32 value __asm__("r0") = (u32)0x1;
    __asm__ volatile("" : "+r"(value));
    *(volatile u32 *)(mp6_sda_base + -30404) = (u32)value;
}

__attribute__((section(".text.ser_field_accessor_main_dol_462_8002d3ec"), used, noinline))
s16 ser_field_accessor_main_dol_462_8002d3ec(s32 index)
{
    u8 *base = *(u8 * volatile *)(mp6_sda_base + -29976);
    s32 row_offset = (s16)index * 308;
    return *(s16 *)(base + row_offset + 12);
}

__attribute__((section(".text.ser_field_accessor_main_dol_462_8002d3ec__direct_expr"), used, noinline))
s16 ser_field_accessor_main_dol_462_8002d3ec__direct_expr(s32 index)
{
    return *(s16 *)(*(u8 **)(mp6_sda_base + -29976) + ((s16)index * 308) + 12);
}

__attribute__((section(".text.ser_field_accessor_main_dol_462_8002d3ec__r31_frame"), used, noinline))
s16 ser_field_accessor_main_dol_462_8002d3ec__r31_frame(s32 index)
{
    u8 *base = *(u8 * volatile *)(mp6_sda_base + -29976);
    s32 row_offset = (s16)index * 308;
    register u8 *slot __asm__("r31") = base + row_offset;
    return *(s16 *)(slot + 12);
}

__attribute__((section(".text.ser_field_accessor_main_dol_462_8002d3ec__r0_r31_frame"), used, noinline))
s16 ser_field_accessor_main_dol_462_8002d3ec__r0_r31_frame(s32 index)
{
    u8 *base = *(u8 * volatile *)(mp6_sda_base + -29976);
    register s32 idx __asm__("r0") = (s16)index;
    __asm__ volatile("" : "+r"(idx));
    register u8 *slot __asm__("r31") = base + (idx * 308);
    __asm__ volatile("" : "+r"(slot));
    return *(s16 *)(slot + 12);
}

__attribute__((section(".text.ser_sda_accessor_store_main_dol_252_80017b28"), used, noinline))
void ser_sda_accessor_store_main_dol_252_80017b28(void)
{
    *(volatile u16 *)(mp6_sda_base + -30096) = (u16)0x0;
}

__attribute__((section(".text.ser_sda_accessor_store_main_dol_252_80017b28__r0_fixed"), used, noinline))
void ser_sda_accessor_store_main_dol_252_80017b28__r0_fixed(void)
{
    register u32 value __asm__("r0") = (u32)0x0;
    __asm__ volatile("" : "+r"(value));
    *(volatile u16 *)(mp6_sda_base + -30096) = (u16)value;
}

__attribute__((section(".text.ser_field_accessor_main_dol_461_8002d3c4"), used, noinline))
s16 ser_field_accessor_main_dol_461_8002d3c4(s32 index)
{
    u8 *base = *(u8 * volatile *)(mp6_sda_base + -29976);
    s32 row_offset = (s16)index * 308;
    return *(s16 *)(base + row_offset + 8);
}

__attribute__((section(".text.ser_field_accessor_main_dol_461_8002d3c4__direct_expr"), used, noinline))
s16 ser_field_accessor_main_dol_461_8002d3c4__direct_expr(s32 index)
{
    return *(s16 *)(*(u8 **)(mp6_sda_base + -29976) + ((s16)index * 308) + 8);
}

__attribute__((section(".text.ser_field_accessor_main_dol_461_8002d3c4__r31_frame"), used, noinline))
s16 ser_field_accessor_main_dol_461_8002d3c4__r31_frame(s32 index)
{
    u8 *base = *(u8 * volatile *)(mp6_sda_base + -29976);
    s32 row_offset = (s16)index * 308;
    register u8 *slot __asm__("r31") = base + row_offset;
    return *(s16 *)(slot + 8);
}

__attribute__((section(".text.ser_field_accessor_main_dol_461_8002d3c4__r0_r31_frame"), used, noinline))
s16 ser_field_accessor_main_dol_461_8002d3c4__r0_r31_frame(s32 index)
{
    u8 *base = *(u8 * volatile *)(mp6_sda_base + -29976);
    register s32 idx __asm__("r0") = (s16)index;
    __asm__ volatile("" : "+r"(idx));
    register u8 *slot __asm__("r31") = base + (idx * 308);
    __asm__ volatile("" : "+r"(slot));
    return *(s16 *)(slot + 8);
}

__attribute__((section(".text.ser_sda_accessor_store_main_dol_785_800460ac"), used, noinline))
void ser_sda_accessor_store_main_dol_785_800460ac(void)
{
    *(volatile u32 *)(mp6_sda_base + -29652) = (u32)0x1;
}

__attribute__((section(".text.ser_sda_accessor_store_main_dol_785_800460ac__r0_fixed"), used, noinline))
void ser_sda_accessor_store_main_dol_785_800460ac__r0_fixed(void)
{
    register u32 value __asm__("r0") = (u32)0x1;
    __asm__ volatile("" : "+r"(value));
    *(volatile u32 *)(mp6_sda_base + -29652) = (u32)value;
}

__attribute__((section(".text.ser_field_accessor_main_dol_465_8002d508"), used, noinline))
f32 ser_field_accessor_main_dol_465_8002d508(s32 index)
{
    u8 *base = *(u8 * volatile *)(mp6_sda_base + -29976);
    s32 row_offset = (s16)index * 308;
    return *(f32 *)(base + row_offset + 132);
}

__attribute__((section(".text.ser_field_accessor_main_dol_465_8002d508__direct_expr"), used, noinline))
f32 ser_field_accessor_main_dol_465_8002d508__direct_expr(s32 index)
{
    return *(f32 *)(*(u8 **)(mp6_sda_base + -29976) + ((s16)index * 308) + 132);
}

__attribute__((section(".text.ser_field_accessor_main_dol_465_8002d508__r31_frame"), used, noinline))
f32 ser_field_accessor_main_dol_465_8002d508__r31_frame(s32 index)
{
    u8 *base = *(u8 * volatile *)(mp6_sda_base + -29976);
    s32 row_offset = (s16)index * 308;
    register u8 *slot __asm__("r31") = base + row_offset;
    return *(f32 *)(slot + 132);
}

__attribute__((section(".text.ser_field_accessor_main_dol_465_8002d508__r0_r31_frame"), used, noinline))
f32 ser_field_accessor_main_dol_465_8002d508__r0_r31_frame(s32 index)
{
    u8 *base = *(u8 * volatile *)(mp6_sda_base + -29976);
    register s32 idx __asm__("r0") = (s16)index;
    __asm__ volatile("" : "+r"(idx));
    register u8 *slot __asm__("r31") = base + (idx * 308);
    __asm__ volatile("" : "+r"(slot));
    return *(f32 *)(slot + 132);
}

__attribute__((section(".text.ser_sda_accessor_store_main_dol_1109_80068db8"), used, noinline))
void ser_sda_accessor_store_main_dol_1109_80068db8(void)
{
    *(volatile u16 *)(mp6_sda_base + -29404) = (u16)0x1;
}

__attribute__((section(".text.ser_sda_accessor_store_main_dol_1109_80068db8__r0_fixed"), used, noinline))
void ser_sda_accessor_store_main_dol_1109_80068db8__r0_fixed(void)
{
    register u32 value __asm__("r0") = (u32)0x1;
    __asm__ volatile("" : "+r"(value));
    *(volatile u16 *)(mp6_sda_base + -29404) = (u16)value;
}

__attribute__((section(".text.ser_field_accessor_main_dol_334_80026a90"), used, noinline))
u32 ser_field_accessor_main_dol_334_80026a90(s32 index)
{
    u8 *base = *(u8 * volatile *)(mp6_sda_base + -29976);
    s32 row_offset = (s16)index * 308;
    return *(u32 *)(base + row_offset + 80);
}

__attribute__((section(".text.ser_field_accessor_main_dol_334_80026a90__direct_expr"), used, noinline))
u32 ser_field_accessor_main_dol_334_80026a90__direct_expr(s32 index)
{
    return *(u32 *)(*(u8 **)(mp6_sda_base + -29976) + ((s16)index * 308) + 80);
}

__attribute__((section(".text.ser_field_accessor_main_dol_334_80026a90__r31_frame"), used, noinline))
u32 ser_field_accessor_main_dol_334_80026a90__r31_frame(s32 index)
{
    u8 *base = *(u8 * volatile *)(mp6_sda_base + -29976);
    s32 row_offset = (s16)index * 308;
    register u8 *slot __asm__("r31") = base + row_offset;
    return *(u32 *)(slot + 80);
}

__attribute__((section(".text.ser_field_accessor_main_dol_334_80026a90__r0_r31_frame"), used, noinline))
u32 ser_field_accessor_main_dol_334_80026a90__r0_r31_frame(s32 index)
{
    u8 *base = *(u8 * volatile *)(mp6_sda_base + -29976);
    register s32 idx __asm__("r0") = (s16)index;
    __asm__ volatile("" : "+r"(idx));
    register u8 *slot __asm__("r31") = base + (idx * 308);
    __asm__ volatile("" : "+r"(slot));
    return *(u32 *)(slot + 80);
}

__attribute__((section(".text.ser_sda_accessor_store_main_dol_1521_8009ad80"), used, noinline))
void ser_sda_accessor_store_main_dol_1521_8009ad80(void)
{
    *(volatile u32 *)(mp6_sda_base + -28960) = (u32)0x1;
}

__attribute__((section(".text.ser_sda_accessor_store_main_dol_1521_8009ad80__r0_fixed"), used, noinline))
void ser_sda_accessor_store_main_dol_1521_8009ad80__r0_fixed(void)
{
    register u32 value __asm__("r0") = (u32)0x1;
    __asm__ volatile("" : "+r"(value));
    *(volatile u32 *)(mp6_sda_base + -28960) = (u32)value;
}

__attribute__((section(".text.ser_field_accessor_main_dol_4179_8016a080"), used, noinline))
s16 ser_field_accessor_main_dol_4179_8016a080(s32 index)
{
    u8 *base = *(u8 * volatile *)(mp6_sda_base + -26992);
    s32 row_offset = (s16)index * 412;
    return *(s16 *)(base + row_offset + 76);
}

__attribute__((section(".text.ser_field_accessor_main_dol_4179_8016a080__direct_expr"), used, noinline))
s16 ser_field_accessor_main_dol_4179_8016a080__direct_expr(s32 index)
{
    return *(s16 *)(*(u8 **)(mp6_sda_base + -26992) + ((s16)index * 412) + 76);
}

__attribute__((section(".text.ser_field_accessor_main_dol_4179_8016a080__r31_frame"), used, noinline))
s16 ser_field_accessor_main_dol_4179_8016a080__r31_frame(s32 index)
{
    u8 *base = *(u8 * volatile *)(mp6_sda_base + -26992);
    s32 row_offset = (s16)index * 412;
    register u8 *slot __asm__("r31") = base + row_offset;
    return *(s16 *)(slot + 76);
}

__attribute__((section(".text.ser_field_accessor_main_dol_4179_8016a080__r0_r31_frame"), used, noinline))
s16 ser_field_accessor_main_dol_4179_8016a080__r0_r31_frame(s32 index)
{
    u8 *base = *(u8 * volatile *)(mp6_sda_base + -26992);
    register s32 idx __asm__("r0") = (s16)index;
    __asm__ volatile("" : "+r"(idx));
    register u8 *slot __asm__("r31") = base + (idx * 412);
    __asm__ volatile("" : "+r"(slot));
    return *(s16 *)(slot + 76);
}

__attribute__((section(".text.ser_sda_accessor_store_main_dol_1590_8009e0b0"), used, noinline))
u32 ser_sda_accessor_store_main_dol_1590_8009e0b0(void)
{
    return *(volatile u32 *)0x8028776C;
}

__attribute__((section(".text.ser_field_accessor_main_dol_5452_801ffb04"), used, noinline))
f32 ser_field_accessor_main_dol_5452_801ffb04(s32 index)
{
    u8 *base = *(u8 * volatile *)(mp6_sda_base + -26292);
    s32 row_offset = (s16)index * 92;
    return *(f32 *)(base + row_offset + 60);
}

__attribute__((section(".text.ser_field_accessor_main_dol_5452_801ffb04__direct_expr"), used, noinline))
f32 ser_field_accessor_main_dol_5452_801ffb04__direct_expr(s32 index)
{
    return *(f32 *)(*(u8 **)(mp6_sda_base + -26292) + ((s16)index * 92) + 60);
}

__attribute__((section(".text.ser_field_accessor_main_dol_5452_801ffb04__r31_frame"), used, noinline))
f32 ser_field_accessor_main_dol_5452_801ffb04__r31_frame(s32 index)
{
    u8 *base = *(u8 * volatile *)(mp6_sda_base + -26292);
    s32 row_offset = (s16)index * 92;
    register u8 *slot __asm__("r31") = base + row_offset;
    return *(f32 *)(slot + 60);
}

__attribute__((section(".text.ser_field_accessor_main_dol_5452_801ffb04__r0_r31_frame"), used, noinline))
f32 ser_field_accessor_main_dol_5452_801ffb04__r0_r31_frame(s32 index)
{
    u8 *base = *(u8 * volatile *)(mp6_sda_base + -26292);
    register s32 idx __asm__("r0") = (s16)index;
    __asm__ volatile("" : "+r"(idx));
    register u8 *slot __asm__("r31") = base + (idx * 92);
    __asm__ volatile("" : "+r"(slot));
    return *(f32 *)(slot + 60);
}

__attribute__((section(".text.ser_sda_accessor_store_main_dol_2553_800ea238"), used, noinline))
void ser_sda_accessor_store_main_dol_2553_800ea238(void)
{
    *(volatile u32 *)(mp6_sda_base + -27424) = (u32)0x0;
}

__attribute__((section(".text.ser_sda_accessor_store_main_dol_2553_800ea238__r0_fixed"), used, noinline))
void ser_sda_accessor_store_main_dol_2553_800ea238__r0_fixed(void)
{
    register u32 value __asm__("r0") = (u32)0x0;
    __asm__ volatile("" : "+r"(value));
    *(volatile u32 *)(mp6_sda_base + -27424) = (u32)value;
}

__attribute__((section(".text.ser_sda_accessor_store_main_dol_2554_800ea244"), used, noinline))
void ser_sda_accessor_store_main_dol_2554_800ea244(void)
{
    *(volatile u32 *)(mp6_sda_base + -27428) = (u32)0x1;
}

__attribute__((section(".text.ser_sda_accessor_store_main_dol_2554_800ea244__r0_fixed"), used, noinline))
void ser_sda_accessor_store_main_dol_2554_800ea244__r0_fixed(void)
{
    register u32 value __asm__("r0") = (u32)0x1;
    __asm__ volatile("" : "+r"(value));
    *(volatile u32 *)(mp6_sda_base + -27428) = (u32)value;
}

__attribute__((section(".text.ser_sda_accessor_store_main_dol_2857_800fcc18"), used, noinline))
u32 ser_sda_accessor_store_main_dol_2857_800fcc18(void)
{
    return *(volatile u32 *)0x802B36A0;
}

__attribute__((section(".text.ser_sda_accessor_store_main_dol_2873_800fda60"), used, noinline))
void ser_sda_accessor_store_main_dol_2873_800fda60(u32 value)
{
    *(volatile u32 *)0x802B374C = value;
}

__attribute__((section(".text.ser_sda_accessor_store_main_dol_2876_800fdca4"), used, noinline))
void ser_sda_accessor_store_main_dol_2876_800fdca4(u32 value)
{
    *(volatile u32 *)0x802B3744 = value;
}

__attribute__((section(".text.ser_sda_accessor_store_main_dol_2877_800fdcb4"), used, noinline))
u32 ser_sda_accessor_store_main_dol_2877_800fdcb4(void)
{
    return *(volatile u32 *)0x802B3744;
}

__attribute__((section(".text.ser_sda_accessor_store_main_dol_2880_800fdf08"), used, noinline))
u32 ser_sda_accessor_store_main_dol_2880_800fdf08(void)
{
    return *(volatile u32 *)0x802B37D0;
}

__attribute__((section(".text.ser_sda_accessor_store_main_dol_2918_800ff95c"), used, noinline))
u8 ser_sda_accessor_store_main_dol_2918_800ff95c(void)
{
    return *(volatile u8 *)0x802B5E60;
}

__attribute__((section(".text.ser_sda_accessor_store_main_dol_3750_8014aafc"), used, noinline))
void ser_sda_accessor_store_main_dol_3750_8014aafc(void)
{
    *(volatile u32 *)(mp6_sda_base + -27144) = (u32)0x0;
}

__attribute__((section(".text.ser_sda_accessor_store_main_dol_3750_8014aafc__r0_fixed"), used, noinline))
void ser_sda_accessor_store_main_dol_3750_8014aafc__r0_fixed(void)
{
    register u32 value __asm__("r0") = (u32)0x0;
    __asm__ volatile("" : "+r"(value));
    *(volatile u32 *)(mp6_sda_base + -27144) = (u32)value;
}

__attribute__((section(".text.ser_sda_accessor_store_main_dol_3751_8014ab08"), used, noinline))
void ser_sda_accessor_store_main_dol_3751_8014ab08(void)
{
    *(volatile u32 *)(mp6_sda_base + -27140) = (u32)0x1;
}

__attribute__((section(".text.ser_sda_accessor_store_main_dol_3751_8014ab08__r0_fixed"), used, noinline))
void ser_sda_accessor_store_main_dol_3751_8014ab08__r0_fixed(void)
{
    register u32 value __asm__("r0") = (u32)0x1;
    __asm__ volatile("" : "+r"(value));
    *(volatile u32 *)(mp6_sda_base + -27140) = (u32)value;
}

__attribute__((section(".text.ser_sda_accessor_store_main_dol_4517_8017f1d4"), used, noinline))
void ser_sda_accessor_store_main_dol_4517_8017f1d4(void)
{
    *(volatile u32 *)(mp6_sda_base + -26712) = (u32)0x0;
}

__attribute__((section(".text.ser_sda_accessor_store_main_dol_4517_8017f1d4__r0_fixed"), used, noinline))
void ser_sda_accessor_store_main_dol_4517_8017f1d4__r0_fixed(void)
{
    register u32 value __asm__("r0") = (u32)0x0;
    __asm__ volatile("" : "+r"(value));
    *(volatile u32 *)(mp6_sda_base + -26712) = (u32)value;
}

__attribute__((section(".text.ser_sda_accessor_store_main_dol_4542_8017fed8"), used, noinline))
void ser_sda_accessor_store_main_dol_4542_8017fed8(void)
{
    *(volatile u32 *)(mp6_sda_base + -26696) = (u32)0x0;
}

__attribute__((section(".text.ser_sda_accessor_store_main_dol_4542_8017fed8__r0_fixed"), used, noinline))
void ser_sda_accessor_store_main_dol_4542_8017fed8__r0_fixed(void)
{
    register u32 value __asm__("r0") = (u32)0x0;
    __asm__ volatile("" : "+r"(value));
    *(volatile u32 *)(mp6_sda_base + -26696) = (u32)value;
}

__attribute__((section(".text.ser_sda_accessor_store_main_dol_4543_8017fee4"), used, noinline))
void ser_sda_accessor_store_main_dol_4543_8017fee4(void)
{
    *(volatile u32 *)(mp6_sda_base + -26700) = (u32)0x0;
}

__attribute__((section(".text.ser_sda_accessor_store_main_dol_4543_8017fee4__r0_fixed"), used, noinline))
void ser_sda_accessor_store_main_dol_4543_8017fee4__r0_fixed(void)
{
    register u32 value __asm__("r0") = (u32)0x0;
    __asm__ volatile("" : "+r"(value));
    *(volatile u32 *)(mp6_sda_base + -26700) = (u32)value;
}

__attribute__((section(".text.ser_sda_accessor_store_main_dol_4667_8018b984"), used, noinline))
void ser_sda_accessor_store_main_dol_4667_8018b984(void)
{
    *(volatile u32 *)(mp6_sda_base + -26676) = (u32)0x0;
}

__attribute__((section(".text.ser_sda_accessor_store_main_dol_4667_8018b984__r0_fixed"), used, noinline))
void ser_sda_accessor_store_main_dol_4667_8018b984__r0_fixed(void)
{
    register u32 value __asm__("r0") = (u32)0x0;
    __asm__ volatile("" : "+r"(value));
    *(volatile u32 *)(mp6_sda_base + -26676) = (u32)value;
}

__attribute__((section(".text.ser_sda_accessor_store_main_dol_4718_801914dc"), used, noinline))
void ser_sda_accessor_store_main_dol_4718_801914dc(void)
{
    *(volatile u32 *)(mp6_sda_base + -26668) = (u32)0x0;
}

__attribute__((section(".text.ser_sda_accessor_store_main_dol_4718_801914dc__r0_fixed"), used, noinline))
void ser_sda_accessor_store_main_dol_4718_801914dc__r0_fixed(void)
{
    register u32 value __asm__("r0") = (u32)0x0;
    __asm__ volatile("" : "+r"(value));
    *(volatile u32 *)(mp6_sda_base + -26668) = (u32)value;
}

__attribute__((section(".text.ser_sda_accessor_store_main_dol_4760_801945e8"), used, noinline))
void ser_sda_accessor_store_main_dol_4760_801945e8(u32 value)
{
    *(volatile u32 *)0x802BD048 = value;
}

__attribute__((section(".text.ser_sda_accessor_store_main_dol_4901_801b8180"), used, noinline))
void ser_sda_accessor_store_main_dol_4901_801b8180(void)
{
    *(volatile u32 *)(mp6_sda_base + -26520) = (u32)0x0;
}

__attribute__((section(".text.ser_sda_accessor_store_main_dol_4901_801b8180__r0_fixed"), used, noinline))
void ser_sda_accessor_store_main_dol_4901_801b8180__r0_fixed(void)
{
    register u32 value __asm__("r0") = (u32)0x0;
    __asm__ volatile("" : "+r"(value));
    *(volatile u32 *)(mp6_sda_base + -26520) = (u32)value;
}

__attribute__((section(".text.ser_sda_accessor_store_main_dol_4907_801b9388"), used, noinline))
void ser_sda_accessor_store_main_dol_4907_801b9388(void)
{
    *(volatile u32 *)(mp6_sda_base + -26516) = (u32)0x0;
}

__attribute__((section(".text.ser_sda_accessor_store_main_dol_4907_801b9388__r0_fixed"), used, noinline))
void ser_sda_accessor_store_main_dol_4907_801b9388__r0_fixed(void)
{
    register u32 value __asm__("r0") = (u32)0x0;
    __asm__ volatile("" : "+r"(value));
    *(volatile u32 *)(mp6_sda_base + -26516) = (u32)value;
}

__attribute__((section(".text.ser_sda_accessor_store_main_dol_5047_801ce558"), used, noinline))
void ser_sda_accessor_store_main_dol_5047_801ce558(void)
{
    *(volatile u32 *)(mp6_sda_base + -26488) = (u32)0x0;
}

__attribute__((section(".text.ser_sda_accessor_store_main_dol_5047_801ce558__r0_fixed"), used, noinline))
void ser_sda_accessor_store_main_dol_5047_801ce558__r0_fixed(void)
{
    register u32 value __asm__("r0") = (u32)0x0;
    __asm__ volatile("" : "+r"(value));
    *(volatile u32 *)(mp6_sda_base + -26488) = (u32)value;
}

__attribute__((section(".text.ser_sda_accessor_store_main_dol_5052_801cea94"), used, noinline))
void ser_sda_accessor_store_main_dol_5052_801cea94(void)
{
    *(volatile u32 *)(mp6_sda_base + -26484) = (u32)0x0;
}

__attribute__((section(".text.ser_sda_accessor_store_main_dol_5052_801cea94__r0_fixed"), used, noinline))
void ser_sda_accessor_store_main_dol_5052_801cea94__r0_fixed(void)
{
    register u32 value __asm__("r0") = (u32)0x0;
    __asm__ volatile("" : "+r"(value));
    *(volatile u32 *)(mp6_sda_base + -26484) = (u32)value;
}

__attribute__((section(".text.ser_sda_accessor_store_main_dol_5057_801cee3c"), used, noinline))
void ser_sda_accessor_store_main_dol_5057_801cee3c(void)
{
    *(volatile u32 *)(mp6_sda_base + -26476) = (u32)0x0;
}

__attribute__((section(".text.ser_sda_accessor_store_main_dol_5057_801cee3c__r0_fixed"), used, noinline))
void ser_sda_accessor_store_main_dol_5057_801cee3c__r0_fixed(void)
{
    register u32 value __asm__("r0") = (u32)0x0;
    __asm__ volatile("" : "+r"(value));
    *(volatile u32 *)(mp6_sda_base + -26476) = (u32)value;
}

__attribute__((section(".text.ser_sda_accessor_store_main_dol_5061_801cf6a8"), used, noinline))
void ser_sda_accessor_store_main_dol_5061_801cf6a8(void)
{
    *(volatile u32 *)(mp6_sda_base + -26472) = (u32)0x0;
}

__attribute__((section(".text.ser_sda_accessor_store_main_dol_5061_801cf6a8__r0_fixed"), used, noinline))
void ser_sda_accessor_store_main_dol_5061_801cf6a8__r0_fixed(void)
{
    register u32 value __asm__("r0") = (u32)0x0;
    __asm__ volatile("" : "+r"(value));
    *(volatile u32 *)(mp6_sda_base + -26472) = (u32)value;
}

__attribute__((section(".text.ser_sda_accessor_store_main_dol_5064_801cfbcc"), used, noinline))
void ser_sda_accessor_store_main_dol_5064_801cfbcc(void)
{
    *(volatile u32 *)(mp6_sda_base + -26468) = (u32)0x0;
}

__attribute__((section(".text.ser_sda_accessor_store_main_dol_5064_801cfbcc__r0_fixed"), used, noinline))
void ser_sda_accessor_store_main_dol_5064_801cfbcc__r0_fixed(void)
{
    register u32 value __asm__("r0") = (u32)0x0;
    __asm__ volatile("" : "+r"(value));
    *(volatile u32 *)(mp6_sda_base + -26468) = (u32)value;
}

__attribute__((section(".text.ser_sda_accessor_store_main_dol_5068_801d0cc0"), used, noinline))
void ser_sda_accessor_store_main_dol_5068_801d0cc0(void)
{
    *(volatile u32 *)(mp6_sda_base + -26464) = (u32)0x0;
}

__attribute__((section(".text.ser_sda_accessor_store_main_dol_5068_801d0cc0__r0_fixed"), used, noinline))
void ser_sda_accessor_store_main_dol_5068_801d0cc0__r0_fixed(void)
{
    register u32 value __asm__("r0") = (u32)0x0;
    __asm__ volatile("" : "+r"(value));
    *(volatile u32 *)(mp6_sda_base + -26464) = (u32)value;
}

__attribute__((section(".text.ser_sda_accessor_store_main_dol_5073_801d1bac"), used, noinline))
void ser_sda_accessor_store_main_dol_5073_801d1bac(void)
{
    *(volatile u32 *)(mp6_sda_base + -26460) = (u32)0x0;
}

__attribute__((section(".text.ser_sda_accessor_store_main_dol_5073_801d1bac__r0_fixed"), used, noinline))
void ser_sda_accessor_store_main_dol_5073_801d1bac__r0_fixed(void)
{
    register u32 value __asm__("r0") = (u32)0x0;
    __asm__ volatile("" : "+r"(value));
    *(volatile u32 *)(mp6_sda_base + -26460) = (u32)value;
}

__attribute__((section(".text.ser_sda_accessor_store_main_dol_5086_801d424c"), used, noinline))
void ser_sda_accessor_store_main_dol_5086_801d424c(void)
{
    *(volatile u32 *)(mp6_sda_base + -31084) = (u32)-1;
}

__attribute__((section(".text.ser_sda_accessor_store_main_dol_5086_801d424c__r0_fixed"), used, noinline))
void ser_sda_accessor_store_main_dol_5086_801d424c__r0_fixed(void)
{
    register u32 value __asm__("r0") = (u32)-1;
    __asm__ volatile("" : "+r"(value));
    *(volatile u32 *)(mp6_sda_base + -31084) = (u32)value;
}

__attribute__((section(".text.ser_sda_accessor_store_main_dol_5343_801ef884"), used, noinline))
void ser_sda_accessor_store_main_dol_5343_801ef884(void)
{
    *(volatile u32 *)(mp6_sda_base + -26348) = (u32)0x0;
}

__attribute__((section(".text.ser_sda_accessor_store_main_dol_5343_801ef884__r0_fixed"), used, noinline))
void ser_sda_accessor_store_main_dol_5343_801ef884__r0_fixed(void)
{
    register u32 value __asm__("r0") = (u32)0x0;
    __asm__ volatile("" : "+r"(value));
    *(volatile u32 *)(mp6_sda_base + -26348) = (u32)value;
}

__attribute__((section(".text.ser_sda_accessor_store_main_dol_5378_801f6c2c"), used, noinline))
void ser_sda_accessor_store_main_dol_5378_801f6c2c(void)
{
    *(volatile u32 *)(mp6_sda_base + -26332) = (u32)0x0;
}

__attribute__((section(".text.ser_sda_accessor_store_main_dol_5378_801f6c2c__r0_fixed"), used, noinline))
void ser_sda_accessor_store_main_dol_5378_801f6c2c__r0_fixed(void)
{
    register u32 value __asm__("r0") = (u32)0x0;
    __asm__ volatile("" : "+r"(value));
    *(volatile u32 *)(mp6_sda_base + -26332) = (u32)value;
}

__attribute__((section(".text.ser_sda_accessor_store_main_dol_5443_801fed9c"), used, noinline))
void ser_sda_accessor_store_main_dol_5443_801fed9c(void)
{
    *(volatile u32 *)(mp6_sda_base + -26288) = (u32)0x1;
}

__attribute__((section(".text.ser_sda_accessor_store_main_dol_5443_801fed9c__r0_fixed"), used, noinline))
void ser_sda_accessor_store_main_dol_5443_801fed9c__r0_fixed(void)
{
    register u32 value __asm__("r0") = (u32)0x1;
    __asm__ volatile("" : "+r"(value));
    *(volatile u32 *)(mp6_sda_base + -26288) = (u32)value;
}
