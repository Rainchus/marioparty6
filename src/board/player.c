/* player.o has no weak sqrtf constants in .sdata2. */
#define _MATH_H
#include "dolphin/math.h"
#include "dolphin/os.h"

#include "game/board/audio.h"
#include "game/board/camera.h"
#include "game/board/effect.h"
#include "game/board/main.h"
#include "game/board/masu.h"
#include "game/board/object.h"
#include "game/board/player.h"
#include "game/audio.h"
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

static GXColor metalDefaultColor[2] = {
    { 128, 190, 140, 255 },
    { 100, 50, 130, 255 }
};

static void PlayerColKill(int playerNo);
static void PlayerMetalKill(int playerNo);
static void PlayerBiriQKill(int playerNo);
static void PlayerMove(void);
static void PlayerTurn(int playerNo);
static BOOL PlayerViewSet(
    int playerNo, BOOL intrF, BOOL waitF, BOOL carF);
static void MasuCoinExec(int playerNo, int coinNum);
void mbDiceNumKill(int playerNo);
void mbDiceObjHit(int playerNo);
void mbObjMetalKill(MBMODELID modelId);
void mbObjBiriQKill(MBMODELID modelId);
BOOL mbWipeSpecialStatGet(void);
void mbWipeFadeIn(void);

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

static void PlayerMoveDestroy(void)
{
    MBPLAYERWORK *workP = HuPrcCurrentGet()->property;

    workP->moveProc = NULL;
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

void mbMoveNumCreate(int playerNo, BOOL carF)
{
    mbMoveNumCreateColor(playerNo, carF, 0);
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

typedef struct PlayerColWork {
    u8 killF : 1;
    u8 _unk0_1 : 1;
    u8 snapF : 1;
    u8 restF : 1;
    u8 _unk0_4 : 2;
    u8 state : 2;
} PLAYERCOLWORK;

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

typedef struct PlayerMetalWork {
    u8 killF : 1;
    u8 _unk0_1 : 1;
    u8 _unk0_2 : 1;
    u8 effectF : 1;
} PLAYERMETALWORK;

typedef struct PlayerBiriQWork {
    u8 killF : 1;
    u8 _unk0_1 : 1;
    u8 flashF : 1;
    u8 _unk0_3 : 1;
    u8 effectF : 1;
} PLAYERBIRIQWORK;

static void PlayerBiriQEffectSet(int playerNo, BOOL effectF);

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
        PlayerBiriQEffectSet(playerNo, effectF);
    }
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
    int i;

    if (!GWTeamFGet()) {
        return playerNo;
    }
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
    playerP = &GwPlayer[playerNo];
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
    mbAudFXPlay(8);
    starNum += mbPlayerStarGet(playerNo);
    if (starNum < 0) {
        starNum = 0;
    }
    mbPlayerStarSet(playerNo, starNum);
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
    GW_PLAYER *playerP = &GwPlayer[playerNo];
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
    GW_PLAYER *playerP = &GwPlayer[playerNo];
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
