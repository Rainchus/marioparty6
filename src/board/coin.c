#include "game/board/coin.h"

#include "game/gamework.h"
#include "game/memory.h"
#include "game/object.h"

#include "string.h"

#define COIN_OBJ_BANK_MAX 64
#define COIN_OBJ_BANK_SIZE 64

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

static OMOBJ *coinDispOMObj[GW_PLAYER_MAX + 1] = {};
static MBCOINOBJDATA coinObjData;

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
