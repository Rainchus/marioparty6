#include "game/board/guide.h"

typedef void (*TUTORIALMAINFUNC)(void);

typedef struct TutorialCallWork_s {
    int scene;
    int callNum;
    int result;
    int stat;
    int mode;
} TUTORIALCALLWORK;

static TUTORIALCALLWORK tutorialCallWork;
static OMOBJ *tutorialGuideObj;
static TUTORIALMAINFUNC tutorialMain;
static BOOL tutorialExitOnF;
static BOOL tutorialExitReqF;

void mbTutorialMainFuncSet(TUTORIALMAINFUNC func)
{
    tutorialMain = func;
}

void mbTutorialCallEnd(void)
{
    tutorialCallWork.stat = -1;
}

void mbTutorialResultSet(int result)
{
    tutorialCallWork.result = result;
}

int mbTutorialSceneGet(void)
{
    return tutorialCallWork.scene;
}

void mbTutorialModeSet(int mode)
{
    tutorialCallWork.mode = mode;
}

int mbTutorialModeGet(void)
{
    return tutorialCallWork.mode;
}

OMOBJ *mbTutorialGuideGet(void)
{
    return tutorialGuideObj;
}

BOOL mbTutorialExitReqGet(void)
{
    return tutorialExitReqF;
}

void mbTutorialExitSet(void)
{
    tutorialExitReqF = TRUE;
}

void mbTutorialExitOnSet(BOOL exitOnF)
{
    tutorialExitOnF = exitOnF;
}
