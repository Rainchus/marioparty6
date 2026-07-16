#define _MATH_H
#define M_PI 3.141592653589793
double sin(double x);
double cos(double x);
double atan2(double y, double x);

#include "datadir_enum.h"
#include "game/data.h"
#include "game/disp.h"
#include "game/flag.h"
#include "game/gamework.h"
#include "game/hu3d.h"
#include "game/init.h"
#include "game/memory.h"
#include "game/sprite.h"
#include "game/wipe.h"
#include "game/board/main.h"

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
    u16 *texData;
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
    Vec pos[2];
    HuVec2f texCoord[2];
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

static HuVec2f paperST[4] = {
    { 0.6f, -1.0f },
    { -0.6f, 1.0f },
    { 0.6f, 1.0f },
    { -0.6f, -1.0f },
};

static Vec paperPos[4] = {
    { 0.0f, 0.0f, 0.0f },
    { 640.0f, 0.0f, 0.0f },
    { 640.0f, 480.0f, 0.0f },
    { 0.0f, 480.0f, 0.0f },
};

static Vec paperLightPos = { 0.0f, 0.0f, 1000.0f };
static Vec paperLightDir = { 0.0f, 0.0f, -1.0f };

static GXColor paperLightColor = { 255, 255, 255, 255 };
static GXColor paperMatColor = { 255, 255, 255, 255 };
static GXColor paperAmbColor = { 192, 192, 192, 255 };

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

static void WipePaperUpdate(HU3D_MODEL *model, Mtx *mtx,
    WIPE_SPECIAL_DATA *wipeData)
{
    WIPE_PAPER_WORK *work;
    WIPE_PAPER_WORK *freeWork;
    WIPE_PAPER_VTX *vtx;
    HuVec2f startPoint;
    HuVec2f endPoint;
    HuVec2f direction;
    HuVec2f point;
    float intersections[4];
    float time;
    float min;
    float max;
    int i;
    int j;
    int k;
    int playerNo;

    if (mbPauseEnableCheck()) {
        return;
    }
    if (wipeData->state == WIPE_MODE_OUT) {
        work = wipeData->work;
        time = (float)wipeData->time / (float)wipeData->duration;
        if (!mbPauseEnableCheck()) {
            wipeData->time++;
        }
        WipePaperVtxUpdate(work, time);
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
        work = mbMalloc(sizeof(WIPE_PAPER_WORK));
        wipeData->work = work;
        playerNo = GwSystem.turnPlayerNo;
        if (playerNo < 0) {
            playerNo = 0;
        } else if (playerNo > 3) {
            playerNo = 3;
        }
        direction = paperST[playerNo];
        startPoint.x = 320.0f - (400.0f * direction.x);
        startPoint.y = 240.0f - (400.0f * direction.y);
        endPoint.x = 320.0f + (400.0f * direction.x);
        endPoint.y = 240.0f + (400.0f * direction.y);
        PSVECSubtract((Vec *)&endPoint, (Vec *)&startPoint, (Vec *)&direction);

        min = 9999.0f;
        max = -9999.0f;
        for (i = 0; i < 4; i++) {
            time = ((direction.y * paperPos[i].y)
                    + ((direction.x * paperPos[i].x)
                        - (startPoint.x * direction.x))
                    - (startPoint.y * direction.y))
                / ((direction.x * direction.x)
                    + (direction.y * direction.y));
            if (time < min) {
                min = time;
            }
            if (time > max) {
                max = time;
            }
        }
        work->start.x = startPoint.x + (min * direction.x);
        work->start.y = startPoint.y + (min * direction.y);
        work->end.x = startPoint.x + (max * direction.x);
        work->end.y = startPoint.y + (max * direction.y);
        PSVECSubtract(&work->end, &work->start, &work->dir);
        work->length = PSVECMag(&work->dir);
        work->normal.x = work->dir.y;
        work->normal.y = -work->dir.x;
        work->normal.z = 0.0f;
        direction.x = work->dir.y;
        direction.y = -work->dir.x;

        vtx = work->vtx;
        for (i = 0; i < 64; i++, vtx++) {
            time = (float)i / 63.0f;
            vtx->distance = work->length * time;
            point.x = work->start.x + (time * work->dir.x);
            point.y = work->start.y + (time * work->dir.y);
            intersections[0] = -point.x / direction.x;
            intersections[1] = (640.0f - point.x) / direction.x;
            intersections[2] = -point.y / direction.y;
            intersections[3] = (480.0f - point.y) / direction.y;
            for (j = 0; j < 3; j++) {
                for (k = j; k < 4; k++) {
                    if (intersections[j] > intersections[k]) {
                        time = intersections[j];
                        intersections[j] = intersections[k];
                        intersections[k] = time;
                    }
                }
            }
            vtx->pos[0].x = point.x + (intersections[1] * direction.x);
            vtx->pos[0].y = point.y + (intersections[1] * direction.y);
            vtx->pos[0].z = -500.0f;
            vtx->texCoord[0].x = vtx->pos[0].x / 640.0f;
            vtx->texCoord[0].y = vtx->pos[0].y / 480.0f;
            vtx->pos[1].x = point.x + (intersections[2] * direction.x);
            vtx->pos[1].y = point.y + (intersections[2] * direction.y);
            vtx->pos[1].z = -500.0f;
            vtx->texCoord[1].x = vtx->pos[1].x / 640.0f;
            vtx->texCoord[1].y = vtx->pos[1].y / 480.0f;
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

static void WipeGridDraw(HU3D_MODEL *model, Mtx *mtx,
    WIPE_SPECIAL_DATA *wipeData)
{
    WIPE_GRID_WORK *work = wipeData->work;
    WIPE_GRID_ENTRY *entry;
    Mtx drawMtx;
    Mtx44 projection;
    GXColor color = { 255, 255, 255, 255 };
    float tileWidth;
    float tileHeight;
    int i;
    int j;

    C_MTXOrtho(projection, 0.0f, 480.0f, 0.0f, 640.0f, 0.0f,
        1000.0f);
    GXSetProjection(projection, GX_ORTHOGRAPHIC);
    GXSetViewport(0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
    GXSetScissor(0, 0, 640, 480);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0,
        GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevOp(GX_TEVSTAGE0, GX_REPLACE);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_VTX, GX_SRC_VTX, 0,
        GX_DF_CLAMP, GX_AF_SPOT);
    GXSetTevKAlphaSel(GX_TEVSTAGE0, GX_TEV_KASEL_1);
    GXSetColorUpdate(GX_TRUE);
    GXSetAlphaUpdate(GX_FALSE);
    GXSetZCompLoc(GX_FALSE);
    GXSetAlphaCompare(GX_GEQUAL, 1, GX_AOP_AND, GX_GEQUAL, 1);
    GXSetCullMode(GX_CULL_NONE);
    GXSetZMode(GX_FALSE, GX_LEQUAL, GX_FALSE);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA,
        GX_LO_NOOP);
    Hu3DTexLoad(wipeData->texData, 320, 240, GX_TF_RGB565, GX_CLAMP,
        GX_CLAMP, GX_FALSE, GX_TEXMAP0);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO,
        GX_CC_TEXC);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
        GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO,
        GX_CA_KONST);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
        GX_TRUE, GX_TEVPREV);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    GXSetZMode(GX_FALSE, GX_LEQUAL, GX_FALSE);

    tileWidth = 640.0f / (work->xCount - 1);
    tileHeight = 480.0f / (work->yCount - 1);
    for (i = 0; i < work->yCount - 1; i++) {
        for (j = 0; j < work->xCount - 1; j++) {
            entry = &work->entry[j + (i * work->xCount)];
            PSMTXTrans(drawMtx, entry->pos.x, entry->pos.y, entry->pos.z);
            PSMTXConcat(drawMtx, entry->mtx, drawMtx);
            GXLoadPosMtxImm(drawMtx, GX_PNMTX0);
            GXBegin(GX_QUADS, GX_VTXFMT0, 4);
            GXPosition3f32(0.5f * -tileWidth, 0.5f * -tileHeight, 0.0f);
            GXTexCoord2f32((float)j / (work->xCount - 1),
                (float)i / (work->yCount - 1));
            GXPosition3f32(0.5f * tileWidth, 0.5f * -tileHeight, 0.0f);
            GXTexCoord2f32((float)(j + 1) / (work->xCount - 1),
                (float)i / (work->yCount - 1));
            GXPosition3f32(0.5f * tileWidth, 0.5f * tileHeight, 0.0f);
            GXTexCoord2f32((float)(j + 1) / (work->xCount - 1),
                (float)(i + 1) / (work->yCount - 1));
            GXPosition3f32(0.5f * -tileWidth, 0.5f * tileHeight, 0.0f);
            GXTexCoord2f32((float)j / (work->xCount - 1),
                (float)(i + 1) / (work->yCount - 1));
        }
    }
}

static void WipePaperDraw(HU3D_MODEL *model, Mtx *mtx,
    WIPE_SPECIAL_DATA *wipeData)
{
    WIPE_PAPER_WORK *work = wipeData->work;
    WIPE_PAPER_VTX *vtx = work->vtx;
    Mtx rotationMtx;
    Mtx normalMtx;
    GXLightObj light;
    Mtx44 projection;
    GXColor color = { 255, 255, 255, 255 };
    Vec current[2];
    Vec next[2];
    Vec delta[2];
    Vec normal;
    int i;

    C_MTXOrtho(projection, 0.0f, 480.0f, 0.0f, 640.0f, 0.0f,
        1000.0f);
    GXSetProjection(projection, GX_ORTHOGRAPHIC);
    GXSetViewport(0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
    GXSetScissor(0, 0, 640, 480);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0,
        GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevOp(GX_TEVSTAGE0, GX_REPLACE);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_VTX, GX_SRC_VTX, 0,
        GX_DF_CLAMP, GX_AF_SPOT);
    GXSetTevKAlphaSel(GX_TEVSTAGE0, GX_TEV_KASEL_1);
    GXSetColorUpdate(GX_TRUE);
    GXSetAlphaUpdate(GX_FALSE);
    GXSetZCompLoc(GX_FALSE);
    GXSetAlphaCompare(GX_GEQUAL, 1, GX_AOP_AND, GX_GEQUAL, 1);
    GXSetCullMode(GX_CULL_NONE);
    GXSetZMode(GX_FALSE, GX_LEQUAL, GX_FALSE);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA,
        GX_LO_NOOP);

    memset(&light, 0, sizeof(light));
    GXInitLightAttnA(&light, 1.0f, 0.0f, 0.0f);
    GXInitLightDistAttn(&light, 0.0f, 1.0f, GX_DA_OFF);
    GXInitLightPos(&light, paperLightPos.x, paperLightPos.y,
        paperLightPos.z);
    GXInitLightDir(&light, paperLightDir.x, paperLightDir.y,
        paperLightDir.z);
    GXInitLightColor(&light, paperLightColor);
    GXLoadLightObjImm(&light, GX_LIGHT0);

    Hu3DTexLoad(wipeData->texData, 320, 240, GX_TF_RGB565, GX_CLAMP,
        GX_CLAMP, GX_FALSE, GX_TEXMAP0);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_TEXC, GX_CC_RASC,
        GX_CC_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
        GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO,
        GX_CA_KONST);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
        GX_TRUE, GX_TEVPREV);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_TRUE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT0,
        GX_DF_CLAMP, GX_AF_SPOT);
    GXSetChanMatColor(GX_COLOR0A0, paperMatColor);
    GXSetChanAmbColor(GX_COLOR0A0, paperAmbColor);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxDesc(GX_VA_NRM, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_NRM, GX_NRM_XYZ, GX_F32, 0);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);

    current[0] = vtx->pos[0];
    current[1] = vtx->pos[1];
    PSMTXIdentity(rotationMtx);
    GXLoadPosMtxImm(rotationMtx, GX_PNMTX0);
    PSMTXInvXpose(rotationMtx, normalMtx);
    GXLoadNrmMtxImm(normalMtx, GX_PNMTX0);
    for (i = 0; i < 63; i++, vtx++) {
        PSVECSubtract(&vtx[1].pos[0], &vtx->pos[0], &delta[0]);
        PSVECSubtract(&vtx[1].pos[1], &vtx->pos[1], &delta[1]);
        PSMTXRotAxisRad(rotationMtx, &work->normal,
            ((float)M_PI / 180.0f) * vtx->angle);
        PSMTXMultVec(rotationMtx, &delta[0], &delta[0]);
        PSMTXMultVec(rotationMtx, &delta[1], &delta[1]);
        PSVECAdd(&current[0], &delta[0], &next[0]);
        PSVECAdd(&current[1], &delta[1], &next[1]);

        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        PSMTXRotAxisRad(rotationMtx, &work->normal,
            ((float)M_PI / 180.0f) * (90.0f + vtx->bend));
        PSMTXMultVec(rotationMtx, &work->dir, &normal);
        GXPosition3f32(current[0].x, current[0].y, current[0].z);
        GXNormal3f32(normal.x, normal.y, normal.z);
        GXTexCoord2f32(vtx->texCoord[0].x, vtx->texCoord[0].y);
        GXPosition3f32(current[1].x, current[1].y, current[1].z);
        GXNormal3f32(normal.x, normal.y, normal.z);
        GXTexCoord2f32(vtx->texCoord[1].x, vtx->texCoord[1].y);

        PSMTXRotAxisRad(rotationMtx, &work->normal,
            ((float)M_PI / 180.0f) * (90.0f + vtx[1].bend));
        PSMTXMultVec(rotationMtx, &work->dir, &normal);
        GXPosition3f32(next[1].x, next[1].y, next[1].z);
        GXNormal3f32(normal.x, normal.y, normal.z);
        GXTexCoord2f32(vtx[1].texCoord[1].x, vtx[1].texCoord[1].y);
        GXPosition3f32(next[0].x, next[0].y, next[0].z);
        GXNormal3f32(normal.x, normal.y, normal.z);
        GXTexCoord2f32(vtx[1].texCoord[0].x, vtx[1].texCoord[0].y);
        current[0] = next[0];
        current[1] = next[1];
    }
}

static void WipeImageDraw(HU3D_MODEL *model, Mtx *mtx,
    WIPE_SPECIAL_DATA *wipeData)
{
    WIPE_IMAGE_WORK *work;
    Mtx44 imageProjection;
    Mtx posMtx;
    Mtx44 projection;
    GXColor color = { 255, 255, 255, 255 };
    float halfSize;
    float left;
    float top;
    float right;
    float bottom;

    work = wipeData->work;
    C_MTXOrtho(projection, 0.0f, 480.0f, 0.0f, 640.0f, 0.0f,
        1000.0f);
    GXSetProjection(projection, GX_ORTHOGRAPHIC);
    GXSetViewport(0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
    GXSetScissor(0, 0, 640, 480);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0,
        GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevOp(GX_TEVSTAGE0, GX_REPLACE);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_VTX, GX_SRC_VTX, 0,
        GX_DF_CLAMP, GX_AF_SPOT);
    GXSetTevKAlphaSel(GX_TEVSTAGE0, GX_TEV_KASEL_1);
    GXSetColorUpdate(GX_TRUE);
    GXSetAlphaUpdate(GX_FALSE);
    GXSetZCompLoc(GX_FALSE);
    GXSetAlphaCompare(GX_GEQUAL, 1, GX_AOP_AND, GX_GEQUAL, 1);
    GXSetCullMode(GX_CULL_NONE);
    GXSetZMode(GX_FALSE, GX_LEQUAL, GX_FALSE);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA,
        GX_LO_NOOP);

    C_MTXOrtho(imageProjection, 0.0f, 480.0f, 0.0f, 576.0f, 0.0f,
        1000.0f);
    GXSetProjection(imageProjection, GX_ORTHOGRAPHIC);
    HuSprTexLoad(wipeImageAnim[work->imageNo], 0, GX_TEXMAP0, GX_CLAMP,
        GX_CLAMP, GX_LINEAR);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    PSMTXIdentity(posMtx);
    GXLoadPosMtxImm(posMtx, GX_PNMTX0);

    halfSize = 0.5f * work->size;
    left = work->x - halfSize;
    top = work->y - halfSize;
    right = work->x + halfSize;
    bottom = work->y + halfSize;
    color.a = 255.0f * work->alpha;
    GXSetTevColor(GX_TEVREG0, color);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO,
        GX_CC_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
        GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO,
        GX_CA_A0);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
        GX_TRUE, GX_TEVPREV);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XY, GX_F32, 0);
    if (top > 0.0f) {
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        GXPosition2f32(0.0f, 0.0f);
        GXPosition2f32(576.0f, 0.0f);
        GXPosition2f32(576.0f, top);
        GXPosition2f32(0.0f, top);
    }
    if (bottom < 480.0f) {
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        GXPosition2f32(0.0f, bottom);
        GXPosition2f32(576.0f, bottom);
        GXPosition2f32(576.0f, 480.0f);
        GXPosition2f32(0.0f, 480.0f);
    }
    if (left > 0.0f) {
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        GXPosition2f32(0.0f, top);
        GXPosition2f32(left, top);
        GXPosition2f32(left, bottom);
        GXPosition2f32(0.0f, bottom);
    }
    if (left < 576.0f) {
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        GXPosition2f32(right, top);
        GXPosition2f32(576.0f, top);
        GXPosition2f32(576.0f, bottom);
        GXPosition2f32(right, bottom);
    }

    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO,
        GX_CC_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
        GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_TEXA, GX_CA_A0,
        GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
        GX_TRUE, GX_TEVPREV);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XY, GX_F32, 0);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition2f32(left, top);
    GXTexCoord2f32(0.0f, 0.0f);
    GXPosition2f32(right, top);
    GXTexCoord2f32(1.0f, 0.0f);
    GXPosition2f32(right, bottom);
    GXTexCoord2f32(1.0f, 1.0f);
    GXPosition2f32(left, bottom);
    GXTexCoord2f32(0.0f, 1.0f);
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
    WIPE_SPECIAL_DATA *wipeData = &wipeSpecialData;
    u32 texSize;
    void *texDataP;
    u16 *texData;
    int i;

    memset(wipeData, 0, sizeof(*wipeData));
    for (i = 0; i < 3; i++) {
        wipeData->masuModelId[i] = Hu3DModelCreate(
            HuDataSelHeapReadNum(wipeMasuFileTbl[i], HU_MEMNUM_OVL, HEAP_MODEL));
        Hu3DModelAttrSet(wipeData->masuModelId[i], HU3D_ATTR_DISPOFF);
        Hu3DModelMatHookSet(wipeData->masuModelId[i], WipeMasuMatHook);
        Hu3DModelPosSet(wipeData->masuModelId[i], 0.0f, 0.0f, -100.0f);
        Hu3DModelCameraSet(wipeData->masuModelId[i], HU3D_CAM2);
        Hu3DModelLayerSet(wipeData->masuModelId[i], 7);
    }
    wipeData->texSize = GXGetTexBufferSize(320, 240, GX_TF_RGB565,
        GX_FALSE, 0);
    texSize = wipeData->texSize;
    texDataP = HuMemDirectMallocNum(HEAP_HEAP, texSize, HU_MEMNUM_OVL);
    texData = texDataP;
    wipeData->texData = texData;
    DCFlushRange(wipeData->texData, wipeData->texSize);
    wipeData->hookModelId = Hu3DHookFuncCreate(WipeSpecialDraw);
    Hu3DModelCameraSet(wipeData->hookModelId, HU3D_CAM2);
    Hu3DModelLayerSet(wipeData->hookModelId, 6);
    for (i = 0; i < 3; i++) {
        wipeImageAnim[i] = HuSprAnimRead(
            HuDataSelHeapReadNum(wipeImageFileTbl[i], HU_MEMNUM_OVL, HEAP_MODEL));
        HuSprAnimLock(wipeImageAnim[i]);
    }
}

void mbWipeSpecialClose(void)
{
    WIPE_SPECIAL_DATA *wipeData = &wipeSpecialData;
    void *texData;
    int i;

    mbWipeSpecialKill();
    if (wipeData->texData) {
        texData = wipeData->texData;
        HuMemDirectFree(texData);
        wipeData->texData = NULL;
    }
    for (i = 0; i < 3; i++) {
        HuSprAnimKill(wipeImageAnim[i]);
        wipeImageAnim[i] = NULL;
    }
}

void mbWipeSpecialKill(void)
{
    WIPE_SPECIAL_DATA *wipeData = &wipeSpecialData;
    void *work;

    if (wipeData->work) {
        work = wipeData->work;
        HuMemDirectFree(work);
        wipeData->work = NULL;
    }
    wipeData->fadeType = 0;
    wipeData->active = FALSE;
    wipeData->type = 0;
    wipeData->stat = 0;
}

void mbWipeSpecialCreate(int state, int type, int time)
{
    WIPE_SPECIAL_DATA *wipeData = &wipeSpecialData;
    void *work;

    if (!_CheckFlag(FLAG_BOARD_TUTORIAL) || !mbTutorialExitReqGet()) {
        wipeData->state = state;
        wipeData->fadeType = type;
        wipeData->time = 0;
        wipeData->duration = time;
        wipeData->active = TRUE;
        wipeData->type = type;
        if (state == WIPE_MODE_IN) {
            if (wipeData->work) {
                work = wipeData->work;
                HuMemDirectFree(work);
            }
            wipeData->work = NULL;
        }
    }
}

void mbWipeSpecialFadeOutCreate(int type, int time)
{
    WIPE_SPECIAL_DATA *wipeData = &wipeSpecialData;
    BOOL wipeF;

    if (_CheckFlag(FLAG_BOARD_TUTORIAL) && mbTutorialExitReqGet()) {
        wipeF = FALSE;
    } else {
        wipeData->state = WIPE_MODE_OUT;
        wipeData->fadeType = type;
        wipeData->time = 0;
        wipeData->duration = time;
        wipeData->active = TRUE;
        wipeData->type = type;
        wipeF = TRUE;
    }
    if (wipeF) {
        mbWipeSpecialWait();
    }
}

void mbWipeSpecialFadeInCreate(int type, int time)
{
    WIPE_SPECIAL_DATA *wipeData = &wipeSpecialData;
    void *work;
    BOOL wipeF;

    if (_CheckFlag(FLAG_BOARD_TUTORIAL) && mbTutorialExitReqGet()) {
        wipeF = FALSE;
    } else {
        wipeData->state = WIPE_MODE_IN;
        wipeData->fadeType = type;
        wipeData->time = 0;
        wipeData->duration = time;
        wipeData->active = TRUE;
        wipeData->type = type;
        if (wipeData->work) {
            work = wipeData->work;
            HuMemDirectFree(work);
        }
        wipeData->work = NULL;
        wipeF = TRUE;
    }
    if (wipeF) {
        mbWipeSpecialWait();
    }
}

BOOL mbWipeSpecialCheck(void)
{
    WIPE_SPECIAL_DATA *wipeData = &wipeSpecialData;

    return wipeData->active | WipeCheck();
}

void mbWipeSpecialWait(void)
{
    while (mbWipeSpecialCheck()) {
        HuPrcVSleep();
    }
}

BOOL mbWipeSpecialStatGet(void)
{
    return wipeSpecialData.stat | WipeCheckIn();
}
