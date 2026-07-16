#include "game/board/main.h"
#include "game/board/pause.h"

#include "game/armem.h"
#include "game/data.h"
#include "game/sprite.h"
#include "game/flag.h"
#include "game/pad.h"

#include "msm.h"
#include "stdlib.h"

#define FLAG_BOARD_WALKDONE FLAGNUM(FLAG_GROUP_COMMON, 16)

typedef struct TauntWork_s {
    unsigned killF : 1;
} TAUNT_WORK;

typedef struct TelopTimeChangeWork_s {
    unsigned killF : 1;
    unsigned completeF : 1;
} TELOP_TIME_CHANGE_WORK;

static float telopTimeBaseTPLvlTbl[8] = {
    1.0f,
    1.0f,
    1.0f,
    1.0f,
    1.0f,
    0.0f,
    0.0f,
    0.0f,
};

static s32 tauntSeNo[GW_PLAYER_MAX] = {
    MSM_SENO_NONE,
    MSM_SENO_NONE,
    MSM_SENO_NONE,
    MSM_SENO_NONE,
};

static OMOBJ *telopOMObj;
static OMOBJ *telopTimeOMObj;
static OMOBJ *tauntOMObj;
static OMOBJ *telopTimeChangeOMObj;

static void TauntOMExec(OMOBJ *obj);
void mbTelopTimeDispSet(s16 grpId, BOOL dispF);

BOOL mbTelopCheck(void)
{
    return telopOMObj == NULL;
}

static void TelopTimePauseHook(BOOL dispF)
{
    if (telopTimeOMObj != NULL) {
        mbTelopTimeDispSet(telopTimeOMObj->mdlId[0], dispF);
    }
}

void mbTelopTimeSprKill(s16 grpId)
{
    HuSprGrpKill(grpId);
}

void mbTelopTimeStarSet(s16 grpId, s32 starNum)
{
    s32 emptyNum = 3 - starNum;
    s32 i;

    for (i = 0; i < 3; i++) {
        s32 bank = 0;

        if (GwSystem.curTime) {
            bank += 2;
        }
        if (i < emptyNum) {
            bank++;
        }
        HuSprBankSet(grpId, i + 2, bank);
        HuSprBankSet(grpId, i + 5, bank);
        HuSprTPLvlSet(grpId, i + 5, 0.0f);
    }
}

void mbTelopTimeTPLvlSet(s16 grpId, float tpLvl)
{
    s32 i;

    for (i = 0; i < 8; i++) {
        HuSprTPLvlSet(grpId, i, tpLvl * telopTimeBaseTPLvlTbl[i]);
    }
}

void mbTelopTimeDispSet(s16 grpId, BOOL dispF)
{
    s32 i;

    for (i = 0; i < 8; i++) {
        if (dispF) {
            HuSprDispOn(grpId, i);
        } else {
            HuSprDispOff(grpId, i);
        }
    }
}

s8 mbPadStkXGet(s32 playerNo)
{
    s8 stkX = HuPadStkX[playerNo];
    s8 subStkX = HuPadSubStkX[playerNo];

    if (abs(stkX) > abs(subStkX)) {
        if (abs(stkX) < 8) {
            return 0;
        } else {
            return stkX;
        }
    } else {
        if (abs(subStkX) < 8) {
            return 0;
        } else {
            return subStkX;
        }
    }
}

s8 mbPadStkYGet(s32 playerNo)
{
    s8 stkY = HuPadStkY[playerNo];
    s8 subStkY = HuPadSubStkY[playerNo];

    if (abs(stkY) > abs(subStkY)) {
        if (abs(stkY) < 8) {
            return 0;
        } else {
            return stkY;
        }
    } else {
        if (abs(subStkY) < 8) {
            return 0;
        } else {
            return subStkY;
        }
    }
}

void mbTauntInit(void)
{
    s32 i;

    tauntOMObj = omAddObj(mbObjMan, 0x7E00, 0, 0, TauntOMExec);
    for (i = 0; i < GW_PLAYER_MAX; i++) {
        tauntSeNo[i] = MSM_SENO_NONE;
    }
    _SetFlag(FLAG_BOARD_WALKDONE);
}

void mbTauntClose(void)
{
    if (tauntOMObj != NULL) {
        TAUNT_WORK *work = omObjGetWork(tauntOMObj, TAUNT_WORK);
        work->killF = TRUE;
        _SetFlag(FLAG_BOARD_WALKDONE);
    }
}

s32 mbLanguageGet(void)
{
    static s32 languageTbl[6][2] = {
        { 0, 0 },
        { 1, 1 },
        { 2, 2 },
        { 3, 3 },
        { 4, 4 },
        { 5, 5 },
    };
    s32 languageNo = GWLanguageGet();
    s32 i;

    for (i = 0; i < 6; i++) {
        if (languageNo == languageTbl[i][0]) {
            break;
        }
    }
    return languageTbl[i][1];
}

void mbLanguageSet(s32 languageNo)
{
    static s32 languageTbl[6][2] = {
        { 0, 0 },
        { 1, 1 },
        { 2, 2 },
        { 3, 3 },
        { 4, 4 },
        { 5, 5 },
    };
    s32 i;

    for (i = 0; i < 6; i++) {
        if (languageNo == languageTbl[i][0]) {
            break;
        }
    }
    GWLanguageSet(languageTbl[i][1]);
}

static u32 boardDataDirTbl[6] = {
    DATA_board,
    DATA_board_us,
    DATA_board,
    DATA_board,
    DATA_board,
    DATA_board,
};

static inline u32 BoardDataDirGet(s32 boardNo)
{
    if (boardNo < 0) {
        boardNo = 0;
    }
    return boardDataDirTbl[boardNo];
}

int mbBoardDataNumGet(int dataNum)
{
    s32 languageNo = mbLanguageGet();

    if (DIRNUM(dataNum) != DATA_board) {
        return mbPauseDataNumGet(dataNum);
    }
    if (languageNo < 0) {
        languageNo = 0;
    }
    return FILENUM(dataNum) | BoardDataDirGet(languageNo);
}

void mbBoardDataDirRead(void)
{
    s32 languageNo = mbLanguageGet();
    s32 i;

    if (languageNo < 0) {
        languageNo = 0;
    }
    for (i = 0; i < 6; i++) {
        if (BoardDataDirGet(i) == BoardDataDirGet(languageNo)) {
            continue;
        }
        if (HuARDirCheck(BoardDataDirGet(i)) != 0) {
            HuARDirFree(BoardDataDirGet(i));
        }
        if (HuDataReadChk(BoardDataDirGet(i)) >= 0) {
            HuDataDirClose(BoardDataDirGet(i));
        }
    }
    if (HuARDirCheck(BoardDataDirGet(languageNo)) == 0) {
        HuAR_DVDtoARAM(BoardDataDirGet(languageNo));
        while (HuARDMACheck()) {
        }
    }
}

void mbTelopTimeChangeKill(void)
{
    if (telopTimeChangeOMObj != NULL) {
        TELOP_TIME_CHANGE_WORK *work = omObjGetWork(telopTimeChangeOMObj, TELOP_TIME_CHANGE_WORK);
        work->killF = TRUE;
    }
}

BOOL mbTelopTimeChangeCheck(void)
{
    TELOP_TIME_CHANGE_WORK *work;

    if (telopTimeChangeOMObj == NULL) {
        return FALSE;
    }
    work = omObjGetWork(telopTimeChangeOMObj, TELOP_TIME_CHANGE_WORK);
    return work->completeF == FALSE;
}
