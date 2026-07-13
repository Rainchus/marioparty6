#define _MATH_H

#include "game/board/window.h"
#include "game/board/main.h"
#include "game/board/pause.h"

#include "game/disp.h"
#include "game/flag.h"
#include "game/gamework.h"
#include "game/memory.h"
#include "game/pad.h"

#include "string.h"

typedef struct MBWinSizeData_s {
    int posX;
    int posY;
    int sizeX;
    int sizeY;
} MBWINSIZEDATA;

extern HUPROCESS *mbMainProc;
extern void *mbMalloc(s32 size);
extern void mbConfigPadDisableSet(BOOL disableF);
extern BOOL mbPlayerAllComCheck(void);

static BOOL mbWinOnF[MBWIN_MAX];
static s8 mbWinChoice[MBWIN_MAX];
static u8 mbWinStack[MBWIN_MAX];

static u8 mbWinTopNo;
static int mbWinNum;
static int mbWinNo;
static MBWIN *mbWinData;
static s8 mbWinLastChoice;

static const u8 speedTbl[4] = { 0, 1, 4, 0 };

static MBWINSIZEDATA winSizeTbl[MBWIN_TYPE_MAX] = {
    { 96, 328, 16, 2 },
    { 72, 128, 18, 6 },
    { 36, 344, 21, 3 },
    { 72, 328, 18, 3 },
    { 128, 312, 18, 4 },
    { 128, 312, 18, 3 },
    { 36, 352, 21, 3 },
    { 96, 416, 16, 2 },
    { 96, 328, 17, 3 },
    { 36, 344, 21, 3 },
};

static void KeyWaitInit(MBWIN *winP);
static void KeyWaitSet(MBWIN *winP, int keyWaitNum);
static void mbWinCenterSet(int winNo);

void mbWinInit(void)
{
    MBWIN *winP = &mbWinData[0];
    int i;

    HuWinInit(1);
    mbWinData = mbMalloc(sizeof(MBWIN) * MBWIN_MAX);
    for (i = 0; i < MBWIN_MAX; i++) {
        mbWinData[i].winId = HUWIN_NONE;
    }
    memset(mbWinOnF, 0, sizeof(mbWinOnF));
    mbWinNum = 0;
    mbWinNo = 0;
    mbWinTopNo = 0;
    mbPauseHookPush(mbWinPauseHook);
}

void mbWinClose(void)
{
    MBWIN *winP;

    mbWinKillAll();
    HuWinAllKill();
    winP = mbWinData;
    HuMemDirectFree(winP);
}

static void mbWinProc(void)
{
    HUPROCESS *proc = HuPrcCurrentGet();
    MBWIN *winP = proc->property;
    HuVec2f pos;
    HUWIN *huWinP;
    int i;

    mbWinCenterSet(winP->no);
    pos.x = winP->pos.x + winP->centerOfs.x;
    pos.y = winP->pos.y + winP->centerOfs.y;
    switch (winP->type) {
        case MBWIN_TYPE_HELP:
            winP->winId = HuWinCreate(pos.x, pos.y, winP->size.x, winP->size.y, 0);
            HuWinBGTPLvlSet(winP->winId, 0.0f);
            HuWinMesSpeedSet(winP->winId, 0);
            break;

        case MBWIN_TYPE_CAPSULE:
            winP->winId = HuWinCreate(pos.x, pos.y, winP->size.x, winP->size.y, winP->frame);
            HuWinMesSpeedSet(winP->winId, 0);
            break;

        case MBWIN_TYPE_PAUSEGUIDE:
            winP->winId = HuWinCreate(pos.x, pos.y, winP->size.x, winP->size.y, winP->frame);
            HuWinMesSpeedSet(winP->winId, 0);
            break;

        case MBWIN_TYPE_BLANK:
            winP->winId = HuWinCreate(pos.x, pos.y, winP->size.x, winP->size.y, 0);
            break;

        default:
            if (winP->speakerNo == HUWIN_SPEAKER_NULL) {
                winP->winId = HuWinExCreateFrame(pos.x, pos.y, winP->size.x, winP->size.y,
                                                  HUWIN_SPEAKER_NULL, winP->frame);
            } else if (winP->speakerNo == HUWIN_SPEAKER_SUN) {
                winP->winId = HuWinExCreateFrame(pos.x, pos.y, winP->size.x, winP->size.y,
                                                  winP->speakerNo, 4);
            } else if (winP->speakerNo == HUWIN_SPEAKER_MOON) {
                winP->winId = HuWinExCreateFrame(pos.x, pos.y, winP->size.x, winP->size.y,
                                                  winP->speakerNo, 3);
            } else {
                winP->winId = HuWinExCreateFrame(pos.x, pos.y, winP->size.x, winP->size.y,
                                                  winP->speakerNo, winP->frame);
            }
            if (winP->speakerNo == HUWIN_SPEAKER_NULL) {
                HuWinExOpen(winP->winId);
            } else {
                HuWinExOpen(winP->winId);
            }
            HuWinMesSpeedSet(winP->winId, winP->mesSpeed);
            break;
    }
    if (winP->prio != -1) {
        HuWinPriSet(winP->winId, winP->prio);
    }
    HuWinAttrSet(winP->winId, winP->attr);
    for (i = 0; i < HUWIN_INSERTMES_MAX; i++) {
        if (winP->insertMes[i] != MBWIN_MES_NONE) {
            HuWinInsertMesSet(winP->winId, winP->insertMes[i], i);
        }
    }
    if (winP->type != MBWIN_TYPE_BLANK) {
        HuWinMesSet(winP->winId, winP->mess);
    }
    HuWinPushKeySet(winP->winId, PAD_BUTTON_A);
    for (i = 0; i < HUWIN_CHOICE_MAX; i++) {
        if (winP->choiceDisable[i]) {
            HuWinChoiceDisable(winP->winId, i);
        }
    }
    mbWinPlayerDisable(winP->no, winP->playerNo);
    if (winP->playerNo < 0 || GwPlayer[winP->playerNo].comF) {
        KeyWaitInit(winP);
        if (winP->choiceF && winP->comKeyHook) {
            winP->comKeyHook();
        }
    }
    huWinP = &winData[winP->winId];
    while (huWinP->stat != HUWIN_STAT_NONE) {
        if ((huWinP->attr & HUWIN_ATTR_KEYWAIT_CLEAR)
            && !(winP->attr & HUWIN_ATTR_KEYWAIT_CLEAR)) {
            KeyWaitSet(winP, 1);
            winP->attr |= HUWIN_ATTR_KEYWAIT_CLEAR;
        }
        HuPrcVSleep();
    }
    if (winP->pauseF) {
        HuPrcSleep(-1);
    }
    if (winP->choiceNo != HUWIN_CHOICE_NONE) {
        mbWinChoice[winP->no] = mbWinLastChoice = HuWinChoiceGet(winP->winId, winP->choiceNo);
    }
    if (winP->speakerNo != HUWIN_SPEAKER_NULL) {
        HuWinExClose(winP->winId);
    }
    HuPrcEnd();
}

static void mbWinDestroy(void)
{
    HUPROCESS *proc = HuPrcCurrentGet();
    MBWIN *winP = proc->property;
    int i;

    if (winP->winId >= 0) {
        if (winP->speakerNo == HUWIN_SPEAKER_NULL) {
            HuWinExKill(winP->winId);
        } else {
            HuWinExKill(winP->winId);
            winP->speakerNo = HUWIN_SPEAKER_NULL;
        }
        winP->winId = HUWIN_NONE;
    }
    if (winP->choiceF) {
        mbConfigPadDisableSet(TRUE);
    }
    winP->mess = 0;
    winP->proc = NULL;
    if (mbWinOnF[winP->no]) {
        mbWinOnF[winP->no] = FALSE;
        for (i = 0; i < mbWinTopNo; i++) {
            if (mbWinStack[i] == winP->no) {
                break;
            }
        }
        for (; i < mbWinTopNo - 1; i++) {
            mbWinStack[i] = mbWinStack[i + 1];
        }
        mbWinTopNo--;
    }
    mbWinNum--;
}

static void mbWinCenterSet(int winNo)
{
    MBWIN *winP = &mbWinData[winNo];
    HuVec2f size;
    int i;

    for (i = 0; i < HUWIN_INSERTMES_MAX; i++) {
        if (winP->insertMes[i] != MBWIN_MES_NONE) {
            HuWinInsertMesSizeGet(winP->insertMes[i], i);
        }
    }
    HuWinMesMaxSizeGet(1, &size, winP->mess);
    if (winP->size.x < size.x) {
        winP->centerOfs.x = -(size.x - winP->size.x) * 0.5f;
        winP->size.x = size.x;
    }
    if (winP->size.y < size.y) {
        winP->centerOfs.y = -(size.y - winP->size.y) * 0.5f;
        winP->size.y = size.y;
    }
    if (winP->winId >= 0) {
        HuWinPosSet(winP->winId, winP->pos.x + winP->centerOfs.x,
                    winP->pos.y + winP->centerOfs.y);
    }
}

int mbWinCreate(int type, u32 mess, int speakerNo)
{
    MBWIN *winP;
    HuVec2f size;
    int i;

    winP = &mbWinData[1];
    for (i = 1; i < MBWIN_MAX; i++, winP++) {
        if (winP->proc == NULL) {
            break;
        }
    }
    if (i >= MBWIN_MAX) {
        return MBWIN_NONE;
    }
    memset(winP, 0, sizeof(MBWIN));
    winP->proc = HuPrcChildCreate(mbWinProc, 0x200F, 0x4000, 0, mbMainProc);
    HuPrcDestructorSet2(winP->proc, mbWinDestroy);
    winP->proc->property = winP;
    winP->winId = HUWIN_NONE;
    winP->mess = mess;
    winP->no = i;
    winP->attr = HUWIN_ATTR_NONE;
    winP->mesSpeed = GWMessSpeedGet();
    winP->prio = -1;
    winP->type = type;
    winP->speakerNo = speakerNo;
    winP->pauseF = winP->choiceF = FALSE;
    winP->choiceNo = HUWIN_CHOICE_NONE;
    winP->scale.x = winP->scale.y = 1.0f;
    winP->comKeyHook = NULL;
    winP->attr |= HUWIN_ATTR_NOCANCEL;
    winP->frame = 0;
    winP->playerNo = (_CheckFlag(FLAG_BOARD_TUTORIAL) == FALSE) ? GwSystem.turnPlayerNo : -1;
    for (i = 0; i < HUWIN_CHOICE_MAX; i++) {
        winP->choiceDisable[i] = FALSE;
    }
    for (i = 0; i < HUWIN_INSERTMES_MAX; i++) {
        winP->insertMes[i] = MBWIN_MES_NONE;
    }
    HuWinMesMaxSizeGet(1, &size, winP->mess);
    if (winP->type != MBWIN_TYPE_HELP) {
        winP->pos.x = winSizeTbl[winP->type].posX;
        winP->pos.y = winSizeTbl[winP->type].posY;
        winP->size.x = winSizeTbl[winP->type].sizeX * 24;
        winP->size.y = winSizeTbl[winP->type].sizeY * 32;
    } else {
        winP->pos.x = HU_DISP_CENTERX - ((size.x / 2) - 16.0f);
        winP->pos.y = 304.0f;
        winP->size = size;
    }
    winP->centerOfs.x = winP->centerOfs.y = 0.0f;
    mbWinChoice[winP->no] = 0;
    mbWinStack[mbWinTopNo++] = winP->no;
    winP->frame = 0;
    mbWinOnF[winP->no] = TRUE;
    mbWinNum++;
    return winP->no;
}

int mbWinCreateChoice(int type, u32 mess, int speakerNo, int choiceNo)
{
    int winNo = mbWinCreate(type, mess, speakerNo);

    if (winNo > 0) {
        MBWIN *winP = &mbWinData[winNo];
        winP->choiceF = TRUE;
        winP->choiceNo = choiceNo;
        winP->attr &= ~HUWIN_ATTR_NOCANCEL;
    }
    mbConfigPadDisableSet(FALSE);
    return winNo;
}

int mbWinCreateHelp(u32 mess)
{
    int winNo = mbWinCreate(MBWIN_TYPE_HELP, mess, HUWIN_SPEAKER_NULL);
    MBWIN *winP = &mbWinData[winNo];

    winP->pauseF = TRUE;
    return winNo;
}

int mbWinCreateFrame(int type, u32 mess, int speakerNo, int frame)
{
    int winNo = mbWinCreate(type, mess, speakerNo);

    if (winNo > 0) {
        MBWIN *winP = &mbWinData[winNo];
        winP->frame = frame;
    }
    return winNo;
}

int mbWinCreateTime(int type, u32 mess, int speakerNo)
{
    if (!GwSystem.curTime) {
        return mbWinCreateFrame(type, mess, speakerNo, 4);
    } else {
        return mbWinCreateFrame(type, mess, speakerNo, 3);
    }
}

int mbWinCreateBlank(void)
{
    int winNo = mbWinCreate(MBWIN_TYPE_BLANK, 0, HUWIN_SPEAKER_NULL);
    MBWIN *winP = &mbWinData[winNo];

    winP->pauseF = TRUE;
    return winNo;
}

int mbWinCreateBlankFrame(int frame)
{
    int winNo = mbWinCreateFrame(MBWIN_TYPE_BLANK, 0, HUWIN_SPEAKER_NULL, frame);
    MBWIN *winP = &mbWinData[winNo];

    winP->pauseF = TRUE;
    return winNo;
}

MBWIN *mbWinGet(int winNo)
{
    return &mbWinData[winNo];
}

void mbWinKill(s16 winNo)
{
    MBWIN *winP = &mbWinData[winNo];
    int i;

    if (winP->proc) {
        HuPrcKill(winP->proc);
    }
    mbWinOnF[winP->no] = FALSE;
    for (i = 0; i < mbWinTopNo; i++) {
        if (mbWinStack[i] == winP->no) {
            break;
        }
    }
    for (; i < mbWinTopNo - 1; i++) {
        mbWinStack[i] = mbWinStack[i + 1];
    }
    mbWinTopNo--;
}

void mbWinTopKill(void)
{
    mbWinKill(mbWinStack[mbWinTopNo - 1]);
}

void mbWinKillAll(void)
{
    MBWIN *winP = mbWinData;
    int i;

    for (i = 0; i < MBWIN_MAX; i++, winP++) {
        if (mbWinOnF[i]) {
            if (winP->proc) {
                HuPrcKill(winP->proc);
            }
            mbWinOnF[i] = FALSE;
        }
    }
    mbWinTopNo = 0;
}

void mbWinPosSet(s16 winNo, s16 posX, s16 posY)
{
    MBWIN *winP = &mbWinData[winNo];

    winP->pos.x = posX;
    winP->pos.y = posY;
    if (winP->winId >= 0) {
        HuWinPosSet(winP->winId, posX, posY);
    }
}

void mbWinTopPosSet(s16 posX, s16 posY)
{
    mbWinPosSet(mbWinStack[mbWinTopNo - 1], posX, posY);
}

void mbWinPosGet(s16 winNo, HuVec2f *pos)
{
    MBWIN *winP = &mbWinData[winNo];

    pos->x = winP->pos.x;
    pos->y = winP->pos.y;
}

void mbWinTopPosGet(HuVec2f *pos)
{
    mbWinPosGet(mbWinStack[mbWinTopNo - 1], pos);
}

void mbWinSizeSet(s16 winNo, s16 sizeX, s16 sizeY)
{
    MBWIN *winP = &mbWinData[winNo];
    HUWIN *huWinP;
    HUSPRITE *sprP;
    float bgTPLvl0;
    float bgTPLvl1;

    winP->size.x = sizeX;
    winP->size.y = sizeY;
    if (winP->winId >= 0) {
        huWinP = &winData[winP->winId];
        mbWinCenterSet(winNo);
        sizeX = ((s16)winP->size.x + 15) & 0xFFF0;
        sizeY = ((s16)winP->size.y + 15) & 0xFFF0;
        huWinP->winW = sizeX;
        huWinP->winH = sizeY;
        huWinP->mesRectX = 8;
        huWinP->mesRectY = 8;
        huWinP->mesRectW = sizeX - 8;
        huWinP->mesRectH = sizeY - 8;
        sprP = &HuSprData[HuSprGrpData[huWinP->grpId].sprId[0]];
        bgTPLvl0 = sprP->a;
        sprP = &HuSprData[HuSprGrpData[huWinP->grpId].sprId[1]];
        bgTPLvl1 = sprP->a;
        HuSprGrpCenterSet(huWinP->grpId, sizeX / 2, sizeY / 2);
        huWinP->charEntryMax = (sizeX / 8) * (sizeY / 24) * 5;
        if (huWinP->charEntry) {
            HuMemDirectFree(huWinP->charEntry);
            huWinP->charEntry = HuMemDirectMalloc(HEAP_HEAP,
                                                   sizeof(WINCHARENTRY) * huWinP->charEntryMax);
        }
        HuWinFrameSet(winP->winId, winP->frame);
        HuSprTPLvlSet(huWinP->grpId, 0, bgTPLvl0 / 255.0f);
        HuSprTPLvlSet(huWinP->grpId, 1, bgTPLvl1 / 255.0f);
    }
}

void mbWinTopSizeSet(s16 sizeX, s16 sizeY)
{
    mbWinSizeSet(mbWinStack[mbWinTopNo - 1], sizeX, sizeY);
}

void mbWinMesMaxSizeGet(s16 winNo, HuVec2f *size)
{
    MBWIN *winP = &mbWinData[winNo];

    HuWinMesMaxSizeGet(1, size, winP->mess);
}

void mbWinTopMesMaxSizeGet(HuVec2f *size)
{
    mbWinMesMaxSizeGet(mbWinStack[mbWinTopNo - 1], size);
}

void mbWinScaleSet(s16 winNo, float scaleX, float scaleY)
{
    MBWIN *winP = &mbWinData[winNo];

    winP->scale.x = scaleX;
    winP->scale.y = scaleY;
    if (winP->winId >= 0) {
        HuWinScaleSet(winP->winId, scaleX, scaleY);
    }
}

void mbWinTopScaleSet(float scaleX, float scaleY)
{
    mbWinScaleSet(mbWinStack[mbWinTopNo - 1], scaleX, scaleY);
}

void mbWinScaleGet(s16 winNo, HuVec2f *scale)
{
    MBWIN *winP = &mbWinData[winNo];

    scale->x = winP->scale.x;
    scale->y = winP->scale.y;
}

void mbWinTopScaleGet(HuVec2f *scale)
{
    mbWinScaleGet(mbWinStack[mbWinTopNo - 1], scale);
}

int mbWinTopChoiceGet(void)
{
    return mbWinLastChoice;
}

int mbWinChoiceGet(s16 winNo)
{
    return mbWinChoice[winNo];
}

void mbWinPause(s16 winNo)
{
    MBWIN *winP = &mbWinData[winNo];

    winP->pauseF = TRUE;
}

void mbWinTopPause(void)
{
    mbWinPause(mbWinStack[mbWinTopNo - 1]);
}

void mbWinInsertMesSet(s16 winNo, u32 insertMes, int insertMesNo)
{
    MBWIN *winP = &mbWinData[winNo];

    winP->insertMes[insertMesNo] = insertMes;
    if (winP->winId >= 0 && winP->insertMes[insertMesNo] != MBWIN_MES_NONE) {
        HuWinInsertMesSet(winP->winId, winP->insertMes[insertMesNo], insertMesNo);
    }
}

void mbWinTopInsertMesSet(u32 insertMes, int insertMesNo)
{
    mbWinInsertMesSet(mbWinStack[mbWinTopNo - 1], insertMes, insertMesNo);
}

BOOL mbWinDoneCheck(s16 winNo)
{
    return mbWinOnF[winNo] == FALSE;
}

BOOL mbWinTopDoneCheck(void)
{
    if (mbWinTopNo == 0) {
        return TRUE;
    }
    return mbWinDoneCheck(mbWinStack[mbWinTopNo - 1]);
}

void mbWinWait(s16 winNo)
{
    while (mbWinOnF[winNo]) {
        HuPrcVSleep();
    }
}

void mbWinTopWait(void)
{
    if (mbWinTopNo == 0) {
        return;
    }
    mbWinWait(mbWinStack[mbWinTopNo - 1]);
}

void mbWinAttrSet(s16 winNo, u32 attr)
{
    MBWIN *winP = &mbWinData[winNo];

    winP->attr |= attr;
    if (winP->winId >= 0) {
        HuWinAttrSet(winP->winId, winP->attr);
    }
}

void mbWinTopAttrSet(u32 attr)
{
    mbWinAttrSet(mbWinStack[mbWinTopNo - 1], attr);
}

void mbWinAttrReset(s16 winNo, u32 attr)
{
    MBWIN *winP = &mbWinData[winNo];

    winP->attr &= ~attr;
    if (winP->winId >= 0) {
        HuWinAttrSet(winP->winId, winP->attr);
    }
}

void mbWinTopAttrReset(u32 attr)
{
    mbWinAttrReset(mbWinStack[mbWinTopNo - 1], attr);
}

void mbWinChoiceDisable(s16 winNo, int choiceNo)
{
    MBWIN *winP = &mbWinData[winNo];

    winP->choiceDisable[choiceNo] = TRUE;
}

void mbWinTopChoiceDisable(int choiceNo)
{
    mbWinChoiceDisable(mbWinStack[mbWinTopNo - 1], choiceNo);
}

void mbWinMesSpeedSet(s16 winNo, int speed)
{
    MBWIN *winP = &mbWinData[winNo];

    winP->mesSpeed = speedTbl[speed];
    if (winP->winId >= 0) {
        HuWinMesSpeedSet(winP->winId, winP->mesSpeed);
    }
}

void mbWinTopMesSpeedSet(int speed)
{
    mbWinMesSpeedSet(mbWinStack[mbWinTopNo - 1], speed);
}

void mbWinMesColSet(s16 winNo, int mesCol)
{
    MBWIN *winP = &mbWinData[winNo];

    HuWinMesColSet(winP->winId, mesCol);
}

void mbWinTopMesColSet(int mesCol)
{
    mbWinMesColSet(mbWinStack[mbWinTopNo - 1], mesCol);
}

s16 mbWinChoiceNowGet(s16 winNo)
{
    MBWIN *winP = &mbWinData[winNo];

    return HuWinChoiceNowGet(winP->winId);
}

s16 mbWinTopChoiceNowGet(void)
{
    return mbWinChoiceNowGet(mbWinStack[mbWinTopNo - 1]);
}

void mbWinPriSet(s16 winNo, s16 prio)
{
    MBWIN *winP = &mbWinData[winNo];

    winP->prio = prio;
    if (winP->winId >= 0) {
        HuWinPriSet(winP->winId, winP->prio);
    }
}

void mbWinTopPriSet(s16 prio)
{
    mbWinPriSet(mbWinStack[mbWinTopNo - 1], prio);
}

s16 mbWinChoiceNowGet2(s16 winNo)
{
    MBWIN *winP = &mbWinData[winNo];

    if (winP->winId < 0) {
        return -1;
    }
    return HuWinChoiceNowGet(winP->winId);
}

s16 mbWinTopChoiceNowGet2(void)
{
    return mbWinChoiceNowGet2(mbWinStack[mbWinTopNo - 1]);
}

void mbWinPlayerDisable(s16 winNo, int playerNo)
{
    MBWIN *winP = &mbWinData[winNo];
    u8 disablePlayer;
    int i;

    if (_CheckFlag(FLAG_BOARD_TUTORIAL)) {
        winP->playerNo = 0;
        disablePlayer = ~HUWIN_PLAYER_1;
    } else {
        winP->playerNo = playerNo;
        if (winP->playerNo == -1) {
            disablePlayer = HUWIN_PLAYER_ALL;
            for (i = 0; i < GW_PLAYER_MAX; i++) {
                if (GwPlayer[i].comF == FALSE) {
                    disablePlayer &= ~(1 << GwPlayer[i].padNo);
                }
            }
        } else if (GwPlayer[winP->playerNo].comF) {
            disablePlayer = HUWIN_PLAYER_ALL;
        } else {
            disablePlayer = ~(1 << GwPlayer[winP->playerNo].padNo);
        }
    }
    if (winP->winId >= 0) {
        HuWinDisablePlayerReset(winP->winId, 0xFF);
        HuWinDisablePlayerSet(winP->winId, disablePlayer);
    }
}

void mbWinTopPlayerDisable(int playerNo)
{
    mbWinPlayerDisable(mbWinStack[mbWinTopNo - 1], playerNo);
}

static void KeyWaitInit(MBWIN *winP)
{
    int waitNum;

    if (_CheckFlag(FLAG_BOARD_TUTORIAL)) {
        return;
    }
    waitNum = HuWinKeyWaitNumGet(winP->mess);
    if (waitNum) {
        HuWinComKeyReset();
        KeyWaitSet(winP, waitNum);
    }
}

static void KeyWaitSet(MBWIN *winP, int keyWaitNum)
{
    s32 key[GW_PLAYER_MAX] = {};
    int i;
    int delay;

    if (keyWaitNum == 0) {
        return;
    }
    if (mbPlayerAllComCheck()) {
        key[0] = key[1] = key[2] = key[3] = PAD_BUTTON_A;
    } else {
        if (winP->playerNo == -1) {
            return;
        }
        if (!GwPlayer[winP->playerNo].comF) {
            return;
        }
        for (i = 0; i < GW_PLAYER_MAX; i++) {
            int padNo = GwPlayer[i].padNo;
            if (winP->playerNo == i) {
                key[padNo] |= PAD_BUTTON_A;
            }
        }
    }
    delay = GWComKeyDelayGet() * 1.5f;
    for (i = 0; i < keyWaitNum; i++) {
        HuWinComKeyWait(key[0], key[1], key[2], key[3], delay);
    }
}

void mbWinTopComKeyHookSet(MBWINCOMKEYHOOK comKeyHook)
{
    s16 winNo = mbWinStack[mbWinTopNo - 1];
    MBWIN *winP = &mbWinData[winNo];

    winP->comKeyHook = comKeyHook;
}

void mbWinDispSet(s16 winNo, BOOL dispF)
{
    MBWIN *winP = &mbWinData[winNo];

    if (winP->proc == NULL || winP->winId < 0) {
        return;
    }
    if (dispF) {
        HuWinDispOn(winP->winId);
    } else {
        HuWinDispOff(winP->winId);
    }
}

void mbWinTopDispSet(BOOL dispF)
{
    mbWinDispSet(mbWinStack[mbWinTopNo - 1], dispF);
}

void mbWinPauseHook(BOOL dispF)
{
    int i;

    for (i = 1; i < MBWIN_MAX; i++) {
        if (mbWinOnF[i]) {
            mbWinDispSet(i, dispF);
        }
    }
}

HUWINID mbWinIDGet(s16 winNo)
{
    MBWIN *winP = &mbWinData[winNo];

    return winP->winId;
}

HUWINID mbWinTopIDGet(void)
{
    return mbWinIDGet(mbWinStack[mbWinTopNo - 1]);
}

void mbWinCenterGet(s16 winNo, HuVec2f *pos)
{
    MBWIN *winP = &mbWinData[winNo];
    HuVec2f size;

    mbWinMesMaxSizeGet(winNo, &size);
    if (winP->size.x > size.x) {
        size.x = winP->size.x;
    }
    if (winP->size.y > size.y) {
        size.y = winP->size.y;
    }
    pos->x = HU_DISP_CENTERX - (size.x / 2);
    pos->y = HU_DISP_CENTERY - (size.y / 2);
}

void mbWinCenterInsertGet(s16 winNo, u32 mess)
{
    MBWIN *winP = &mbWinData[winNo];
    int i;

    winP->mess = mess;
    if (winP->winId >= 0) {
        for (i = 0; i < HUWIN_INSERTMES_MAX; i++) {
            if (winP->insertMes[i] != MBWIN_MES_NONE) {
                HuWinInsertMesSet(winP->winId, winP->insertMes[i], i);
            }
        }
        HuWinMesSet(winP->winId, winP->mess);
    }
    mbWinCenterSet(winNo);
}

void mbWinTopCenterInsertGet(u32 mess)
{
    mbWinCenterInsertGet(mbWinStack[mbWinTopNo - 1], mess);
}
