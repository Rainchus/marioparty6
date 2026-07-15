#include "game/gamework_enum.h"
#include "game/gamework.h"
#include "game/object.h"
#include "game/process.h"

#define DICE_PLAYERNO_NULL GW_PLAYER_MAX
#define DICE_MAX DICE_PLAYERNO_NULL+1

typedef void (*DICEHITHOOK)(int result);
typedef u16 (*DICEPADBTNHOOK)(int playerNo);
typedef void (*DICEMOTHOOK)(int playerNo);

typedef struct DiceSNpcNumWork_s {
    u8 killF : 1;
    u8 dispF : 1;
    u8 value;
} DICE_SNPC_NUM_WORK;

static s8 diceMaxTbl[] = {
    1, 2, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 3
};

static s8 diceValueMaxTbl[] = {
    10, 10, 10, 5, 10, 1, 1, 7, 0, 0, 1, 5, 5, 6, 10, 10, 10, 10, 10, 10, 10
};

static s8 diceSingleValueMaxTbl[] = {
    6, 6, 6, 3, 10, 1, 1, 7, 0, 0, 1, 5, 5, 6, 6, 10, 10, 10, 10, 10, 10
};

static int diceResult[DICE_MAX];
static HUPROCESS *diceProc[DICE_MAX];

static DICEHITHOOK diceHitHook[DICE_MAX] = {};
static DICEPADBTNHOOK dicePadBtnHook[DICE_MAX] = {};
static DICEMOTHOOK diceMotHook[DICE_MAX] = {};

void mbDiceStub(void)
{
}

BOOL mbDiceKillCheck(int playerNo)
{
    if (playerNo < 0) {
        playerNo = DICE_PLAYERNO_NULL;
    }
    return diceProc[playerNo] == NULL;
}

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

int mbDiceMaxGet(int type)
{
    return diceMaxTbl[type];
}

int mbDiceValueMaxGet(int type)
{
    if (!GWPartyGet()) {
        return diceSingleValueMaxTbl[type];
    }
    return diceValueMaxTbl[type];
}

void mbDiceSNpcNumKill(OMOBJ *obj)
{
    DICE_SNPC_NUM_WORK *work = (DICE_SNPC_NUM_WORK *)obj->work;

    work->killF = TRUE;
}

void mbDiceSNpcNumDispSet(OMOBJ *obj, BOOL dispF)
{
    DICE_SNPC_NUM_WORK *work = (DICE_SNPC_NUM_WORK *)obj->work;

    work->dispF = dispF;
}

void mbDiceSNpcNumSet(OMOBJ *obj, u8 value)
{
    DICE_SNPC_NUM_WORK *work = (DICE_SNPC_NUM_WORK *)obj->work;

    work->value = value;
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
