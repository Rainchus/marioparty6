#include <dolphin/PPCArch.h>
#include <dolphin/hw_regs.h>
#include <dolphin/os.h>
#include <dolphin/os/OSBootInfo.h>
#include <dolphin/os/__os.h>
#include <dolphin/pad.h>
#include <string.h>

extern OSThreadQueue __OSActiveThreadQueue;
extern OSExecParams __OSRebootParams;
extern void __OSStopAudioSystem(void);
static OSResetFunctionQueue ResetFunctionQueue;
static u32 bootThisDol;

void OSRegisterResetFunction(OSResetFunctionInfo* info)
{
    OSResetFunctionInfo* prev;
    OSResetFunctionInfo* next;

    for (next = ResetFunctionQueue.head;
         next != NULL && next->priority <= info->priority;
         next = next->next) {
    }

    if (next == NULL) {
        prev = ResetFunctionQueue.tail;
        if (prev == NULL) {
            ResetFunctionQueue.head = info;
        } else {
            prev->next = info;
        }
        info->prev = prev;
        info->next = NULL;
        ResetFunctionQueue.tail = info;
    } else {
        info->next = next;
        prev = next->prev;
        next->prev = info;
        info->prev = prev;
        if (prev == NULL) {
            ResetFunctionQueue.head = info;
        } else {
            prev->next = info;
        }
    }
}

BOOL __OSCallResetFunctions(BOOL final)
{
    OSResetFunctionInfo* info;
    BOOL error;
    u32 priority;

    priority = 0;
    error = FALSE;
    for (info = ResetFunctionQueue.head; info != NULL; info = info->next) {
        if (error && priority != info->priority) {
            break;
        }
        error |= !info->func(final);
        priority = info->priority;
    }

    error |= !__OSSyncSram();
    if (error) {
        return FALSE;
    }
    return TRUE;
}

static void Reset(u32 resetCode)
{
    OSTick start;

    PPCMthid0(PPCMfhid0() | HID0_ABE);
    PPCSync();
    start = OSGetTick();
    while (OSGetTick() - start < 0x1124) {
    }
    __PIRegs[PI_RESETCODE] = 3;
    __PIRegs[PI_RESETCODE] = resetCode;
    PPCHalt();
}

static void KillThreads(void)
{
    OSThread* thread;
    OSThread* next;

    for (thread = __OSActiveThreadQueue.head; thread != NULL; thread = next) {
        next = thread->linkActive.next;
        switch (thread->state) {
        case OS_THREAD_STATE_READY:
        case OS_THREAD_STATE_WAITING:
            OSCancelThread(thread);
            continue;
        default:
            continue;
        }
    }
}

void __OSDoHotReset(u32 resetCode)
{
    OSDisableInterrupts();
    __VIRegs[1] = 0;
    ICFlashInvalidate();
    Reset(resetCode * 8);
}

static void __OSShutdownDevices(BOOL doRecalibration)
{
    BOOL disableRecalibration;

    __OSStopAudioSystem();
    if (!doRecalibration) {
        disableRecalibration = __PADDisableRecalibration(TRUE);
    }

    while (!__OSCallResetFunctions(FALSE)) {
    }
    while (!__OSSyncSram()) {
    }

    OSDisableInterrupts();
    __OSCallResetFunctions(TRUE);
    LCDisable();
    if (!doRecalibration) {
        __PADDisableRecalibration(disableRecalibration);
    }
    KillThreads();
}

void OSResetSystem(s32 reset, u32 resetCode, BOOL forceMenu)
{
    OSSram* sram;

    OSDisableScheduler();

    if (reset == OS_RESET_HOTRESET && forceMenu) {
        sram = __OSLockSram();
        sram->flags |= 0x40;
        __OSUnlockSram(TRUE);
        resetCode = 0;
    }

    if (reset == OS_RESET_SHUTDOWN ||
        (reset == OS_RESET_RESTART &&
         (bootThisDol != 0 || resetCode + 0x3FFF0000 == 0))) {
        __OSShutdownDevices(FALSE);
    } else {
        __OSShutdownDevices(TRUE);
    }

    if (reset == OS_RESET_HOTRESET) {
        __OSDoHotReset(resetCode);
    } else if (reset == OS_RESET_RESTART) {
        if (forceMenu == TRUE) {
            OSReport("OSResetSystem(): You can't specify TRUE to forceMenu if you restart. Ignored\n");
        }
        OSEnableScheduler();
        __OSReboot(resetCode, bootThisDol);
    }

    memset(OSPhysicalToCached(0x40), 0, 0xCC - 0x40);
    memset(OSPhysicalToCached(0xD4), 0, 0xE8 - 0xD4);
    memset(OSPhysicalToCached(0xF4), 0, 0xF8 - 0xF4);
    memset(OSPhysicalToCached(0x3000), 0, 0xC0);
    memset(OSPhysicalToCached(0x30C8), 0, 0xD4 - 0xC8);
    memset(OSPhysicalToCached(0x30E2), 0, 1);
}

u32 OSGetResetCode(void)
{
    u32 resetCode;

    if (__OSRebootParams.valid) {
        resetCode = OS_RESETCODE_RESTART | __OSRebootParams.restartCode;
    } else {
        resetCode = (__PIRegs[PI_RESETCODE] & ~7) / 8;
    }
    return resetCode;
}
