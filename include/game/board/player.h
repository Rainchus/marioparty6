#ifndef _BOARD_PLAYER_H
#define _BOARD_PLAYER_H

#include "game/board/object_data.h"
#include "game/gamework.h"
#include "humath.h"

static inline int mbPlayerGrpGet(int playerNo)
{
    return GwPlayer[playerNo].team;
}

void mbPlayerTurnInitHookSet(void (*hook)(int playerNo));
void mbPlayerTurnCloseHookSet(void (*hook)(int playerNo));
void mbPlayerAmbSet(int playerNo, float ambR, float ambG, float ambB);
MBMODELID mbPlayerObjIDGet(int playerNo);
BOOL mbPlayerAllComCheck(void);
int mbPlayerTeamFind(int playerNo);
int mbPlayerTeamFindOther(int playerNo);
int mbPlayerTeamFindPlayer(int teamNo, int memberNo);
float mbPlayerWalkSpeedGet(void);
void mbPlayerStubValSet(int playerNo, BOOL value);

int mbPlayerBestPathGet(void);
void mbPlayerPosGet(int playerNo, HuVecF *pos);
void mbPlayerRotateStart(int playerNo, s16 endAngle, s16 maxTime);
BOOL mbPlayerRotateCheck(int playerNo);
void mbPlayerMotIdleSet(int playerNo);
int mbPlayerCoinGet(int playerNo);
int mbPlayerCapsuleFind(int playerNo, int capsuleNo);
void mbMoveNumDispSet(int playerNo, BOOL dispF);
void mbPlayerBlackoutSet(BOOL value);
BOOL mbPlayerBlackoutGet(void);

#endif
