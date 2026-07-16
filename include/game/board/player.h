#ifndef _BOARD_PLAYER_H
#define _BOARD_PLAYER_H

#include "game/board/object_data.h"
#include "game/gamework.h"
#include "game/object.h"
#include "humath.h"

typedef BOOL (*MBPLAYERTURNHOOK)(void);
typedef void (*MBPLAYERMOVEHOOK)(int playerNo);

typedef struct MbPlayerWork_s {
    MBMODELID objId;
    s16 motNo;
    u8 _unk04;
    s8 masuCorner;
    s16 _unk06;
    s16 _unk08;
    s16 masuMoveF;
    s16 _unk0C;
    s16 masuNext;
    u8 moveF : 1;
    u8 playerNo : 2;
    u8 _unk10_3 : 1;
    u8 moveEndF : 1;
    u8 _unk10_5 : 3;
    u8 _unk11[3];
    OMOBJ *rotateObj;
    OMOBJ *moveObj;
    OMOBJ *posFixObj;
    OMOBJ *moveNumObj;
    OMOBJ *colObj;
    OMOBJ *metalObj;
    OMOBJ *biriQObj;
    MBPLAYERTURNHOOK startTurnHook;
    MBPLAYERTURNHOOK endTurnHook;
    MBPLAYERMOVEHOOK moveHook;
    HuVecF _unk3C;
    struct Process_s *moveProc;
    HSF_MATERIAL *matCopy;
} MBPLAYERWORK;

static inline int mbPlayerGrpGet(int playerNo)
{
    return GwPlayer[playerNo].team;
}

static inline int mbPlayerHandicapGet(int playerNo)
{
    return GwPlayer[playerNo].handicap;
}

void mbPlayerTurnInitHookSet(void (*hook)(int playerNo));
void mbPlayerTurnCloseHookSet(void (*hook)(int playerNo));
void mbPlayerInit(BOOL noEventF);
void mbPlayerClose(void);
MBPLAYERWORK *mbPlayerWorkGet(int playerNo);
void mbPlayerStartTurnHookSet(int playerNo, MBPLAYERTURNHOOK hook);
void mbPlayerEndTurnHookSet(int playerNo, MBPLAYERTURNHOOK hook);
void mbPlayerMoveHookSet(int playerNo, MBPLAYERMOVEHOOK hook);
void mbTurnExec(BOOL intrF);
void mbSingleTurnExec(BOOL intrF);
void mbPlayerAmbSet(int playerNo, float ambR, float ambG, float ambB);
MBMODELID mbPlayerObjIDGet(int playerNo);
HU3D_MODELID mbPlayerModelIDGet(int playerNo);
BOOL mbPlayerAllComCheck(void);
int mbPlayerTeamFind(int playerNo);
int mbPlayerTeamFindOther(int playerNo);
int mbPlayerTeamFindPlayer(int teamNo, int memberNo);
int mbPlayerTeamFindOpp(int playerNo);
BOOL mbPlayerTeamCheckSame(int playerNo1, int playerNo2);
int mbPlayerTeamGet(int playerNo);
float mbPlayerWalkSpeedGet(void);
void mbPlayerLayerSet(int playerNo, int layer);
void mbPlayerCameraSet(int playerNo, u16 cameraBit);
void mbPlayerCullRadiusSet(int playerNo, float radius);
void mbPlayerStubValSet(int playerNo, BOOL value);
void mbPlayerPosReset(int playerNo);
void mbPlayerPosResetAll(void);

int mbPlayerBestPathGet(void);
void mbPlayerMtxSet(int playerNo, Mtx *matrix);
void mbPlayerMtxGet(int playerNo, Mtx *matrix);
void mbPlayerPosSetV(int playerNo, const HuVecF *pos);
void mbPlayerPosSet(int playerNo, float posX, float posY, float posZ);
void mbPlayerPosGet(int playerNo, HuVecF *pos);
void mbPlayerRotSetV(int playerNo, const HuVecF *rot);
void mbPlayerRotSet(int playerNo, float rotX, float rotY, float rotZ);
void mbPlayerRotGet(int playerNo, HuVecF *rot);
void mbPlayerRotYSet(int playerNo, float rotY);
float mbPlayerRotYGet(int playerNo);
void mbPlayerScaleSetV(int playerNo, const HuVecF *scale);
void mbPlayerScaleSet(int playerNo, float scaleX, float scaleY, float scaleZ);
void mbPlayerScaleGet(int playerNo, HuVecF *scale);
void mbPlayerRotateStart(int playerNo, s16 endAngle, s16 maxTime);
BOOL mbPlayerRotateCheck(int playerNo);
BOOL mbPlayerRotateCheckAll(void);
void mbPlayerEyeMatDarkSet(int playerNo, BOOL darkF);
void mbPlayerMatClone(int playerNo);
void mbPlayerSwap(int playerNo1, int playerNo2);
u32 mbPlayerNameMesGet(int playerNo);
char *mbPlayerNameGet(int playerNo);
u32 mbPlayerTagNameMesGet(int teamNo);
int mbPlayerDiceTypeGet(int diceNo);
void mbPlayerMotionSet(int playerNo, int motNo, u32 attr);
int mbPlayerMotionGet(int playerNo);
void mbPlayerMotionShiftSet(int playerNo, int motNo, float start, float end,
    u32 attr);
int mbPlayerMotionCreate(int playerNo, int dataNum);
int mbPlayerMotionKill(int playerNo, int motNo);
void mbPlayerMotionSpeedSet(int playerNo, float speed);
void mbPlayerMotionTimeSet(int playerNo, float time);
float mbPlayerMotionTimeGet(int playerNo);
float mbPlayerMotionMaxTimeGet(int playerNo);
void mbPlayerMotionStartEndSet(int playerNo, float start, float end);
void mbPlayerAttrSet(int playerNo, u32 attr);
void mbPlayerAttrReset(int playerNo, u32 attr);
void mbPlayerMotionVoiceOnSet(int playerNo, int motNo, BOOL voiceOnF);
BOOL mbPlayerMotionEndCheck(int playerNo);
BOOL mbPlayerMotionEndCheckAll(void);
void mbPlayerMotionEndWait(int playerNo);
void mbPlayerMotIdleSet(int playerNo);
void mbPlayerMasuMoveTo(int playerNo, int masuId, BOOL waitF);
void mbPlayerMasuMove(int playerNo, BOOL waitF);
void mbPlayerMasuMovePos(int playerNo, HuVecF *pos, BOOL waitF);
void mbPlayerMasuMoveSpeed(int playerNo, int masuId, s16 maxTime, BOOL waitF);
void mbPlayerMoveExec(int playerNo, HuVecF *srcPos, HuVecF *dstPos,
    s16 maxTime, HuVecF *rot, BOOL waitF);
void mbPlayerMoveMain(int playerNo, HuVecF *srcPos, HuVecF *dstPos, u32 motNo,
    float motSpeed, u32 motAttr, s16 maxTime, HuVecF *rot, BOOL waitF);
void mbPlayerDiceMotExec(int playerNo);
void mbMoveNumCreateColor(int playerNo, BOOL carF, int color);
void mbMoveNumCreate(int playerNo, BOOL carF);
void mbMoveNumKill(int playerNo);
void mbev_PlayerColMasuAllSet(int *masuId, BOOL waitF);
void mbev_PlayerColMasu(int playerNo, int masuId, BOOL waitF);
void mbev_PlayerColCircleAdd(
    int playerNo, int masuId, BOOL waitF, float radius);
void mbev_PlayerColMasuAdd(int playerNo, int masuId, BOOL waitF);
void mbev_PlayerColBall(int masuId, int *playerNo, HuVecF *pos);
void mbev_PlayerColMasuSet(int playerNo, int masuId, BOOL waitF);
void mbev_PlayerColReserve(int playerNo, int masuId, BOOL waitF);
void mbev_PlayerColSet(int playerNo, int masuId);
BOOL mbPlayerColCheck(void);
void mbPlayerColSnapSet(BOOL snapF);
void mbPlayerColSnapPlayerSet(int playerNo, BOOL snapF);
BOOL mbPlayerColSnapGet(int playerNo);
void mbPlayerColRestSet(int playerNo, BOOL restF);
void mbPlayerColFirstSet(int playerNo);
void mbPlayerColOrderReset(void);
void mbPlayerEffectSet(int playerNo, BOOL effectF);
void mbPlayerMetalSet(int playerNo, BOOL metalF);
void mbPlayerBiriQSet(int playerNo, BOOL biriQF);
void mbPlayerMetalColorSet(const GXColor *shadowColor,
    const GXColor *hiliteColor);
void mbPlayerCoinSet(int playerNo, int coinNum);
int mbPlayerCoinGet(int playerNo);
void mbPlayerCoinAdd(int playerNo, int coinNum);
void mbPlayerTeamCoinSet(int teamNo, int coinNum);
s16 mbPlayerTeamCoinGet(int teamNo);
int mbPlayerMaxCoinGet(void);
void mbPlayerStarSet(int playerNo, int starNum);
int mbPlayerStarGet(int playerNo);
void mbPlayerStarAdd(int playerNo, int starNum);
void mbPlayerGrpStarSet(int teamNo, int starNum);
s16 mbPlayerGrpStarGet(int teamNo);
int mbPlayerRankGet(int playerNo);
s16 mbPlayerTeamRankGet(int teamNo);
void mbPlayerCapsuleUseSet(int capsuleNo);
int mbPlayerCapsuleUseGet(void);
int mbPlayerCapsuleMaxGet(void);
int mbPlayerCapsuleAdd(int playerNo, int capsuleNo);
int mbPlayerCapsuleRemove(int playerNo, int index);
int mbPlayerCapsuleFind(int playerNo, int capsuleNo);
s8 mbPlayerCapsuleGet(int playerNo, int index);
s8 mbPlayerTeamCapsuleGet(int teamNo, int index);
int mbPlayerCapsuleNumGet(int playerNo);
int mbPlayerTeamCapsuleNumGet(int teamNo);
BOOL mbPlayerKettouCheck(int playerNo, s16 masuId);
void mbPlayerWinLoseVoicePlay(int playerNo, int motNo, int seId);
int mbPlayerVoicePanPlay(int playerNo, s16 seId);
int mbPlayerVoicePlay(int playerNo, s16 seId);
void mbPlayerDispSet(int playerNo, BOOL dispF);
BOOL mbPlayerDispGet(int playerNo);
GXColor mbPlayerColorGet(int playerNo);
void mbMoveNumDispSet(int playerNo, BOOL dispF);
void mbPlayerBlackoutSet(BOOL value);
BOOL mbPlayerBlackoutGet(void);
void mbPlayerMasuCornerSet(int playerNo, s8 cornerNo);
s8 mbPlayerMasuCornerGet(int playerNo);
void mbPlayerPlusMasuExec(int playerNo);
void mbPlayerCapCoinMasuExec(int playerNo);
void mbPlayerMinusMasuExec(int playerNo);

#endif
