#include "game/board/main.h"
#include "game/board/object.h"
#include "game/board/player.h"
#include "game/data.h"
#include "game/disp.h"
#include "game/esprite.h"
#include "game/gamework.h"
#include "game/hu3d.h"
#include "game/memory.h"
#include "game/process.h"
#include "game/sprite.h"

#include "dolphin/gx.h"
#include "dolphin/mtx.h"

extern void *mbMalloc(s32 size);
extern void *mbMallocFlush(s32 size);
extern void *mbMallocFlushModel(s32 size);

typedef struct PausePanelWork_s {
    int modelId;              /* 0x00 */
    int batsuModelId;         /* 0x04 */
    int sprId;                /* 0x08 */
    ANIMDATA *anim;           /* 0x0C */
    HU3D_ANIMID animId[2];    /* 0x10 */
    HuVecF pos;               /* 0x14 */
    HuVecF posStart;          /* 0x20 */
    HuVecF posTarget;         /* 0x2C */
    BOOL batsuF;              /* 0x38 */
    float scale;              /* 0x3C */
    float scaleStart;         /* 0x40 */
    float scaleTarget;        /* 0x44 */
    float scaleBase;          /* 0x48 */
    s16 bank;                 /* 0x4C */
    s16 motion;               /* 0x4E */
    s16 time;                 /* 0x50 */
    s16 maxTime;              /* 0x52 */
    s16 delay;                /* 0x54 */
    s16 animTime;             /* 0x56 */
    s16 animMaxTime;          /* 0x58 */
} PAUSE_PANEL_WORK;

static BOOL playerDispF[GW_PLAYER_MAX];

static s32 configPadDisable;
static HUPROCESS *pauseGuideProc;
static PAUSE_PANEL_WORK *pausePanelWork;
static BOOL pauseGuideKillF;
static s32 pauseDispCopyModelId;
static s32 pauseDispCopyCounter;
static void *pauseDispCopyFb;

static void PauseDispCopyDraw(HU3D_MODEL *modelP, Mtx *mtx);
static void PauseGuideMain(void);
static void PauseGuideDestroy(void);
static BOOL GWStorySingleCheck(void);

void mbPauseDispCopyCreate(void)
{
    int i;
    int fbSize;

    pauseDispCopyFb = NULL;
    pauseDispCopyCounter = 0;
    fbSize = GXGetTexBufferSize(HU_FB_WIDTH / 2, HU_FB_HEIGHT / 2, GX_TF_RGB565, GX_FALSE, 0);
    if (!GWStorySingleCheck) {
        pauseDispCopyFb = mbMallocFlush(fbSize);
    } else {
        pauseDispCopyFb = mbMallocFlushModel(fbSize);
    }
    pauseDispCopyModelId = Hu3DHookFuncCreate(PauseDispCopyDraw);
    Hu3DModelCameraSet(pauseDispCopyModelId, 4);
    Hu3DModelLayerSet(pauseDispCopyModelId, 2);
    HuPrcVSleep();
    for (i = 0; i < GW_PLAYER_MAX; i++) {
        playerDispF[i] = mbObjDispGet(mbPlayerObjIDGet(i));
        mbObjDispSet(mbPlayerObjIDGet(i), FALSE);
    }
}

void mbPauseDispCopyKill(void)
{
    int i;

    pauseDispCopyCounter = 0;
    HuMemDirectFree(pauseDispCopyFb);
    pauseDispCopyFb = NULL;
    Hu3DModelKill(pauseDispCopyModelId);
    for (i = 0; i < GW_PLAYER_MAX; i++) {
        if (playerDispF[i]) {
            mbObjDispSet(mbPlayerObjIDGet(i), TRUE);
        }
    }
}

static void PauseDispCopyDraw(HU3D_MODEL *modelP, Mtx *mtx)
{
    if (!pauseDispCopyCounter) {
        GXSetTexCopySrc(0, 0, HU_FB_WIDTH, HU_FB_HEIGHT);
        GXSetTexCopyDst(HU_FB_WIDTH / 2, HU_FB_HEIGHT / 2, GX_TF_RGB565, GX_TRUE);
        GXCopyTex(pauseDispCopyFb, FALSE);
        GXPixModeSync();
        pauseDispCopyCounter++;
    } else {
        Mtx modelview;
        Mtx44 proj;
        GXTexObj texObj;

        MTXOrtho(proj, 0, HU_FB_HEIGHT, 0, HU_FB_WIDTH, 0, 100);
        GXSetProjection(proj, GX_ORTHOGRAPHIC);
        GXSetViewport(0, 0, HU_FB_WIDTH, HU_FB_HEIGHT, 0, 1);
        GXSetScissor(0, 0, HU_FB_WIDTH, HU_FB_HEIGHT);
        MTXIdentity(modelview);
        GXLoadPosMtxImm(modelview, GX_PNMTX0);
        GXSetCullMode(GX_CULL_NONE);
        GXSetNumChans(1);
        GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, 0, GX_DF_CLAMP, GX_AF_NONE);
        GXSetNumTexGens(1);
        GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
        GXSetNumTevStages(1);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
        GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_FALSE, GX_TEVPREV);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_KONST);
        GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_FALSE, GX_TEVPREV);
        GXSetBlendMode(GX_BM_NONE, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_SET);
        GXSetAlphaCompare(GX_GEQUAL, 1, GX_AOP_AND, GX_GEQUAL, 1);
        GXSetZMode(GX_FALSE, GX_LEQUAL, GX_FALSE);
        GXInitTexObj(&texObj, pauseDispCopyFb, HU_FB_WIDTH / 2, HU_FB_HEIGHT / 2, GX_TF_RGB565, GX_CLAMP,
            GX_CLAMP, GX_TRUE);
        GXInitTexObjLOD(&texObj, GX_LINEAR, GX_LINEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE, GX_ANISO_1);
        GXLoadTexObj(&texObj, GX_TEXMAP0);
        GXClearVtxDesc();
        GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
        GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_S16, 0);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_S16, 8);
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        GXPosition3s16(0, 0, -50);
        GXTexCoord2s16(0, 0);
        GXPosition3s16(HU_FB_WIDTH, 0, -50);
        GXTexCoord2s16(256, 0);
        GXPosition3s16(HU_FB_WIDTH, HU_FB_HEIGHT, -50);
        GXTexCoord2s16(256, 256);
        GXPosition3s16(0, HU_FB_HEIGHT, -50);
        GXTexCoord2s16(0, 256);
        GXEnd();
    }
}

void mbPauseGuideCreate(void)
{
    pausePanelWork = mbMalloc(sizeof(PAUSE_PANEL_WORK) * 20);
    pauseGuideKillF = FALSE;
    pauseGuideProc = HuPrcChildCreate(PauseGuideMain, 0x2011, 0x2000, 0,
        mbMainProc);
    HuPrcDestructorSet2(pauseGuideProc, PauseGuideDestroy);
    HuPrcSetStat(pauseGuideProc,
        HU_PRC_STAT_PAUSE_ON | HU_PRC_STAT_UPAUSE_ON);
}

static void PauseGuideDestroy(void)
{
    int i;
    PAUSE_PANEL_WORK *work;

    if (pausePanelWork) {
        for (i = 0; i < 20; i++) {
        }
        work = pausePanelWork;
        HuMemDirectFree(work);
        pausePanelWork = NULL;
    }
    pauseGuideProc = NULL;
}

s16 mbPausePanelCreate(int dataNum, unsigned int espDataNum)
{
    int i;
    PAUSE_PANEL_WORK *work;
    int panelId;

    for (panelId = 1; panelId < 20; panelId++) {
        if (pausePanelWork[panelId].modelId <= 0) {
            break;
        }
    }
    work = &pausePanelWork[panelId];
    memset(work, 0, sizeof(PAUSE_PANEL_WORK));
    work->scale = work->scaleStart = work->scaleTarget = work->scaleBase = 1.0f;
    work->pos.z = work->posStart.z = work->posTarget.z = -500.0f;
    work->modelId = mbObjCreate(mbBoardDataNumGet(DATANUM(DATA_bpause6, 0x25)),
        NULL, FALSE);
    mbObjCameraSet(work->modelId, 4);
    mbObjLayerSet(work->modelId, 4);
    {
        MBMODELID modelId = work->modelId;

        mbObjAttrSet(modelId, HU3D_MOTATTR_LOOP);
    }
    {
        MBMODELID modelId = work->modelId;

        mbObjAttrSet(modelId, 0x00200000);
    }
    mbObjDispSet(work->modelId, FALSE);
    work->anim = HuSprAnimRead(HuDataSelHeapReadNum(
        mbBoardDataNumGet(dataNum), HU_MEMNUM_OVL, HEAP_MODEL));
    for (i = 0; i < 2; i++) {
        if (i == 0) {
            work->animId[0] = Hu3DAnimCreate(work->anim,
                mbObjModelIDGet(work->modelId), "S3TCys77120");
        } else {
            work->animId[i] = Hu3DAnimLink(work->animId[0],
                mbObjModelIDGet(work->modelId), "S3TCys77121");
        }
        Hu3DAnmNoSet(work->animId[i], 0);
    }
    work->batsuModelId = mbObjCreate(
        mbBoardDataNumGet(DATANUM(DATA_bpause6, 0x24)), NULL, TRUE);
    mbObjCameraSet(work->batsuModelId, 4);
    mbObjLayerSet(work->batsuModelId, 4);
    {
        MBMODELID modelId = work->batsuModelId;

        mbObjAttrSet(modelId, 0x00200000);
    }
    mbObjDispSet(work->batsuModelId, FALSE);
    work->sprId = -1;
    if (espDataNum != 0) {
        work->sprId = espEntry(mbBoardDataNumGet(espDataNum), 100, 0);
        espAttrSet(work->sprId, HUSPR_ATTR_LINEAR);
        espDispOff(work->sprId);
    }
    return panelId;
}

void mbPausePanelKill(s16 panelId)
{
    PAUSE_PANEL_WORK *work = &pausePanelWork[panelId];
    int i;

    for (i = 0; i < 2; i++) {
        if (work->animId[i] >= 0) {
            Hu3DAnimKill(work->animId[i]);
        }
        work->animId[i] = -1;
    }
    if (work->anim) {
        HuSprAnimKill(work->anim);
    }
    work->anim = NULL;
    if (work->modelId != 0) {
        mbObjKill(work->modelId);
        work->modelId = 0;
    }
    if (work->batsuModelId != 0) {
        mbObjKill(work->batsuModelId);
        work->batsuModelId = 0;
    }
    if (work->sprId >= 0) {
        espKill(work->sprId);
        work->sprId = -1;
    }
}

void mbPauseGuideKill(void)
{
    pauseGuideKillF = TRUE;
}

void mbPausePanelPosSet(s16 panelId, float x, float y)
{
    PAUSE_PANEL_WORK *work = &pausePanelWork[panelId];

    work->pos.x = work->posStart.x = work->posTarget.x = x;
    work->pos.y = work->posStart.y = work->posTarget.y = y;
}

void mbPausePanelPosGet(s16 panelId, HuVecF *pos)
{
    PAUSE_PANEL_WORK *work = &pausePanelWork[panelId];

    pos->x = work->pos.x;
    pos->y = work->pos.y;
    pos->z = 0.0f;
}

void mbPausePanelRotSet(s16 panelId, float rotX, float rotY, float rotZ)
{
    PAUSE_PANEL_WORK *work = &pausePanelWork[panelId];

    mbObjRotSet(work->modelId, rotX, rotY, rotZ);
}

void mbPausePanelScaleSet(s16 panelId, float scale)
{
    PAUSE_PANEL_WORK *work = &pausePanelWork[panelId];

    work->scale = work->scaleStart = work->scaleTarget = scale;
}

float mbPausePanelScaleGet(s16 panelId)
{
    PAUSE_PANEL_WORK *work = &pausePanelWork[panelId];

    return work->scale;
}

void mbPausePanelBankSet(s16 panelId, int bank)
{
    PAUSE_PANEL_WORK *work = &pausePanelWork[panelId];

    work->bank = bank;
    Hu3DAnmNoSet(work->animId[0], bank);
    Hu3DAnmNoSet(work->animId[1], bank);
    if (work->sprId >= 0) {
        espBankSet(work->sprId, bank);
    }
}

void mbPausePanelBatsuSet(s16 panelId, BOOL batsuF)
{
    PAUSE_PANEL_WORK *work = &pausePanelWork[panelId];

    work->batsuF = batsuF;
}

BOOL mbPausePanelFreezeGet(s16 panelId)
{
    PAUSE_PANEL_WORK *work = &pausePanelWork[panelId];
    BOOL freezeF = FALSE;

    if (work->motion == 0 && work->animMaxTime == 0) {
        freezeF = TRUE;
    }
    return freezeF;
}

void mbPausePanelSizeSet(s16 panelId, int time, float scale)
{
    PAUSE_PANEL_WORK *work = &pausePanelWork[panelId];

    work->motion = 2;
    work->maxTime = time;
    work->time = 0;
    work->scaleTarget = scale;
    work->scaleStart = work->scale;
}

void mbPausePanelGrowSet(s16 panelId, int time, int delay, float scale)
{
    PAUSE_PANEL_WORK *work = &pausePanelWork[panelId];

    work->motion = 4;
    work->maxTime = time;
    work->delay = delay + 1;
    work->time = 0;
    work->scaleTarget = 1.0f;
    work->scaleStart = work->scale = 0.00001f;
    work->scaleBase = scale;
    mbObjDispSet(work->modelId, FALSE);
    if (work->batsuModelId != 0) {
        mbObjDispSet(work->batsuModelId, FALSE);
    }
}

void mbPausePanelShrinkSet(s16 panelId, int time, int delay)
{
    PAUSE_PANEL_WORK *work = &pausePanelWork[panelId];

    work->motion = 5;
    work->maxTime = time;
    work->delay = delay + 1;
    work->time = 0;
}

void mbConfigPadDisableSet(BOOL disableF)
{
    configPadDisable = disableF;
}

static BOOL GWStorySingleCheck(void)
{
    return !GWPartyGet();
}
