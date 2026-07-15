/* masu.o has no weak sqrtf constants in .sdata2. */
#define _MATH_H
#include "dolphin/math.h"

#include "game/board/masu.h"
#include "game/board/audio.h"
#include "game/board/branch.h"
#include "game/board/camera.h"
#include "game/board/gate.h"
#include "game/board/main.h"
#include "game/board/object.h"
#include "game/board/player.h"
#include "game/board/status.h"
#include "game/board/tutorial.h"
#include "game/data.h"
#include "game/gamework.h"
#include "game/hsfex.h"
#include "game/hu3d.h"
#include "game/object.h"
#include "game/sprite.h"
#include <stdlib.h>
#include <string.h>

typedef struct MasuFindWork_s {
    s16 id;
    s16 linkNo;
} MASUFINDWORK;

typedef BOOL (*MASUFINDCHECK)(int id, u32 value, u32 mask);

typedef struct MasuNextWork_s {
    int playerNo;
    int state;
    s16 masuId;
    float angle;
    float scale;
    u8 alpha;
    s16 delay;
    s16 time;
    s16 duration;
} MASUNEXTWORK;

typedef struct MasuPKinokoResult_s {
    u8 id;
    u8 step;
} MASUPKINOKORESULT;

static MASUFINDWORK masuFindWork[MASU_MAX];
static s16 masuFindResult[MASU_MAX];
static u8 masuFindVisit[MASU_MAX];

static int masuLayer;
static s16 masuNum[MASU_LAYER_MAX];
static MASU *masuData[MASU_LAYER_MAX];
static BOOL masuDispF;
static u16 masuDispAttrMask[MASU_LAYER_MAX];
static u32 masuDispMAttrMask[MASU_LAYER_MAX];
static BOOL masuCapsuleDispF;
static BOOL masuCapsuleFadeOnF;
static MASUEVENTHOOK masuev_MasuStart;
static MASUEVENTHOOK masuev_MasuEnd;
static MASUEVENTHOOK masuev_HatenaHook;
static MASUPATHCHECKHOOK masuev_LinkTblHook;
static s16 masuMdlId;
static s16 masuDispCnt;
static ANIMDATA *masuAnimTbl[2];
static void *masuDisplayList;
static u32 masuDisplayListLen;
static void *masuDisplayListKao;
static u32 masuDisplayListKaoLen;
static MASUNEXTWORK *masuNextWork;
static ANIMDATA *masuNextAnim;
static s16 masuNextId;
static HUPROCESS *masuNextProc;
static s16 masuNextMdlId;
static BOOL masuNextDispF;
static s16 masuFindNo;
static s16 masuFindStep;
static s16 masuFindId;
static s16 masuFindResultNum;

extern void *mbMalloc(s32 size);
extern float mbCosDeg(float angle);
extern float mbSinDeg(float angle);
extern s16 mbCapMasuPlayerGet(s16 id);
extern s16 mbCapMasuDispTypeGet(s16 id);
extern void mbPlayerColSnapPlayerSet(int playerNo, BOOL snapF);
extern int mbSingleCall(int mode, int arg);
extern void mbev_StarMasu();
extern int mbev_Shop(int playerNo, int shopNo);
extern void mbCapMasuExec(int playerNo, int id);
extern int mbev_CapCall(int playerNo, int capsuleNo, BOOL moveF, BOOL stopF);
extern void mbPlayerCapCoinMasuExec(int playerNo);
extern void mbPlayerPlusMasuExec(int playerNo);
extern void mbPlayerMinusMasuExec(int playerNo);
extern void mbev_CapCallDonkey(int playerNo);
extern void mbev_CapCallKoopa(int playerNo);
extern void mbev_CapCallKettou(int playerNo, s16 id, BOOL stopF);
extern void mbev_CapCallMiracle(int playerNo, int id);
extern void mbev_SingleMg(int playerNo, s16 id);
static void MasuDispInit(void);
static void MasuDispClose(void);
static u32 MasuDisplayListMake(void **displayList);
static u32 MasuDisplayListKaoMake(void **displayList);
void MasuDraw(HU3D_MODEL *modelP, Mtx *mtx);
static void MasuNextCreate(void);
static void MasuNextMain(void);
static void MasuNextKill(void);
void MasuNextDraw(HU3D_MODEL *modelP, Mtx *mtx);
static int ev_MasuHatena(int playerNo, s16 id);

typedef struct MasuDisp_s {
    int type;
    BOOL dispF;
} MASUDISP;

static int masuFileTbl[] = {
    DATANUM(DATA_bmasu, 0),
    DATANUM(DATA_bmasu, 3),
};

static int masuSingleFileTbl[] = {
    DATANUM(DATA_bmasu, 1),
    DATANUM(DATA_bmasu, 3),
};

static s16 masuPatTbl[] = {
    -1, 0, 1, 2, 6, 7, -2, 5, 8, -1, 9,
};

static s16 masuSinglePatTbl[] = {
    -1, 0, 1, 2, 6, 7, 3, 5, 8, 9, 10, 11, 0,
};

#define DATA_READ16(ptr, dst) do { \
    (dst) = *(u16 *)(ptr); \
    (ptr) = (u8 *)(ptr) + sizeof(u16); \
} while (0)

#define DATA_READLINK(ptr, dst) do { \
    (dst) = *(u16 *)(ptr) + 1; \
    (ptr) = (u8 *)(ptr) + sizeof(u16); \
} while (0)

#define DATA_READ32(ptr, dst) do { \
    (dst) = *(u32 *)(ptr); \
    (ptr) = (u8 *)(ptr) + sizeof(u32); \
} while (0)

#define DATA_READCOUNT(data, ptr, dst) do { \
    (dst) = *(s32 *)(data); \
    (ptr) = (u8 *)(data) + sizeof(s32); \
} while (0)

#define DATA_READVEC(ptr, dst) do { \
    memcpy(&(dst), (ptr), sizeof(HuVecF)); \
    (ptr) = (u8 *)(ptr) + sizeof(HuVecF); \
} while (0)

void mbMasuInit(int dataNum)
{
    int i;

    for (i = 0; i < MASU_LAYER_MAX; i++) {
        masuData[i] = mbMalloc(sizeof(MASU) * MASU_MAX);
    }
    mbMasuLayerSet(MASU_LAYER_DEFAULT);
    for (i = 0; i < MASU_LAYER_MAX; i++) {
        masuDispAttrMask[i] = 0x10;
        masuDispMAttrMask[i] = 0;
    }
    masuev_MasuStart = NULL;
    masuev_MasuEnd = NULL;
    masuev_HatenaHook = NULL;
    masuev_LinkTblHook = NULL;
    masuDispF = TRUE;
    masuCapsuleDispF = TRUE;
    masuCapsuleFadeOnF = FALSE;
    mbMasuDataRead(mbObjDataNumGet(dataNum));
    MasuDispInit();
    mbGateCreate();
    MasuNextCreate();
    HuDataDirClose(DATA_bmasu);
}

void mbMasuClose(void)
{
    int i;

    MasuDispClose();
    MasuNextKill();
    for (i = 0; i < MASU_LAYER_MAX; i++) {
        MASU *masuP = masuData[i];

        HuMemDirectFree(masuP);
        masuData[i] = NULL;
    }
}

BOOL mbMasuDataRead(int dataNum)
{
    MASU *masuP;
    void *ptr;
    int i;
    int j;
    int starNum;
    int *data;

    starNum = 0;
    data = HuDataSelHeapReadNum(dataNum, HU_MEMNUM_OVL, HEAP_MODEL);
    DATA_READCOUNT(data, ptr, masuNum[masuLayer]);

    masuP = &masuData[masuLayer][0];
    masuP->pos.x = masuP->pos.y = masuP->pos.z = 0.0f;
    masuP->pos.z = 100000.0f;
    masuP->rot.x = masuP->rot.y = masuP->rot.z = 0.0f;
    masuP->scale.x = masuP->scale.y = masuP->scale.z = 1.0f;
    PSMTXIdentity(masuP->matrix);
    masuP->linkNum = 0;
    masuP->capsuleNo = MASU_NULL;

    masuP++;
    for (i = 0; i < masuNum[masuLayer]; i++, masuP++) {
        masuP->id = i + 1;
        DATA_READVEC(ptr, masuP->pos);
        DATA_READVEC(ptr, masuP->rot);
        DATA_READVEC(ptr, masuP->scale);
        DATA_READ16(ptr, masuP->flag);
        DATA_READ32(ptr, masuP->mAttr);
        DATA_READ16(ptr, masuP->type);
        masuP->capsuleNo = GwSystem.masuCapsule[masuP->id];
        DATA_READ16(ptr, masuP->linkNum);
        for (j = 0; j < masuP->linkNum; j++) {
            DATA_READLINK(ptr, masuP->linkTbl[j]);
        }
    }
    HuDataClose(data);
    return TRUE;
}

static void MasuDispInit(void)
{
    int i;
    BOOL partyF = GwSystem.partyF;

    if (partyF) {
        for (i = 0; i < 2; i++) {
            masuAnimTbl[i] = HuSprAnimDataRead(masuFileTbl[i]);
            HuSprAnimLock(masuAnimTbl[i]);
        }
    } else {
        for (i = 0; i < 2; i++) {
            masuAnimTbl[i] = HuSprAnimDataRead(masuSingleFileTbl[i]);
            HuSprAnimLock(masuAnimTbl[i]);
        }
    }
    masuDisplayListLen = MasuDisplayListMake(&masuDisplayList);
    masuDisplayListKaoLen = MasuDisplayListKaoMake(&masuDisplayListKao);
    masuMdlId = Hu3DHookFuncCreate(MasuDraw);
    Hu3DModelCameraSet(masuMdlId, HU3D_CAM0);
    Hu3DModelLayerSet(masuMdlId, 2);
}

static void MasuDispClose(void)
{
    int i;

    for (i = 0; i < 2; i++) {
        if (masuAnimTbl[i] != NULL) {
            HuSprAnimKill(masuAnimTbl[i]);
            masuAnimTbl[i] = NULL;
        }
    }
    if (masuDisplayList != NULL) {
        void *displayList = masuDisplayList;

        HuMemDirectFree(displayList);
        masuDisplayList = NULL;
    }
    if (masuMdlId >= 0) {
        Hu3DModelKill(masuMdlId);
        masuMdlId = HU3D_MODELID_NONE;
    }
}

#define MASU_DL_BUF_SIZE 0x1000

static u32 MasuDisplayListMake(void **displayList)
{
    u8 *dlBufRaw = HuMemDirectMallocNum(HEAP_HEAP, MASU_DL_BUF_SIZE, HU_MEMNUM_OVL);
    void *dlBuf = dlBufRaw;
    u8 *displayListP;
    u32 dlSize;

    DCInvalidateRange(dlBuf, MASU_DL_BUF_SIZE);
    GXBeginDisplayList(dlBuf, MASU_DL_BUF_SIZE);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3f32(-100.0f, 0.0f, -100.0f);
    GXTexCoord2f32(0.0f, 0.0f);
    GXPosition3f32(100.0f, 0.0f, -100.0f);
    GXTexCoord2f32(1.0f, 0.0f);
    GXPosition3f32(100.0f, 0.0f, 100.0f);
    GXTexCoord2f32(1.0f, 1.0f);
    GXPosition3f32(-100.0f, 0.0f, 100.0f);
    GXTexCoord2f32(0.0f, 1.0f);
    GXEnd();
    dlSize = GXEndDisplayList();
    displayListP = HuMemDirectMallocNum(HEAP_HEAP, dlSize, HU_MEMNUM_OVL);
    *displayList = displayListP;
    memcpy(*displayList, dlBuf, dlSize);
    DCFlushRange(*displayList, dlSize);
    HuMemDirectFree(dlBuf);
    return dlSize;
}

static u32 MasuDisplayListKaoMake(void **displayList)
{
    u8 *dlBufRaw = HuMemDirectMallocNum(HEAP_HEAP, MASU_DL_BUF_SIZE, HU_MEMNUM_OVL);
    void *dlBuf = dlBufRaw;
    u8 *displayListP;
    u32 dlSize;

    DCInvalidateRange(dlBuf, MASU_DL_BUF_SIZE);
    GXBeginDisplayList(dlBuf, MASU_DL_BUF_SIZE);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3f32(-100.0f, 200.0f, 0.0f);
    GXTexCoord2f32(0.0f, 0.0f);
    GXPosition3f32(100.0f, 200.0f, 0.0f);
    GXTexCoord2f32(1.0f, 0.0f);
    GXPosition3f32(100.0f, 0.0f, 0.0f);
    GXTexCoord2f32(1.0f, 1.0f);
    GXPosition3f32(-100.0f, 0.0f, 0.0f);
    GXTexCoord2f32(0.0f, 1.0f);
    GXEnd();
    dlSize = GXEndDisplayList();
    displayListP = HuMemDirectMallocNum(HEAP_HEAP, dlSize, HU_MEMNUM_OVL);
    *displayList = displayListP;
    memcpy(*displayList, dlBuf, dlSize);
    DCFlushRange(*displayList, dlSize);
    HuMemDirectFree(dlBuf);
    return dlSize;
}

#undef MASU_DL_BUF_SIZE

void MasuDraw(HU3D_MODEL *modelP, Mtx *mtx)
{
    GXColor color = { 0xFF, 0xFF, 0xFF, 0xFF };
    GXColor colorKao = { 0xFF, 0xFF, 0xFF, 0xFF };
    MASU *masuP;
    Mtx modelView;
    Mtx trans;
    Mtx rot;
    Mtx model;
    HuVecF pos;
    Mtx texMtx;
    int i;

    if (!masuDispF) {
        return;
    }

    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    GXInvalidateTexAll();
    HuSprTexLoad(masuAnimTbl[0], 0, GX_TEXMAP0, GX_CLAMP, GX_CLAMP, GX_TRUE);
    HuSprTexLoad(masuAnimTbl[1], 0, GX_TEXMAP1, GX_CLAMP, GX_CLAMP, GX_TRUE);
    HuSprTexLoad(masuAnimTbl[1], 0, GX_TEXMAP2, GX_CLAMP, GX_CLAMP, GX_TRUE);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0,
        GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    GXSetNumTevStages(1);
    GXSetTevOp(GX_TEVSTAGE0, GX_MODULATE);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL,
        GX_DF_CLAMP, GX_AF_SPOT);
    GXSetColorUpdate(GX_TRUE);
    GXSetAlphaUpdate(GX_FALSE);
    GXSetZCompLoc(GX_FALSE);
    GXSetAlphaCompare(GX_GEQUAL, 1, GX_AOP_AND, GX_GEQUAL, 1);
    GXSetCullMode(GX_CULL_BACK);
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA,
        GX_LO_NOOP);

    masuDispCnt = 0;
    masuP = &masuData[masuLayer][1];
    for (i = 0; i < masuNum[masuLayer]; i++, masuP++) {
        int patNo;
        int capPlayerNo;

        if (GWPartyGet() != FALSE) {
            patNo = masuPatTbl[masuP->type];
            if (patNo < 0) {
                BOOL dayF;

                if (patNo == -1) {
                    continue;
                }
                dayF = !GwSystem.curTime;
                patNo = dayF ? 4 : 3;
            }
        } else {
            patNo = masuSinglePatTbl[masuP->type];
            if (patNo < 0) {
                continue;
            }
        }

        mbMasuPosGet(i + 1, &pos);
        if (!mbCameraCullCheck(&pos, 200.0f)) {
            continue;
        }

        GXSetNumTevStages(1);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0,
            GX_COLOR0A0);
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO,
            GX_CC_TEXC);
        GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO,
            GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO,
            GX_CA_TEXA);
        GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO,
            GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);

        if (!masuP->useMtxF) {
            mtxRot(rot, masuP->rot.x, masuP->rot.y, masuP->rot.z);
            PSMTXTrans(trans, masuP->pos.x, masuP->pos.y + 3.0f,
                masuP->pos.z);
            PSMTXConcat(trans, rot, model);
        } else {
            PSMTXCopy(masuP->matrix, model);
        }
        PSMTXConcat(*mtx, model, modelView);
        GXLoadPosMtxImm(modelView, GX_PNMTX0);

        if (GWPartyGet() != FALSE
            && (capPlayerNo = (s16)mbCapMasuPlayerGet(i + 1)) >= 0
            && mbCapMasuDispTypeGet(i + 1) == 1) {
            PSMTXScale(texMtx, 1.0f, 1.0f / 13.0f, 0.0f);
            if (!GWTeamFGet()) {
                mtxTransCat(texMtx, 0.0f,
                    (float)GwPlayer[capPlayerNo].charNo / 13.0f, 0.0f);
            } else {
                int team = GwPlayer[capPlayerNo].team;

                mtxTransCat(texMtx, 0.0f, (float)(team + 11) / 13.0f,
                    0.0f);
            }
            GXLoadTexMtxImm(texMtx, GX_TEXMTX0, GX_MTX2x4);
            GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP1,
                GX_COLOR0A0);
            GXCallDisplayList(masuDisplayList, masuDisplayListLen);
        } else if ((masuP->flag & masuDispAttrMask[masuLayer]) == 0
            && (masuP->mAttr & masuDispMAttrMask[masuLayer]) == 0) {
            PSMTXScale(texMtx, 0.25f, 1.0f / 3.0f, 0.0f);
            mtxTransCat(texMtx, (patNo % 4) / 4.0f,
                (float)(patNo / 4) / 3.0f, 0.0f);
            GXLoadTexMtxImm(texMtx, GX_TEXMTX0, GX_MTX2x4);
            GXCallDisplayList(masuDisplayList, masuDisplayListLen);
        }
        masuDispCnt++;
    }

    if (masuCapsuleDispF == FALSE || GWPartyGet() == FALSE) {
        return;
    }

    masuP = &masuData[masuLayer][1];
    GXSetCullMode(GX_CULL_NONE);
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
    for (i = 0; i < masuNum[masuLayer]; i++, masuP++) {
        int capPlayerNo = (s16)mbCapMasuPlayerGet(i + 1);
        float scale;

        if (capPlayerNo < 0 || mbCapMasuDispTypeGet(i + 1) != 2) {
            continue;
        }
        mbMasuPosGet(i + 1, &pos);
        if (!mbCameraCullCheck(&pos, 200.0f)) {
            continue;
        }

        GXSetNumTevStages(1);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP2,
            GX_COLOR0A0);
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO,
            GX_CC_TEXC);
        GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO,
            GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_TEXA, GX_CA_A1,
            GX_CA_ZERO);
        GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO,
            GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        PSMTXScale(texMtx, 1.0f, 1.0f / 13.0f, 0.0f);
        if (!GWTeamFGet()) {
            mtxTransCat(texMtx, 0.0f,
                (float)GwPlayer[capPlayerNo].charNo / 13.0f, 0.0f);
        } else {
            int team = GwPlayer[capPlayerNo].team;

            mtxTransCat(texMtx, 0.0f, (float)(team + 11) / 13.0f, 0.0f);
        }
        GXLoadTexMtxImm(texMtx, GX_TEXMTX0, GX_MTX2x4);

        if (!masuP->useMtxF) {
            mtxRot(rot, masuP->rot.x, masuP->rot.y, masuP->rot.z);
            PSMTXTrans(trans, masuP->pos.x, masuP->pos.y, masuP->pos.z);
            PSMTXConcat(trans, rot, model);
        } else {
            PSMTXCopy(masuP->matrix, model);
        }

        if (!masuCapsuleFadeOnF) {
            HuVecF nearestDist;
            float minDist = 100000.0f;
            int nearestPlayer = -1;
            int playerNo;

            for (playerNo = 0; playerNo < GW_PLAYER_MAX; playerNo++) {
                mbPlayerPosGet(playerNo, &pos);
                pos.x = abs((int)(pos.x - model[0][3]));
                pos.y = abs((int)(pos.y - model[1][3]));
                pos.z = abs((int)(pos.z - model[2][3]));
                if (minDist > pos.x + pos.y + pos.z) {
                    minDist = pos.x + pos.y + pos.z;
                    nearestDist = pos;
                    nearestPlayer = playerNo;
                }
            }

            colorKao.a = 224;
            scale = 1.0f;
            if (nearestPlayer >= 0) {
                float distance = nearestDist.x;

                if (distance < nearestDist.y) {
                    distance = nearestDist.y;
                }
                if (distance < nearestDist.z) {
                    distance = nearestDist.z;
                }
                if (distance < 250.0f) {
                    distance -= 150.0f;
                    if (distance < 0.0f) {
                        distance = 0.0f;
                    }
                    distance /= 100.0f;
                    colorKao.a = 224.0f - 64.0f * (1.0f - distance);
                    scale = 1.0f + 0.5f * (1.0f - distance);
                }
            }
        } else {
            colorKao.a = 224;
            scale = 1.0f;
        }

        GXSetTevColor(GX_TEVREG1, colorKao);
        PSMTXScale(modelView, scale, scale, scale);
        PSMTXConcat(model, modelView, model);
        PSMTXConcat(*mtx, model, modelView);
        GXLoadPosMtxImm(modelView, GX_PNMTX0);
        GXCallDisplayList(masuDisplayListKao, masuDisplayListKaoLen);
    }
}

static void MasuNextCreate(void)
{
    masuNextProc = HuPrcChildCreate(MasuNextMain, 0x2003, 0x2000, 0, mbMainProc);
    masuNextWork = mbMalloc(sizeof(MASUNEXTWORK) * GW_PLAYER_MAX);
    masuNextMdlId = Hu3DHookFuncCreate(MasuNextDraw);
    Hu3DModelCameraSet(masuNextMdlId, HU3D_CAM0);
    Hu3DModelLayerSet(masuNextMdlId, 2);
    masuNextAnim = HuSprAnimDataRead(DATANUM(DATA_bmasu, 5));
    masuNextDispF = TRUE;
}

static void MasuNextMain(void)
{
    MASUNEXTWORK *work;
    int playerNo;
    int i;

    for (;;) {
        playerNo = GwSystem.turnPlayerNo;
        work = masuNextWork;
        for (i = 0; i < GW_PLAYER_MAX; i++, work++) {
            float time;

            if (work->masuId == 0) {
                continue;
            }
            if (work->delay != 0) {
                work->delay--;
                continue;
            }
            if (work->playerNo != playerNo) {
                work->masuId = 0;
                continue;
            }
            switch (work->state) {
                case 0:
                    time = (float)work->time / work->duration;
                    if (work->time < work->duration) {
                        work->time++;
                    }
                    work->scale = 1.0f
                        + mbCosDeg(90.0f * work->time / work->duration);
                    work->alpha = 192.0f * time;
                    if (masuNextId != work->masuId) {
                        work->state = 1;
                        work->time = 0;
                    }
                    break;
                case 1:
                    time = (float)work->time++ / work->duration;
                    work->alpha = 192.0f * (1.0f - time);
                    if (work->time > work->duration) {
                        work->masuId = 0;
                    }
                    break;
            }
        }
        HuPrcVSleep();
    }
}

static void MasuNextKill(void)
{
    if (masuNextWork != NULL) {
        MASUNEXTWORK *work = masuNextWork;

        HuMemDirectFree(work);
        masuNextWork = NULL;
    }
    if (masuNextMdlId >= 0) {
        Hu3DModelKill(masuNextMdlId);
        masuNextMdlId = HU3D_MODELID_NONE;
    }
    if (masuNextAnim != NULL) {
        HuSprAnimKill(masuNextAnim);
        masuNextAnim = NULL;
    }
}

void MasuNextDraw(HU3D_MODEL *modelP, Mtx *mtx)
{
    MASUNEXTWORK *work;
    Mtx masuMtx;
    Mtx model;
    Mtx rot;
    int dispNum = 0;
    GXColor color = { 0xFF, 0xFF, 0xFF, 0xFF };
    int i;

    if (!masuNextDispF) {
        return;
    }

    HuSprTexLoad(masuNextAnim, 0, GX_TEXMAP0, GX_CLAMP, GX_CLAMP,
        GX_LINEAR);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0,
        GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0,
        GX_COLOR0A0);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO,
        GX_CC_TEXC);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO,
        GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA,
        GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO,
        GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG,
        GX_LIGHT_NULL, GX_DF_CLAMP, GX_AF_SPOT);
    GXSetZCompLoc(GX_FALSE);
    GXSetAlphaCompare(GX_GEQUAL, 1, GX_AOP_AND, GX_GEQUAL, 1);
    GXSetCullMode(GX_CULL_BACK);
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA,
        GX_LO_NOOP);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);

    work = masuNextWork;
    for (i = 0; i < GW_PLAYER_MAX; i++, work++) {
        int j;

        if (work->masuId == 0) {
            continue;
        }
        mbMasuMtxGet(work->masuId, masuMtx);
        color.a = work->alpha;
        GXSetTevColor(GX_TEVREG0, color);
        for (j = 0; j < 1; j++) {
            float angle = (j & 1) ? work->angle : -work->angle;

            PSMTXRotRad(rot, 'Y', 0.017453292f * angle);
            PSMTXConcat(masuMtx, rot, model);
            mtxScaleCat(model, work->scale, 1.0f, work->scale);
            PSMTXConcat(*mtx, model, model);
            GXLoadPosMtxImm(model, GX_PNMTX0);
            GXBegin(GX_QUADS, GX_VTXFMT0, 4);
            GXPosition3f32(-120.00001f, 0.5f, -120.00001f);
            GXTexCoord2f32(0.0f, 0.0f);
            GXPosition3f32(120.00001f, 0.5f, -120.00001f);
            GXTexCoord2f32(1.0f, 0.0f);
            GXPosition3f32(120.00001f, 0.5f, 120.00001f);
            GXTexCoord2f32(1.0f, 1.0f);
            GXPosition3f32(-120.00001f, 0.5f, 120.00001f);
            GXTexCoord2f32(0.0f, 1.0f);
            GXEnd();
        }
        dispNum++;
    }
}

void mbMasuNextSet(s16 id)
{
    MASUNEXTWORK *work = masuNextWork;
    int i;

    if (masuNextId == id) {
        return;
    }
    masuNextId = id;
    if (!mbMasuDispCheck(id)) {
        return;
    }
    for (i = 0; i < GW_PLAYER_MAX; i++, work++) {
        if (work->masuId == 0) {
            break;
        }
    }
    work->masuId = id;
    work->state = 0;
    work->time = 0;
    work->duration = 12;
    work->angle = 0.0f;
    work->playerNo = GwSystem.turnPlayerNo;
    work->delay = 6;
    work->alpha = 0;
}

void mbMasuNextDispSet(BOOL dispF)
{
    masuNextDispF = dispF;
}

int mbev_MasuMove(int playerNo, s16 id)
{
    BOOL partyF = GwSystem.partyF;

    if (partyF) {
        switch (mbMasuTypeGet(id)) {
            case 6:
                break;
            case 7:
                mbev_StarMasu(playerNo, id);
                return TRUE;
            case 9:
                mbev_Shop(playerNo, id);
                break;
            case 8:
                mbCapMasuExec(playerNo, id);
                break;
        }
    } else {
        mbSingleCall(11, id);
        switch (mbMasuTypeGet(id)) {
            case 8:
                mbCapMasuExec(playerNo, id);
                break;
        }
    }
    mbev_CapCall(playerNo, mbMasuCapsuleGet(id), FALSE, TRUE);
    return FALSE;
}

int mbev_MasuCapStop(int playerNo, s16 id)
{
    BOOL result = TRUE;
    BOOL partyF;

    if (_CheckFlag(FLAG_BOARD_TUTORIAL)) {
        mbTutorialCall(10);
    }
    partyF = GwSystem.partyF;
    if (partyF) {
        if (mbMasuPlayerCapStopCheck(playerNo, id) == 0) {
            switch (mbMasuTypeGet(id)) {
                case 0:
                case 1:
                    GwPlayer[playerNo].plusMasuNum++;
                    if (GwPlayer[playerNo].plusMasuNum > 99) {
                        GwPlayer[playerNo].plusMasuNum = 99;
                    }
                    break;
                case 2:
                    GwPlayer[playerNo].minusMasuNum++;
                    if (GwPlayer[playerNo].minusMasuNum > 99) {
                        GwPlayer[playerNo].minusMasuNum = 99;
                    }
                    break;
                case 6:
                    if (!GwSystem.curTime) {
                        GwPlayer[playerNo].donkeyMasuNum++;
                        if (GwPlayer[playerNo].donkeyMasuNum > 99) {
                            GwPlayer[playerNo].donkeyMasuNum = 99;
                        }
                    } else {
                        GwPlayer[playerNo].koopaMasuNum++;
                        if (GwPlayer[playerNo].koopaMasuNum > 99) {
                            GwPlayer[playerNo].koopaMasuNum = 99;
                        }
                    }
                    break;
                case 3:
                    GwPlayer[playerNo].hatenaMasuNum++;
                    if (GwPlayer[playerNo].hatenaMasuNum > 99) {
                        GwPlayer[playerNo].hatenaMasuNum = 99;
                    }
                    break;
                case 4:
                    GwPlayer[playerNo].miracleMasuNum++;
                    if (GwPlayer[playerNo].miracleMasuNum > 99) {
                        GwPlayer[playerNo].miracleMasuNum = 99;
                    }
                    break;
                case 5:
                    GwPlayer[playerNo].kettouMasuNum++;
                    if (GwPlayer[playerNo].kettouMasuNum > 99) {
                        GwPlayer[playerNo].kettouMasuNum = 99;
                    }
                    break;
            }
        } else {
            result = mbev_CapCall(playerNo, mbMasuCapsuleGet(id), FALSE,
                FALSE);
        }
    } else {
        switch (mbMasuTypeGet(id)) {
            case 6:
                GwPlayer[playerNo].koopaMasuNum++;
                if (GwPlayer[playerNo].koopaMasuNum > 99) {
                    GwPlayer[playerNo].koopaMasuNum = 99;
                }
                break;
            case 3:
                GwPlayer[playerNo].hatenaMasuNum++;
                if (GwPlayer[playerNo].hatenaMasuNum > 99) {
                    GwPlayer[playerNo].hatenaMasuNum = 99;
                }
                break;
        }
    }
    if (_CheckFlag(FLAG_BOARD_TUTORIAL)) {
        mbTutorialCall(11);
    }
    return result;
}

int mbev_MasuStop(int playerNo, s16 id)
{
    BOOL result = TRUE;
    BOOL partyF;

    if (_CheckFlag(FLAG_BOARD_TUTORIAL)) {
        mbTutorialCall(12);
    }
    partyF = GwSystem.partyF;
    if (partyF) {
        if (mbMasuPlayerCapStopCheck(playerNo, id) > 0) {
            mbPlayerCapCoinMasuExec(playerNo);
            return TRUE;
        }
        switch (mbMasuTypeGet(id)) {
            case 1:
                mbPlayerPlusMasuExec(playerNo);
                break;
            case 2:
                mbPlayerMinusMasuExec(playerNo);
                break;
            case 6:
                if (!_CheckFlag(FLAG_BOARD_TUTORIAL)) {
                    if (!GwSystem.curTime) {
                        mbev_CapCallDonkey(playerNo);
                    } else {
                        mbev_CapCallKoopa(playerNo);
                    }
                }
                break;
            case 3:
                result = ev_MasuHatena(playerNo, id);
                break;
            case 7:
                mbev_StarMasu(playerNo, id);
                break;
            case 5:
                if (!_CheckFlag(FLAG_BOARD_TUTORIAL)) {
                    mbev_CapCallKettou(playerNo,
                        GwPlayer[playerNo].masuId, TRUE);
                }
                break;
            case 4:
                if (!_CheckFlag(FLAG_BOARD_TUTORIAL)) {
                    mbev_CapCallMiracle(playerNo, id);
                }
                break;
        }
    } else {
        mbSingleCall(7, id);
        switch (mbMasuTypeGet(id)) {
            case 1:
            case 2:
            case 4:
            case 5:
            case 6:
            case 7:
            case 9:
            case 10:
            case 11:
                if (!_CheckFlag(FLAG_BOARD_TUTORIAL)) {
                    mbev_SingleMg(playerNo, id);
                }
                break;
            case 3:
                if (!_CheckFlag(FLAG_BOARD_TUTORIAL)) {
                    result = ev_MasuHatena(playerNo, id);
                }
                break;
        }
    }
    return result;
}

static MASUDISP masuDispTbl[] = {
    { 0, FALSE },
    { 1, TRUE },
    { 2, TRUE },
    { 3, TRUE },
    { 4, TRUE },
    { 5, TRUE },
    { 6, TRUE },
    { 7, FALSE },
    { 8, FALSE },
    { 9, FALSE },
    { 10, FALSE },
};

static MASUDISP masuSingleDispTbl[] = {
    { 0, FALSE },
    { 1, TRUE },
    { 2, TRUE },
    { 3, TRUE },
    { 4, TRUE },
    { 5, TRUE },
    { 6, TRUE },
    { 7, TRUE },
    { 8, FALSE },
    { 9, TRUE },
    { 10, TRUE },
    { 11, TRUE },
};

int mbev_MasuMasuStart(int playerNo)
{
    int result;
    s16 id;
    MASUNEXTWORK *work;
    int i;

    if (masuev_MasuEnd) {
        mbPlayerColSnapPlayerSet(playerNo, FALSE);
        result = masuev_MasuEnd(playerNo, GwPlayer[playerNo].masuId);
        mbPlayerColSnapPlayerSet(playerNo, TRUE);
    } else {
        result = 0;
    }
    id = GwPlayer[playerNo].masuId;
    work = masuNextWork;
    if (masuNextId != id) {
        masuNextId = id;
        if (mbMasuDispCheck(id)) {
            for (i = 0; i < GW_PLAYER_MAX; i++, work++) {
                if (work->masuId == 0) {
                    break;
                }
            }
            work->masuId = id;
            work->state = 0;
            work->time = 0;
            work->duration = 12;
            work->angle = 0.0f;
            work->playerNo = GwSystem.turnPlayerNo;
            work->delay = 6;
            work->alpha = 0;
        }
    }
    return result;
}

int mbev_MasuMasuEnd(int id)
{
    int playerNo = GwSystem.turnPlayerNo;
    int result;
    MASUNEXTWORK *work;
    int i;

    if (masuev_MasuStart) {
        mbPlayerColSnapPlayerSet(playerNo, FALSE);
        result = masuev_MasuStart(playerNo, id);
        mbPlayerColSnapPlayerSet(playerNo, TRUE);
    } else {
        result = 0;
    }
    work = masuNextWork;
    if ((s16)masuNextId != (s16)id) {
        masuNextId = id;
        if (mbMasuDispCheck(id)) {
            for (i = 0; i < GW_PLAYER_MAX; i++, work++) {
                if (work->masuId == 0) {
                    break;
                }
            }
            work->masuId = id;
            work->state = 0;
            work->time = 0;
            work->duration = 12;
            work->angle = 0.0f;
            work->playerNo = GwSystem.turnPlayerNo;
            work->delay = 6;
            work->alpha = 0;
        }
    }
    return result;
}

static int ev_MasuHatena(int playerNo, s16 id)
{
    int result = TRUE;

    if (masuev_HatenaHook) {
        mbAudFXPlay(0x450);
        omVibrate(playerNo, 20, 7, 3);
        mbPlayerColSnapPlayerSet(playerNo, FALSE);
        result = masuev_HatenaHook(playerNo, id);
    }
    return result;
}

#undef DATA_READ16
#undef DATA_READLINK
#undef DATA_READ32
#undef DATA_READCOUNT
#undef DATA_READVEC

int mbMasuNumGet(void)
{
    return masuNum[masuLayer] + 1;
}

int mbMasuRawNumGet(void)
{
    return masuNum[masuLayer];
}

MASU *mbMasuGet(s16 id)
{
    return &masuData[masuLayer][id];
}

void mbMasuLayerSet(int layer)
{
    masuLayer = layer;
}

int mbMasuLayerGet(void)
{
    return masuLayer;
}

u16 mbMasuAttrGet(s16 id)
{
    return masuData[masuLayer][id].flag;
}

void mbMasuAttrSet(s16 id, u16 attr)
{
    masuData[masuLayer][id].flag = attr;
}

u32 mbMasuMAttrGet(s16 id)
{
    return masuData[masuLayer][id].mAttr;
}

void mbMasuMAttrSet(s16 id, u32 attr)
{
    masuData[masuLayer][id].mAttr = attr;
}

u32 mbev_MasuBitGet(u32 outMask, u32 inMask)
{
    u32 bitIn;
    int i;
    u32 result;
    int bitOut;

    outMask &= inMask;
    result = 0;
    bitOut = bitIn = 1;
    for (i = 0; i < 32; i++) {
        if (inMask & bitIn) {
            if (outMask & bitIn) {
                result |= bitOut;
            }
            bitOut <<= 1;
        }
        bitIn <<= 1;
    }
    return result;
}

u32 mbev_MasuAttrGet(int outMask, u32 inMask)
{
    u32 bitIn;
    int i;
    int bitOut;
    u32 result;

    bitIn = bitOut = 1;
    result = 0;
    for (i = 0; i < 32; i++) {
        if (inMask & bitIn) {
            if (outMask & bitOut) {
                result |= bitIn;
            }
            bitOut <<= 1;
        }
        bitIn <<= 1;
    }
    return result;
}

int mbMasuTypeGet(s16 id)
{
    return masuData[masuLayer][id].type;
}

void mbMasuTypeSet(s16 id, int type)
{
    masuData[masuLayer][id].type = type;
}

void mbMasuTypeChange(u16 oldType, u16 newType)
{
    int i;
    MASU *masuP = &masuData[masuLayer][1];

    for (i = 0; i < masuNum[masuLayer]; i++) {
        if (masuP->type == oldType) {
            masuP->type = newType;
            mbMasuCapsuleSet(i + 1, MASU_NULL);
        }
    }
}

BOOL mbMasuDispCheck(s16 id)
{
    MASU *masuP = &masuData[masuLayer][id];
    BOOL partyF = GwSystem.partyF;
    int i;

    if (partyF) {
        for (i = 0; i < 12; i++) {
            if (masuP->type == masuDispTbl[i].type) {
                break;
            }
        }
        return masuDispTbl[i].dispF;
    }
    for (i = 0; i < 12; i++) {
        if (masuP->type == masuSingleDispTbl[i].type) {
            break;
        }
    }
    return masuSingleDispTbl[i].dispF;
}

void mbMasuCapsuleReset(void)
{
    int i;

    for (i = 0; i < MASU_MAX; i++) {
        GwSystem.masuCapsule[i] = MASU_NULL;
    }
}

int mbMasuCapsuleGet(s16 id)
{
    return masuData[masuLayer][id].capsuleNo;
}

void mbMasuCapsuleSet(s16 id, int capsuleNo)
{
    GwSystem.masuCapsule[id] = capsuleNo;
    masuData[masuLayer][id].capsuleNo = capsuleNo;
}

void mbMasuPosGet(s16 id, HuVecF *pos)
{
    MASU *masuP = &masuData[masuLayer][id];

    if (!masuP->useMtxF) {
        *pos = masuP->pos;
    } else {
        pos->x = masuP->matrix[0][3];
        pos->y = masuP->matrix[1][3];
        pos->z = masuP->matrix[2][3];
    }
}

void mbMasuPosSet(s16 id, float x, float y, float z)
{
    MASU *masuP = &masuData[masuLayer][id];

    masuP->pos.x = x;
    masuP->pos.y = y;
    masuP->pos.z = z;
}

void mbMasuPosSetV(s16 id, HuVecF *pos)
{
    mbMasuPosSet(id, pos->x, pos->y, pos->z);
}

#define MASU_CORNER_MAX 8

typedef struct MasuCorner_s {
    int no;
    int order;
} MASUCORNER;

void mbMasuCornerPosGet(s16 id, int cornerNo, HuVecF *pos)
{
    MASU *masuP = mbMasuGet(id);
    MASUCORNER cornerTbl[MASU_CORNER_MAX];
    s16 linkTbl[MASU_LINK_MAX * 2];
    int cornerNum[MASU_CORNER_MAX];
    HuVecF posMasu;
    HuVecF posLink;
    HuVecF linkDir;
    int i;
    int j;
    int linkNum;
    static int cornerNoTbl[MASU_CORNER_MAX] = {
        3, 1,
        7, 5,
        4, 2,
        0, 6,
    };

    for (linkNum = 0, i = 0; i < masuP->linkNum; i++) {
        s32 link = masuP->linkTbl[i];

        linkTbl[linkNum++] = link;
    }
    linkNum += mbMasuLinkParentGet(id, &linkTbl[linkNum]);
    for (i = 0; i < MASU_CORNER_MAX; i++) {
        cornerNum[i] = 0;
    }
    {
        MASU *masuPosP = &masuData[masuLayer][id];

        if (!masuPosP->useMtxF) {
            posMasu = masuPosP->pos;
        } else {
            posMasu.x = masuPosP->matrix[0][3];
            posMasu.y = masuPosP->matrix[1][3];
            posMasu.z = masuPosP->matrix[2][3];
        }
    }
    for (i = 0; i < linkNum; i++) {
        s32 cornerIdx;
        s16 link = linkTbl[i];
        MASU *linkP = &masuData[masuLayer][link];
        float linkAngle;

        if (!linkP->useMtxF) {
            posLink = linkP->pos;
        } else {
            posLink.x = linkP->matrix[0][3];
            posLink.y = linkP->matrix[1][3];
            posLink.z = linkP->matrix[2][3];
        }
        VECSubtract(&posLink, &posMasu, &linkDir);
        linkAngle = HuAtan(-linkDir.z, linkDir.x);
        if (linkAngle < 0.0f) {
            linkAngle += 360.0f;
        }
        cornerIdx = (s16)((linkAngle + 22.5f) / 45.0f);
        cornerNum[cornerIdx]++;
    }
    for (i = 0; i < MASU_CORNER_MAX; i++) {
        cornerTbl[i].no = cornerNoTbl[i];
        cornerTbl[i].order = cornerNum[cornerTbl[i].no];
    }
    for (i = 0; i < MASU_CORNER_MAX - 1; i++) {
        for (j = i; j < MASU_CORNER_MAX; j++) {
            if (cornerTbl[j].order < cornerTbl[i].order) {
                MASUCORNER temp = cornerTbl[i];

                cornerTbl[i] = cornerTbl[j];
                cornerTbl[j] = temp;
            }
        }
    }
    pos->x = mbCosDeg((cornerTbl[cornerNo].no * 360.0f) / 8.0f) * 100.0f;
    pos->y = 0.0f;
    pos->z = -mbSinDeg((cornerTbl[cornerNo].no * 360.0f) / 8.0f) * 100.0f;
}

#undef MASU_CORNER_MAX

void mbMasuCornerRotPosGet(s16 id, int cornerNo, HuVecF *pos)
{
    HuVecF posMasu;
    HuVecF posCorner;
    HuVecF rot;
    MASU *masuP;

    mbMasuCornerPosGet(id, cornerNo, &posCorner);
    masuP = &masuData[masuLayer][id];
    if (!masuP->useMtxF) {
        posMasu = masuP->pos;
    } else {
        posMasu.x = masuP->matrix[0][3];
        posMasu.y = masuP->matrix[1][3];
        posMasu.z = masuP->matrix[2][3];
    }
    mbMasuRotGet(id, &rot);
    pos->x = ((posCorner.x * mbCosDeg(rot.z)) + posMasu.x
        + (posCorner.y * mbSinDeg(rot.z)));
    pos->y = (posCorner.z * mbSinDeg(-rot.x))
        + (posMasu.y + (posCorner.x * mbSinDeg(rot.z))
        + (posCorner.y * (mbCosDeg(rot.x) * mbCosDeg(rot.z))));
    pos->z = ((posCorner.y * mbSinDeg(rot.x)) + posMasu.z
        + (posCorner.z * mbCosDeg(rot.x)));
}

void mbMasuRotGet(s16 id, HuVecF *rot)
{
    MASU *masuP = &masuData[masuLayer][id];

    if (masuP->useMtxF) {
        Hu3DMtxRotGet(masuP->matrix, rot);
    } else {
        *rot = masuP->rot;
    }
}

void mbMasuRotSet(s16 id, float x, float y, float z)
{
    MASU *masuP = &masuData[masuLayer][id];

    masuP->rot.x = x;
    masuP->rot.y = y;
    masuP->rot.z = z;
}

void mbMasuRotSetV(s16 id, HuVecF *rot)
{
    mbMasuRotSet(id, rot->x, rot->y, rot->z);
}

void mbMasuMtxGet(s16 id, Mtx matrix)
{
    MASU *masuP = &masuData[masuLayer][id];

    if (masuP->useMtxF) {
        PSMTXCopy(masuP->matrix, matrix);
    } else {
        Mtx rot;
        Mtx trans;

        mtxRot(rot, masuP->rot.x, masuP->rot.y, masuP->rot.z);
        PSMTXTrans(trans, masuP->pos.x, masuP->pos.y, masuP->pos.z);
        PSMTXConcat(trans, rot, matrix);
    }
}

void mbMasuMtxSet(s16 id, Mtx matrix)
{
    MASU *masuP = &masuData[masuLayer][id];

    if (matrix != NULL) {
        masuP->useMtxF = TRUE;
        PSMTXCopy(matrix, masuP->matrix);
    } else {
        masuP->useMtxF = FALSE;
    }
}

BOOL mbMasuDispGet(void)
{
    return masuDispF;
}

void mbMasuDispSet(BOOL dispF)
{
    masuDispF = dispF;
}

void mbMasuDispAttrSet(u16 attr)
{
    masuDispAttrMask[masuLayer] |= attr;
}

void mbMasuDispAttrReset(u16 attr)
{
    masuDispAttrMask[masuLayer] &= ~attr;
}

u16 mbMasuDispAttrGet(void)
{
    return masuDispAttrMask[masuLayer];
}

void mbMasuDispMAttrSet(u32 attr)
{
    masuDispMAttrMask[masuLayer] |= attr;
}

void mbMasuDispMAttrReset(u32 attr)
{
    masuDispMAttrMask[masuLayer] &= ~attr;
}

u32 mbMasuDispMAttrGet(void)
{
    return masuDispMAttrMask[masuLayer];
}

void mbMasuModelDispSet(BOOL dispF)
{
    if (dispF) {
        Hu3DModelDispOn(masuMdlId);
    } else {
        Hu3DModelDispOff(masuMdlId);
    }
}

s16 mbMasuLinkGet(s16 id, int linkNo)
{
    MASU *masuP = &masuData[masuLayer][id];

    return masuP->linkTbl[linkNo];
}

s16 mbMasuLinkNumGet(s16 id)
{
    MASU *masuP = &masuData[masuLayer][id];

    return masuP->linkNum;
}

int mbMasuLinkTblGet(s16 id, s16 *linkTbl)
{
    return mbMasuLinkTblGet2(id, linkTbl, FALSE);
}

int mbMasuLinkTblGet2(s16 id, s16 *linkTbl, BOOL hookF)
{
    MASU *masuP = &masuData[masuLayer][id];
    int linkNum;
    int i;

    if (masuev_LinkTblHook != NULL && hookF) {
        linkNum = masuev_LinkTblHook(id, masuP->mAttr, linkTbl, hookF);
        if (linkNum > 0) {
            return linkNum;
        }
    }
    linkNum = 0;
    for (i = 0; i < masuP->linkNum; i++) {
        MASU *linkMasuP = mbMasuGet(masuP->linkTbl[i]);

        if ((linkMasuP->flag & mbBranchAttrGet()) != 0
            || (linkMasuP->mAttr & mbBranchMAttrGet()) != 0) {
            continue;
        }
        if (linkTbl) {
            linkTbl[linkNum] = masuP->linkTbl[i];
        }
        linkNum++;
    }
    return linkNum;
}

s16 mbMasuAttrFindLink(s16 id, u16 attr)
{
    MASU *masuP = &masuData[masuLayer][id];
    int i;

    for (i = 0; i < masuP->linkNum; i++) {
        MASU *linkMasuP = mbMasuGet(masuP->linkTbl[i]);

        if ((linkMasuP->flag & attr) != 0) {
            return masuP->linkTbl[i];
        }
    }
    return MASU_NULL;
}

s16 mbMasuAttrMatchFindLink(s16 id, u16 attr, u16 mask)
{
    MASU *masuP = &masuData[masuLayer][id];
    int i;

    for (i = 0; i < masuP->linkNum; i++) {
        MASU *linkMasuP = mbMasuGet(masuP->linkTbl[i]);

        if (attr == (linkMasuP->flag & mask)) {
            return masuP->linkTbl[i];
        }
    }
    return MASU_NULL;
}

s16 mbMasuMAttrFindLink(s16 id, u32 attr)
{
    MASU *masuP = &masuData[masuLayer][id];
    int i;

    for (i = 0; i < masuP->linkNum; i++) {
        MASU *linkMasuP = mbMasuGet(masuP->linkTbl[i]);

        if ((linkMasuP->mAttr & attr) != 0) {
            return masuP->linkTbl[i];
        }
    }
    return MASU_NULL;
}

s16 mbMasuMAttrMatchFindLink(s16 id, u32 attr, u32 mask)
{
    MASU *masuP = &masuData[masuLayer][id];
    int i;

    for (i = 0; i < masuP->linkNum; i++) {
        MASU *linkMasuP = mbMasuGet(masuP->linkTbl[i]);

        if (attr == (linkMasuP->mAttr & mask)) {
            return masuP->linkTbl[i];
        }
    }
    return MASU_NULL;
}

s16 mbMasuTypeFindLink(s16 id, int type)
{
    MASU *masuP = &masuData[masuLayer][id];
    int i;

    for (i = 0; i < masuP->linkNum; i++) {
        MASU *linkMasuP = mbMasuGet(masuP->linkTbl[i]);

        if (linkMasuP->type == type) {
            return masuP->linkTbl[i];
        }
    }
    return MASU_NULL;
}

s16 mbMasuLinkParentGet(s16 id, s16 *linkTbl)
{
    MASU *masuP;
    int i;
    int j;
    int linkNum = 0;

    for (masuP = &masuData[masuLayer][1], i = 0;
         i < masuNum[masuLayer];
         i++, masuP++) {
        for (j = 0; j < masuP->linkNum; j++) {
            if (masuP->linkTbl[j] == id) {
                linkTbl[linkNum++] = i + 1;
            }
        }
    }
    return linkNum;
}

static BOOL MasuTypeCheck(int id, u32 type, u32 unused)
{
    return mbMasuTypeGet(id) == type;
}

static BOOL MasuAttrCheck(int id, u32 attr, u32 unused)
{
    return mbMasuAttrGet(id) & attr;
}

static BOOL MasuMAttrCheck(int id, u32 attr, u32 unused)
{
    return mbMasuMAttrGet(id) & attr;
}

static BOOL MasuAttrMatchCheck(int id, u32 mask, u32 attr)
{
    return (mbMasuAttrGet(id) & mask) == attr;
}

static BOOL MasuMAttrMatchCheck(int id, u32 mask, u32 attr)
{
    return (mbMasuMAttrGet(id) & mask) == attr;
}

static BOOL MasuIdCheck(int id, u32 targetId, u32 unused)
{
    return id == targetId;
}

static void MasuFind(s16 id, MASUFINDCHECK check, u32 value, u32 mask,
    BOOL hookF, BOOL dispF)
{
    s16 linkTbl[MASU_LINK_MAX];
    s16 workNo = 0;
    s16 startId = id;
    int linkNo;
    int linkNum;
    int i;

    masuFindNo = 0;
    masuFindStep = 9999;
    masuFindId = MASU_NULL;
    memset(masuFindVisit, 0, sizeof(masuFindVisit));
    masuFindResultNum = 0;
    linkNo = 0;

    while (TRUE) {
        masuFindVisit[id] = TRUE;
        if (check(id, value, mask)) {
            if (dispF && id != startId && !mbMasuDispCheck(id)) {
                masuFindNo++;
            }
            if (masuFindNo < masuFindStep) {
                masuFindStep = masuFindNo;
                masuFindId = id;
                for (i = 0; i < workNo; i++) {
                    masuFindResult[i] = masuFindWork[i].id;
                }
                masuFindResult[i] = id;
                masuFindResultNum = workNo + 1;
            }
        } else {
            linkNum = mbMasuLinkTblGet2(id, linkTbl, hookF);
            while (linkNo < linkNum) {
                if (!masuFindVisit[linkTbl[linkNo]]) {
                    break;
                }
                linkNo++;
            }
            if (linkNo < linkNum) {
                masuFindWork[workNo].id = id;
                masuFindWork[workNo].linkNo = linkNo + 1;
                workNo++;
                id = linkTbl[linkNo];
                linkNo = 0;
                if (!dispF) {
                    masuFindNo++;
                } else if (mbMasuDispCheck(id)) {
                    masuFindNo++;
                }
                continue;
            }
        }

        masuFindVisit[id] = FALSE;
        workNo--;
        if (workNo < 0) {
            break;
        }
        if (!dispF) {
            masuFindNo--;
        } else if (mbMasuDispCheck(id)) {
            masuFindNo--;
        }
        id = masuFindWork[workNo].id;
        linkNo = masuFindWork[workNo].linkNo;
    }
}

int mbMasuFind_TypeStepGet(s16 id, s16 type)
{
    MasuFind(id, MasuTypeCheck, type, 0, FALSE, TRUE);
    return masuFindStep;
}

int mbMasuFind_TypeStepGet2(s16 id, s16 type, BOOL hookF, BOOL dispF)
{
    MasuFind(id, MasuTypeCheck, type, 0, hookF, dispF);
    return masuFindStep;
}

int mbMasuFind_AttrStepGet(s16 id, u16 attr)
{
    MasuFind(id, MasuAttrCheck, attr, 0, FALSE, TRUE);
    return masuFindStep;
}

int mbMasuFind_AttrMatchStepGet(s16 id, u16 arg1, u16 arg2)
{
    MasuFind(id, MasuAttrMatchCheck, arg1, arg2, FALSE, TRUE);
    return masuFindStep;
}

int mbMasuFind_AttrStepGet2(s16 id, u16 arg1, u16 arg2, BOOL hookF,
    BOOL dispF)
{
    MasuFind(id, MasuAttrMatchCheck, arg1, arg2, hookF, dispF);
    return masuFindStep;
}

int mbMasuFind_MAttrStepGet(s16 id, u32 attr)
{
    MasuFind(id, MasuMAttrCheck, attr, 0, FALSE, TRUE);
    return masuFindStep;
}

int mbMasuFind_MAttrMatchStepGet(s16 id, u32 arg1, u32 arg2)
{
    MasuFind(id, MasuMAttrMatchCheck, arg1, arg2, FALSE, TRUE);
    return masuFindStep;
}

int mbMasuFind_MAttrStepGet2(s16 id, u32 arg1, u32 arg2, BOOL hookF,
    BOOL dispF)
{
    MasuFind(id, MasuMAttrMatchCheck, arg1, arg2, hookF, dispF);
    return masuFindStep;
}

int mbMasuFind_IdStepGet(s16 id, s16 targetId)
{
    MasuFind(id, MasuIdCheck, targetId, 0, FALSE, TRUE);
    return masuFindStep;
}

int mbMasuFind_IdStepGet2(s16 id, s16 targetId, BOOL hookF, BOOL dispF)
{
    MasuFind(id, MasuIdCheck, targetId, 0, hookF, dispF);
    return masuFindStep;
}

s16 mbMasuFind_TypeSearch(s16 id, s16 type)
{
    return mbMasuFind_TypeIdGet(id, type, FALSE, TRUE);
}

s16 mbMasuFind_TypeIdGet(s16 id, s16 type, BOOL hookF, BOOL dispF)
{
    int i;
    MASU *masuP;

    if (id >= 0) {
        MasuFind(id, MasuTypeCheck, type, 0, hookF, dispF);
        return masuFindId;
    }
    masuP = &masuData[masuLayer][1];
    for (i = 0; i < masuNum[masuLayer]; i++, masuP++) {
        if (masuP->type == type) {
            return i + 1;
        }
    }
    return MASU_NULL;
}

s16 mbMasuFind_AttrIdGet(s16 id, u16 attr)
{
    int i;
    MASU *masuP;

    if (id >= 0) {
        MasuFind(id, MasuAttrCheck, attr, 0, FALSE, TRUE);
        return masuFindId;
    }
    masuP = &masuData[masuLayer][1];
    for (i = 0; i < masuNum[masuLayer]; i++, masuP++) {
        if (masuP->flag & attr) {
            return i + 1;
        }
    }
    return MASU_NULL;
}

s16 mbMasuFind_AttrMatchIdGet(s16 id, u16 arg1, u16 arg2)
{
    return mbMasuFind_AttrMatchIdGet2(id, arg1, arg2, FALSE, TRUE);
}

s16 mbMasuFind_AttrMatchIdGet2(s16 id, u16 arg1, u16 arg2, BOOL hookF,
    BOOL dispF)
{
    int i;
    MASU *masuP;

    if (id >= 0) {
        MasuFind(id, MasuAttrMatchCheck, arg1, arg2, hookF, dispF);
        return masuFindId;
    }
    masuP = &masuData[masuLayer][1];
    for (i = 0; i < masuNum[masuLayer]; i++, masuP++) {
        if (arg1 == (masuP->flag & arg2)) {
            return i + 1;
        }
    }
    return MASU_NULL;
}

s16 mbMasuFind_MAttrIdGet(s16 id, u32 attr)
{
    int i;
    MASU *masuP;

    if (id >= 0) {
        MasuFind(id, MasuMAttrCheck, attr, 0, FALSE, TRUE);
        return masuFindId;
    }
    masuP = &masuData[masuLayer][1];
    for (i = 0; i < masuNum[masuLayer]; i++, masuP++) {
        if (masuP->mAttr & attr) {
            return i + 1;
        }
    }
    return MASU_NULL;
}

s16 mbMasuFind_MAttrMatchIdGet(s16 id, u32 arg1, u32 arg2)
{
    return mbMasuFind_MAttrMatchIdGet2(id, arg1, arg2, FALSE, TRUE);
}

s16 mbMasuFind_MAttrMatchIdGet2(s16 id, u32 arg1, u32 arg2, BOOL hookF,
    BOOL dispF)
{
    int i;
    MASU *masuP;

    if (id >= 0) {
        MasuFind(id, MasuMAttrMatchCheck, arg1, arg2, hookF, dispF);
        return masuFindId;
    }
    masuP = &masuData[masuLayer][1];
    for (i = 0; i < masuNum[masuLayer]; i++, masuP++) {
        if (arg1 == (masuP->mAttr & arg2)) {
            return i + 1;
        }
    }
    return MASU_NULL;
}

int mbMasuTypeListGet(s16 type, s16 *list)
{
    int i;
    int num = 0;
    MASU *masuP = &masuData[masuLayer][1];

    for (i = 0; i < masuNum[masuLayer]; i++, masuP++) {
        if (masuP->type == type) {
            if (list != NULL) {
                list[num] = i + 1;
            }
            num++;
        }
    }
    return num;
}

int mbMasuAttrListGet(u16 attr, s16 *list)
{
    int i;
    int num = 0;
    MASU *masuP = &masuData[masuLayer][1];

    for (i = 0; i < masuNum[masuLayer]; i++, masuP++) {
        if ((masuP->flag & attr) != 0) {
            if (list != NULL) {
                list[num] = i + 1;
            }
            num++;
        }
    }
    return num;
}

int mbMasuAttrMatchListGet(u16 attr, u16 mask, s16 *list)
{
    int i;
    int num = 0;
    MASU *masuP = &masuData[masuLayer][1];

    for (i = 0; i < masuNum[masuLayer]; i++, masuP++) {
        if (attr == (masuP->flag & mask)) {
            if (list != NULL) {
                list[num] = i + 1;
            }
            num++;
        }
    }
    return num;
}

int mbMasuMAttrListGet(u32 attr, s16 *list)
{
    int i;
    int num = 0;
    MASU *masuP = &masuData[masuLayer][1];

    for (i = 0; i < masuNum[masuLayer]; i++, masuP++) {
        if ((masuP->mAttr & attr) != 0) {
            if (list != NULL) {
                list[num] = i + 1;
            }
            num++;
        }
    }
    return num;
}

int mbMasuMAttrMatchTblGet(u32 attr, u32 mask, s16 *list)
{
    int i;
    int num = 0;
    MASU *masuP = &masuData[masuLayer][1];

    for (i = 0; i < masuNum[masuLayer]; i++, masuP++) {
        if (attr == (masuP->mAttr & mask)) {
            if (list != NULL) {
                list[num] = i + 1;
            }
            num++;
        }
    }
    return num;
}

int mbMasuFind_TypeListGet(s16 id, s16 type, s16 *list)
{
    return mbMasuFind_TypeListGet2(id, type, FALSE, TRUE, list);
}

int mbMasuFind_TypeListGet2(s16 id, s16 type, BOOL hookF, BOOL dispF,
    s16 *list)
{
    MasuFind(id, MasuTypeCheck, type, 0, hookF, dispF);
    memcpy(list, masuFindResult, masuFindResultNum * sizeof(s16));
    return masuFindResultNum;
}

int mbMasuFind_AttrNumGet(s16 id, u16 attr, u32 unused)
{
    return mbMasuFind_AttrMatchNumGet2(id, attr, attr, FALSE, TRUE, unused);
}

int mbMasuFind_AttrMatchNumGet(s16 id, u16 arg1, u16 arg2, u32 unused)
{
    return mbMasuFind_AttrMatchNumGet2(id, arg1, arg2, FALSE, TRUE, unused);
}

int mbMasuFind_AttrMatchNumGet2(s16 id, u16 arg1, u16 arg2, BOOL hookF,
    BOOL dispF, u32 unused)
{
    MasuFind(id, MasuAttrMatchCheck, arg1, arg2, hookF, dispF);
    return masuFindResultNum;
}

s16 mbMasuFind_MAttrNumGet(s16 id, u32 attr)
{
    s16 result;

    if (id >= 0) {
        MasuFind(id, MasuMAttrMatchCheck, attr, attr, FALSE, TRUE);
        result = masuFindId;
    } else {
        MASU *masuP = &masuData[masuLayer][1];
        int i;

        for (i = 0; i < masuNum[masuLayer]; i++, masuP++) {
            if ((masuP->mAttr & attr) == attr) {
                result = i + 1;
                goto done;
            }
        }
        result = MASU_NULL;
    }
done:
    return result;
}

int mbMasuFind_MAttrMatchListGet(s16 id, u32 arg1, u32 arg2, s16 *list)
{
    return mbMasuFind_MAttrMatchListGet2(id, arg1, arg2, FALSE, TRUE, list);
}

int mbMasuFind_MAttrMatchListGet2(s16 id, u32 arg1, u32 arg2, BOOL hookF,
    BOOL dispF, s16 *list)
{
    MasuFind(id, MasuMAttrMatchCheck, arg1, arg2, hookF, dispF);
    memcpy(list, masuFindResult, masuFindResultNum * sizeof(s16));
    return masuFindResultNum;
}

int mbMasuFind_IdListGet(s16 id, s16 targetId, s16 *list)
{
    return mbMasuFind_IdListGet2(id, targetId, FALSE, TRUE, list);
}

int mbMasuFind_IdListGet2(s16 id, s16 targetId, BOOL hookF, BOOL dispF,
    s16 *list)
{
    MasuFind(id, MasuIdCheck, targetId, 0, hookF, dispF);
    memcpy(list, masuFindResult, masuFindResultNum * sizeof(s16));
    return masuFindResultNum;
}

void mbev_MasuMoveEndSet(MASUEVENTHOOK hook)
{
    masuev_MasuEnd = hook;
}

void mbev_MasuMoveStartSet(MASUEVENTHOOK hook)
{
    masuev_MasuStart = hook;
}

void mbev_MasuHatenaSet(MASUEVENTHOOK hook)
{
    masuev_HatenaHook = hook;
}

void mbev_MasuLinkTblHookSet(MASUPATHCHECKHOOK hook)
{
    masuev_LinkTblHook = hook;
}

int mbMasuPlayerCapMoveCheck(int playerNo, s16 id)
{
    int capPlayerNo = mbCapMasuPlayerGet(id);

    if (capPlayerNo < 0) {
        return 0;
    }
    if (mbCapMasuDispTypeGet(id) == 2) {
        if (capPlayerNo == playerNo
            || (GWTeamFGet() && capPlayerNo == mbPlayerTeamFind(playerNo))) {
            return 1;
        }
        return -1;
    }
    return 0;
}

int mbMasuPlayerCapStopCheck(int playerNo, s16 id)
{
    int capPlayerNo = mbCapMasuPlayerGet(id);

    if (capPlayerNo < 0) {
        return 0;
    }
    if (mbCapMasuDispTypeGet(id) == 1) {
        if (capPlayerNo == playerNo
            || (GWTeamFGet() && capPlayerNo == mbPlayerTeamFind(playerNo))) {
            return 1;
        }
        return -1;
    }
    return 0;
}

void mbMasuPlayerColorSet(int playerNo)
{
    int colorTbl[][2] = {
        { 1, STATUS_COLOR_BLUE },
        { 2, STATUS_COLOR_RED },
        { 6, STATUS_COLOR_GRAY },
        { 3, STATUS_COLOR_GREEN },
        { 5, STATUS_COLOR_GREEN },
        { 4, STATUS_COLOR_GREEN },
        { 8, STATUS_COLOR_GREEN },
        { -1, STATUS_COLOR_NULL },
    };
    int type;
    BOOL partyF;
    s16 id;
    int capResult;
    int i;
    int color;

    partyF = GwSystem.partyF;
    if (!partyF) {
        return;
    }
    id = GwPlayer[playerNo].masuId;
    capResult = mbMasuPlayerCapStopCheck(playerNo, id);
    if (capResult == 0) {
        type = mbMasuTypeGet(id);
        for (i = 0; colorTbl[i][0] >= 0; i++) {
            if (type == colorTbl[i][0]) {
                break;
            }
        }
        if (colorTbl[i][0] < 0) {
            mbStatusColorSet(playerNo, STATUS_COLOR_GRAY);
            return;
        }
        if (colorTbl[i][0] == 6) {
            BOOL dayF;

            dayF = !GwSystem.curTime;
            color = dayF ? STATUS_COLOR_BLUE : STATUS_COLOR_RED;
        } else {
            color = colorTbl[i][1];
        }
    } else {
        if (capResult > 0) {
            color = STATUS_COLOR_PURPLE;
        } else {
            color = STATUS_COLOR_PURPLE;
        }
    }
    mbStatusColorSet(playerNo, color);
}

void mbMasuPlayerDispSet(BOOL dispF)
{
    masuCapsuleDispF = dispF;
}

void mbMasuPlayerFadeSet(BOOL fadeF)
{
    masuCapsuleFadeOnF = fadeF;
}

void mbMasuPlayerPrizeReset(int playerNo)
{
    GwPlayer[playerNo].plusMasuNum = 0;
    GwPlayer[playerNo].minusMasuNum = 0;
    GwPlayer[playerNo].capsuleMasuNum = 0;
    GwPlayer[playerNo].hatenaMasuNum = 0;
    GwPlayer[playerNo].koopaMasuNum = 0;
    GwPlayer[playerNo].miracleMasuNum = 0;
    GwPlayer[playerNo].kettouMasuNum = 0;
    GwPlayer[playerNo].donkeyMasuNum = 0;
}

int mbMasuPKinokoValueGet(int playerNo, s16 id)
{
    s16 linkTbl[MASU_LINK_MAX];
    u8 resultVisit[MASU_MAX];
    u8 candidateTbl[MASU_MAX];
    MASUPKINOKORESULT resultTbl[MASU_MAX];
    int linkNo;
    s16 workNo;
    s16 startId;
    s16 resultNum;
    s16 candidateNum;
    int linkNum;
    int type;
    int i;

    workNo = 0;
    startId = id;
    resultNum = 0;
    candidateNum = 0;
    if (mbRandMod(100) < 50) {
        return 9;
    }

    masuFindNo = 0;
    masuFindStep = 9999;
    masuFindId = MASU_NULL;
    memset(masuFindVisit, 0, sizeof(masuFindVisit));
    memset(resultVisit, 0, sizeof(resultVisit));
    resultVisit[id] = TRUE;
    masuFindResultNum = 0;
    linkNo = 0;

    while (TRUE) {
        masuFindVisit[id] = TRUE;
        if (masuFindNo <= 10) {
            linkNum = mbMasuLinkTblGet2(id, linkTbl, FALSE);
            while (linkNo < linkNum) {
                if (!masuFindVisit[linkTbl[linkNo]]) {
                    break;
                }
                linkNo++;
            }
            if (linkNo < linkNum) {
                masuFindWork[workNo].id = id;
                masuFindWork[workNo].linkNo = linkNo + 1;
                workNo++;
                if (mbMasuDispCheck(id)) {
                    if (!resultVisit[id]) {
                        resultTbl[resultNum].id = id;
                        resultTbl[resultNum].step = masuFindNo;
                        resultNum++;
                        resultVisit[id] = TRUE;
                    } else if (resultNum != 0) {
                        for (i = 0; i < resultNum; i++) {
                            if (id == resultTbl[i].id) {
                                break;
                            }
                        }
                        if (resultTbl[i].step > masuFindNo) {
                            resultTbl[i].step = masuFindNo;
                        }
                    }
                }
                id = linkTbl[linkNo];
                linkNo = 0;
                if (mbMasuDispCheck(id)) {
                    masuFindNo++;
                }
                continue;
            }
        }

        masuFindVisit[id] = FALSE;
        workNo--;
        if (workNo < 0) {
            break;
        }
        if (mbMasuDispCheck(id)) {
            masuFindNo--;
        }
        id = masuFindWork[workNo].id;
        linkNo = masuFindWork[workNo].linkNo;
    }

    for (i = 0; i < resultNum; i++) {
        type = mbMasuTypeGet(resultTbl[i].id);
        if (type == 3 || type == 4 || type == 5
            || (!GwSystem.curTime && type == 6)) {
            candidateTbl[candidateNum++] = i;
        }
    }
    if (candidateNum == 0) {
        for (i = 0; i < resultNum; i++) {
            if (mbCapMasuPlayerGet(resultTbl[i].id) == playerNo
                && mbCapMasuDispTypeGet(resultTbl[i].id) == 1) {
                candidateTbl[candidateNum++] = i;
            }
        }
    }
    if (candidateNum == 0) {
        return 9;
    }

    i = mbRandMod(candidateNum);
    mbPlayerWorkGet(playerNo)->masuNext = resultTbl[candidateTbl[i]].id;
    return resultTbl[candidateTbl[i]].step - 1;
}

int mbMasuStub(void)
{
    return 0;
}
