#ifndef _BOARD_GATE_H
#define _BOARD_GATE_H

#include "game/board/main.h"

#define MESS_BOARD_GATE_BATTAN_YES 0x290001
#define MESS_BOARD_GATE_BATTAN_NO 0x290002
#define MESS_BOARD_GATE_BATTAN_COIN 0x290003
#define MESS_BOARD_GATE_BATTAN_CHOICE 0x290000

void mbGateInit(void);
void mbGateClose(void);
void mbGateCreate(void);

void mbGateBattanCreate(int masuId);
void mbGateBattanKill(int no);

s16 mbev_GateMasu(int playerNo, s16 masuId, s16 masuNext);
int mbev_GateBattanMasuFind(s16 masuId);
int mbev_GateBattan(int playerNo, int masuId);

#endif
