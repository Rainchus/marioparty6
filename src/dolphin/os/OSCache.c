#include "dolphin/PPCArch.h"
#include "dolphin/os.h"
#include "dolphin/db.h"

void __LCEnable(void);

void DCFlushRangeNoSync(void* addr, u32 nBytes)
{
    if (nBytes == 0) {
        return;
    }

    nBytes += (u32)addr & 31;
    nBytes = (nBytes + 31) >> 5;
    do {
        __dcbf(addr, 0);
        addr = (u8*)addr + 32;
    } while (--nBytes != 0);
}

void DCStoreRangeNoSync(void* addr, u32 nBytes)
{
    if (nBytes == 0) {
        return;
    }

    nBytes += (u32)addr & 31;
    nBytes = (nBytes + 31) >> 5;
    do {
        __dcbst(addr, 0);
        addr = (u8*)addr + 32;
    } while (--nBytes != 0);
}

void DCZeroRange(void* addr, u32 nBytes)
{
    if (nBytes == 0) {
        return;
    }

    nBytes += (u32)addr & 31;
    nBytes = (nBytes + 31) >> 5;
    do {
        __dcbz(addr, 0);
        addr = (u8*)addr + 32;
    } while (--nBytes != 0);
}

void LCEnable()
{
    BOOL enabled;

    enabled = OSDisableInterrupts();
    __LCEnable();
    OSRestoreInterrupts(enabled);
}

u32 LCStoreData(void *destAddr, void *srcAddr, u32 nBytes)
{
    u32 numBlocks = (nBytes + 31) / 32;
    u32 numTransactions = (numBlocks + 128 - 1) / 128;

    while (numBlocks > 0) {
        if (numBlocks < 128) {
            LCStoreBlocks(destAddr, srcAddr, numBlocks);
            numBlocks = 0;
        }
        else {
            LCStoreBlocks(destAddr, srcAddr, 0);
            numBlocks -= 128;
            destAddr = (void *)((u32)destAddr + 4096);
            srcAddr = (void *)((u32)srcAddr + 4096);
        }
    }

    return numTransactions;
}

static inline void L2Disable(void)
{
    __sync();
    PPCMtl2cr(PPCMfl2cr() & ~0x80000000);
    __sync();
}

void L2GlobalInvalidate(void)
{
    L2Disable();
    PPCMtl2cr(PPCMfl2cr() | 0x00200000);
    while (PPCMfl2cr() & 0x00000001u)
        ;
    PPCMtl2cr(PPCMfl2cr() & ~0x00200000);
    while (PPCMfl2cr() & 0x00000001u) {
        DBPrintf(">>> L2 INVALIDATE : SHOULD NEVER HAPPEN\n");
    }
}

static inline void L2Init(void)
{
    u32 oldMSR;
    oldMSR = PPCMfmsr();
    __sync();
    PPCMtmsr(MSR_IR | MSR_DR);
    __sync();
    L2Disable();
    L2GlobalInvalidate();
    PPCMtmsr(oldMSR);
}

static inline void L2Enable(void)
{
    PPCMtl2cr((PPCMfl2cr() | L2CR_L2E) & ~L2CR_L2I);
}

void DMAErrorHandler(OSError error, OSContext *context, ...)
{
    u32 hid2 = PPCMfhid2();

    OSReport("Machine check received\n");
    OSReport("HID2 = 0x%x   SRR1 = 0x%x\n", hid2, context->srr1);
    if (!(hid2 & (HID2_DCHERR | HID2_DNCERR | HID2_DCMERR | HID2_DQOERR)) || !(context->srr1 & SRR1_DMA_BIT)) {
        OSReport("Machine check was not DMA/locked cache related\n");
        OSDumpContext(context);
        PPCHalt();
    }

    OSReport("DMAErrorHandler(): An error occurred while processing DMA.\n");
    OSReport("The following errors have been detected and cleared :\n");

    if (hid2 & HID2_DCHERR) {
        OSReport("\t- Requested a locked cache tag that was already in the cache\n");
    }

    if (hid2 & HID2_DNCERR) {
        OSReport("\t- DMA attempted to access normal cache\n");
    }

    if (hid2 & HID2_DCMERR) {
        OSReport("\t- DMA missed in data cache\n");
    }

    if (hid2 & HID2_DQOERR) {
        OSReport("\t- DMA queue overflowed\n");
    }

    PPCMthid2(hid2);
}

void __OSCacheInit()
{
    if (!(PPCMfhid0() & HID0_ICE)) {
        ICEnable();
        DBPrintf("L1 i-caches initialized\n");
    }
    if (!(PPCMfhid0() & HID0_DCE)) {
        DCEnable();
        DBPrintf("L1 d-caches initialized\n");
    }

    if (!(PPCMfl2cr() & L2CR_L2E)) {
        L2Init();
        L2Enable();
        DBPrintf("L2 cache initialized\n");
    }

    OSSetErrorHandler(OS_ERROR_MACHINE_CHECK, DMAErrorHandler);
    DBPrintf("Locked cache machine check handler installed\n");
}
