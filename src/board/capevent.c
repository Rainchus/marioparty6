#include "game/gamework.h"

#define CAPSULE_KOOPA 43
#define CAPSULE_INVALID -99

typedef void (*CAPSULE_HOOK)(int, int, int, BOOL, BOOL, BOOL);

static int kettouCoinLose = 10;
static int kettouOppCoinLose = 5;
static int capsuleEventMasu = -1;
static int capsuleEventPlayer = -1;
static int capsuleEventPrevMasu = -1;
static int capsuleEventPrevPlayer = -1;
static CAPSULE_HOOK capsuleHook;

void mbev_CapMoveMasuSet(int playerNo, int masuId)
{
    capsuleEventPlayer = playerNo;
    capsuleEventMasu = masuId;
}

void mbev_CapStopMasuSet(int playerNo, int masuId)
{
    capsuleEventPrevPlayer = playerNo;
    capsuleEventPrevMasu = masuId;
}

int mbev_CapBankCoinGet(void)
{
    return GwSystem.bankCoin;
}

int mbev_CapKettouCoinLoseGet(void)
{
    return kettouCoinLose;
}

int mbev_CapKettouOppCoinLoseGet(void)
{
    return kettouOppCoinLose;
}

int mbev_CapKettouCoinLoseGet2(void)
{
    return kettouCoinLose;
}

int mbev_CapKettouOppCoinLoseGet2(void)
{
    return kettouOppCoinLose;
}

void mbev_CapBubbleHookSet(CAPSULE_HOOK hook)
{
    capsuleHook = hook;
}

void mbev_CapBubbleHookCall(int type, int modelId, BOOL flag1, BOOL flag2, BOOL flag3)
{
    if (capsuleHook != NULL) {
        capsuleHook(-1, type, modelId, flag1, flag2, flag3);
    }
}

void mbev_CapBubbleHookCallStory(int eventType, int type, int modelId)
{
    if (capsuleHook != NULL) {
        switch (eventType) {
            case 0:
                capsuleHook(CAPSULE_KOOPA, type, modelId, FALSE, FALSE, FALSE);
                break;
            case 1:
                capsuleHook(CAPSULE_INVALID, type, modelId, FALSE, FALSE, FALSE);
                break;
        }
    }
}

void mbev_CapBankCoinInit(void)
{
    GwSystem.bankCoin = 0;
}
