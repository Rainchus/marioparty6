#define _MATH_H
#define M_PI 3.141592653589793
double sin(double);
double cos(double);

#include "game/board/pause.h"
#include "game/board/main.h"
#include "game/board/audio.h"
#include "game/board/effect.h"
#include "game/board/guide.h"
#include "game/board/player.h"
#include "game/board/window.h"

#include "game/esprite.h"
#include "game/sprite.h"
#include "game/hu3d.h"
#include "game/process.h"
#include "game/object.h"
#include "game/gamework.h"
#include "game/mgdata.h"
#include "game/pad.h"
#include "game/flag.h"
#include "game/memory.h"
#include "game/data.h"
#include "game/audio.h"

#include "humath.h"
#include "string.h"

#define PAUSE_HOOK_MAX 32

static inline s32 GWMgPackGet(void)
{
    if(GwSystem.mgPack >= 5) {
        GwSystem.mgPack = 0;
    }
    return GwSystem.mgPack;
}

/* board externs not yet in headers */
extern HUPROCESS *mbMainProc;
extern s32 mbBGRead(int dataNum);
extern void mbBGReadWait(s32 handle);
extern BOOL mbWipeSpecialCheck(void);
extern int mbWipeSpecialStatGet(void);
extern void mbExitReq(void);
extern int mbLanguageGet(void);
extern void mbPauseGuideCreate(void);
extern void mbPauseGuideKill(void);
extern void mbPauseDispCopyCreate(void);
extern void mbPauseDispCopyKill(void);
extern void mbPauseEnableSet(void);
extern void mbPauseEnableReset(void);
extern s16 mbTelopTimeSprCreate(void);
extern void mbTelopTimeSprRotSet(s16 id, float rot);
extern void mbTelopTimeDispSet(s16 id, BOOL disp);
extern void mbTelopTimeSprKill(s16 id);
extern void mbNormPosto3D(HuVecF *src, int camId, HuVecF *dst);
extern BOOL mbConfigExec(int playerNo, int modelId);
extern s16 mbPausePanelCreate(int dataNum, unsigned int espDataNum);
extern void mbPausePanelPosSet(s16 id, float x, float y);
extern void mbPausePanelBankSet(s16 id, int bank);
extern void mbPausePanelGrowSet(s16 id, int a, int b, float scale);
extern void mbPausePanelKill(s16 id);
extern int mbSingleStepGet(void);
extern int mbMasuNumGet(void);
extern BOOL mbSingleMgUnlockGet(int mgNo);

typedef struct MgList_s {
    s16 type;       /* 0x00 */
    s16 count;      /* 0x02 */
    s16 num;        /* 0x04 */
    struct {
        u8 mgNo;
        u8 flag;
    } mg[32];       /* 0x06 */
} MGLIST;

typedef struct pauseWork_s {
    s16 sprId[6];   /* 0x00 */
    s16 panelId;    /* 0x0C */
    s16 telopId;    /* 0x0E */
    s32 winId[2];   /* 0x10 */
} PAUSEWORK;

typedef struct pauseSingleWork_s {
    s32 bgWin;      /* 0x00 */
    s32 titleWin;   /* 0x04 */
    s32 mgWin[2];   /* 0x08 */
    s32 moveCnt[2]; /* 0x10 */
    s32 growF[2];   /* 0x18 */
    s16 sprId[14];  /* 0x20 */
} PAUSESINGLEWORK;

static MBPAUSEHOOK pauseHook[PAUSE_HOOK_MAX];
static MGLIST pauseMGList[6];
static PAUSESINGLEWORK pauseSingleWork;

static int pausePlayer = -1;
static s8 guideMotTbl[] = { 3, 12, 11, 23, -1 };
static s8 guideMotSingleTbl[] = { 3, 12, 11, 23, -1 };

static HUPROCESS *pauseProc;
static int pauseHookNum;
static OMOBJ *pauseGuideObj;

static const u32 HelpWinMesTbl[2] = { 0x00260008, 0x00280018 };

static void PauseMain(void);
static void PauseDestroy(void);
static void PauseScreenCreate(PAUSEWORK *work);
static void PauseScreenSingleCreate(PAUSEWORK *work);
static void PauseScreenKill(PAUSEWORK *work);
static void PauseScreenSingleKill(PAUSEWORK *work);
static int PauseScreenExec(PAUSEWORK *work);
static int PauseSingleMGListGet(MGLIST *list);
static void PauseSingleSprCreate(MGLIST *list, PAUSESINGLEWORK *work);
static void PauseSingleMGTypeSet(MGLIST *list, PAUSESINGLEWORK *work, int page, int top);
static int PauseSingleExec(PAUSEWORK *work);

void mbPauseInit(void)
{
    int i;
    pauseProc = NULL;
    pauseHookNum = 0;
    HuPrcAllPause(FALSE);
    Hu3DPauseSet(FALSE);
    HuSprPauseSet(FALSE);
    for(i=0; i<PAUSE_HOOK_MAX; i++) {
        pauseHook[i] = NULL;
    }
}

void mbPauseCreate(int playerNo)
{
    pauseProc = HuPrcChildCreate(PauseMain, 0x2012, 0x3800, 0, mbMainProc);
    HuPrcDestructorSet2(pauseProc, PauseDestroy);
    pausePlayer = playerNo;
    HuPrcSetStat(pauseProc, HU_PRC_STAT_PAUSE_ON|HU_PRC_STAT_UPAUSE_ON);
    mbPauseSet(TRUE);
}

int mbPauseStartCheck(void)
{
    int i;
    if(omUPauseFlag) {
        return -1;
    }
    if(mbPauseProcCheck()) {
        return -1;
    }
    if(mbPauseDisableGet()) {
        return -1;
    }
    if(_CheckFlag(FLAG_BOARD_TUTORIAL)) {
        return -1;
    }
    if(!mbEffFadeCheck()) {
        return -1;
    }
    if(mbWipeSpecialCheck()) {
        return -1;
    }
    if(mbWipeSpecialStatGet()) {
        return -1;
    }
    for(i=0; i<GW_PLAYER_MAX; i++) {
        int padNo = GwPlayer[i].padNo;
        if(HuPadStatGet(padNo) == 0) {
            if((GWPartyGet() != FALSE || GwPlayer[i].comF == FALSE) && (HuPadBtnDown[padNo] & PAD_BUTTON_START)) {
                return i;
            }
        }
    }
    return -1;
}

BOOL mbPauseProcCheck(void)
{
    return (pauseProc != NULL) ? TRUE : FALSE;
}

void mbPauseDisableSet(BOOL disableF)
{
    if(disableF) {
        _SetFlag(FLAGNUM(FLAG_GROUP_COMMON, 31));
    } else {
        _ClearFlag(FLAGNUM(FLAG_GROUP_COMMON, 31));
    }
}

BOOL mbPauseDisableGet(void)
{
    return (_CheckFlag(FLAGNUM(FLAG_GROUP_COMMON, 31))) ? TRUE : FALSE;
}

void mbPauseHookPush(MBPAUSEHOOK hook)
{
    pauseHook[pauseHookNum++] = hook;
}

void mbPauseHookPop(MBPAUSEHOOK hook)
{
    int i;
    for(i=0; i<pauseHookNum; i++) {
        if(pauseHook[i] == hook) {
            break;
        }
    }
    if(i >= pauseHookNum) {
        return;
    }
    for(; i<pauseHookNum-1; i++) {
        pauseHook[i] = pauseHook[i+1];
    }
    pauseHookNum--;
}

static void PauseMain(void)
{
    PAUSEWORK pauseWork;
    PAUSEWORK *work = &pauseWork;
    int i;
    s32 bgHandle;
    int cancelF = FALSE;
    int result = TRUE;
    int boardNo;
    int partyF;
    static const u32 logoFileTbl[] = {
        DATANUM(DATA_board, 0x44),
        DATANUM(DATA_board, 0x45),
        DATANUM(DATA_board, 0x46),
        DATANUM(DATA_board, 0x47),
        DATANUM(DATA_board, 0x48),
        DATANUM(DATA_board, 0x49),
        DATANUM(DATA_board, 0x4C),
        DATANUM(DATA_board, 0x4B),
        DATANUM(DATA_board, 0x4A),
        DATANUM(DATA_board, 0x44),
        DATANUM(DATA_board, 0x44),
    };
    HuMemHeapDump(HuMemHeapPtrGet(HEAP_DVD), -1);
    pauseGuideObj = NULL;
    boardNo = GwSystem.boardNo;
    work->sprId[0] = espEntry(mbBoardDataNumGet(logoFileTbl[boardNo]), 100, 0);
    espDispOff(work->sprId[0]);
    work->telopId = -1;
    partyF = GwSystem.partyF;
    if(partyF) {
        work->telopId = mbTelopTimeSprCreate();
        mbTelopTimeSprRotSet(work->telopId, 0.0f);
        mbTelopTimeDispSet(work->telopId, FALSE);
        HuSprGrpPosSet(work->telopId, 288.0f * 0.35f, 216.0f);
        HuSprGrpScaleSet(work->telopId, 0.65f, 0.65f);
    }
    HuDataDirClose(mbBoardDataNumGet(DATA_board));
    bgHandle = mbBGRead(mbBoardDataNumGet(DATA_bpause6));
    mbEffFadeCreate(30, 160);
    for(i=0; i<pauseHookNum; i++) {
        if(pauseHook[i] != NULL) {
            pauseHook[i](FALSE);
        }
    }
    HuPadRumbleAllStop();
    HuPrcSleep(1);
    mbAudFXPlay(5);
    while(!mbEffFadeDoneCheck()) {
        HuPrcVSleep();
    }
    mbPauseGuideCreate();
    mbPauseDispCopyCreate();
    mbBGReadWait(bgHandle);
    while(cancelF == 0 && result != 0) {
        PauseScreenCreate(work);
        result = PauseScreenExec(work);
        PauseScreenKill(work);
        if(result != 0) {
            cancelF = mbConfigExec(pausePlayer, mbGuideModelGet(pauseGuideObj));
        }
    }
    if(pauseGuideObj) {
        mbGuideKill(pauseGuideObj);
        pauseGuideObj = NULL;
    }
    espKill(work->sprId[0]);
    work->sprId[0] = -1;
    if(work->telopId >= 0) {
        mbTelopTimeSprKill(work->telopId);
        work->telopId = -1;
    }
    mbPauseGuideKill();
    HuDataDirClose(mbBoardDataNumGet(DATA_bpause6));
    mbPauseDispCopyKill();
    if(cancelF) {
        mbExitReq();
        HuPrcSleep(-1);
    }
    bgHandle = mbBGRead(mbBoardDataNumGet(DATA_board));
    mbEffFadeOutSet(30);
    HuPrcSleep(30);
    mbBGReadWait(bgHandle);
    HuPrcEnd();
}

static void PauseDestroy(void)
{
    int i;
    if(mbExitCheck() == FALSE) {
        for(i=0; i<pauseHookNum; i++) {
            if(pauseHook[i] != NULL) {
                pauseHook[i](TRUE);
            }
        }
        mbPauseSet(FALSE);
    }
    if(GWMgInstDispGet()) {
        _SetFlag(FLAGNUM(FLAG_GROUP_SAVE, 5));
    } else {
        _ClearFlag(FLAGNUM(FLAG_GROUP_SAVE, 5));
    }
    if(GWPartyGet() == FALSE) {
        GwCommon.storyMgInstDispF = GWMgInstDispGet();
        GwCommon.storyMgComDispF = GWMgComDispGet();
        GwCommon.storyMgPack = GWMgPackGet();
        GwCommon.storyMessSpeed = GWMessSpeedGet();
        GwCommon.storySaveMode = GWSaveModeGet();
    } else {
        GwCommon.partyMgInstDispF = GWMgInstDispGet();
        GwCommon.partyMgComDispF = GWMgComDispGet();
        GwCommon.partyMgPack = GWMgPackGet();
        GwCommon.partyMessSpeed = GWMessSpeedGet();
        GwCommon.partySaveMode = GWSaveModeGet();
    }
    pausePlayer = -1;
    pauseProc = NULL;
}

static void PauseScreenCreate(PAUSEWORK *work)
{
    int i;
    int lang;
    int bank;
    int turn;
    int partyF;
    int tag;
    HuVec2f center;
    HuVecF pos3d;
    static const HuVec2f turnNoPos[] = {
        { -74, -25 },
        { -30, -25 },
        { 30, 25 },
        { 74, 25 },
    };
    static const float turnOfsTbl[] = { 0.0f, 64.0f, 100.0f, 100.0f, 100.0f, 100.0f };
    static const int anmNoTbl[] = { 2, 0, 1 };
    static HuVecF guidePos = { -0.65f, -0.75f, -750.0f };

    partyF = GwSystem.partyF;
    if(!partyF) {
        PauseScreenSingleCreate(work);
        return;
    }
    lang = mbLanguageGet();
    espDispOn(work->sprId[0]);
    espAttrSet(work->sprId[0], HUSPR_ATTR_LINEAR);
    espPosSet(work->sprId[0], 288.0f, 128.0f);
    work->panelId = mbPausePanelCreate(mbBoardDataNumGet(DATA_bpause6), 0);
    mbPausePanelPosSet((s16)work->panelId, 0.65f, 0.0f);
    tag = GwSystem.tagF;
    if(tag) {
        bank = 2;
    } else {
        bank = GWPartyGet();
    }
    mbPausePanelBankSet((s16)work->panelId, anmNoTbl[bank]);
    mbPausePanelGrowSet((s16)work->panelId, 16, 0, 1.5f);
    work->sprId[1] = espEntry(mbBoardDataNumGet(DATANUM(DATA_bpause6, 1)), 100, 0);
    espAttrSet(work->sprId[1], HUSPR_ATTR_LINEAR);
    pos3d.x = 273.6f + turnOfsTbl[lang];
    pos3d.y = 304.0f;
    espPosSet(work->sprId[1], pos3d.x, pos3d.y);
    for(i=0; i<4; i++) {
        work->sprId[i+2] = espEntry(mbBoardDataNumGet(DATANUM(DATA_bpause6, 2)), 95, 0);
        espPosSet(work->sprId[i+2], pos3d.x + turnNoPos[i].x, pos3d.y + turnNoPos[i].y);
        espAttrSet(work->sprId[i+2], HUSPR_ATTR_LINEAR);
    }
    turn = GwSystem.turnNo;
    if(turn > 99) {
        turn = 99;
    }
    if(turn/10 != 0) {
        espBankSet(work->sprId[2], turn/10);
    } else {
        espAttrSet(work->sprId[2], HUSPR_ATTR_DISPOFF);
    }
    espBankSet(work->sprId[3], turn%10);
    turn = GwSystem.turnMax;
    if(turn > 99) {
        turn = 99;
    }
    if(turn/10 != 0) {
        espBankSet(work->sprId[4], turn/10);
    } else {
        espAttrSet(work->sprId[4], HUSPR_ATTR_DISPOFF);
    }
    espBankSet(work->sprId[5], turn%10);
    if(pauseGuideObj == NULL) {
        mbNormPosto3D(&guidePos, 4, &pos3d);
        pauseGuideObj = mbGuideCreateFlag(&pos3d, guideMotTbl, TRUE, FALSE, TRUE);
        mbObjDispSet(mbGuideModelGet(pauseGuideObj), FALSE);
    }
    if(work->telopId >= 0) {
        mbTelopTimeDispSet(work->telopId, TRUE);
    }
    work->winId[0] = work->winId[1] = -1;
    work->winId[0] = mbWinCreateHelp(HelpWinMesTbl[0]);
    mbWinCenterGet(work->winId[0], &center);
    mbWinPosSet(work->winId[0], center.x, 376);
    HuPrcVSleep();
    mbObjDispSet(mbGuideModelGet(pauseGuideObj), TRUE);
}

static void PauseScreenSingleCreate(PAUSEWORK *work)
{
    HuVec2f center;
    HuVecF pos3d;
    static HuVecF guidePos = { -0.7f, -0.8f, -750.0f };

    espDispOn(work->sprId[0]);
    espAttrSet(work->sprId[0], HUSPR_ATTR_LINEAR);
    espPosSet(work->sprId[0], 288.0f * 0.32f, 120.0f);
    espScaleSet(work->sprId[0], 0.5f, 0.5f);
    work->panelId = 0;
    if(pauseGuideObj == NULL) {
        mbNormPosto3D(&guidePos, 4, &pos3d);
        pauseGuideObj = mbGuideCreateFlag(&pos3d, guideMotSingleTbl, TRUE, FALSE, TRUE);
    }
    work->winId[0] = work->winId[1] = -1;
    work->winId[0] = mbWinCreateHelp(0x00260008);
    mbWinCenterGet(work->winId[0], &center);
    mbWinPosSet(work->winId[0], center.x, 376);
    work->winId[1] = mbWinCreateHelp(0x00280027);
    mbWinCenterGet(work->winId[1], &center);
    mbWinPosSet(work->winId[1], center.x, 332);
}

static void PauseScreenKill(PAUSEWORK *work)
{
    int i;
    int partyF = GwSystem.partyF;
    if(!partyF) {
        PauseScreenSingleKill(work);
        return;
    }
    if(work->winId[0] != -1) {
        mbWinKill(work->winId[0]);
        work->winId[0] = -1;
    }
    espDispOff(work->sprId[0]);
    if(work->telopId >= 0) {
        mbTelopTimeDispSet(work->telopId, FALSE);
    }
    for(i=1; i<6; i++) {
        if(work->sprId[i] >= 0) {
            espKill(work->sprId[i]);
        }
        work->sprId[i] = -1;
    }
    if(work->panelId != 0) {
        mbPausePanelKill((s16)work->panelId);
        work->panelId = 0;
    }
}

static void PauseScreenSingleKill(PAUSEWORK *work)
{
    espDispOff(work->sprId[0]);
    if(work->winId[0] != -1) {
        mbWinKill(work->winId[0]);
        work->winId[0] = -1;
    }
    if(work->winId[1] != -1) {
        mbWinKill(work->winId[1]);
        work->winId[1] = -1;
    }
}

static int PauseScreenExec(PAUSEWORK *work)
{
    int i = 0;
    int result;
    int padNo;
    int partyF = GwSystem.partyF;
    if(!partyF) {
        return PauseSingleExec(work);
    }
    while(1) {
        HuPrcVSleep();
        padNo = GwPlayer[pausePlayer].padNo;
        if(HuPadStatGet(padNo)) {
            continue;
        }
        if(HuPadBtnDown[padNo] & PAD_BUTTON_A) {
            result = 1;
            mbAudFXPlay(2);
            break;
        }
        i++;
        if(i >= 60) {
            i = 0;
        }
        if(work->telopId >= 0) {
            mbTelopTimeSprRotSet(work->telopId, i * (1.0f/60.0f));
        }
        if(HuPadBtnDown[padNo] & PAD_BUTTON_START) {
            result = 0;
            break;
        }
    }
    return result;
}

void mbPauseSet(BOOL pauseF)
{
    OMOBJWORK *objWork = mbObjMan->property;
    int i;
    if(pauseF) {
        for(i=0; i<objWork->objMax; i++) {
            if((objWork->objData[i].stat & (OM_STAT_DELETED|OM_STAT_NOPAUSE)) == 0) {
                omSetStatBit(&objWork->objData[i], OM_STAT_PAUSED);
            }
        }
        mbPauseEnableSet();
    } else {
        for(i=0; i<objWork->objMax; i++) {
            if((objWork->objData[i].stat & (OM_STAT_DELETED|OM_STAT_NOPAUSE)) == 0) {
                omResetStatBit(&objWork->objData[i], OM_STAT_PAUSED);
            }
        }
        mbPauseEnableReset();
    }
    HuPrcAllPause(pauseF);
    Hu3DPauseSet(pauseF);
    HuSprPauseSet(pauseF);
    HuAudFXPauseAll(pauseF);
    HuAudSeqPauseAll(pauseF);
    mbMusPauseSet(pauseF);
}

static u32 pauseDataDirTbl[] = {
    DATA_bpause6,
    DATA_bpause6_us,
    DATA_bpause6,
    DATA_bpause6,
    DATA_bpause6,
    DATA_bpause6,
};

int mbPauseDataDirGet(int type)
{
    if(type < 0) {
        type = 0;
    }
    return pauseDataDirTbl[type];
}

int mbPauseDataNumGet(int dataNum)
{
    int lang = mbLanguageGet();
    if((dataNum & 0xFFFF0000) != DATA_bpause6) {
        return dataNum;
    }
    if(lang < 0) {
        lang = 0;
    }
    return (dataNum & 0xFFFF) | mbPauseDataDirGet(lang);
}

static s16 mgTypeTbl[6] = { 0, 1, 2, 3, 6, -1 };

static int PauseSingleMGListGet(MGLIST *list)
{
    int i;
    int j;
    int listNo;
    int total;
    MGLIST *entry;

    memset(list, 0, sizeof(pauseMGList));
    for(i=0; i<6; i++) {
        list[i].type = i;
    }
    for(i=0; MgDataTbl[i].ovl != 0xFFFF; i++) {
        listNo = 6;
        if(MgDataTbl[i].flag & 0x80) {
            listNo = 5;
            if(MgDataTbl[i].ovl == 0x52) {
                listNo = 6;
            }
        } else {
            for(j=0; j<5; j++) {
                if(mgTypeTbl[j] == MgDataTbl[i].type) {
                    listNo = j;
                    break;
                }
            }
        }
        if(listNo >= 6) {
            continue;
        }
        entry = &list[listNo];
        entry->mg[entry->num].mgNo = i;
        if(GWMgUnlockGet(MgNoGet(MgDataTbl[i].ovl) + 0x259)) {
            entry->mg[entry->num].flag = 1;
        }
        if(mbSingleMgUnlockGet(MgNoGet(MgDataTbl[i].ovl) + 0x259)) {
            entry->mg[entry->num].flag |= 2;
        }
        if(entry->mg[entry->num].flag != 0) {
            entry->count++;
        }
        entry->num++;
    }
    total = 0;
    for(i=0; i<6; i++) {
        total += list[i].count;
    }
    return total;
}

static void PauseSingleSprCreate(MGLIST *list, PAUSESINGLEWORK *work)
{
    int i;
    int rem;
    int page;
    int total;
    int lang;
    HuVec2f winSize;
    HuVecF winPos;
    static int fileTbl[] = {
        DATANUM(DATA_bpause6, 0x2C),
        DATANUM(DATA_bpause6, 0x28),
        DATANUM(DATA_bpause6, 0x29),
        DATANUM(DATA_bpause6, 0x29),
        DATANUM(DATA_bpause6, 0x2B),
        DATANUM(DATA_bpause6, 0x2B),
        DATANUM(DATA_bpause6, 0x2A),
        DATANUM(DATA_bpause6, 0x2A),
        DATANUM(DATA_bpause6, 0x27),
        DATANUM(DATA_bpause6, 0x26),
        DATANUM(DATA_bpause6, 0x26),
        DATANUM(DATA_bpause6, 0x26),
        DATANUM(DATA_bpause6, 0x20),
        DATANUM(DATA_bpause6, 0x20),
    };
    static HuVec2f posTbl[] = {
        { 288, 136 }, { 472, 72 }, { 424, 72 }, { 520, 72 },
        { 424, 72 }, { 520, 72 }, { 460, 72 }, { 484, 72 },
        { 480, 152 }, { 504, 152 }, { 524, 152 }, { 542, 152 },
        { 288, 172 }, { 288, 312 },
    };
    static s16 langOfsTbl[6][6] = {
        { -48, 48, -48, 48, -12, 12 },
        { -28, -28, -28, -28, -64, -40 },
        { -28, -28, -28, -28, -64, -40 },
        { -28, -28, -28, -28, -64, -40 },
        { -28, -28, -28, -28, -64, -40 },
        { -28, -28, -28, -28, -64, -40 },
    };

    total = PauseSingleMGListGet(list);
    memset(work, 0, sizeof(PAUSESINGLEWORK));
    for(i=0; i<14; i++) {
        work->sprId[i] = espEntry(mbBoardDataNumGet(fileTbl[i]), 100, 0);
        espAttrSet(work->sprId[i], HUSPR_ATTR_LINEAR);
        espPosSet(work->sprId[i], posTbl[i].x, posTbl[i].y);
    }
    espBankSet(work->sprId[12], 1);
    espBankSet(work->sprId[13], 3);
    espDispOff(work->sprId[12]);
    espDispOff(work->sprId[13]);
    espPriSet(work->sprId[0], 2000);
    work->moveCnt[0] = work->moveCnt[1] = 0;
    work->growF[0] = work->growF[1] = 0;
    lang = mbLanguageGet();
    for(i=0; i<6; i++) {
        espPosSet(work->sprId[i+2], langOfsTbl[lang][i] + 472, 72.0f);
    }
    espBankSet(work->sprId[3], 1);
    espBankSet(work->sprId[5], 1);
    espDispOff(work->sprId[6]);
    i = mbSingleStepGet();
    if(i <= 1) {
        espDispOff(work->sprId[2]);
        espDispOff(work->sprId[3]);
    } else {
        espDispOff(work->sprId[4]);
        espDispOff(work->sprId[5]);
    }
    if(i < 0) {
        i = 0;
    }
    if(i >= mbMasuNumGet()) {
        i = 0;
    }
    rem = i;
    page = rem/10;
    if(page > 0) {
        espDispOn(work->sprId[6]);
        espBankSet(work->sprId[6], page);
    }
    rem = rem - (page*10);
    if(rem > 0) {
        if(i <= 5) {
            rem += 9;
        }
        espBankSet(work->sprId[7], rem);
    }
    for(i=0; i<4; i++) {
        espScaleSet(work->sprId[i+8], 1.2f, 1.2f);
    }
    espBankSet(work->sprId[9], 10);
    espDispOff(work->sprId[11]);
    i = 0;
    page = total/10;
    if(page > 0) {
        espDispOn(work->sprId[11]);
        espBankSet(work->sprId[10], page);
        i++;
    }
    rem = total - (page*10);
    if(rem > 0) {
        espBankSet(work->sprId[i+10], rem);
    }
    winPos.x = 288.0f;
    winPos.y = 240.0f;
    winPos.z = 0.0f;
    work->bgWin = mbWinCreateBlank();
    winSize.x = 540.0f;
    winSize.y = 120.0f;
    mbWinSizeSet(work->bgWin, winSize.x, winSize.y);
    winPos.x = winPos.x + (0.5f * -winSize.x);
    winPos.y = winPos.y + (0.5f * -winSize.y);
    mbWinPosSet(work->bgWin, winPos.x, winPos.y);
    winPos.x += 4.0f;
    winPos.y += 4.0f;
    for(i=0; i<2; i++) {
        work->mgWin[i] = mbWinCreateHelp(0x0028001A);
        mbWinMesMaxSizeGet(work->mgWin[i], &winSize);
        mbWinSizeSet(work->mgWin[i], 268, winSize.y);
        mbWinPosSet(work->mgWin[i], winPos.x + ((i & 1) ? 268 : 0), winPos.y);
    }
    work->titleWin = mbWinCreateHelp(0x0028001F);
    mbWinMesMaxSizeGet(work->titleWin, &winSize);
    winPos.x = 288.0f;
    winPos.y = 240.0f;
    mbWinPosSet(work->titleWin, winPos.x - (0.5f * winSize.x), (winPos.y - 108.0f) - (0.5f * winSize.y));
}

static void PauseSingleMGTypeSet(MGLIST *list, PAUSESINGLEWORK *work, int page, int top)
{
    int i;
    MGLIST *entry;
    u32 mesA;
    u32 mesB;
    int lang;
    HuVec2f winSize;
    static u32 insertMesTbl[] = {
        0x0028001F, 0x00280020, 0x00280021, 0x00280022, 0x00280023, 0x00280026,
    };
    static int insertMesTbl2[] = { 0, 0, 1, 1, 2, 2, 3, 3 };
    static int insertMesTbl3[] = { 4, 4, 5, 5, 6, 6, 7, 7 };
    static const float nameScaleTbl[] = { 1.0f, 0.85f, 0.85f, 0.85f, 0.85f, 0.85f };

    entry = &list[page];
    lang = mbLanguageGet();
    mbWinCenterInsertGet(work->titleWin, insertMesTbl[page]);
    mbWinScaleSet(work->titleWin, nameScaleTbl[lang], 1.0f);
    mbWinMesMaxSizeGet(work->titleWin, &winSize);
    mbWinPosSet(work->titleWin, 288.0f - (0.5f * winSize.x * nameScaleTbl[lang]), 136.0f - (0.5f * winSize.y));
    for(i=0; i<8; i++) {
        int slot = i + top;
        mesA = (u32)" ";
        mesB = 0x0028001D;
        if(slot < entry->num) {
            if(entry->mg[slot].flag != 0) {
                mesA = MgDataTbl[entry->mg[slot].mgNo].nameMes;
                if(entry->mg[slot].flag >= 2) {
                    mesB = 0x0028001E;
                }
            } else {
                mesA = 0x0028001C;
            }
        }
        mbWinInsertMesSet(work->mgWin[i & 1], mesA, insertMesTbl2[i]);
        mbWinInsertMesSet(work->mgWin[i & 1], mesB, insertMesTbl3[i]);
    }
    for(i=0; i<2; i++) {
        mbWinCenterInsertGet(work->mgWin[i], 0x0028001A);
    }
    work->growF[0] = work->growF[1] = 1;
    if(top <= 0) {
        work->growF[0] = 0;
    }
    if(top + 8 >= entry->num) {
        work->growF[1] = 0;
    }
}

static int PauseSingleExec(PAUSEWORK *work)
{
    int i;
    int n;
    int result;
    int padNo;
    int wait = 0;
    int page = 0;
    int top = 0;
    int newPage;
    int newTop;
    float scale;
    float target;
    PauseSingleSprCreate(pauseMGList, &pauseSingleWork);
    HuPrcVSleep();
    PauseSingleMGTypeSet(pauseMGList, &pauseSingleWork, 0, 0);
    while(1) {
        HuPrcVSleep();
        padNo = GwPlayer[pausePlayer].padNo;
        if(HuPadStatGet(padNo)) {
            continue;
        }
        if(HuPadBtnDown[padNo] & PAD_BUTTON_A) {
            result = 1;
            mbAudFXPlay(2);
            break;
        }
        if(HuPadBtnDown[padNo] & PAD_BUTTON_START) {
            result = 0;
            break;
        }
        if(wait != 0) {
            wait--;
        } else {
            newPage = page;
            newTop = top;
            if(HuPadBtnRep[padNo] & PAD_TRIGGER_L) {
                newPage--;
                if(newPage < 0) {
                    newPage += 6;
                }
                newTop = 0;
                wait = 8;
            } else if(HuPadBtnRep[padNo] & PAD_TRIGGER_R) {
                newPage++;
                if(newPage >= 6) {
                    newPage -= 6;
                }
                newTop = 0;
                wait = 8;
            } else if(HuPadDStkRep[padNo] & PAD_BUTTON_DOWN) {
                if(newTop + 8 < pauseMGList[page].num) {
                    newTop += 2;
                    pauseSingleWork.moveCnt[1] = 8;
                }
            } else if(HuPadDStkRep[padNo] & PAD_BUTTON_UP) {
                if(newTop > 0) {
                    newTop -= 2;
                    if(newTop < 0) {
                        newTop = 0;
                    }
                    pauseSingleWork.moveCnt[0] = 8;
                }
            }
            if(wait != 0) {
                pauseSingleWork.moveCnt[0] = pauseSingleWork.moveCnt[1] = 0;
            }
            if(page != newPage || top != newTop) {
                mbAudFXPlay(0);
                PauseSingleMGTypeSet(pauseMGList, &pauseSingleWork, newPage, newTop);
                page = newPage;
                top = newTop;
            }
        }
        for(i=0; i<2; i++) {
            scale = 1.0f;
            if(pauseSingleWork.growF[i]) {
                espDispOn(pauseSingleWork.sprId[i+12]);
                espTPLvlSet(pauseSingleWork.sprId[i+12], 1.0f);
            }
            if(pauseSingleWork.moveCnt[i]) {
                pauseSingleWork.moveCnt[i]--;
                target = pauseSingleWork.moveCnt[i] / 8.0f;
                scale = target;
                scale = 1.5f - (0.5f * mbCosDeg(90.0f * scale));
                if(!pauseSingleWork.growF[i]) {
                    espTPLvlSet(pauseSingleWork.sprId[i+12], mbSinDeg(90.0f * target));
                }
            } else if(!pauseSingleWork.growF[i]) {
                espDispOff(pauseSingleWork.sprId[i+12]);
            }
            espScaleSet(pauseSingleWork.sprId[i+12], scale, scale);
        }
    }
    for(n=0; n<14; n++) {
        espKill(pauseSingleWork.sprId[n]);
        pauseSingleWork.sprId[n] = -1;
    }
    for(n=0; n<2; n++) {
        mbWinKill(pauseSingleWork.mgWin[n]);
        pauseSingleWork.mgWin[n] = -1;
    }
    mbWinKill(pauseSingleWork.titleWin);
    pauseSingleWork.titleWin = -1;
    mbWinKill(pauseSingleWork.bgWin);
    pauseSingleWork.bgWin = -1;
    return result;
}
