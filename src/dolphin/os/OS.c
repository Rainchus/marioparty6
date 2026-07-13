#include <dolphin.h>
#include <dolphin/DVDPriv.h>
#include <dolphin/exi.h>
#include <dolphin/os.h>
#include <dolphin/os/OSBootInfo.h>
#include <dolphin/os/OSPriv.h>
#include <dolphin/sipriv.h>
#include <dolphin/db.h>

#include <dolphin/os/__os.h>

/*
 * Clean-C SDK subset. The FPR, debugger-vector, exception-vector, default
 * exception, and paired-single routines remain owned by the target fallback.
 */

#define NOP 0x60000000

// external functions
extern void EnableMetroTRKInterrupts(void);
extern void __OSInitMemoryProtection(void);
extern void __OSInitSystemCall(void);
extern void __OSModuleInit(void);
extern void __OSInterruptInit(void);
extern void __OSResetSWInterruptHandler(__OSInterrupt interrupt, OSContext* context);
extern void __OSContextInit(void);
extern void __OSCacheInit(void);
extern void __OSInitAudioSystem(void);
extern BOOL __DBIsExceptionMarked(__OSException exception);

#define DB_EXCEPTIONRET_OFFSET 0xC
#define DB_EXCEPTIONDEST_OFFSET 0x8
#define OS_CURRENTCONTEXT_PADDR 0x00C0
#define OS_EXCEPTIONTABLE_ADDR 0x3000
#define OS_DBJUMPPOINT_ADDR 0x60

#if SDK_REVISION < 1
#define BUILD_DATE  "Apr  5 2004"
#define DBUILD_TIME "03:55:13"
#define RBUILD_TIME "04:13:58"
#elif SDK_REVISION < 2
#define BUILD_DATE  "May 21 2004"
#define DBUILD_TIME "09:15:32"
#define RBUILD_TIME "09:28:09"
#else
#define BUILD_DATE  "Nov 10 2004"
#define DBUILD_TIME "06:08:19"
#define RBUILD_TIME "06:26:41"
#endif

#ifdef DEBUG
const char* __OSVersion = "<< Dolphin SDK - OS\tdebug build: "BUILD_DATE" "DBUILD_TIME" (0x2301) >>";
#else
const char* __OSVersion = "<< Dolphin SDK - OS\trelease build: "BUILD_DATE" "RBUILD_TIME" (0x2301) >>";
#endif

static DVDDriveInfo DriveInfo ATTRIBUTE_ALIGN(32);
static DVDCommandBlock DriveBlock;
OSExecParams __OSRebootParams;

extern u32 __DVDLongFileNameFlag;
extern u32 __PADSpec;

// defined in link script
extern u8 __ArenaLo[];
extern char _stack_addr[];
extern u8 __ArenaHi[];

static OSBootInfo* BootInfo;
static u32* BI2DebugFlag;
static u32 BI2DebugFlagHolder;

OSTime __OSStartTime;
BOOL __OSInIPL;
void (**OSExceptionTable)(u8, OSContext*);
BOOL AreWeInitialized;
f32 ZeroPS[2];
f64 ZeroF;
__declspec(weak) BOOL __OSIsGcam;

// prototypes
static void OSExceptionInit(void);
__OSExceptionHandler __OSSetExceptionHandler(__OSException exception, __OSExceptionHandler handler);

// dummy entry points to the OS Exception vector
void __OSEVStart(void);
void __OSEVEnd(void);
void __OSEVSetNumber(void);
void __DBVECTOR(void);
void __OSDBINTSTART(void);
void __OSDBINTEND(void);
void __OSDBJUMPSTART(void);
void __OSDBJUMPEND(void);
extern void OSDefaultExceptionHandler(__OSException exception, OSContext* context);

static void DisableWriteGatherPipe(void) {
    u32 hid2;

    hid2 = PPCMfhid2();
    hid2 &= ~0x40000000;
    PPCMthid2(hid2);
}

u32 OSGetConsoleType(void) {
    if (!BootInfo || BootInfo->consoleType == 0) {
        return OS_CONSOLE_ARTHUR;
    }
    return BootInfo->consoleType;
}

// needed for assert
#undef NULL
#define NULL 0

static void ClearArena(void) {
    if (!((OSGetResetCode() & 0x80000000) ? TRUE : FALSE)) {
        memset(OSGetArenaLo(), 0, (u32)OSGetArenaHi() - (u32)OSGetArenaLo());
        return;
    }

    if (*(u32*)&__OSRebootParams.regionStart == 0) {
        memset(OSGetArenaLo(), 0, (u32)OSGetArenaHi() - (u32)OSGetArenaLo());
        return;
    }

    ASSERTLINE(683, __OSRebootParams.regionEnd != NULL);

    if ((u32)OSGetArenaLo() < *(u32*)&__OSRebootParams.regionStart) {
        if ((u32)OSGetArenaHi() <= *(u32*)&__OSRebootParams.regionStart) {
            memset(OSGetArenaLo(), 0, (u32)OSGetArenaHi() - (u32)OSGetArenaLo());
            return;
        }

        memset(OSGetArenaLo(), 0, *(u32*)&__OSRebootParams.regionStart - (u32)OSGetArenaLo());

        if ((u32)OSGetArenaHi() > (u32)__OSRebootParams.regionEnd) {
            memset(__OSRebootParams.regionEnd, 0, (u32)OSGetArenaHi() - (u32)__OSRebootParams.regionEnd);
        }
    }
}

static void InquiryCallback(s32, DVDCommandBlock* block) {
    switch (block->state) {
    case 0:
        __OSDeviceCode = (u16)(0x8000 | DriveInfo.deviceCode);
        break;
    default:
        __OSDeviceCode = 1;
        break;
    }
}

void OSInit(void) {
    u32 consoleType;
    void* bi2StartAddr;

    if (AreWeInitialized == FALSE) {
        AreWeInitialized = TRUE;

        __OSStartTime = __OSGetSystemTime();
        OSDisableInterrupts();

        __OSGetExecParams(&__OSRebootParams);
        PPCMtmmcr0(0);
        PPCMtmmcr1(0);
        PPCMtpmc1(0);
        PPCMtpmc2(0);
        PPCMtpmc3(0);
        PPCMtpmc4(0);
        PPCDisableSpeculation();
        PPCSetFpNonIEEEMode();

        BootInfo = (OSBootInfo*)OSPhysicalToCached(0);
        BI2DebugFlag = 0;
        __DVDLongFileNameFlag = 0;

        bi2StartAddr = (void*)(*(u32*)OSPhysicalToCached(0xF4));
        if (bi2StartAddr) {
            BI2DebugFlag = (void*)((char*)bi2StartAddr + 0xC);
            __PADSpec = ((u32*)bi2StartAddr)[9];
            *(u8*)OSPhysicalToCached(0x30E8) = *BI2DebugFlag;
            *(u8*)OSPhysicalToCached(0x30E9) = __PADSpec;
        } else if (BootInfo->arenaHi) {
            BI2DebugFlagHolder = *(u8*)OSPhysicalToCached(0x30E8);
            BI2DebugFlag = &BI2DebugFlagHolder;
            __PADSpec = *(u8*)OSPhysicalToCached(0x30E9);
        }

        __DVDLongFileNameFlag = 1;

        OSSetArenaLo((!BootInfo->arenaLo) ? &__ArenaLo : BootInfo->arenaLo);
        if ((!BootInfo->arenaLo) && (BI2DebugFlag) && (*(u32*)BI2DebugFlag < 2)) {
            OSSetArenaLo((void*)(((u32)(char*)&_stack_addr + 0x1F) & 0xFFFFFFE0));
        }
        OSSetArenaHi((!BootInfo->arenaHi) ? &__ArenaHi : BootInfo->arenaHi);

        OSExceptionInit();
        __OSInitSystemCall();
        OSInitAlarm();
        __OSModuleInit();
        __OSInterruptInit();
        __OSSetInterruptHandler(0x16, &__OSResetSWInterruptHandler);
        __OSContextInit();
        __OSCacheInit();
        EXIInit();
        SIInit();
        __OSInitSram();
        __OSThreadInit();
        __OSInitAudioSystem();

        DisableWriteGatherPipe();

        if (!__OSInIPL) {
            __OSInitMemoryProtection();
        }

        OSReport("\nDolphin OS\n");
#if DEBUG
        OSReport("Kernel built : %s %s\n", BUILD_DATE, DBUILD_TIME);
#else
        OSReport("Kernel built : %s %s\n", BUILD_DATE, RBUILD_TIME);
#endif
        OSReport("Console Type : ");

        consoleType = OSGetConsoleType();
        switch (consoleType & 0xF0000000) {
        case OS_CONSOLE_RETAIL:
            OSReport("Retail %d\n", consoleType);
            break;
        case OS_CONSOLE_DEVELOPMENT:
        case OS_CONSOLE_TDEV:
            switch (consoleType & 0x0FFFFFFF) {
            case OS_CONSOLE_EMULATOR:
                OSReport("Mac Emulator\n");
                break;
            case OS_CONSOLE_PC_EMULATOR:
                OSReport("PC Emulator\n");
                break;
            case OS_CONSOLE_ARTHUR:
                OSReport("EPPC Arthur\n");
                break;
            case OS_CONSOLE_MINNOW:
                OSReport("EPPC Minnow\n");
                break;
            default:
                OSReport("Development HW%d (%08x)\n", (consoleType & 0xFFFFFFF) - 3, consoleType);
                break;
            }
            break;
        default:
            OSReport("%08x\n", consoleType);
            break;
        }

        OSReport("Memory %d MB\n", (u32)BootInfo->memorySize >> 0x14U);
        OSReport("Arena : 0x%x - 0x%x\n", OSGetArenaLo(), OSGetArenaHi());
        OSRegisterVersion(__OSVersion);

        // if location of debug flag exists, and flag is >= 2, enable MetroTRKInterrupts
        if (BI2DebugFlag && ((*BI2DebugFlag) >= 2)) {
            EnableMetroTRKInterrupts();
        }

        ClearArena();
        OSEnableInterrupts();

        if (!__OSInIPL) {
            DVDInit();

            if (__OSIsGcam) {
                __OSDeviceCode = 0x9000;
                return;
            }

            DCInvalidateRange(&DriveInfo, sizeof(DriveInfo));
            DVDInquiryAsync(&DriveBlock, &DriveInfo, InquiryCallback);
        }
    }
}

static u32 __OSExceptionLocations[] = {
    0x00000100, 0x00000200, 0x00000300, 0x00000400, 0x00000500, 0x00000600, 0x00000700, 0x00000800,
    0x00000900, 0x00000C00, 0x00000D00, 0x00000F00, 0x00001300, 0x00001400, 0x00001700,
};

#if DEBUG
char * __OSExceptionNames[17] = {
	"System reset",
	"MachineCheck",
	"DSI",
	"ISI",
	"External Int.",
	"Alignment",
	"Program",
	"FP Unavailable",
	"Decrementer",
	"System call",
	"Trace",
	"Perf mon",
	"IABR",
	"SMI",
	"Thermal Int.",
    "Protection error",
    "FP Exception",
};
#endif

static void OSExceptionInit(void) {
    __OSException exception;
    void* destAddr;

    // These two vars help us change the exception number embedded
    // in the exception handler code.
    u32* opCodeAddr;
    u32 oldOpCode;

    // Address range of the actual code to be copied.
    u8* handlerStart;
    u32 handlerSize;

    ASSERTMSGLINE(1063, ((u32)&__OSEVEnd - (u32)&__OSEVStart) <= 0x100, "OSExceptionInit(): too big exception vector code.");

    // Install the first level exception vector.
    opCodeAddr = (u32*)__OSEVSetNumber;
    oldOpCode = *opCodeAddr;
    handlerStart = (u8*)__OSEVStart;
    handlerSize = (u32)((u8*)__OSEVEnd - (u8*)__OSEVStart);

    // Install the DB integrator, only if we are the first OSInit to be run
    destAddr = (void*)OSPhysicalToCached(OS_DBJUMPPOINT_ADDR);
    if (*(u32*)destAddr == 0) // Lomem should be zero cleared only once by BS2
    {
        DBPrintf("Installing OSDBIntegrator\n");
        memcpy(destAddr, (void*)__OSDBINTSTART, (u32)__OSDBINTEND - (u32)__OSDBINTSTART);
        DCFlushRangeNoSync(destAddr, (u32)__OSDBINTEND - (u32)__OSDBINTSTART);
        __sync();
        ICInvalidateRange(destAddr, (u32)__OSDBINTEND - (u32)__OSDBINTSTART);
    }

    // Copy the right vector into the table
    for (exception = 0; exception < __OS_EXCEPTION_MAX; exception++) {
        if (BI2DebugFlag && (*BI2DebugFlag >= 2) && __DBIsExceptionMarked(exception)) {
            // this DBPrintf is suspicious.
            DBPrintf(">>> OSINIT: exception %d commandeered by TRK\n", exception);
            continue;
        }

        // Modify the copy of code in text before transferring
        // to the exception table.
        *opCodeAddr = oldOpCode | exception;

        // Modify opcodes at __DBVECTOR if necessary
        if (__DBIsExceptionMarked(exception)) {
            DBPrintf(">>> OSINIT: exception %d vectored to debugger\n", exception);
            memcpy((void*)__DBVECTOR, (void*)__OSDBJUMPSTART, (u32)__OSDBJUMPEND - (u32)__OSDBJUMPSTART);
        } else {
            // make sure the opcodes are still nop
            u32* ops = (u32*)__DBVECTOR;
            int cb;

            for (cb = 0; cb < (u32)__OSDBJUMPEND - (u32)__OSDBJUMPSTART; cb += sizeof(u32)) {
                *ops++ = NOP;
            }
        }

        // Install the modified handler.
        destAddr = (void*)OSPhysicalToCached(__OSExceptionLocations[(u32)exception]);
        memcpy(destAddr, handlerStart, handlerSize);
        DCFlushRangeNoSync(destAddr, handlerSize);
        __sync();
        ICInvalidateRange(destAddr, handlerSize);
    }
    // initialize pointer to exception table
    OSExceptionTable = (void*)OSPhysicalToCached(OS_EXCEPTIONTABLE_ADDR);

    // install default exception handlers
    for (exception = 0; exception < __OS_EXCEPTION_MAX; exception++) {
        __OSSetExceptionHandler(exception, OSDefaultExceptionHandler);
    }

    // restore the old opcode, so that we can re-start an application without
    // downloading the text segments
    *opCodeAddr = oldOpCode;

    DBPrintf("Exceptions initialized...\n");
}

__OSExceptionHandler __OSSetExceptionHandler(__OSException exception, __OSExceptionHandler handler) {
    __OSExceptionHandler oldHandler;

    ASSERTMSGLINE(1205, exception < __OS_EXCEPTION_MAX, "__OSSetExceptionHandler(): unknown exception.");

    oldHandler = OSExceptionTable[exception];
    OSExceptionTable[exception] = handler;
    return oldHandler;
}

__OSExceptionHandler __OSGetExceptionHandler(__OSException exception) {
    ASSERTMSGLINE(1228, exception < __OS_EXCEPTION_MAX, "__OSGetExceptionHandler(): unknown exception.");
    return OSExceptionTable[exception];
}

u32 __OSGetDIConfig(void) {
    return (__DIRegs[9] & 0xFF);
}

void OSRegisterVersion(const char* id) {
    OSReport("%s\n", id);
}
