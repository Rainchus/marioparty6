#include "game/board/main.h"

static int starAddNum = 1;
static void (*starMasuFunc)(void);
static void (*starMoveHook)(void);
static int starNum;

void mbStarMoveHookSet(void (*hook)(void))
{
    starMoveHook = hook;
}

void mbStarMasuFuncSet(void (*func)(void))
{
    starMasuFunc = func;
}

void mbStarAddNumSet(int num)
{
    starAddNum = num;
}

void mbStarStub(void)
{
}

void mbStarStub2(void)
{
}

void mbStarMapViewExec(int playerNo, int no)
{
}

void mbStarNumSet(int num)
{
    starNum = num;
}

int mbStarNumGet(void)
{
    return starNum;
}

void mbStarFlagInit(void)
{
    GwSystem.starFlag = 0;
}

void mbStarFlagSet(int no)
{
    GwSystem.starFlag |= (1 << no);
}

void mbStarFlagReset(int no)
{
    GwSystem.starFlag &= ~(1 << no);
}

u8 mbStarFlagGet(void)
{
    return GwSystem.starFlag;
}

void mbStarNextNoSet(s8 nextNo)
{
    GwSystem.starPos = nextNo;
}

u8 mbStarNextNoGet(void)
{
    return GwSystem.starPos;
}

void mbStarNoSet(int no)
{
}

int mbStarNoRandGet(void)
{
    return -1;
}
