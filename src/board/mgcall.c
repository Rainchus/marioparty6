#include "game/board/window.h"

#include "game/data.h"
#include "game/gamework.h"
#include "game/object.h"

#include <string.h>

typedef struct MgListWork_s {
    unsigned killF : 1;
    unsigned slideF : 1;
    unsigned mode : 3;
    unsigned no : 3;
    unsigned dispF : 1;
    s16 winNo;
    s16 sprId;
    s16 time;
    s16 maxTime;
    int hiddenMes;
} MGLISTWORK;

static const HuVec2f statusPos4PBase[GW_PLAYER_MAX] = {
    { 176.0f, 184.0f },
    { 400.0f, 184.0f },
    { 176.0f, 296.0f },
    { 400.0f, 296.0f },
};

static const HuVec2f statusPos2Vs2Base[GW_PLAYER_MAX] = {
    { 176.0f, 200.0f },
    { 176.0f, 280.0f },
    { 400.0f, 200.0f },
    { 400.0f, 280.0f },
};

static const HuVec2f statusPos1Vs3Base[GW_PLAYER_MAX] = {
    { 176.0f, 240.0f },
    { 400.0f, 160.0f },
    { 400.0f, 240.0f },
    { 400.0f, 320.0f },
};

static const HuVec2f statusPosKettouBase[2] = {
    { 152.0f, 240.0f },
    { 424.0f, 240.0f },
};

static s16 mgCallHisOfsTbl[9];
static s16 mgCallHis4P[16];
static s16 mgCallHis1Vs3[6];
static s16 mgCallHis2Vs2[6];
static s16 mgCallHisKettou[14];
static HuVec2f mgStatusPos4P[GW_PLAYER_MAX];
static HuVec2f mgStatusPos1Vs3[GW_PLAYER_MAX];
static HuVec2f mgStatusPos2Vs2[GW_PLAYER_MAX];
static OMOBJ *mgListObj[4];

static s16 mgCallHisBattle[2];

static s16 *mgCallHisPtr[9] = {
    mgCallHis4P,
    mgCallHis1Vs3,
    mgCallHis2Vs2,
    mgCallHisBattle,
    NULL,
    NULL,
    mgCallHisKettou,
    NULL,
    NULL,
};

static int mgCallHisSize[9] = {
    16,
    6,
    6,
    2,
    0,
    0,
    14,
    0,
    0,
};

static u32 mgCallDataDir = -1;
static OMOBJ *mgCallVsEffOMObj;
static s16 mgCallFocus;

void mbev_MgCallSingleKoopa(int playerNo, BOOL koopaF);

void mbMgCallInit(void)
{
    int i;

    for (i = 0; i < 9; i++) {
        mgCallHisOfsTbl[i] = 0;
    }
    memset(mgCallHis4P, 0, sizeof(mgCallHis4P));
    memset(mgCallHis1Vs3, 0, sizeof(mgCallHis1Vs3));
    memset(mgCallHis2Vs2, 0, sizeof(mgCallHis2Vs2));
    memset(mgCallHisBattle, 0, sizeof(mgCallHisBattle));
    memset(mgCallHisKettou, 0, sizeof(mgCallHisKettou));
    memcpy(mgStatusPos4P, statusPos4PBase, sizeof(mgStatusPos4P));
    memcpy(mgStatusPos1Vs3, statusPos1Vs3Base, sizeof(mgStatusPos1Vs3));
    memcpy(mgStatusPos2Vs2, statusPos2Vs2Base, sizeof(mgStatusPos2Vs2));
}

static s16 MgNameColorGet(u32 nameMes)
{
    char *mesPtr;
    s16 i;

    if (nameMes > 0x80000000) {
        mesPtr = (char *)nameMes;
    } else {
        mesPtr = HuWinMesPtrGet(nameMes);
    }
    for (i = 0; *mesPtr; mesPtr++) {
        if (*mesPtr == 0x1E) {
            return mesPtr[1] - 1;
        }
    }
    return 7;
}

static void MgRouletteSlide(OMOBJ *obj)
{
    MGLISTWORK *work = omObjGetWork(obj, MGLISTWORK);

    work->slideF = TRUE;
    work->mode = 0;
    work->time = 0;
    work->maxTime = 30;
}

static void MgRouletteFocus(OMOBJ *obj)
{
    MGLISTWORK *work = omObjGetWork(obj, MGLISTWORK);

    work->mode = 4;
    work->time = 0;
    work->maxTime = 90;
    if (work->hiddenMes >= 0) {
        mbWinKill(work->winNo);
        work->winNo = mbWinCreateHelp(work->hiddenMes);
    }
}

static BOOL MgRouletteSlideCheck(OMOBJ *obj)
{
    MGLISTWORK *work = omObjGetWork(obj, MGLISTWORK);

    if (work->slideF) {
        return FALSE;
    } else {
        return TRUE;
    }
}

void mbMgRouletteFocusKill(BOOL killF)
{
    MGLISTWORK *work = omObjGetWork(mgListObj[mgCallFocus], MGLISTWORK);

    work->dispF = killF;
}

static BOOL MgCallHisCheck(int type, s16 no)
{
    s16 i;

    if (mgCallHisPtr[type] == NULL) {
        return FALSE;
    }
    for (i = 0; i < mgCallHisSize[type]; i++) {
        if (no == mgCallHisPtr[type][i]) {
            return TRUE;
        }
    }
    return FALSE;
}

void mbev_MgCallSingle(int playerNo)
{
    mbev_MgCallSingleKoopa(playerNo, FALSE);
}

void mbMgCallDataClose(void)
{
    if (mgCallDataDir != -1) {
        HuDataDirClose(mgCallDataDir);
    }
}

static void MgCallVsEffKill(void)
{
    mgCallVsEffOMObj = NULL;
}

BOOL mbMgCallSingleOnCheck(void)
{
    return TRUE;
}

static u32 MgCallBattleMesGet(u32 mess)
{
    if (!GwSystem.curTime) {
        return mess;
    } else {
        return mess + 4;
    }
}
