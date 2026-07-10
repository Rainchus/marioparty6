#ifndef _BOARD_WINDOW_H
#define _BOARD_WINDOW_H

#include "types.h"
#include "game/window.h"

#define MBWIN_TYPE_RESULT 2
#define MBWIN_TYPE_EVENT 2

void mbWinCreate(int type, int mess, int speaker);
void mbWinCreateChoice(int type, int mess, int speaker, int defChoice);
void mbWinTopWait(void);
void mbWinTopAttrReset(u32 attr);
int mbWinTopChoiceGet(void);

#endif
