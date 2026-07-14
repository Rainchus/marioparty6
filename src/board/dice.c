#include "game/gamework_enum.h"
#include "game/process.h"

#define DICE_PLAYERNO_NULL GW_PLAYER_MAX
#define DICE_MAX DICE_PLAYERNO_NULL+1

typedef void (*DICEHITHOOK)(int result);
typedef u16 (*DICEPADBTNHOOK)(int playerNo);
typedef void (*DICEMOTHOOK)(int playerNo);

static int diceResult[DICE_MAX];
static HUPROCESS *diceProc[DICE_MAX];

static DICEHITHOOK diceHitHook[DICE_MAX] = {};
static DICEPADBTNHOOK dicePadBtnHook[DICE_MAX] = {};
static DICEMOTHOOK diceMotHook[DICE_MAX] = {};

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
