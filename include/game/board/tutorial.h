#ifndef _BOARD_TUTORIAL_H
#define _BOARD_TUTORIAL_H

#include "game/board/object_data.h"
#include "game/object.h"

typedef void (*TUTORIALMAINFUNC)(void);

void mbTutorialInit(void);
void mbTutorialMainFuncSet(TUTORIALMAINFUNC func);
void mbTutorialCreate(void);
int mbTutorialCall(int scene);
void mbTutorialCallEnd(void);
void mbTutorialVSleep(void);
void mbTutorialCallWait(int scene);
void mbTutorialMultiCall(int scene, int callNum);
void mbTutorialCallResult(int scene, int result);
void mbTutorialMesCall(int scene, int message);
void mbTutorialMesMasuCall(int scene, int message, int masuId);
void mbTutorialTurnCall(int turn);
void mbTutorialGuideCall(int guideNo);
void mbTutorialCapsuleUseCall(int capsuleNo);
void mbTutorialResultSet(int result);
int mbTutorialSceneGet(void);
void mbTutorialModeSet(int mode);
int mbTutorialModeGet(void);
s16 mbTutorialViewSet(void);
void mbTutorialViewMasuSet(s16 masuId);
s16 mbTutorialSprCreate(unsigned int dataNum);
s16 mbTutorialSprDispOn(unsigned int dataNum);
void mbTutorialSprKill(s16 sprId);
void mbTutorialSprDispOff(s16 sprId);
void mbTutorialSprGrpSet(s16 grpId);
void mbTutorialSprGrpKill(s16 grpId);
MBMODELID mbTutorialModelCreate(int dataNum, BOOL linkF);
void mbTutorialModelKill(MBMODELID modelId);
OMOBJ *mbTutorialGuideCreate(s8 *motTbl, BOOL screenF);
void mbTutorialGuideClose(OMOBJ *obj);
OMOBJ *mbTutorialGuideGet(void);
BOOL mbTutorialExitReqGet(void);
void mbTutorialExitSet(void);
void mbTutorialExitOnSet(BOOL exitOnF);

BOOL mbTutorialWinWait(int winNo);
void mbTutorialWinClose(int winNo);
int mbTutorialWinCreate(int message);
void mbTutorialWinKeyWait(int winNo);
void mbTutorialWinMesExec(int message);
void mbTutorialWinMesMasuExec(int message, int masuId);
void mbTutorialMgCallInit(void);
void mbTutorialMgCallClose(void);

#endif
