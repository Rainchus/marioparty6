#include "datadir_enum.h"
#include "game/data.h"
#include "game/flag.h"
#include "game/hu3d.h"
#include "game/memory.h"
#include "game/sprite.h"
#include "game/wipe.h"

#include <string.h>

extern BOOL mbTutorialExitReqGet(void);
void mbWipeSpecialKill(void);
void mbWipeSpecialWait(void);

typedef struct WipeSpecialData_s {
    BOOL active;
    int state;
    int stat;
    s16 fadeType;
    s16 time;
    s16 duration;
    s16 masuModelId[3];
    s16 hookModelId;
    u32 texSize;
    void *texData;
    void *work;
    int type;
} WIPE_SPECIAL_DATA;

static void WipeMasuMatHook(HU3D_DRAW_OBJ *drawObj, HSF_MATERIAL *material);
static void WipeSpecialDraw(HU3D_MODEL *model, Mtx *mtx);

static int wipeMasuFileTbl[3] = {
    DATANUM(DATA_bmasu, 7),
    DATANUM(DATA_bmasu, 8),
    DATANUM(DATA_bmasu, 9),
};

static int wipeImageFileTbl[3] = {
    DATANUM(DATA_bmasu, 10),
    DATANUM(DATA_bmasu, 12),
    DATANUM(DATA_bmasu, 11),
};

static WIPE_SPECIAL_DATA wipeSpecialData;
static ANIMDATA *wipeImageAnim[3];

void mbWipeCreate(s16 mode, s16 type, s16 time)
{
    if (!_CheckFlag(FLAG_BOARD_TUTORIAL) || !mbTutorialExitReqGet()) {
        WipeCreate(mode, type, time);
    }
}

void mbWipeWait(void)
{
    while (WipeCheck()) {
        HuPrcVSleep();
    }
}

void mbWipeFadeOut(void)
{
    BOOL wipeF;

    if (_CheckFlag(FLAG_BOARD_TUTORIAL) && mbTutorialExitReqGet()) {
        wipeF = FALSE;
    } else {
        WipeCreate(WIPE_MODE_OUT, WIPE_TYPE_NORMAL, 21);
        wipeF = TRUE;
    }
    if (wipeF) {
        WipeColorSet(0, 0, 0);
        while (WipeCheck()) {
            HuPrcVSleep();
        }
    }
}

void mbWipeFadeIn(void)
{
    BOOL wipeF;

    if (_CheckFlag(FLAG_BOARD_TUTORIAL) && mbTutorialExitReqGet()) {
        wipeF = FALSE;
    } else {
        WipeCreate(WIPE_MODE_IN, WIPE_TYPE_NORMAL, 21);
        wipeF = TRUE;
    }
    if (wipeF) {
        while (WipeCheck()) {
            HuPrcVSleep();
        }
    }
}

void mbWipeWhiteFadeOut(void)
{
    BOOL wipeF;

    if (_CheckFlag(FLAG_BOARD_TUTORIAL) && mbTutorialExitReqGet()) {
        wipeF = FALSE;
    } else {
        WipeCreate(WIPE_MODE_OUT, WIPE_TYPE_WHITE, 21);
        wipeF = TRUE;
    }
    if (wipeF) {
        while (WipeCheck()) {
            HuPrcVSleep();
        }
    }
}

void mbWipeWhiteFadeIn(void)
{
    BOOL wipeF;

    if (_CheckFlag(FLAG_BOARD_TUTORIAL) && mbTutorialExitReqGet()) {
        wipeF = FALSE;
    } else {
        WipeCreate(WIPE_MODE_IN, WIPE_TYPE_WHITE, 21);
        wipeF = TRUE;
    }
    if (wipeF) {
        while (WipeCheck()) {
            HuPrcVSleep();
        }
    }
}

void mbWipeDissolveFadeOut(void)
{
    BOOL wipeF;

    if (_CheckFlag(FLAG_BOARD_TUTORIAL) && mbTutorialExitReqGet()) {
        wipeF = FALSE;
    } else {
        WipeCreate(WIPE_MODE_OUT, WIPE_TYPE_CROSS_COPY, 1);
        wipeF = TRUE;
    }
    if (wipeF) {
        while (WipeCheck()) {
            HuPrcVSleep();
        }
    }
}

void mbWipeDissolveFadeIn(void)
{
    BOOL wipeF;

    if (_CheckFlag(FLAG_BOARD_TUTORIAL) && mbTutorialExitReqGet()) {
        wipeF = FALSE;
    } else {
        WipeCreate(WIPE_MODE_IN, WIPE_TYPE_CROSS_COPY, 30);
        wipeF = TRUE;
    }
    if (wipeF) {
        while (WipeCheck()) {
            HuPrcVSleep();
        }
    }
}

void mbWipeFadeOutTime(int time)
{
    BOOL wipeF;

    if (_CheckFlag(FLAG_BOARD_TUTORIAL) && mbTutorialExitReqGet()) {
        wipeF = FALSE;
    } else {
        WipeCreate(WIPE_MODE_OUT, WIPE_TYPE_NORMAL, time);
        wipeF = TRUE;
    }
    if (wipeF) {
        WipeColorSet(0, 0, 0);
        while (WipeCheck()) {
            HuPrcVSleep();
        }
    }
}

void mbWipeFadeInTime(int time)
{
    BOOL wipeF;

    if (_CheckFlag(FLAG_BOARD_TUTORIAL) && mbTutorialExitReqGet()) {
        wipeF = FALSE;
    } else {
        WipeCreate(WIPE_MODE_IN, WIPE_TYPE_NORMAL, time);
        wipeF = TRUE;
    }
    if (wipeF) {
        while (WipeCheck()) {
            HuPrcVSleep();
        }
    }
}

void mbWipeWhiteFadeOutTime(int time)
{
    BOOL wipeF;

    if (_CheckFlag(FLAG_BOARD_TUTORIAL) && mbTutorialExitReqGet()) {
        wipeF = FALSE;
    } else {
        WipeCreate(WIPE_MODE_OUT, WIPE_TYPE_WHITE, time);
        wipeF = TRUE;
    }
    if (wipeF) {
        while (WipeCheck()) {
            HuPrcVSleep();
        }
    }
}

void mbWipeWhiteFadeInTime(int time)
{
    BOOL wipeF;

    if (_CheckFlag(FLAG_BOARD_TUTORIAL) && mbTutorialExitReqGet()) {
        wipeF = FALSE;
    } else {
        WipeCreate(WIPE_MODE_IN, WIPE_TYPE_WHITE, time);
        wipeF = TRUE;
    }
    if (wipeF) {
        while (WipeCheck()) {
            HuPrcVSleep();
        }
    }
}

void mbWipeDissolveFadeOutTime(int time)
{
    BOOL wipeF;

    if (_CheckFlag(FLAG_BOARD_TUTORIAL) && mbTutorialExitReqGet()) {
        wipeF = FALSE;
    } else {
        WipeCreate(WIPE_MODE_OUT, WIPE_TYPE_CROSS_COPY, time);
        wipeF = TRUE;
    }
    if (wipeF) {
        while (WipeCheck()) {
            HuPrcVSleep();
        }
    }
}

void mbWipeDissolveFadeInTime(int time)
{
    BOOL wipeF;

    if (_CheckFlag(FLAG_BOARD_TUTORIAL) && mbTutorialExitReqGet()) {
        wipeF = FALSE;
    } else {
        WipeCreate(WIPE_MODE_IN, WIPE_TYPE_CROSS_COPY, time);
        wipeF = TRUE;
    }
    if (wipeF) {
        while (WipeCheck()) {
            HuPrcVSleep();
        }
    }
}

void mbWipeSpecialInit(void)
{
    int i;

    memset(&wipeSpecialData, 0, sizeof(wipeSpecialData));
    for (i = 0; i < 3; i++) {
        wipeSpecialData.masuModelId[i] = Hu3DModelCreate(
            HuDataSelHeapReadNum(wipeMasuFileTbl[i], HU_MEMNUM_OVL, HEAP_MODEL));
        Hu3DModelAttrSet(wipeSpecialData.masuModelId[i], HU3D_ATTR_DISPOFF);
        Hu3DModelMatHookSet(wipeSpecialData.masuModelId[i], WipeMasuMatHook);
        Hu3DModelPosSet(wipeSpecialData.masuModelId[i], 0.0f, 0.0f, -100.0f);
        Hu3DModelCameraSet(wipeSpecialData.masuModelId[i], HU3D_CAM2);
        Hu3DModelLayerSet(wipeSpecialData.masuModelId[i], 7);
    }
    wipeSpecialData.texSize = GXGetTexBufferSize(320, 240, GX_TF_RGB565,
        GX_FALSE, 0);
    wipeSpecialData.texData = HuMemDirectMallocNum(HEAP_HEAP,
        wipeSpecialData.texSize, HU_MEMNUM_OVL);
    DCFlushRange(wipeSpecialData.texData, wipeSpecialData.texSize);
    wipeSpecialData.hookModelId = Hu3DHookFuncCreate(WipeSpecialDraw);
    Hu3DModelCameraSet(wipeSpecialData.hookModelId, HU3D_CAM2);
    Hu3DModelLayerSet(wipeSpecialData.hookModelId, 6);
    for (i = 0; i < 3; i++) {
        wipeImageAnim[i] = HuSprAnimRead(
            HuDataSelHeapReadNum(wipeImageFileTbl[i], HU_MEMNUM_OVL, HEAP_MODEL));
        HuSprAnimLock(wipeImageAnim[i]);
    }
}

void mbWipeSpecialClose(void)
{
    int i;

    mbWipeSpecialKill();
    if (wipeSpecialData.texData != NULL) {
        HuMemDirectFree(wipeSpecialData.texData);
        wipeSpecialData.texData = NULL;
    }
    for (i = 0; i < 3; i++) {
        HuSprAnimKill(wipeImageAnim[i]);
        wipeImageAnim[i] = NULL;
    }
}

void mbWipeSpecialKill(void)
{
    if (wipeSpecialData.work != NULL) {
        HuMemDirectFree(wipeSpecialData.work);
        wipeSpecialData.work = NULL;
    }
    wipeSpecialData.fadeType = 0;
    wipeSpecialData.active = FALSE;
    wipeSpecialData.type = 0;
    wipeSpecialData.stat = 0;
}

void mbWipeSpecialCreate(int state, int type, int time)
{
    if (!_CheckFlag(FLAG_BOARD_TUTORIAL) || !mbTutorialExitReqGet()) {
        wipeSpecialData.state = state;
        wipeSpecialData.fadeType = type;
        wipeSpecialData.time = 0;
        wipeSpecialData.duration = time;
        wipeSpecialData.active = TRUE;
        wipeSpecialData.type = type;
        if (state == WIPE_MODE_IN && wipeSpecialData.work != NULL) {
            HuMemDirectFree(wipeSpecialData.work);
            wipeSpecialData.work = NULL;
        }
    }
}

void mbWipeSpecialFadeOutCreate(int type, int time)
{
    BOOL wipeF;

    if (_CheckFlag(FLAG_BOARD_TUTORIAL) && mbTutorialExitReqGet()) {
        wipeF = FALSE;
    } else {
        wipeSpecialData.state = WIPE_MODE_OUT;
        wipeSpecialData.fadeType = type;
        wipeSpecialData.time = 0;
        wipeSpecialData.duration = time;
        wipeSpecialData.active = TRUE;
        wipeSpecialData.type = type;
        wipeF = TRUE;
    }
    if (wipeF) {
        mbWipeSpecialWait();
    }
}

void mbWipeSpecialFadeInCreate(int type, int time)
{
    BOOL wipeF;

    if (_CheckFlag(FLAG_BOARD_TUTORIAL) && mbTutorialExitReqGet()) {
        wipeF = FALSE;
    } else {
        wipeSpecialData.state = WIPE_MODE_IN;
        wipeSpecialData.fadeType = type;
        wipeSpecialData.time = 0;
        wipeSpecialData.duration = time;
        wipeSpecialData.active = TRUE;
        wipeSpecialData.type = type;
        if (wipeSpecialData.work != NULL) {
            HuMemDirectFree(wipeSpecialData.work);
            wipeSpecialData.work = NULL;
        }
        wipeF = TRUE;
    }
    if (wipeF) {
        mbWipeSpecialWait();
    }
}

BOOL mbWipeSpecialCheck(void)
{
    return wipeSpecialData.active | WipeCheck();
}

void mbWipeSpecialWait(void)
{
    while (wipeSpecialData.active | WipeCheck()) {
        HuPrcVSleep();
    }
}

BOOL mbWipeSpecialStatGet(void)
{
    return wipeSpecialData.stat | WipeCheckIn();
}
