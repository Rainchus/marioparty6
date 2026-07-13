/* player.o has no weak sqrtf constants in .sdata2. */
#define _MATH_H
#include "dolphin/math.h"

#include "game/board/object.h"
#include "game/board/player.h"

static BOOL blackoutF;
static void (*turnInitHook)(int playerNo);
static void (*turnCloseHook)(int playerNo);

void mbPlayerTurnInitHookSet(void (*hook)(int playerNo))
{
    turnInitHook = hook;
}

void mbPlayerTurnCloseHookSet(void (*hook)(int playerNo))
{
    turnCloseHook = hook;
}

void mbPlayerAmbSet(int playerNo, float ambR, float ambG, float ambB)
{
    mbObjAmbSet(mbPlayerObjIDGet(playerNo), ambR, ambG, ambB);
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

float mbPlayerWalkSpeedGet(void)
{
    return 20;
}

void mbPlayerStubValSet(int playerNo, BOOL value)
{
}

void mbPlayerBlackoutSet(BOOL value)
{
    blackoutF = value;
}

BOOL mbPlayerBlackoutGet(void)
{
    return blackoutF;
}
