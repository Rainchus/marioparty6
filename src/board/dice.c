#include "game/board/audio.h"
#include "game/board/camera.h"
#include "game/board/coin.h"
#include "game/board/effect.h"
#include "game/board/main.h"
#include "game/board/object.h"
#include "game/board/pause.h"
#include "game/board/player.h"
#include "game/board/window.h"

#include "game/flag.h"
#include "game/frand.h"
#include "game/hu3d.h"
#include "game/memory.h"
#include "game/pad.h"
#include "game/sprite.h"

#include "humath.h"
#include "stdio.h"
#include "string.h"

#define DICE_PLAYERNO_NULL GW_PLAYER_MAX
#define DICE_MAX DICE_PLAYERNO_NULL+1
#define DICE_VALUENUM_MAX 11
#define DICE_EFF_PUFF_MAX 50
#define DICE_EFF_TRI_MAX 20

#define DICETYPE_EVENT 6
#define DICETYPE_PATAPATA_COIN 7
#define DICETYPE_CHANCE_TRADE 8
#define DICETYPE_CHANCE_CHAR 9
#define DICETYPE_BLOCK 10

#define DICE_COLOR_GREEN 0

typedef void (*DICEHITHOOK)(int result);
typedef u16 (*DICEPADBTNHOOK)(int playerNo);
typedef void (*DICEMOTHOOK)(int playerNo);

typedef struct DiceEff_s {
    HuVecF rot;
    float radius;
    HuVecF scale;
} DICE_EFF;

typedef struct DiceWork_s {
    int playerNo;
    int diceType;
    int max;
    int no;
    s8 valueTbl[DICE_VALUENUM_MAX];
    int valueNum;
    int tutorialVal[3];
    BOOL padWinF;
    s8 result[3];
    HuVecF pos;
    int color;
    OMOBJ *obj;
    DICE_EFF *eff;
    u32 _unk4C;
    void *dlBuf;
    u32 dlSize;
    ANIMDATA *animEffPuff;
    ANIMDATA *animEffTri;
    s8 puffTime;
    s8 triTime;
} DICE_WORK;

typedef struct DiceObjWork_s {
    u8 killF : 1;
    u8 lockF : 1;
    u8 fadeF : 1;
    u8 mode : 3;
    u8 no : 2;
    s16 lockTime;
    s16 time;
    s16 maxTime;
    s16 valueNo;
    int diceSeNo;
} DICE_OBJ_WORK;

typedef struct DiceFadeWork_s {
    u8 killF : 1;
    u8 fadeF : 1;
    s16 playerNo;
    u16 time;
    u16 angle;
} DICE_FADE_WORK;

typedef struct DiceNumWork_s {
    u8 killF : 1;
    u8 rotF : 1;
    u8 updateF : 1;
    u8 modelNo : 2;
    u8 bendF : 1;
    u8 playerNo : 2;
    s8 value;
    u8 color;
    s16 bendMode;
    s16 time;
    s16 maxTime;
} DICE_NUM_WORK;

typedef struct DiceSNpcNumWork_s {
    u8 killF : 1;
    u8 dispF : 1;
    u8 _unk2 : 1;
    u8 value;
} DICE_SNPC_NUM_WORK;

typedef struct DiceNumVtx_s {
    HuVecF pos;
    float weight;
} DICE_NUM_VTX;

static const int diceObjFileTbl[] = {
    0x00050017, 0x00050018, 0x00050019, 0x00050022, 0x0005001C,
    0x00050020, 0x0005001F, 0x00050017, 0x0005001F, 0x00050017,
    0x00030003, 0x0005001D, 0x0005001E, 0x00050021, 0x0005001A,
    0x00050017, 0x00050017, 0x00050017, 0x00050018, 0x00050017,
    0x00050017
};

static const u8 diceFadeFlagTbl[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0
};

static const int numberFileTbl[] = {
    0x0005000B, 0x0005000C, 0x0005000D, 0x0005000E, 0x0005000F,
    0x00050010, 0x00050011, 0x00050012, 0x00050013, 0x00050014
};

static HUPROCESS *diceProc[DICE_MAX];
static OMOBJ *diceFadeOMObj[DICE_MAX];
static int diceResult[DICE_MAX];

static DICEHITHOOK diceHitHook[DICE_MAX] = {};
static DICEPADBTNHOOK dicePadBtnHook[DICE_MAX] = {};
static DICEMOTHOOK diceMotHook[DICE_MAX] = {};
static OMOBJ *diceNumOMObj[DICE_MAX][3] = {};

static HuVecF dice1PosTbl[] = {
    0, 300, 0
};

static HuVecF dice2PosTbl[] = {
    -200, 300, 0,
    200, 300, 0
};

static HuVecF dice3PosTbl[] = {
    -200, 300, 0,
    200, 300, 0,
    0, 300, 0
};

static HuVecF *dicePosTbl[] = { dice1PosTbl, dice2PosTbl, dice3PosTbl };

static int diceKuriboResultTbl[] = {
    0, 3, 5, 10, 20, 30, 40
};

static int diceDicePatapataResultTbl[] = {
    1, 3, 5, 10, 15, 20, 25, -1
};

static s8 diceMaxTbl[] ATTRIBUTE_ALIGN(8) = {
    1, 2, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 3
};

static s8 diceValueMaxTbl[] ATTRIBUTE_ALIGN(8) = {
    10, 10, 10, 5, 10, 1, 1, 7, 0, 0, 1, 5, 5, 6, 10, 10, 10, 10, 10, 10, 10
};

static s8 diceSingleValueMaxTbl[] ATTRIBUTE_ALIGN(8) = {
    6, 6, 6, 3, 10, 1, 1, 7, 0, 0, 1, 5, 5, 6, 6, 10, 10, 10, 10, 10, 10
};

static u8 diceMatchEffAnimBankTbl[16] = {
    0, 1, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3
};

static float diceMatchEffScale[4] = {
    1.0f, 1.0f, 1.0f, 0.6f
};

static GXColor diceMatchEffColorTbl[8] = {
    { 220, 64, 64, 0 }, { 64, 220, 64, 0 },
    { 220, 220, 64, 0 }, { 64, 64, 220, 0 },
    { 220, 64, 220, 0 }, { 64, 220, 220, 0 },
    { 220, 120, 64, 0 }, { 64, 120, 220, 0 }
};

static float diceInEffScaleRatio[4] = {
    1.0f, 1.0f, 0.7f, 0.4f
};

static GXColor diceInEffColorTbl[8] = {
    { 220, 100, 100, 0 }, { 100, 220, 100, 0 },
    { 220, 220, 100, 0 }, { 100, 100, 220, 0 },
    { 220, 100, 220, 0 }, { 100, 220, 220, 0 },
    { 220, 150, 100, 0 }, { 100, 150, 220, 0 }
};

static float diceInDotEffScaleRatio[4] = {
    1.0f, 1.0f, 0.7f, 0.4f
};

static GXColor diceInDotEffColorTbl[8] = {
    { 220, 64, 64, 0 }, { 64, 220, 64, 0 },
    { 220, 220, 64, 0 }, { 64, 64, 220, 0 },
    { 220, 64, 220, 0 }, { 64, 220, 220, 0 },
    { 220, 120, 64, 0 }, { 64, 120, 220, 0 }
};

static u8 diceInEffAnimTbl[8] = {
    0, 1, 2, 2, 3, 3, 3, 3
};

static char diceMatchCoinStr[8];

static void DiceProcMain(void);
static void DiceProcDestroy(void);
static void DiceHelpWinCreate(DICE_WORK *work);
static void DiceObjCreate(DICE_WORK *work);
static void DiceObjOMExec(OMOBJ *obj);
static void DiceExec(DICE_WORK *work);
static u16 DicePlayerPadBtn(int playerNo);
static void DiceObjHit(DICE_WORK *work);
static void DiceObjKillSet(OMOBJ *obj);
static void DiceFadeOMExec(OMOBJ *obj);
static void DiceKill(DICE_WORK *work);
static void DiceReadyWait(DICE_WORK *work);
static void DiceHitEffSet(HU3D_MODELID modelId);
static void DiceObjEffCreate(DICE_WORK *work);
static void DiceObjEffKill(DICE_WORK *work);
static void DiceObjEffPuffDraw(HU3D_MODEL *modelP, Mtx *mtx);
static void DiceObjEffTriDraw(HU3D_MODEL *modelP, Mtx *mtx);
static HU3D_MODELID DiceInEffCreate(void);
static HU3D_MODELID DiceInDotEffCreate(void);
static void DiceInEffHook(HU3D_MODEL *modelP, MBPARTICLE *particleP,
    Mtx mtx);
static void DiceInDotEffHook(HU3D_MODEL *modelP, MBPARTICLE *particleP,
    Mtx mtx);
static void DiceNumObjOMExec(OMOBJ *obj);
static void DiceNumObjBendOMExec(OMOBJ *obj);
static void DiceNumObjShrinkOMExec(OMOBJ *obj);
static void DiceNumObjReset(int playerNo);
static DICE_NUM_VTX *DiceNumObjMdlCopy(int modelId);
static void DiceNumObjMdlBend(int modelId, DICE_NUM_VTX *vtx, Mtx mtx1,
    Mtx mtx2);
static void DiceSNpcNumUpdate(OMOBJ *obj);

void mbObjBiriQColorSet(MBMODELID modelId, BOOL setF, float alpha,
    GXColor color);
void mbObjBiriQCreate(MBMODELID modelId);
float mbSinDeg(float angle);
int mbSingleCall(int mode, int arg);
static void ev_DiceZorome(DICE_WORK *work);
static void ev_DiceZoromeCoin(int playerNo, int coin);
static void DiceZoromeEffHook(HU3D_MODEL *modelP, MBPARTICLE *particleP,
    Mtx mtx);
BOOL mbPauseEnableCheck(void);
OMOBJ *mbDiceNumObjCreate(int playerNo, HuVecF *pos1, HuVecF *pos2,
    int value, int color, BOOL followF);
OMOBJ *mbDiceSNpcNumObjCreate(HuVecF *pos, HuVecF *offset, int value,
    BOOL flagF, int color);
void mbCoinAddExec(int playerNo, int coin);
int mbGuideSpeakerNoGet(void);
void mbPos3DtoNorm(HuVecF *src, s16 cameraMask, HuVecF *dst);

BOOL mbDiceKillCheck(int playerNo);
void mbDiceKill(int playerNo);
int mbDiceResultGet(int playerNo);
int mbDiceMaxGet(int diceType);
int mbDiceValueMaxGet(int diceType);
void mbDiceNumObjKill(OMOBJ *obj);
int mbDiceTypeGet(int playerNo);
void mbDiceTutorialNumSet(int playerNo, int tutorialVal);
int mbDiceTutorialNumGet(int playerNo);
int mbDiceValueNoGet(int playerNo);

void mbDiceInit(void)
{
    HUPROCESS **proc = diceProc;
    DICEHITHOOK *hitHook = diceHitHook;
    DICEPADBTNHOOK *padBtnHook = dicePadBtnHook;
    DICEMOTHOOK *motHook = diceMotHook;
    OMOBJ **fadeObj = diceFadeOMObj;
    OMOBJ **numObj = &diceNumOMObj[0][0];
    int i;

    for (i = 0; i < DICE_MAX;
        i++, proc++, hitHook++, padBtnHook++, motHook++, fadeObj++, numObj += 3) {
        *proc = NULL;
        *hitHook = NULL;
        *padBtnHook = NULL;
        *motHook = NULL;
        *fadeObj = NULL;
        numObj[0] = NULL;
        numObj[1] = NULL;
        numObj[2] = NULL;
    }
}

static void DiceProcExec(int playerNo, int diceType, s8 *valueTbl,
    int *tutorialVal, BOOL padWinF, BOOL waitF, HuVecF *pos, int color)
{
    DICE_WORK *work;
    int i;
    HuVecF playerPos;

    if (playerNo < 0) {
        playerNo = DICE_PLAYERNO_NULL;
    }
    diceProc[playerNo] = HuPrcChildCreate(DiceProcMain, 8203, 24576, 0,
        mbMainProc);
    HuPrcDestructorSet2(diceProc[playerNo], DiceProcDestroy);
    work = HuMemDirectMallocNum(HEAP_HEAP, sizeof(DICE_WORK), HU_MEMNUM_OVL);
    diceProc[playerNo]->property = work;
    memset(work, 0, sizeof(DICE_WORK));
    work->playerNo = playerNo;
    work->no = 0;
    work->diceType = diceType;
    work->max = mbDiceMaxGet(diceType);
    if (playerNo < GW_PLAYER_MAX) {
        dicePadBtnHook[playerNo] = DicePlayerPadBtn;
        diceMotHook[playerNo] = mbPlayerDiceMotExec;
    } else {
        dicePadBtnHook[playerNo] = NULL;
        diceMotHook[playerNo] = NULL;
    }
    if (pos == NULL) {
        if (playerNo < GW_PLAYER_MAX) {
            mbPlayerPosGet(work->playerNo, &playerPos);
            work->pos.x = playerPos.x;
            work->pos.y = playerPos.y;
            work->pos.z = playerPos.z;
        } else {
            work->pos.x = work->pos.y = work->pos.z = 0;
        }
    } else {
        work->pos = *pos;
    }
    for (i = 0; valueTbl[i] >= 0; i++) {
        work->valueTbl[i] = (s8)valueTbl[i];
    }
    work->valueNum = i;
    if (tutorialVal != NULL) {
        for (i = 0; i < 3; i++) {
            work->tutorialVal[i] = tutorialVal[i];
        }
    } else {
        work->tutorialVal[0] = work->tutorialVal[1] = work->tutorialVal[2] = -1;
    }
    work->padWinF = padWinF;
    work->color = color;
    if (playerNo < GW_PLAYER_MAX) {
        GwPlayer[playerNo].diceNum = work->max;
    }
    if (waitF) {
        while (!mbDiceKillCheck(playerNo)) {
            HuPrcVSleep();
        }
    }
}

int mbDiceProcExec(int playerNo, int diceType, s8 *valueTbl,
    int *tutorialVal, BOOL padWinF, BOOL waitF, HuVecF *pos, int color)
{
    s8 valueTblNew[DICE_VALUENUM_MAX];
    int i;

    if (valueTbl == NULL) {
        for (i = 0; i < mbDiceValueMaxGet(diceType); i++) {
            valueTblNew[i] = i;
        }
        valueTblNew[i] = -1;
        DiceProcExec(playerNo, diceType, valueTblNew, tutorialVal, padWinF,
            waitF, pos, color);
    } else {
        DiceProcExec(playerNo, diceType, valueTbl, tutorialVal, padWinF,
            waitF, pos, color);
    }
    if (waitF) {
        return mbDiceResultGet(playerNo);
    }
    return -1;
}

inline int mbDiceProcExec(int playerNo, int diceType, s8 *valueTbl,
    int *tutorialVal, BOOL padWinF, BOOL waitF, HuVecF *pos, int color);

int mbDiceExec(int playerNo, int diceType, s8 *valueTbl, int tutorialVal,
    BOOL padWinF, BOOL waitF, HuVecF *pos, int color)
{
    int tutorialValData[3];

    if (tutorialVal < 0) {
        return mbDiceProcExec(playerNo, diceType, valueTbl, NULL, padWinF,
            waitF, pos, color);
    }
    tutorialValData[0] = tutorialVal;
    tutorialValData[1] = -1;
    tutorialValData[2] = -1;
    return mbDiceProcExec(playerNo, diceType, valueTbl, tutorialValData,
        padWinF, waitF, pos, color);
}

int mbDicePlayerExec(int playerNo, int diceType)
{
    mbDiceProcExec(playerNo, diceType, NULL, NULL, TRUE, TRUE, NULL,
        DICE_COLOR_GREEN);
    return mbDiceResultGet(playerNo);
}

int mbDiceChanceTradeExec(int playerNo)
{
    s8 valueTbl[8];
    int i;

    for (i = 0; i < 7; i++) {
        valueTbl[i] = i;
    }
    valueTbl[i] = -1;
    DiceProcExec(playerNo, DICETYPE_CHANCE_TRADE, valueTbl, NULL, TRUE, TRUE,
        NULL, DICE_COLOR_GREEN);
    return mbDiceResultGet(playerNo);
}

int mbDiceChanceCharExec(int playerNo, int *playerNoTbl)
{
    s8 valueTbl[GW_PLAYER_MAX + 1];
    int num;
    int i;

    for (num = 0, i = 0; i < GW_PLAYER_MAX; i++) {
        if (playerNoTbl[i] >= 0) {
            valueTbl[num++] = GwPlayer[playerNoTbl[i]].charNo;
        }
    }
    valueTbl[num] = -1;
    DiceProcExec(playerNo, DICETYPE_CHANCE_CHAR, valueTbl, NULL, TRUE, TRUE,
        NULL, DICE_COLOR_GREEN);
    return mbDiceResultGet(playerNo);
}

static void DiceProcMain(void)
{
    int i;
    DICE_WORK *work = HuPrcCurrentGet()->property;

    memset(&work->result[0], 0, 3);
    DiceObjEffCreate(work);
    for (i = 0; i < work->max; i++) {
        if (work->padWinF) {
            DiceHelpWinCreate(work);
        }
        DiceObjCreate(work);
        DiceReadyWait(work);
        DiceExec(work);
        if (work->padWinF) {
            mbWinTopKill();
        }
        if (diceResult[work->playerNo] == -2
            || diceResult[work->playerNo] == -1
            || diceResult[work->playerNo] == -5
            || diceResult[work->playerNo] == -4
            || diceResult[work->playerNo] == -3
            || diceResult[work->playerNo] == -6) {
            HuPrcEnd();
        }
        if (diceMotHook[work->playerNo] != NULL) {
            diceMotHook[work->playerNo](work->playerNo);
        }
        while (work->obj != NULL) {
            HuPrcVSleep();
        }
        work->no++;
    }
    while (!mbDiceNumStopCheck(work->playerNo)) {
        HuPrcVSleep();
    }
    if (work->max >= 2 && GWPartyGet() != FALSE
        && _CheckFlag(FLAG_BOARD_OPENING) && work->diceType <= 14) {
        for (i = 0; i < work->max - 1; i++) {
            if (work->result[i] != work->result[i + 1]) {
                break;
            }
        }
        if (i >= work->max - 1) {
            ev_DiceZorome(work);
        }
    }
    if (work->max >= 2) {
        HuPrcSleep(30);
        DiceNumObjReset(work->playerNo);
        while (!mbDiceNumStopCheck(work->playerNo)) {
            HuPrcVSleep();
        }
    }
    diceResult[work->playerNo] = 0;
    for (i = 0; i < work->max; i++) {
        diceResult[work->playerNo] += work->result[i];
    }
    HuPrcEnd();
}

static void DiceProcDestroy(void)
{
    DICE_WORK *work = HuPrcCurrentGet()->property;

    mbDiceKill(work->playerNo);
    DiceObjEffKill(work);
    diceProc[work->playerNo] = NULL;
    diceHitHook[work->playerNo] = NULL;
    diceMotHook[work->playerNo] = NULL;
    dicePadBtnHook[work->playerNo] = NULL;
    HuMemDirectFree(work);
}

static void DiceHelpWinCreate(DICE_WORK *work)
{
    u32 mess;

    if (_CheckFlag(FLAG_BOARD_TUTORIAL)) {
        return;
    }
    if (work->diceType == DICETYPE_PATAPATA_COIN
        || work->diceType == DICETYPE_EVENT
        || work->diceType == DICETYPE_CHANCE_TRADE
        || work->diceType == DICETYPE_CHANCE_CHAR
        || work->diceType == 11 || work->diceType == 15
        || work->diceType == 19 || work->diceType == 20
        || work->diceType == 13 || work->diceType == DICETYPE_BLOCK) {
        mess = 0x00260002;
    } else if (work->no == 0) {
        int capsuleNum = mbPlayerCapsuleNumGet(work->playerNo);

        if (GWPartyGet() == FALSE) {
            capsuleNum = TRUE;
            if (GwSystem.turnNo <= 1) {
                capsuleNum = FALSE;
            }
        }
        if (mbPlayerCapsuleUseGet() != -1 || capsuleNum == 0) {
            mess = 0x00260001;
            if (GWPartyGet() == FALSE) {
                mess = 0x0026000E;
            }
        } else {
            mess = 0x00260000;
            if (GWPartyGet() == FALSE) {
                mess = 0x0026000D;
            }
        }
    } else {
        mess = 0x00260002;
    }
    mbWinCreateHelp(mess);
}

static void DiceObjCreate(DICE_WORK *work)
{
    MBCAMERA *cameraP = mbCameraGet();
    OMOBJ *obj;
    DICE_OBJ_WORK *objWork;
    float time;
    HuVecF parManVec;

    obj = work->obj = omAddObj(mbObjMan, 258, 5, 0, DiceObjOMExec);
    omSetStatBit(obj, OM_STAT_MODELPAUSE);
    obj->data = work;
    if (work->diceType != DICETYPE_BLOCK) {
        obj->mdlId[0] = mbObjCreate(
            mbBoardDataNumGet(diceObjFileTbl[work->diceType]), NULL, FALSE);
    } else {
        obj->mdlId[0] = mbObjCreate(
            mbBoardDataNumGet(diceObjFileTbl[work->diceType]), NULL, FALSE);
    }
    mbObjLayerSet(obj->mdlId[0], 4);
    mbObjCameraSet(obj->mdlId[0], HU3D_CAM1);
    mbAudFXPlay(0x3EF);
    if (mbObjMotionIDGet(obj->mdlId[0], 0) >= 0) {
        mbObjMotionSet(obj->mdlId[0], 0, 0);
        mbObjMotionSpeedSet(obj->mdlId[0], 0.0f);
    }
    time = mbRandMod(work->valueNum) + 0.5f;
    mbObjMotionTimeSet(obj->mdlId[0], time);
    mbObjAlphaSet(obj->mdlId[0], 0);
    objWork = omObjGetWork(obj, DICE_OBJ_WORK);
    objWork->killF = FALSE;
    objWork->lockF = FALSE;
    objWork->fadeF = diceFadeFlagTbl[work->diceType];
    objWork->mode = 0;
    objWork->no = work->no;
    objWork->time = 0;
    objWork->maxTime = 20;
    objWork->lockTime = 0;
    objWork->valueNo = time;
    obj->trans.x = work->pos.x;
    obj->trans.y = work->pos.y + 250.0f;
    obj->trans.z = work->pos.z;
    mbObjPosSet(obj->mdlId[0], obj->trans.x, obj->trans.y, obj->trans.z);
    obj->mdlId[1] = DiceInEffCreate();
    obj->mdlId[2] = DiceInDotEffCreate();
    Hu3DModelPosSet(obj->mdlId[1], obj->trans.x, obj->trans.y,
        obj->trans.z);
    Hu3DModelPosSet(obj->mdlId[2], obj->trans.x, obj->trans.y,
        obj->trans.z);
    VECSubtract(&cameraP->eye, &obj->trans, &parManVec);
    obj->mdlId[3] = Hu3DHookFuncCreate(DiceObjEffPuffDraw);
    Hu3DModelCameraSet(obj->mdlId[3], HU3D_CAM0);
    Hu3DModelLayerSet(obj->mdlId[3], 5);
    Hu3DModelDispOff(obj->mdlId[3]);
    Hu3DData[obj->mdlId[3]].hookData = obj;
    work->puffTime = 0;
    obj->mdlId[4] = Hu3DHookFuncCreate(DiceObjEffTriDraw);
    Hu3DModelCameraSet(obj->mdlId[4], HU3D_CAM0);
    Hu3DModelLayerSet(obj->mdlId[4], 5);
    Hu3DModelDispOff(obj->mdlId[4]);
    Hu3DData[obj->mdlId[4]].hookData = obj;
    work->triTime = 0;
}

static void DiceObjOMExec(OMOBJ *obj)
{
    DICE_OBJ_WORK *objWork = omObjGetWork(obj, DICE_OBJ_WORK);
    DICE_WORK *work = obj->data;
    int i;
    float time;
    float scale;
    HuVecF modelPos;
    HuVecF pos;

    if (objWork->killF || mbExitCheck()) {
        if (objWork->diceSeNo >= 0) {
            HuAudFXStop(objWork->diceSeNo);
            objWork->diceSeNo = -1;
        }
        if (obj->mdlId[0] >= 0) {
            mbObjKill(obj->mdlId[0]);
            obj->mdlId[0] = MB_MODEL_NONE;
        }
        mbParticleKill(obj->mdlId[1]);
        mbParticleKill(obj->mdlId[2]);
        obj->mdlId[1] = HU3D_MODELID_NONE;
        obj->mdlId[2] = HU3D_MODELID_NONE;
        for (i = 3; i <= 4; i++) {
            if (obj->mdlId[i] >= 0) {
                Hu3DModelKill(obj->mdlId[i]);
                obj->mdlId[i] = HU3D_MODELID_NONE;
            }
        }
        obj->data = NULL;
        omDelObjEx(HuPrcCurrentGet(), obj);
        work->obj = NULL;
        return;
    }
    switch (objWork->mode) {
        case 0:
            time = (float)objWork->time / objWork->maxTime;
            mbObjAlphaSet(obj->mdlId[0], time * 255.0f);
            if (++objWork->time > objWork->maxTime) {
                objWork->mode = 1;
                objWork->time = 0;
                objWork->maxTime = 12;
                objWork->diceSeNo = mbAudFXPlay(0x3ED);
            }
            break;

        case 2:
            time = (float)objWork->time / objWork->maxTime;
            if (time > 1.0f) {
                time = 1.0f;
            }
            objWork->time++;
            scale = HuSin(time * 180.0f);
            mbObjScaleSet(obj->mdlId[0], 1.0f + scale,
                1.0f - (0.5f * scale), 1.0f + scale);
            modelPos.y = obj->trans.y + (150.0f * scale);
            mbObjPosSet(obj->mdlId[0], obj->trans.x, modelPos.y,
                obj->trans.z);
            if (objWork->time == 8) {
                mbObjPosGet(obj->mdlId[0], &modelPos);
                if (work->diceType != DICETYPE_CHANCE_TRADE
                    && work->diceType != DICETYPE_CHANCE_CHAR
                    && work->diceType != 11 && work->diceType != 12
                    && work->diceType != DICETYPE_EVENT
                    && work->diceType != DICETYPE_BLOCK) {
                    if (work->playerNo < GW_PLAYER_MAX) {
                        mbPlayerPosGet(work->playerNo, &pos);
                    } else {
                        pos.x = work->pos.x;
                        pos.y = work->pos.y;
                        pos.z = work->pos.z;
                    }
                    VECAdd(&pos, &dicePosTbl[work->max - 1][work->no], &pos);
                    if (work->diceType != 13) {
                        if (work->no == 2) {
                            pos.z += 100.0f;
                        }
                        diceNumOMObj[work->playerNo][work->no] =
                            mbDiceNumObjCreate(work->playerNo, &modelPos, &pos,
                                work->result[work->no], work->color, TRUE);
                    } else {
                        diceNumOMObj[work->playerNo][work->no] =
                            mbDiceNumObjCreate(work->playerNo, &modelPos, &pos,
                                diceKuriboResultTbl[work->result[work->no]],
                                work->color, TRUE);
                    }
                }
            }
            if (objWork->time > objWork->maxTime) {
                objWork->mode = 3;
                objWork->time = 0;
                objWork->maxTime = 18;
            }
            break;

        case 3:
            time = (float)objWork->time / 15.0f;
            if (time > 1.0f) {
                time = 1.0f;
            }
            if (++objWork->time > objWork->maxTime) {
                if (objWork->fadeF) {
                    diceFadeOMObj[work->playerNo] = omAddObj(mbObjMan, 258,
                        2, 0, DiceFadeOMExec);
                    omSetStatBit(diceFadeOMObj[work->playerNo],
                        OM_STAT_MODELPAUSE);
                    diceFadeOMObj[work->playerNo]->mdlId[0] = obj->mdlId[0];
                    obj->mdlId[0] = HU3D_MODELID_NONE;
                }
                DiceObjKillSet(obj);
            }
            if (!objWork->fadeF) {
                mbObjAlphaSet(obj->mdlId[0], (1.0f - time) * 255.0f);
            }
            break;
    }
    if (!objWork->lockF && objWork->mode < 2 && work->valueNum > 1) {
        if (work->diceType != 14) {
            if (objWork->lockTime++ > 4) {
                s16 valueTbl[DICE_VALUENUM_MAX];
                int valueNum;

                objWork->lockTime = 0;
                valueNum = 0;
                for (i = 0; i < work->valueNum; i++) {
                    if (i != objWork->valueNo) {
                        valueTbl[valueNum++] = i;
                    }
                }
                objWork->valueNo = valueTbl[mbRandMod(valueNum)];
            }
        } else {
            if (objWork->lockTime++ > 20) {
                objWork->lockTime = 0;
                objWork->valueNo++;
                if (objWork->valueNo >= work->valueNum) {
                    objWork->valueNo = 0;
                }
            }
        }
        mbObjMotionTimeSet(obj->mdlId[0],
            work->valueTbl[objWork->valueNo] + 0.5f);
    }
}

static void DiceObjKillSet(OMOBJ *obj)
{
    omObjGetWork(obj, DICE_OBJ_WORK)->killF = TRUE;
}

static void DiceFadeOMExec(OMOBJ *obj)
{
    DICE_FADE_WORK *work = omObjGetWork(obj, DICE_FADE_WORK);
    float time;
    float scale;

    if (work->killF || mbExitCheck()) {
        if (obj->mdlId[0] >= 0) {
            mbObjKill(obj->mdlId[0]);
            obj->mdlId[0] = MB_MODEL_NONE;
        }
        omDelObjEx(HuPrcCurrentGet(), obj);
        diceFadeOMObj[work->playerNo] = NULL;
        return;
    }
    work->angle++;
    if (work->angle >= 30) {
        work->angle -= 30;
    }
    time = (float)work->angle * (1.0f / 30.0f);
    time = 360.0f * time;
    scale = 1.15f + (0.15f * mbSinDeg(time - 90.0f));
    mbObjScaleSet(obj->mdlId[0], scale, scale, scale);
    if (work->fadeF) {
        time = (float)work->time / 15.0f;
        if (time > 1.0f) {
            time = 1.0f;
        }
        if (++work->time > 16) {
            work->killF = TRUE;
        }
        mbObjAlphaSet(obj->mdlId[0], (1.0f - time) * 255.0f);
    }
}

void mbDiceFadeSet(int playerNo)
{
    OMOBJ *obj;
    DICE_FADE_WORK *work;

    if (playerNo < 0) {
        playerNo = DICE_PLAYERNO_NULL;
    }
    obj = diceFadeOMObj[playerNo];
    if (obj != NULL) {
        work = omObjGetWork(obj, DICE_FADE_WORK);
        work->fadeF = TRUE;
        work->time = 0;
    }
}

void mbDiceKill(int playerNo)
{
    if (diceProc[playerNo] != NULL) {
        DICE_WORK *work = diceProc[playerNo]->property;

        DiceKill(work);
        HuPrcKill(diceProc[playerNo]);
    }
}

inline void mbDiceKill(int playerNo);

void mbDiceClose(void)
{
    int i;

    for (i = 0; i < GW_PLAYER_MAX; i++) {
        mbDiceKill(i);
    }
}

static void DiceKill(DICE_WORK *work)
{
    if (work->obj != NULL) {
        DiceObjKillSet(work->obj);
    }
}

static void DiceExec(DICE_WORK *work)
{
    u16 btn = 0;
    int capsuleNum;

    diceResult[work->playerNo] = 0;
    while (TRUE) {
        do {
            HuPrcVSleep();
        } while (mbPauseProcCheck());
        if (dicePadBtnHook[work->playerNo] != NULL) {
            btn = dicePadBtnHook[work->playerNo](work->playerNo);
        }
        if (btn == PAD_BUTTON_A) {
            break;
        }
        if (work->no != 0 || !_CheckFlag(FLAG_BOARD_OPENING)
            || work->diceType == DICETYPE_EVENT || work->diceType == 15
            || work->diceType == 19 || work->diceType == 20
            || work->diceType == 13
            || work->diceType == DICETYPE_PATAPATA_COIN
            || work->diceType == DICETYPE_CHANCE_TRADE
            || work->diceType == DICETYPE_CHANCE_CHAR
            || work->diceType == DICETYPE_BLOCK || work->diceType == 11) {
            continue;
        }
        if (btn == PAD_BUTTON_Y && GWPartyGet() != FALSE) {
            diceResult[work->playerNo] = -4;
            mbAudFXPlay(1);
            break;
        }
        if (btn == PAD_BUTTON_X) {
            diceResult[work->playerNo] = -3;
            mbAudFXPlay(1);
            break;
        }
        if (btn == PAD_BUTTON_B && work->playerNo < GW_PLAYER_MAX) {
            capsuleNum = mbPlayerCapsuleNumGet(work->playerNo);
            if (GWPartyGet() == FALSE) {
                capsuleNum = TRUE;
                if (GwSystem.turnNo <= 1) {
                    capsuleNum = FALSE;
                }
            }
            if (mbPlayerCapsuleUseGet() == -1 && capsuleNum != 0) {
                diceResult[work->playerNo] = -6;
                break;
            }
        }
    }
}

static u16 DicePlayerPadBtn(int playerNo)
{
    int valueNo;
    u16 ret;

    if (!GwPlayer[playerNo].comF) {
        ret = HuPadBtnDown[GwPlayer[playerNo].padNo];
    } else {
        ret = PAD_BUTTON_A;
        if (mbDiceTypeGet(playerNo) == 14 &&
            mbDiceTutorialNumGet(playerNo) >= 0) {
            valueNo = mbDiceTutorialNumGet(playerNo) - 1;

            if (valueNo < 0) {
                valueNo += mbDiceValueMaxGet(14);
            }
            if (valueNo != mbDiceValueNoGet(playerNo)) {
                ret = 0;
            } else {
                mbDiceTutorialNumSet(playerNo, -1);
            }
        }
    }
    return ret;
}

static void DiceReadyWait(DICE_WORK *work)
{
    DICE_OBJ_WORK *objWork = omObjGetWork(work->obj, DICE_OBJ_WORK);

    while (objWork->mode != 1) {
        HuPrcVSleep();
    }
}

static void DiceObjHit(DICE_WORK *work)
{
    OMOBJ *obj = work->obj;
    DICE_OBJ_WORK *objWork = omObjGetWork(work->obj, DICE_OBJ_WORK);

    objWork->mode = 2;
    objWork->lockF = TRUE;
    objWork->time = 0;
    objWork->maxTime = 12;
    if (work->playerNo >= 0 && work->playerNo < GW_PLAYER_MAX) {
        omVibrate(work->playerNo, 20, 7, 3);
    }
    if (objWork->diceSeNo >= 0) {
        HuAudFXStop(objWork->diceSeNo);
        objWork->diceSeNo = -1;
    }
    DiceHitEffSet(obj->mdlId[1]);
    DiceHitEffSet(obj->mdlId[2]);
    Hu3DModelDispOn(obj->mdlId[4]);
    if (work->diceType != DICETYPE_CHANCE_TRADE
        && work->diceType != DICETYPE_CHANCE_CHAR
        && work->diceType != 11 && work->diceType != 12) {
        if (GWPartyGet() == FALSE && work->playerNo == 0
            && !_CheckFlag(FLAG_BOARD_TUTORIAL)) {
            work->tutorialVal[work->no] = mbSingleCall(2, -1);
        }
        if (work->tutorialVal[work->no] >= 0) {
            work->result[work->no] = work->tutorialVal[work->no] + 1;
            mbObjMotionTimeSet(obj->mdlId[0],
                work->tutorialVal[work->no] + 0.5f);
        } else {
            work->result[work->no] = work->valueTbl[objWork->valueNo] + 1;
        }
        if (work->diceType == DICETYPE_PATAPATA_COIN) {
            work->result[work->no] =
                diceDicePatapataResultTbl[work->result[work->no] - 1];
        }
    } else {
        if (work->tutorialVal[work->no] >= 0) {
            work->result[work->no] = work->tutorialVal[work->no];
            mbObjMotionTimeSet(obj->mdlId[0], work->result[work->no] + 0.5f);
        } else {
            work->result[work->no] = mbObjMotionTimeGet(obj->mdlId[0]) + 0.5f;
        }
    }
    mbAudFXPlay(0x3F0);
    if (diceHitHook[work->playerNo] != NULL) {
        diceHitHook[work->playerNo](work->result[work->no]);
    }
}

BOOL mbDiceKillCheck(int playerNo)
{
    if (playerNo < 0) {
        playerNo = DICE_PLAYERNO_NULL;
    }
    return diceProc[playerNo] == NULL;
}

inline BOOL mbDiceKillCheck(int playerNo);

BOOL mbDiceKillCheckAll(void)
{
    int i;

    for (i = 0; i < GW_PLAYER_MAX; i++) {
        if (!mbDiceKillCheck(i)) {
            return FALSE;
        }
    }
    return TRUE;
}

int mbDiceResultGet(int playerNo)
{
    if (playerNo < 0) {
        playerNo = DICE_PLAYERNO_NULL;
    }
    return diceResult[playerNo];
}

void mbDiceHitHookSet(int playerNo, DICEHITHOOK hook)
{
    if (playerNo < 0) {
        playerNo = DICE_PLAYERNO_NULL;
    }
    diceHitHook[playerNo] = hook;
}

void mbDiceObjHit(int playerNo)
{
    if (playerNo < 0) {
        playerNo = DICE_PLAYERNO_NULL;
    }
    DiceObjHit(diceProc[playerNo]->property);
}

void mbDicePadBtnHookSet(int playerNo, DICEPADBTNHOOK hook)
{
    if (playerNo < 0) {
        playerNo = DICE_PLAYERNO_NULL;
    }
    dicePadBtnHook[playerNo] = hook;
}

void mbDiceMotHookSet(int playerNo, DICEMOTHOOK hook)
{
    if (playerNo < 0) {
        playerNo = DICE_PLAYERNO_NULL;
    }
    diceMotHook[playerNo] = hook;
}

int mbDiceTypeGet(int playerNo)
{
    DICE_WORK *work;

    if (playerNo < 0) {
        playerNo = DICE_PLAYERNO_NULL;
    }
    work = diceProc[playerNo]->property;
    return work->diceType;
}

void mbDiceTutorialNumSet(int playerNo, int tutorialVal)
{
    DICE_WORK *work;

    if (playerNo < 0) {
        playerNo = DICE_PLAYERNO_NULL;
    }
    work = diceProc[playerNo]->property;
    work->tutorialVal[work->no] = tutorialVal;
}

int mbDiceTutorialNumGet(int playerNo)
{
    DICE_WORK *work;

    if (playerNo < 0) {
        playerNo = DICE_PLAYERNO_NULL;
    }
    work = diceProc[playerNo]->property;
    return work->tutorialVal[work->no];
}

int mbDiceValueNoGet(int playerNo)
{
    DICE_WORK *work;
    DICE_OBJ_WORK *objWork;

    if (playerNo < 0) {
        playerNo = DICE_PLAYERNO_NULL;
    }
    work = diceProc[playerNo]->property;
    objWork = omObjGetWork(work->obj, DICE_OBJ_WORK);
    return objWork->valueNo;
}

int mbDiceMaxGet(int type)
{
    return diceMaxTbl[type];
}

int mbDiceValueMaxGet(int type)
{
    if (GWPartyGet() == FALSE) {
        return diceSingleValueMaxTbl[type];
    } else {
        return diceValueMaxTbl[type];
    }
}

static inline void DiceNumPosSet(HuVecF *src, HuVecF *dst)
{
    HU3D_CAMERA *cameraP = &Hu3DCamera[0];
    Mtx lookAt;
    HuVecF normPos;
    HuVecF camPos;
    float tanFov;
    float x;
    float y;

    MTXLookAt(lookAt, &cameraP->pos, &cameraP->up, &cameraP->target);
    MTXMultVec(lookAt, src, &camPos);
    tanFov = HuSin(cameraP->fov * 0.5f) / HuCos(cameraP->fov * 0.5f);
    x = camPos.x / (tanFov * camPos.z);
    y = camPos.y / (tanFov * camPos.z);
    normPos.x = -2000.0f * (x * tanFov);
    normPos.y = -2000.0f * (y * tanFov);
    normPos.z = -2000.0f;
    MTXInverse(lookAt, lookAt);
    MTXMultVec(lookAt, &normPos, dst);
}

OMOBJ *mbDiceNumObjCreate(int playerNo, HuVecF *pos1, HuVecF *pos2,
    int value, int color, BOOL followF)
{
    int modelNo;
    int digit;
    DICE_WORK *diceWork = diceProc[playerNo]->property;
    OMOBJ *obj;
    DICE_NUM_WORK *objWork;
    DICE_NUM_VTX **vtx;
    int modelId;
    HU3D_MODELID tempMdlId;
    MBCAMERA *cameraP;
    Mtx rot;

    cameraP = mbCameraGet();
    if (value > 99) {
        value = 99;
    }
    obj = omAddObjEx(mbObjMan, 258, 3, 0, OM_GRP_NONE, DiceNumObjOMExec);
    omSetStatBit(obj, OM_STAT_MODELPAUSE);
    obj->data = vtx = HuMemDirectMallocNum(HEAP_HEAP,
        sizeof(DICE_NUM_VTX *) * 2, HU_MEMNUM_OVL);
    memset(obj->data, 0, sizeof(DICE_NUM_VTX *) * 2);
    objWork = omObjGetWork(obj, DICE_NUM_WORK);
    objWork->killF = FALSE;
    objWork->rotF = FALSE;
    objWork->updateF = TRUE;
    objWork->value = value;
    objWork->time = 0;
    objWork->maxTime = 30;
    objWork->playerNo = playerNo;
    objWork->color = color;
    modelNo = 0;
    digit = value;
    MTXRotDeg(rot, 'y', cameraP->rot.y);
    while (TRUE) {
        static GXColor biriQColor = { 255, 255, 255, 255 };

        modelId = mbObjCreate(
            mbBoardDataNumGet(numberFileTbl[digit % 10]), NULL, FALSE);
        vtx[modelNo] = DiceNumObjMdlCopy(mbObjModelIDGet(modelId));
        mbObjPosSetV(modelId, pos1);
        mbObjMotionSet(modelId, 0, 0);
        mbObjMotionSpeedSet(modelId, 0.0f);
        mbObjMotionTimeSet(modelId, color + 0.5f);
        mbObjScaleSet(modelId, 1.0f, 1.0f, 1.0f);
        mbObjMtxSet(modelId, &rot);
        tempMdlId = mbObjModelIDGet(modelId);
        mbObjCameraSet(modelId, HU3D_CAM1);
        Hu3DModelLayerSet(tempMdlId, 4);
        mbObjBiriQCreate(modelId);
        mbObjBiriQColorSet(modelId, TRUE, 0.0f, biriQColor);
        obj->mdlId[modelNo] = modelId;
        modelNo++;
        digit /= 10;
        if (digit == 0) {
            break;
        }
    }
    objWork->modelNo = modelNo;
    for (; modelNo < 2; modelNo++) {
        obj->mdlId[modelNo] = MB_MODEL_NONE;
    }
    obj->trans.x = obj->scale.x = pos1->x;
    obj->trans.y = obj->scale.y = pos1->y;
    obj->trans.z = obj->scale.z = pos1->z;
    if (pos2 != NULL) {
        obj->rot.x = pos2->x;
        obj->rot.y = pos2->y;
        obj->rot.z = pos2->z;
        if (__fabsf(obj->rot.x - obj->trans.x) > 25.0f) {
            obj->rot.y += -50.0f;
        } else if (__fabsf(obj->rot.z - obj->trans.z) > 25.0f) {
            obj->rot.y -= -50.0f;
        }
    } else {
        obj->rot.x = pos1->x;
        obj->rot.y = pos1->y;
        obj->rot.z = pos1->z;
    }
    if (!followF) {
        DiceNumPosSet(&obj->rot, &obj->rot);
        DiceNumPosSet(&obj->trans, &obj->scale);
    }
    return obj;
}

static void DiceNumObjOMExec(OMOBJ *obj)
{
    DICE_NUM_WORK *objWork = omObjGetWork(obj, DICE_NUM_WORK);
    DICE_NUM_VTX **vtx = obj->data;
    float time;
    HuVecF modelPos;
    HuVecF bendPos;
    HuVecF basePos;
    HuVecF dir;
    int i;
    Mtx rot1;
    Mtx rot2;

    if (objWork->killF || mbExitCheck()) {
        mbDiceNumObjKill(obj);
        return;
    }
    if (!objWork->updateF) {
        return;
    }
    if (++objWork->time >= objWork->maxTime) {
        objWork->updateF = FALSE;
    }
    time = (float)objWork->time / objWork->maxTime;
    dir.z = HuCos(time * 90);
    dir.y = HuSin(time * 90);
    dir.x = HuSin(time * 180);
    for (i = 0; i < 2; i++) {
        float angle;

        if (obj->mdlId[i] < 0) {
            continue;
        }
        angle = 450 * time;
        if (angle > 360) {
            angle = 360;
        }
        MTXRotDeg(rot2, 'y', angle);
        angle = (450 * time) - 90;
        if (angle < 0) {
            angle = 0;
        }
        MTXRotDeg(rot1, 'y', angle);
        DiceNumObjMdlBend(mbObjModelIDGet(obj->mdlId[i]), vtx[i], rot1,
            rot2);
        if (objWork->modelNo < 2) {
            bendPos.x = obj->rot.x;
            bendPos.y = obj->rot.y;
            bendPos.z = obj->rot.z;
        } else {
            Mtx modelMtx;

            mbObjMtxGet(obj->mdlId[i], &modelMtx);
            if (i != 0) {
                basePos.x = -60.000004f;
                basePos.y = basePos.z = 0;
            } else {
                basePos.x = 60.000004f;
                basePos.y = basePos.z = 0;
            }
            MTXMultVec(modelMtx, &basePos, &bendPos);
            bendPos.x += obj->rot.x;
            bendPos.y += obj->rot.y;
            bendPos.z += obj->rot.z;
        }
        modelPos.x = obj->trans.x + (time * (bendPos.x - obj->trans.x));
        if (!objWork->rotF) {
            modelPos.y = obj->trans.y
                + (time * (bendPos.y - obj->trans.y))
                + (100 * (1.1f * HuSin(time * 180)));
        } else {
            modelPos.y = obj->trans.y
                + (time * (bendPos.y - obj->trans.y));
        }
        modelPos.z = obj->trans.z + (time * (bendPos.z - obj->trans.z));
        mbObjPosSetV(obj->mdlId[i], &modelPos);
        if (objWork->rotF) {
            float alpha = time - 0.3f;

            if (alpha > 0) {
                alpha *= 2.5f;
                alpha *= alpha;
                if (alpha > 1) {
                    alpha = 1;
                }
                {
                    static GXColor biriQColor = { 255, 255, 255, 255 };

                    mbObjBiriQColorSet(obj->mdlId[i], TRUE, alpha,
                        biriQColor);
                }
            }
        }
    }
}

static void DiceNumObjBendOMExec(OMOBJ *obj)
{
    DICE_NUM_WORK *objWork = omObjGetWork(obj, DICE_NUM_WORK);
    DICE_NUM_VTX **vtx = obj->data;
    float angle;
    float time;
    float posY;
    float rotAngle;
    float maxAngle;
    float colorTime = 0;
    Mtx rot1;
    Mtx rot2;
    HuVecF pos;
    int i;

    if (objWork->killF || mbExitCheck()) {
        mbDiceNumObjKill(obj);
        return;
    }
    if (!objWork->updateF) {
        return;
    }
    switch (objWork->bendMode) {
        case 0:
            time = (float)objWork->time / objWork->maxTime;
            if (objWork->value == 10) {
                rotAngle = 30;
            } else {
                rotAngle = 60;
            }
            maxAngle = rotAngle;
            angle = time * 720;
            MTXRotDeg(rot2, 'z', maxAngle * HuSin(angle));
            MTXIdentity(rot1);
            posY = obj->rot.y;
            if (++objWork->time > objWork->maxTime) {
                if (objWork->bendF) {
                    objWork->bendMode = 1;
                    objWork->time = 0;
                    objWork->maxTime = 45;
                } else {
                    objWork->updateF = FALSE;
                }
            }
            colorTime = fmod(time * 2, 1);
            break;

        case 1:
            time = (float)objWork->time / objWork->maxTime;
            angle = time * 450;
            if (angle > 360) {
                angle = 360;
            }
            MTXRotDeg(rot2, 'y', angle);
            angle = (450 * time) - 90;
            if (angle < 0) {
                angle = 0;
            }
            MTXRotDeg(rot1, 'y', angle);
            posY = obj->rot.y + (100 * (2 * HuSin(time * 180)));
            if (++objWork->time > objWork->maxTime) {
                if (objWork->bendF) {
                    objWork->bendMode = 0;
                    objWork->time = 0;
                    objWork->maxTime = 120;
                } else {
                    objWork->updateF = FALSE;
                }
            }
            colorTime = time;
            break;
    }
    colorTime *= 360;
    for (i = 0; i < 2; i++) {
        static GXColor biriQColor = { 255, 255, 255, 255 };

        if (obj->mdlId[i] >= 0) {
            float alpha;

            DiceNumObjMdlBend(mbObjModelIDGet(obj->mdlId[i]), vtx[i], rot1,
                rot2);
            mbObjPosGet(obj->mdlId[i], &pos);
            pos.y = posY;
            mbObjPosSetV(obj->mdlId[i], &pos);
            alpha = fabs(0.4f * mbSinDeg(colorTime));
            mbObjBiriQColorSet(obj->mdlId[i], TRUE, alpha, biriQColor);
        }
    }
}

void mbDiceNumObjKill(OMOBJ *obj)
{
    int i;

    for (i = 0; i < 2; i++) {
        if (obj->mdlId[i] != MB_MODEL_NONE) {
            mbObjKill(obj->mdlId[i]);
        }
    }
    omDelObj(HuPrcCurrentGet(), obj);
}

static void DiceNumObjShrinkOMExec(OMOBJ *obj)
{
    DICE_NUM_WORK *objWork = omObjGetWork(obj, DICE_NUM_WORK);
    HuVecF pos;
    float time;
    float scaleX;
    float scaleY;
    float posY;
    int i;

    if (objWork->time > objWork->maxTime) {
        objWork->killF = TRUE;
    }
    if (objWork->killF || mbExitCheck()) {
        mbDiceNumObjKill(obj);
        return;
    }
    time = (float)(objWork->time++) / objWork->maxTime;
    scaleX = HuSin(time * 270);
    scaleY = HuSin((time * 270) + 180);
    posY = obj->rot.y + (100 * time);
    for (i = 0; i < 2; i++) {
        if (obj->mdlId[i] >= 0) {
            mbObjScaleSet(obj->mdlId[i], 1 + scaleX, 1 + scaleY, 1);
            mbObjPosGet(obj->mdlId[i], &pos);
            pos.y = posY;
            mbObjPosSetV(obj->mdlId[i], &pos);
        }
    }
}

static void DiceNumObjReset(int playerNo)
{
    int i;

    for (i = 0; i < 3; i++) {
        OMOBJ *obj = diceNumOMObj[playerNo][i];

        if (obj != NULL) {
            DICE_NUM_WORK *work = omObjGetWork(obj, DICE_NUM_WORK);

            work->rotF = TRUE;
            work->updateF = TRUE;
            work->time = 0;
            work->maxTime = 30;
            obj->trans.x = obj->rot.x;
            obj->trans.y = obj->rot.y;
            obj->trans.z = obj->rot.z;
            if (__fabsf(obj->rot.x - obj->scale.x) > 25.0f) {
                obj->rot.y -= -50.0f;
            } else if (__fabsf(obj->rot.z - obj->scale.z) > 25.0f) {
                obj->rot.y += -50.0f;
            }
            obj->rot.x = obj->scale.x;
            obj->rot.z = obj->scale.z;
            obj->objFunc = DiceNumObjOMExec;
        }
    }
}

BOOL mbDiceNumStopCheck(int playerNo)
{
    int i;

    for (i = 0; i < 3; i++) {
        if (diceNumOMObj[playerNo][i] != NULL) {
            DICE_NUM_WORK *work = omObjGetWork(diceNumOMObj[playerNo][i],
                DICE_NUM_WORK);

            if (work->updateF) {
                return FALSE;
            }
        }
    }
    return TRUE;
}

void mbDiceNumKill(int playerNo)
{
    int i;

    if (playerNo < 0) {
        playerNo = DICE_PLAYERNO_NULL;
    }
    for (i = 0; i < 3; i++) {
        if (diceNumOMObj[playerNo][i] != NULL) {
            DICE_NUM_WORK *work = omObjGetWork(diceNumOMObj[playerNo][i],
                DICE_NUM_WORK);

            work->killF = TRUE;
            diceNumOMObj[playerNo][i] = NULL;
        }
    }
}

void mbDiceNumShrinkSet(int playerNo)
{
    int i;

    if (playerNo < 0) {
        playerNo = DICE_PLAYERNO_NULL;
    }
    for (i = 0; i < 3; i++) {
        OMOBJ *obj = diceNumOMObj[playerNo][i];

        if (obj != NULL) {
            DICE_NUM_WORK *work = omObjGetWork(obj, DICE_NUM_WORK);

            work->time = 0;
            work->maxTime = 30;
            obj->objFunc = DiceNumObjShrinkOMExec;
        }
    }
}

void mbDiceStub(void)
{
}

OMOBJ *mbDiceSNpcNumCreate(int playerNo, HuVecF *pos)
{
    OMOBJ *snpcObj = NULL;
    int value = 0;
    int color = 0;
    BOOL createF = FALSE;
    int i;
    HuVecF objPos;
    HuVecF offset;

    if (playerNo < 0) {
        playerNo = DICE_PLAYERNO_NULL;
    }
    for (i = 0; i < 3; i++) {
        OMOBJ *obj = diceNumOMObj[playerNo][i];

        if (obj != NULL) {
            DICE_NUM_WORK *work = omObjGetWork(obj, DICE_NUM_WORK);

            value += (s8)work->value;
            if (!createF) {
                color = work->color;
                objPos = obj->rot;
                VECSubtract(&objPos, pos, &offset);
                createF = TRUE;
            }
            work->killF = TRUE;
            diceNumOMObj[playerNo][i] = NULL;
        }
    }
    if (createF) {
        snpcObj = mbDiceSNpcNumObjCreate(pos, &offset, value, FALSE, color);
    }
    return snpcObj;
}

static void ev_DiceZorome(DICE_WORK *work)
{
    int streamNo = -1;
    int coin;
    int streamId;
    HuVecF posPlayer;
    int i;
    int winNo;

    switch (work->max) {
        case 2:
            if (work->result[0] == 7) {
                coin = 30;
            } else {
                coin = 10;
            }
            streamId = 0x28;
            break;

        case 3:
            if (work->result[0] == 7) {
                coin = 50;
            } else {
                coin = 30;
            }
            streamId = 0x29;
            break;

        default:
            return;
    }
    sprintf(diceMatchCoinStr, "%d", coin);
    mbMusPauseFadeOut(0, TRUE, 1000);
    mbPlayerPosGet(work->playerNo, &posPlayer);
    for (i = 0; i < 3; i++) {
        OMOBJ *obj = diceNumOMObj[work->playerNo][i];

        if (obj != NULL) {
            DICE_NUM_WORK *numWork = omObjGetWork(obj, DICE_NUM_WORK);

            numWork->updateF = TRUE;
            numWork->bendF = TRUE;
            numWork->time = 0;
            numWork->maxTime = 120;
            obj->mdlId[2] = mbParticleCreate(HuSprAnimDataRead(
                mbBoardDataNumGet(0x00050064)), 100);
            mbParticleHookSet(obj->mdlId[2], DiceZoromeEffHook);
            Hu3DModelCameraSet(obj->mdlId[2], HU3D_CAM1);
            Hu3DModelLayerSet(obj->mdlId[2], 5);
            Hu3DModelPosSet(obj->mdlId[2], obj->rot.x,
                obj->rot.y + 30.000002f, obj->rot.z + 50.0f);
            obj->objFunc = DiceNumObjBendOMExec;
        }
    }
    HuPrcSleep(10);
    mbMusJingleWait(mbMusJinglePlay((s16)streamId));
    mbMusPauseFadeOut(0, FALSE, 1000);
    mbAudGuidePlay(0x3B6);
    winNo = mbWinCreate(2, 0x0026000A, mbGuideSpeakerNoGet());
    mbWinInsertMesSet(winNo, MESSNUM_PTR(diceMatchCoinStr), 0);
    mbWinWait(winNo);
    ev_DiceZoromeCoin(work->playerNo, coin);
    for (i = 0; i < 3; i++) {
        static GXColor biriQColor = { 0, 0, 0, 0 };
        OMOBJ *obj = diceNumOMObj[work->playerNo][i];

        if (obj != NULL) {
            DICE_NUM_WORK *numWork = omObjGetWork(obj, DICE_NUM_WORK);
            int j;

            for (j = 0; j < 2; j++) {
                if (obj->mdlId[j] > 0) {
                    mbObjBiriQColorSet(obj->mdlId[j], FALSE, 0.0f,
                        biriQColor);
                }
            }
            if (obj->mdlId[2] >= 0) {
                mbParticleKill(obj->mdlId[2]);
                obj->mdlId[2] = HU3D_MODELID_NONE;
            }
            numWork->bendF = FALSE;
        }
    }
    while (!mbDiceNumStopCheck(work->playerNo)) {
        HuPrcVSleep();
    }
    mbMusBoardPlay();
}

static void ev_DiceZoromeCoin(int playerNo, int coin)
{
    int coinObjId[64] = {};
    float velocity[64];
    HuVecF playerPos;
    HuVecF pos;
    int activeNum = 0;
    int coinNum = coin;
    int i;

    mbPlayerPosGet(playerNo, &playerPos);
    while (coinNum != 0 || activeNum != 0) {
        if (coinNum != 0) {
            for (i = 0; i < 64; i++) {
                if (coinObjId[i] == 0) {
                    break;
                }
            }
            if (i < 64) {
                coinObjId[i] = mbCoinCreate2();
                velocity[i] = -13.333334f;
                coinNum--;
                pos.x = playerPos.x +
                    (60.000004f * (frandf() - 0.5f));
                pos.y = playerPos.y + (50.0f * frandf());
                pos.z = playerPos.z +
                    (60.000004f * (frandf() - 0.5f));
                pos.y += 800.0f;
                mbCoinObjPosSetV(coinObjId[i], &pos);
                mbCoinObjRotSet(coinObjId[i],
                    40.0f * (frandf() - 0.5f), 360.0f * frandf(), 0.0f);
                mbCoinObjScaleSet(coinObjId[i], 0.7f, 0.7f, 0.7f);
            }
        }
        activeNum = 0;
        for (i = 0; i < 64; i++) {
            if (coinObjId[i] != 0) {
                activeNum++;
                mbCoinObjPosGet(coinObjId[i], &pos);
                velocity[i] += -0.5444445f;
                pos.y += velocity[i];
                mbCoinObjPosSetV(coinObjId[i], &pos);
                if (pos.y < playerPos.y + 100.0f) {
                    mbCoinObjKill(coinObjId[i]);
                    coinObjId[i] = 0;
                }
            }
        }
        HuPrcVSleep();
    }
    mbCoinAddExec(playerNo, coin);
}

static void DiceZoromeEffHook(HU3D_MODEL *modelP, MBPARTICLE *particleP,
    Mtx mtx)
{
    MBPARTICLEDATA *data;
    int i;
    int createNum = 3;
    int animBank;
    int colorNo;
    float angle;
    float angleY;
    float speed;
    float ratio;
    HuVecF vec;

    if (particleP->count == 0 || particleP->mode == 0) {
        createNum = 0.5f * particleP->num;
        particleP->count = 1;
        particleP->mode = 0;
        particleP->blendMode = 1;
        particleP->unk14 = 0.0f;
    }
    data = particleP->data;
    for (i = 0; i < particleP->num; i++, data++) {
        if (createNum <= 0) {
            break;
        }
        if (data->time == 0) {
            animBank = diceMatchEffAnimBankTbl[mbRandMod(16)];
            data->animBank = animBank;
            data->pos.x = 0.0f;
            data->pos.y = 0.0f;
            data->pos.z = 0.0f;
            data->pos.y += particleP->unk14;
            angle = 360.0f * frandf();
            ratio = (1.6f * frandf()) - 0.8f;
            angleY = 90.0f * (ratio * __fabsf(ratio));
            data->vel.x = HuSin(angle) * HuCos(angleY);
            data->vel.y = HuSin(angleY);
            data->vel.z = HuCos(angle) * HuCos(angleY);
            speed = 0.016666668f *
                (100.0f + (100.0f * (2.0f * frandf())));
            VECScale(&data->vel, &data->vel, speed);
            VECScale(&data->vel, &vec, 20.0f);
            VECAdd(&data->pos, &vec, &data->pos);
            VECScale(&data->vel, &data->vel, 0.25f);
            data->guideScaleBase = data->scale =
                diceMatchEffScale[animBank]
                * (25.0f * (0.5f + (0.5f * frandf())));
            colorNo = mbRandMod(8);
            ratio = 0.2f + (0.5f * frandf());
            data->color.r = diceMatchEffColorTbl[colorNo].r
                + ratio * (255.0f - diceMatchEffColorTbl[colorNo].r);
            data->color.g = diceMatchEffColorTbl[colorNo].g
                + ratio * (255.0f - diceMatchEffColorTbl[colorNo].g);
            data->color.b = diceMatchEffColorTbl[colorNo].b
                + ratio * (255.0f - diceMatchEffColorTbl[colorNo].b);
            data->color.a = 150 + mbRandMod(70);
            data->alphaF = data->color.a;
            data->color.a = 0;
            data->rot.z = 0.0f;
            if (data->animBank < 3) {
                data->rot.z = 360.0f * frandf();
            }
            data->time = 15 + mbRandMod(15);
            createNum--;
        }
    }
    data = particleP->data;
    for (i = 0; i < particleP->num; i++, data++) {
        if (data->time != 0) {
            switch (particleP->time) {
                case 0:
                    VECScale(&data->vel, &data->vel, 0.96f);
                    VECAdd(&data->pos, &data->vel, &data->pos);
                    data->vel.y += 0.08333334f;
                    data->scale -= 0.4f;
                    break;

                case 1:
                    VECAdd(&data->pos, &data->vel, &data->pos);
                    data->vel.y += -0.41666672f;
                    break;
            }
        }
    }
    data = particleP->data;
    for (i = 0; i < particleP->num; i++, data++) {
        if (data->time != 0) {
            data->time--;
            if (data->time < 10) {
                data->scale *= 0.7f;
                if (data->time == 0) {
                    data->color.a = 0;
                    data->scale = 0.0f;
                }
            } else {
                data->color.a += 1.0f
                    + (0.2f * (data->alphaF - data->color.a));
            }
        }
    }
}

static HU3D_MODELID DiceInEffCreate(void)
{
    int modelId;

    modelId = mbParticleCreate(HuSprAnimRead(HuDataSelHeapReadNum(
        mbBoardDataNumGet(0x00050064), HU_MEMNUM_OVL, HEAP_MODEL)), 80);
    mbParticleHookSet(modelId, DiceInEffHook);
    Hu3DModelLayerSet(modelId, 5);
    mbParticleAttrSet(modelId, MB_PARTICLE_ATTR_3D);
    mbParticleColorCreate(modelId);
    return modelId;
}

static HU3D_MODELID DiceInDotEffCreate(void)
{
    int modelId;

    modelId = mbParticleCreate(HuSprAnimRead(HuDataSelHeapReadNum(
        mbBoardDataNumGet(0x00050064), HU_MEMNUM_OVL, HEAP_MODEL)), 150);
    mbParticleHookSet(modelId, DiceInDotEffHook);
    Hu3DModelLayerSet(modelId, 5);
    return modelId;
}

static void DiceHitEffSet(HU3D_MODELID modelId)
{
    MBPARTICLE *particleP = Hu3DData[modelId].hookData;

    particleP->mode = 1;
    particleP->time = 1;
}

static void DiceInEffHook(HU3D_MODEL *modelP, MBPARTICLE *particleP,
    Mtx mtx)
{
    MBPARTICLEDATA *data;
    float rotY;
    float rotX;
    float ratio;
    float speed;
    int colorNo;
    int i;

    if (!particleP->initF || particleP->mode != 0) {
        data = particleP->data;
        switch (particleP->time) {
            case 0:
                for (i = 0; i < particleP->num; i++, data++) {
                    data->animBank = diceInEffAnimTbl[mbRandMod(4)];
                    colorNo = mbRandMod(8);
                    data->color.r = diceInEffColorTbl[colorNo].r
                        + mbRandMod(20);
                    data->color.g = diceInEffColorTbl[colorNo].g
                        + mbRandMod(20);
                    data->color.b = diceInEffColorTbl[colorNo].b
                        + mbRandMod(20);
                    data->color.a = 250;
                    data->pos.x = 0.0f;
                    data->pos.y = 0.0f;
                    data->pos.z = 0.0f;
                    rotY = 360.0f * frandf();
                    ratio = (1.6f * frandf()) - 0.8f;
                    rotX = 90.0f * (ratio * __fabsf(ratio));
                    data->vel.x = HuSin(rotY) * HuCos(rotX);
                    data->vel.y = HuSin(rotX);
                    data->vel.z = HuCos(rotY) * HuCos(rotX);
                    speed = (800.0f + (100.0f * (8.0f * frandf())))
                        / 60.0f;
                    VECScale(&data->vel, &data->vel, speed);
                    data->scale = 60.0f * (0.5f + (0.7f * frandf()))
                        * diceInEffScaleRatio[data->animBank];
                    data->accel.x = data->scale;
                    data->speedDecay = 0.5f;
                    data->rot.x = 360.0f * frandf();
                    data->rot.y = 360.0f * frandf();
                    data->scaleBase = 7.0f;
                    data->time = 20 + mbRandMod(20);
                    data->accel.y = 0.0f;
                    data->accel.z = 0.93f;
                }
                break;

            case 1:
                for (i = 0; i < particleP->num; i++, data++) {
                    data->pos.x = 0.0f;
                    data->pos.y = 50.0f;
                    data->pos.z = 0.0f;
                    rotY = 360.0f * frandf();
                    ratio = 0.7f + (0.3f * frandf());
                    rotX = 90.0f * (ratio * __fabsf(ratio));
                    data->vel.x = HuSin(rotY) * HuCos(rotX);
                    data->vel.y = HuSin(rotX);
                    data->vel.z = HuCos(rotY) * HuCos(rotX);
                    speed = (300.0f + (100.0f * (3.0f * frandf())))
                        / 60.0f;
                    VECScale(&data->vel, &data->vel, speed);
                    data->vel.y *= 1.5f;
                    data->scale = 0.7f * data->accel.x;
                    data->speedDecay = 0.3f;
                    if (data->color.r < 150) {
                        data->color.r -= 30;
                    }
                    if (data->color.g < 150) {
                        data->color.g -= 30;
                    }
                    if (data->color.b < 150) {
                        data->color.b -= 30;
                    }
                    data->color.a = 255;
                    data->time = 20 + mbRandMod(20);
                    data->accel.y = -0.66666675f;
                    data->accel.z = 1.0f;
                }
                break;
        }
        particleP->initF = TRUE;
        particleP->mode = 0;
    }
    data = particleP->data;
    for (i = 0; i < particleP->num; i++, data++) {
        if (data->time != 0) {
            VECScale(&data->vel, &data->vel, data->accel.z);
            VECAdd(&data->pos, &data->vel, &data->pos);
            data->vel.y += data->accel.y;
            data->scale += data->speedDecay;
            data->rot.z += data->scaleBase;
            data->time--;
            if (data->time < 10) {
                data->color.a *= 0.8f;
                if (data->time == 0) {
                    data->color.a = 0;
                    data->scale = 0.0f;
                }
            }
        }
    }
}

static void DiceInDotEffHook(HU3D_MODEL *modelP, MBPARTICLE *particleP,
    Mtx mtx)
{
    MBPARTICLEDATA *data;
    float rotY;
    float rotX;
    float ratio;
    float speed;
    int colorNo;
    int i;

    if (!particleP->initF || particleP->mode != 0) {
        data = particleP->data;
        switch (particleP->time) {
            case 0:
                for (i = 0; i < particleP->num; i++, data++) {
                    data->animBank = 3;
                    data->pos.x = 0.0f;
                    data->pos.y = 0.0f;
                    data->pos.z = 0.0f;
                    rotY = 360.0f * frandf();
                    ratio = (1.6f * frandf()) - 0.8f;
                    rotX = 90.0f * (ratio * __fabsf(ratio));
                    data->vel.x = HuSin(rotY) * HuCos(rotX);
                    data->vel.y = HuSin(rotX);
                    data->vel.z = HuCos(rotY) * HuCos(rotX);
                    speed = (500.0f + (100.0f * (9.0f * frandf())))
                        / 60.0f;
                    VECScale(&data->vel, &data->vel, speed);
                    data->scale = 60.0f * (0.5f + (0.7f * frandf()))
                        * diceInDotEffScaleRatio[data->animBank];
                    data->accel.x = data->scale;
                    colorNo = mbRandMod(8);
                    ratio = 0.2f + (0.4f * frandf());
                    data->color.r = diceInDotEffColorTbl[colorNo].r
                        + ratio * (255.0f - diceInDotEffColorTbl[colorNo].r);
                    data->color.g = diceInDotEffColorTbl[colorNo].g
                        + ratio * (255.0f - diceInDotEffColorTbl[colorNo].g);
                    data->color.b = diceInDotEffColorTbl[colorNo].b
                        + ratio * (255.0f - diceInDotEffColorTbl[colorNo].b);
                    data->color.a = 100 + mbRandMod(50);
                    data->time = 20 + mbRandMod(20);
                }
                break;

            case 1:
                for (i = 0; i < particleP->num; i++, data++) {
                    data->pos.x = 0.0f;
                    data->pos.y = 50.0f;
                    data->pos.z = 0.0f;
                    rotY = 360.0f * frandf();
                    ratio = 0.7f + (0.3f * frandf());
                    rotX = 90.0f * (ratio * __fabsf(ratio));
                    data->vel.x = HuSin(rotY) * HuCos(rotX);
                    data->vel.y = HuSin(rotX);
                    data->vel.z = HuCos(rotY) * HuCos(rotX);
                    speed = (300.0f + (100.0f * (3.0f * frandf())))
                        / 60.0f;
                    VECScale(&data->vel, &data->vel, speed);
                    data->vel.y *= 1.5f;
                    data->scale = 0.8f * data->accel.x;
                    data->color.a = 120 + mbRandMod(50);
                    data->time = 20 + mbRandMod(10);
                }
                break;
        }
        particleP->initF = TRUE;
        particleP->mode = 0;
        particleP->blendMode = MB_PARTICLE_BLEND_ADDCOL;
    }
    data = particleP->data;
    for (i = 0; i < particleP->num; i++, data++) {
        if (data->time != 0) {
            switch (particleP->time) {
                case 0:
                    VECScale(&data->vel, &data->vel, 0.92f);
                    VECAdd(&data->pos, &data->vel, &data->pos);
                    data->vel.y += -0.22222224f;
                    data->scale -= 0.15f;
                    break;

                case 1:
                    VECAdd(&data->pos, &data->vel, &data->pos);
                    data->vel.y += -0.41666672f;
                    break;
            }
            data->time--;
            if (data->time < 6) {
                data->color.a *= 0.7f;
                if (data->time == 0) {
                    data->color.a = 0;
                    data->scale = 0.0f;
                }
            }
        }
    }
}

static float diceInEffRotYRatio[3] = {
    1.0f / 1.0f, 1.0f / 2.0f, 1.0f / 3.0f
};

static void DiceObjEffCreate(DICE_WORK *work)
{
    int hitOrderTbl[20] = {
        0, 0, 0, 0, 0, 0,
        1, 1, 1, 1, 1, 1, 1, 1,
        2, 2, 2, 2, 2, 2
    };
    float posY;
    DICE_EFF *eff;
    void *dlBuf;
    int i;

    work->eff = eff = HuMemDirectMallocNum(HEAP_HEAP,
        (DICE_EFF_PUFF_MAX + DICE_EFF_TRI_MAX) * sizeof(DICE_EFF),
        HU_MEMNUM_OVL);
    memset(eff, 0,
        (DICE_EFF_PUFF_MAX + DICE_EFF_TRI_MAX) * sizeof(DICE_EFF));
    for (i = 0; i < DICE_EFF_PUFF_MAX; i++, eff++) {
        eff->rot.x = (frandf() * 15.0f) + 30.0f;
        eff->rot.y = frandf() * 360.0f;
        eff->radius = (1.0f + frandf()) * 100.0f;
    }
    for (i = 0; i < 100; i++) {
        u32 idx1;
        u32 idx2;
        int temp;

        idx1 = mbRandMod(DICE_EFF_TRI_MAX);
        idx2 = mbRandMod(DICE_EFF_TRI_MAX);
        temp = hitOrderTbl[idx1];
        hitOrderTbl[idx1] = hitOrderTbl[idx2];
        hitOrderTbl[idx2] = temp;
    }
    posY = 0.0f;
    for (i = 0; i < DICE_EFF_TRI_MAX; i++, eff++) {
        eff->rot.x = 0.0f;
        eff->rot.y = posY;
        eff->radius = 150.0f;
        eff->scale.x = diceInEffRotYRatio[hitOrderTbl[i]];
        eff->scale.y = 1.0f;
        eff->scale.z = 1.0f + (2.0f * frandf());
        posY += (360.0f * eff->scale.x) / 12.0f;
    }
    dlBuf = HuMemDirectMallocNum(HEAP_HEAP, 4096, HU_MEMNUM_OVL);
    DCInvalidateRange(dlBuf, 4096);
    GXBeginDisplayList(dlBuf, 4096);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3f32(-20.0f, 0.0f, -20.0f);
    GXTexCoord2f32(0.0f, 0.0f);
    GXPosition3f32(20.0f, 0.0f, -20.0f);
    GXTexCoord2f32(1.0f, 0.0f);
    GXPosition3f32(20.0f, 0.0f, 20.0f);
    GXTexCoord2f32(1.0f, 1.0f);
    GXPosition3f32(-20.0f, 0.0f, 20.0f);
    GXTexCoord2f32(0.0f, 1.0f);
    GXEnd();
    work->dlSize = GXEndDisplayList();
    work->dlBuf = HuMemDirectMallocNum(HEAP_HEAP, work->dlSize,
        HU_MEMNUM_OVL);
    memcpy(work->dlBuf, dlBuf, work->dlSize);
    HuMemDirectFree(dlBuf);
    DCFlushRangeNoSync(work->dlBuf, work->dlSize);
    work->animEffPuff = HuSprAnimDataRead(
        mbBoardDataNumGet(0x0005005D));
    work->animEffTri = HuSprAnimDataRead(
        mbBoardDataNumGet(0x00050062));
}

static void DiceObjEffKill(DICE_WORK *work)
{
    if (work->dlBuf != NULL) {
        HuMemDirectFree(work->dlBuf);
        work->dlBuf = NULL;
    }
    if (work->eff != NULL) {
        HuMemDirectFree(work->eff);
        work->eff = NULL;
    }
    if (work->animEffPuff != NULL) {
        HuSprAnimKill(work->animEffPuff);
        work->animEffPuff = NULL;
    }
    if (work->animEffTri != NULL) {
        HuSprAnimKill(work->animEffTri);
        work->animEffTri = NULL;
    }
}

static void DiceObjEffPuffDraw(HU3D_MODEL *modelP, Mtx *mtx)
{
    int i;
    OMOBJ *obj = modelP->hookData;
    DICE_WORK *work = obj->data;
    GXColor color = { 0, 0, 0, 255 };
    DICE_EFF *eff;
    float time = work->puffTime / 45.0f;

    if (time > 1.0f) {
        time = 1.0f;
    }
    HuSprTexLoad(work->animEffPuff, 0, GX_TEXMAP0, GX_REPEAT, GX_REPEAT,
        GX_NEAR);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA,
        GX_LO_NOOP);
    GXSetZMode(GX_FALSE, GX_LEQUAL, GX_FALSE);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG,
        GX_LIGHT_NULL, GX_DF_CLAMP, GX_AF_NONE);
    GXSetNumTexGens(1);
    GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0,
        GX_IDENTITY);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO,
        GX_CC_ONE);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
        GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_TEXA, GX_CA_A0,
        GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
        GX_TRUE, GX_TEVPREV);
    color.a = (1.0f - time) * 255.0f;
    GXSetTevColor(GX_TEVREG0, color);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    GXSetCullMode(GX_CULL_NONE);
    for (eff = work->eff, i = 0; i < DICE_EFF_PUFF_MAX; i++, eff++) {
        Mtx modelview;
        Mtx rot;
        Mtx trans;
        HuVecF pos;
        static HuVecF back = { 0, 0, -1 };

        mtxRot(rot, eff->rot.x, eff->rot.y, eff->rot.z);
        MTXMultVec(rot, &back, &pos);
        VECScale(&pos, &pos, HuSin(time * 90.0f) * eff->radius);
        MTXTrans(trans, obj->trans.x + pos.x, obj->trans.y + pos.y + 50.0f,
            obj->trans.z + pos.z);
        MTXConcat(trans, rot, modelview);
        MTXConcat(*mtx, modelview, modelview);
        GXLoadPosMtxImm(modelview, GX_PNMTX0);
        GXCallDisplayList(work->dlBuf, work->dlSize);
    }
    if (!mbPauseEnableCheck()) {
        if (++work->puffTime >= 45u) {
            modelP->attr |= HU3D_ATTR_DISPOFF;
        }
    }
}

static void DiceObjEffTriDraw(HU3D_MODEL *modelP, Mtx *mtx)
{
    int i;
    OMOBJ *obj = modelP->hookData;
    DICE_WORK *work = obj->data;
    GXColor color = { 255, 192, 64, 255 };
    float time = work->triTime / 16.0f;
    float alpha;
    DICE_EFF *eff;

    if (time > 1.0f) {
        time = 1.0f;
    }
    HuSprTexLoad(work->animEffTri, 0, GX_TEXMAP0, GX_REPEAT, GX_REPEAT,
        GX_NEAR);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA,
        GX_LO_NOOP);
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG,
        GX_LIGHT_NULL, GX_DF_CLAMP, GX_AF_NONE);
    alpha = 255.0f * (4.0f * (1.0f - time));
    if (alpha > 255.0f) {
        alpha = 255.0f;
    }
    color.a = alpha;
    GXSetChanMatColor(GX_COLOR0A0, color);
    GXSetNumTexGens(1);
    GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0,
        GX_IDENTITY);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO,
        GX_CC_RASC);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
        GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_TEXA, GX_CA_RASA,
        GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
        GX_TRUE, GX_TEVPREV);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    GXSetCullMode(GX_CULL_BACK);
    for (eff = work->eff + DICE_EFF_PUFF_MAX, i = 0;
        i < DICE_EFF_TRI_MAX; i++, eff++) {
        Mtx modelview;
        Mtx trans;
        Mtx rot;
        Mtx scale;
        HuVecF pos;
        static HuVecF back = { 0, 0, -1 };

        MTXScale(scale, (2.0f * eff->scale.x) + time,
            2.0f * eff->scale.y, 2.0f * eff->scale.z);
        mtxRot(rot, eff->rot.x, eff->rot.y, eff->rot.z);
        MTXMultVec(rot, &back, &pos);
        VECScale(&pos, &pos, HuSin(time * 90.0f) * eff->radius);
        MTXTrans(trans, obj->trans.x + pos.x, obj->trans.y + pos.y - 50.0f,
            obj->trans.z + pos.z);
        MTXConcat(rot, scale, modelview);
        MTXConcat(trans, modelview, modelview);
        MTXConcat(*mtx, modelview, modelview);
        GXLoadPosMtxImm(modelview, GX_PNMTX0);
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        GXPosition3f32(-20.0f, 0.0f, -30.0f);
        GXTexCoord2f32(0.0f, 0.0f);
        GXPosition3f32(20.0f, 0.0f, -30.0f);
        GXTexCoord2f32(1.0f, 0.0f);
        GXPosition3f32(20.0f, 0.0f, 10.0f);
        GXTexCoord2f32(1.0f, 1.0f);
        GXPosition3f32(-20.0f, 0.0f, 10.0f);
        GXTexCoord2f32(0.0f, 1.0f);
        GXEnd();
    }
    if (!mbPauseEnableCheck()) {
        if (++work->triTime >= 16) {
            modelP->attr |= HU3D_ATTR_DISPOFF;
        }
    }
}

static DICE_NUM_VTX *DiceNumObjMdlCopy(int modelId)
{
    HU3D_MODEL *model = &Hu3DData[modelId];
    HSF_DATA *hsf = model->hsf;
    HSF_OBJECT *obj = hsf->object;
    HuVecF *pos;
    DICE_NUM_VTX *vtx;
    int size;
    DICE_NUM_VTX *vtxBuf;
    int i, j;
    float posMin;
    float posMax;
    float sizeY;

    for (i = 0; i < hsf->objectNum; i++, obj++) {
        if (obj->type == HSF_OBJ_MESH) {
            size = obj->mesh.vertex->count * sizeof(DICE_NUM_VTX);
            pos = obj->mesh.vertex->data;
            posMin = posMax = pos->y;
            pos++;
            for (j = 1; j < obj->mesh.vertex->count; j++, pos++) {
                if (pos->y > posMax) {
                    posMax = pos->y;
                }
                if (pos->y < posMin) {
                    posMin = pos->y;
                }
            }
            sizeY = posMax - posMin;
            vtxBuf = vtx = HuMemDirectMallocNum(HEAP_MODEL, size,
                model->mallocNo);
            pos = obj->mesh.vertex->data;
            for (j = 0; j < obj->mesh.vertex->count; j++, pos++, vtx++) {
                vtx->pos = *pos;
                vtx->weight = (pos->y - posMin) / sizeY;
            }
            break;
        }
    }
    return vtxBuf;
}

static void DiceNumObjMdlBend(int modelId, DICE_NUM_VTX *vtx, Mtx mtx1,
    Mtx mtx2)
{
    HU3D_MODEL *model = &Hu3DData[modelId];
    HSF_DATA *hsf = model->hsf;
    int i, j;
    HSF_OBJECT *obj = hsf->object;

    for (i = 0; i < hsf->objectNum; i++, obj++) {
        if (obj->type == HSF_OBJ_MESH) {
            HuVecF *pos;

            pos = obj->mesh.vertex->data;
            for (j = 0; j < obj->mesh.vertex->count;
                j++, vtx++, pos++) {
                HuVecF vtx1, vtx2;

                MTXMultVec(mtx1, &vtx->pos, &vtx1);
                MTXMultVec(mtx2, &vtx->pos, &vtx2);
                pos->x = vtx1.x + (vtx->weight * (vtx2.x - vtx1.x));
                pos->y = vtx1.y + (vtx->weight * (vtx2.y - vtx1.y));
                pos->z = vtx1.z + (vtx->weight * (vtx2.z - vtx1.z));
            }
            DCStoreRangeNoSync(obj->mesh.vertex->data,
                obj->mesh.vertex->count * sizeof(HuVecF));
            break;
        }
    }
}

OMOBJ *mbDiceSNpcNumObjCreate(HuVecF *pos, HuVecF *offset, int value,
    BOOL flagF, int color)
{
    int modelId;
    OMOBJ *obj;
    DICE_SNPC_NUM_WORK *work;
    int i;
    HuVecF posSum;
    HuVecF posNorm;

    obj = omAddObjEx(mbObjMan, 0x7E02, 20, 0, OM_GRP_NONE,
        DiceSNpcNumUpdate);
    omSetStatBit(obj, OM_STAT_MODELPAUSE);
    work = omObjGetWork(obj, DICE_SNPC_NUM_WORK);
    work->dispF = TRUE;
    work->killF = FALSE;
    work->_unk2 = flagF;
    work->value = value;
    for (i = 0; i < 20; i++) {
        modelId = mbCoinObjCreate(i % 10, color);
        mbCoinObjLayerSet(modelId, 4);
        mbCoinObjDispSet(modelId, FALSE);
        obj->mdlId[i] = modelId;
    }
    HuAddVecF(&posSum, pos, offset);
    mbPos3DtoNorm(&posSum, 1, &posNorm);
    obj->rot.z = posNorm.z;
    obj->trans = *pos;
    obj->scale = *offset;
    return obj;
}

static void DiceSNpcNumUpdate(OMOBJ *obj)
{
    int digitNum = 0;
    DICE_SNPC_NUM_WORK *work = omObjGetWork(obj, DICE_SNPC_NUM_WORK);
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

    if (work->killF || mbExitCheck()) {
        for (i = 0; i < 20; i++) {
            if (obj->mdlId[i] != -1) {
                mbCoinObjNumDec(obj->mdlId[i]);
            }
        }
        omDelObjEx(HuPrcCurrentGet(), obj);
        return;
    }
    if (mbPauseEnableCheck()) {
        return;
    }
    HuAddVecF(&pos, &obj->trans, &obj->scale);
    mbPos3DtoNorm(&pos, 1, &posNorm);
    posNorm.z = obj->rot.z;
    tanFov = HuSin(camera2P->fov * 0.5f)
        / HuCos(camera2P->fov * 0.5f);
    scaleX = 1.2f * (tanFov * -posNorm.z);
    scaleY = tanFov * -posNorm.z;
    posNorm.x *= scaleX;
    posNorm.y *= scaleY;
    pos = posNorm;
    mbCameraRotGet(&posNorm);
    rotZ = -posNorm.x;
    for (i = 0; i < 20; i++) {
        mbCoinObjDispSet(obj->mdlId[i], FALSE);
    }
    if (work->dispF) {
        int modelNo;

        scale = HuSin(cameraP->fov * 0.5f)
            / HuCos(cameraP->fov * 0.5f);
        scale = tanFov / scale;
        modelNo = work->value / 10;
        if (modelNo != 0) {
            mbCoinObjDispSet(obj->mdlId[modelNo], TRUE);
            mbCoinObjPosSet(obj->mdlId[modelNo],
                pos.x - (60.000004f * scale), pos.y, pos.z);
            mbCoinObjRotSet(obj->mdlId[modelNo], rotZ, 0.0f, 0.0f);
            mbCoinObjScaleSet(obj->mdlId[modelNo], scale, scale, scale);
            digitNum++;
        }
        modelNo = (work->value % 10) + 10;
        if (modelNo != 0) {
            mbCoinObjDispSet(obj->mdlId[modelNo], TRUE);
            if (digitNum == 0) {
                mbCoinObjPosSet(obj->mdlId[modelNo], pos.x, pos.y, pos.z);
            } else {
                mbCoinObjPosSet(obj->mdlId[modelNo],
                    pos.x + (60.000004f * scale), pos.y, pos.z);
            }
            mbCoinObjRotSet(obj->mdlId[modelNo], rotZ, 0.0f, 0.0f);
            mbCoinObjScaleSet(obj->mdlId[modelNo], scale, scale, scale);
        }
    }
}

void mbDiceSNpcNumKill(OMOBJ *obj)
{
    omObjGetWork(obj, DICE_SNPC_NUM_WORK)->killF = TRUE;
}

void mbDiceSNpcNumDispSet(OMOBJ *obj, BOOL dispF)
{
    omObjGetWork(obj, DICE_SNPC_NUM_WORK)->dispF = dispF;
}

void mbDiceSNpcNumSet(OMOBJ *obj, u8 value)
{
    omObjGetWork(obj, DICE_SNPC_NUM_WORK)->value = (u8)value;
}

void mbDiceSNpcNumPosSet(OMOBJ *obj, HuVecF *pos)
{
    obj->trans = *pos;
}

void mbDiceSNpcNumOfsSet(OMOBJ *obj, HuVecF *offset)
{
    obj->scale = *offset;
}

void mbDiceSNpcNumOfsGet(OMOBJ *obj, HuVecF *offset)
{
    *offset = obj->scale;
}
