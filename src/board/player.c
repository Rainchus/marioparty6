/* player.o has no weak sqrtf constants in .sdata2. */
#define _MATH_H
#include "dolphin/math.h"

#include "game/board/audio.h"
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

static MBPLAYERWORK playerWork[GW_PLAYER_MAX];
static BOOL blackoutF;
static void (*turnInitHook)(int playerNo);
static void (*turnCloseHook)(int playerNo);

static void PlayerMetalKill(int playerNo);
static void PlayerBiriQKill(int playerNo);
void mbDiceNumKill(int playerNo);

void mbPlayerClose(void)
{
    MBPLAYERWORK *workP;
    int i;

    workP = &playerWork[0];
    for (i = 0; i < GW_PLAYER_MAX; i++, workP++) {
        GW_PLAYER *playerP = &GwPlayer[i];

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
    HSF_MATERIAL *matP = HuMemDirectMallocNum(
        HEAP_HEAP, hsf->materialNum * sizeof(HSF_MATERIAL), HU_MEMNUM_OVL);

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
    return mbObjModelIDGet(mbPlayerObjIDGet(playerNo));
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
    GW_PLAYER *playerP = &GwPlayer[playerNo];

    (void)(motNo == -1);
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
    GW_PLAYER *playerP = &GwPlayer[playerNo];

    (void)(motNo == -1);
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
    mbObjAttrSet(mbPlayerObjIDGet(playerNo), attr);
}

void mbPlayerAttrReset(int playerNo, u32 attr)
{
    mbObjAttrReset(mbPlayerObjIDGet(playerNo), attr);
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
