#ifndef _BOARD_PAUSE_H
#define _BOARD_PAUSE_H

#include "types.h"

typedef void (*MBPAUSEHOOK)(BOOL pauseF);

void mbPauseInit(void);
void mbPauseCreate(int playerNo);
int mbPauseStartCheck(void);
BOOL mbPauseProcCheck(void);
void mbPauseDisableSet(BOOL disableF);
BOOL mbPauseDisableGet(void);
void mbPauseHookPush(MBPAUSEHOOK hook);
void mbPauseHookPop(MBPAUSEHOOK hook);
void mbPauseSet(BOOL pauseF);
int mbPauseDataDirGet(int type);
int mbPauseDataNumGet(int dataNum);

#endif
