/* player.o has no weak sqrtf constants in .sdata2. */
#define _MATH_H
#include "dolphin/math.h"
#include "dolphin/os.h"

#include "game/board/audio.h"
#include "game/board/branch.h"
#include "game/board/camera.h"
#include "game/board/coin.h"
#include "game/board/effect.h"
#include "game/board/gate.h"
#include "game/board/main.h"
#include "game/board/masu.h"
#include "game/board/object.h"
#include "game/board/pause.h"
#include "game/board/player.h"
#include "game/board/status.h"
#include "game/board/tutorial.h"
#include "game/audio.h"
#include "game/charman.h"
#include "game/data.h"
#include "game/frand.h"
#include "game/process.h"
#include "game/msm.h"

#include "string.h"

enum {
    MESS_CHARANAME_MARIO,
    MESS_CHARANAME_LUIGI,
    MESS_CHARANAME_PEACH,
    MESS_CHARANAME_YOSHI,
    MESS_CHARANAME_WARIO,
    MESS_CHARANAME_DAISY,
    MESS_CHARANAME_WALUIGI,
    MESS_CHARANAME_KINOPIO,
    MESS_CHARANAME_TERESA,
    MESS_CHARANAME_MINIKOOPA,
    MESS_CHARANAME_KINOPICO,
    MESS_CHARANAME_MINIKOOPAR,
    MESS_CHARANAME_MINIKOOPAG,
    MESS_CHARANAME_MINIKOOPAB
};

#define FLAG_BOARD_WALKDONE FLAGNUM(FLAG_GROUP_COMMON, 16)

static MBPLAYERWORK playerWork[GW_PLAYER_MAX];
static BOOL turnIntrF;
static BOOL blackoutF;
static void (*turnInitHook)(int playerNo);
static void (*turnCloseHook)(int playerNo);
static GXColor metalShadowColor;
static GXColor metalHiliteColor;
static BOOL playerColSnapF;

typedef struct PlayerColWork {
    u8 motStartF : 1;
    u8 killF : 1;
    u8 snapF : 1;
    u8 restF : 1;
    u8 playerNo : 2;
    u8 state : 2;
    u8 circleF;
    u8 masuId;
    u8 masuIdNext;
    s8 time;
    s8 maxTime;
    u8 _pad06[2];
    float rotYStart;
    float radius;
} PLAYERCOLWORK;

static GXColor metalDefaultColor[2] = {
    { 128, 190, 140, 255 },
    { 100, 50, 130, 255 }
};

#define CHAR_MDLFILE(name) DATA_##name##mdl1
#define CHAR_MOTDIR(name) DATA_##name##mot

static const int charMdlFileTbl[CHARNO_MAX] = {
    CHAR_MDLFILE(mario),
    CHAR_MDLFILE(luigi),
    CHAR_MDLFILE(peach),
    CHAR_MDLFILE(yoshi),
    CHAR_MDLFILE(wario),
    CHAR_MDLFILE(daisy),
    CHAR_MDLFILE(waluigi),
    CHAR_MDLFILE(kinopio),
    CHAR_MDLFILE(teresa),
    CHAR_MDLFILE(minikoopa),
    CHAR_MDLFILE(kinopiko),
    CHAR_MDLFILE(minikoopaR),
    CHAR_MDLFILE(minikoopaG),
    CHAR_MDLFILE(minikoopaB)
};

static const int charMotDirTbl[CHARNO_MAX] = {
    CHAR_MOTDIR(mario),
    CHAR_MOTDIR(luigi),
    CHAR_MOTDIR(peach),
    CHAR_MOTDIR(yoshi),
    CHAR_MOTDIR(wario),
    CHAR_MOTDIR(daisy),
    CHAR_MOTDIR(waluigi),
    CHAR_MOTDIR(kinopio),
    CHAR_MOTDIR(teresa),
    CHAR_MOTDIR(minikoopa),
    CHAR_MOTDIR(kinopiko),
    CHAR_MOTDIR(minikoopa),
    CHAR_MOTDIR(minikoopa),
    CHAR_MOTDIR(minikoopa)
};

static const u16 charMotNoTbl[15] = {
    CHAR_MOTNO(CHARMOT_HSF_c000m1_300),
    CHAR_MOTNO(CHARMOT_HSF_c000m1_301),
    CHAR_MOTNO(CHARMOT_HSF_c000m1_302),
    CHAR_MOTNO(CHARMOT_HSF_c000m1_303),
    CHAR_MOTNO(CHARMOT_HSF_c000m1_304),
    CHAR_MOTNO(CHARMOT_HSF_c000m1_322),
    CHAR_MOTNO(CHARMOT_HSF_c000m1_306),
    CHAR_MOTNO(CHARMOT_HSF_c000m1_307),
    CHAR_MOTNO(CHARMOT_HSF_c000m1_324),
    CHAR_MOTNO(CHARMOT_HSF_c000m1_357),
    CHAR_MOTNO(CHARMOT_HSF_c000m1_311),
    CHAR_MOTNO(CHARMOT_HSF_c000m1_346),
    CHAR_MOTNO(CHARMOT_HSF_c000m1_348),
    CHAR_MOTNO(CHARMOT_HSF_c000m1_386),
    CHAR_MOTNO(CHARMOT_HSF_c000m1_320)
};

static const int tutorialCharNoTbl[GW_PLAYER_MAX] = {
    CHARNO_YOSHI,
    CHARNO_MARIO,
    CHARNO_PEACH,
    CHARNO_WARIO
};

static const int singleCharNoTbl[GW_PLAYER_MAX] = {
    CHARNO_MARIO,
    CHARNO_MINIKOOPAR,
    CHARNO_MINIKOOPAG,
    CHARNO_MINIKOOPAB
};

#undef CHAR_MDLFILE
#undef CHAR_MOTDIR

static void PlayerColKill(int playerNo);
static void PlayerColOMExec(OMOBJ *obj);
static void PlayerMetalKill(int playerNo);
static void PlayerMetalOMExec(OMOBJ *objP);
static void ResetMetalColor(void);
static void MetalEffectCreate(OMOBJ *objP);
static void MetalEffectHook(
    HU3D_MODEL *modelP, MBPARTICLE *particleP, Mtx matrix);
static void PlayerBiriQKill(int playerNo);
static void PlayerBiriQFlashSet(int playerNo);
static void PlayerBiriQOMExec(OMOBJ *objP);
static float GetBiriQEffectRadius(
    OMOBJ *objP, int playerNo, int *effectCount);
static void BiriQEffectCreate(OMOBJ *objP);
static void BiriQEffect1Hook(
    HU3D_MODEL *modelP, MBPARTICLE *particleP, Mtx matrix);
static void BiriQEffect2Hook(
    HU3D_MODEL *modelP, MBPARTICLE *particleP, Mtx matrix);
static void PlayerMove(void);
static void PlayerMoveCall(int playerNo);
static void PlayerMoveDestroy(void);
static void PlayerTurn(int playerNo);
static BOOL DiceRun(int playerNo);
static BOOL PlayerViewSet(
    int playerNo, BOOL intrF, BOOL waitF, BOOL carF);
static void MasuCoinExec(int playerNo, int coinNum);
static void ev_PlayerStartTurn(int playerNo);
static void ev_PlayerEndTurn(int playerNo);
int mbSingleTeamCharGet(void);
int mbCapSelect(void);
int mbSingleCall(int mode, int arg);
int mbDiceProcExec(int playerNo, int diceType, s8 *valueTbl,
    int *tutorialVal, BOOL padWinF, BOOL waitF, HuVecF *pos, int color);
int mbDiceExec(int playerNo, int diceType, s8 *valueTbl, int tutorialVal,
    BOOL padWinF, BOOL waitF, HuVecF *pos, int color);
void mbDiceKill(int playerNo);
int mbDiceMaxGet(int diceType);
int mbDiceValueMaxGet(int diceType);
void mbev_Scroll(int playerNo, BOOL mapF);
void mbev_CapKillerMoveCall(int playerNo);
void mbev_CapCallKettou(int playerNo, s16 id, BOOL stopF);
void mbev_CapCallTrap(int playerNo, s16 id, s16 idNext);
void mbev_CapBiriQShockCreate(int playerNo);
void mbStarDispSetAll(BOOL dispF);
void mbStarMasuDispSet(int masuId, BOOL dispF);
void mbTelopTimeCreate(void);
void mbTelopPlayerCreate(int playerNo);
s16 mbCoinDispMasuCreate(HuVecF *pos, int coinNum, BOOL playSe);
void mbCoinAddExec(int playerNo, int coinNum);
void mbDiceNumKill(int playerNo);
void mbDiceObjHit(int playerNo);
void mbObjMetalCreate(MBMODELID modelId);
void mbObjMetalKill(MBMODELID modelId);
void mbObjMetalTPLvlSet(MBMODELID modelId, float level);
void mbObjMetalColorSet(
    MBMODELID modelId, GXColor shadowColor, GXColor hiliteColor);
void mbObjBiriQCreate(MBMODELID modelId);
void mbObjBiriQKill(MBMODELID modelId);
void mbObjBiriQColorSet(
    MBMODELID modelId, BOOL enableF, float level, GXColor color);
BOOL mbWipeSpecialStatGet(void);
void mbWipeFadeIn(void);
void mbWipeFadeOut(void);
void mbWipeDissolveFadeIn(void);
void mbWipeSpecialFadeOutCreate(int type, int time);
void mbWipeSpecialFadeInCreate(int type, int time);
BOOL mbPauseEnableCheck(void);
void mbPos3DtoNorm(HuVecF *src, s16 cameraMask, HuVecF *dst);
float mbSinDeg(float angle);
float mbAngleEaseOut(float angleStart, float angleEnd, float weight);

void mbPlayerInit(BOOL noEventF)
{
    MBPLAYERWORK *workP = &playerWork[0];
    int motDataNum[16];
    int i;
    int j;

    memset(playerWork, 0, sizeof(playerWork));
    ResetMetalColor();
    if (_CheckFlag(FLAG_BOARD_MG)) {
        GwSystem.turnPlayerNo = 0;
    }
    if (noEventF) {
        s16 startMasu = mbMasuFind_AttrIdGet(-1, MASU_FLAG_START);
        int grp;

        for (i = 0; i < GW_PLAYER_MAX; i++) {
            if (_CheckFlag(FLAG_BOARD_TUTORIAL)) {
                GwPlayer[i].comF = TRUE;
                GwPlayerConf[i].type = TRUE;
            }
            if (GwSystem.partyF) {
                _CheckFlag(FLAG_BOARD_TUTORIAL);
            } else if (i > 0) {
                GwPlayerConf[i].charNo = singleCharNoTbl[i];
                GwPlayer[i].comF = TRUE;
                GwPlayerConf[i].type = TRUE;
            }
            GwPlayer[i].charNo = GwPlayerConf[i].charNo;
            GwPlayerConf[i].charNo = GwPlayerConf[i].charNo;
            GwPlayer[i].padNo = GwPlayerConf[i].padNo;
            GwPlayerConf[i].padNo = GwPlayerConf[i].padNo;
            GwPlayer[i].comF = GwPlayerConf[i].type;
            GwPlayerConf[i].type = GwPlayerConf[i].type;
            GwPlayer[i].comDif = GwPlayerConf[i].comDif;
            GwPlayerConf[i].comDif = GwPlayerConf[i].comDif;
            GwPlayer[i].masuId = startMasu;
            GwPlayer[i].masuIdNext = startMasu;
            GwPlayer[i].statusColor = 0;
            GwPlayer[i].diceMode = 0;
            for (j = 0; j < 3; j++) {
                GwPlayer[i].capsule[j] = -1;
            }
            GwPlayer[i].team = grp = GwPlayerConf[i].grpNo;
            GwPlayerConf[i].grpNo = grp;
            GwPlayer[i].orderNo = i;
            mbPlayerMetalSet(i, FALSE);
            mbPlayerBiriQSet(i, FALSE);
        }
        for (i = 0; i < CHARNO_MAX; i++) {
            if (CharMotionAMemPGet(i)) {
                if (!GwSystem.partyF && i == mbSingleTeamCharGet()) {
                    continue;
                }
                for (j = 0; j < GW_PLAYER_MAX; j++) {
                    if (i == GwPlayer[j].charNo) {
                        break;
                    }
                }
                if (j >= GW_PLAYER_MAX) {
                    CharDataClose(i);
                }
            }
        }
        for (i = 0; i < GW_PLAYER_MAX; i++) {
            if (!CharMotionAMemPGet(GwPlayer[i].charNo)) {
                CharMotionInit(GwPlayer[i].charNo);
            }
        }
        if (!GwSystem.partyF) {
            int charNo = mbSingleTeamCharGet();

            if (!CharMotionAMemPGet(charNo)) {
                CharMotionInit(charNo);
            }
        }
        GwSystem.playerMode = 0;
    }
    for (i = 0; i < GW_PLAYER_MAX; i++, workP++) {
        GW_PLAYER *playerP;
        PLAYERCOLWORK *colWorkP;
        int charNo;
        MBMODELID modelId;

        workP->startTurnHook = workP->endTurnHook = NULL;
        workP->rotateObj = workP->moveObj = workP->posFixObj = NULL;
        playerP = GWPlayerGet(i);
        playerP->playerNo = workP->playerNo = i;
        GwPlayerConf[i].type = GwPlayer[i].comF;
        GwPlayerConf[i].padNo = GwPlayer[i].padNo;
        GwPlayerConf[i].grpNo = mbPlayerGrpGet(i);
        GwPlayerConf[i].comDif = GwPlayer[i].comDif;
        charNo = GwPlayer[i].charNo;
        GwPlayerConf[i].charNo = charNo;
        for (j = 0; j < 15; j++) {
            motDataNum[j] = charMotDirTbl[charNo] | charMotNoTbl[j];
        }
        motDataNum[j] = HU_DATANUM_NONE;
        modelId = workP->objId =
            mbObjCharCreate(charNo, charMdlFileTbl[charNo], motDataNum, FALSE);
        mbPlayerMotionVoiceOnSet(i, 7, FALSE);
        mbPlayerMotionVoiceOnSet(i, 12, FALSE);
        mbPlayerMotionVoiceOnSet(i, 8, FALSE);
        mbPlayerMotionVoiceOnSet(i, 13, FALSE);
        workP->colObj =
            omAddObjEx(mbObjMan, 0x100, 0, 0, -1, PlayerColOMExec);
        colWorkP = omObjGetWork(workP->colObj, PLAYERCOLWORK);
        colWorkP->playerNo = i;
        colWorkP->killF = TRUE;
        colWorkP->masuIdNext = GwPlayer[i].masuId;
        mbPlayerMatClone(i);
        workP->motNo = 1;
        mbObjMotionSet(modelId, workP->motNo, HU3D_MOTATTR_LOOP);
        GwPlayer[i].dispLightF = TRUE;
        GwPlayer[i].masuIdPrev = -1;
        mbPlayerWorkGet(i)->_unk10_3 = TRUE;
        CharModelDataClose(charNo);
    }
    mbPlayerColSnapSet(FALSE);
    if (GwSystem.partyF) {
        mbPlayerPosResetAll();
    } else {
        for (i = 0; i < GW_PLAYER_MAX; i++) {
            if (i > 0) {
                GwPlayer[i].masuIdPrev =
                    GwPlayer[i].masuIdNext = GwPlayer[i].masuId = 0;
                mbPlayerDispSet(i, FALSE);
            } else {
                mbPlayerPosReset(i);
            }
        }
    }
    CharEffectLayerSet(5);
}

void mbPlayerClose(void)
{
    MBPLAYERWORK *workP;
    int i;

    workP = &playerWork[0];
    for (i = 0; i < GW_PLAYER_MAX; i++, workP++) {
        GW_PLAYER *playerP;

        playerP = GWPlayerGet(i);

        if (workP->objId != MB_MODEL_NONE) {
            PlayerMetalKill(i);
            PlayerBiriQKill(i);
            mbObjKill(workP->objId);
            workP->objId = MB_MODEL_NONE;
        }
        if (workP->matCopy) {
            HSF_MATERIAL *matCopy = workP->matCopy;

            HuMemDirectFree(matCopy);
            workP->matCopy = NULL;
        }
        mbDiceNumKill(i);
    }
}

MBPLAYERWORK *mbPlayerWorkGet(int playerNo)
{
    return &playerWork[playerNo];
}

void mbPlayerTurnInitHookSet(void (*hook)(int playerNo))
{
    turnInitHook = hook;
}

void mbPlayerTurnCloseHookSet(void (*hook)(int playerNo))
{
    turnCloseHook = hook;
}

void mbPlayerStartTurnHookSet(int playerNo, MBPLAYERTURNHOOK hook)
{
    playerWork[playerNo].startTurnHook = hook;
}

void mbPlayerEndTurnHookSet(int playerNo, MBPLAYERTURNHOOK hook)
{
    playerWork[playerNo].endTurnHook = hook;
}

void mbPlayerMoveHookSet(int playerNo, MBPLAYERMOVEHOOK hook)
{
    playerWork[playerNo].moveHook = hook;
}

void mbTurnExec(BOOL intrF)
{
    int playerNo;

    turnIntrF = intrF;
    blackoutF = FALSE;
    playerNo = GwSystem.turnPlayerNo;
    for (; playerNo < GW_PLAYER_MAX; playerNo++) {
        int orderNo;
        int i;

        GwSystem.turnPlayerNo = playerNo;
        orderNo = 1;
        GwPlayer[playerNo].orderNo = 0;
        for (i = 0; i < GW_PLAYER_MAX; i++) {
            if (playerNo != i) {
                GwPlayer[i].orderNo = orderNo++;
            }
            mbPlayerMotionSet(i, 1, HU3D_MOTATTR_LOOP);
            GwPlayer[i].masuIdNext = GwPlayer[i].masuId;
        }
        PlayerTurn(playerNo);
        turnIntrF = FALSE;
    }
}

void mbSingleTurnExec(BOOL intrF)
{
    turnIntrF = intrF;
    blackoutF = FALSE;
    GwSystem.turnPlayerNo = 0;
    GwPlayer[0].orderNo = 0;
    mbPlayerMotionSet(0, 1, HU3D_MOTATTR_LOOP);
    PlayerTurn(0);
    turnIntrF = FALSE;
}

static void PlayerTurn(int playerNo)
{
    BOOL telopF = FALSE;
    BOOL killerF;
    BOOL eventResult;
    int i;

    GwSystem.turnPlayerNo = playerNo;
    mbPlayerPosReset(playerNo);
    mbPlayerColSnapSet(TRUE);
    mbev_PlayerColMasuSet(playerNo, GwPlayer[playerNo].masuId, TRUE);
    mbStarDispSetAll(TRUE);
    mbStarMasuDispSet(GwPlayer[playerNo].masuId, FALSE);
    if (!turnIntrF) {
        MBPLAYERWORK *workP;

        mbCameraPlayerViewSetFast(playerNo, MB_CAMERA_VIEW_ZOOMIN);
        mbCameraMoveOnSet(FALSE);
        mbCameraMoveWait();
        mbPlayerMotionSet(playerNo, 1, HU3D_MOTATTR_LOOP);
        mbPlayerRotYSet(playerNo, 0.0f);
        if (turnInitHook) {
            turnInitHook(playerNo);
        }
        mbStatusDispForceSetAll(TRUE);
        if (GwSystem.partyF && playerNo == 0 && GwSystem.timeTurn > 0) {
            mbTelopTimeCreate();
            telopF = TRUE;
        }
        if (mbWipeSpecialStatGet()) {
            if (playerNo == 0) {
                mbWipeDissolveFadeIn();
            } else {
                mbWipeSpecialFadeOutCreate(5, 42);
            }
        }
        GwPlayer[playerNo].capsuleUse = -1;
        workP = &playerWork[playerNo];
        workP->masuNext = 0;
        if (telopF) {
            for (i = 0; i < 100; i++) {
                HuPrcVSleep();
            }
        }
        ev_PlayerStartTurn(playerNo);
        omVibrate(playerNo, 20, 20, 0);
        mbPauseDisableSet(FALSE);
        mbTutorialCall(3);
        if (GwSystem.partyF || GwSystem.turnNo == 1) {
            mbTelopPlayerCreate(playerNo);
        }
        GwPlayer[playerNo].moveNum = -1;
        GwPlayer[playerNo].skipEventF = FALSE;
        GwSystem.playerMode = 0;
    }
repeat:
    switch (GwSystem.playerMode) {
        case 0:
        case 2:
            _ClearFlag(FLAG_BOARD_WALKDONE);
            killerF = DiceRun(playerNo);
            if (GwPlayer[playerNo].moveNum != 0) {
                GwSystem.playerMode = 3;
            }
            if (killerF) {
                mbev_CapKillerMoveCall(playerNo);
                GwSystem.playerMode = 4;
                goto repeat;
            }
        case 3:
            mbPlayerColSnapSet(TRUE);
            if (GwPlayer[playerNo].moveNum != 0) {
                _ClearFlag(FLAG_BOARD_WALKDONE);
                turnIntrF = PlayerViewSet(playerNo, TRUE, TRUE, TRUE);
                mbCameraMoveOnSet(TRUE);
                if (GwPlayer[playerNo].moveNum != 0) {
                    PlayerMoveCall(playerNo);
                }
            }
            mbPlayerMetalSet(playerNo, FALSE);
            mbPlayerBiriQSet(playerNo, FALSE);
            GwSystem.playerMode = 4;
        case 4:
            GwSystem.playerMode = 5;
            if (_CheckFlag(FLAG_BOARD_LAST5) &&
                mbPlayerKettouCheck(playerNo, GwPlayer[playerNo].masuId)) {
                mbev_CapCallKettou(
                    playerNo, GwPlayer[playerNo].masuId, FALSE);
            }
        case 5:
            turnIntrF =
                PlayerViewSet(playerNo, turnIntrF, TRUE, FALSE);
            GwSystem.playerMode = 6;
            mbMasuPlayerColorSet(playerNo);
            eventResult =
                mbev_MasuCapStop(playerNo, GwPlayer[playerNo].masuId);
            if (!eventResult) {
                GwSystem.playerMode = 7;
                goto repeat;
            }
        case 6:
            turnIntrF =
                PlayerViewSet(playerNo, turnIntrF, FALSE, FALSE);
            GwSystem.playerMode = 7;
            mbCameraMoveWait();
            if (!mbev_MasuStop(playerNo, GwPlayer[playerNo].masuId)) {
                break;
            }
        case 7:
            turnIntrF =
                PlayerViewSet(playerNo, turnIntrF, FALSE, FALSE);
            mbPlayerRotateStart(playerNo, 0, 15);
            HuPrcSleep(15);
            mbCameraMoveWait();
            mbPlayerMotIdleSet(playerNo);
        case 1:
        default:
            break;
    }
    ev_PlayerEndTurn(playerNo);
    mbTutorialCall(4);
    if (GwSystem.partyF) {
        if (playerNo != GW_PLAYER_MAX - 1) {
            mbWipeSpecialFadeInCreate(5, 1);
        } else {
            mbWipeFadeOut();
        }
    }
    mbPlayerColSnapPlayerSet(playerNo, TRUE);
    if (turnCloseHook) {
        turnCloseHook(playerNo);
    }
}

static BOOL PlayerViewSet(
    int playerNo, BOOL intrF, BOOL waitF, BOOL carF)
{
    BOOL wipeF = mbWipeSpecialStatGet();

    if (intrF || wipeF) {
        if (!wipeF) {
            mbCameraPlayerViewSet(playerNo,
                carF ? MB_CAMERA_VIEW_WALK : MB_CAMERA_VIEW_ZOOMIN);
        } else {
            mbCameraPlayerViewSetFast(playerNo,
                carF ? MB_CAMERA_VIEW_WALK : MB_CAMERA_VIEW_ZOOMIN);
        }
        if (waitF) {
            mbCameraMoveWait();
        }
        if (carF && GwPlayer[playerNo].moveNum != 0) {
            mbMoveNumCreate(playerNo, TRUE);
        }
        if (wipeF) {
            mbWipeFadeIn();
        }
        intrF = FALSE;
    }
    return intrF;
}

int mbPlayerDiceTypeGet(int diceNo)
{
    int diceTypeTbl[7][2] = {
        { 0, 0 },
        { 1, 1 },
        { 2, 2 },
        { 3, 14 },
        { 4, 0 },
        { 5, 4 },
        { 6, 3 }
    };
    int i;

    for (i = 0; i < 7; i++) {
        if (diceNo == diceTypeTbl[i][0]) {
            return diceTypeTbl[i][1];
        }
    }
    return 0;
}

static BOOL DiceRun(int playerNo)
{
    BOOL killerF = FALSE;
    BOOL capsuleSkipF = FALSE;
    int capsuleNum;
    int result;

    GwPlayer[playerNo].diceNum = 1;
    capsuleNum = mbPlayerCapsuleNumGet(playerNo);
    if (!GwSystem.partyF) {
        capsuleNum = 1;
        if (GwSystem.turnNo <= 1) {
            capsuleNum = 0;
        }
    }
repeat:
    if (GwPlayer[playerNo].capsuleUse == -1 && !capsuleSkipF &&
        capsuleNum != 0) {
        GwSystem.playerMode = 0;
        result = mbCapSelect();
        if (GwPlayer[playerNo].capsuleUse != -1) {
            GwPlayer[playerNo].capsuleUseNum++;
        }
    } else {
        mbStatusDispSetAll(TRUE);
        while (!mbStatusOffCheckAll()) {
            HuPrcVSleep();
        }
        GwSystem.playerMode = 2;
        if (_CheckFlag(FLAG_BOARD_TUTORIAL)) {
            int tutorialVal[3];
            int diceType;
            int diceMax;
            int value;
            int i;

            diceType = mbPlayerDiceTypeGet(GwPlayer[playerNo].diceMode);
            diceMax = mbDiceMaxGet(diceType);
            for (i = 0; i < diceMax; i++) {
                value = mbTutorialCall(5);
                if (value < 0) {
                    value = mbRandMod(mbDiceValueMaxGet(diceType));
                }
                tutorialVal[i] = value;
            }
            if (diceMax <= 1) {
                result = mbDiceExec(playerNo, diceType, NULL,
                    tutorialVal[0], FALSE, TRUE, NULL, 0);
            } else {
                result = mbDiceProcExec(playerNo, diceType, NULL,
                    tutorialVal, FALSE, TRUE, NULL, 0);
            }
            mbTutorialCall(6);
        } else {
            int tutorialVal = -1;

            if (!GwSystem.partyF) {
                tutorialVal = mbSingleCall(0, -1);
            } else if (GwPlayer[playerNo].comF &&
                mbPlayerDiceTypeGet(GwPlayer[playerNo].diceMode) == 14) {
                tutorialVal = mbMasuPKinokoValueGet(
                    playerNo, GwPlayer[playerNo].masuId);
            }
            result = mbDiceExec(playerNo,
                mbPlayerDiceTypeGet(GwPlayer[playerNo].diceMode), NULL,
                tutorialVal, TRUE, TRUE, NULL, 0);
        }
    }
    switch (result) {
        case -3:
            if (!GwSystem.partyF && !_CheckFlag(FLAG_BOARD_TUTORIAL)) {
                mbSingleCall(1, -1);
            }
            mbDiceKill(playerNo);
            mbev_Scroll(playerNo, FALSE);
            break;
        case -4:
            if (!GwSystem.partyF && !_CheckFlag(FLAG_BOARD_TUTORIAL)) {
                mbSingleCall(1, -1);
            }
            mbDiceKill(playerNo);
            mbev_Scroll(playerNo, TRUE);
            break;
        case -6:
            if (!GwSystem.partyF && !_CheckFlag(FLAG_BOARD_TUTORIAL)) {
                mbSingleCall(1, -1);
            }
            capsuleSkipF = FALSE;
            mbDiceKill(playerNo);
            mbAudFXPlay(3);
            break;
        case -7:
            capsuleSkipF = TRUE;
            mbDiceKill(playerNo);
            break;
        case -5:
        default:
            break;
    }
    if (result <= 0) {
        goto repeat;
    }
    if (GwPlayer[playerNo].diceMode == 5) {
        killerF = TRUE;
    }
    GwPlayer[playerNo].diceMode = 0;
    GwPlayer[playerNo].moveNum = result;
    mbMoveNumCreate(playerNo, TRUE);
    mbDiceNumKill(playerNo);
    if (!GwSystem.partyF) {
        mbSingleCall(3, result);
    }
    return killerF;
}

static void PlayerMoveCall(int playerNo)
{
    MBPLAYERWORK *workP = &playerWork[playerNo];

    mbPlayerColSnapSet(TRUE);
    workP->moveProc =
        HuPrcChildCreate(PlayerMove, 0x200D, 0x6000, 0, mbMainProc);
    workP->moveProc->property = workP;
    HuPrcDestructorSet2(workP->moveProc, PlayerMoveDestroy);
    while (workP->moveProc) {
        HuPrcVSleep();
    }
    _SetFlag(FLAG_BOARD_WALKDONE);
}

static void PlayerMove(void)
{
    MBPLAYERWORK *workP = HuPrcCurrentGet()->property;
    int playerNo = workP->playerNo;
    s16 masuIdNext;
    BOOL hiddenF;

    playerWork[playerNo]._unk0C = 0;
    playerWork[playerNo]._unk10_3 = TRUE;
    workP->moveHook = NULL;
    GwPlayer[playerNo].masuIdPrev = GwPlayer[playerNo].masuId;
repeat:
    if (!_CheckFlag(FLAG_BOARD_DEBUG) ||
        _CheckFlag(FLAG_BOARD_TUTORIAL)) {
        if (mbev_Branch(playerNo, &masuIdNext)) {
            goto end;
        }
    } else {
        if (mbev_BranchDebug(playerNo, &masuIdNext) || masuIdNext < 0) {
            goto end;
        }
    }
    masuIdNext = mbev_GateMasu(
        playerNo, GwPlayer[playerNo].masuId, masuIdNext);
    GwPlayer[playerNo].masuIdNext = masuIdNext;
    playerWork[playerNo]._unk08 = -1;
    workP->_unk06 = masuIdNext;
    if (workP->_unk10_3) {
        HuPrcSleep(-1);
    }
    PlayerColKill(playerNo);
    mbev_CapCallTrap(
        playerNo, GwPlayer[playerNo].masuId, masuIdNext);
    mbev_MasuMasuEnd(masuIdNext);
    if (workP->moveHook) {
        playerWork[playerNo]._unk0C = 4;
        workP->moveHook(playerNo);
        workP->moveHook = NULL;
    } else {
        mbPlayerMasuMove(playerNo, TRUE);
    }
    playerWork[playerNo]._unk0C = 0;
    playerWork[playerNo]._unk10_3 = TRUE;
    playerWork[playerNo].masuMoveF = FALSE;
    GwPlayer[playerNo].masuId = masuIdNext;
    mbTutorialCall(8);
    hiddenF = !mbMasuDispCheck(masuIdNext);
    if (!hiddenF && GwPlayer[playerNo].biriQF) {
        PlayerBiriQFlashSet(playerNo);
        mbev_CapBiriQShockCreate(playerNo);
    }
    if (!mbev_MasuMasuStart(playerNo)) {
        masuIdNext = GwPlayer[playerNo].masuId;
        if (!mbev_MasuMove(playerNo, masuIdNext) &&
            mbMasuDispCheck(masuIdNext)) {
            mbAudFXPlay(0x3EE);
            GwPlayer[playerNo].moveNum--;
            if (GwPlayer[playerNo].moveNum < 0) {
                GwPlayer[playerNo].moveNum = 0;
            }
            if (GwPlayer[playerNo].moveNum == 0) {
                mbMoveNumKill(playerNo);
            }
        }
    }
    if (workP->_unk10_3) {
        workP->moveEndF = TRUE;
        HuPrcSleep(-1);
    }
    mbTutorialCall(9);
    if (GwPlayer[playerNo].moveNum != 0) {
        goto repeat;
    }
end:
    workP->moveHook = NULL;
    mbMoveNumKill(playerNo);
    playerWork[playerNo]._unk0C = 0;
    playerWork[playerNo]._unk10_3 = TRUE;
    if (workP->_unk10_3) {
        mbPlayerRotateStart(playerNo, 0, 15);
        while (!mbPlayerRotateCheck(playerNo)) {
            HuPrcVSleep();
        }
        mbPlayerMotIdleSet(playerNo);
        mbPlayerColOrderReset();
    } else {
        mbPlayerMotIdleSet(playerNo);
    }
    HuPrcEnd();
}

static void PlayerMoveDestroy(void)
{
    MBPLAYERWORK *workP = HuPrcCurrentGet()->property;

    workP->moveProc = NULL;
}

static void ev_PlayerStartTurn(int playerNo)
{
    if (playerWork[playerNo].startTurnHook) {
        if (playerWork[playerNo].startTurnHook()) {
            playerWork[playerNo].startTurnHook = NULL;
        }
    }
}

static void ev_PlayerEndTurn(int playerNo)
{
    if (playerWork[playerNo].endTurnHook) {
        if (playerWork[playerNo].endTurnHook()) {
            playerWork[playerNo].endTurnHook = NULL;
        }
    }
}

void mbPlayerMasuMoveTo(int playerNo, int masuId, BOOL waitF)
{
    GwPlayer[playerNo].masuIdNext = masuId;
    mbPlayerMasuMove(playerNo, waitF);
}

void mbPlayerMasuMove(int playerNo, BOOL waitF)
{
    MBPLAYERWORK *workP = &playerWork[playerNo];
    MBPLAYERWORK *workP2;

    workP->masuMoveF = TRUE;
    mbPlayerMoveExec(
        playerNo, NULL, NULL, mbPlayerWalkSpeedGet(), NULL, waitF);
    workP2 = &playerWork[playerNo];
    workP2->masuMoveF = FALSE;
}

void mbPlayerMasuMovePos(int playerNo, HuVecF *pos, BOOL waitF)
{
    mbPlayerMoveExec(
        playerNo, NULL, pos, mbPlayerWalkSpeedGet(), NULL, waitF);
}

void mbPlayerMasuMoveSpeed(
    int playerNo, int masuId, s16 maxTime, BOOL waitF)
{
    MBPLAYERWORK *workP;
    MBPLAYERWORK *workP2;
    HuVecF pos;

    mbMasuPosGet(masuId, &pos);
    workP = &playerWork[playerNo];
    workP->masuMoveF = TRUE;
    mbPlayerMoveExec(playerNo, NULL, &pos, maxTime, NULL, waitF);
    workP2 = &playerWork[playerNo];
    workP2->masuMoveF = FALSE;
}

void mbPlayerMoveExec(int playerNo, HuVecF *srcPos, HuVecF *dstPos,
    s16 maxTime, HuVecF *rot, BOOL waitF)
{
    mbPlayerMoveMain(playerNo, srcPos, dstPos, 0, 1.0f, HU3D_MOTATTR_LOOP,
        maxTime, rot, waitF);
}

enum {
    PLAYER_MOVE_MODE_RUN,
    PLAYER_MOVE_MODE_JUMP,
    PLAYER_MOVE_MODE_CLIMB
};

typedef struct PlayerMoveWork {
    u8 killF : 1;
    u8 mode : 2;
    u8 playerNo : 2;
    s16 time;
    s16 maxTime;
} PLAYERMOVEWORK;

static void PlayerMoveOMExec(OMOBJ *objP);

void mbPlayerMoveMain(int playerNo, HuVecF *srcPos, HuVecF *dstPos, u32 motNo,
    float motSpeed, u32 motAttr, s16 maxTime, HuVecF *rot, BOOL waitF)
{
    BOOL setAngle = FALSE;
    OMOBJ *objP = playerWork[playerNo].moveObj = omAddObjEx(mbObjMan, 0x100,
        0, 0, -1, PlayerMoveOMExec);
    PLAYERMOVEWORK *workP = omObjGetWork(objP, PLAYERMOVEWORK);
    int mode;
    HuVecF moveDir;
    HuVecF playerRot;

    if (srcPos != NULL) {
        objP->trans.x = srcPos->x;
        objP->trans.y = srcPos->y;
        objP->trans.z = srcPos->z;
    } else {
        mbPlayerPosGet(playerNo, &objP->trans);
    }
    if (dstPos != NULL) {
        objP->rot.x = dstPos->x;
        objP->rot.y = dstPos->y;
        objP->rot.z = dstPos->z;
        workP->mode = PLAYER_MOVE_MODE_RUN;
    } else {
        MASU *masuPrev;
        MASU *masuNext;
        s16 masuIdPrev;
        int masuIdNext;

        masuIdNext = GwPlayer[playerNo].masuIdNext;
        masuNext = mbMasuGet(masuIdNext);
        masuIdPrev = GwPlayer[playerNo].masuIdPrev;
        if (masuIdPrev < 0) {
            mode = PLAYER_MOVE_MODE_RUN;
        } else {
            masuPrev = mbMasuGet(masuIdPrev);
            if ((masuPrev->flag & MASU_FLAG_JUMPFROM)
                && (masuNext->flag & MASU_FLAG_JUMPTO)) {
                mode = PLAYER_MOVE_MODE_JUMP;
            } else if ((masuPrev->flag & MASU_FLAG_CLIMBFROM)
                && (masuNext->flag & MASU_FLAG_CLIMBTO)) {
                mode = PLAYER_MOVE_MODE_CLIMB;
            } else {
                mode = PLAYER_MOVE_MODE_RUN;
            }
        }
        workP->mode = mode;
        mbMasuPosGet(GwPlayer[playerNo].masuIdNext, &objP->rot);
    }
    if (motNo == 0) {
        switch (workP->mode) {
            case PLAYER_MOVE_MODE_RUN:
                mbPlayerWorkGet(playerNo)->_unk0C = 1;
                mbPlayerMotionShiftSet(playerNo, 3, 0.0f, 4.0f,
                    HU3D_MOTATTR_LOOP);
                break;
            case PLAYER_MOVE_MODE_JUMP:
                mbPlayerWorkGet(playerNo)->_unk0C = 2;
                mbPlayerMotionShiftSet(playerNo, 4, 6.0f, 2.0f,
                    HU3D_MOTATTR_NONE);
                maxTime = 24;
                break;
            case PLAYER_MOVE_MODE_CLIMB:
                mbPlayerWorkGet(playerNo)->_unk0C = 3;
                mbPlayerMotionShiftSet(playerNo, 14, 0.0f, 4.0f,
                    HU3D_MOTATTR_LOOP);
                maxTime = 100;
                motSpeed = 2.0f;
                VECSubtract(&objP->rot, &objP->trans, &moveDir);
                maxTime = VECMag(&moveDir) / 15.000001f;
                if (objP->trans.y >= objP->rot.y) {
                    moveDir.x = -moveDir.x;
                    moveDir.y = -moveDir.y;
                    moveDir.z = -moveDir.z;
                    motSpeed = -motSpeed;
                }
                playerRot.x = playerRot.z = 0.0f;
                playerRot.y = HuAtan(moveDir.x, moveDir.z);
                mbPlayerRotSetV(playerNo, &playerRot);
                setAngle = TRUE;
                break;
        }
    } else {
        mbPlayerMotionShiftSet(
            playerNo, motNo, 0.0f, 4.0f, motAttr);
    }
    mbPlayerMotionSpeedSet(playerNo, motSpeed);
    if (setAngle == FALSE) {
        if (rot == NULL) {
            float rotY;

            VECSubtract(&objP->rot, &objP->trans, &playerRot);
            rotY = 90.0f - HuAtan(playerRot.z, playerRot.x);
            mbPlayerRotYSet(playerNo, rotY);
        } else {
            mbPlayerRotSetV(playerNo, rot);
        }
    }
    if (srcPos) {
        mbPlayerPosSetV(playerNo, srcPos);
    }
    objP->scale.x = objP->trans.x;
    objP->scale.y = objP->trans.y;
    objP->scale.z = objP->trans.z;
    workP->playerNo = playerNo;
    workP->time = 0;
    workP->maxTime = maxTime;
    {
        int movePlayerNo = workP->playerNo;

        if (playerWork[movePlayerNo].masuMoveF) {
            int moveMaxTime = workP->maxTime;
            int movePlayerNo2 = workP->playerNo;
            MBPLAYERWORK *workP2 = mbPlayerWorkGet(movePlayerNo2);

            workP2->_unk08 = moveMaxTime;
        }
    }
    GwPlayer[playerNo].moveF = TRUE;
    if (!waitF) {
        return;
    }
    while (GwPlayer[playerNo].moveF) {
        HuPrcVSleep();
    }
    mbPlayerWorkGet(playerNo)->_unk0C = 0;
    mbPlayerWorkGet(playerNo)->moveEndF = TRUE;
}

static void PlayerMoveOMExec(OMOBJ *objP)
{
    PLAYERMOVEWORK *workP = omObjGetWork(objP, PLAYERMOVEWORK);
    float weight;

    if (mbExitCheck() || workP->killF) {
        GwPlayer[workP->playerNo].moveF = FALSE;
        omDelObjEx(HuPrcCurrentGet(), objP);
        playerWork[workP->playerNo].moveObj = NULL;
        return;
    }
    workP->time++;
    weight = (float)workP->time / workP->maxTime;
    objP->trans.x = objP->scale.x
        + (weight * (objP->rot.x - objP->scale.x));
    objP->trans.y = objP->scale.y
        + (weight * (objP->rot.y - objP->scale.y));
    objP->trans.z = objP->scale.z
        + (weight * (objP->rot.z - objP->scale.z));
    if (playerWork[workP->playerNo].masuMoveF) {
        playerWork[workP->playerNo]._unk08 = workP->maxTime - workP->time;
    }
    if (workP->time >= workP->maxTime) {
        GwPlayer[workP->playerNo].moveF = FALSE;
        mbPlayerPosSet(workP->playerNo, objP->rot.x, objP->rot.y,
            objP->rot.z);
        omDelObjEx(HuPrcCurrentGet(), objP);
        playerWork[workP->playerNo].moveObj = NULL;
    } else if (workP->mode != PLAYER_MOVE_MODE_JUMP) {
        mbPlayerPosSet(workP->playerNo, objP->trans.x, objP->trans.y,
            objP->trans.z);
    } else {
        playerWork[workP->playerNo].moveEndF = FALSE;
        if (workP->time >= workP->maxTime - 2) {
            weight = 1.0f;
            playerWork[workP->playerNo].moveEndF = TRUE;
        } else {
            weight = (float)workP->time / (workP->maxTime - 2);
        }
        mbPlayerPosSet(workP->playerNo, objP->trans.x,
            objP->trans.y + (100.0f * (1.5f * HuSin(weight * 180.0f))),
            objP->trans.z);
        if (workP->time == workP->maxTime - 5) {
            mbPlayerMotionShiftSet(workP->playerNo, 5, 2.0f, 2.0f,
                HU3D_MOTATTR_NONE);
        }
    }
}

typedef struct PlayerRotateWork {
    u8 killF : 1;
    s8 playerNo;
    s16 maxTime;
    s16 time;
} PLAYERROTATEWORK;

static void PlayerRotateOMExec(OMOBJ *objP);

void mbPlayerRotateStart(int playerNo, s16 endAngle, s16 maxTime)
{
    OMOBJ *objP;
    PLAYERROTATEWORK *workP;
    float angle;

    if (maxTime <= 0) {
        return;
    }
    if (playerWork[playerNo].rotateObj) {
        objP = playerWork[playerNo].rotateObj;
    } else {
        playerWork[playerNo].rotateObj = objP = omAddObjEx(mbObjMan, 0x100,
            0, 0, -1, PlayerRotateOMExec);
    }
    workP = omObjGetWork(objP, PLAYERROTATEWORK);
    workP->killF = FALSE;
    workP->maxTime = maxTime;
    workP->playerNo = playerNo;
    workP->time = 0;
    objP->rot.y = mbPlayerRotYGet(playerNo);
    objP->scale.z = endAngle;
    angle = fmod(endAngle - objP->rot.y, 360.0f);
    if ((s16)angle == 0) {
        mbPlayerMotionShiftSet(playerNo, 1, 0.0f, 5.0f,
            HU3D_MOTATTR_LOOP);
        workP->killF = TRUE;
    } else {
        if (angle < 0.0f) {
            angle += 360.0f;
        }
        if (angle > 180.0f) {
            angle -= 360.0f;
        }
        objP->scale.y = angle;
        if (fabs(angle) > 5.0f) {
            mbPlayerMotionShiftSet(playerNo, 2, 0.0f, 5.0f,
                HU3D_MOTATTR_LOOP);
        } else {
            mbPlayerMotionShiftSet(playerNo, 1, 0.0f, 5.0f,
                HU3D_MOTATTR_LOOP);
        }
    }
}

static void PlayerRotateOMExec(OMOBJ *objP)
{
    PLAYERROTATEWORK *workP = omObjGetWork(objP, PLAYERROTATEWORK);
    float rotY;
    float angle;
    float weight;

    if (workP->killF || mbExitCheck()) {
        mbPlayerRotYSet(workP->playerNo, objP->scale.z);
        playerWork[workP->playerNo].rotateObj = NULL;
        omDelObjEx(HuPrcCurrentGet(), objP);
        return;
    }
    angle = (float)(workP->time++) / workP->maxTime;
    weight = HuSin(angle * 90.0f);
    rotY = objP->rot.y + (weight * objP->scale.y);
    mbPlayerRotYSet(workP->playerNo, rotY);
    if (workP->time >= workP->maxTime) {
        workP->killF = TRUE;
        mbPlayerMotionSet(workP->playerNo, 1, HU3D_MOTATTR_LOOP);
        return;
    }
}

BOOL mbPlayerRotateCheck(int playerNo)
{
    return playerWork[playerNo].rotateObj == NULL;
}

BOOL mbPlayerRotateCheckAll(void)
{
    int i;

    for (i = 0; i < GW_PLAYER_MAX; i++) {
        if (playerWork[i].rotateObj != NULL) {
            return FALSE;
        }
    }
    return TRUE;
}

void mbPlayerDiceMotExec(int playerNo)
{
    int time;

    mbPlayerMotionSet(playerNo, 11, HU3D_MOTATTR_NONE);
    time = 0;
    do {
        if (time++ == 27) {
            mbDiceObjHit(playerNo);
        }
        HuPrcVSleep();
    } while (!mbPlayerMotionEndCheck(playerNo));
    mbPlayerMotIdleSet(playerNo);
}

typedef struct MoveNumWork {
    u8 killF : 1;
    u8 dispF : 1;
    u8 playerNo : 2;
    u8 carF : 1;
} MOVENUMWORK;

static void MoveNumOMExec(OMOBJ *objP);

void mbMoveNumCreateColor(int playerNo, BOOL carF, int color)
{
    int modelId;
    HU3D_CAMERA *cameraP;
    OMOBJ *objP;
    MOVENUMWORK *workP;
    int i;
    HuVecF pos;
    HuVecF posNorm;

    cameraP = &Hu3DCamera[0];
    if (playerWork[playerNo].moveNumObj) {
        return;
    }
    playerWork[playerNo].moveNumObj = objP = omAddObjEx(mbObjMan, 0x7E02,
        20, 0, -1, MoveNumOMExec);
    omSetStatBit(objP, OM_STAT_MODELPAUSE);
    workP = omObjGetWork(objP, MOVENUMWORK);
    workP->dispF = TRUE;
    workP->killF = FALSE;
    workP->playerNo = playerNo;
    workP->carF = carF;
    for (i = 0; i < 20; i++) {
        modelId = mbCoinObjCreate(i % 10, color);
        mbCoinObjLayerSet(modelId, 4);
        mbCoinObjDispSet(modelId, FALSE);
        objP->mdlId[i] = modelId;
    }
    mbPlayerPosGet(playerNo, &pos);
    mbPos3DtoNorm(&pos, 1, &posNorm);
    objP->trans.y = posNorm.y;
    pos.y += 300.0f;
    mbPos3DtoNorm(&pos, 1, &posNorm);
    objP->trans.y = posNorm.y - objP->trans.y;
    objP->trans.z = posNorm.z;
    if (carF) {
        Mtx lookAt;
        float tanFov;

        mbPlayerPosGet(playerNo, &pos);
        pos.y += 300.0f;
        MTXLookAt(lookAt, &cameraP->pos, &cameraP->up, &cameraP->target);
        MTXMultVec(lookAt, &pos, &posNorm);
        tanFov = HuSin(cameraP->fov * 0.5f)
            / HuCos(cameraP->fov * 0.5f);
        objP->rot.y = posNorm.y / (tanFov * posNorm.z);
        objP->rot.z = -posNorm.z;
    }
}

void mbMoveNumCreate(int playerNo, BOOL carF)
{
    mbMoveNumCreateColor(playerNo, carF, 0);
}

static void MoveNumOMExec(OMOBJ *objP)
{
    int digitNum = 0;
    MOVENUMWORK *workP = omObjGetWork(objP, MOVENUMWORK);
    HU3D_CAMERA *cameraP = &Hu3DCamera[0];
    HU3D_CAMERA *camera2P = &Hu3DCamera[2];
    int i;
    HuVecF pos;
    HuVecF posNorm;
    float scaleX;
    float scaleY;
    float tanFov;
    float scale;
    float rotZ;

    if (workP->killF || mbExitCheck()) {
        for (i = 0; i < 20; i++) {
            if (objP->mdlId[i] != -1) {
                mbCoinObjNumDec(objP->mdlId[i]);
                objP->mdlId[i] = -1;
            }
        }
        omDelObjEx(HuPrcCurrentGet(), objP);
        playerWork[workP->playerNo].moveNumObj = NULL;
        return;
    }
    if (mbPauseEnableCheck()) {
        return;
    }
    mbPlayerPosGet(workP->playerNo, &pos);
    mbPos3DtoNorm(&pos, 1, &posNorm);
    posNorm.y += objP->trans.y;
    posNorm.z = objP->trans.z;
    tanFov = HuSin(camera2P->fov * 0.5f)
        / HuCos(camera2P->fov * 0.5f);
    scaleX = 1.2f * (tanFov * -posNorm.z);
    scaleY = tanFov * -posNorm.z;
    posNorm.x *= scaleX;
    posNorm.y *= scaleY;
    pos = posNorm;
    mbCameraRotGet(&posNorm);
    rotZ = -posNorm.z;
    for (i = 0; i < 20; i++) {
        mbCoinObjDispSet(objP->mdlId[i], FALSE);
    }
    if (workP->dispF) {
        int modelNo;

        scale = HuSin(cameraP->fov * 0.5f)
            / HuCos(cameraP->fov * 0.5f);
        scale = tanFov / scale;
        modelNo = GwPlayer[workP->playerNo].moveNum / 10;
        if (modelNo != 0) {
            mbCoinObjDispSet(objP->mdlId[modelNo], TRUE);
            mbCoinObjPosSet(objP->mdlId[modelNo],
                pos.x - (60.000004f * scale), pos.y, pos.z);
            mbCoinObjRotSet(objP->mdlId[modelNo], rotZ, 0.0f, 0.0f);
            mbCoinObjScaleSet(
                objP->mdlId[modelNo], scale, scale, scale);
            digitNum++;
        }
        modelNo = (GwPlayer[workP->playerNo].moveNum % 10) + 10;
        if (modelNo != 0) {
            mbCoinObjDispSet(objP->mdlId[modelNo], TRUE);
            if (digitNum == 0) {
                mbCoinObjPosSet(
                    objP->mdlId[modelNo], pos.x, pos.y, pos.z);
            } else {
                mbCoinObjPosSet(objP->mdlId[modelNo],
                    pos.x + (60.000004f * scale), pos.y, pos.z);
            }
            mbCoinObjRotSet(objP->mdlId[modelNo], rotZ, 0.0f, 0.0f);
            mbCoinObjScaleSet(
                objP->mdlId[modelNo], scale, scale, scale);
        }
    }
}

void mbMoveNumKill(int playerNo)
{
    if (playerWork[playerNo].moveNumObj) {
        MOVENUMWORK *workP =
            omObjGetWork(playerWork[playerNo].moveNumObj, MOVENUMWORK);

        workP->killF = TRUE;
    }
}

void mbMoveNumDispSet(int playerNo, BOOL dispF)
{
    if (playerWork[playerNo].moveNumObj) {
        MOVENUMWORK *workP =
            omObjGetWork(playerWork[playerNo].moveNumObj, MOVENUMWORK);

        workP->dispF = dispF;
    }
}

static void PlayerColCornerSet(int playerNo, int masuIdNext);
static void PlayerColCornerSnap(int playerNo, int masuId, int cornerNo);
static void PlayerColInit(int playerNo, int masuId, int cornerNo);

void mbev_PlayerColMasuAllSet(int *masuIdFix, BOOL snapF)
{
    int i;
    int j;
    int cornerNo;
    int masuId;
    s8 orderNo;
    HuVecF pos;

    for (i = 0; i < GW_PLAYER_MAX; i++) {
        if (playerWork[i].colObj) {
            PLAYERCOLWORK *workP;

            if (GwPlayer[i].masuId == 0) {
                continue;
            }
            workP = omObjGetWork(playerWork[i].colObj, PLAYERCOLWORK);
            workP->masuIdNext = GwPlayer[i].masuIdNext;
            if (workP->restF) {
                continue;
            }
        }
        if (masuIdFix[i] >= 0) {
            mbPlayerMasuCornerSet(i, 0);
            PlayerColCornerSnap(i, masuIdFix[i], 0);
            continue;
        }
        if (playerWork[i].colObj) {
            PLAYERCOLWORK *workP =
                omObjGetWork(playerWork[i].colObj, PLAYERCOLWORK);

            if (!workP->snapF) {
                continue;
            }
        }
        masuId = GwPlayer[i].masuId;
        orderNo = GwPlayer[i].orderNo;
        cornerNo = 0;
        for (j = 0; j < GW_PLAYER_MAX; j++) {
            if (i != j && masuId == masuIdFix[j]) {
                cornerNo++;
            }
        }
        for (j = 0; j < GW_PLAYER_MAX; j++) {
            if (i != j && masuIdFix[j] < 0 &&
                masuId == GwPlayer[j].masuId &&
                orderNo > GwPlayer[j].orderNo) {
                cornerNo++;
            }
        }
        if (cornerNo == 0) {
            mbMasuPosGet(masuId, &pos);
        } else {
            mbMasuCornerRotPosGet(masuId, cornerNo - 1, &pos);
        }
        {
            PLAYERCOLWORK *workP =
                omObjGetWork(playerWork[i].colObj, PLAYERCOLWORK);
            u8 circleF = workP->circleF;

            workP->circleF = FALSE;
            if (snapF) {
                mbPlayerPosSetV(i, &pos);
                PlayerColCornerSnap(i, masuId, cornerNo);
            } else if (cornerNo != mbPlayerMasuCornerGet(i) || circleF) {
                PlayerColInit(i, masuId, cornerNo);
            }
        }
        mbPlayerMasuCornerSet(i, cornerNo);
    }
}

void mbev_PlayerColMasu(int playerNo, int masuId, BOOL snapF)
{
    int orderNo[GW_PLAYER_MAX];
    int playerNoTbl[GW_PLAYER_MAX];
    HuVecF pos;
    int num = 0;
    int i;
    int j;

    for (i = 0; i < GW_PLAYER_MAX; i++) {
        if (i != playerNo) {
            if (GwPlayer[i].masuId == 0) {
                continue;
            }
            if (masuId != GwPlayer[i].masuId) {
                continue;
            }
        }
        orderNo[num] = GwPlayer[i].orderNo;
        if (i == playerNo) {
            orderNo[num] = -1;
        }
        playerNoTbl[num] = i;
        num++;
        mbPlayerColSnapPlayerSet(i, TRUE);
        mbPlayerColRestSet(i, TRUE);
    }
    for (i = 0; i < num - 1; i++) {
        for (j = i + 1; j < num; j++) {
            if (orderNo[i] > orderNo[j]) {
                int temp = orderNo[i];

                orderNo[i] = orderNo[j];
                orderNo[j] = temp;
                temp = playerNoTbl[i];
                playerNoTbl[i] = playerNoTbl[j];
                playerNoTbl[j] = temp;
            }
        }
    }
    for (i = 0; i < num; i++) {
        int playerNoSet = playerNoTbl[i];
        int cornerNo = i;
        PLAYERCOLWORK *workP;
        u8 circleF;

        if (cornerNo != 0) {
            mbMasuCornerRotPosGet(masuId, cornerNo - 1, &pos);
        } else {
            mbMasuPosGet(masuId, &pos);
        }
        workP = omObjGetWork(playerWork[playerNoSet].colObj, PLAYERCOLWORK);
        circleF = workP->circleF;
        workP->circleF = FALSE;
        if (snapF) {
            mbPlayerPosSetV(playerNoSet, &pos);
            PlayerColCornerSnap(playerNoSet, masuId, cornerNo);
        } else if (cornerNo != mbPlayerMasuCornerGet(playerNoSet) || circleF) {
            PlayerColInit(playerNoSet, masuId, cornerNo);
        }
        mbPlayerMasuCornerSet(playerNoSet, cornerNo);
    }
}

void mbev_PlayerColCircleAdd(
    int playerNo, int masuId, BOOL snapF, float radius)
{
    int orderNo[GW_PLAYER_MAX];
    int playerNoTbl[GW_PLAYER_MAX];
    HuVecF pos;
    HuVecF posCenter;
    int num = 0;
    int i;
    int j;

    for (i = 0; i < GW_PLAYER_MAX; i++) {
        PLAYERCOLWORK *workP;

        if (i != playerNo) {
            if (GwPlayer[i].masuId == 0) {
                continue;
            }
            if (masuId != GwPlayer[i].masuId) {
                continue;
            }
        }
        workP = omObjGetWork(playerWork[i].colObj, PLAYERCOLWORK);
        workP->circleF = TRUE;
        workP->radius = radius;
        orderNo[num] = GwPlayer[i].orderNo;
        if (i == playerNo) {
            orderNo[num] = -1;
        }
        playerNoTbl[num] = i;
        num++;
        mbPlayerColSnapPlayerSet(i, TRUE);
        mbPlayerColRestSet(i, TRUE);
    }
    for (i = 0; i < num - 1; i++) {
        for (j = i + 1; j < num; j++) {
            if (orderNo[i] > orderNo[j]) {
                int temp = orderNo[i];

                orderNo[i] = orderNo[j];
                orderNo[j] = temp;
                temp = playerNoTbl[i];
                playerNoTbl[i] = playerNoTbl[j];
                playerNoTbl[j] = temp;
            }
        }
    }
    for (i = 0; i < num; i++) {
        int playerNoSet = playerNoTbl[i];
        int cornerNo = i;

        if (playerNo < 0 && cornerNo == 0) {
            cornerNo = num;
        }
        mbMasuPosGet(masuId, &posCenter);
        if (cornerNo != 0) {
            float scale;

            mbMasuCornerRotPosGet(masuId, cornerNo - 1, &pos);
            VECSubtract(&pos, &posCenter, &pos);
            scale = radius / VECMag(&pos);
            VECScale(&pos, &pos, scale);
            VECAdd(&posCenter, &pos, &posCenter);
        }
        if (snapF) {
            mbPlayerPosSetV(playerNoSet, &posCenter);
            PlayerColCornerSnap(playerNoSet, masuId, cornerNo);
        } else {
            PlayerColInit(playerNoSet, masuId, cornerNo);
        }
        mbPlayerMasuCornerSet(playerNoSet, cornerNo);
    }
}

void mbev_PlayerColMasuAdd(int playerNo, int masuId, BOOL snapF)
{
    HuVecF pos;
    int i;
    int j;

    for (i = 0; i < GW_PLAYER_MAX; i++) {
        s8 orderNo;
        int cornerNo;
        PLAYERCOLWORK *workP;
        u8 circleF;

        if (masuId != GwPlayer[i].masuId || i == playerNo) {
            continue;
        }
        if (playerWork[i].colObj) {
            workP = omObjGetWork(playerWork[i].colObj, PLAYERCOLWORK);
            workP->masuIdNext = GwPlayer[i].masuIdNext;
            if (workP->restF || !workP->snapF) {
                continue;
            }
        }
        orderNo = GwPlayer[i].orderNo;
        cornerNo = 1;
        if (playerNo < 0) {
            cornerNo = 0;
        }
        for (j = 0; j < GW_PLAYER_MAX; j++) {
            if (i != j && masuId == GwPlayer[j].masuId &&
                orderNo > GwPlayer[j].orderNo) {
                cornerNo++;
            }
        }
        if (cornerNo != 0) {
            mbMasuCornerRotPosGet(masuId, cornerNo - 1, &pos);
        } else {
            mbMasuPosGet(masuId, &pos);
        }
        workP = omObjGetWork(playerWork[i].colObj, PLAYERCOLWORK);
        circleF = workP->circleF;
        workP->circleF = FALSE;
        if (snapF) {
            mbPlayerPosSetV(i, &pos);
            PlayerColCornerSnap(i, masuId, cornerNo);
        } else if (cornerNo != mbPlayerMasuCornerGet(i) || circleF) {
            PlayerColInit(i, masuId, cornerNo);
        }
        mbPlayerMasuCornerSet(i, cornerNo);
    }
    if (playerNo >= 0) {
        mbPlayerMasuCornerSet(playerNo, 0);
    }
}

void mbev_PlayerColBall(int masuId, int *playerNoTbl, HuVecF *posTbl)
{
    int useF[GW_PLAYER_MAX] = { 0, 0, 0, 0 };
    int playerNoSort[GW_PLAYER_MAX];
    int orderNo[GW_PLAYER_MAX];
    HuVecF pos[GW_PLAYER_MAX];
    int num;
    int i;
    int j;

    for (i = 0; i < GW_PLAYER_MAX; i++) {
        if (playerNoTbl[i] >= 0) {
            useF[playerNoTbl[i]] = TRUE;
        }
    }
    num = 0;
    for (i = 0; i < GW_PLAYER_MAX; i++) {
        if (masuId != GwPlayer[i].masuId && !useF[i]) {
            continue;
        }
        orderNo[num] = GwPlayer[i].orderNo;
        if (orderNo[num] != 0 && !mbPlayerColSnapGet(i)) {
            orderNo[num] += GW_PLAYER_MAX;
        }
        playerNoSort[num] = i;
        num++;
    }
    for (i = 0; i < num - 1; i++) {
        for (j = i + 1; j < num; j++) {
            if (orderNo[i] > orderNo[j]) {
                int temp = orderNo[i];

                orderNo[i] = orderNo[j];
                orderNo[j] = temp;
                temp = playerNoSort[i];
                playerNoSort[i] = playerNoSort[j];
                playerNoSort[j] = temp;
            }
        }
    }
    for (i = 0; i < num; i++) {
        int playerNo = playerNoSort[i];
        int cornerNo = i;

        if (cornerNo != 0) {
            mbMasuCornerRotPosGet(masuId, cornerNo - 1, &pos[playerNo]);
        } else {
            mbMasuPosGet(masuId, &pos[playerNo]);
        }
    }
    for (i = 0; i < GW_PLAYER_MAX; i++) {
        if (playerNoTbl[i] >= 0) {
            posTbl[i] = pos[playerNoTbl[i]];
        }
    }
}

void mbev_PlayerColMasuSet(int playerNo, int masuId, BOOL waitF)
{
    int masuIdTbl[GW_PLAYER_MAX];
    int i;

    for (i = 0; i < GW_PLAYER_MAX; i++) {
        masuIdTbl[i] = -1;
    }
    masuIdTbl[playerNo] = masuId;
    mbev_PlayerColMasuAllSet(masuIdTbl, waitF);
}

static void PlayerColCornerSet(int playerNo, int masuIdNext)
{
    int masuIdFix[GW_PLAYER_MAX];
    int i;
    int j;
    int cornerNo;
    int masuId;
    s8 orderNo;
    HuVecF pos;

    for (i = 0; i < GW_PLAYER_MAX; i++) {
        masuIdFix[i] = -1;
    }
    masuIdFix[playerNo] = masuIdNext;
    for (i = 0; i < GW_PLAYER_MAX; i++) {
        PLAYERCOLWORK *workP;
        u8 circleF;

        if (playerWork[i].colObj == NULL) {
            continue;
        }
        if (GwPlayer[i].masuId == 0) {
            continue;
        }
        workP = omObjGetWork(playerWork[i].colObj, PLAYERCOLWORK);
        workP->masuIdNext = GwPlayer[i].masuIdNext;
        if (i == playerNo) {
            continue;
        }
        if (workP->restF || !workP->snapF) {
            continue;
        }
        masuId = GwPlayer[i].masuId;
        orderNo = GwPlayer[i].orderNo;
        cornerNo = 0;
        for (j = 0; j < GW_PLAYER_MAX; j++) {
            if (i != j && masuId == masuIdFix[j]) {
                cornerNo++;
            }
        }
        for (j = 0; j < GW_PLAYER_MAX; j++) {
            if (i != j && masuIdFix[j] < 0 &&
                masuId == GwPlayer[j].masuId &&
                orderNo > GwPlayer[j].orderNo) {
                cornerNo++;
            }
        }
        if (cornerNo == 0) {
            mbMasuPosGet(masuId, &pos);
        } else {
            mbMasuCornerRotPosGet(masuId, cornerNo - 1, &pos);
        }
        circleF = workP->circleF;
        workP->circleF = FALSE;
        if (cornerNo != mbPlayerMasuCornerGet(i) || circleF) {
            PlayerColInit(i, masuId, cornerNo);
        }
        mbPlayerMasuCornerSet(i, cornerNo);
    }
}

static void PlayerColCornerSnap(int playerNo, int masuId, int cornerNo)
{
    MBPLAYERWORK *playerWorkP = mbPlayerWorkGet(playerNo);
    PLAYERCOLWORK *workP =
        omObjGetWork(playerWorkP->colObj, PLAYERCOLWORK);
    Mtx masuMtx;
    Mtx masuMtxInv;
    HuVecF pos;

    if (workP->snapF) {
        workP->motStartF = FALSE;
        workP->state = 0;
        workP->killF = FALSE;
        workP->masuId = masuId;
        playerWorkP->masuCorner = cornerNo;
        mbMasuMtxGet(workP->masuId, masuMtx);
        MTXInverse(masuMtx, masuMtxInv);
        mbPlayerPosGet(playerNo, &pos);
        MTXMultVec(masuMtxInv, &pos, &playerWork[playerNo]._unk3C);
        MTXMultVec(
            masuMtxInv, &pos, &playerWork[playerNo].colObj->trans);
        mbPlayerRotYSet(playerNo, 0.0f);
        workP->rotYStart = 0.0f;
        GwPlayer[playerNo].moveF = FALSE;
    }
}

static void PlayerColInit(int playerNo, int masuId, int cornerNo)
{
    MBPLAYERWORK *playerWorkP = mbPlayerWorkGet(playerNo);
    PLAYERCOLWORK *workP =
        omObjGetWork(playerWorkP->colObj, PLAYERCOLWORK);
    Mtx masuMtx;
    Mtx masuMtxInv;
    HuVecF posPlayer;
    HuVecF posMasu;
    HuVecF posDiff;
    float rotY;

    if (workP->snapF) {
        workP->motStartF = FALSE;
        workP->state = 1;
        workP->killF = FALSE;
        workP->masuId = masuId;
        playerWorkP->masuCorner = cornerNo;
        mbMasuMtxGet(workP->masuId, masuMtx);
        MTXInverse(masuMtx, masuMtxInv);
        mbPlayerPosGet(playerNo, &posPlayer);
        MTXMultVec(
            masuMtxInv, &posPlayer, &playerWork[playerNo].colObj->trans);
        GwPlayer[playerNo].moveF = TRUE;
        if (playerWorkP->masuCorner == 0) {
            mbMasuPosGet(workP->masuId, &posMasu);
        } else {
            mbMasuCornerRotPosGet(workP->masuId,
                playerWorkP->masuCorner - 1, &posMasu);
            if (workP->circleF) {
                float scale;

                mbMasuPosGet(workP->masuId, &posDiff);
                VECSubtract(&posMasu, &posDiff, &posMasu);
                scale = workP->radius / VECMag(&posMasu);
                VECScale(&posMasu, &posMasu, scale);
                VECAdd(&posMasu, &posDiff, &posMasu);
            }
        }
        VECSubtract(&posMasu, &posPlayer, &posDiff);
        rotY = 90.0f - HuAtan(posDiff.z, posDiff.x);
        mbPlayerRotYSet(playerNo, rotY);
        workP->rotYStart = rotY;
    }
}

BOOL mbPlayerColCheck(void)
{
    int i;

    for (i = 0; i < GW_PLAYER_MAX; i++) {
        PLAYERCOLWORK *workP =
            omObjGetWork(playerWork[i].colObj, PLAYERCOLWORK);

        if (workP->state) {
            return FALSE;
        }
    }
    return TRUE;
}

static void PlayerColKill(int playerNo)
{
}

void mbev_PlayerColReserve(int playerNo, int masuId, BOOL waitF)
{
    int masuIdTbl[GW_PLAYER_MAX];
    int i;

    for (i = 0; i < GW_PLAYER_MAX; i++) {
        masuIdTbl[i] = -1;
    }
    masuIdTbl[playerNo] = masuId;
    mbev_PlayerColMasuAllSet(masuIdTbl, waitF);
}

static void PlayerColOMExec(OMOBJ *obj)
{
    PLAYERCOLWORK *workP = omObjGetWork(obj, PLAYERCOLWORK);
    int playerNo = workP->playerNo;
    MBPLAYERWORK *playerWorkP = mbPlayerWorkGet(playerNo);
    Mtx masuMtx;
    Mtx masuMtxInv;
    HuVecF pos;
    HuVecF posDiff;
    float weight;
    float rotY;

    if (mbExitCheck()) {
        omDelObjEx(HuPrcCurrentGet(), obj);
        playerWorkP->colObj = NULL;
        return;
    }
    if (GwPlayer[playerNo].masuId == 0) {
        return;
    }
    if (workP->masuIdNext != GwPlayer[playerNo].masuIdNext) {
        workP->masuIdNext = GwPlayer[playerNo].masuIdNext;
        PlayerColCornerSet(playerNo, workP->masuIdNext);
    }
    if (!workP->snapF || playerWorkP->moveObj || playerWorkP->posFixObj) {
        workP->killF = TRUE;
        workP->state = 0;
        return;
    }
    if (workP->killF) {
        workP->killF = FALSE;
        workP->masuId = GwPlayer[playerNo].masuId;
        workP->masuIdNext = GwPlayer[playerNo].masuIdNext;
        mbMasuMtxGet(workP->masuId, masuMtx);
        MTXInverse(masuMtx, masuMtxInv);
        mbPlayerPosGet(playerNo, &pos);
        MTXMultVec(masuMtxInv, &pos, &playerWorkP->_unk3C);
        MTXMultVec(masuMtxInv, &pos, &obj->trans);
        playerWorkP->_unk3C.y = obj->trans.y = 0.0f;
    }
    switch (workP->state) {
        case 0:
            obj->trans.x = playerWorkP->_unk3C.x;
            obj->trans.y = playerWorkP->_unk3C.y;
            obj->trans.z = playerWorkP->_unk3C.z;
            break;

        case 1:
            if (!workP->motStartF) {
                mbPlayerMotionSet(playerNo, 3, HU3D_MOTATTR_LOOP);
                workP->motStartF = TRUE;
                workP->time = 0;
                workP->maxTime = 12;
                obj->rot.x = obj->trans.x;
                obj->rot.y = obj->trans.y;
                obj->rot.z = obj->trans.z;
            }
            if (playerWorkP->masuCorner == 0) {
                playerWorkP->_unk3C.x = playerWorkP->_unk3C.y =
                    playerWorkP->_unk3C.z = 0.0f;
            } else {
                mbMasuCornerPosGet(workP->masuId,
                    playerWorkP->masuCorner - 1, &playerWorkP->_unk3C);
                if (workP->circleF) {
                    weight = workP->radius / VECMag(&playerWorkP->_unk3C);
                    VECScale(&playerWorkP->_unk3C,
                        &playerWorkP->_unk3C, weight);
                }
            }
            if (workP->time > workP->maxTime) {
                workP->state = 2;
                workP->time = 0;
                workP->maxTime = 8;
                obj->trans.x = playerWorkP->_unk3C.x;
                obj->trans.y = playerWorkP->_unk3C.y;
                obj->trans.z = playerWorkP->_unk3C.z;
                mbPlayerMotionShiftSet(
                    playerNo, 1, 0.0f, 8.0f, HU3D_MOTATTR_LOOP);
            } else {
                weight = (float)(workP->time++) / workP->maxTime;
                VECSubtract(&playerWorkP->_unk3C, &obj->rot, &posDiff);
                obj->trans.x = obj->rot.x +
                    (weight * (playerWorkP->_unk3C.x - obj->rot.x));
                obj->trans.y = obj->rot.y +
                    (weight * (playerWorkP->_unk3C.y - obj->rot.y));
                obj->trans.z = obj->rot.z +
                    (weight * (playerWorkP->_unk3C.z - obj->rot.z));
            }
            break;

        case 2:
            if (workP->time > workP->maxTime) {
                mbPlayerRotYSet(playerNo, 0.0f);
                GwPlayer[playerNo].moveF = FALSE;
                workP->state = 0;
            }
            weight = (float)(workP->time++) / workP->maxTime;
            rotY = mbAngleEaseOut(workP->rotYStart, 0.0f, weight);
            mbPlayerRotYSet(playerNo, rotY);
            break;
    }
    mbMasuMtxGet(workP->masuId, masuMtx);
    MTXMultVec(masuMtx, &obj->trans, &pos);
    mbPlayerPosSetV(playerNo, &pos);
}

void mbev_PlayerColSet(int playerNo, int masuId)
{
    int i;
    int cornerNo;
    HuVecF pos;
    Mtx masuMtx;
    Mtx masuMtxInv;
    HuVecF posPlayer;

    if (!playerColSnapF) {
        return;
    }
    for (i = 0; i < GW_PLAYER_MAX; i++) {
        MBPLAYERWORK *playerWorkP;
        PLAYERCOLWORK *workP;

        if (GwPlayer[i].moveF) {
            continue;
        }
        if (masuId != GwPlayer[i].masuId) {
            continue;
        }
        cornerNo = mbPlayerMasuCornerGet(i);
        if (cornerNo == 0) {
            mbMasuPosGet(masuId, &pos);
        } else {
            mbMasuCornerRotPosGet(masuId, cornerNo - 1, &pos);
        }
        mbPlayerPosSetV(i, &pos);
        playerWorkP = mbPlayerWorkGet(i);
        workP = omObjGetWork(playerWorkP->colObj, PLAYERCOLWORK);
        if (!workP->snapF) {
            continue;
        }
        workP->motStartF = FALSE;
        workP->state = 0;
        workP->killF = FALSE;
        workP->masuId = masuId;
        playerWorkP->masuCorner = cornerNo;
        mbMasuMtxGet(workP->masuId, masuMtx);
        MTXInverse(masuMtx, masuMtxInv);
        mbPlayerPosGet(i, &posPlayer);
        MTXMultVec(masuMtxInv, &posPlayer, &playerWorkP->_unk3C);
        MTXMultVec(
            masuMtxInv, &posPlayer, &playerWorkP->colObj->trans);
        mbPlayerRotYSet(i, 0.0f);
        workP->rotYStart = 0.0f;
        GwPlayer[i].moveF = FALSE;
    }
}

void mbPlayerColSnapSet(BOOL snapF)
{
    BOOL snap = snapF ? TRUE : FALSE;
    int i;

    for (i = 0; i < GW_PLAYER_MAX; i++) {
        if (playerWork[i].colObj) {
            PLAYERCOLWORK *workP =
                omObjGetWork(playerWork[i].colObj, PLAYERCOLWORK);

            workP->snapF = snap;
        }
    }
    playerColSnapF = snapF;
}

void mbPlayerColSnapPlayerSet(int playerNo, BOOL snapF)
{
    BOOL snap = snapF ? TRUE : FALSE;

    if (playerWork[playerNo].colObj) {
        PLAYERCOLWORK *workP =
            omObjGetWork(playerWork[playerNo].colObj, PLAYERCOLWORK);

        workP->snapF = snap;
    }
}

BOOL mbPlayerColSnapGet(int playerNo)
{
    PLAYERCOLWORK *workP =
        omObjGetWork(playerWork[playerNo].colObj, PLAYERCOLWORK);

    return workP->snapF;
}

void mbPlayerColRestSet(int playerNo, BOOL restF)
{
    BOOL rest = restF ? FALSE : TRUE;

    if (playerWork[playerNo].colObj) {
        PLAYERCOLWORK *workP =
            omObjGetWork(playerWork[playerNo].colObj, PLAYERCOLWORK);

        workP->restF = rest;
    }
}

void mbPlayerColFirstSet(int playerNo)
{
    int orderNo = 1;
    int i;

    GwPlayer[playerNo].orderNo = 0;
    for (i = 0; i < GW_PLAYER_MAX; i++) {
        if (playerNo != i) {
            GwPlayer[i].orderNo = orderNo++;
        }
    }
}

void mbPlayerColOrderReset(void)
{
    s8 playerNo[GW_PLAYER_MAX];
    s8 orderNo[GW_PLAYER_MAX];
    s8 fixF[GW_PLAYER_MAX];
    int i;
    int j;
    int k;
    int num;
    s16 masuId;
    s8 temp;

    memset(fixF, 0, GW_PLAYER_MAX);
    for (i = 0; i < GW_PLAYER_MAX; i++) {
        PLAYERCOLWORK *workP;
        BOOL restF = FALSE;

        if (playerWork[i].colObj) {
            workP = omObjGetWork(playerWork[i].colObj, PLAYERCOLWORK);

            workP->restF = restF;
        }
        if (GwPlayer[i].masuId == 0) {
            continue;
        }
        if (fixF[i]) {
            continue;
        }
        masuId = GwPlayer[i].masuId;
        playerNo[0] = i;
        orderNo[0] = GwPlayer[i].orderNo;
        num = 1;
        for (j = 0; j < GW_PLAYER_MAX; j++) {
            if (i != j && masuId == GwPlayer[j].masuId) {
                playerNo[num] = j;
                orderNo[num] = GwPlayer[j].orderNo;
                num++;
            }
        }
        if (num > 1) {
            for (j = 0; j < num - 1; j++) {
                for (k = j + 1; k < num; k++) {
                    if (orderNo[j] > orderNo[k]) {
                        temp = orderNo[j];
                        orderNo[j] = orderNo[k];
                        orderNo[k] = temp;
                    }
                }
            }
        }
        for (j = 0; j < num; j++) {
            GwPlayer[playerNo[j]].orderNo =
                orderNo[mbPlayerMasuCornerGet(playerNo[j])];
            fixF[playerNo[j]] = TRUE;
        }
    }
}

typedef struct PlayerMetalWork {
    u8 killF : 1;
    u8 _unk0_1 : 1;
    u8 _unk0_2 : 1;
    u8 effectF : 1;
    u8 playerNo : 2;
    s16 time;
    s16 maxTime;
    s16 _unk06;
    s16 _unk08;
} PLAYERMETALWORK;

typedef struct PlayerBiriQWork {
    u8 killF : 1;
    u8 _unk0_1 : 1;
    u8 flashF : 1;
    u8 _unk0_3 : 1;
    u8 effectF : 1;
    u8 playerNo : 2;
    s16 time;
    s16 maxTime;
    s16 _unk06;
    s16 _unk08;
} PLAYERBIRIQWORK;

static void PlayerBiriQEffectSet(int playerNo, BOOL effectF);

static void PlayerMetalOMExec(OMOBJ *objP)
{
    PLAYERMETALWORK *workP = omObjGetWork(objP, PLAYERMETALWORK);
    BOOL killF = FALSE;
    float weight;

    if (mbExitCheck() || workP->killF) {
        if (workP->killF) {
            killF = TRUE;
        }
        workP->killF = TRUE;
    }
    if (!workP->killF) {
        if (!GwPlayer[workP->playerNo].metalF && !workP->_unk0_2) {
            workP->_unk0_2 = TRUE;
            workP->time = 0;
            workP->maxTime = 20;
        }
        if (workP->_unk0_2) {
            workP->time++;
            weight = (float)workP->time / workP->maxTime;
            if (weight > 1.0f) {
                weight = 1.0f;
            }
            mbObjMetalTPLvlSet(
                mbPlayerObjIDGet(workP->playerNo), 1.0f - weight);
            if (workP->time >= workP->maxTime) {
                workP->killF = TRUE;
            }
        } else if (!workP->_unk0_1) {
            workP->time++;
            weight = (float)workP->time / workP->maxTime;
            if (weight > 1.0f) {
                weight = 1.0f;
            }
            mbObjMetalTPLvlSet(mbPlayerObjIDGet(workP->playerNo), weight);
            if (workP->time >= workP->maxTime) {
                workP->_unk0_1 = TRUE;
            }
        }
        if (objP->mdlId[0] >= 0) {
            if (!mbObjGet(mbPlayerObjIDGet(workP->playerNo))->dispF
                || !workP->effectF) {
                Hu3DModelAttrSet(objP->mdlId[0], HU3D_ATTR_DISPOFF);
            } else {
                Hu3DModelAttrReset(objP->mdlId[0], HU3D_ATTR_DISPOFF);
            }
        }
    }
    if (workP->killF) {
        if (!killF) {
            PlayerMetalKill(workP->playerNo);
        }
        if (objP->data != NULL) {
            void *dataP = objP->data;

            HuMemDirectFree(dataP);
        }
        omDelObjEx(HuPrcCurrentGet(), objP);
    }
}

void mbPlayerMetalSet(int playerNo, BOOL metalF)
{
    OMOBJ *objP;
    PLAYERMETALWORK *workP;

    if (metalF) {
        if (playerWork[playerNo].metalObj != NULL) {
            OSReport("------------already METAL!!----------");
        }
        GwPlayer[playerNo].metalF = TRUE;
        objP = playerWork[playerNo].metalObj;
        if (objP == NULL) {
            objP = playerWork[playerNo].metalObj = omAddObjEx(mbObjMan,
                0x100, 1, 0, -1, PlayerMetalOMExec);
            omSetStatBit(objP, OM_STAT_MODELPAUSE);
            mbObjMetalCreate(mbPlayerObjIDGet(playerNo));
            mbObjMetalTPLvlSet(mbPlayerObjIDGet(playerNo), 0.0f);
            mbObjMetalColorSet(mbPlayerObjIDGet(playerNo),
                metalShadowColor, metalHiliteColor);
        }
        workP = omObjGetWork(objP, PLAYERMETALWORK);
        workP->playerNo = playerNo;
        workP->_unk0_2 = FALSE;
        workP->_unk0_1 = FALSE;
        workP->effectF = TRUE;
        workP->time = 0;
        workP->maxTime = 20;
        if (objP->mdlId[0] <= 0) {
            MetalEffectCreate(objP);
        }
        CharModelStepSet(GwPlayer[playerNo].charNo, 6);
    } else {
        GwPlayer[playerNo].metalF = FALSE;
        CharModelStepSet(GwPlayer[playerNo].charNo, 0);
    }
}

static void PlayerMetalKill(int playerNo)
{
    OMOBJ *objP = playerWork[playerNo].metalObj;

    if (objP != NULL) {
        PLAYERMETALWORK *workP = omObjGetWork(objP, PLAYERMETALWORK);

        workP->killF = TRUE;
        playerWork[playerNo].metalObj = NULL;
        mbObjMetalKill(mbPlayerObjIDGet(playerNo));
        mbParticleKill(objP->mdlId[0]);
        objP->mdlId[0] = -1;
    }
}

void mbPlayerEffectSet(int playerNo, BOOL effectF)
{
    OMOBJ *objP = playerWork[playerNo].metalObj;

    if (objP) {
        PLAYERMETALWORK *workP = omObjGetWork(objP, PLAYERMETALWORK);

        workP->effectF = effectF;
    }
    PlayerBiriQEffectSet(playerNo, effectF);
}

static void ResetMetalColor(void)
{
    metalShadowColor = metalDefaultColor[0];
    metalHiliteColor = metalDefaultColor[1];
}

void mbPlayerMetalColorSet(
    const GXColor *shadowColor, const GXColor *hiliteColor)
{
    metalShadowColor = *shadowColor;
    metalHiliteColor = *hiliteColor;
}

static float GetBiriQEffectRadius(
    OMOBJ *objP, int playerNo, int *effectCount)
{
    HSF_DATA *hsfP;
    HSF_OBJECT *objectP;
    HuVecF min;
    HuVecF size;
    HuVecF pos;
    HuVecF *vertexP;
    void *dataP;
    s16 *groupP;
    s16 *countP;
    s16 (*vertexNoP)[8];
    int objectNo = -1;
    int vertexNum = 0;
    int groupNum;
    int i;
    int x;
    int y;
    int z;
    int groupNo;
    int randomNo;
    s16 count;
    float radius;

    hsfP = Hu3DData[mbPlayerModelIDGet(playerNo)].hsf;
    objectP = hsfP->object;
    for (i = 0; i < hsfP->objectNum; i++, objectP++) {
        if (objectP->type == HSF_OBJ_MESH
            && objectP->mesh.vertex->count > 0
            && vertexNum < objectP->mesh.vertex->count) {
            vertexNum = objectP->mesh.vertex->count;
            objectNo = i;
        }
    }
    effectCount[0] = objectNo;
    dataP = HuMemDirectMallocNum(HEAP_HEAP,
        (125 + 125 + (125 * 8)) * sizeof(s16), HU_MEMNUM_OVL);
    objP->data = dataP;
    groupP = objP->data;
    countP = groupP + 125;
    vertexNoP = (s16 (*)[8])(countP + 125);
    memset(countP, 0, 125 * sizeof(s16));
    objectP = &hsfP->object[effectCount[0]];
    VECScale(&objectP->mesh.mesh.min, &min, -1.0f);
    VECSubtract(&objectP->mesh.mesh.max,
        &objectP->mesh.mesh.min, &size);
    size.x += 1.0f;
    size.y += 1.0f;
    size.z += 1.0f;
    if (size.x > 0.0f) {
        size.x = 5.0f / size.x;
    }
    if (size.y > 0.0f) {
        size.y = 5.0f / size.y;
    }
    if (size.z > 0.0f) {
        size.z = 5.0f / size.z;
    }
    vertexNum = objectP->mesh.vertex->count;
    vertexP = objectP->mesh.vertex->data;
    for (i = 0; i < vertexNum; i++, vertexP++) {
        VECAdd(vertexP, &min, &pos);
        pos.x *= size.x;
        pos.y *= size.y;
        pos.z *= size.z;
        x = pos.x;
        if (x < 0) {
            x += 5;
        }
        if (x >= 5) {
            x -= 5;
        }
        y = pos.y;
        if (y < 0) {
            y += 5;
        }
        if (y >= 5) {
            y -= 5;
        }
        z = pos.z;
        if (z < 0) {
            z += 5;
        }
        if (z >= 5) {
            z -= 5;
        }
        groupNo = x + (5 * y) + (25 * z);
        count = countP[groupNo];
        if (count < 8) {
            vertexNoP[groupNo][count] = i;
        } else {
            randomNo = mbRandMod(count + 1);
            if (randomNo < 8) {
                vertexNoP[groupNo][randomNo] = i;
            }
        }
        countP[groupNo]++;
    }
    groupNum = 0;
    for (i = 0; i < 125; i++) {
        if (countP[i] > 0) {
            groupP[groupNum++] = i;
            if (countP[i] > 8) {
                countP[i] = 8;
            }
        }
    }
    effectCount[1] = groupNum;
    radius = objectP->mesh.mesh.max.y - objectP->mesh.mesh.min.y;
    return radius;
}

static void MetalEffectCreate(OMOBJ *objP)
{
    PLAYERMETALWORK *workP = omObjGetWork(objP, PLAYERMETALWORK);
    HSF_DATA *hsfP;
    HSF_OBJECT *objectP;
    MBPARTICLE *particleP;
    MBPARTICLEDATA *particleDataP;
    HuVecF min;
    HuVecF size;
    HuVecF pos;
    HuVecF *vertexP;
    void *dataP;
    s16 *groupP;
    s16 *countP;
    s16 (*vertexNoP)[8];
    int particleNum = 5;
    int objectNo = -1;
    int vertexNum = 0;
    int groupNum;
    int i;
    int x;
    int y;
    int z;
    int groupNo;
    int randomNo;
    s16 count;

    objP->mdlId[0] = mbParticleCreate(HuSprAnimRead(HuDataReadNum(
        mbBoardDataNumGet(DATANUM(DATA_board, 106)), HU_MEMNUM_OVL)),
        (s16)particleNum);
    mbParticleHookSet(objP->mdlId[0], MetalEffectHook);
    Hu3DModelLayerSet(objP->mdlId[0], 5);
    hsfP = Hu3DData[mbPlayerModelIDGet(workP->playerNo)].hsf;
    objectP = hsfP->object;
    for (i = 0; i < hsfP->objectNum; i++, objectP++) {
        if (objectP->type == HSF_OBJ_MESH
            && objectP->mesh.vertex->count > 0
            && vertexNum < objectP->mesh.vertex->count) {
            vertexNum = objectP->mesh.vertex->count;
            objectNo = i;
        }
    }
    workP->_unk06 = objectNo;
    particleP = Hu3DData[objP->mdlId[0]].hookData;
    particleP->hookData = objP;
    particleDataP = particleP->data;
    for (i = 0; i < particleNum; i++, particleDataP++) {
        particleDataP->vertexNo = 0;
    }
    dataP = HuMemDirectMallocNum(HEAP_HEAP,
        (125 + 125 + (125 * 8)) * sizeof(s16), HU_MEMNUM_OVL);
    objP->data = dataP;
    groupP = objP->data;
    countP = groupP + 125;
    vertexNoP = (s16 (*)[8])(countP + 125);
    memset(countP, 0, 125 * sizeof(s16));
    objectP = &hsfP->object[workP->_unk06];
    VECScale(&objectP->mesh.mesh.min, &min, -1.0f);
    VECSubtract(&objectP->mesh.mesh.max,
        &objectP->mesh.mesh.min, &size);
    size.x += 1.0f;
    size.y += 1.0f;
    size.z += 1.0f;
    if (size.x > 0.0f) {
        size.x = 5.0f / size.x;
    }
    if (size.y > 0.0f) {
        size.y = 5.0f / size.y;
    }
    if (size.z > 0.0f) {
        size.z = 5.0f / size.z;
    }
    vertexNum = objectP->mesh.vertex->count;
    vertexP = objectP->mesh.vertex->data;
    for (i = 0; i < vertexNum; i++, vertexP++) {
        VECAdd(vertexP, &min, &pos);
        pos.x *= size.x;
        pos.y *= size.y;
        pos.z *= size.z;
        x = pos.x;
        if (x < 0) {
            x += 5;
        }
        if (x >= 5) {
            x -= 5;
        }
        y = pos.y;
        if (y < 0) {
            y += 5;
        }
        if (y >= 5) {
            y -= 5;
        }
        z = pos.z;
        if (z < 0) {
            z += 5;
        }
        if (z >= 5) {
            z -= 5;
        }
        groupNo = x + (5 * y) + (25 * z);
        count = countP[groupNo];
        if (count < 8) {
            vertexNoP[groupNo][count] = i;
        } else {
            randomNo = mbRandMod(count + 1);
            if (randomNo < 8) {
                vertexNoP[groupNo][randomNo] = i;
            }
        }
        countP[groupNo]++;
    }
    groupNum = 0;
    for (i = 0; i < 125; i++) {
        if (countP[i] > 0) {
            groupP[groupNum++] = i;
            if (countP[i] > 8) {
                countP[i] = 8;
            }
        }
    }
    workP->_unk08 = groupNum;
}

static void MetalEffectHook(
    HU3D_MODEL *modelP, MBPARTICLE *particleP, Mtx matrix)
{
    OMOBJ *objP = particleP->hookData;
    PLAYERMETALWORK *workP = omObjGetWork(objP, PLAYERMETALWORK);
    HSF_DATA *hsfP;
    HSF_OBJECT *objectP = NULL;
    MBPARTICLEDATA *dataP;
    Mtx modelMtx;
    HuVecF cameraPos;
    HuVecF pos;
    HuVecF dir;
    s16 *groupP;
    s16 *countP;
    s16 (*vertexNoP)[8];
    int groupNo;
    int randomNo;
    int i;
    BOOL firstF = FALSE;
    BOOL posF;
    float weight;
    float mag;

    if (!particleP->initF) {
        dataP = particleP->data;
        for (i = 0; i < particleP->num; i++, dataP++) {
            dataP->scale = 0.0f;
            dataP->color.a = 0;
            dataP->time = 0;
        }
        particleP->initF = TRUE;
        particleP->colorIn[0] = GX_CC_RASC;
        particleP->colorIn[1] = GX_CC_C0;
        particleP->colorIn[2] = GX_CC_TEXC;
        particleP->colorIn[3] = GX_CC_ZERO;
        particleP->tevColor[0].r = particleP->tevColor[0].g =
            particleP->tevColor[0].b = particleP->tevColor[0].a = 255;
        particleP->blendMode = MB_PARTICLE_BLEND_ADDCOL;
        firstF = TRUE;
    }
    MTXInverse(Hu3DCameraMtx, modelMtx);
    cameraPos.x = modelMtx[0][3];
    cameraPos.y = modelMtx[1][3];
    cameraPos.z = modelMtx[2][3];
    hsfP = Hu3DData[mbPlayerModelIDGet(workP->playerNo)].hsf;
    objectP = &hsfP->object[workP->_unk06];
    Hu3DModelObjMtxGet(
        mbPlayerModelIDGet(workP->playerNo), objectP->name, modelMtx);
    dataP = particleP->data;
    for (i = 0; i < particleP->num; i++, dataP++) {
        if (dataP->time == 0) {
            if (workP->_unk0_2) {
                continue;
            }
            dataP->time++;
            groupP = objP->data;
            countP = groupP + 125;
            vertexNoP = (s16 (*)[8])(countP + 125);
            groupNo = groupP[mbRandMod(workP->_unk08)];
            randomNo = mbRandMod(countP[groupNo]);
            dataP->vertexNo = vertexNoP[groupNo][randomNo];
            dataP->color.r = mbRandMod(120) + 120;
            dataP->color.g = mbRandMod(120) + 120;
            dataP->color.b = mbRandMod(120) + 120;
            dataP->color.a = mbRandMod(48) + 100;
            dataP->vel.x = 30.0f + (60.0f * frandf());
            dataP->scale = 0.0f;
            dataP->vel.y = (6.0f * frandf()) - 3.0f;
            dataP->rot.z = 360.0f * frandf();
            dataP->activeF = mbRandMod(30) + 30;
            dataP->animBank = mbRandMod(2);
            dataP->dispF = TRUE;
            if (firstF) {
                dataP->vel.x = 60.0f + (80.0f * frandf());
                dataP->activeF = mbRandMod(40) + 40;
            }
        }
        posF = TRUE;
        dataP->time++;
        weight = (float)dataP->time / dataP->activeF;
        dataP->scale = dataP->vel.x * mbSinDeg(180.0f * weight);
        dataP->rot.z += dataP->vel.y;
        if (weight > 0.8f) {
            dataP->color.a *= 0.9f;
        }
        if (weight > 0.6f) {
            posF = FALSE;
        }
        if (dataP->time >= dataP->activeF) {
            dataP->time = 0;
            dataP->color.a = 0;
            dataP->scale = 0.0f;
            dataP->dispF = FALSE;
        }
        if (posF) {
            MTXMultVec(modelMtx,
                &((HuVecF *)objectP->mesh.vertex->data)[dataP->vertexNo],
                &pos);
            VECSubtract(&cameraPos, &pos, &dir);
            mag = VECMag(&dir);
            if (mag > 0.0f) {
                VECScale(&dir, &dir, 200.0f / mag);
            }
            VECAdd(&pos, &dir, &dataP->pos);
        }
    }
}

static void PlayerBiriQOMExec(OMOBJ *objP)
{
    PLAYERBIRIQWORK *workP = omObjGetWork(objP, PLAYERBIRIQWORK);
    BOOL killF = FALSE;
    BOOL dispF = TRUE;

    if (mbExitCheck() || workP->killF) {
        if (workP->killF) {
            killF = TRUE;
        }
        workP->killF = TRUE;
    }
    if (!workP->killF) {
        if (!GwPlayer[workP->playerNo].biriQF && !workP->_unk0_3) {
            workP->_unk0_3 = TRUE;
            workP->time = 0;
            workP->maxTime = 20;
        }
        if (workP->_unk0_3) {
            workP->killF = TRUE;
        } else if (workP->flashF) {
            workP->time = 0;
            workP->maxTime = 24;
            workP->flashF = FALSE;
            workP->_unk0_1 = FALSE;
        } else if (!workP->_unk0_1) {
            GXColor color = { 255, 255, 255, 255 };
            int colorNoTbl[4] = { 1, 3, 2, 3 };
            float level = 1.0f;
            int colorNo = colorNoTbl[(workP->time >> 1) & 3];

            if (colorNo == 0) {
                level = 0.0f;
            }
            mbObjBiriQColorSet(
                mbPlayerObjIDGet(workP->playerNo), colorNo, level, color);
            if (workP->time >= workP->maxTime) {
                workP->_unk0_1 = TRUE;
                mbObjBiriQColorSet(mbPlayerObjIDGet(workP->playerNo),
                    FALSE, 0.0f, color);
                if (objP->mdlId[0] < 0) {
                    BiriQEffectCreate(objP);
                }
            }
            workP->time++;
            dispF = FALSE;
        } else {
            GXColor color = { 255, 255, 255, 255 };
            int colorNoTbl[4] = { 1, 3, 2, 3 };
            float level = 0.1f;
            int colorNo = colorNoTbl[(workP->time >> 1) & 3];

            if (colorNo == 0) {
                level = 0.0f;
            }
            mbObjBiriQColorSet(
                mbPlayerObjIDGet(workP->playerNo), colorNo, level, color);
            workP->time++;
        }
        if (objP->mdlId[0] >= 0) {
            if (!mbObjGet(mbPlayerObjIDGet(workP->playerNo))->dispF
                || !workP->effectF || !dispF) {
                Hu3DModelAttrSet(objP->mdlId[0], HU3D_ATTR_DISPOFF);
                Hu3DModelAttrSet(objP->mdlId[1], HU3D_ATTR_DISPOFF);
            } else {
                Hu3DModelAttrReset(objP->mdlId[0], HU3D_ATTR_DISPOFF);
                Hu3DModelAttrReset(objP->mdlId[1], HU3D_ATTR_DISPOFF);
            }
        }
    }
    if (workP->killF) {
        if (!killF) {
            PlayerBiriQKill(workP->playerNo);
        }
        if (objP->data) {
            void *dataP = objP->data;

            HuMemDirectFree(dataP);
        }
        omDelObjEx(HuPrcCurrentGet(), objP);
        return;
    }
}

void mbPlayerBiriQSet(int playerNo, BOOL biriQF)
{
    OMOBJ *objP;
    PLAYERBIRIQWORK *workP;

    if (biriQF) {
        if (playerWork[playerNo].biriQObj != NULL) {
            OSReport("------------already BiriQ!!----------");
        }
        GwPlayer[playerNo].biriQF = TRUE;
        objP = playerWork[playerNo].biriQObj;
        if (objP == NULL) {
            GXColor color = { 255, 255, 255, 255 };

            objP = playerWork[playerNo].biriQObj = omAddObjEx(mbObjMan,
                0x100, 2, 0, -1, PlayerBiriQOMExec);
            omSetStatBit(objP, OM_STAT_MODELPAUSE);
            objP->mdlId[0] = objP->mdlId[1] = MB_MODEL_NONE;
            mbObjBiriQCreate(mbPlayerObjIDGet(playerNo));
            mbObjBiriQColorSet(
                mbPlayerObjIDGet(playerNo), TRUE, 0.0f, color);
        }
        workP = omObjGetWork(objP, PLAYERBIRIQWORK);
        workP->playerNo = playerNo;
        workP->_unk0_3 = FALSE;
        workP->_unk0_1 = FALSE;
        workP->time = 0;
        workP->maxTime = 20;
        workP->effectF = TRUE;
        PlayerBiriQFlashSet(playerNo);
    } else {
        GwPlayer[playerNo].biriQF = FALSE;
    }
}

static void PlayerBiriQFlashSet(int playerNo)
{
    OMOBJ *objP = playerWork[playerNo].biriQObj;

    if (objP != NULL) {
        PLAYERBIRIQWORK *workP = omObjGetWork(objP, PLAYERBIRIQWORK);

        workP->flashF = TRUE;
    }
}

static void PlayerBiriQKill(int playerNo)
{
    OMOBJ *objP = playerWork[playerNo].biriQObj;

    if (objP != NULL) {
        PLAYERBIRIQWORK *workP = omObjGetWork(objP, PLAYERBIRIQWORK);

        workP->killF = TRUE;
        playerWork[playerNo].biriQObj = NULL;
        mbObjBiriQKill(mbPlayerObjIDGet(playerNo));
        if (objP->mdlId[0] >= 0) {
            mbParticleKill(objP->mdlId[0]);
            objP->mdlId[0] = -1;
        }
        if (objP->mdlId[1] >= 0) {
            mbParticleKill(objP->mdlId[1]);
            objP->mdlId[1] = -1;
        }
    }
}

static void PlayerBiriQEffectSet(int playerNo, BOOL effectF)
{
    OMOBJ *objP = playerWork[playerNo].biriQObj;

    if (objP) {
        PLAYERBIRIQWORK *workP = omObjGetWork(objP, PLAYERBIRIQWORK);

        workP->effectF = effectF;
    }
}

static void BiriQEffectCreate(OMOBJ *objP)
{
    PLAYERBIRIQWORK *workP = omObjGetWork(objP, PLAYERBIRIQWORK);
    MBPARTICLE *particleP;
    int effectCount[2];
    float radius;
    int particleNum;

    radius = GetBiriQEffectRadius(objP, workP->playerNo, effectCount);
    workP->_unk06 = effectCount[0];
    workP->_unk08 = effectCount[1];
    particleNum = 21.0f * (0.006666667f * radius);
    objP->mdlId[0] = mbParticleCreate(HuSprAnimRead(HuDataReadNum(
        mbBoardDataNumGet(DATANUM(DATA_board, 107)), HU_MEMNUM_OVL)),
        (s16)particleNum);
    objP->mdlId[1] = mbParticleCreate(HuSprAnimRead(HuDataReadNum(
        mbBoardDataNumGet(DATANUM(DATA_board, 108)), HU_MEMNUM_OVL)),
        (s16)particleNum);
    mbParticleHookSet(objP->mdlId[0], BiriQEffect1Hook);
    mbParticleHookSet(objP->mdlId[1], BiriQEffect2Hook);
    Hu3DModelLayerSet(objP->mdlId[0], 3);
    Hu3DModelLayerSet(objP->mdlId[1], 3);
    {
        HU3D_MODELID modelId = objP->mdlId[0];

        particleP = (MBPARTICLE *)Hu3DData[modelId].hookData;
        particleP->hookData = objP;
        particleP->mode = 0;
    }
    {
        HU3D_MODELID modelId = objP->mdlId[1];

        particleP = (MBPARTICLE *)Hu3DData[modelId].hookData;
        {
            HU3D_MODELID sourceModelId = objP->mdlId[0];

            particleP->hookData =
                (MBPARTICLE *)Hu3DData[sourceModelId].hookData;
        }
        particleP->mode = 0;
    }
}

static void BiriQEffect1Hook(
    HU3D_MODEL *modelP, MBPARTICLE *particleP, Mtx matrix)
{
    OMOBJ *objP = particleP->hookData;
    PLAYERBIRIQWORK *workP = omObjGetWork(objP, PLAYERBIRIQWORK);
    HSF_DATA *hsfP;
    HSF_OBJECT *objectP = NULL;
    MBPARTICLEDATA *dataP;
    Mtx modelMtx;
    HuVecF cameraPos;
    HuVecF pos;
    HuVecF dir;
    s16 *groupP;
    s16 *countP;
    s16 (*vertexNoP)[8];
    int groupNo;
    int randomNo;
    int i;
    BOOL firstF = FALSE;
    BOOL posF;
    float weight;
    float mag;

    if (particleP->mode == 0) {
        dataP = particleP->data;
        for (i = 0; i < particleP->num; i++, dataP++) {
            dataP->scale = 0.0f;
            dataP->color.a = 0;
            dataP->time = 0;
        }
        particleP->mode = 1;
        firstF = TRUE;
    }
    MTXInverse(Hu3DCameraMtx, modelMtx);
    cameraPos.x = modelMtx[0][3];
    cameraPos.y = modelMtx[1][3];
    cameraPos.z = modelMtx[2][3];
    hsfP = Hu3DData[mbPlayerModelIDGet(workP->playerNo)].hsf;
    objectP = &hsfP->object[workP->_unk06];
    Hu3DModelObjMtxGet(
        mbPlayerModelIDGet(workP->playerNo), objectP->name, modelMtx);
    dataP = particleP->data;
    for (i = 0; i < particleP->num; i++, dataP++) {
        if (dataP->time == 0) {
            if (workP->_unk0_3) {
                continue;
            }
            dataP->time++;
            groupP = objP->data;
            countP = groupP + 125;
            vertexNoP = (s16 (*)[8])(countP + 125);
            groupNo = groupP[mbRandMod(workP->_unk08)];
            randomNo = mbRandMod(countP[groupNo]);
            dataP->vertexNo = vertexNoP[groupNo][randomNo];
            dataP->color.r = mbRandMod(40) + 30;
            dataP->color.g = mbRandMod(40) + 90;
            dataP->color.b = mbRandMod(40) + 180;
            dataP->color.a = mbRandMod(58) + 130;
            dataP->vel.x = 30.0f + (60.0f * frandf());
            dataP->scale = 0.0f;
            dataP->rot.z = 360.0f * frandf();
            dataP->activeF = mbRandMod(12) + 12;
            dataP->animBank = mbRandMod(4);
            dataP->dispF = TRUE;
        }
        posF = TRUE;
        dataP->time++;
        weight = (float)dataP->time / dataP->activeF;
        dataP->scale = dataP->vel.x
            * (0.1f + (0.9f * mbSinDeg(180.0f * weight)));
        if (dataP->time & 1) {
            dataP->animBank = mbRandMod(4);
            if (mbRandMod(100) < 25) {
                dataP->rot.z += 180.0f;
            }
        }
        if (weight > 0.8f) {
            dataP->color.a *= 0.9f;
        }
        if (weight > 0.6f) {
            posF = FALSE;
        }
        if (dataP->time >= dataP->activeF) {
            dataP->time = 0;
            dataP->color.a = 0;
            dataP->scale = 0.0f;
            dataP->dispF = FALSE;
        }
        if (posF) {
            MTXMultVec(modelMtx,
                &((HuVecF *)objectP->mesh.vertex->data)[dataP->vertexNo],
                &pos);
            VECSubtract(&cameraPos, &pos, &dir);
            mag = VECMag(&dir);
            if (mag > 0.0f) {
                VECScale(&dir, &dir, 18.0f / mag);
            }
            VECAdd(&pos, &dir, &dataP->pos);
        }
    }
}

static void BiriQEffect2Hook(
    HU3D_MODEL *modelP, MBPARTICLE *particleP, Mtx matrix)
{
    MBPARTICLE *sourceP;
    MBPARTICLEDATA *dataP;
    int i;
    int alpha;

    if (particleP->mode == 0) {
        dataP = particleP->data;
        for (i = 0; i < particleP->num; i++, dataP++) {
            dataP->scale = 0.0f;
            dataP->color.a = 0;
            dataP->time = 0;
        }
        particleP->mode = 1;
        particleP->blendMode = MB_PARTICLE_BLEND_ADDCOL;
    }
    sourceP = particleP->hookData;
    memcpy(particleP->data, sourceP->data,
        particleP->num * sizeof(MBPARTICLEDATA));
    dataP = particleP->data;
    for (i = 0; i < particleP->num; i++, dataP++) {
        dataP->color.r = 255;
        dataP->color.g = 255;
        dataP->color.b = 255;
        alpha = 1.2f * dataP->color.a;
        if (alpha > 255) {
            alpha = 255;
        }
        dataP->color.a = alpha;
    }
}

static char *eyeMatNameTbl[CHARNO_MAX][2] = {
    { "eye1", "eye2" },
    { "eye1", "eye2" },
    { "mat14", "mat16" },
    { "eye1", "eye2" },
    { "Clswario_eye_l1_AUTO14", "Clswario_eye_l1_AUTO15" },
    { "m_donkey_eye4", "m_donkey_eye5" },
    { "mat65", "mat66" },
    { "Clswaluigi_eye_l1_AUTO1", "Clswaluigi_eye_l1_AUTO2" }
};

void mbPlayerEyeMatDarkSet(int playerNo, BOOL darkF)
{
    BOOL validF;
    HU3D_MODELID modelId = mbObjModelIDGet(mbPlayerObjIDGet(playerNo));
    HU3D_MODEL *modelP = &Hu3DData[modelId];
    HSF_DATA *hsf = modelP->hsf;
    HSF_MATERIAL *matP = hsf->material;
    HSF_MATERIAL *matCopy = playerWork[playerNo].matCopy;

    if (darkF) {
        char **name = &eyeMatNameTbl[GwPlayer[playerNo].charNo][0];
        int i;
        int j;

        for (i = 0; i < hsf->materialNum; i++, matP++, matCopy++) {
            validF = TRUE;
            for (j = 0; j < matP->attrNum; j++) {
                HSF_ATTRIBUTE *attrP = &hsf->attribute[matP->attr[j]];

                if (strcmp(name[0], attrP->bitmap->name) == 0
                    || strcmp(name[1], attrP->bitmap->name) == 0) {
                    validF = FALSE;
                }
            }
            if (validF) {
                if (darkF) {
                    matP->color[0] *= 0.0f;
                    matP->color[1] *= 0.0f;
                    matP->color[2] *= 0.0f;
                } else {
                    matP->color[0] = matCopy->color[0];
                    matP->color[1] = matCopy->color[1];
                    matP->color[2] = matCopy->color[2];
                }
            }
        }
    } else {
        memcpy(hsf->material, matCopy,
            hsf->materialNum * sizeof(HSF_MATERIAL));
    }
    DCStoreRange(hsf->material, hsf->materialNum * sizeof(HSF_MATERIAL));
}

void mbPlayerMatClone(int playerNo)
{
    HU3D_MODELID modelId = mbObjModelIDGet(mbPlayerObjIDGet(playerNo));
    HU3D_MODEL *modelP = &Hu3DData[modelId];
    HSF_DATA *hsf = modelP->hsf;
    int size = hsf->materialNum * sizeof(HSF_MATERIAL);
    HSF_MATERIAL *matP =
        HuMemDirectMallocNum(HEAP_HEAP, size, HU_MEMNUM_OVL);

    memcpy(matP, hsf->material, hsf->materialNum * sizeof(HSF_MATERIAL));
    playerWork[playerNo].matCopy = matP;
}

void mbPlayerSwap(int playerNo1, int playerNo2)
{
    GW_PLAYER player;
    MBPLAYERWORK work;
    GW_PLAYER_CONF playerConf;
    OMOBJ *colObj1;
    OMOBJ *colObj2;

    colObj1 = mbPlayerWorkGet(playerNo1)->colObj;
    colObj2 = mbPlayerWorkGet(playerNo2)->colObj;
    playerConf = GwPlayerConf[playerNo1];
    GwPlayerConf[playerNo1] = GwPlayerConf[playerNo2];
    GwPlayerConf[playerNo2] = playerConf;
    player = GwPlayer[playerNo1];
    GwPlayer[playerNo1] = GwPlayer[playerNo2];
    GwPlayer[playerNo2] = player;
    memcpy(&work, mbPlayerWorkGet(playerNo1), sizeof(MBPLAYERWORK));
    memcpy(mbPlayerWorkGet(playerNo1), mbPlayerWorkGet(playerNo2),
        sizeof(MBPLAYERWORK));
    memcpy(mbPlayerWorkGet(playerNo2), &work, sizeof(MBPLAYERWORK));
    mbPlayerWorkGet(playerNo1)->colObj = colObj1;
    mbPlayerWorkGet(playerNo2)->colObj = colObj2;
    GwPlayer[playerNo1].padNo = GwPlayerConf[playerNo1].padNo;
    GwPlayerConf[playerNo1].padNo = GwPlayerConf[playerNo1].padNo;
    GwPlayer[playerNo2].padNo = GwPlayerConf[playerNo2].padNo;
    GwPlayerConf[playerNo2].padNo = GwPlayerConf[playerNo2].padNo;
}

u32 mbPlayerNameMesGet(int playerNo)
{
    u32 nameTbl[CHARNO_MAX] = {
        MESS_CHARANAME_MARIO,
        MESS_CHARANAME_LUIGI,
        MESS_CHARANAME_PEACH,
        MESS_CHARANAME_YOSHI,
        MESS_CHARANAME_WARIO,
        MESS_CHARANAME_DAISY,
        MESS_CHARANAME_WALUIGI,
        MESS_CHARANAME_KINOPIO,
        MESS_CHARANAME_TERESA,
        MESS_CHARANAME_MINIKOOPA,
        MESS_CHARANAME_KINOPICO,
        MESS_CHARANAME_MINIKOOPAR,
        MESS_CHARANAME_MINIKOOPAG,
        MESS_CHARANAME_MINIKOOPAB
    };

    return nameTbl[GwPlayer[playerNo].charNo];
}

char *mbPlayerNameGet(int playerNo)
{
    char *nameTbl[CHARNO_MAX] = {
        "Mario",
        "Luigi",
        "Peach",
        "Yoshi",
        "Wario",
        "Daisy",
        "Waluigi",
        "Kinopio",
        "Teresa",
        "Mini Koopa",
        "Kinopiko"
    };

    return nameTbl[GwPlayer[playerNo].charNo];
}

static s8 tagIdTbl[110] = {
    -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
    -1, -1, 10, 11, 12, 13, 14, 15, 16, 17, 18,
    -1, -1, -1, 19, 20, 21, 22, 23, 24, 25, 26,
    -1, -1, -1, -1, 27, 28, 29, 30, 31, 32, 33,
    -1, -1, -1, -1, -1, 34, 35, 36, 37, 38, 39,
    -1, -1, -1, -1, -1, -1, 40, 41, 42, 43, 44,
    -1, -1, -1, -1, -1, -1, -1, 45, 46, 47, 48,
    -1, -1, -1, -1, -1, -1, -1, -1, 49, 50, 51,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, 52, 53,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 54
};

u32 mbPlayerTagNameMesGet(int teamNo)
{
    int charNo1;
    int charNo2;
    int temp;
    int tagId;

    charNo1 = GwPlayer[mbPlayerTeamFindPlayer(teamNo, 0)].charNo;
    charNo2 = GwPlayer[mbPlayerTeamFindPlayer(teamNo, 1)].charNo;
    if (charNo1 > charNo2) {
        temp = charNo1;
        charNo1 = charNo2;
        charNo2 = temp;
    }
    tagId = tagIdTbl[(charNo1 * 11) + charNo2];
    OSReport("%d:%d->%d\n", charNo1, charNo2, tagId);
    if (tagId == -1) {
        return 0x30037;
    }
    return 0x30000 + tagId;
}

void mbPlayerAmbSet(int playerNo, float ambR, float ambG, float ambB)
{
    mbObjAmbSet(mbPlayerObjIDGet(playerNo), ambR, ambG, ambB);
}

MBMODELID mbPlayerObjIDGet(int playerNo)
{
    return playerWork[playerNo].objId;
}

HU3D_MODELID mbPlayerModelIDGet(int playerNo)
{
    return mbObjModelIDGet(playerWork[playerNo].objId);
}

BOOL mbPlayerAllComCheck(void)
{
    int i;

    for (i = 0; i < GW_PLAYER_MAX; i++) {
        if (!GwPlayer[i].comF) {
            return FALSE;
        }
    }
    return TRUE;
}

int mbPlayerTeamFind(int playerNo)
{
    int i;

    for (i = 0; i < GW_PLAYER_MAX; i++) {
        if (i == playerNo) {
            continue;
        }
        if (mbPlayerGrpGet(playerNo) == mbPlayerGrpGet(i)) {
            break;
        }
    }
    return i;
}

int mbPlayerTeamFindOther(int playerNo)
{
    int i;

    for (i = 0; i < GW_PLAYER_MAX; i++) {
        if (i == playerNo) {
            continue;
        }
        if (mbPlayerGrpGet(playerNo) != mbPlayerGrpGet(i)) {
            break;
        }
    }
    return i;
}

int mbPlayerTeamFindPlayer(int teamNo, int memberNo)
{
    int i;
    int no;

    no = 0;
    for (i = 0; i < GW_PLAYER_MAX; i++) {
        if (teamNo != mbPlayerGrpGet(i)) {
            continue;
        }
        if (no == memberNo) {
            return i;
        }
        no++;
    }
    return -1;
}

int mbPlayerTeamFindOpp(int playerNo)
{
    if (!GWTeamFGet()) {
        return playerNo;
    }
    return mbPlayerTeamFind(playerNo);
}

BOOL mbPlayerTeamCheckSame(int playerNo1, int playerNo2)
{
    BOOL sameF;

    sameF = FALSE;
    if (GWTeamFGet()) {
        if (mbPlayerGrpGet(playerNo1) == mbPlayerGrpGet(playerNo2)) {
            sameF = TRUE;
        }
    } else if (playerNo1 == playerNo2) {
        sameF = TRUE;
    }
    return sameF;
}

int mbPlayerTeamGet(int playerNo)
{
    if (!GWTeamFGet()) {
        return playerNo;
    }
    return mbPlayerGrpGet(playerNo);
}

float mbPlayerWalkSpeedGet(void)
{
    return 20;
}

void mbPlayerLayerSet(int playerNo, int layer)
{
    mbObjLayerSet(mbPlayerObjIDGet(playerNo), layer);
}

void mbPlayerCameraSet(int playerNo, u16 cameraBit)
{
    mbObjCameraSet(mbPlayerObjIDGet(playerNo), cameraBit);
}

void mbPlayerCullRadiusSet(int playerNo, float radius)
{
    mbObjCullRadiusSet(mbPlayerObjIDGet(playerNo), radius);
}

void mbPlayerStubValSet(int playerNo, BOOL value)
{
}

void mbPlayerPosReset(int playerNo)
{
    HuVecF pos;

    mbMasuPosGet(GwPlayer[playerNo].masuId, &pos);
    mbPlayerPosSetV(playerNo, &pos);
    PlayerColCornerSnap(playerNo, GwPlayer[playerNo].masuId, 0);
}

void mbPlayerPosResetAll(void)
{
    int i;
    int j;
    int cornerNo;
    s16 masuId;
    s8 orderNo;
    HuVecF pos;

    for (i = 0; i < GW_PLAYER_MAX; i++) {
        orderNo = GwPlayer[i].orderNo;
        masuId = GwPlayer[i].masuId;
        cornerNo = 0;
        for (j = 0; j < GW_PLAYER_MAX; j++) {
            if (i != j && masuId == GwPlayer[j].masuId
                && orderNo > GwPlayer[j].orderNo) {
                cornerNo++;
            }
        }
        mbPlayerMasuCornerSet(i, cornerNo);
        if (cornerNo == 0) {
            mbMasuPosGet(masuId, &pos);
        } else {
            mbMasuCornerRotPosGet(masuId, cornerNo - 1, &pos);
        }
        mbPlayerPosSetV(i, &pos);
        PlayerColCornerSnap(i, masuId, cornerNo);
    }
}

void mbPlayerMtxSet(int playerNo, Mtx *matrix)
{
    mbObjMtxSet(mbPlayerObjIDGet(playerNo), matrix);
}

void mbPlayerMtxGet(int playerNo, Mtx *matrix)
{
    mbObjMtxGet(mbPlayerObjIDGet(playerNo), matrix);
}

void mbPlayerPosSetV(int playerNo, const HuVecF *pos)
{
    mbObjPosSetV(mbPlayerObjIDGet(playerNo), pos);
}

void mbPlayerPosSet(int playerNo, float posX, float posY, float posZ)
{
    mbObjPosSet(mbPlayerObjIDGet(playerNo), posX, posY, posZ);
}

void mbPlayerPosGet(int playerNo, HuVecF *pos)
{
    mbObjPosGet(mbPlayerObjIDGet(playerNo), pos);
}

void mbPlayerRotSetV(int playerNo, const HuVecF *rot)
{
    mbObjRotSetV(mbPlayerObjIDGet(playerNo), rot);
}

void mbPlayerRotSet(int playerNo, float rotX, float rotY, float rotZ)
{
    mbObjRotSet(mbPlayerObjIDGet(playerNo), rotX, rotY, rotZ);
}

void mbPlayerRotGet(int playerNo, HuVecF *rot)
{
    mbObjRotGet(mbPlayerObjIDGet(playerNo), rot);
}

void mbPlayerRotYSet(int playerNo, float rotY)
{
    rotY = fmod(rotY, 360);
    if (rotY < 0) {
        rotY += 360;
    }
    mbObjRotYSet(mbPlayerObjIDGet(playerNo), rotY);
}

float mbPlayerRotYGet(int playerNo)
{
    return mbObjRotYGet(mbPlayerObjIDGet(playerNo));
}

void mbPlayerScaleSetV(int playerNo, const HuVecF *scale)
{
    mbObjScaleSetV(mbPlayerObjIDGet(playerNo), scale);
}

void mbPlayerScaleSet(int playerNo, float scaleX, float scaleY, float scaleZ)
{
    mbObjScaleSet(mbPlayerObjIDGet(playerNo), scaleX, scaleY, scaleZ);
}

void mbPlayerScaleGet(int playerNo, HuVecF *scale)
{
    mbObjScaleGet(mbPlayerObjIDGet(playerNo), scale);
}

void mbPlayerMotionSet(int playerNo, int motNo, u32 attr)
{
    GW_PLAYER *playerP;

    playerP = GWPlayerGet(playerNo);
    if (motNo == playerWork[playerNo].motNo) {
        return;
    }
    mbObjMotionSet(mbPlayerObjIDGet(playerNo), motNo, attr);
    playerWork[playerNo].motNo = motNo;
    if (motNo == 10) {
        mbObjOffsetSet(mbPlayerObjIDGet(playerNo), 0, 4, 0);
    } else {
        mbObjOffsetSet(mbPlayerObjIDGet(playerNo), 0, 0, 0);
    }
}

int mbPlayerMotionGet(int playerNo)
{
    return playerWork[playerNo].motNo;
}

void mbPlayerMotionShiftSet(int playerNo, int motNo, float start, float end,
    u32 attr)
{
    GW_PLAYER *playerP;

    playerP = GWPlayerGet(playerNo);
    if (motNo == playerWork[playerNo].motNo) {
        return;
    }
    mbObjMotionShiftSet(mbPlayerObjIDGet(playerNo), motNo, start, end, attr);
    playerWork[playerNo].motNo = motNo;
    if (motNo == 10) {
        mbObjOffsetSet(mbPlayerObjIDGet(playerNo), 0, 4, 0);
    } else {
        mbObjOffsetSet(mbPlayerObjIDGet(playerNo), 0, 0, 0);
    }
}

int mbPlayerMotionCreate(int playerNo, int dataNum)
{
    return mbObjMotionCreate(mbPlayerObjIDGet(playerNo), dataNum);
}

int mbPlayerMotionKill(int playerNo, int motNo)
{
    mbObjMotionKill(mbPlayerObjIDGet(playerNo), motNo);
    return TRUE;
}

void mbPlayerMotionSpeedSet(int playerNo, float speed)
{
    mbObjMotionSpeedSet(mbPlayerObjIDGet(playerNo), speed);
}

void mbPlayerMotionTimeSet(int playerNo, float time)
{
    mbObjMotionTimeSet(mbPlayerObjIDGet(playerNo), time);
}

float mbPlayerMotionTimeGet(int playerNo)
{
    return mbObjMotionTimeGet(mbPlayerObjIDGet(playerNo));
}

float mbPlayerMotionMaxTimeGet(int playerNo)
{
    return mbObjMotionMaxTimeGet(mbPlayerObjIDGet(playerNo));
}

void mbPlayerMotionStartEndSet(int playerNo, float start, float end)
{
    mbObjMotionStartEndSet(mbPlayerObjIDGet(playerNo), start, end);
}

void mbPlayerAttrSet(int playerNo, u32 attr)
{
    MBMODELID modelId = mbPlayerObjIDGet(playerNo);

    mbObjAttrSet(modelId, attr);
}

void mbPlayerAttrReset(int playerNo, u32 attr)
{
    MBMODELID modelId = mbPlayerObjIDGet(playerNo);

    mbObjAttrReset(modelId, attr);
}

void mbPlayerMotionVoiceOnSet(int playerNo, int motNo, BOOL voiceOnF)
{
    mbObjMotionVoiceOnSet(mbPlayerObjIDGet(playerNo), motNo, voiceOnF);
}

BOOL mbPlayerMotionEndCheck(int playerNo)
{
    int modelId;
    BOOL endF;

    modelId = mbPlayerObjIDGet(playerNo);
    return mbObjMotionShiftIDGet(modelId) < 0 && mbObjMotionEndCheck(modelId);
}

BOOL mbPlayerMotionEndCheckAll(void)
{
    int i;

    for (i = 0; i < GW_PLAYER_MAX; i++) {
        if (!mbPlayerMotionEndCheck(i)) {
            return FALSE;
        }
    }
    return TRUE;
}

void mbPlayerMotionEndWait(int playerNo)
{
    while (!mbPlayerMotionEndCheck(playerNo)) {
        HuPrcVSleep();
    }
}

void mbPlayerMotIdleSet(int playerNo)
{
    mbPlayerMotionShiftSet(playerNo, 1, 0, 8, HU3D_MOTATTR_LOOP);
}

void mbPlayerCoinSet(int playerNo, int coinNum)
{
    if (!GWTeamFGet()) {
        GWPlayerCoinSet(playerNo, coinNum);
    } else {
        GWPlayerCoinSet(
            mbPlayerTeamFindPlayer(mbPlayerGrpGet(playerNo), 0), coinNum);
    }
}

int mbPlayerCoinGet(int playerNo)
{
    if (!GWTeamFGet()) {
        return GWPlayerCoinGet(playerNo);
    } else {
        return GWPlayerCoinGet(
            mbPlayerTeamFindPlayer(mbPlayerGrpGet(playerNo), 0));
    }
}

void mbPlayerCoinAdd(int playerNo, int coinNum)
{
    GW_PLAYER *playerP;

    if (GWTeamFGet()) {
        playerNo = mbPlayerTeamFindPlayer(mbPlayerGrpGet(playerNo), 0);
    }
    playerP = GWPlayerGet(playerNo);
    if (coinNum > 0 && playerP->coinTotal < 999) {
        playerP->coinTotal += coinNum;
        if (playerP->coinTotal > 999) {
            playerP->coinTotal = 999;
        }
    }
    mbPlayerCoinSet(playerNo, coinNum + mbPlayerCoinGet(playerNo));
}

void mbPlayerTeamCoinSet(int teamNo, int coinNum)
{
    GWPlayerCoinSet(mbPlayerTeamFindPlayer(teamNo, 0), coinNum);
}

s16 mbPlayerTeamCoinGet(int teamNo)
{
    return GWPlayerCoinGet(mbPlayerTeamFindPlayer(teamNo, 0));
}

int mbPlayerMaxCoinGet(void)
{
    int maxCoin;
    int i;

    maxCoin = 0;
    for (i = 0; i < GW_PLAYER_MAX; i++) {
        if (mbPlayerCoinGet(i) >= maxCoin) {
            maxCoin = mbPlayerCoinGet(i);
        }
    }
    return maxCoin;
}

void mbPlayerStarSet(int playerNo, int starNum)
{
    if (!GWTeamFGet()) {
        GWPlayerStarSet(playerNo, starNum);
    } else {
        GWPlayerStarSet(
            mbPlayerTeamFindPlayer(mbPlayerGrpGet(playerNo), 0), starNum);
    }
}

int mbPlayerStarGet(int playerNo)
{
    if (!GWTeamFGet()) {
        return GWPlayerStarGet(playerNo);
    } else {
        return GWPlayerStarGet(
            mbPlayerTeamFindPlayer(mbPlayerGrpGet(playerNo), 0));
    }
}

void mbPlayerStarAdd(int playerNo, int starNum)
{
    int star;

    mbAudFXPlay(8);
    star = mbPlayerStarGet(playerNo) + starNum;
    if (star < 0) {
        star = 0;
    }
    mbPlayerStarSet(playerNo, star);
}

void mbPlayerGrpStarSet(int teamNo, int starNum)
{
    GWPlayerStarSet(mbPlayerTeamFindPlayer(teamNo, 0), starNum);
}

s16 mbPlayerGrpStarGet(int teamNo)
{
    return GWPlayerStarGet(mbPlayerTeamFindPlayer(teamNo, 0));
}

int mbPlayerBestPathGet(void)
{
    int i;
    int bestPlayer;
    int len;
    int minLen;

    minLen = 9999;
    bestPlayer = -1;
    for (i = 0; i < GW_PLAYER_MAX; i++) {
        len = mbMasuFind_TypeStepGet2(GwPlayer[i].masuId, 7, TRUE, TRUE);
        if (len < minLen) {
            minLen = len;
            bestPlayer = i;
        }
    }
    if (bestPlayer < 0) {
        return mbRandMod(GW_PLAYER_MAX);
    } else {
        return bestPlayer;
    }
}

int mbPlayerRankGet(int playerNo)
{
    int score[GW_PLAYER_MAX];
    int i;
    int rank;

    for (i = 0; i < GW_PLAYER_MAX; i++) {
        score[i] = mbPlayerCoinGet(i) | (mbPlayerStarGet(i) * 1024);
    }
    rank = 0;
    for (i = 0; i < GW_PLAYER_MAX; i++) {
        if (i != playerNo && score[playerNo] < score[i]) {
            rank++;
        }
    }
    return rank;
}

s16 mbPlayerTeamRankGet(int teamNo)
{
    int score[2];
    int i;
    int rank;

    for (i = 0; i < 2; i++) {
        score[i] = mbPlayerTeamCoinGet(i) | (mbPlayerGrpStarGet(i) * 2048);
    }
    rank = 0;
    for (i = 0; i < 2; i++) {
        if (i != teamNo && score[teamNo] < score[i]) {
            rank++;
        }
    }
    return rank;
}

void mbPlayerCapsuleUseSet(int capsuleNo)
{
    GwPlayer[GwSystem.turnPlayerNo].capsuleUse = capsuleNo;
}

int mbPlayerCapsuleUseGet(void)
{
    return GwPlayer[GwSystem.turnPlayerNo].capsuleUse;
}

int mbPlayerCapsuleMaxGet(void)
{
    return (GWTeamFGet() == FALSE) ? 3 : 5;
}

static s8 *PlayerCapsulePtrGet(int playerNo, int index)
{
    if (!GWTeamFGet()) {
        return &GwPlayer[playerNo].capsule[index];
    } else {
        int memberNo = (index < 3) ? 0 : 1;
        int teamNo = mbPlayerGrpGet(playerNo);
        int no = -1;
        int j;

        for (j = 0; j < GW_PLAYER_MAX; j++) {
            if (teamNo == mbPlayerGrpGet(j)) {
                no++;
                if (no == memberNo) {
                    break;
                }
            }
        }
        if (j < GW_PLAYER_MAX) {
            return &GwPlayer[j].capsule[index - (memberNo * 3)];
        }
    }
}

int mbPlayerCapsuleAdd(int playerNo, int capsuleNo)
{
    GW_PLAYER *playerP = GWPlayerGet(playerNo);
    int max = mbPlayerCapsuleMaxGet();
    int i;

    for (i = 0; i < max; i++) {
        s8 *capsuleP;

        if (mbPlayerCapsuleGet(playerNo, i) != -1) {
            continue;
        }
        *PlayerCapsulePtrGet(playerNo, i) = capsuleNo;
        return i;
    }
    return -1;
}

int mbPlayerCapsuleRemove(int playerNo, int index)
{
    int capsuleNo = mbPlayerCapsuleGet(playerNo, index);
    GW_PLAYER *playerP = GWPlayerGet(playerNo);
    int max;
    int i;

    if (capsuleNo == -1) {
        return capsuleNo;
    }
    max = mbPlayerCapsuleMaxGet();
    for (i = index; i < max - 1; i++) {
        *PlayerCapsulePtrGet(playerNo, i) =
            *PlayerCapsulePtrGet(playerNo, i + 1);
    }
    for (; i < max; i++) {
        *PlayerCapsulePtrGet(playerNo, i) = -1;
    }
    return capsuleNo;
}

int mbPlayerCapsuleFind(int playerNo, int capsuleNo)
{
    int max = mbPlayerCapsuleMaxGet();
    int i;

    for (i = 0; i < max; i++) {
        if (capsuleNo == mbPlayerCapsuleGet(playerNo, i)) {
            return i;
        }
    }
    return -1;
}

s8 mbPlayerCapsuleGet(int playerNo, int index)
{
    return *PlayerCapsulePtrGet(playerNo, index);
}

s8 mbPlayerTeamCapsuleGet(int teamNo, int index)
{
    return mbPlayerCapsuleGet(mbPlayerTeamFindPlayer(teamNo, 0), index);
}

static int PlayerCountCapsules(int playerNo)
{
    int i;

    for (i = 0; i < 3; i++) {
        if (GwPlayer[playerNo].capsule[i] == -1) {
            break;
        }
    }
    return i;
}

int mbPlayerCapsuleNumGet(int playerNo)
{
    int num = PlayerCountCapsules(playerNo);

    if (GWTeamFGet()) {
        int otherPlayer = mbPlayerTeamFind(playerNo);

        num += PlayerCountCapsules(otherPlayer);
        if (num > 5) {
            num = 5;
        }
    }
    return num;
}

int mbPlayerTeamCapsuleNumGet(int teamNo)
{
    return mbPlayerCapsuleNumGet(mbPlayerTeamFindPlayer(teamNo, 0));
}

BOOL mbPlayerKettouCheck(int playerNo, s16 masuId)
{
    int i;

    for (i = 0; i < GW_PLAYER_MAX; i++) {
        if (playerNo != i && masuId == GwPlayer[i].masuId) {
            return TRUE;
        }
    }
    return FALSE;
}

void mbPlayerWinLoseVoicePlay(int playerNo, int motNo, int seId)
{
    MBOBJMODEL *objP = mbObjGet(mbPlayerObjIDGet(playerNo));
    u8 charNo = GwPlayer[playerNo].charNo;

    CharWinLoseVoicePlay(charNo, objP->motId[motNo], seId);
}

int mbPlayerVoicePanPlay(int playerNo, s16 seId)
{
    return mbObjSePlay(mbPlayerObjIDGet(playerNo), seId);
}

int mbPlayerVoicePlay(int playerNo, s16 seId)
{
    HuVecF pos;
    u8 pan;

    mbPlayerPosGet(playerNo, &pos);
    pan = mbAudFXPosPanGet(&pos);
    return CharFXPlayVolPan(
        GwPlayer[playerNo].charNo, seId, MSM_VOL_MAX, pan);
}

void mbPlayerDispSet(int playerNo, BOOL dispF)
{
    if (GwPlayer[playerNo].masuId == 0) {
        dispF = FALSE;
    }
    mbObjDispSet(mbPlayerObjIDGet(playerNo), dispF);
}

BOOL mbPlayerDispGet(int playerNo)
{
    return mbObjDispGet(mbPlayerObjIDGet(playerNo));
}

GXColor mbPlayerColorGet(int playerNo)
{
    GXColor color[] = {
        { 227, 67, 67, 255 },
        { 68, 67, 227, 255 },
        { 241, 158, 220, 255 },
        { 67, 228, 68, 255 },
        { 138, 60, 180, 255 },
        { 227, 228, 68, 255 },
        { 192, 192, 192, 255 },
        { 227, 227, 227, 255 },
        { 40, 227, 227, 255 },
        { 227, 139, 40, 255 },
        { 180, 40, 40, 255 },
        { 40, 180, 40, 255 },
        { 40, 40, 180, 255 }
    };

    return color[GwPlayer[playerNo].charNo];
}

void mbPlayerBlackoutSet(BOOL value)
{
    blackoutF = value;
}

BOOL mbPlayerBlackoutGet(void)
{
    return blackoutF;
}

void mbPlayerMasuCornerSet(int playerNo, s8 cornerNo)
{
    playerWork[playerNo].masuCorner = cornerNo;
}

s8 mbPlayerMasuCornerGet(int playerNo)
{
    return playerWork[playerNo].masuCorner;
}

static void MasuCoinExec(int playerNo, int coinNum)
{
    HuVecF pos;
    BOOL doneF;
    s8 dispId;

    if (coinNum < 0) {
        omVibrate(playerNo, 20, 4, 4);
    }
    mbPlayerRotateStart(playerNo, 0, 15);
    if (GwSystem.last5Effect == 1) {
        coinNum *= 3;
    }
    mbPlayerPosGet(playerNo, &pos);
    pos.y += 250.0f;
    if (coinNum >= 0) {
        mbAudFXPlay(0x44D);
    } else {
        mbAudFXPlay(0x44E);
    }
    dispId = mbCoinDispMasuCreate(&pos, coinNum, FALSE);
    while (!mbPlayerRotateCheck(playerNo)) {
        HuPrcVSleep();
    }
    if (coinNum >= 0) {
        mbPlayerWinLoseVoicePlay(playerNo, 12, CHARVOICEID(6));
        mbPlayerMotionShiftSet(
            playerNo, 12, 0.0f, 4.0f, HU3D_ATTR_NONE);
    } else {
        mbPlayerWinLoseVoicePlay(playerNo, 13, CHARVOICEID(12));
        mbPlayerMotionShiftSet(
            playerNo, 13, 0.0f, 4.0f, HU3D_ATTR_NONE);
    }
    mbCoinAddExec(playerNo, coinNum);
    mbCameraMoveWait();
    for (doneF = FALSE;
         !mbCoinDispKillCheck(dispId) || !doneF;) {
        if (mbPlayerMotionEndCheck(playerNo) && !doneF) {
            mbPlayerMotIdleSet(playerNo);
            doneF = TRUE;
        }
        HuPrcVSleep();
    }
}

void mbPlayerPlusMasuExec(int playerNo)
{
    mbCameraMoveOnSet(TRUE);
    mbCameraPlayerViewSet(playerNo, MB_CAMERA_VIEW_ZOOMIN);
    MasuCoinExec(playerNo, 3);
}

void mbPlayerCapCoinMasuExec(int playerNo)
{
    mbCameraMoveOnSet(TRUE);
    mbCameraPlayerViewSet(playerNo, MB_CAMERA_VIEW_ZOOMIN);
    MasuCoinExec(playerNo, 5);
}

void mbPlayerMinusMasuExec(int playerNo)
{
    mbCameraMoveOnSet(TRUE);
    mbCameraPlayerViewSet(playerNo, MB_CAMERA_VIEW_ZOOMIN);
    MasuCoinExec(playerNo, -3);
}
