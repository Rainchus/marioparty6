#include "game/board/object.h"
#include "game/board/player.h"
#include "game/disp.h"
#include "game/gamework.h"
#include "game/hu3d.h"
#include "game/memory.h"

#include "dolphin/gx.h"
#include "dolphin/mtx.h"

extern void *mbMallocFlush(s32 size);
extern void *mbMallocFlushModel(s32 size);

static BOOL playerDispF[GW_PLAYER_MAX];

static s32 configPadDisable;
static s32 pauseDispCopyModelId;
static s32 pauseDispCopyCounter;
static void *pauseDispCopyFb;
static BOOL pauseGuideKillF;

static void PauseDispCopyDraw(HU3D_MODEL *modelP, Mtx *mtx);
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

void mbConfigPadDisableSet(BOOL disableF)
{
    configPadDisable = disableF;
}

void mbPauseGuideKill(void)
{
    pauseGuideKillF = TRUE;
}

static BOOL GWStorySingleCheck(void)
{
    BOOL partyF = GWPartyGet();

    return !partyF;
}
