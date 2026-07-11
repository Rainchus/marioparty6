/* Block math.h's extern inline sqrtf: its weak _half/_three statics would
 * prepend 16 bytes to .sdata2 that the original mgtimer.o does not have. */
#define _MATH_H

#include "game/gamemes.h"
#include "game/memory.h"
#include "game/process.h"
#include "game/main.h"
#include "game/mg/timer.h"
#include "datanum/mgconst.h"


const static s16 MinuteLenTbl[4] = {
    0, 60, 100, 60
};

const static s16 boxSizeTbl[MGTIMER_TYPE_MAX][2] = {
    { 40, 32 },
    { 152, 64 },
    { 104, 32 },
    { 134, 32 }
};

const static s16 timerPosTbl[MGTIMER_TYPE_MAX][2] = {
    { 288, 64 },
    { 292, 70 },
    { 292, 64 },
    { 292, 64 }
};

typedef void (*TIMERFUNC)(MGTIMER *timer);

static void TimerExec();
static void TimerExecOn(MGTIMER* timer);
static void TimerExecOff(MGTIMER* timer);
static void CreateSprite(MGTIMER* timer);
static void KillSprite(MGTIMER* timer);
static void UpdateSprite(MGTIMER* timer);


static const HuVec2f digitOfsTbl[MGTIMER_TYPE_MAX][15] = {
    {
        {   0.0f,   0.0f}, {   0.0f,   0.0f}, {   0.0f,   0.0f}, {   0.0f,   0.0f},
        {   0.0f,   0.0f}, {   0.0f,   0.0f}, {   0.0f,   0.0f}, {   0.0f,   0.0f},
        {   0.0f,   0.0f}, {   0.0f,   0.0f}, {   0.0f,   0.0f}, {   0.0f,   0.0f},
        {   0.0f,   0.0f}, {   0.0f,   0.0f}, {   0.0f,   0.0f}
    },
    {
        { -46.0f,   0.0f}, { -30.0f,   0.0f}, { -14.0f,   0.0f}, {   2.0f,   0.0f},
        {  18.0f,   0.0f}, {  34.0f,   0.0f}, {  50.0f,   0.0f}, { -46.0f, -16.0f},
        { -30.0f, -16.0f}, { -14.0f, -16.0f}, {   2.0f, -16.0f}, {  18.0f, -16.0f},
        {  34.0f, -16.0f}, {  50.0f, -16.0f}, { -64.0f, -16.0f},
    },
    {
        {-900.0f, -900.0f}, {-900.0f, -900.0f}, { -39.0f,  -8.0f}, { -23.0f,  -8.0f},
        {  -7.0f,  -8.0f}, {   9.0f,  -8.0f}, {  25.0f,  -8.0f}, {-900.0f, -900.0f},
        {-900.0f, -900.0f}, {-900.0f, -900.0f}, {-900.0f, -900.0f}, {-900.0f, -900.0f},
        {-900.0f, -900.0f}, {-900.0f, -900.0f}, {-900.0f, -900.0f}
    },
    {
        { -55.0f,  -8.0f}, { -39.0f,  -8.0f}, { -23.0f,  -8.0f}, {  -7.0f,  -8.0f},
        {   9.0f,  -8.0f}, {  25.0f,  -8.0f}, {  41.0f,  -8.0f}, {-900.0f, -900.0f},
        {-900.0f, -900.0f}, {-900.0f, -900.0f}, {-900.0f, -900.0f}, {-900.0f, -900.0f},
        {-900.0f, -900.0f}, {-900.0f, -900.0f}, {-900.0f, -900.0f}
    },
};

static TIMERFUNC modeTbl[MGTIMER_MODE_MAX] = { TimerExecOff, TimerExecOn };

MGTIMER* MgTimerCreate(int type) {
    MGTIMER* timer;
    int i;
    timer = HuMemDirectMallocNum(HEAP_HEAP, sizeof(MGTIMER), HU_MEMNUM_OVL);
    if (timer == NULL) {
        return NULL;
    }
    timer->type = type;
    timer->mode = MGTIMER_MODE_OFF;
    timer->prevMode = MGTIMER_MODE_OFF;
    timer->dispOnF = FALSE;
    timer->boxDispF = TRUE;
    timer->stopF = FALSE;
    timer->scaleDir = 0;
    timer->flashFlag = 0;
    timer->time = 0;
    timer->maxTime = 0;
    timer->recordTime = 0;
    timer->endTime = 0;
    timer->speed = 0;
    timer->pos.x = timerPosTbl[type][0];
    timer->pos.y = timerPosTbl[type][1];
    timer->gameMesId = GAMEMES_ID_NONE;
    timer->boxId = HUSPR_GROUP_NONE;
    timer->digitR = 255;
    timer->digitG = 255;
    timer->digitB = 255;
    timer->digitScale = 1.0f;
    timer->timeUnit = MinuteLenTbl[type];
    timer->fadeOutTime = 0;
    for (i = 0; i < 15; i++) {
        timer->espId[i] = -1;
    }
    CreateSprite(timer);
    switch (type) {
        case MGTIMER_TYPE_NORMAL:
            break;

        case MGTIMER_TYPE_RECORD:
        case MGTIMER_TYPE_SCORE:
        case MGTIMER_TYPE_WIDESCORE:
            timer->boxId = MgScoreBoxCreate(boxSizeTbl[timer->type][0], boxSizeTbl[timer->type][1]);
            MgScoreBoxDispSet(timer->boxId, FALSE);
            break;

    }
    MgTimerColorSet(timer, 255, 255, 255);
    UpdateSprite(timer);
    timer->proc = HuPrcChildCreate(TimerExec, 0x1000, 0x1200, 0, HuPrcCurrentGet());
    timer->proc->property = timer;
    return timer;
}

void MgTimerKill(MGTIMER* timer) {
    HuPrcKill(timer->proc);
    KillSprite(timer);
    if (timer->boxId != HUSPR_GROUP_NONE) {
        MgScoreBoxKill(timer->boxId);
    }
    if (timer->gameMesId != GAMEMES_ID_NONE) {
        GameMesKill(timer->gameMesId);
    }
    HuMemDirectFree(timer);
}

int MgTimerModeGet(MGTIMER* timer) {
    return timer->mode;
}

void MgTimerParamSet(MGTIMER* timer, int maxTime, int endTime, int recordTime) {
    timer->maxTime = maxTime;
    timer->time = timer->maxTime;
    timer->endTime = endTime;
    timer->recordTime = recordTime;
    timer->speed = (maxTime == endTime) ? 0 : ((maxTime > endTime) ? -1 : 1);
    UpdateSprite(timer);
}

int MgTimerValueGet(MGTIMER* timer) {
    return timer->time;
}

void MgTimerPosSet(MGTIMER *timer, float posX, float posY) {
    timer->pos.x = posX;
    timer->pos.y = posY;
    UpdateSprite(timer);
}

void MgTimerPosGet(MGTIMER* timer, float *posX, float *posY) {
    *posX = timer->pos.x;
    *posY = timer->pos.y;
}

void MgTimerModeOnSet(MGTIMER* timer, int offType) {
    timer->offType = offType;
    timer->stopF = FALSE;
    timer->mode = MGTIMER_MODE_ON;
}

void MgTimerModeOffSet(MGTIMER* timer) {
    timer->mode = MGTIMER_MODE_OFF;
}

BOOL MgTimerDoneCheck(MGTIMER* timer) {
    return timer->endTime == timer->time;
}

void MgTimerRecordSet(MGTIMER* timer, int recordTime) {
    if (recordTime == -1) {
        if (((timer->speed > 0) && (timer->time < timer->recordTime)) || ((timer->speed < 0) && (timer->time > timer->recordTime))) {
            timer->recordTime = timer->time;
            timer->flashFlag = timer->flashFlag | MGTIMER_FLASH_RECORD;
        }
    } else {
        timer->recordTime = recordTime;
        timer->flashFlag = timer->flashFlag | MGTIMER_FLASH_RECORD;
    }
}

void MgTimerColorSet(MGTIMER* timer, u8 r, u8 g, u8 b) {
    int i;
    timer->digitR = r;
    timer->digitG = g;
    timer->digitB = b;
    for (i = 0; i <= 6; i++) {
        if (timer->espId[i] != -1) {
            espColorSet(timer->espId[i], r, g, b);
        }
    }
}

void MgTimerBackColorSet(MGTIMER* timer, u8 r, u8 g, u8 b) {
    if (timer->boxId != HUSPR_GROUP_NONE) {
        MgScoreBoxColorSet(timer->boxId, r, g, b);
    }
}

static void TimerExec(void) {
    MGTIMER* timer;

    timer = HuPrcCurrentGet()->property;
    while (1) {
        modeTbl[timer->mode](timer);
    }
}

static void TimerExecOff(MGTIMER* timer) {
    int i;
    s16 mode;
    int maxCheck;
    s16 prevMode;

    while (1) {
        UpdateSprite(timer);
        for(maxCheck=1, i=0; i<maxCheck; i++) {
            HuPrcVSleep();
            mode = ((MGTIMER *)HuPrcCurrentGet()->property)->mode;
            prevMode = ((MGTIMER *)HuPrcCurrentGet()->property)->prevMode;
            ((MGTIMER *)HuPrcCurrentGet()->property)->prevMode = mode;
            if (mode != prevMode) {
                return;
            }
        }
    }
}

static void TimerExecOn(MGTIMER* timer) {
    int i;
    s16 mode;
    int maxCheck;
    s16 prevMode;

    if (timer->dispOnF == 0) {
        MgTimerDispOn(timer);
    }
    while ((timer->time != timer->endTime) && (timer->stopF == 0)) {
        timer->time = timer->time + timer->speed;
        UpdateSprite(timer);
        for(maxCheck=1, i=0; i<maxCheck; i++) {
            HuPrcVSleep();
            mode = ((MGTIMER*)HuPrcCurrentGet()->property)->mode;
            prevMode = ((MGTIMER*)HuPrcCurrentGet()->property)->prevMode;
            ((MGTIMER*)HuPrcCurrentGet()->property)->prevMode = mode;
            if (mode != prevMode) {
                return;
            }
        }
    }
    timer->stopF = TRUE;
    UpdateSprite(timer);
    timer->fadeOutTime = 0;
    switch (timer->offType) {
        case MGTIMER_OFFTYPE_NORMAL:
            timer->mode = MGTIMER_MODE_OFF;
            return;

        case MGTIMER_OFFTYPE_FADEOUT:
            if (timer->type == MGTIMER_TYPE_NORMAL) {
                MgTimerDispOff(timer);
            } else {
                timer->flashFlag = timer->flashFlag | MGTIMER_FLASH_FADEOUT;
            }
            timer->mode = MGTIMER_MODE_OFF;
            return;

        case MGTIMER_OFFTYPE_FLASH:
            timer->flashFlag = timer->flashFlag | MGTIMER_FLASH_COLOR;
            timer->mode = MGTIMER_MODE_OFF;
            break;
    }
}

static void CreateSprite(MGTIMER* timer) {
    int i;

    switch (timer->type) {
        case MGTIMER_TYPE_NORMAL:
            break;

        case MGTIMER_TYPE_RECORD:
        case MGTIMER_TYPE_SCORE:
        case MGTIMER_TYPE_WIDESCORE:
            for (i = 0; i < 7; i++) {
                timer->espId[i] = espEntry(MGCONST_ANM_scoreSmall, 0, 0);
                espColorSet(timer->espId[i], 255, 255, 255);
            }
            for (i = 0; i < 7; i++) {
                timer->espId[i + 7] = espEntry(MGCONST_ANM_scoreSmall, 0, 0);
                espColorSet(timer->espId[i + 7], 66, 255, 122);
            }
            timer->espId[14] = espEntry(MGCONST_ANM_crown, 0, 0);
            espBankSet(timer->espId[1], 10);
            espBankSet(timer->espId[4], 11);
            espBankSet(timer->espId[8], 10);
            espBankSet(timer->espId[11], 11);
            for (i = 0; i < 15; i++) {
                if (timer->espId[i] != -1) {
                    espDispOff(timer->espId[i]);
                    espPriSet(timer->espId[i], 9);
                }
            }
            break;
    }
}

static void KillSprite(MGTIMER* timer) {
    int i;
    for (i = 0; i < 15; i++) {
        if (timer->espId[i] != -1) {
            espKill(timer->espId[i]);
            timer->espId[i] = -1;
        }
    }
}

static void UpdateSprite(MGTIMER* timer) {
    int i;
    s32 second;
    s32 centisecs;
    s32 minute;
    s32 time;
    u8 r;
    u8 g;
    u8 b;
    float tpLvl;

    switch (timer->type) {
        case MGTIMER_TYPE_NORMAL:
            if ((timer->time % 60) == 0) {
                GameMesTimerValueSet(timer->gameMesId, timer->time / 60);
            }
            GameMesPosSet(timer->gameMesId, timer->pos.x, timer->pos.y);
            break;

        case MGTIMER_TYPE_RECORD:
        case MGTIMER_TYPE_SCORE:
        case MGTIMER_TYPE_WIDESCORE:
            for (i = 0; i < 15; i++) {
                if (timer->espId[i] != -1) {
                    espPosSet(timer->espId[i],  timer->pos.x + digitOfsTbl[timer->type][i].x, timer->pos.y + digitOfsTbl[timer->type][i].y);
                }
            }
            if (timer->boxId != -1) {
                MgScoreBoxPosSet(timer->boxId, timer->pos.x - 8.0f, timer->pos.y - 8.0f);
            }
            if (timer->time >= 35999) {
                minute = 9;
                second = 59;
                centisecs = 99;
            } else {
                time = (timer->time >= 35999) ? 35999 : timer->time;
                minute = time / (timer->timeUnit * 60);
                second = (time % (timer->timeUnit * 60)) / 60;
                centisecs = 1.6666666f * ((time % 3600) % 60);
            }
            espBankSet(timer->espId[0], minute % 10);
            espBankSet(timer->espId[2], second / 10);
            espBankSet(timer->espId[3], second % 10);
            espBankSet(timer->espId[5], centisecs / 10);
            espBankSet(timer->espId[6], centisecs % 10);

            minute = timer->recordTime / (timer->timeUnit * 60);
            second = (timer->recordTime % (timer->timeUnit * 60)) / 60;
            centisecs = 1.6666666f * ((timer->recordTime % 3600) % 60);
            espBankSet(timer->espId[7], minute % 10);
            espBankSet(timer->espId[9], second / 10);
            espBankSet(timer->espId[10], second % 10);
            espBankSet(timer->espId[12], centisecs / 10);
            espBankSet(timer->espId[13],  centisecs - (centisecs / 10 * 10));
            if (timer->flashFlag & MGTIMER_FLASH_COLOR) {
                if (((GlobalCounter / 10) & 1) == 0) {
                    r = timer->digitR;
                    g = timer->digitG;
                    b = timer->digitB;
                } else {
                    r = 255;
                    g = 216;
                    b = 0;
                }
                for (i = 0; i < 7; i++) {
                    if (timer->espId[i] != -1) {
                        espColorSet(timer->espId[i], r, g, b);
                    }
                }
            }
            if (timer->flashFlag & MGTIMER_FLASH_RECORD) {
                if (timer->scaleDir) {
                    if ((timer->digitScale += 0.02f) >= 1.2f) {
                        timer->digitScale = 1.2f;
                        timer->scaleDir = 0;
                    }
                } else {
                    if ((timer->digitScale -= 0.04f) <= 1.0f) {
                        timer->digitScale = 1.0f;
                        timer->scaleDir = 1;
                    }
                }
                for (i = 0; i < 8; i++) {
                    if (timer->espId[i+7] != -1) {
                        espScaleSet(timer->espId[i+7], timer->digitScale, timer->digitScale);
                    }
                }
            }
            if (timer->flashFlag & MGTIMER_FLASH_FADEOUT) {
                tpLvl = 1.0f - (0.016666668f * timer->fadeOutTime);
                for (i = 0; i < 15; i++) {
                    if (timer->espId[i] != -1) {
                        espTPLvlSet(timer->espId[i], tpLvl);
                    }
                    if (timer->boxId != -1) {
                        MgScoreBoxTPLvlSet(timer->boxId, tpLvl);
                    }
                }
                if (timer->fadeOutTime++ >= 60) {
                    timer->flashFlag = timer->flashFlag & ~MGTIMER_FLASH_FADEOUT;
                    MgTimerDispOff(timer);
                }
            }
    }
    (void)second;
}

void MgTimerDispOn(MGTIMER* timer) {
    int i;

    switch (timer->type) {
        case MGTIMER_TYPE_NORMAL:
            if (timer->gameMesId == GAMEMES_ID_NONE) {
                timer->gameMesId = GameMesTimerCreate(timer->time / 60);
                GameMesPosSet(timer->gameMesId, timer->pos.x, timer->pos.y);
            }
            break;

        case MGTIMER_TYPE_SCORE:
        case MGTIMER_TYPE_RECORD:
        case MGTIMER_TYPE_WIDESCORE:
            for (i = 0; i < 15; i++) {
                if (timer->espId[i] != -1) {
                    espDispOn(timer->espId[i]);
                }
            }
            if (timer->boxId != -1) {
                MgScoreBoxDispSet(timer->boxId, timer->boxDispF);
            }
            break;

    }
    timer->dispOnF = TRUE;
}

void MgTimerDispOff(MGTIMER* timer) {
    int i;

    switch (timer->type) {
        case MGTIMER_TYPE_NORMAL:
            if (timer->gameMesId != GAMEMES_ID_NONE) {
                GameMesTimerEnd(timer->gameMesId);
                timer->gameMesId = GAMEMES_ID_NONE;
            }
            break;

        case MGTIMER_TYPE_RECORD:
        case MGTIMER_TYPE_SCORE:
        case MGTIMER_TYPE_WIDESCORE:
            for (i = 0; i < 15; i++) {
                if (timer->espId[i] != -1) {
                    espDispOff(timer->espId[i]);
                }
            }
            if (timer->boxId != -1) {
                MgScoreBoxDispSet(timer->boxId, FALSE);
            }
            break;
    }
    timer->dispOnF = FALSE;
}

void MgTimerRecordDispOn(MGTIMER* timer) {
    int i;

    switch (timer->type) {
        case MGTIMER_TYPE_NORMAL:
            if (timer->gameMesId == GAMEMES_ID_NONE) {
                timer->gameMesId = GameMesTimerCreate(timer->time / 60);
                GameMesPosSet(timer->gameMesId, timer->pos.x, timer->pos.y);
            }
            break;

        case MGTIMER_TYPE_RECORD:
        case MGTIMER_TYPE_SCORE:
        case MGTIMER_TYPE_WIDESCORE:
            for (i = 0; i < 15; i++) {
                if (timer->espId[i] != -1) {
                    espDispOn(timer->espId[i]);
                }
            }
            if (timer->boxId != HUSPR_GROUP_NONE) {
                MgScoreBoxDispSet(timer->boxId, timer->boxDispF);
            }
            break;
    }
    timer->dispOnF = TRUE;
}

void MgTimerRecordDispOff(MGTIMER* timer) {
    int i;

    switch (timer->type) {
        case MGTIMER_TYPE_NORMAL:
            if(timer->gameMesId != GAMEMES_ID_NONE) {
                GameMesTimerEnd(timer->gameMesId);
                timer->gameMesId = GAMEMES_ID_NONE;
            }
            break;

        case MGTIMER_TYPE_RECORD:
        case MGTIMER_TYPE_SCORE:
        case MGTIMER_TYPE_WIDESCORE:
            for(i = 0; i < 15; i++) {
                if (timer->espId[i] != -1) {
                    espDispOff(timer->espId[i]);
                }
            }
            if(timer->boxId != -1) {
                MgScoreBoxDispSet(timer->boxId, FALSE);
            }
    }

    timer->dispOnF = FALSE;
}
