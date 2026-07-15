#include <dolphin/PPCArch.h>
#include <dolphin/hw_regs.h>
#include <dolphin/os.h>
#include <dolphin/os/OSBootInfo.h>
#include <dolphin/os/__os.h>
#include <dolphin/pad.h>
#include <string.h>

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

static asm void Reset(u32 resetCode)
{
    nofralloc
    b L_000001BC
L_000001A0:
    mfspr r8, HID0
    ori r8, r8, 0x8
    mtspr HID0, r8
    isync
    sync
    nop
    b L_000001C0
L_000001BC:
    b L_000001DC
L_000001C0:
    mftb r5, 268
L_000001C4:
    mftb r6, 268
    subf r7, r5, r6
    cmplwi r7, 0x1124
    blt L_000001C4
    nop
    b L_000001E0
L_000001DC:
    b L_000001FC
L_000001E0:
    lis r8, 0xcc00
    ori r8, r8, 0x3000
    li r4, 0x3
    stw r4, 0x24(r8)
    stw r3, 0x24(r8)
    nop
    b L_00000200
L_000001FC:
    b L_00000208
L_00000200:
    nop
    b L_00000200
L_00000208:
    b L_000001A0
}

static void KillThreads(void)
{
    OSThread* thread;
    OSThread* next;

    for (thread = __OSActiveThreadQueue.head; thread; thread = next) {
        next = thread->linkActive.next;
        switch (thread->state) {
        case 1:
        case 4:
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

void __OSShutdownDevices(BOOL doRecalibration)
{
    int rc;
    BOOL disableRecalibration;

    __OSStopAudioSystem();
    if (!doRecalibration) {
        disableRecalibration = __PADDisableRecalibration(TRUE);
    }

    do {
    } while (!__OSCallResetFunctions(FALSE));
    do {
    } while (!__OSSyncSram());

    OSDisableInterrupts();
    rc = __OSCallResetFunctions(TRUE);
    ASSERTLINE(408, rc);
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
        __OSUnlockSram(1);
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

    if (__OSRebootParams.valid)
        resetCode = 0x80000000 | __OSRebootParams.restartCode;
    else
        resetCode = (__PIRegs[9] & 0xFFFFFFF8) / 8;

    return resetCode;
}
