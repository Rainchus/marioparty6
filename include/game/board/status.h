#ifndef _BOARD_STATUS_H
#define _BOARD_STATUS_H

#include "game/sprite.h"
#include "game/animdata.h"

#define STATUS_COLOR_GRAY 0
#define STATUS_COLOR_BLUE 1
#define STATUS_COLOR_RED 2
#define STATUS_COLOR_GREEN 3
#define STATUS_COLOR_PURPLE 4
#define STATUS_COLOR_MAX 5
#define STATUS_COLOR_NULL -1

#define STATUS_MOVE_LINEAR 0
#define STATUS_MOVE_SIN 1
#define STATUS_MOVE_REVCOS 2

#define STATUS_LAYOUT_TOP 0
#define STATUS_LAYOUT_BOTTOM 1

#define STATUS_KAO_NORMAL 0
#define STATUS_KAO_WIN 1
#define STATUS_KAO_LOSE 2

void mbStatusInit(void);
void mbStatusClose(void);
void mbStatusReset(void);
void mbStatusMoveTo(int playerNo, HuVecF *posBegin, HuVecF *posEnd);
void mbStatusMoveSet(int playerNo, HuVecF *posBegin, HuVecF *posEnd, int type, int time);
void mbStatusNoMoveSet(int statusNo, HuVecF *posBegin, HuVecF *posEnd, int type, int time);
void mbStatusPosOnGet(int statusNo, HuVecF *pos);
void mbStatusPosOffGet(int statusNo, HuVecF *pos);
BOOL mbStatusMoveCheck(int playerNo);
BOOL mbStatusOffCheckAll(void);
void mbStatusDispSet(int playerNo, BOOL dispF);
void mbStatusDispFocusSet(int playerNo, BOOL dispF);
void mbStatusDispBackup(void);
void mbStatusDispRestoreForce(void);
void mbStatusDispRestore(void);
BOOL mbStatusDispGet(int playerNo);
int mbStatusNoGet(int playerNo);
void mbStatusDispSetAll(BOOL dispF);
void mbStatusDispForceSet(int playerNo, BOOL dispF);
void mbStatusDispForceSetAll(BOOL dispF);
void mbStatusMasuDispSet(BOOL dispF);
void mbStatusCapsuleDispSet(int statusNo, BOOL dispF);
void mbStatusLayoutSet(int statusNo, int layoutNo);
void mbStatusCapsuleFocusSet(int statusNo, int index);
void mbStatusPosGet(int playerNo, HuVecF *pos);
void mbStatusNoPosGet(int statusNo, HuVecF *pos);
void mbStatusKaoNoSet(int statusNo, int kaoNo);
void mbSprCreate(int dataNum, int prio, ANIMDATA **animP, HUSPRID *sprIdP);
void mbSprNumSet(HUSPR_GROUPID gid, s16 memberNo, int num);
void mbStatusRainbowSet(int statusNo, float time, int colorEnd);
void mbStatusColorSet(int playerNo, int color);
int mbStatusColorGet(int playerNo);
void mbStatusColorAllSet(int color);

#endif
