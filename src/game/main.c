/* Block math.h: its "extern inline" sqrtf (compiled as C++ via #pragma cplusplus)
 * emits weak local-static pool data (_half/_three) doubles into every including
 * TU's .sdata2. The original main object's copy was discarded at link, so the
 * split target main object must not contain it. main.c uses no math functions. */
#define _MATH_H

#include "game/main.h"
#include "game/object.h"
#include "game/gamework.h"
#include "game/charman.h"
#include "game/process.h"
#include "game/printfunc.h"
#include "game/perf.h"
#include "game/sprite.h"
#include "game/hu3d.h"
#include "game/init.h"
#include "game/sreset.h"
#include "game/saveload.h"
#include "game/gamemes.h"
#include "game/memory.h"
#include "game/wipe.h"
#include "game/frand.h"

extern OVLTBL _ovltbl[];

extern void HuMCSysInit(void);

static void LoadProcExec(void *param);
static void LoadProcWatch(void);

u32 GlobalCounter;
static OSThread *LoadThread;
static void (*LoadProcHook)(void);
static u8 *LoadProcBuf;
BOOL HuDvdErrWait;
static s32 LoadProcMode;
BOOL HuSRDisableF;
BOOL NintendoDispF;

void main(void)
{
    s16 i;
    s32 retrace;
    s16 temp = 0;

    HuSRDisableF = FALSE;
    HuDvdErrWait = 0;
    NintendoDispF = 0;
    HuSysInit(&GXNtsc480IntDf);
    HuPrcInit();
    HuPadInit();
    GWInit();
    pfInit();
    GlobalCounter = 0;
    HuDataInit();
    HuSprInit();
    Hu3DInit();
    SLSaveFlagSet(FALSE);
    HuPerfInit();
    HuPerfCreate("USR0", 0xFF, 0xFF, 0xFF, 0xFF);
    HuPerfCreate("USR1", 0, 0xFF, 0xFF, 0xFF);
    WipeInit(RenderMode);
    HuMCSysInit();

    for(i=0; i<GW_PLAYER_MAX; i++) {
        GwPlayerConf[i].charNo = CHARNO_NONE;
    }

    omMasterInit(0, _ovltbl, DLL_MAX, DLL_bootdll);
    VIWaitForRetrace();

    if(VIGetNextField() == 0) {
        OSReport("VI_FIELD_BELOW\n");
        VIWaitForRetrace();
    }
    OSReport("%s USA Mode\n", "Sep 25 2004");
    SLSaveFlagSet(FALSE);
    while(1) {
        retrace = VIGetRetraceCount();
        if (HuSoftResetButtonCheck() || HuDvdErrWait) {
            continue;
        }
        HuPerfZero();
        HuPerfBegin(2);
        HuSysBeforeRender();
        HuPerfBegin(0);
        Hu3DPreProc();
        HuPadRead();
        pfClsScr();
        if(LoadProcMode == 0) {
            HuPrcCall(1);
            GameMesExec();
            HuPerfBegin(1);
            Hu3DExec();
        } else {
            HuPerfBegin(1);
            LoadProcWatch();
        }
        HuDvdErrorWatch();
        WipeExecAlways();
        HuPerfEnd(0);
        pfDrawFonts();
        HuPerfEnd(1);
        msmMusFdoutEnd();
        HuSysDoneRender(retrace);
        frand();
        rand8();
        HuPerfEnd(2);
        GlobalCounter++;
    }
}

void HuSysVWaitSet(s16 vWait)
{
    minimumVcount = vWait;
    minimumVcountf = vWait;
}

s16 HuSysVWaitGet(s16 prev)
{
    return minimumVcount;
}

s32 rnd_seed = 0x0000D9ED;

int rand8(void)
{
    rnd_seed = (rnd_seed * 0x41C64E6D) + 0x3039;
    return (u8)(((rnd_seed + 1) >> 16) & 0xFF);
}

static void LoadProcExec(void *param)
{
    void (*hook)(void) = LoadProcHook;

    hook();
    LoadProcMode = 2;
    OSReport("Init All Finished\n");
    OSCancelThread(LoadThread);
}

static void LoadProcWatch(void)
{
    if(LoadProcMode == 2) {
        HuMemDirectFree(LoadProcBuf);
        LoadProcMode = 0;
        WipeLoadKill();
    }
}

void HuLoadProcStart(void (*hook)(void))
{
    WipeLoadCreate();
    LoadProcBuf = HuMemDirectMalloc(HEAP_HEAP, 0x8000);
    LoadThread = OSSetIdleFunction(LoadProcExec, NULL, LoadProcBuf + 0x8000, 0x8000);
    LoadProcHook = hook;
    LoadProcMode = 1;
}

s16 HuLoadProcModeGet(void)
{
    return LoadProcMode;
}
