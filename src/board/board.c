#define _DOLPHIN_MATH
#define _MATH_H

#include "game/board/main.h"
#include "game/board/pause.h"
#include "game/board/player.h"
#include "game/hu3d.h"
#include "game/data.h"
#include "game/gamework.h"
#include "game/flag.h"
#include "game/memory.h"
#include "game/object.h"
#include "game/process.h"
#include "game/sprite.h"
#include "game/saveload.h"
#include "string.h"

typedef void (*MBHOOK)(void);

static inline void GWMgPackSet(s32 value)
{
    GwSystem.mgPack = value;
}

static void mbOMDestroy(void);
static void mbMain(void);
static void mbMainKill(void);
void mbClose(void);
void mbInit(void);
void mbNextTime(void);
BOOL mbNextTimeSet(void);
void mbev_NextTimeSet(MBHOOK hook);
void mbLightSet(void);

static Vec lightPos = {-500.0f, 500.0f, 2000.0f};
static Vec lightDir = {0.5f, -0.5f, -2.0f};
static OMOVL nextOvl = DLL_NONE;
static s32 lbl_802BFBAC = 20;
static s32 lbl_802BFBB0 = 20;
static GXColor lightColor = {255, 255, 255, 255};
static const float lbl_802C3088 = 10000.0f;
static const float lbl_802C308C = -45.0f;
static const float lbl_802C3090 = 0.0f;
#pragma force_active on
static const float gap_11_802C3094 = 0.0f;
#pragma force_active reset

static s32 gap_10_802C0D14;
static MBHOOK ev_NextTime;
static MBHOOK ev_LoadTime;
static MBHOOK ev_TurnEnd;
static MBHOOK ev_TurnStart;
static MBHOOK lightResetFunc;
static MBHOOK lightSetFunc;
static MBHOOK closeHook;
static MBHOOK initHook;
BOOL mbSaveNewF;
HUPROCESS *mbMainProc;
OMOBJMAN *mbObjMan;

extern void mbBoardDataDirRead(void);
extern void mbMathInit(void);
extern void mbMathClose(void);
extern void mbSNpcInit(void);
extern void mbTutorialInit(void);
extern void mbTutorialCreate(void);
extern void mbTutorialCall(s32 mode);
extern BOOL mbTutorialExitReqGet(void);
extern void mbConfigPadDisableSet(BOOL disableF);
extern void mbExitWatchCreate(void);
extern void mbExitReq(void);
extern void mbWipeWait(void);
extern void mbWipeFadeIn(void);
extern void mbWipeDissolveFadeOut(void);
extern void mbWipeSpecialInit(void);
extern void mbWipeSpecialClose(void);
extern void mbCameraInit(void);
extern void mbCameraZoomSet(float zoom);
extern void mbCameraRotSet(float x, float y, float z);
extern void mbCameraCenterSet(float x, float y, float z);
extern void mbCameraMoveOnSet(BOOL onF);
extern void mbCameraMoveStop(void);
extern void mbAudInit(void);
extern void mbAudClose(void);
extern void mbObjInit(void);
extern void mbObjClose(void);
extern void mbPauseInit(void);
extern void mbWinInit(void);
extern void mbWinClose(void);
extern void mbEffInit(void);
extern void mbEffClose(void);
extern void mbGateInit(void);
extern void mbGateClose(void);
extern void mbCapInit(void);
extern void mbCoinInit(void);
extern void mbCoinClose(void);
extern void mbBranchInit(void);
extern void mbStarInit(void);
extern void mbStarClose(void);
extern void mbStarFlagInit(void);
extern void mbStarNextNoSet(s32 no);
extern s32 mbStarNoRandGet(void);
extern void mbStarNoSet(s32 no);
extern void mbStarReset(void);
extern void mbMasuCapsuleReset(void);
extern void mbev_CapBankCoinInit(void);
extern void mbev_CapInit(void);
extern void mbMasuClose(void);
extern void mbMasuPlayerPrizeReset(s32 playerNo);
extern void mbStatusInit(void);
extern void mbStatusClose(void);
extern void mbStatusDispForceSetAll(BOOL dispF);
extern void mbStatusColorAllSet(s32 color);
extern void mbStatusMasuDispSet(BOOL dispF);
extern void mbDiceInit(void);
extern void mbDiceClose(void);
extern void mbTauntInit(void);
extern void mbTauntClose(void);
extern void mbScrollClose(void);
extern void fn_8019A620(void);
extern void mbSingleInit(void);
extern void mbSingleClose(void);
extern void mbSingleSaveInit(s32 teamChar, s32 mgPack, s32 storyComDif);
extern void mbSingleGameEnd(void);
extern s32 mbSingleCall(s32 mode, s32 arg);
extern s32 mbev_SingleMgEnd(s32 playerNo);
extern s32 mbev_MgCall(void);
extern void mbMgCallDataClose(void);
extern void mbMgCallInit(void);
extern void mbev_Opening(void);
extern void mbev_Last5(void);
extern void mbev_CapKettouEndCall(s32 playerNo);
extern void mbev_CapDonkeyEndCall(s32 playerNo);
extern void mbev_CapKoopaEndCall(s32 playerNo);
extern void mbMusBoardPlay(void);
extern void mbTelopLastTurnCreate(void);
extern void mbTelopTimeChangeCreate(void);
extern void mbTelopTimeChangeKill(void);
extern BOOL mbTelopTimeChangeCheck(void);
extern void HuAudAllStop(void);
extern void mbLightFuncSet(MBHOOK setHook, MBHOOK resetHook);
extern void mbDirClose(void);
extern void mbBGReadWait(s32 statId);
extern s32 mbBGRead(s32 dataNum);
void evdef_ChangeTime(void);
BOOL mbReturnMgCheck(void);

void mbObjectSetup(s32 boardNo, MBHOOK init, MBHOOK close)
{
    s16 nightF;

    omSysPauseEnable(FALSE);
    mbPauseDisableSet(TRUE);
    mbSaveNewF = !_CheckFlag(FLAG_BOARD_SAVEINIT);
    mbBoardDataDirRead();
    mbMathInit();
    if (mbSaveNewF) {
        if (!_CheckFlag(FLAG_BOARD_TUTORIAL)) {
            if (GWPartyGet() != FALSE) {
                GWMgInstDispSet(GwCommon.partyMgInstDispF);
                GWMgComDispSet(GwCommon.partyMgComDispF);
                GWMgPackSet(GwCommon.partyMgPack);
                GWMessSpeedSet(GwCommon.partyMessSpeed);
                GWSaveModeSet(GwCommon.partySaveMode);
            } else {
                GWMgInstDispSet(GwCommon.storyMgInstDispF);
                GWMgComDispSet(GwCommon.storyMgComDispF);
                GWMgPackSet(GwCommon.storyMgPack);
                GWMessSpeedSet(GwCommon.storyMessSpeed);
                GWSaveModeSet(GwCommon.storySaveMode);
            }
        } else {
            GwSystem.curTime = 0;
            GwMgNightF = 0;
        }
        _SetFlag(FLAG_BOARD_SAVEINIT);
        _ClearFlag(FLAG_BOARD_OPENING);
        _ClearFlag(FLAG_BOARD_MG);
        _ClearFlag(FLAG_BOARD_MG_KETTOU);
        _ClearFlag(FLAG_BOARD_MG_DONKEY);
        _ClearFlag(FLAG_BOARD_MG_KOOPA);
        _ClearFlag(FLAG_BOARD_MG_VS);
        GwSystem.turnNo = 1;
        mbStarFlagInit();
        mbStarNextNoSet(0);
        GwSystem.starTotal = 0;
        GwSystem.last5Effect = 0;
        GwSystem.curTime = GwSystem.nextTime = 0;
        nightF = GwMgNightF;
        if (nightF == 1) {
            GwSystem.curTime = GwSystem.nextTime = 1;
        }
        GwSystem.timeTurn = 0;
        GwSystem.timeTurnMax = 3;
        _ClearFlag(FLAGNUM(FLAG_GROUP_BOARD, 1));
        _ClearFlag(FLAGNUM(FLAG_GROUP_BOARD, 3));
        _ClearFlag(FLAGNUM(FLAG_GROUP_BOARD, 7));
        _ClearFlag(FLAGNUM(FLAG_GROUP_BOARD, 8));
        _ClearFlag(FLAGNUM(FLAG_GROUP_BOARD, 0));
        _ClearFlag(FLAGNUM(FLAG_GROUP_BOARD, 2));
        _ClearFlag(FLAGNUM(FLAG_GROUP_BOARD, 4));
        _ClearFlag(FLAGNUM(FLAG_GROUP_BOARD, 6));
        _ClearFlag(3);
        mbMgCallInit();
        mbStarReset();
        mbMasuCapsuleReset();
        mbev_CapBankCoinInit();
        saveExecF = FALSE;
    }
    if (_CheckFlag(FLAGNUM(FLAG_GROUP_COMMON, 35))) {
        mbMgCallInit();
        mbStarReset();
        _ClearFlag(FLAGNUM(FLAG_GROUP_COMMON, 35));
        saveExecF = TRUE;
    }
    nextOvl = DLL_NONE;
    _SetFlag(FLAGNUM(FLAG_GROUP_COMMON, 34));
    _SetFlag(FLAG_MG_CIRCUIT);
    _ClearFlag(FLAGNUM(FLAG_GROUP_COMMON, 21));
    _ClearFlag(FLAGNUM(FLAG_GROUP_COMMON, 22));
    _ClearFlag(FLAGNUM(FLAG_GROUP_COMMON, 23));
    _ClearFlag(FLAGNUM(FLAG_GROUP_COMMON, 26));
    _ClearFlag(FLAGNUM(FLAG_GROUP_COMMON, 28));
    _ClearFlag(FLAGNUM(FLAG_GROUP_COMMON, 29));
    _ClearFlag(FLAG_BOARD_TURN_NOSTART);
    mbConfigPadDisableSet(TRUE);
    initHook = init;
    closeHook = close;
    lightSetFunc = lightResetFunc = NULL;
    mbSNpcInit();
    mbTutorialInit();
    mbev_CapInit();
    ev_TurnStart = ev_TurnEnd = NULL;
    ev_LoadTime = ev_NextTime = NULL;
    mbev_NextTimeSet((MBHOOK)evdef_ChangeTime);
    mbExitWatchCreate();
    mbObjMan = omInitObjMan(64, 8194);
    HuPrcDestructorSet2(mbObjMan, mbOMDestroy);
    omSystemKeyCheckSetup(mbObjMan);
    mbMainProc = HuPrcChildCreate(mbMain, 8208, 0x8000, 0, mbObjMan);
    HuPrcDestructorSet2(mbMainProc, mbMainKill);
    GwSystem.boardNo = boardNo;
}

static void mbOMDestroy(void)
{
    s32 ovl;
    mbClose();
    if (nextOvl != DLL_NONE) {
        omOvlCallEx(nextOvl, TRUE, 0, 0);
        return;
    }
    if (_CheckFlag(FLAG_BOARD_MOVE_DONE)) {
        ovl = omCurrentOvlGet();
        _ClearFlag(FLAG_BOARD_MOVE_DONE);
        omOvlGotoEx(ovl, TRUE, 0, 0);
        return;
    }
    _ClearFlag(FLAG_BOARD_SAVEINIT);
    if (_CheckFlag(FLAG_BOARD_TURN_NOSTART)) {
        omOvlReturnEx(1, TRUE);
    } else {
        if (GWPartyGet() != FALSE && !_CheckFlag(FLAG_BOARD_TUTORIAL)) {
            omOvlGotoEx(92, TRUE, 0, 0);
        } else {
            omOvlReturnEx(1, TRUE);
        }
    }
}

static void mbMain(void)
{
    s32 i;
    s32 interruptF = FALSE;
    s32 mgCallF;
    s32 curTime;
    s32 nightF;

    mbWipeWait();
    if (GwSystem.turnNo > GwSystem.turnMax && !_CheckFlag(FLAG_BOARD_TUTORIAL)) {
        _ClearFlag(FLAG_BOARD_MOVE_DONE);
        mbExitReq();
        HuPrcSleep(-1);
    }
    mbInit();
    if (!_CheckFlag(FLAG_BOARD_OPENING)) {
        if (_CheckFlag(FLAG_BOARD_DEBUG) && !_CheckFlag(FLAG_BOARD_TUTORIAL)) {
            if (GWPartyGet() != FALSE) {
                s32 starNo = mbStarNoRandGet();
                if (starNo >= 0) {
                    mbStarNoSet(starNo);
                }
            }
        } else {
            GwSystem.turnPlayerNo = -1;
            if (!_CheckFlag(FLAG_BOARD_TUTORIAL)) {
                mbev_Opening();
            } else {
                mbMusBoardPlay();
                _ClearFlag(FLAG_BOARD_STAR_RESET);
                mbTutorialCall(0);
            }
        }
        if (GWPartyGet() != FALSE) {
            if (!GWTeamFGet()) {
                for (i = 0; i < 4; i++) {
                    mbPlayerCoinSet(i, 10);
                }
            } else {
                for (i = 0; i < 2; i++) {
                    mbPlayerTeamCoinSet(i, 20);
                }
            }
        } else {
            for (i = 0; i < 4; i++) {
                mbPlayerCoinSet(i, 0);
            }
        }
        GwSystem.turnPlayerNo = 0;
        _SetFlag(FLAG_BOARD_OPENING);
    }
    if (GwSystem.curTime != GwSystem.nextTime) {
        GwSystem.timeTurn = 0;
        curTime = GwSystem.curTime;
        GwMgNightF = curTime;
        if (ev_NextTime != NULL) {
            mbMusBoardPlay();
            _ClearFlag(FLAG_BOARD_STAR_RESET);
            ev_NextTime();
            mbTelopTimeChangeKill();
            mbCameraMoveStop();
        }
        GwSystem.nextTime = GwSystem.curTime;
    }
    {
        BOOL dayF;

        dayF = (GwSystem.curTime == 0);
        nightF = dayF ? FALSE : TRUE;
        GwMgNightF = nightF;
    }
    if (mbReturnMgCheck()) {
        if (GWPartyGet() == FALSE) {
            interruptF = mbev_SingleMgEnd(0);
        } else {
            if (_CheckFlag(FLAG_BOARD_MG)) {
                _ClearFlag(FLAG_BOARD_MG);
            } else if (_CheckFlag(FLAG_BOARD_MG_KETTOU)) {
                mbev_CapKettouEndCall(GwSystem.turnPlayerNo);
                _ClearFlag(FLAG_BOARD_MG_KETTOU);
                interruptF = TRUE;
            } else if (_CheckFlag(FLAG_BOARD_MG_DONKEY)) {
                mbev_CapDonkeyEndCall(GwSystem.turnPlayerNo);
                _ClearFlag(FLAG_BOARD_MG_DONKEY);
                interruptF = TRUE;
            } else if (_CheckFlag(FLAG_BOARD_MG_KOOPA)) {
                mbev_CapKoopaEndCall(GwSystem.turnPlayerNo);
                _ClearFlag(FLAG_BOARD_MG_KOOPA);
                interruptF = TRUE;
            } else if (_CheckFlag(FLAGNUM(FLAG_GROUP_COMMON, 19))) {
                if (ev_LoadTime != NULL) {
                    ev_LoadTime();
                }
                _ClearFlag(FLAGNUM(FLAG_GROUP_COMMON, 19));
                interruptF = TRUE;
            }
            _ClearFlag(FLAG_BOARD_MOVE_DONE);
        }
    }
    while (1) {
    if (ev_TurnStart && !interruptF) {
        ev_TurnStart();
    }
    if (GwSystem.turnPlayerNo == 0 && !interruptF && GwSystem.turnMax - GwSystem.turnNo < 5) {
        if (!_CheckFlag(FLAG_BOARD_LAST5) && GWPartyGet() != FALSE) {
            mbev_Last5();
            _SetFlag(FLAG_BOARD_LAST5);
        } else {
            mbTelopLastTurnCreate();
            if (!_CheckFlag(FLAG_BOARD_LAST5)) {
                if (GWPartyGet() == FALSE) {
                    mbSingleCall(12, 0);
                    _SetFlag(FLAG_BOARD_LAST5);
                }
            }
        }
    }
    mbMusBoardPlay();
    _ClearFlag(FLAG_BOARD_STAR_RESET);
    if (_CheckFlag(FLAG_BOARD_TUTORIAL)) {
        mbTutorialCall(1);
        if (mbTutorialExitReqGet()) {
            HuPrcSleep(-1);
        }
    }
    if (GWPartyGet() != FALSE) {
        mbStatusDispForceSetAll(TRUE);
        mbTurnExec(interruptF);
        if (ev_TurnEnd) {
            ev_TurnEnd();
        }
        GwSystem.turnNo++;
        GwSystem.timeTurn++;
        GwSystem.turnPlayerNo = 0;
        if (_CheckFlag(FLAG_BOARD_TUTORIAL)) {
            mbTutorialCall(2);
            mbStatusColorAllSet(0);
            mbNextTime();
            continue;
        } else if (_CheckFlag(FLAG_BOARD_NOMG)) {
            interruptF = FALSE;
            mbStatusColorAllSet(0);
            mbNextTime();
            continue;
        } else {
            GwSystem.turnPlayerNo = -1;
            mgCallF = mbev_MgCall();
            mbStatusColorAllSet(0);
            if (!mgCallF) {
                mbNextTimeSet();
                HuPrcSleep(-1);
            } else {
                GwSystem.turnPlayerNo = 0;
                mbNextTime();
            }
        }
    } else {
        mbStatusDispForceSetAll(TRUE);
        mbStatusMasuDispSet(TRUE);
        mbSingleTurnExec(interruptF);
        if (ev_TurnEnd) {
            ev_TurnEnd();
        }
        GwSystem.turnNo++;
        if (GwSystem.turnNo > GwSystem.turnMax) {
            mbSingleGameEnd();
            HuPrcSleep(-1);
        }
        GwSystem.turnPlayerNo = 0;
        if (_CheckFlag(FLAG_BOARD_NOMG)) {
            interruptF = FALSE;
            mbStatusColorAllSet(0);
            continue;
        }
        if (_CheckFlag(FLAG_BOARD_TUTORIAL)) {
            mbStatusColorAllSet(0);
            continue;
        }
        GwSystem.turnPlayerNo = -1;
        mbStatusColorAllSet(0);
        GwSystem.turnPlayerNo = 0;
    }
    interruptF = FALSE;
    }
}

static void mbMainKill(void)
{
    mbMainProc = NULL;
}

void mbChangeTimeSet(void)
{
    GwSystem.nextTime = GwSystem.curTime;
    GwSystem.curTime ^= 1;
}

BOOL mbNextTimeSet(void)
{
    s32 timeTurnMax = GwSystem.timeTurnMax;
    s32 timeTurn = GwSystem.timeTurn;

    if (timeTurn >= timeTurnMax) {
        mbChangeTimeSet();
        return TRUE;
    }
    return FALSE;
}

void mbChangeTime(void)
{
    nextOvl = DLL_NONE;
    _SetFlag(FLAG_BOARD_MOVE_DONE);
    _SetFlag(FLAGNUM(FLAG_GROUP_COMMON, 19));
    mbChangeTimeSet();
    mbExitReq();
    HuPrcSleep(-1);
}

void mbNextTime(void)
{
    if (mbNextTimeSet()) {
        nextOvl = DLL_NONE;
        _SetFlag(FLAG_BOARD_MOVE_DONE);
        _SetFlag(FLAG_BOARD_MG);
        mbExitReq();
        HuPrcSleep(-1);
    }
}

void evdef_ChangeTime(void)
{
    mbCameraZoomSet(lbl_802C3088);
    mbCameraRotSet(lbl_802C308C, lbl_802C3090, lbl_802C3090);
    mbCameraCenterSet(lbl_802C3090, lbl_802C3090, lbl_802C3090);
    mbCameraMoveOnSet(FALSE);
    mbWipeFadeIn();
    mbTelopTimeChangeCreate();
    while (mbTelopTimeChangeCheck()) {
        HuPrcVSleep();
    }
    mbWipeDissolveFadeOut();
}

void mbOvlCall(OMOVL ovl)
{
    nextOvl = ovl;
    mbExitReq();
}

BOOL mbReturnMgCheck(void)
{
    if (_CheckFlag(FLAG_BOARD_MG)
        || _CheckFlag(FLAG_BOARD_MG_KETTOU)
        || _CheckFlag(FLAG_BOARD_MG_KOOPA)
        || _CheckFlag(FLAG_BOARD_MG_DONKEY)
        || _CheckFlag(FLAGNUM(FLAG_GROUP_COMMON, 19))
        || _CheckFlag(FLAG_BOARD_MOVE_DONE)) {
        return TRUE;
    }
    return FALSE;
}

void mbInit(void)
{
    if (!SLSaveFlagGet()) {
        GWSaveModeSet(GW_SAVE_MODE_NEVER);
    }
    if (GWMgInstDispGet()) {
        _SetFlag(5);
    } else {
        _ClearFlag(5);
    }
    if (!mbReturnMgCheck()) {
        _ClearFlag(FLAG_BOARD_MG);
        _ClearFlag(FLAG_BOARD_MG_KETTOU);
        _ClearFlag(FLAG_BOARD_MG_KOOPA);
        _ClearFlag(FLAG_BOARD_MG_DONKEY);
        _ClearFlag(FLAG_BOARD_MG_VS);
        _ClearFlag(FLAGNUM(FLAG_GROUP_COMMON, 19));
        _ClearFlag(FLAG_BOARD_MOVE_DONE);
    }
    if (mbReturnMgCheck()) {
        mbMgCallDataClose();
    }
    HuSprExecLayerCameraSet(32, 2, 7);
    HuSprExecLayerCameraSet(64, 4, 3);
    mbCameraInit();
    mbAudInit();
    mbObjInit();
    mbPauseInit();
    mbWinInit();
    mbEffInit();
    mbWipeSpecialInit();
    mbGateInit();
    mbCapInit();
    mbCoinInit();
    mbBranchInit();
    mbStarInit();
    mbPlayerTurnInitHookSet(NULL);
    mbPlayerTurnCloseHookSet(NULL);
    initHook();
    mbPlayerInit(!mbReturnMgCheck());
    mbStatusInit();
    mbDiceInit();
    mbLightSet();
    CharModelDataClose(-1);
    mbTauntInit();
    Hu3DReflectNoSet(0);
    mbCapMasuObjInit();
    if (GWPartyGet() == FALSE) {
        mbSingleInit();
    }
    if (_CheckFlag(FLAG_BOARD_TUTORIAL)) {
        mbTutorialCreate();
    }
    _SetFlag(FLAGNUM(FLAG_GROUP_COMMON, 21));
}

void mbClose(void)
{
    HuMemHeapDump(HuMemHeapPtrGet(3), -1);
    if (!_CheckFlag(FLAGNUM(FLAG_GROUP_COMMON, 21))) {
        mbDirClose();
        mbMathClose();
        return;
    }
    if (GWPartyGet() == FALSE) {
        mbSingleClose();
    }
    mbTauntClose();
    mbAudClose();
    HuAudAllStop();
    mbDiceClose();
    mbStatusClose();
    mbMasuClose();
    mbStarClose();
    mbGateClose();
    fn_8019A620();
    mbScrollClose();
    mbPlayerClose();
    if (closeHook) {
        closeHook();
    }
    mbCoinClose();
    mbEffClose();
    mbObjClose();
    mbWinClose();
    mbDirClose();
    initHook = closeHook = NULL;
    ev_TurnStart = NULL;
    ev_TurnEnd = NULL;
    ev_LoadTime = NULL;
    ev_NextTime = NULL;
    _SetFlag(FLAGNUM(FLAG_GROUP_COMMON, 34));
    _SetFlag(FLAG_MG_CIRCUIT);
    _SetFlag(FLAGNUM(FLAG_GROUP_COMMON, 22));
    mbMathClose();
}

void mbDirClose(void)
{
    HuDataDirCloseAll();
}

void mbLightFuncSet(MBHOOK setHook, MBHOOK resetHook)
{
    lightSetFunc = setHook;
    lightResetFunc = resetHook;
}

void mbLightSet(void)
{
    HU3D_LIGHTID lightId;
    Hu3DBGColorSet(0, 0, 0);
    if (lightSetFunc) {
        lightSetFunc();
    }
    lightId = (s16)Hu3DGLightCreateV(&lightPos, &lightDir, &lightColor);
    Hu3DGLightStaticSet(lightId, FALSE);
    Hu3DGLightInfinitytSet((s16)lightId);
}

void mbLightReset(void)
{
    if (lightResetFunc) {
        lightResetFunc();
    }
    Hu3DBGColorSet(0, 0, 0);
    Hu3DFogClear();
    Hu3DReflectNoSet(0);
}

void mbPauseEnableSet(void)
{
    _SetFlag(FLAGNUM(FLAG_GROUP_COMMON, 23));
}

void mbPauseEnableReset(void)
{
    _ClearFlag(FLAGNUM(FLAG_GROUP_COMMON, 23));
}

BOOL mbPauseEnableCheck(void)
{
    return _CheckFlag(FLAGNUM(FLAG_GROUP_COMMON, 23)) ? TRUE : FALSE;
}

s32 mbBGRead(s32 dataNum)
{
    return HuDataDirReadAsync(dataNum);
}

void mbBGReadWait(s32 statId)
{
    if (statId == HU_DATA_STAT_NONE) {
        return;
    }
    while (HuDataGetAsyncStat(statId) == FALSE) {
        HuPrcVSleep();
    }
}

void mbev_NextTimeSet(MBHOOK hook)
{
    ev_NextTime = hook;
}

void mbev_LoadTimeSet(MBHOOK hook)
{
    ev_LoadTime = hook;
}

void mbev_TurnStartSet(MBHOOK hook)
{
    ev_TurnStart = hook;
}

void mbev_TurnEndSet(MBHOOK hook)
{
    ev_TurnEnd = hook;
}

void fn_8014C3B4(s32 value)
{
    lbl_802BFBAC = value;
}

void fn_8014C3BC(s32 value)
{
    lbl_802BFBB0 = value;
}

s32 fn_8014C3C4(void)
{
    return lbl_802BFBAC;
}

s32 fn_8014C3CC(void)
{
    return lbl_802BFBB0;
}

void mbSaveInit(s32 boardNo)
{
    s32 i;
    s32 j;
    GwSystem.boardNo = boardNo;
    _ClearFlag(FLAG_BOARD_SAVEINIT);
    _ClearFlag(FLAG_MG_PRACTICE);
    GwSystem.mgNo = 0;
    GwSystem.subGameNo = 0;
    GwSystem.hiddenBlockMasuId = 0;
    memset(&GwSystem.boardWork[0], 0, sizeof(GwSystem.boardWork));
    GwSystem.turnNo = 1;
    for (i = 0; i < GW_PLAYER_MAX; i++) {
        GwPlayer[i].coin = 0;
        if (GWPartyGet() == FALSE || _CheckFlag(FLAG_BOARD_TUTORIAL)) {
            GwPlayer[i].star = 0;
            GwSystem.tagF = FALSE;
        } else {
            GwPlayer[i].star = mbPlayerHandicapGet(i);
        }
        mbMasuPlayerPrizeReset(i);
        GwPlayer[i].coinTotalMg = 0;
        GwPlayer[i].coinTotal = 0;
        GwPlayer[i].coinMax = 0;
        GwPlayer[i].starMax = 0;
        GwPlayer[i].coinBattle = 0;
        GwPlayer[i].mgCoin = 0;
        GwPlayer[i].mgCoinBonus = 0;
        GwPlayer[i].capsuleUseNum = 0;
        for (j = 0; j < 3; j++) {
            GwPlayer[i].capsule[j] = -1;
        }
    }
}

void mbSaveStoryInit(s32 teamChar, s32 mgPack, s32 storyComDif)
{
    mbSingleSaveInit(teamChar, mgPack, storyComDif);
    GwMgNightF = 0;
}

void mbSavePartyInit(s32 boardNo, s32 storyDif, s32 mgPack, s32 turnMax,
                     s32 handicapP1, s32 handicapP2, s32 handicapP3, s32 handicapP4)
{
    GwSystem.partyF = TRUE;
    GwSystem.tagF = boardNo;
    GwSystem.storyComDif = 0;
    GwSystem.bonusStarF = storyDif;
    GwSystem.mgPack = mgPack;
    GwSystem.turnMax = turnMax;
    GwPlayer[0].handicap = handicapP1;
    GwPlayer[1].handicap = handicapP2;
    GwPlayer[2].handicap = handicapP3;
    GwPlayer[3].handicap = handicapP4;
    _ClearFlag(FLAG_BOARD_TUTORIAL);
    _SetFlag(5);
    _SetFlag(FLAGNUM(FLAG_GROUP_COMMON, 13));
    _ClearFlag(FLAG_BOARD_INIT);
    _ClearFlag(FLAGNUM(FLAG_GROUP_SYSTEM, 2));
    GwSystem.curTime = 0;
    GwMgNightF = 0;
}

void fn_8014C818(void)
{
}

void fn_8014C81C(void)
{
}
