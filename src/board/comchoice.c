#include "game/board/comchoice.h"
#include "game/board/window.h"
#include "game/gamework.h"

static s32 listDownNum;
static s32 listRightNum;

static void ComChoiceKeySet(s32 playerNo, s32 leftRight, s32 upDown);
static void ComChoiceListKeySet(s32 playerNo, s32 rightNum, s32 downNum);

static void ComChoiceLeft(void)
{
    ComChoiceKeySet(GwSystem.turnPlayerNo, 0, 1);
}

void mbComChoiceLeftSet(void)
{
    mbWinTopComKeyHookSet(ComChoiceLeft);
}

static void ComChoiceRight(void)
{
    ComChoiceKeySet(GwSystem.turnPlayerNo, 1, 1);
}

void mbComChoiceRightSet(void)
{
    mbWinTopComKeyHookSet(ComChoiceRight);
}

static void ComChoiceUp(void)
{
    ComChoiceKeySet(GwSystem.turnPlayerNo, 0, 0);
}

void mbComChoiceUpSet(void)
{
    mbWinTopComKeyHookSet(ComChoiceUp);
}

static void ComChoiceDown(void)
{
    ComChoiceKeySet(GwSystem.turnPlayerNo, 1, 0);
}

void mbComChoiceDownSet(void)
{
    mbWinTopComKeyHookSet(ComChoiceDown);
}

static void ComChoiceKeySet(s32 playerNo, s32 leftRight, s32 upDown)
{
    s32 key[4];
    s32 padNo;
    s16 time;
    s32 keyValue;

    key[0] = key[1] = key[2] = key[3] = 0;
    padNo = GwPlayer[playerNo].padNo;
    time = GWComKeyDelayGet();
    if (upDown == 0) {
        keyValue = 4;
    } else {
        keyValue = 2;
    }
    if (leftRight == 1) {
        key[padNo] = keyValue;
        HuWinComKeyWait(key[0], key[1], key[2], key[3], time);
    }
    key[padNo] = 256;
    HuWinComKeyWait(key[0], key[1], key[2], key[3], time);
}

static void ComChoiceList(void)
{
    ComChoiceListKeySet(GwSystem.turnPlayerNo, listRightNum, listDownNum);
}

static void ComChoiceListKeySet(s32 playerNo, s32 rightNum, s32 downNum)
{
    s32 key[4];
    s32 padNo;
    s16 time;
    s32 i;

    key[0] = key[1] = key[2] = key[3] = 0;
    padNo = GwPlayer[playerNo].padNo;
    time = GWComKeyDelayGet();
    for (i = 0; i < downNum; i++) {
        key[padNo] = 4;
        HuWinComKeyWait(key[0], key[1], key[2], key[3], time);
    }
    for (i = 0; i < rightNum; i++) {
        key[padNo] = 2;
        HuWinComKeyWait(key[0], key[1], key[2], key[3], time);
    }
    key[padNo] = 256;
    HuWinComKeyWait(key[0], key[1], key[2], key[3], time);
}

void mbComChoiceListDownSet(s32 num)
{
    listRightNum = 0;
    listDownNum = num;
    mbWinTopComKeyHookSet(ComChoiceList);
}

void mbComChoiceListRightSet(s32 num)
{
    listRightNum = num;
    listDownNum = 0;
    mbWinTopComKeyHookSet(ComChoiceList);
}

void mbComChoiceListSet(s32 rightNum, s32 downNum)
{
    listRightNum = rightNum;
    listDownNum = downNum;
    mbWinTopComKeyHookSet(ComChoiceList);
}
