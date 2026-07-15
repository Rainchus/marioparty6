#include "datadir_enum.h"
#include "game/data.h"
#include "game/disp.h"
#include "game/flag.h"
#include "game/hu3d.h"
#include "game/init.h"
#include "game/memory.h"
#include "game/sprite.h"
#include "game/wipe.h"
#include "game/board/main.h"

#include <math.h>
#include <string.h>

extern BOOL mbTutorialExitReqGet(void);
extern BOOL mbPauseEnableCheck(void);
extern void *mbMalloc(s32 size);
extern float mbSinDeg(float angle);
extern float mbCosDeg(float angle);
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

typedef struct WipeGridEntry_s {
    Vec pos;
    Mtx mtx;
} WIPE_GRID_ENTRY;

typedef struct WipeGridWork_s {
    int xCount;
    int yCount;
    int count;
    WIPE_GRID_ENTRY entry[88];
} WIPE_GRID_WORK;

typedef struct WipeImageWork_s {
    int imageNo;
    float x;
    float y;
    float unkC;
    float size;
    float alpha;
} WIPE_IMAGE_WORK;

typedef struct WipePaperVtx_s {
    float distance;
    float angle;
    float bend;
    float drawData[10];
} WIPE_PAPER_VTX;

typedef struct WipePaperWork_s {
    Vec start;
    Vec end;
    Vec dir;
    Vec normal;
    float length;
    WIPE_PAPER_VTX vtx[64];
} WIPE_PAPER_WORK;

static void WipeMasuMatHook(HU3D_DRAW_OBJ *drawObj, HSF_MATERIAL *material);
static void WipeSpecialDraw(HU3D_MODEL *model, Mtx *mtx);
static void WipeShapeUpdate(HU3D_MODEL *model, Mtx *mtx,
    WIPE_SPECIAL_DATA *wipeData);
static void WipeGridUpdate(HU3D_MODEL *model, Mtx *mtx,
    WIPE_SPECIAL_DATA *wipeData);
static void WipePaperVtxUpdate(WIPE_PAPER_WORK *work, float time);
static void WipePaperUpdate(HU3D_MODEL *model, Mtx *mtx,
    WIPE_SPECIAL_DATA *wipeData);
static void WipeImageUpdate(HU3D_MODEL *model, Mtx *mtx,
    WIPE_SPECIAL_DATA *wipeData);
static void WipeGridDraw(HU3D_MODEL *model, Mtx *mtx,
    WIPE_SPECIAL_DATA *wipeData);
static void WipePaperDraw(HU3D_MODEL *model, Mtx *mtx,
    WIPE_SPECIAL_DATA *wipeData);
static void WipeImageDraw(HU3D_MODEL *model, Mtx *mtx,
    WIPE_SPECIAL_DATA *wipeData);

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

static void (*fadeFunc[8])(HU3D_MODEL *, Mtx *, WIPE_SPECIAL_DATA *) = {
    WipeShapeUpdate,
    WipeShapeUpdate,
    WipeShapeUpdate,
    WipeGridUpdate,
    WipePaperUpdate,
    WipeImageUpdate,
    WipeImageUpdate,
    WipeImageUpdate,
};

static WIPE_SPECIAL_DATA wipeSpecialData;
static ANIMDATA *wipeImageAnim[3];

static void WipeMasuMatHook(HU3D_DRAW_OBJ *drawObj, HSF_MATERIAL *material)
{
    WIPE_SPECIAL_DATA *wipeData;
    HU3D_CAMERA *camera;
    Mtx texMtx;
    GXColor color;
    float tanFov;

    camera = &Hu3DCamera[Hu3DCameraNo];
    wipeData = &wipeSpecialData;
    GXSetNumTexGens(1);
    tanFov = sin(M_PI * (0.5f * camera->fov) / 180.0)
        / cos(M_PI * (0.5f * camera->fov) / 180.0);
    MTXIdentity(texMtx);
    texMtx[0][0] = (-0.5f * drawObj->matrix[0][0])
        / (camera->aspect * (tanFov * drawObj->matrix[2][3]));
    texMtx[1][1] = (0.5f * drawObj->matrix[1][1])
        / (tanFov * drawObj->matrix[2][3]);
    texMtx[0][3] = 0.5f;
    texMtx[1][3] = 0.5f;
    GXLoadTexMtxImm(texMtx, GX_TEXMTX0, GX_MTX2x4);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_POS, GX_TEXMTX0,
        GX_FALSE, GX_PTIDENTITY);
    Hu3DTexLoad(wipeData->texData, 320, 240, GX_TF_RGB565, GX_CLAMP,
        GX_CLAMP, GX_FALSE, GX_TEXMAP0);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0);
    color.a = 255.0f * (1.0f - material->invAlpha);
    GXSetTevColor(GX_TEVREG0, color);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO,
        GX_CC_TEXC);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
        GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO,
        GX_CA_A0);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
        GX_TRUE, GX_TEVPREV);
    Hu3DMatLightSet(drawObj->model, 2, material->hiliteScale);
}

static void WipeSpecialDraw(HU3D_MODEL *model, Mtx *mtx)
{
    WIPE_SPECIAL_DATA *wipeData = &wipeSpecialData;

    if (mbExitCheck()) {
        return;
    }
    if (wipeData->active && fadeFunc[wipeData->fadeType - 1]) {
        fadeFunc[wipeData->fadeType - 1](model, mtx, wipeData);
    }
    switch (wipeData->type) {
        case 4:
            WipeGridDraw(model, mtx, wipeData);
            break;
        case 5:
            WipePaperDraw(model, mtx, wipeData);
            break;
        case 6:
        case 7:
        case 8:
            WipeImageDraw(model, mtx, wipeData);
            break;
    }
}

static void WipeShapeUpdate(HU3D_MODEL *model, Mtx *mtx,
    WIPE_SPECIAL_DATA *wipeData)
{
    float time;
    float scale;

    if (wipeData->state == WIPE_MODE_OUT) {
        time = (float)wipeData->time / (float)wipeData->duration;
        if (!mbPauseEnableCheck()) {
            wipeData->time++;
        }
        scale = 0.5 * (1.0 - sin(M_PI * (90.0f * time) / 180.0));
        Hu3DModelScaleSet(wipeData->masuModelId[wipeData->fadeType - 1],
            scale, scale, scale);
        if (wipeData->time > wipeData->duration) {
            Hu3DModelAttrSet(wipeData->masuModelId[wipeData->fadeType - 1],
                HU3D_ATTR_DISPOFF);
            wipeData->fadeType = 0;
            wipeData->active = FALSE;
            wipeData->type = 0;
            wipeData->stat = 0;
        }
    } else {
        Hu3DModelAttrReset(wipeData->masuModelId[wipeData->fadeType - 1],
            HU3D_ATTR_DISPOFF);
        Hu3DModelPosSet(wipeData->masuModelId[wipeData->fadeType - 1],
            0.0f, 0.0f, -100.0f);
        Hu3DModelScaleSet(wipeData->masuModelId[wipeData->fadeType - 1],
            1.0f, 1.0f, 1.0f);
        GXSetCopyFilter(GX_TRUE, RenderMode->sample_pattern, GX_TRUE,
            RenderMode->vfilter);
        Hu3DFbCopyExec(0, 0, HU_FB_WIDTH, HU_FB_HEIGHT, GX_TF_RGB565,
            GX_TRUE, wipeData->texData);
        GXPixModeSync();
        GXSetCopyFilter(RenderMode->aa, RenderMode->sample_pattern, GX_TRUE,
            RenderMode->vfilter);
        wipeData->fadeType = 0;
        wipeData->active = FALSE;
        wipeData->stat = 1;
    }
}

static void WipePaperVtxUpdate(WIPE_PAPER_WORK *work, float time)
{
    WIPE_PAPER_VTX *vtx;
    HuVec2f pos[64];
    int i;
    float radius;
    float advance;
    float point;
    float angle;

    vtx = work->vtx;
    radius = 200.0f * (1.0f - time);
    advance = time * (work->length + ((float)M_PI * radius));
    for (i = 0; i < 64; i++, vtx++) {
        point = vtx->distance + advance;
        if (point < work->length) {
            pos[i].x = point;
            pos[i].y = 0.0f;
            vtx->bend = 0.0f;
        } else if (point < work->length + ((float)M_PI * radius)) {
            point -= work->length;
            angle = 360.0f * (point / (2.0f * (float)M_PI * radius));
            pos[i].x = work->length + (radius * mbSinDeg(angle));
            pos[i].y = radius * (1.0f - mbCosDeg(angle));
            if (angle > 90.0f) {
                angle = 180.0f - angle;
            }
            vtx->bend = angle;
        } else {
            point -= work->length + ((float)M_PI * radius);
            pos[i].x = work->length - point;
            pos[i].y = 2.0f * radius;
            vtx->bend = 0.0f;
        }
    }
    vtx = work->vtx;
    for (i = 0; i < 63; i++, vtx++) {
        HuVec2f dir;

        dir.x = pos[i + 1].x - pos[i].x;
        dir.y = pos[i + 1].y - pos[i].y;
        vtx->angle = 180.0 * (atan2(dir.y, dir.x) / M_PI);
    }
}

static void WipeGridUpdate(HU3D_MODEL *model, Mtx *mtx,
    WIPE_SPECIAL_DATA *wipeData)
{
    WIPE_GRID_WORK *work;
    WIPE_GRID_WORK *freeWork;
    WIPE_GRID_ENTRY *entry;
    int i;
    int j;
    float time;
    float scale;

    if (mbPauseEnableCheck()) {
        return;
    }
    if (wipeData->state == WIPE_MODE_OUT) {
        work = wipeData->work;
        time = (float)wipeData->time / (float)wipeData->duration;
        if (!mbPauseEnableCheck()) {
            wipeData->time++;
        }
        scale = 1.0 - sin(M_PI * (90.0f * time) / 180.0);
        entry = work->entry;
        for (i = 0; i < work->count; entry++, i++) {
            MTXScale(entry->mtx, scale, scale, 1.0f);
            mtxRotCat(entry->mtx, 0.0f, 0.0f, 180.0f * time);
        }
        if (wipeData->time > wipeData->duration) {
            freeWork = wipeData->work;
            HuMemDirectFree(freeWork);
            wipeData->work = NULL;
            wipeData->fadeType = 0;
            wipeData->active = FALSE;
            wipeData->type = 0;
            wipeData->stat = 0;
        }
    } else {
        work = mbMalloc(sizeof(WIPE_GRID_WORK));
        wipeData->work = work;
        work->xCount = 11;
        work->yCount = 8;
        work->count = work->xCount * work->yCount;
        entry = work->entry;
        for (i = 0; i < 8; i++) {
            for (j = 0; j < 11; entry++, j++) {
                entry->pos.x = 320.0f / (work->xCount - 1)
                    + (640.0f * j) / (work->xCount - 1);
                entry->pos.y = 240.0f / (work->yCount - 1)
                    + (480.0f * i) / (work->yCount - 1);
                entry->pos.z = -100.0f;
                MTXIdentity(entry->mtx);
            }
        }
        GXSetCopyFilter(GX_TRUE, RenderMode->sample_pattern, GX_TRUE,
            RenderMode->vfilter);
        Hu3DFbCopyExec(0, 0, HU_FB_WIDTH, HU_FB_HEIGHT, GX_TF_RGB565,
            GX_TRUE, wipeData->texData);
        GXPixModeSync();
        GXSetCopyFilter(RenderMode->aa, RenderMode->sample_pattern, GX_TRUE,
            RenderMode->vfilter);
        wipeData->fadeType = 0;
        wipeData->active = FALSE;
        wipeData->stat = 1;
    }
}

static void WipeImageUpdate(HU3D_MODEL *model, Mtx *mtx,
    WIPE_SPECIAL_DATA *wipeData)
{
    WIPE_IMAGE_WORK *work;
    WIPE_IMAGE_WORK *freeWork;
    float time;

    if (mbPauseEnableCheck()) {
        return;
    }
    if (wipeData->state == WIPE_MODE_OUT) {
        work = wipeData->work;
        time = (float)wipeData->time / (float)wipeData->duration;
        if (!mbPauseEnableCheck()) {
            wipeData->time++;
        }
        work->size = 1000.0f * time;
        if (wipeData->time > wipeData->duration) {
            if (wipeData->work) {
                freeWork = wipeData->work;
                HuMemDirectFree(freeWork);
                wipeData->work = NULL;
            }
            wipeData->fadeType = 0;
            wipeData->active = FALSE;
            wipeData->type = 0;
            wipeData->stat = 0;
        }
    } else {
        if (!wipeData->work) {
            work = mbMalloc(sizeof(WIPE_IMAGE_WORK));
            wipeData->work = work;
            work->x = 288.0f;
            work->y = 240.0f;
            work->alpha = 1.0f;
            work->imageNo = wipeData->fadeType - 6;
        } else {
            work = wipeData->work;
        }
        time = (float)wipeData->time / (float)wipeData->duration;
        if (!mbPauseEnableCheck()) {
            wipeData->time++;
        }
        work->size = 1000.0f * (1.0f - time);
        if (wipeData->time > wipeData->duration) {
            wipeData->fadeType = 0;
            wipeData->active = FALSE;
            wipeData->stat = 1;
            return;
        }
    }
}

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
