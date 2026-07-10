#ifndef _BOARD_PLAYER_H
#define _BOARD_PLAYER_H

#include "game/gamework.h"
#include "humath.h"

int mbPlayerBestPathGet(void);
void mbPlayerPosGet(int playerNo, HuVecF *pos);
void mbPlayerRotateStart(int playerNo, s16 endAngle, s16 maxTime);
BOOL mbPlayerRotateCheck(int playerNo);
void mbPlayerMotIdleSet(int playerNo);
int mbPlayerCoinGet(int playerNo);
void mbMoveNumDispSet(int playerNo, BOOL dispF);

#endif
