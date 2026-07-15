#ifndef _BOARD_WINDOW_H
#define _BOARD_WINDOW_H

#include "game/process.h"
#include "game/window.h"

#define MBWIN_MAX 16

#define MBWIN_TYPE_RESULT 2
#define MBWIN_TYPE_EVENT 2
#define MBWIN_TYPE_GUIDE 5
#define MBWIN_TYPE_CAPSULE 6
#define MBWIN_TYPE_HELP 7
#define MBWIN_TYPE_PAUSEGUIDE 8
#define MBWIN_TYPE_BLANK 9
#define MBWIN_TYPE_MAX 10

#define MBWIN_MES_NONE ((u32)-1)
#define MBWIN_NONE -1

typedef void (*MBWINCOMKEYHOOK)(void);

typedef struct MBWin_s {
    HUPROCESS *proc;
    int winId;
    u32 mess;
    u8 no;
    s8 mesSpeed;
    u32 attr;
    int type;
    s8 speakerNo;
    s16 prio;
    int playerNo;
    HuVec2f pos;
    HuVec2f size;
    HuVec2f scale;
    HuVec2f centerOfs;
    BOOL pauseF;
    BOOL choiceF;
    int choiceNo;
    BOOL choiceDisable[HUWIN_CHOICE_MAX];
    u32 insertMes[HUWIN_INSERTMES_MAX];
    MBWINCOMKEYHOOK comKeyHook;
    int frame;
} MBWIN;

void mbWinInit(void);
void mbWinClose(void);
int mbWinCreate(int type, u32 mess, int speakerNo);
int mbWinCreateChoice(int type, u32 mess, int speakerNo, int choiceNo);
int mbWinCreateHelp(u32 mess);
int mbWinCreateFrame(int type, u32 mess, int speakerNo, int frame);
int mbWinCreateTime(int type, u32 mess, int speakerNo);
int mbWinCreateBlank(void);
int mbWinCreateBlankFrame(int frame);
MBWIN *mbWinGet(int winNo);
void mbWinKill(s16 winNo);
void mbWinTopKill(void);
void mbWinKillAll(void);

void mbWinPosSet(s16 winNo, s16 posX, s16 posY);
void mbWinTopPosSet(s16 posX, s16 posY);
void mbWinPosGet(s16 winNo, HuVec2f *pos);
void mbWinTopPosGet(HuVec2f *pos);
void mbWinSizeSet(s16 winNo, s16 sizeX, s16 sizeY);
void mbWinTopSizeSet(s16 sizeX, s16 sizeY);
void mbWinMesMaxSizeGet(s16 winNo, HuVec2f *size);
void mbWinTopMesMaxSizeGet(HuVec2f *size);
void mbWinScaleSet(s16 winNo, float scaleX, float scaleY);
void mbWinTopScaleSet(float scaleX, float scaleY);
void mbWinScaleGet(s16 winNo, HuVec2f *scale);
void mbWinTopScaleGet(HuVec2f *scale);
int mbWinTopChoiceGet(void);
int mbWinChoiceGet(s16 winNo);
void mbWinPause(s16 winNo);
void mbWinTopPause(void);
void mbWinInsertMesSet(s16 winNo, u32 insertMes, int insertMesNo);
void mbWinTopInsertMesSet(u32 insertMes, int insertMesNo);
BOOL mbWinDoneCheck(s16 winNo);
BOOL mbWinTopDoneCheck(void);
void mbWinWait(s16 winNo);
void mbWinTopWait(void);
void mbWinAttrSet(s16 winNo, u32 attr);
void mbWinTopAttrSet(u32 attr);
void mbWinAttrReset(s16 winNo, u32 attr);
void mbWinTopAttrReset(u32 attr);
void mbWinChoiceDisable(s16 winNo, int choiceNo);
void mbWinTopChoiceDisable(int choiceNo);
void mbWinMesSpeedSet(s16 winNo, int speed);
void mbWinTopMesSpeedSet(int speed);
void mbWinMesColSet(s16 winNo, int mesCol);
void mbWinTopMesColSet(int mesCol);
s16 mbWinChoiceNowGet(s16 winNo);
s16 mbWinTopChoiceNowGet(void);
void mbWinPriSet(s16 winNo, s16 prio);
void mbWinTopPriSet(s16 prio);
s16 mbWinChoiceNowGet2(s16 winNo);
s16 mbWinTopChoiceNowGet2(void);
void mbWinPlayerDisable(s16 winNo, int playerNo);
void mbWinTopPlayerDisable(int playerNo);
void mbWinTopComKeyHookSet(MBWINCOMKEYHOOK comKeyHook);
void mbWinDispSet(s16 winNo, BOOL dispF);
void mbWinTopDispSet(BOOL dispF);
void mbWinPauseHook(BOOL dispF);
HUWINID mbWinIDGet(s16 winNo);
HUWINID mbWinTopIDGet(void);
void mbWinCenterGet(s16 winNo, HuVec2f *pos);
void mbWinCenterInsertGet(s16 winNo, u32 mess);
void mbWinTopCenterInsertGet(u32 mess);

#endif
