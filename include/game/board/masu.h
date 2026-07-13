#ifndef _BOARD_MASU_H
#define _BOARD_MASU_H

#include "humath.h"

#define MASU_LINK_MAX 5

#define MASU_LAYER_DEFAULT 0
#define MASU_LAYER_MAX 2

#define MASU_FLAG_BATTAN 0x4000

typedef struct Masu_s {
    u8 useMtxF : 1;
    u8 _unk00_1 : 7;
    u8 _unk01;
    s16 _unk02;
    HuVecF pos;
    u16 flag;
    u16 _unk12;
    u32 mAttr;
    HuVecF scale;
    HuVecF rot;
    u16 type;
    s16 capsuleNo;
    Mtx matrix;
    u16 linkNum;
    u16 linkTbl[MASU_LINK_MAX];
} MASU;

int mbMasuNumGet(void);
int mbMasuRawNumGet(void);
MASU *mbMasuGet(s16 id);
void mbMasuLayerSet(int layer);
int mbMasuLayerGet(void);
u16 mbMasuAttrGet(s16 id);
void mbMasuAttrSet(s16 id, u16 attr);
u32 mbMasuMAttrGet(s16 id);
void mbMasuMAttrSet(s16 id, u32 attr);
u16 mbMasuTypeGet(s16 id);
void mbMasuTypeSet(s16 id, int type);
s16 mbMasuCapsuleGet(s16 id);
void mbMasuPosGet(s16 id, HuVecF *pos);
void mbMasuPosSet(s16 id, float x, float y, float z);
void mbMasuPosSetV(s16 id, HuVecF *pos);
void mbMasuRotGet(s16 id, HuVecF *rot);
void mbMasuRotSet(s16 id, float x, float y, float z);
void mbMasuRotSetV(s16 id, HuVecF *rot);
void mbMasuMtxGet(s16 id, Mtx matrix);
void mbMasuMtxSet(s16 id, Mtx matrix);
BOOL mbMasuDispGet(void);
void mbMasuDispSet(BOOL dispF);
void mbMasuDispAttrSet(u16 attr);
void mbMasuDispAttrReset(u16 attr);
u16 mbMasuDispAttrGet(void);
void mbMasuDispMAttrSet(u32 attr);
void mbMasuDispMAttrReset(u32 attr);
u32 mbMasuDispMAttrGet(void);
s16 mbMasuLinkGet(s16 id, int linkNo);
s16 mbMasuLinkNumGet(s16 id);
int mbMasuLinkTblGet(s16 id, s16 *linkTbl);
int mbMasuLinkTblGet2(s16 id, s16 *linkTbl, BOOL hookF);
s16 mbMasuLinkParentGet(s16 id, s16 *linkTbl);
int mbMasuFind_IdStepGet(s16 id, s16 targetId);
int mbMasuFind_TypeStepGet2(s16 id, int type, BOOL arg2, BOOL arg3);

#endif
