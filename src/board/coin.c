#define _MATH_H
#include "dolphin/math.h"

#include "game/board/audio.h"
#include "game/board/coin.h"
#include "game/board/effect.h"
#include "game/board/main.h"
#include "game/board/object.h"

#include "game/data.h"
#include "game/gamework.h"
#include "game/hu3d.h"
#include "game/memory.h"
#include "game/object.h"
#include "game/process.h"
#include "game/sprite.h"

#include "string.h"

#define COIN_OBJ_BANK_MAX 64
#define COIN_OBJ_BANK_SIZE 64
#define COIN_EFF_MAX 64
#define COIN_MODEL_MAX 14

#define COIN_OBJ_ID_MASK 0x0FFF
#define COIN_OBJ_ID_BASE 0x4000

#define COIN_OBJ_ATTR_USED 0x00000001
#define COIN_OBJ_ATTR_DISP 0x00100000

typedef struct MbCoinObjBank_s {
    int count;
    u32 attr[COIN_OBJ_BANK_SIZE];
    s8 motNo[COIN_OBJ_BANK_SIZE];
    MBCOINOBJ obj[COIN_OBJ_BANK_SIZE];
} MBCOINOBJBANK;

typedef struct MbCoinObjData_s {
    int hookModelId[3];
    s16 modelId[14];
    HSF_OBJECT *modelObj[14];
    MBCOINOBJBANK *bank[COIN_OBJ_BANK_MAX];
} MBCOINOBJDATA;

typedef struct CoinDispWork_s {
    unsigned killF : 1;
    unsigned sign : 1;
    unsigned mode : 3;
    u8 modelNum;
    s16 no;
    u16 delay;
    u16 time;
    u16 maxTime;
} COINDISPWORK;

typedef struct CoinEffData_s {
    HuVecF pos;
    s16 modelId;
    s16 count;
} COINEFFDATA;

extern void mbMtxRotYDeg(Mtx mtx, float angle);
extern void mbMtxRotZDeg(Mtx mtx, float angle);
extern void mbMtxScaleRotXDeg(Mtx mtx, float angle, HuVecF *scale);
extern float mbSinDeg(float angle);
extern float mbCosDeg(float angle);

static void CoinInit(void);
static void CoinClose(void);
static void CoinDraw(HU3D_MODEL *modelP, Mtx *mtxP);
static void CoinMain(void);
static void CoinEffCreate(int no, HuVecF *pos);
static void CoinEffHook(HU3D_MODEL *modelP, MBPARTICLE *particleP, Mtx mtx);

static OMOBJ *coinDispOMObj[GW_PLAYER_MAX + 1] = {};

static int numberFileTbl[] = {
    0x0005000B,
    0x0005000C,
    0x0005000D,
    0x0005000E,
    0x0005000F,
    0x00050010,
    0x00050011,
    0x00050012,
    0x00050013,
    0x00050014,
    0x00050015,
    0x00050016,
};

static int coinObjFileTbl[] = {
    0x00050003,
    0x00050004,
    0x0005000B,
    0x0005000C,
    0x0005000D,
    0x0005000E,
    0x0005000F,
    0x00050010,
    0x00050011,
    0x00050012,
    0x00050013,
    0x00050014,
    0x00050015,
    0x00050016,
};

static char *coinObjNameTbl[] = {
    "Rcoin-coin",
    "coin",
    "num0-no_0",
    "num1-no_1",
    "num2-no_2",
    "num3-no_3",
    "num4-no_4",
    "num5-no_5",
    "num6-no_6",
    "num7-no_7",
    "num8-no_8",
    "num9-no_9",
    "plus",
    "plus-minus",
};

static GXColor coinEffColorTbl[] = {
    { 220, 220, 64, 120 },
    { 220, 220, 220, 120 },
    { 64, 160, 220, 140 },
    { 220, 140, 140, 140 },
};

static u8 coinEffColorNoTbl[] = {
    0, 1, 0, 1, 0, 1, 0, 1,
    0, 0, 2, 3, 2, 3, 2, 3,
};

static u8 coinEffBankTbl[] = {
    2, 2, 2, 2, 2, 3, 3, 3,
    2, 2, 2, 2, 2, 3, 3, 3,
};

static u8 coinEffBankTbl2[] = {
    2, 2, 2, 2, 2, 3, 3, 3,
    2, 3, 2, 3, 2, 3, 0, 1,
};

static COINEFFDATA coinEffData[COIN_EFF_MAX];
static MBCOINOBJDATA coinObjData;
static int coin1MdlId;
static int coin2MdlId;
static HUPROCESS *coinMdlProc;

void mbCoinInit(void)
{
    memset(coinDispOMObj, 0, sizeof(coinDispOMObj));
    memset(coinEffData, 0, sizeof(coinEffData));
    coin1MdlId = mbObjCreate(mbBoardDataNumGet(0x00050003), NULL, TRUE);
    coin2MdlId = mbObjCreate(mbBoardDataNumGet(0x00050004), NULL, TRUE);
    mbObjDispSet(coin1MdlId, FALSE);
    mbObjDispSet(coin2MdlId, FALSE);
    CoinInit();
    coinMdlProc = HuPrcChildCreate(CoinMain, 8206, 8192, 0, mbMainProc);
}

void mbCoinClose(void)
{
    int i;

    CoinClose();
    for (i = 0; i < COIN_EFF_MAX; i++) {
        if (coinEffData[i].modelId > 0) {
            mbParticleKill(coinEffData[i].modelId);
            coinEffData[i].modelId = 0;
        }
    }
    if (coin1MdlId > 0) {
        mbObjKill(coin1MdlId);
        coin1MdlId = 0;
    }
    if (coin2MdlId > 0) {
        mbObjKill(coin2MdlId);
        coin2MdlId = 0;
    }
    HuPrcKill(coinMdlProc);
}

void mbCoinEffObjCreate(int modelId)
{
    HuVecF pos;

    mbObjPosGet(modelId, &pos);
    mbCoinEffCreate(&pos);
    mbObjKill(modelId);
}

static void CoinInit(void)
{
    int i;
    s16 modelId;

    for (i = 0; i < COIN_MODEL_MAX; i++) {
        modelId = mbObjCreate(mbBoardDataNumGet(coinObjFileTbl[i]), NULL, FALSE);
        coinObjData.modelId[i] = modelId;
        coinObjData.modelObj[i] = Hu3DModelObjPtrGet(mbObjModelIDGet(modelId), coinObjNameTbl[i]);
        mbObjDispSet(modelId, FALSE);
        if (i >= 2) {
            mbObjMotionSet(modelId, 0, HU3D_MOTATTR_NONE);
            mbObjMotionTimeSet(modelId, 0.5f);
            mbObjMotionSpeedSet(modelId, 0.0f);
        }
    }
    for (i = 0; i < COIN_OBJ_BANK_MAX; i++) {
        coinObjData.bank[i] = NULL;
    }
    coinObjData.hookModelId[0] = Hu3DHookFuncCreate(CoinDraw);
    Hu3DModelCameraSet(coinObjData.hookModelId[0], HU3D_CAM0);
    Hu3DModelLayerSet(coinObjData.hookModelId[0], 5);
    coinObjData.hookModelId[1] = Hu3DHookFuncCreate(CoinDraw);
    Hu3DModelCameraSet(coinObjData.hookModelId[1], HU3D_CAM1);
    Hu3DModelLayerSet(coinObjData.hookModelId[1], 5);
    coinObjData.hookModelId[2] = Hu3DHookFuncCreate(CoinDraw);
    Hu3DModelCameraSet(coinObjData.hookModelId[2], HU3D_CAM2);
    Hu3DModelLayerSet(coinObjData.hookModelId[2], 0);
}

static void CoinClose(void)
{
    MBCOINOBJBANK *bankP;
    int i;

    Hu3DModelKill(coinObjData.hookModelId[0]);
    coinObjData.hookModelId[0] = -1;
    Hu3DModelKill(coinObjData.hookModelId[1]);
    coinObjData.hookModelId[1] = -1;
    Hu3DModelKill(coinObjData.hookModelId[2]);
    coinObjData.hookModelId[2] = -1;
    for (i = 0; i < COIN_OBJ_BANK_MAX; i++) {
        if (coinObjData.bank[i] != NULL) {
            bankP = coinObjData.bank[i];
            HuMemDirectFree(bankP);
            coinObjData.bank[i] = NULL;
        }
    }
    for (i = 0; i < COIN_MODEL_MAX; i++) {
        mbObjKill(coinObjData.modelId[i]);
        coinObjData.modelId[i] = 0;
    }
}

static void CoinDraw(HU3D_MODEL *modelP, Mtx *mtxP)
{
    float motTimeTbl[] = { 0.5f, 1.5f, 2.5f };
    HU3D_MODEL *modelData[COIN_MODEL_MAX];
    int modelId[COIN_MODEL_MAX];
    int motNo[COIN_MODEL_MAX];
    int alpha[COIN_MODEL_MAX];
    Mtx mtx;
    MBCOINOBJBANK **bankPP;
    MBCOINOBJ *objP;
    u32 *attrP;
    u16 cameraBit;
    int bankNo;
    int objNo;
    int modelNo;
    int alphaVal;
    s8 motion;
    int i;

    cameraBit = modelP->cameraBit;
    Hu3DModelObjDrawInit();
    for (i = 0; i < COIN_MODEL_MAX; i++) {
        alpha[i] = -1;
        motNo[i] = -1;
        modelId[i] = mbObjModelIDGet(coinObjData.modelId[i]);
        modelData[i] = &Hu3DData[modelId[i]];
    }
    for (bankPP = &coinObjData.bank[0], bankNo = 0;
         bankNo < COIN_OBJ_BANK_MAX;
         bankNo++, bankPP++) {
        if (*bankPP != NULL && (*bankPP)->count != 0) {
            attrP = &(*bankPP)->attr[0];
            for (objNo = 0; objNo < COIN_OBJ_BANK_SIZE; objNo++, attrP++) {
                if (*attrP != 0
                    && (*attrP & cameraBit) != 0
                    && (*attrP & COIN_OBJ_ATTR_DISP) != 0) {
                    objP = &(*bankPP)->obj[objNo];
                    if (objP->alpha > 0.0f
                        && objP->scale.x != 0.0f
                        && objP->scale.y != 0.0f
                        && objP->scale.z != 0.0f) {
                        if (objP->rot.x != 0.0f) {
                            mbMtxScaleRotXDeg(mtx, objP->rot.x, &objP->scale);
                        } else {
                            PSMTXScale(mtx, objP->scale.x, objP->scale.y, objP->scale.z);
                        }
                        if (objP->rot.y != 0.0f) {
                            mbMtxRotYDeg(mtx, objP->rot.y);
                        }
                        if (objP->rot.z != 0.0f) {
                            mbMtxRotZDeg(mtx, objP->rot.z);
                        }
                        mtx[0][3] = objP->pos.x;
                        mtx[1][3] = objP->pos.y;
                        mtx[2][3] = objP->pos.z;
                        PSMTXConcat(*mtxP, mtx, mtx);
                        modelNo = (*attrP & 0x000F0000) >> 16;
                        alphaVal = 255.0f * objP->alpha;
                        if (alphaVal != alpha[modelNo]) {
                            alpha[modelNo] = alphaVal;
                            mbObjAlphaSet(coinObjData.modelId[modelNo], alphaVal);
                        }
                        if (modelNo >= 2) {
                            motion = (*bankPP)->motNo[objNo];
                            if (motion != motNo[modelNo]) {
                                motNo[modelNo] = motion;
                                Hu3DMotionExec(modelId[modelNo], modelData[modelNo]->motId,
                                    motTimeTbl[motion], FALSE);
                            }
                        }
                        Hu3DModelObjPtrDraw(modelId[modelNo], coinObjData.modelObj[modelNo], mtx);
                    }
                }
            }
        }
    }
}

s16 mbCoinCreate(void)
{
    MBCOINOBJBANK *bankP;
    MBCOINOBJ *objP;
    int bankNo;
    int objNo;

    for (bankNo = 0; bankNo < COIN_OBJ_BANK_MAX; bankNo++) {
        if (!coinObjData.bank[bankNo]) {
            bankP = HuMemDirectMallocNum(HEAP_HEAP, sizeof(MBCOINOBJBANK), HU_MEMNUM_OVL);
            bankP->count = 0;
            memset(bankP->attr, 0, sizeof(bankP->attr));
            memset(bankP->motNo, 0, sizeof(bankP->motNo));
            coinObjData.bank[bankNo] = bankP;
            break;
        }
        if (coinObjData.bank[bankNo]->count < COIN_OBJ_BANK_SIZE) {
            break;
        }
    }
    bankP = coinObjData.bank[bankNo];
    for (objNo = 0; objNo < COIN_OBJ_BANK_SIZE; objNo++) {
        if (!bankP->attr[objNo]) {
            break;
        }
    }
    bankP->count++;
    bankP->attr[objNo] = COIN_OBJ_ATTR_DISP | COIN_OBJ_ATTR_USED;
    bankP->motNo[objNo] = 0;
    objP = &bankP->obj[objNo];
    memset(objP, 0, sizeof(MBCOINOBJ));
    objP->scale.x = objP->scale.y = objP->scale.z = 1.0f;
    objP->alpha = 1.0f;
    objNo |= (bankNo << 6) | COIN_OBJ_ID_BASE;
    return objNo;
}

s16 mbCoinCreate2(void)
{
    MBCOINOBJBANK *bankP;
    MBCOINOBJ *objP;
    int bankNo;
    int objNo;

    for (bankNo = 0; bankNo < COIN_OBJ_BANK_MAX; bankNo++) {
        if (!coinObjData.bank[bankNo]) {
            bankP = HuMemDirectMallocNum(HEAP_HEAP, sizeof(MBCOINOBJBANK), HU_MEMNUM_OVL);
            bankP->count = 0;
            memset(bankP->attr, 0, sizeof(bankP->attr));
            memset(bankP->motNo, 0, sizeof(bankP->motNo));
            coinObjData.bank[bankNo] = bankP;
            break;
        }
        if (coinObjData.bank[bankNo]->count < COIN_OBJ_BANK_SIZE) {
            break;
        }
    }
    bankP = coinObjData.bank[bankNo];
    for (objNo = 0; objNo < COIN_OBJ_BANK_SIZE; objNo++) {
        if (!bankP->attr[objNo]) {
            break;
        }
    }
    bankP->count++;
    bankP->attr[objNo] = (1 << 16) | COIN_OBJ_ATTR_DISP | COIN_OBJ_ATTR_USED;
    bankP->motNo[objNo] = 0;
    objP = &bankP->obj[objNo];
    memset(objP, 0, sizeof(MBCOINOBJ));
    objP->scale.x = objP->scale.y = objP->scale.z = 1.0f;
    objP->alpha = 1.0f;
    objNo |= (bankNo << 6) | COIN_OBJ_ID_BASE;
    return objNo;
}

s16 mbCoinObjCreate(int modelNo, int motNo)
{
    MBCOINOBJBANK *bankP;
    MBCOINOBJ *objP;
    int bankNo;
    int objNo;
    s16 objId;

    for (bankNo = 0; bankNo < COIN_OBJ_BANK_MAX; bankNo++) {
        if (!coinObjData.bank[bankNo]) {
            bankP = HuMemDirectMallocNum(HEAP_HEAP, sizeof(MBCOINOBJBANK), HU_MEMNUM_OVL);
            bankP->count = 0;
            memset(bankP->attr, 0, sizeof(bankP->attr));
            memset(bankP->motNo, 0, sizeof(bankP->motNo));
            coinObjData.bank[bankNo] = bankP;
            break;
        }
        if (coinObjData.bank[bankNo]->count < COIN_OBJ_BANK_SIZE) {
            break;
        }
    }
    bankP = coinObjData.bank[bankNo];
    for (objNo = 0; objNo < COIN_OBJ_BANK_SIZE; objNo++) {
        if (!bankP->attr[objNo]) {
            break;
        }
    }
    bankP->count++;
    bankP->attr[objNo] = ((modelNo + 2) << 16) | COIN_OBJ_ATTR_DISP | COIN_OBJ_ATTR_USED;
    bankP->motNo[objNo] = 0;
    objP = &bankP->obj[objNo];
    memset(objP, 0, sizeof(MBCOINOBJ));
    objP->scale.x = objP->scale.y = objP->scale.z = 1.0f;
    objP->alpha = 1.0f;
    objNo |= (bankNo << 6) | COIN_OBJ_ID_BASE;
    objId = objNo;
    mbCoinObjMotSet(objId, motNo);
    return objId;
}

s16 mbCoinObjCreate2(int modelNo, int motNo)
{
    MBCOINOBJBANK *bankP;
    MBCOINOBJ *objP;
    int bankNo;
    int objNo;
    s16 objId;

    for (bankNo = 0; bankNo < COIN_OBJ_BANK_MAX; bankNo++) {
        if (!coinObjData.bank[bankNo]) {
            bankP = HuMemDirectMallocNum(HEAP_HEAP, sizeof(MBCOINOBJBANK), HU_MEMNUM_OVL);
            bankP->count = 0;
            memset(bankP->attr, 0, sizeof(bankP->attr));
            memset(bankP->motNo, 0, sizeof(bankP->motNo));
            coinObjData.bank[bankNo] = bankP;
            break;
        }
        if (coinObjData.bank[bankNo]->count < COIN_OBJ_BANK_SIZE) {
            break;
        }
    }
    bankP = coinObjData.bank[bankNo];
    for (objNo = 0; objNo < COIN_OBJ_BANK_SIZE; objNo++) {
        if (!bankP->attr[objNo]) {
            break;
        }
    }
    bankP->count++;
    bankP->attr[objNo] = ((modelNo + 12) << 16) | COIN_OBJ_ATTR_DISP | COIN_OBJ_ATTR_USED;
    bankP->motNo[objNo] = 0;
    objP = &bankP->obj[objNo];
    memset(objP, 0, sizeof(MBCOINOBJ));
    objP->scale.x = objP->scale.y = objP->scale.z = 1.0f;
    objP->alpha = 1.0f;
    objNo |= (bankNo << 6) | COIN_OBJ_ID_BASE;
    objId = objNo;
    mbCoinObjMotSet(objId, motNo);
    return objId;
}

MBCOINOBJ *mbCoinObjGet(s16 objId)
{
    MBCOINOBJBANK *bankP;
    int bankNo;
    int objNo;

    objId &= COIN_OBJ_ID_MASK;
    bankNo = objId >> 6;
    objNo = objId & 0x3F;
    bankP = coinObjData.bank[bankNo];
    return &bankP->obj[objNo];
}

void mbCoinObjNumDec(s16 objId)
{
    MBCOINOBJBANK *bankP;
    int bankNo;
    int objNo;

    objId &= COIN_OBJ_ID_MASK;
    bankNo = objId >> 6;
    objNo = objId & 0x3F;
    bankP = coinObjData.bank[bankNo];
    bankP->attr[objNo] = 0;
    bankP->count--;
}

void mbCoinObjKill(s16 objId)
{
    MBCOINOBJBANK *bankP;
    MBCOINOBJ *objP;
    int bankNo;
    int objNo;

    objId &= COIN_OBJ_ID_MASK;
    bankNo = objId >> 6;
    objNo = objId & 0x3F;
    bankP = coinObjData.bank[bankNo];
    objP = &bankP->obj[objNo];
    mbCoinEffCreate(&objP->pos);
    bankP->attr[objNo] = 0;
    bankP->count--;
}

void mbCoinObjPosSet(s16 objId, float x, float y, float z)
{
    MBCOINOBJ *objP = mbCoinObjGet(objId);

    objP->pos.x = x;
    objP->pos.y = y;
    objP->pos.z = z;
}

void mbCoinObjPosSetV(s16 objId, HuVecF *pos)
{
    mbCoinObjPosSet(objId, pos->x, pos->y, pos->z);
}

void mbCoinObjPosGet(s16 objId, HuVecF *pos)
{
    MBCOINOBJ *objP = mbCoinObjGet(objId);

    *pos = objP->pos;
}

void mbCoinObjRotSet(s16 objId, float x, float y, float z)
{
    MBCOINOBJ *objP = mbCoinObjGet(objId);

    objP->rot.x = x;
    objP->rot.y = y;
    objP->rot.z = z;
}

void mbCoinObjRotSetV(s16 objId, HuVecF *rot)
{
    mbCoinObjRotSet(objId, rot->x, rot->y, rot->z);
}

void mbCoinObjRotGet(s16 objId, HuVecF *rot)
{
    MBCOINOBJ *objP = mbCoinObjGet(objId);

    *rot = objP->rot;
}

void mbCoinObjScaleSet(s16 objId, float x, float y, float z)
{
    MBCOINOBJ *objP = mbCoinObjGet(objId);

    objP->scale.x = x;
    objP->scale.y = y;
    objP->scale.z = z;
}

void mbCoinObjScaleSetV(s16 objId, HuVecF *scale)
{
    mbCoinObjScaleSet(objId, scale->x, scale->y, scale->z);
}

void mbCoinObjScaleGet(s16 objId, HuVecF *scale)
{
    MBCOINOBJ *objP = mbCoinObjGet(objId);

    *scale = objP->scale;
}

void mbCoinObjAlphaSet(s16 objId, float alpha)
{
    MBCOINOBJ *objP = mbCoinObjGet(objId);

    objP->alpha = alpha;
}

float mbCoinObjAlphaGet(s16 objId)
{
    MBCOINOBJ *objP = mbCoinObjGet(objId);

    return objP->alpha;
}

void mbCoinObjDispSet(s16 objId, BOOL dispF)
{
    MBCOINOBJBANK *bankP;
    int bankNo;
    int objNo;

    objId &= COIN_OBJ_ID_MASK;
    bankNo = objId >> 6;
    objNo = objId & 0x3F;
    bankP = coinObjData.bank[bankNo];
    bankP->attr[objNo] &= ~COIN_OBJ_ATTR_DISP;
    if (dispF) {
        bankP->attr[objNo] |= COIN_OBJ_ATTR_DISP;
    }
}

BOOL mbCoinObjDispGet(s16 objId)
{
    MBCOINOBJBANK *bankP;
    int bankNo;
    int objNo;

    objId &= COIN_OBJ_ID_MASK;
    bankNo = objId >> 6;
    objNo = objId & 0x3F;
    bankP = coinObjData.bank[bankNo];
    return (bankP->attr[objNo] & COIN_OBJ_ATTR_DISP) == COIN_OBJ_ATTR_DISP;
}

void mbCoinObjLayerSet(s16 objId, u16 layer)
{
    MBCOINOBJBANK *bankP;
    int bankNo;
    int objNo;

    objId &= COIN_OBJ_ID_MASK;
    bankNo = objId >> 6;
    objNo = objId & 0x3F;
    bankP = coinObjData.bank[bankNo];
    bankP->attr[objNo] &= 0xFFFF0000;
    bankP->attr[objNo] |= layer;
}

void mbCoinObjMotSet(s16 objId, int motNo)
{
    MBCOINOBJBANK *bankP;
    int bankNo;
    int objNo;

    objId &= COIN_OBJ_ID_MASK;
    bankNo = objId >> 6;
    objNo = objId & 0x3F;
    bankP = coinObjData.bank[bankNo];
    bankP->motNo[objNo] = motNo;
}

static void CoinMain(void)
{
    HuVecF pos = { 0.0f, 0.0f, 0.0f };
    MBCOINOBJBANK *bankP;
    MBCOINOBJBANK **bankPP;
    COINEFFDATA *effP;
    int i;
    int bankNo;
    int num;
    int no;

    for (i = 0; i < 5; i++) {
        CoinEffCreate(i, &pos);
    }
    for (bankNo = 0, num = 0; num < 2; bankNo++) {
        if (coinObjData.bank[bankNo] == NULL) {
            bankP = HuMemDirectMallocNum(HEAP_HEAP, sizeof(MBCOINOBJBANK), HU_MEMNUM_OVL);
            bankP->count = 0;
            memset(bankP->attr, 0, sizeof(bankP->attr));
            memset(bankP->motNo, 0, sizeof(bankP->motNo));
            coinObjData.bank[bankNo] = bankP;
            num++;
        }
    }
    while (TRUE) {
        num = 0;
        for (effP = &coinEffData[i], no = i; no < COIN_EFF_MAX; no++, effP++) {
            if (effP->modelId != 0) {
                num++;
                if (effP->count == 0) {
                    mbParticleKill(effP->modelId);
                    effP->modelId = 0;
                }
            }
        }
        num = 0;
        for (bankPP = &coinObjData.bank[bankNo], no = bankNo;
             no < COIN_OBJ_BANK_MAX;
             no++, bankPP++) {
            if (*bankPP != NULL && (*bankPP)->count == 0) {
                bankP = *bankPP;
                HuMemDirectFree(bankP);
                *bankPP = NULL;
            }
        }
        HuPrcVSleep();
    }
}

void mbCoinEffCreate(HuVecF *pos)
{
    COINEFFDATA *effP;
    MBPARTICLE *particleP;
    MBPARTICLEDATA *particleDataP;
    HuVecF offset;
    float angle;
    float angleY;
    float cosY;
    int effNo;
    int remaining;
    int colorNo;
    int i;

    mbAudFXPlay(7);
    effNo = -1;
    for (effP = &coinEffData[0], i = 0; i < COIN_EFF_MAX; i++, effP++) {
        if (effP->modelId != 0) {
            if (effP->count == 0) {
                effNo = i;
                break;
            }
            if (effP->count <= 80
                && fabs(pos->x - effP->pos.x) < 300.0f
                && fabs(pos->y - effP->pos.y) < 300.0f
                && fabs(pos->z - effP->pos.z) < 300.0f) {
                effNo = i;
                break;
            }
        } else if (effNo < 0) {
            effNo = i;
            break;
        }
    }
    if (effNo < 0) {
        return;
    }
    CoinEffCreate(effNo, pos);
    effP = &coinEffData[effNo];
    particleP = Hu3DData[effP->modelId].hookData;
    VECSubtract(pos, &effP->pos, &offset);
    remaining = 20;
    effP->count += remaining;
    for (particleDataP = particleP->data, i = 0;
         i < particleP->num && remaining != 0;
         i++, particleDataP++) {
        if (particleDataP->time == 0) {
            colorNo = coinEffColorNoTbl[mbRandMod(16)];
            particleDataP->color.r = coinEffColorTbl[colorNo].r + mbRandMod(20);
            particleDataP->color.g = coinEffColorTbl[colorNo].g + mbRandMod(20);
            particleDataP->color.b = coinEffColorTbl[colorNo].b + mbRandMod(20);
            particleDataP->color.a = coinEffColorTbl[colorNo].a + mbRandMod(20);
            particleDataP->animBank = coinEffBankTbl[mbRandMod(32)];
            particleDataP->pos.x = particleDataP->pos.y = particleDataP->pos.z = 0.0f;
            angle = 360.0f * frandf();
            angleY = (1.7f * frandf()) - 0.7f;
            angleY = 90.0f * (angleY * fabs(angleY));
            cosY = mbCosDeg(angleY);
            particleDataP->vel.x = mbSinDeg(angle) * cosY;
            particleDataP->vel.y = mbSinDeg(angleY);
            cosY = mbCosDeg(angleY);
            particleDataP->vel.z = mbCosDeg(angle) * cosY;
            VECScale(&particleDataP->vel, &particleDataP->pos,
                100.0f * (0.5f * frandf()));
            VECScale(&particleDataP->vel, &particleDataP->vel,
                (1.0f / 60.0f) * (300.0f + (100.0f * (4.0f * frandf()))));
            VECAdd(&particleDataP->pos, &offset, &particleDataP->pos);
            particleDataP->speedDecay = (1.0f / 60.0f)
                * (100.0f * (0.7f * (0.2f + (0.3f * frandf()))));
            particleDataP->colorIdx = 90.0f + angle;
            particleDataP->scale = 70.0f * (0.5f + (0.5f * frandf()));
            particleDataP->accel.x = -1.3f;
            particleDataP->rot.z = 360.0f * frandf();
            particleDataP->scaleBase = (20.0f * frandf()) - 10.0f;
            particleDataP->time = mbRandMod(10) + 20;
            particleDataP->accel.y = 0.44444448f;
            particleDataP->accel.z = 0.92f;
            remaining--;
        }
    }
}

static void CoinEffCreate(int no, HuVecF *pos)
{
    COINEFFDATA *effP;
    MBPARTICLE *particleP;
    MBPARTICLEDATA *particleDataP;
    int i;

    effP = &coinEffData[no];
    if (effP->modelId <= 0) {
        effP->modelId = mbParticleCreate(
            HuSprAnimRead(HuDataSelHeapReadNum(mbBoardDataNumGet(0x00050065), HU_MEMNUM_OVL, HEAP_MODEL)),
            100);
        mbParticleHookSet(effP->modelId, CoinEffHook);
        Hu3DModelLayerSet(effP->modelId, 5);
        effP->pos = *pos;
        Hu3DModelPosSetV(effP->modelId, pos);
        effP->count = 0;
        particleP = Hu3DData[effP->modelId].hookData;
        for (particleDataP = particleP->data, i = 0;
             i < particleP->num;
             i++, particleDataP++) {
            particleDataP->scale = 0.0f;
            particleDataP->color.a = 0;
            particleDataP->time = 0;
        }
        particleP->attr = MB_PARTICLE_ATTR_LOOP;
        particleP->time = no;
    } else if (effP->count == 0) {
        effP->pos = *pos;
        Hu3DModelPosSetV(effP->modelId, pos);
    }
}

static void CoinEffHook(HU3D_MODEL *modelP, MBPARTICLE *particleP, Mtx mtx)
{
    MBPARTICLEDATA *particleDataP;
    int count;
    int num;
    int i;

    count = 0;
    num = particleP->num;
    for (particleDataP = particleP->data, i = 0;
         i < particleP->num;
         i++, particleDataP++) {
        if (particleDataP->time != 0) {
            count++;
            VECScale(&particleDataP->vel, &particleDataP->vel, particleDataP->accel.z);
            VECAdd(&particleDataP->pos, &particleDataP->vel, &particleDataP->pos);
            particleDataP->vel.y += particleDataP->accel.y;
            particleDataP->vel.x += particleDataP->speedDecay * mbSinDeg(particleDataP->colorIdx);
            particleDataP->vel.z += particleDataP->speedDecay * mbCosDeg(particleDataP->colorIdx);
            particleDataP->colorIdx += 5.0f;
            particleDataP->speedDecay += 0.02777778f;
            particleDataP->scale += particleDataP->accel.x;
            particleDataP->rot.z += particleDataP->scaleBase;
            particleDataP->time--;
            if (particleDataP->time < 10) {
                particleDataP->color.a = particleDataP->color.a * 0.8f;
                if (particleDataP->time == 0) {
                    particleDataP->color.a = 0;
                    particleDataP->scale = 0.0f;
                    count--;
                }
            }
        }
    }
    coinEffData[particleP->time].count = count;
}

void mbCoinDispKill(s16 no)
{
    if (no <= 0 || no > GW_PLAYER_MAX) {
        return;
    }
    if (coinDispOMObj[no]) {
        COINDISPWORK *work = omObjGetWork(coinDispOMObj[no], COINDISPWORK);

        work->killF = TRUE;
    }
}

BOOL mbCoinDispKillCheck(s16 no)
{
    if (no <= 0 || no > GW_PLAYER_MAX) {
        return TRUE;
    }
    if (coinDispOMObj[no]) {
        COINDISPWORK *work = omObjGetWork(coinDispOMObj[no], COINDISPWORK);

        return FALSE;
    }
    return TRUE;
}
