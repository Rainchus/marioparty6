#ifndef _BOARD_COMCHOICE_H
#define _BOARD_COMCHOICE_H

#include "types.h"

void mbComChoiceLeftSet(void);
void mbComChoiceRightSet(void);
void mbComChoiceUpSet(void);
void mbComChoiceDownSet(void);
void mbComChoiceListDownSet(s32 num);
void mbComChoiceListRightSet(s32 num);
void mbComChoiceListSet(s32 rightNum, s32 downNum);

#endif
