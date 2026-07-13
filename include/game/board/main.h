#ifndef _BOARD_MAIN_H
#define _BOARD_MAIN_H

#include "game/object.h"
#include "game/process.h"
#include "game/gamework.h"

#include "datadir_enum.h"

#ifndef HUSPR_GRP_NONE
#define HUSPR_GRP_NONE -1
#endif

extern OMOBJMAN *mbObjMan;
extern HUPROCESS *mbMainProc;

BOOL mbExitCheck(void);
int mbBoardDataNumGet(int dataNum);
u32 mbRandMod(u32 mod);
BOOL mbAngleMoveTo(float *dest, float angle, float speed);

#endif
