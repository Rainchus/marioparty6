#define _MATH_H
#include "dolphin/math.h"

#include "game/board/audio.h"
#include "game/board/camera.h"
#include "game/board/coin.h"
#include "game/board/effect.h"
#include "game/board/main.h"
#include "game/board/object.h"
#include "game/board/player.h"

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

#define COINDISP_MODE_ON 0
#define COINDISP_MODE_MAIN 1
#define COINDISP_MODE_NONE 2
#define COINDISP_MODE_OFF 3
#define COINDISP_MODEL_MAX 5

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

typedef struct coinDispModel_s {
    HuVecF pos;
    HuVecF scale;
    float rotY;
} COINDISPMODEL;

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
extern void mbPos3Dto2D(HuVecF *src, HuVecF *dst);
extern void mbPos2Dto3D(HuVecF *src, HuVecF *dst);

static void CoinInit(void);
static void CoinClose(void);
static void CoinDraw(HU3D_MODEL *modelP, Mtx *mtxP);
static void CoinMain(void);
static void CoinEffCreate(int no, HuVecF *pos);
static void CoinEffHook(HU3D_MODEL *modelP, MBPARTICLE *particleP, Mtx mtx);
static void CoinDispCreate(OMOBJ *obj, int coinNum);
static void CoinDispOMExec(OMOBJ *obj);
static void CoinDispObjUpdate(OMOBJ *obj);
static void CoinDispOn(OMOBJ *obj);
static void CoinDispMain(OMOBJ *obj);
static void CoinDispOff(OMOBJ *obj);
static void CoinDispObjKill(OMOBJ *obj);
static void CoinAddAllProc(int *addNum, BOOL fastF, int *result);

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
    int modelId;

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
                particleDataP->color.a *= 0.8f;
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

s16 mbCoinDispCreate(HuVecF *pos, int coinNum, int sign, BOOL playSe)
{
    OMOBJ *obj;
    COINDISPWORK *work;
    s8 i;

    for (i = 1; i < GW_PLAYER_MAX + 1; i++) {
        if (!coinDispOMObj[i]) {
            break;
        }
    }
    if (i >= GW_PLAYER_MAX + 1) {
        return -1;
    }
    if (coinNum > 999) {
        coinNum = 999;
    } else if (coinNum < -999) {
        coinNum = -999;
    }
    obj = omAddObjEx(mbObjMan, 261, 5, 0, OM_GRP_NONE, CoinDispOMExec);
    obj->data = HuMemDirectMallocNum(
        HEAP_HEAP,
        COINDISP_MODEL_MAX * sizeof(COINDISPMODEL),
        HU_MEMNUM_OVL);
    omSetStatBit(obj, OM_STAT_MODELPAUSE);
    coinDispOMObj[i] = obj;
    work = omObjGetWork(obj, COINDISPWORK);
    work->killF = FALSE;
    if (coinNum != 0) {
        work->sign = (coinNum < 0) ? 1 : 0;
    } else {
        work->sign = (sign < 0) ? 1 : 0;
    }
    work->mode = COINDISP_MODE_ON;
    work->no = i;
    work->delay = 0;
    work->time = 0;
    obj->trans.x = pos->x;
    obj->trans.y = pos->y;
    obj->trans.z = pos->z;
    obj->rot.x = 0.0f;
    obj->rot.y = 0.01f;
    CoinDispCreate(obj, coinNum);
    CoinDispObjUpdate(obj);
    if (playSe) {
        if (!work->sign) {
            mbAudFXPlay(0x467);
        } else {
            mbAudFXPlay(0x468);
        }
    }
    return work->no;
}

s16 mbCoinDispMasuCreate(HuVecF *pos, int coinNum, BOOL playSe)
{
    return mbCoinDispCreate(pos, coinNum, 1, playSe);
}

s16 mbCoinDispCapsuleCreate(HuVecF *pos, int coinNum)
{
    return mbCoinDispCreate(pos, coinNum, 1, TRUE);
}

static void CoinDispCreate(OMOBJ *obj, int coinNum)
{
    int digitVal;
    COINDISPWORK *work = omObjGetWork(obj, COINDISPWORK);
    COINDISPMODEL *model = obj->data;
    BOOL dispF = FALSE;
    float motTime;
    int i;
    int modelNum;

    obj->mdlId[0] = mbCoinCreate();
    if (work->sign) {
        motTime = 2.5f;
    } else {
        motTime = 1.5f;
    }
    obj->mdlId[1] = mbCoinObjCreate(
        work->sign + 10,
        work->sign ? 2 : 1);
    digitVal = 100;
    work->modelNum = 0;
    modelNum = 2;
    coinNum = abs(coinNum);
    for (i = 0; i < 3; i++) {
        int digit = coinNum / digitVal;

        if (i == 2) {
            dispF = TRUE;
        }
        if (dispF || digit != 0) {
            dispF = TRUE;
            obj->mdlId[modelNum] = mbCoinObjCreate(
                digit,
                work->sign ? 2 : 1);
            modelNum++;
        }
        coinNum -= digit * digitVal;
        digitVal /= 10;
    }
    work->modelNum = modelNum;
    for (i = 0; i < work->modelNum; i++, model++) {
        model->pos.x = model->pos.y = model->pos.z = 0.0f;
        model->rotY = 0.0f;
        model->scale.x = model->scale.y = model->scale.z = 0.001f;
        mbCoinObjLayerSet(obj->mdlId[i], HU3D_CAM1);
    }
}

static void CoinDispOMExec(OMOBJ *obj)
{
    COINDISPWORK *work = omObjGetWork(obj, COINDISPWORK);

    if (work->killF || mbExitCheck()) {
        CoinDispObjKill(obj);
        coinDispOMObj[work->no] = NULL;
        omDelObjEx(HuPrcCurrentGet(), obj);
        return;
    }
    if (work->delay == 0) {
        switch (work->mode) {
            case COINDISP_MODE_ON:
                CoinDispOn(obj);
                break;

            case COINDISP_MODE_MAIN:
                CoinDispMain(obj);
                break;

            case COINDISP_MODE_OFF:
                CoinDispOff(obj);
                break;
        }
    } else {
        work->delay--;
    }
    CoinDispObjUpdate(obj);
}

static void CoinDispObjUpdate(OMOBJ *obj)
{
    COINDISPWORK *work = omObjGetWork(obj, COINDISPWORK);
    COINDISPMODEL *model = obj->data;
    HuVecF pos2D;
    HuVecF pos;
    Mtx rotMtx;
    float cameraRotY;
    int i;

    mbCameraRotGet(&pos2D);
    cameraRotY = pos2D.y;
    PSMTXRotRad(rotMtx, 'Y', 0.017453292f * cameraRotY);
    for (i = 0; i < work->modelNum; i++, model++) {
        float scale;
        float z;

        PSMTXMultVec(rotMtx, &model->pos, &pos2D);
        VECAdd(&obj->trans, &pos2D, &pos);
        mbPos3Dto2D(&pos, &pos2D);
        z = pos2D.z;
        pos2D.z = 800.0f;
        mbPos2Dto3D(&pos2D, &pos);
        scale = 800.0f / z;
        mbCoinObjPosSetV(obj->mdlId[i], &pos);
        mbCoinObjRotSet(
            obj->mdlId[i],
            0.0f,
            model->rotY + cameraRotY,
            0.0f);
        mbCoinObjScaleSet(
            obj->mdlId[i],
            model->scale.x * scale,
            model->scale.y * scale,
            model->scale.z * scale);
    }
}

static void CoinDispOn(OMOBJ *obj)
{
    COINDISPWORK *work = omObjGetWork(obj, COINDISPWORK);
    COINDISPMODEL *model = obj->data;
    float s;
    float scale;
    int i;

    s = mbSinDeg((float)work->time);
    scale = s;
    obj->rot.x = 405.0f * s;
    model->scale.x = model->scale.y = model->scale.z = scale;
    model->pos.x = model->pos.y = model->pos.z = 0.0f;
    model->rotY = obj->rot.x;
    if (work->time < 90) {
        work->time += 6;
        return;
    }
    work->mode = COINDISP_MODE_MAIN;
    work->time = 0;
    work->maxTime = 30;
    model = ((COINDISPMODEL *)obj->data) + 1;
    for (i = 1; i < work->modelNum; i++, model++) {
        model->scale.x = model->scale.y = model->scale.z = scale;
        model->pos.x = model->pos.y = model->pos.z = 0.0f;
        model->rotY = obj->rot.x;
    }
}

static void CoinDispMain(OMOBJ *obj)
{
    COINDISPWORK *work = omObjGetWork(obj, COINDISPWORK);
    COINDISPMODEL *model = obj->data;
    float weight = (float)work->time / work->maxTime;
    float ofsX = -0.5f * ((work->modelNum - 1) * 120.00001f);
    float s = mbSinDeg(weight * 90.0f);
    float angle;
    float sy;
    int i;

    obj->rot.x = 45.0f + (s * 315.0f);
    angle = weight * (((work->modelNum - 1) * 30.0f) + 180.0f);
    for (i = 0; i < work->modelNum; i++, model++) {
        sy = mbSinDeg(angle);
        if (sy < 0.0f) {
            sy = 0.0f;
        }
        model->pos.x = s * ofsX;
        model->pos.y = 200.0f * sy;
        model->pos.z = 0.0f;
        model->rotY = obj->rot.x;
        ofsX += 120.00001f;
        angle -= 30.0f;
    }
    if (++work->time > work->maxTime) {
        work->mode = COINDISP_MODE_OFF;
        work->time = 0;
        work->maxTime = 24;
        work->delay = 30;
    }
}

static void CoinDispOff(OMOBJ *obj)
{
    COINDISPWORK *work = omObjGetWork(obj, COINDISPWORK);
    COINDISPMODEL *model = obj->data;
    float weight = (float)work->time / work->maxTime;
    float angle;
    float s;
    float posY;
    float angleBase;
    int i;

    obj->rot.x = (270.0f * mbSinDeg(weight * 90.0f)) + 90.0f;
    angleBase = weight * (((work->modelNum - 1) * 30.0f) + 90.0f);
    for (i = 0; i < work->modelNum; i++, model++) {
        angle = angleBase - (i * 30.0f);
        if (angle < 0.0f) {
            angle = 0.0f;
        } else if (angle > 90.0f) {
            angle = 90.0f;
        }
        s = mbSinDeg(angle);
        if (work->sign) {
            posY = -100.0f * s;
        } else {
            posY = 100.0f * s;
        }
        model->pos.y = posY;
        model->rotY = obj->rot.x;
        mbCoinObjAlphaSet(obj->mdlId[i], mbCosDeg(angle));
    }
    if (++work->time > work->maxTime) {
        for (i = 0; i < work->modelNum; i++) {
            mbCoinObjDispSet(obj->mdlId[i], FALSE);
        }
        work->killF = TRUE;
    }
}

static void CoinDispObjKill(OMOBJ *obj)
{
    COINDISPWORK *work = omObjGetWork(obj, COINDISPWORK);
    int i;

    for (i = 0; i < work->modelNum; i++) {
        if (obj->mdlId[i] >= 0) {
            mbCoinObjNumDec(obj->mdlId[i]);
        }
        obj->mdlId[i] = -1;
    }
}

void mbCoinDispKill(s16 no)
{
    if (no <= 0 || no >= GW_PLAYER_MAX + 1) {
        return;
    }
    if (coinDispOMObj[no]) {
        COINDISPWORK *work = omObjGetWork(coinDispOMObj[no], COINDISPWORK);

        work->killF = TRUE;
    }
}

BOOL mbCoinDispKillCheck(s16 no)
{
    if (no <= 0 || no >= GW_PLAYER_MAX + 1) {
        return TRUE;
    }
    if (coinDispOMObj[no]) {
        COINDISPWORK *work = omObjGetWork(coinDispOMObj[no], COINDISPWORK);

        return FALSE;
    }
    return TRUE;
}

int mbCoinAddProcExec(int playerNo, int coinNum, BOOL dispF, BOOL fastF)
{
    int coinDiff;
    s16 dispNo;
    int i;
    int delay;
    int coinChg;
    int coinNew;
    s16 seId;

    if (abs(coinNum) >= 50) {
        delay = 1;
    } else if (abs(coinNum) >= 20) {
        delay = 3;
    } else {
        delay = 6;
    }
    coinNew = coinNum + mbPlayerCoinGet(playerNo);
    if (coinNew > 999) {
        coinNum = 999 - mbPlayerCoinGet(playerNo);
    } else if (coinNew < 0) {
        coinNum = -mbPlayerCoinGet(playerNo);
    }
    coinDiff = coinNum;
    if (!fastF) {
        coinChg = (coinNum >= 0) ? 1 : -1;
        seId = (coinChg > 0) ? 7 : 14;
        for (i = 0; i < abs(coinNum); i++) {
            mbPlayerCoinAdd(playerNo, coinChg);
            mbAudFXPlay(seId);
            HuPrcSleep(delay);
        }
    } else {
        mbPlayerCoinAdd(playerNo, coinDiff);
    }
    if (coinDiff != 0 || dispF) {
        HuVecF pos;

        mbAudFXPlay(15);
        mbPlayerPosGet(playerNo, &pos);
        pos.y += 250.0f;
        dispNo = mbCoinDispCreate(&pos, coinDiff, dispF, TRUE);
        while (!mbCoinDispKillCheck(dispNo)) {
            HuPrcVSleep();
        }
    }
    return coinDiff;
}

int mbCoinAddDispExec(int playerNo, int coinNum, BOOL dispF, BOOL fastF)
{
    int coinDiff;
    s16 dispNo;
    int i;
    int delay;
    int coinChg;
    int coinNew;
    s16 seId;

    if (abs(coinNum) >= 50) {
        delay = 1;
    } else if (abs(coinNum) >= 20) {
        delay = 3;
    } else {
        delay = 6;
    }
    coinNew = coinNum + mbPlayerCoinGet(playerNo);
    if (coinNew > 999) {
        coinNum = 999 - mbPlayerCoinGet(playerNo);
    } else if (coinNew < 0) {
        coinNum = -mbPlayerCoinGet(playerNo);
    }
    coinDiff = coinNum;
    if (!fastF) {
        coinChg = (coinNum >= 0) ? 1 : -1;
        seId = (coinChg > 0) ? 7 : 14;
        for (i = 0; i < abs(coinNum); i++) {
            mbPlayerCoinAdd(playerNo, coinChg);
            mbAudFXPlay(seId);
            HuPrcSleep(delay);
        }
    } else {
        mbPlayerCoinAdd(playerNo, coinDiff);
    }
    if (coinDiff != 0) {
        mbAudFXPlay(15);
    }
    if (dispF && coinDiff != 0) {
        HuVecF pos;

        mbPlayerPosGet(playerNo, &pos);
        pos.y += 250.0f;
        dispNo = mbCoinDispCreate(&pos, coinDiff, TRUE, TRUE);
        while (!mbCoinDispKillCheck(dispNo)) {
            HuPrcVSleep();
        }
    }
    return coinDiff;
}

int mbCoinAddExec(int playerNo, int coinNum)
{
    return mbCoinAddDispExec(playerNo, coinNum, FALSE, FALSE);
}

int mbStatTeamMinValGet(int teamNo, int value, int max, int *addNum,
    int *result)
{
    int playerNo[2];
    int stat;
    int player;
    int i;
    int j;

    stat = value;
    for (i = 0; i < 2; i++) {
        player = mbPlayerTeamFindPlayer(teamNo, i);
        playerNo[i] = player;
        stat += addNum[player];
        result[player] = addNum[player];
    }
    if (stat > max) {
        if (addNum[playerNo[0]] > addNum[playerNo[1]]) {
            player = playerNo[0];
            playerNo[0] = playerNo[1];
            playerNo[1] = player;
        }
        stat = value;
        for (j = 0; j < 2; j++) {
            player = playerNo[j];
            if (stat + addNum[player] > max) {
                result[player] = max - stat;
                stat = max;
            } else {
                stat += addNum[player];
            }
        }
    } else if (stat < 0) {
        if (addNum[playerNo[0]] < addNum[playerNo[1]]) {
            player = playerNo[0];
            playerNo[0] = playerNo[1];
            playerNo[1] = player;
        }
        stat = value;
        for (j = 0; j < 2; j++) {
            player = playerNo[j];
            if (stat + addNum[player] < 0) {
                result[player] = -stat;
                stat = 0;
            } else {
                stat += addNum[player];
            }
        }
    }
    return stat - value;
}

static void CoinAddAllProc(int *addNum, BOOL fastF, int *result)
{
    int coinNum[4];
    int delay[4];
    int time[4];
    int coinChg[4];
    BOOL activeF[4];
    int playerNum;
    int i;

    for (i = 0; i < 4; i++) {
        coinNum[i] = addNum[i];
        activeF[i] = FALSE;
    }
    if (!GWTeamFGet()) {
        for (i = 0; i < 4; i++) {
            int coinNew = coinNum[i] + mbPlayerCoinGet(i);

            if (coinNew > 999) {
                coinNum[i] = 999 - mbPlayerCoinGet(i);
            } else if (coinNew < 0) {
                coinNum[i] = -mbPlayerCoinGet(i);
            }
            result[i] = coinNum[i];
            activeF[i] = TRUE;
        }
    } else {
        for (i = 0; i < 2; i++) {
            playerNum = mbPlayerTeamFindPlayer(i, 0);
            activeF[playerNum] = TRUE;
            coinNum[playerNum] = mbStatTeamMinValGet(i,
                mbPlayerTeamCoinGet(i), 999, coinNum, result);
        }
    }
    if (!fastF) {
        for (i = 0; i < 4; i++) {
            if (!activeF[i]) {
                continue;
            }
            if (abs(coinNum[i]) >= 50) {
                delay[i] = 1;
            } else if (abs(coinNum[i]) >= 20) {
                delay[i] = 3;
            } else {
                delay[i] = 6;
            }
            time[i] = delay[i];
            coinChg[i] = (coinNum[i] >= 0) ? 1 : -1;
        }
        do {
            playerNum = 0;
            for (i = 0; i < 4; i++) {
                if (!activeF[i] || coinNum[i] == 0) {
                    continue;
                }
                if (--time[i] == 0) {
                    mbPlayerCoinAdd(i, coinChg[i]);
                    coinNum[i] -= coinChg[i];
                    mbAudFXPlay(7);
                    time[i] = delay[i];
                }
                playerNum++;
            }
            HuPrcVSleep();
        } while (playerNum != 0);
    } else {
        for (i = 0; i < 4; i++) {
            if (activeF[i]) {
                mbPlayerCoinAdd(i, coinNum[i]);
            }
        }
    }
    mbAudFXPlay(15);
}

void mbCoinAddAllProcExecV(int *addNum, BOOL *dispF, BOOL fastF)
{
    int result[4];
    int dispNo[4];
    HuVecF pos;
    BOOL waitF;
    int i;

    CoinAddAllProc(addNum, fastF, result);
    for (i = 0; i < 4; i++) {
        dispNo[i] = 0;
        if (result[i] != 0 || dispF[i]) {
            mbPlayerPosGet(i, &pos);
            pos.y += 250.0f;
            dispNo[i] = mbCoinDispCreate(&pos, result[i], dispF[i], TRUE);
        }
    }
    do {
        waitF = FALSE;
        for (i = 0; i < 4; i++) {
            if (dispNo[i] != 0 && !mbCoinDispKillCheck(dispNo[i])) {
                waitF = TRUE;
            }
        }
        HuPrcVSleep();
    } while (waitF);
}

void mbCoinAddAllProcExec(int num0, int num1, int num2, int num3,
    BOOL dispF, BOOL fastF)
{
    int addNum[4];
    int result[4];
    HuVecF pos;
    s16 dispNo;
    int i;

    addNum[0] = num0;
    addNum[1] = num1;
    addNum[2] = num2;
    addNum[3] = num3;
    CoinAddAllProc(addNum, fastF, result);
    if (dispF) {
        for (i = 0; i < 4; i++) {
            if (result[i] != 0) {
                mbPlayerPosGet(i, &pos);
                pos.y += 250.0f;
                dispNo = mbCoinDispCreate(&pos, result[i], TRUE, TRUE);
            }
        }
        while (!mbCoinDispKillCheck(dispNo)) {
            HuPrcVSleep();
        }
    }
}

void mbCoinAddAllExec(int num0, int num1, int num2, int num3)
{
    int addNum[4];
    int result[4];

    addNum[0] = num0;
    addNum[1] = num1;
    addNum[2] = num2;
    addNum[3] = num3;
    CoinAddAllProc(addNum, FALSE, result);
}
