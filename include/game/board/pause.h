#ifndef _BOARD_PAUSE_H
#define _BOARD_PAUSE_H

#include "types.h"

typedef void (*MBPAUSEHOOK)(BOOL pauseF);

void mbPauseHookPush(MBPAUSEHOOK hook);
void mbPauseHookPop(MBPAUSEHOOK hook);

#endif
