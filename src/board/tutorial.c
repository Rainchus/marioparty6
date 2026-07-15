#include "game/board/camera.h"
#include "game/board/guide.h"
#include "game/board/player.h"
#include "game/board/tutorial.h"
#include "game/board/window.h"
#include "game/esprite.h"

typedef struct TutorialCallWork_s {
    int scene;
    int callNum;
    int result;
    int stat;
    int mode;
} TUTORIALCALLWORK;

typedef struct TutorialWinData_s {
    s8 stat;
    s8 delay;
    s16 time;
} TUTORIALWINDATA;

static s16 tutorialSprId[16];
static s16 tutorialSprGrpId[16];
static s16 tutorialMdlId[32];
static TUTORIALCALLWORK tutorialCallWork;
static TUTORIALWINDATA tutorialWinData[HUWIN_MAX];
static OMOBJ *tutorialGuideObj;
static TUTORIALMAINFUNC tutorialMain;
static BOOL tutorialExitOnF;
static BOOL tutorialExitReqF;
static HUPROCESS *tutorialMainProc;

void mbTutorialInit(void)
{
    int i;

    tutorialGuideObj = NULL;
    for (i = 0; i < 16; i++) {
        tutorialSprId[i] = -1;
    }
    for (i = 0; i < 16; i++) {
        tutorialSprGrpId[i] = -1;
    }
    for (i = 0; i < 32; i++) {
        tutorialMdlId[i] = -1;
    }
    mbTutorialMainFuncSet(NULL);
    mbTutorialMgCallInit();
    tutorialExitReqF = FALSE;
    tutorialExitOnF = FALSE;
}

void mbTutorialMainFuncSet(TUTORIALMAINFUNC func)
{
    tutorialMain = func;
}

int mbTutorialCall(int scene)
{
    if (!_CheckFlag(FLAG_BOARD_TUTORIAL)) {
        return -1;
    }
    if (tutorialMainProc == NULL) {
        return -1;
    }
    while (tutorialCallWork.stat == 0) {
        HuPrcVSleep();
    }
    if (tutorialCallWork.stat < 0) {
        return -1;
    }
    if (tutorialCallWork.scene >= 0) {
        if (tutorialCallWork.scene != scene) {
            return -1;
        }
        tutorialCallWork.callNum--;
        if (tutorialCallWork.callNum > 0) {
            return -1;
        }
    }
    tutorialCallWork.callNum = scene;
    tutorialCallWork.result = -1;
    tutorialCallWork.stat = 0;
    while (tutorialCallWork.stat == 0) {
        HuPrcVSleep();
    }
    return tutorialCallWork.result;
}

void mbTutorialCallEnd(void)
{
    tutorialCallWork.stat = -1;
}

void mbTutorialVSleep(void)
{
    if (mbTutorialExitReqGet()) {
        HuPrcSleep(-1);
    }
    HuPrcVSleep();
}

void mbTutorialCallWait(int scene)
{
    tutorialCallWork.scene = scene;
    tutorialCallWork.callNum = 0;
    tutorialCallWork.stat = 1;
    while (tutorialCallWork.stat != 0) {
        mbTutorialVSleep();
    }
}

void mbTutorialMultiCall(int scene, int callNum)
{
    tutorialCallWork.scene = scene;
    tutorialCallWork.callNum = callNum;
    tutorialCallWork.stat = 1;
    while (tutorialCallWork.stat != 0) {
        mbTutorialVSleep();
    }
}

void mbTutorialCallResult(int scene, int result)
{
    tutorialCallWork.scene = scene;
    tutorialCallWork.callNum = 0;
    tutorialCallWork.stat = 1;
    while (tutorialCallWork.stat != 0) {
        mbTutorialVSleep();
    }
    mbTutorialResultSet(result);
}

void mbTutorialMesCall(int scene, int message)
{
    tutorialCallWork.scene = scene;
    tutorialCallWork.callNum = 0;
    tutorialCallWork.stat = 1;
    while (tutorialCallWork.stat != 0) {
        mbTutorialVSleep();
    }
    mbTutorialWinMesExec(message);
}

void mbTutorialMesMasuCall(int scene, int message, int masuId)
{
    tutorialCallWork.scene = scene;
    tutorialCallWork.callNum = 0;
    tutorialCallWork.stat = 1;
    while (tutorialCallWork.stat != 0) {
        mbTutorialVSleep();
    }
    mbTutorialWinMesMasuExec(message, masuId);
}

void mbTutorialTurnCall(int turn)
{
    tutorialCallWork.scene = 5;
    tutorialCallWork.callNum = 0;
    tutorialCallWork.stat = 1;
    while (tutorialCallWork.stat != 0) {
        mbTutorialVSleep();
    }
    mbTutorialResultSet(turn - 1);
}

void mbTutorialGuideCall(int guideNo)
{
    tutorialCallWork.scene = 13;
    tutorialCallWork.callNum = 0;
    tutorialCallWork.stat = 1;
    while (tutorialCallWork.stat != 0) {
        mbTutorialVSleep();
    }
    mbTutorialResultSet(guideNo);
}

void mbTutorialCapsuleUseCall(int capsuleNo)
{
    int capsuleIndex;

    tutorialCallWork.scene = 15;
    tutorialCallWork.callNum = 0;
    tutorialCallWork.stat = 1;
    while (tutorialCallWork.stat != 0) {
        mbTutorialVSleep();
    }
    capsuleIndex = mbPlayerCapsuleFind(GwSystem.turnPlayerNo, capsuleNo);
    mbTutorialResultSet(capsuleIndex);
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

s16 mbTutorialViewSet(void)
{
    HuVecF rot = { -35.0f, 0.0f, 0.0f };
    HuVecF offset = { 0.0f, 200.0f, 0.0f };
    s16 masuId;

    masuId = mbMasuFind_AttrIdGet(-1, 0x8000);
    mbCameraMoveMasu(masuId, &rot, &offset, mbCameraPlayerViewZoomGet(0), -1.0f, -1);
    mbCameraMoveOnSet(FALSE);
    return masuId;
}

void mbTutorialViewMasuSet(s16 masuId)
{
    HuVecF rot = { -35.0f, 0.0f, 0.0f };
    HuVecF offset = { 0.0f, 50.0f, 0.0f };

    mbCameraMoveMasu(masuId, &rot, &offset, mbCameraPlayerViewZoomGet(0), -1.0f, -1);
    mbCameraMoveOnSet(FALSE);
}

s16 mbTutorialSprCreate(unsigned int dataNum)
{
    s16 sprId;
    int i;

    for (i = 0; i < 16; i++) {
        if (tutorialSprId[i] < 0) {
            break;
        }
    }
    sprId = tutorialSprId[i] = espEntry(dataNum, 1500, 0);
    espAttrSet(sprId, HUSPR_ATTR_LINEAR);
    return sprId;
}

s16 mbTutorialSprDispOn(unsigned int dataNum)
{
    s16 sprId;
    int i;

    sprId = mbTutorialSprCreate(dataNum);
    espPosSet(sprId, 288.0f, 144.0f);
    for (i = 0; i <= 12u; i++) {
        float scale = i / 12.0f;
        espScaleSet(sprId, scale, scale);
        mbTutorialVSleep();
    }
    return sprId;
}

void mbTutorialSprKill(s16 sprId)
{
    int i;

    for (i = 0; i < 16; i++) {
        if (sprId == tutorialSprId[i]) {
            break;
        }
    }
    espKill(sprId);
    tutorialSprId[i] = -1;
}

void mbTutorialSprDispOff(s16 sprId)
{
    int i;

    for (i = 0; i <= 12u; i++) {
        float scale = 1.0f - (i / 12.0f);
        espScaleSet(sprId, scale, scale);
        mbTutorialVSleep();
    }
    mbTutorialSprKill(sprId);
}

static void TutorialSprClose(void)
{
    int i;

    for (i = 0; i < 16; i++) {
        if (tutorialSprId[i] >= 0) {
            espKill(tutorialSprId[i]);
        }
        tutorialSprId[i] = -1;
    }
}

void mbTutorialSprGrpSet(s16 grpId)
{
    int i;

    for (i = 0; i < 16; i++) {
        if (tutorialSprGrpId[i] < 0) {
            break;
        }
    }
    tutorialSprGrpId[i] = grpId;
}

void mbTutorialSprGrpKill(s16 grpId)
{
    int i;

    for (i = 0; i < 16; i++) {
        if (grpId == tutorialSprGrpId[i]) {
            break;
        }
    }
    HuSprGrpKill(grpId);
    tutorialSprId[i] = -1;
}

static void TutorialSprGrpClose(void)
{
    int i;

    for (i = 0; i < 16; i++) {
        if (tutorialSprGrpId[i] >= 0) {
            HuSprGrpKill(tutorialSprGrpId[i]);
        }
        tutorialSprGrpId[i] = -1;
    }
}

MBMODELID mbTutorialModelCreate(int dataNum, BOOL linkF)
{
    MBMODELID modelId;
    int i;

    for (i = 0; i < 32; i++) {
        if (tutorialMdlId[i] < 0) {
            break;
        }
    }
    modelId = tutorialMdlId[i] = mbObjCreate(dataNum, NULL, linkF);
    return modelId;
}

void mbTutorialModelKill(MBMODELID modelId)
{
    int i;

    for (i = 0; i < 32; i++) {
        if (modelId == tutorialMdlId[i]) {
            break;
        }
    }
    mbObjKill(modelId);
    tutorialMdlId[i] = -1;
}

static void TutorialModelKillAll(void)
{
    int i;

    for (i = 0; i < 32; i++) {
        if (tutorialMdlId[i] >= 0) {
            mbObjKill(tutorialMdlId[i]);
        }
        tutorialMdlId[i] = -1;
    }
}

OMOBJ *mbTutorialGuideCreate(s8 *motTbl, BOOL screenF)
{
    tutorialGuideObj = mbGuideCreateFlag(NULL, motTbl, screenF, FALSE, TRUE);
    mbObjDispSet(mbGuideModelGet(tutorialGuideObj), FALSE);
    mbGuideMotionNextSet(tutorialGuideObj, 1);
    mbGuideMotionShiftSet(tutorialGuideObj, 1, FALSE);
    return tutorialGuideObj;
}

void mbTutorialGuideClose(OMOBJ *obj)
{
    mbGuideKill(obj);
    tutorialGuideObj = NULL;
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

static void TutorialWinInit(void)
{
    int i;

    for (i = 0; i < HUWIN_MAX; i++) {
        tutorialWinData[i].delay = tutorialWinData[i].stat = -1;
        tutorialWinData[i].time = -1;
    }
}

static void TutorialWinUpdate(void)
{
    int i;

    for (i = 0; i < HUWIN_MAX; i++) {
        if (winData[i].grpId != HUSPR_GROUP_NONE) {
            if (tutorialWinData[i].stat < 0) {
                tutorialWinData[i].stat = tutorialWinData[i].time = 0;
                tutorialWinData[i].delay = 0;
            }
            if (winData[i].stat == 2) {
                tutorialWinData[i].stat = 1;
                tutorialWinData[i].delay = 1;
            } else if (winData[i].stat != 0 && tutorialWinData[i].stat != 0) {
                if (tutorialWinData[i].delay != 0) {
                    tutorialWinData[i].delay--;
                } else {
                    tutorialWinData[i].stat = 0;
                    tutorialWinData[i].time++;
                }
            }
        } else {
            tutorialWinData[i].stat = tutorialWinData[i].time = -1;
        }
    }
}

BOOL mbTutorialWinWait(int winNo)
{
    MBWIN *win;
    BOOL waitF = FALSE;

    while (TRUE) {
        mbTutorialVSleep();
        if (mbWinDoneCheck(winNo)) {
            break;
        }
        win = mbWinGet(winNo);
        if (win->winId < 0) {
            break;
        }
        if (tutorialWinData[win->winId].stat < 0) {
            break;
        }
        if (tutorialWinData[win->winId].time > 0) {
            tutorialWinData[win->winId].time--;
            waitF = TRUE;
            break;
        }
    }
    return waitF;
}

void mbTutorialWinClose(int winNo)
{
    MBWIN *win;

    if (!mbWinDoneCheck(winNo)) {
        win = mbWinGet(winNo);
        if (win->winId >= 0 && tutorialWinData[win->winId].stat >= 0) {
            tutorialWinData[win->winId].stat = tutorialWinData[win->winId].time = 0;
        }
    }
}
