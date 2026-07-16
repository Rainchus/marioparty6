#ifndef _BOARD_MASU_H
#define _BOARD_MASU_H

#include "humath.h"

#define MASU_LINK_MAX 5
#define MASU_MAX 256
#define MASU_NULL -1

#define MASU_LAYER_DEFAULT 0
#define MASU_LAYER_MAX 2

#define MASU_FLAG_JUMPTO 0x0001
#define MASU_FLAG_JUMPFROM 0x0002
#define MASU_FLAG_CLIMBTO 0x0004
#define MASU_FLAG_CLIMBFROM 0x0008
#define MASU_FLAG_BATTAN 0x4000
#define MASU_FLAG_START 0x8000

typedef struct Masu_s {
    u8 useMtxF : 1;
    u8 _unk00_1 : 7;
    u8 _unk01;
    s16 id;
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

typedef int (*MASUPATHCHECKHOOK)(s16 id, u32 mAttr, s16 *linkTbl, BOOL endF);
typedef int (*MASUEVENTHOOK)(int playerNo, s16 id);

void mbMasuInit(int dataNum);
void mbMasuClose(void);
BOOL mbMasuDataRead(int dataNum);
void mbMasuNextSet(s16 id);
void mbMasuNextDispSet(BOOL dispF);
int mbev_MasuMove(int playerNo, s16 id);
int mbev_MasuCapStop(int playerNo, s16 id);
int mbev_MasuStop(int playerNo, s16 id);
int mbev_MasuMasuStart(int playerNo);
int mbev_MasuMasuEnd(int id);
int mbMasuNumGet(void);
int mbMasuRawNumGet(void);
MASU *mbMasuGet(s16 id);
void mbMasuLayerSet(int layer);
int mbMasuLayerGet(void);
u16 mbMasuAttrGet(s16 id);
void mbMasuAttrSet(s16 id, u16 attr);
u32 mbMasuMAttrGet(s16 id);
void mbMasuMAttrSet(s16 id, u32 attr);
u32 mbev_MasuBitGet(u32 attr, u32 mask);
u32 mbev_MasuAttrGet(int attr, u32 mask);
int mbMasuTypeGet(s16 id);
void mbMasuTypeSet(s16 id, int type);
void mbMasuTypeChange(u16 oldType, u16 newType);
BOOL mbMasuDispCheck(s16 id);
void mbMasuCapsuleReset(void);
int mbMasuCapsuleGet(s16 id);
void mbMasuCapsuleSet(s16 id, int capsuleNo);
void mbMasuPosGet(s16 id, HuVecF *pos);
void mbMasuPosSet(s16 id, float x, float y, float z);
void mbMasuPosSetV(s16 id, HuVecF *pos);
void mbMasuCornerRotPosGet(s16 id, int cornerNo, HuVecF *pos);
void mbMasuCornerPosGet(s16 id, int cornerNo, HuVecF *pos);
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
void mbMasuModelDispSet(BOOL dispF);
s16 mbMasuLinkGet(s16 id, int linkNo);
s16 mbMasuLinkNumGet(s16 id);
int mbMasuLinkTblGet(s16 id, s16 *linkTbl);
int mbMasuLinkTblGet2(s16 id, s16 *linkTbl, BOOL hookF);
s16 mbMasuAttrFindLink(s16 id, u16 attr);
s16 mbMasuAttrMatchFindLink(s16 id, u16 attr, u16 mask);
s16 mbMasuMAttrFindLink(s16 id, u32 attr);
s16 mbMasuMAttrMatchFindLink(s16 id, u32 attr, u32 mask);
s16 mbMasuTypeFindLink(s16 id, int type);
s16 mbMasuLinkParentGet(s16 id, s16 *linkTbl);
int mbMasuTypeListGet(s16 type, s16 *list);
int mbMasuAttrListGet(u16 attr, s16 *list);
int mbMasuAttrMatchListGet(u16 attr, u16 mask, s16 *list);
int mbMasuMAttrListGet(u32 attr, s16 *list);
int mbMasuMAttrMatchTblGet(u32 attr, u32 mask, s16 *list);
int mbMasuFind_TypeStepGet(s16 id, s16 type);
int mbMasuFind_TypeStepGet2(s16 id, s16 type, BOOL hookF, BOOL dispF);
int mbMasuFind_AttrStepGet(s16 id, u16 attr);
int mbMasuFind_AttrMatchStepGet(s16 id, u16 arg1, u16 arg2);
int mbMasuFind_AttrStepGet2(s16 id, u16 arg1, u16 arg2, BOOL hookF,
    BOOL dispF);
int mbMasuFind_MAttrStepGet(s16 id, u32 attr);
int mbMasuFind_MAttrMatchStepGet(s16 id, u32 arg1, u32 arg2);
int mbMasuFind_MAttrStepGet2(s16 id, u32 arg1, u32 arg2, BOOL hookF,
    BOOL dispF);
int mbMasuFind_IdStepGet(s16 id, s16 targetId);
int mbMasuFind_IdStepGet2(s16 id, s16 targetId, BOOL hookF, BOOL dispF);
s16 mbMasuFind_TypeSearch(s16 id, s16 type);
s16 mbMasuFind_TypeIdGet(s16 id, s16 type, BOOL hookF, BOOL dispF);
s16 mbMasuFind_AttrIdGet(s16 id, u16 attr);
s16 mbMasuFind_AttrMatchIdGet(s16 id, u16 arg1, u16 arg2);
s16 mbMasuFind_AttrMatchIdGet2(s16 id, u16 arg1, u16 arg2, BOOL hookF,
    BOOL dispF);
s16 mbMasuFind_MAttrIdGet(s16 id, u32 attr);
s16 mbMasuFind_MAttrMatchIdGet(s16 id, u32 arg1, u32 arg2);
s16 mbMasuFind_MAttrMatchIdGet2(s16 id, u32 arg1, u32 arg2, BOOL hookF,
    BOOL dispF);
int mbMasuFind_TypeListGet(s16 id, s16 type, s16 *list);
int mbMasuFind_TypeListGet2(s16 id, s16 type, BOOL hookF, BOOL dispF,
    s16 *list);
int mbMasuFind_AttrNumGet(s16 id, u16 attr, u32 unused);
int mbMasuFind_AttrMatchNumGet(s16 id, u16 arg1, u16 arg2, u32 unused);
int mbMasuFind_AttrMatchNumGet2(s16 id, u16 arg1, u16 arg2, BOOL hookF,
    BOOL dispF, u32 unused);
s16 mbMasuFind_MAttrNumGet(s16 id, u32 attr);
int mbMasuFind_MAttrMatchListGet(s16 id, u32 arg1, u32 arg2, s16 *list);
int mbMasuFind_MAttrMatchListGet2(s16 id, u32 arg1, u32 arg2, BOOL hookF,
    BOOL dispF, s16 *list);
int mbMasuFind_IdListGet(s16 id, s16 targetId, s16 *list);
int mbMasuFind_IdListGet2(s16 id, s16 targetId, BOOL hookF, BOOL dispF,
    s16 *list);
void mbev_MasuMoveEndSet(MASUEVENTHOOK hook);
void mbev_MasuMoveStartSet(MASUEVENTHOOK hook);
void mbev_MasuHatenaSet(MASUEVENTHOOK hook);
void mbev_MasuLinkTblHookSet(MASUPATHCHECKHOOK hook);
int mbMasuPlayerCapMoveCheck(int playerNo, s16 id);
int mbMasuPlayerCapStopCheck(int playerNo, s16 id);
void mbMasuPlayerColorSet(int playerNo);
void mbMasuPlayerDispSet(BOOL dispF);
void mbMasuPlayerFadeSet(BOOL fadeF);
void mbMasuPlayerPrizeReset(int playerNo);
int mbMasuPKinokoValueGet(int playerNo, s16 id);
int mbMasuStub(void);

#endif
