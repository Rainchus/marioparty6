#include "game/board/main.h"
#include "game/board/pause.h"
#include "game/board/player.h"
#include "game/memory.h"

#include "string.h"

typedef void (*MBSHOPOBJHOOK)(int modelId, int shopNo);

typedef struct MBSHOPWORK {
    int playerNo;
    int shopNo;
} MBSHOPWORK;

void mbev_ShopCreate(int dataNum, int motDataNum);
void mbev_ShopBackMotCreate(int dataNum, int motDataNum, int motNo, BOOL linkF, int hookModelId);
void ev_Shop(MBSHOPWORK *work);

static BOOL ev_ShopEnableF;
static MBSHOPOBJHOOK ev_ShopExObjHook;

void mbev_ShopEnableSet(BOOL enableF)
{
    ev_ShopEnableF = enableF;
}

void mbev_ShopExObjHookSet(MBSHOPOBJHOOK hook)
{
    ev_ShopExObjHook = hook;
}

void mbev_ShopInit(int dataNum)
{
    mbev_ShopExObjHookSet(NULL);
    mbev_ShopCreate(dataNum, -1);
}

void mbev_ShopExInit(int dataNum, MBSHOPOBJHOOK hook)
{
    mbev_ShopExObjHookSet(hook);
    mbev_ShopCreate(dataNum, -1);
}

void mbev_ShopBackCreate(int dataNum, int motDataNum, int motNo, BOOL linkF)
{
    mbev_ShopBackMotCreate(dataNum, motDataNum, motNo, linkF, 0);
}

int mbev_Shop(int playerNo, int shopNo)
{
    MBSHOPWORK *work;

    mbMoveNumDispSet(playerNo, FALSE);
    work = HuMemDirectMallocNum(HEAP_HEAP, sizeof(MBSHOPWORK), HU_MEMNUM_OVL);
    memset(work, 0, sizeof(MBSHOPWORK));
    work->playerNo = playerNo;
    work->shopNo = shopNo;
    mbPauseDisableSet(TRUE);
    ev_Shop(work);
    HuMemDirectFree(work);
    mbPauseDisableSet(FALSE);
    mbMoveNumDispSet(playerNo, TRUE);
    return 0;
}
