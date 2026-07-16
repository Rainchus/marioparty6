#include "game/board/main.h"

#include "game/board/masu.h"

extern s8 mbPlayerCapsuleGet(int playerNo, int capsuleNo);
extern int mbPlayerCapsuleNumGet(int playerNo);

static HUPROCESS *ev_CapSelectShrinkProc[4] = { NULL, NULL, NULL, NULL };

static int ev_CapSelectObjId[4];
static int ev_CapSelectResult[4];
static int ev_CapSelectExtra[4];
static OMOBJ *ev_CapMasuOMObj[16];

static BOOL ev_CapSelectStoryF;
static BOOL ev_CapMasuDispF;
static int ev_CapSelectMdlId;
static int ev_CapSelectValue;

void mbCapMasuObjCreateAll(void);
void mbCapMasuObjCreate(int masuId);

void mbCapSelectResultSet(int playerNo, int objId, int result)
{
    ev_CapSelectObjId[playerNo] = objId;
    ev_CapSelectResult[playerNo] = result;
}

void mbCapSelectResultGet(int playerNo, int *objId, int *result)
{
    if (objId != NULL) {
        *objId = ev_CapSelectObjId[playerNo];
    }
    if (result != NULL) {
        *result = ev_CapSelectResult[playerNo];
    }
}

void mbCapSelectResultReset(int playerNo)
{
    ev_CapSelectObjId[playerNo] = -1;
    ev_CapSelectResult[playerNo] = -1;
}

BOOL mbCapSelectShrinkCheck(int playerNo)
{
    if (ev_CapSelectShrinkProc[playerNo] == NULL) {
        return TRUE;
    }
    return FALSE;
}

static void CapSelectStoryFSet(BOOL storyF)
{
    ev_CapSelectStoryF = storyF;
}

static void CapSelectExtraCapsuleGet(int playerNo, int capsuleNo)
{
    ev_CapSelectExtra[playerNo] = capsuleNo;
}

static int CapSelectCapsuleGet(int playerNo, int selectNo)
{
    if (ev_CapSelectStoryF) {
        if (selectNo == 0) {
            return 47;
        }
        if (selectNo == 1) {
            return 48;
        }
        return mbPlayerCapsuleGet(playerNo, selectNo - 2);
    }
    if (selectNo == mbPlayerCapsuleNumGet(playerNo)) {
        return ev_CapSelectExtra[playerNo];
    }
    return mbPlayerCapsuleGet(playerNo, selectNo);
}

static int CapSelectNumGet(int playerNo)
{
    if (ev_CapSelectStoryF) {
        return mbPlayerCapsuleNumGet(playerNo) + 2;
    }
    if (ev_CapSelectExtra[playerNo] >= 0) {
        return mbPlayerCapsuleNumGet(playerNo) + 1;
    }
    return mbPlayerCapsuleNumGet(playerNo);
}

void fn_8019A618(void)
{
}

void fn_8019A61C(void)
{
}

void fn_8019A620(void)
{
}

void fn_8019A624(void)
{
}

void fn_8019A628(void)
{
}

void fn_8019A62C(void)
{
}

void mbCapMasuObjInit(void)
{
    int i;

    for (i = 0; i < 16; i++) {
        ev_CapMasuOMObj[i] = NULL;
    }
    mbCapMasuObjCreateAll();
    ev_CapMasuDispF = TRUE;
}

void mbCapMasuObjClose(void)
{
    int i;

    for (i = 0; i < 16; i++) {
        ev_CapMasuOMObj[i] = NULL;
    }
}

void mbCapMasuObjCreateAll(void)
{
    int masuId;

    for (masuId = 1; masuId <= mbMasuRawNumGet(); masuId++) {
        if (mbMasuTypeGet(masuId) == 8) {
            mbCapMasuObjCreate(masuId);
        }
    }
}

void mbCapMasuDispSet(BOOL dispF)
{
    ev_CapMasuDispF = dispF;
}
