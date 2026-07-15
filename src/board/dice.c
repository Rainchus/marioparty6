#include "game/board/main.h"
#include "game/board/object.h"
#include "game/board/player.h"

#include "game/hu3d.h"
#include "game/memory.h"
#include "game/pad.h"
#include "game/sprite.h"

#include "humath.h"
#include "string.h"

#define DICE_PLAYERNO_NULL GW_PLAYER_MAX
#define DICE_MAX DICE_PLAYERNO_NULL+1
#define DICE_VALUENUM_MAX 11

#define DICETYPE_CHANCE_TRADE 8
#define DICETYPE_CHANCE_CHAR 9

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

static void DiceProcMain(void);
static void DiceProcDestroy(void);
static u16 DicePlayerPadBtn(int playerNo);
static void DiceObjHit(DICE_WORK *work);
static void DiceObjKillSet(OMOBJ *obj);
static void DiceKill(DICE_WORK *work);
static void DiceReadyWait(DICE_WORK *work);
static void DiceNumObjOMExec(OMOBJ *obj);
static void DiceNumObjBendOMExec(OMOBJ *obj);
static void DiceNumObjShrinkOMExec(OMOBJ *obj);
static DICE_NUM_VTX *DiceNumObjMdlCopy(int modelId);
static void DiceNumObjMdlBend(int modelId, DICE_NUM_VTX *vtx, Mtx mtx1,
    Mtx mtx2);

void mbObjBiriQColorSet(MBMODELID modelId, BOOL setF, GXColor color,
    float alpha);
float mbSinDeg(float angle);

BOOL mbDiceKillCheck(int playerNo);
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

static void DiceObjKillSet(OMOBJ *obj)
{
    omObjGetWork(obj, DICE_OBJ_WORK)->killF = TRUE;
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

                    mbObjBiriQColorSet(obj->mdlId[i], TRUE, biriQColor,
                        alpha);
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
            mbObjBiriQColorSet(obj->mdlId[i], TRUE, biriQColor, alpha);
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
    HuCopyVecF(&obj->trans, pos);
}

void mbDiceSNpcNumOfsSet(OMOBJ *obj, HuVecF *offset)
{
    HuCopyVecF(&obj->scale, offset);
}

void mbDiceSNpcNumOfsGet(OMOBJ *obj, HuVecF *offset)
{
    HuCopyVecF(offset, &obj->scale);
}
