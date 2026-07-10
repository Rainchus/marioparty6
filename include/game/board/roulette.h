#ifndef _BOARD_ROULETTE_H
#define _BOARD_ROULETTE_H

#include "game/board/main.h"

BOOL mbRouletteCreate(int playerNo, int maxPlayer);
BOOL mbRouletteKaneCreate(int playerNo);
BOOL mbRouletteCheck(void);
void mbRouletteWait(void);
int mbRouletteResultGet(void);
void mbRouletteMaxSpeedSet(float speed);
void mbRouletteComValueSet(s16 value);

#endif
