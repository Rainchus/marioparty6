#include <dolphin/os.h>
#include <dolphin/hw_regs.h>

#define TRUNC(n, a) (((u32)(n)) & ~((a)-1))
#define ROUND(n, a) (((u32)(n) + (a)-1) & ~((a)-1))

static BOOL OnReset(BOOL final);
void Config24MB(void);
void Config48MB(void);
void RealMode(u32 addr);

static OSResetFunctionInfo ResetFunctionInfo = {
    OnReset,
    127,
};

u32 OSGetPhysicalMemSize()
{
    return *(u32 *)(OSPhysicalToCached(0x0028));
}

u32 OSGetConsoleSimulatedMemSize()
{
    return *(u32 *)(OSPhysicalToCached(0x00F0));
}

static BOOL OnReset(BOOL final)
{
    if (final != FALSE) {
        __MEMRegs[8] = 0xFF;
        __OSMaskInterrupts(0xf0000000);
    }
    return TRUE;
}

static void MEMIntrruptHandler(__OSInterrupt interrupt, OSContext *context)
{
    u32 addr;
    u32 cause;

    cause = __MEMRegs[0xf];
    addr = (((u32)__MEMRegs[0x12] & 0x3ff) << 16) | __MEMRegs[0x11];
    __MEMRegs[0x10] = 0;

    if (__OSErrorTable[OS_ERROR_PROTECTION]) {
        __OSErrorTable[OS_ERROR_PROTECTION](OS_ERROR_PROTECTION, context, cause, addr);
        return;
    }

    __OSUnhandledException(OS_ERROR_PROTECTION, context, cause, addr);
}

void __OSInitMemoryProtection()
{
    u32 simulatedSize;
    BOOL enabled;

    simulatedSize = OSGetConsoleSimulatedMemSize();
    enabled = OSDisableInterrupts();
    __MEMRegs[16] = 0;
    __MEMRegs[8] = 0xFF;

    __OSMaskInterrupts(OS_INTERRUPTMASK_MEM_0 | OS_INTERRUPTMASK_MEM_1 | OS_INTERRUPTMASK_MEM_2 | OS_INTERRUPTMASK_MEM_3);
    __OSSetInterruptHandler(__OS_INTERRUPT_MEM_0, MEMIntrruptHandler);
    __OSSetInterruptHandler(__OS_INTERRUPT_MEM_1, MEMIntrruptHandler);
    __OSSetInterruptHandler(__OS_INTERRUPT_MEM_2, MEMIntrruptHandler);
    __OSSetInterruptHandler(__OS_INTERRUPT_MEM_3, MEMIntrruptHandler);
    __OSSetInterruptHandler(__OS_INTERRUPT_MEM_ADDRESS, MEMIntrruptHandler);
    OSRegisterResetFunction(&ResetFunctionInfo);

    if (OSGetConsoleSimulatedMemSize() < OSGetPhysicalMemSize() && OSGetConsoleSimulatedMemSize() == 0x1800000) {
        DCInvalidateRange((void *)0x81800000, 0x1800000);
        __MEMRegs[20] = 2;
    }

    if (simulatedSize <= 0x1800000) {
        RealMode((u32)&Config24MB);
    } else if (simulatedSize <= 0x3000000) {
        RealMode((u32)&Config48MB);
    }

    __OSUnmaskInterrupts(OS_INTERRUPTMASK_MEM_ADDRESS);
    OSRestoreInterrupts(enabled);
}
