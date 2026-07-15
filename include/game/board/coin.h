#ifndef _BOARD_COIN_H
#define _BOARD_COIN_H

#include "game/hu3d.h"

typedef struct MbCoinObj_s {
    HuVecF pos;
    HuVecF rot;
    HuVecF scale;
    float alpha;
    u32 work[6];
} MBCOINOBJ;

s16 mbCoinCreate(void);
s16 mbCoinCreate2(void);
s16 mbCoinObjCreate(int modelNo, int motNo);
s16 mbCoinObjCreate2(int modelNo, int motNo);
MBCOINOBJ *mbCoinObjGet(s16 objId);
void mbCoinObjNumDec(s16 objId);
void mbCoinObjKill(s16 objId);
void mbCoinObjPosSet(s16 objId, float x, float y, float z);
void mbCoinObjPosSetV(s16 objId, HuVecF *pos);
void mbCoinObjPosGet(s16 objId, HuVecF *pos);
void mbCoinObjRotSet(s16 objId, float x, float y, float z);
void mbCoinObjRotSetV(s16 objId, HuVecF *rot);
void mbCoinObjRotGet(s16 objId, HuVecF *rot);
void mbCoinObjScaleSet(s16 objId, float x, float y, float z);
void mbCoinObjScaleSetV(s16 objId, HuVecF *scale);
void mbCoinObjScaleGet(s16 objId, HuVecF *scale);
void mbCoinObjAlphaSet(s16 objId, float alpha);
float mbCoinObjAlphaGet(s16 objId);
void mbCoinObjDispSet(s16 objId, BOOL dispF);
BOOL mbCoinObjDispGet(s16 objId);
void mbCoinObjLayerSet(s16 objId, u16 layer);
void mbCoinObjMotSet(s16 objId, int motNo);
void mbCoinDispKill(s16 no);
BOOL mbCoinDispKillCheck(s16 no);

void mbCoinEffCreate(HuVecF *pos);

int mbCoinAddDispExec(int playerNo, int coinNum, BOOL dispF, BOOL fastF);

#endif
