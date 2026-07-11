/* Block math.h's extern inline sqrtf: its weak _half/_three statics would
 * prepend 16 bytes to .sdata2 that the original seqman.o does not have. */
#define _MATH_H

#include "game/mg/seqman.h"
#include "game/mg/timer.h"
#include "game/gamemes.h"
#include "game/process.h"
#include "game/audio.h"
#include "game/object.h"
#include "game/flag.h"
#include "game/wipe.h"

typedef struct MgSeqWork_s {
    MGSEQ_PARAM param;                       // 0x00
    MGTIMER *timer;                          // 0x28
    u16 stat;                                // 0x2C
    u16 unk2E;                               // 0x2E
    u16 mode;                                // 0x30
    u16 unk32;                               // 0x32
    int recordVal;                           // 0x34
    s16 winner[4];                           // 0x38
    s16 modeDelay[MGSEQ_MODE_MAX];           // 0x40
    u16 hookNum;                             // 0x58
    MGSEQ_MODEHOOK hook[MGSEQ_MODEHOOK_MAX]; // 0x5C
    s32 unk25C;                              // 0x25C
} MGSEQ_WORK;

static void SeqExecProc(void);
static void SeqExit(void);
static void SeqExecModeHooks(s16 mode, s16 frameNo);
static void SeqSetTimerPos(s32 pos, MGTIMER *timer);

static s16 defModeDelayTbl[MGSEQ_MODE_MAX] = {
    -1, -1, 240, -1, -1, -1, -1, 240, 210, 60, -1, -1
};

static MGSEQ_WORK seqWork;

static HUPROCESS *seqProc;
static u32 seqFrameNo;
static GAMEMESID seqGameMesId;

void MgSeqCreate(MGSEQ_PARAM *param)
{
    MgSeqCreatePrio(param, 100);
}

void MgSeqCreatePrio(MGSEQ_PARAM *param, s32 prio)
{
    s16 i;

    seqWork.param = *param;
    seqWork.stat = 0;
    seqWork.unk32 = 0;
    seqWork.mode = MGSEQ_MODE_NONE;
    seqWork.timer = NULL;
    seqWork.hookNum = 0;
    for (i = 0; i < MGSEQ_MODE_MAX; i++) {
        if (defModeDelayTbl[i] == -1) {
            seqWork.modeDelay[i] = -1;
        } else {
            seqWork.modeDelay[i] = (s16)(s32)(f64)defModeDelayTbl[i];
        }
    }
    seqProc = HuPrcChildCreate(SeqExecProc, prio, 0x5000, 0, HuPrcCurrentGet());
    HuPrcChildCreate(SeqExit, prio, 0x3000, 0, seqProc);
    seqGameMesId = GAMEMES_ID_NONE;
}

void MgSeqKill(void)
{
    seqWork.mode = MGSEQ_MODE_NONE;
    HuPrcKill(seqProc);
}

static void SeqExecProc(void)
{
    s32 delay;
    MGSEQ_FUNC hook;

    do {
        if (seqWork.mode == MGSEQ_MODE_NONE) {
            seqWork.mode = MGSEQ_MODE_INIT;
            delay = 0;
            if (seqWork.modeDelay[MGSEQ_MODE_INIT] == -1) {
                seqWork.stat |= MGSEQ_STAT_MODECHANGE_OFF;
            } else {
                delay = seqWork.modeDelay[MGSEQ_MODE_INIT];
            }
            seqFrameNo = 0;
            while (seqFrameNo < delay || (seqWork.stat & MGSEQ_STAT_MODECHANGE_OFF)) {
                if (seqWork.param.initHook) {
                    hook = seqWork.param.initHook;
                    hook(seqWork.mode, seqFrameNo);
                }
                SeqExecModeHooks(seqWork.mode, seqFrameNo);
                if (seqWork.stat & MGSEQ_STAT_MODENEXT) {
                    seqWork.stat &= ~MGSEQ_STAT_MODENEXT;
                    break;
                }
                HuPrcVSleep();
                if (seqWork.stat & MGSEQ_STAT_MODENEXT) {
                    seqWork.stat &= ~MGSEQ_STAT_MODENEXT;
                    break;
                }
                seqFrameNo++;
            }
        }
        seqWork.stat &= ~MGSEQ_STAT_MODECHANGE_OFF;
        if (seqWork.mode == MGSEQ_MODE_INIT || seqWork.mode == MGSEQ_MODE_FADEIN) {
            seqWork.mode = MGSEQ_MODE_FADEIN;
            if (!(seqWork.stat & MGSEQ_STAT_FADEIN_OFF)) {
                WipeCreate(WIPE_MODE_IN, WIPE_TYPE_PREV, 60);
            }
            delay = 0;
            if (seqWork.modeDelay[MGSEQ_MODE_FADEIN] == -1) {
                seqWork.stat |= MGSEQ_STAT_MODECHANGE_OFF;
            } else {
                delay = seqWork.modeDelay[MGSEQ_MODE_FADEIN];
            }
            seqFrameNo = 0;
            while (seqFrameNo < delay || (seqWork.stat & MGSEQ_STAT_MODECHANGE_OFF)) {
                if (seqWork.param.fadeInHook) {
                    hook = seqWork.param.fadeInHook;
                    hook(seqWork.mode, seqFrameNo);
                }
                SeqExecModeHooks(seqWork.mode, seqFrameNo);
                if (seqWork.stat & MGSEQ_STAT_MODENEXT) {
                    seqWork.stat &= ~MGSEQ_STAT_MODENEXT;
                    break;
                }
                HuPrcVSleep();
                seqFrameNo++;
            }
        }
        seqWork.stat &= ~MGSEQ_STAT_MODECHANGE_OFF;
        if (seqWork.mode == MGSEQ_MODE_FADEIN || seqWork.mode == MGSEQ_MODE_START) {
            seqWork.mode = MGSEQ_MODE_START;
            seqGameMesId = GameMesCreate(GAMEMES_MES_MG, GAMEMES_MG_TYPE_START);
            if (seqWork.param.maxTime != 0 && seqWork.param.maxTime != 300) {
                seqWork.timer = MgTimerCreate(MGTIMER_POS_TOP);
                MgTimerParamSet(seqWork.timer, seqWork.param.maxTime * 60, 0, 0);
                SeqSetTimerPos(seqWork.param.timerPos, seqWork.timer);
            }
            seqFrameNo = 0;
            while (GameMesStatGet(seqGameMesId)) {
                if (seqWork.param.startHook) {
                    hook = seqWork.param.startHook;
                    hook(seqWork.mode, seqFrameNo);
                }
                SeqExecModeHooks(seqWork.mode, seqFrameNo);
                if (seqWork.stat & MGSEQ_STAT_MODENEXT) {
                    seqWork.stat &= ~MGSEQ_STAT_MODENEXT;
                    break;
                }
                HuPrcVSleep();
                seqFrameNo++;
            }
            if (seqWork.timer) {
                MgTimerModeOnSet(seqWork.timer, 1);
            }
            seqFrameNo = 0;
            SeqExecModeHooks(MGSEQ_MODE_PREMAIN, seqFrameNo);
        }
        seqWork.stat &= ~MGSEQ_STAT_MODECHANGE_OFF;
        if (seqWork.mode == MGSEQ_MODE_START || seqWork.mode == MGSEQ_MODE_MAIN) {
            seqWork.mode = MGSEQ_MODE_MAIN;
            seqFrameNo = 0;
            while (1) {
                if (seqWork.param.mainHook) {
                    hook = seqWork.param.mainHook;
                    hook(seqWork.mode, seqFrameNo);
                }
                SeqExecModeHooks(seqWork.mode, seqFrameNo);
                if (seqWork.param.maxTime == 300 && seqFrameNo == 16200) {
                    seqWork.timer = MgTimerCreate(MGTIMER_POS_TOP);
                    MgTimerParamSet(seqWork.timer, 1800, 0, 0);
                    SeqSetTimerPos(seqWork.param.timerPos, seqWork.timer);
                    MgTimerModeOnSet(seqWork.timer, 1);
                }
                if ((seqWork.timer && MgTimerDoneCheck(seqWork.timer)) || (seqWork.stat & MGSEQ_STAT_MODENEXT)) {
                    HuPrcVSleep();
                    seqWork.stat &= ~MGSEQ_STAT_MODENEXT;
                    break;
                }
                HuPrcVSleep();
                seqFrameNo++;
            }
        }
        if (seqWork.timer && !MgTimerDoneCheck(seqWork.timer)) {
            if (seqWork.timer->stopF == 0) {
                seqWork.timer->stopF = 1;
                seqWork.timer->mode = 1;
            }
            seqWork.timer = NULL;
        }
        seqWork.stat &= ~MGSEQ_STAT_MODECHANGE_OFF;
        if (seqWork.mode == MGSEQ_MODE_MAIN || seqWork.mode == MGSEQ_MODE_FINISH) {
            seqWork.mode = MGSEQ_MODE_FINISH;
            seqGameMesId = GameMesCreate(GAMEMES_MES_MG, GAMEMES_MG_TYPE_FINISH);
            seqFrameNo = 0;
            while (GameMesStatGet(seqGameMesId)) {
                if (seqWork.param.finishHook) {
                    hook = seqWork.param.finishHook;
                    hook(seqWork.mode, seqFrameNo);
                }
                SeqExecModeHooks(seqWork.mode, seqFrameNo);
                if (seqWork.stat & MGSEQ_STAT_MODENEXT) {
                    seqWork.stat &= ~MGSEQ_STAT_MODENEXT;
                    break;
                }
                HuPrcVSleep();
                seqFrameNo++;
            }
        }
        seqWork.stat &= ~MGSEQ_STAT_MODECHANGE_OFF;
        if (seqWork.mode == MGSEQ_MODE_FINISH || seqWork.mode == MGSEQ_MODE_PREWIN) {
            if (seqWork.mode != MGSEQ_MODE_FINISH || seqFrameNo >= 90) {
                seqFrameNo = 0;
            }
            seqWork.mode = MGSEQ_MODE_PREWIN;
            delay = 0;
            if (seqWork.modeDelay[MGSEQ_MODE_PREWIN] == -1) {
                seqWork.stat |= MGSEQ_STAT_MODECHANGE_OFF;
            } else {
                delay = seqWork.modeDelay[MGSEQ_MODE_PREWIN];
            }
            while (seqFrameNo < delay || (seqWork.stat & MGSEQ_STAT_MODECHANGE_OFF)) {
                if (seqWork.param.preWinnerHook) {
                    hook = seqWork.param.preWinnerHook;
                    hook(seqWork.mode, seqFrameNo);
                }
                SeqExecModeHooks(seqWork.mode, seqFrameNo);
                if ((seqWork.stat & MGSEQ_STAT_RECORD) && seqFrameNo == 90) {
                    if (delay == (s32)(f64)defModeDelayTbl[MGSEQ_MODE_PREWIN]) {
                        delay = 270;
                    }
                    GameMesCreate(GAMEMES_MES_MG_RECORD, seqWork.recordVal);
                    HuAudJinglePlay(66);
                }
                if (seqWork.stat & MGSEQ_STAT_MODENEXT) {
                    seqWork.stat &= ~MGSEQ_STAT_MODENEXT;
                    break;
                }
                HuPrcVSleep();
                seqFrameNo++;
            }
        }
        seqWork.stat &= ~MGSEQ_STAT_MODECHANGE_OFF;
        if (seqWork.mode == MGSEQ_MODE_PREWIN || seqWork.mode == MGSEQ_MODE_WINNER) {
            seqWork.mode = MGSEQ_MODE_WINNER;
            if (seqWork.stat & MGSEQ_STAT_WINNER) {
                if (seqWork.winner[0] == -1 && seqWork.winner[1] == -1 && seqWork.winner[2] == -1 && seqWork.winner[3] == -1) {
                    GameMesCreate(GAMEMES_MES_MG, GAMEMES_MG_TYPE_DRAW);
                    HuAudJinglePlay(69);
                } else {
                    GameMesCreate(GAMEMES_MES_MG_WINNER, GAMEMES_MG_TYPE_WIN, seqWork.winner[0], seqWork.winner[1], seqWork.winner[2],
                        seqWork.winner[3]);
                    HuAudJinglePlay(67);
                }
            }
            delay = 0;
            if (seqWork.modeDelay[MGSEQ_MODE_WINNER] == -1) {
                seqWork.stat |= MGSEQ_STAT_MODECHANGE_OFF;
            } else {
                delay = seqWork.modeDelay[MGSEQ_MODE_WINNER];
            }
            seqFrameNo = 0;
            while (seqFrameNo < delay || (seqWork.stat & MGSEQ_STAT_MODECHANGE_OFF)) {
                if (seqWork.param.winnerHook) {
                    hook = seqWork.param.winnerHook;
                    hook(seqWork.mode, seqFrameNo);
                }
                SeqExecModeHooks(seqWork.mode, seqFrameNo);
                if (seqWork.stat & MGSEQ_STAT_MODENEXT) {
                    seqWork.stat &= ~MGSEQ_STAT_MODENEXT;
                    break;
                }
                HuPrcVSleep();
                seqFrameNo++;
            }
        }
        seqWork.stat &= ~MGSEQ_STAT_MODECHANGE_OFF;
        if (seqWork.mode == MGSEQ_MODE_WINNER || seqWork.mode == MGSEQ_MODE_FADEOUT) {
            seqWork.mode = MGSEQ_MODE_FADEOUT;
            delay = 0;
            if (seqWork.modeDelay[MGSEQ_MODE_FADEOUT] == -1) {
                seqWork.stat |= MGSEQ_STAT_MODECHANGE_OFF;
            } else {
                delay = seqWork.modeDelay[MGSEQ_MODE_FADEOUT];
            }
            if (!(seqWork.stat & MGSEQ_STAT_FADEOUT_OFF)) {
                if (!_CheckFlag(FLAG_INST_DECA)) {
                    WipeCreate(WIPE_MODE_OUT, WIPE_TYPE_NORMAL, delay);
                } else {
                    WipeCreate(WIPE_MODE_OUT, WIPE_TYPE_NORMAL, delay);
                }
                HuAudFadeOut(1000);
            }
            seqFrameNo = 0;
            while (seqFrameNo < delay || (seqWork.stat & MGSEQ_STAT_MODECHANGE_OFF)) {
                if (seqWork.param.fadeOutHook) {
                    hook = seqWork.param.fadeOutHook;
                    hook(seqWork.mode, seqFrameNo);
                }
                SeqExecModeHooks(seqWork.mode, seqFrameNo);
                if (seqWork.stat & MGSEQ_STAT_MODENEXT) {
                    seqWork.stat &= ~MGSEQ_STAT_MODENEXT;
                    break;
                }
                HuPrcVSleep();
                seqFrameNo++;
            }
        }
    } while (seqWork.mode != MGSEQ_MODE_FADEOUT && seqWork.mode != MGSEQ_MODE_CLOSE);
    seqWork.mode = MGSEQ_MODE_CLOSE;
    seqFrameNo = 0;
    if (seqWork.param.closeHook) {
        hook = seqWork.param.closeHook;
        hook(seqWork.mode, seqFrameNo);
    }
    SeqExecModeHooks(seqWork.mode, seqFrameNo);
    omOvlReturnEx(1, 1);
    HuPrcEnd();
    while (1) {
        HuPrcVSleep();
    }
}

static void SeqExit(void)
{
    MGSEQ_FUNC hook;

    while (omSysExitReq == 0) {
        HuPrcVSleep();
    }
    WipeCreate(WIPE_MODE_OUT, WIPE_TYPE_NORMAL, 60);
    HuAudFadeOut(1000);
    while (WipeCheck()) {
        HuPrcVSleep();
    }
    if (seqWork.param.closeHook) {
        hook = seqWork.param.closeHook;
        hook(MGSEQ_MODE_EXIT, 0);
    }
    SeqExecModeHooks(MGSEQ_MODE_EXIT, 0);
    omOvlReturnEx(1, 1);
    HuPrcKill(seqProc);
    while (1) {
        HuPrcVSleep();
    }
}

static void SeqExecModeHooks(s16 mode, s16 frameNo)
{
    s16 i;
    MGSEQ_FUNC hook;

    for (i = 0; i < seqWork.hookNum; i++) {
        if (seqWork.hook[i].mode == mode) {
            hook = seqWork.hook[i].hook;
            hook(mode, frameNo);
        }
    }
}

static void SeqSetTimerPos(s32 pos, MGTIMER *timer)
{
    switch (pos) {
        case MGTIMER_POS_BOTTOM:
            MgTimerPosSet(timer, 288.0f, 420.0f);
            break;
        case MGTIMER_POS_RIGHT:
            MgTimerPosSet(timer, 525.0f, 64.0f);
            break;
    }
}

u32 MgSeqModeGet(void)
{
    return seqWork.mode;
}

u32 MgSeqModeNext(void)
{
    seqWork.stat |= MGSEQ_STAT_MODENEXT;
    return seqWork.mode;
}

u32 MgSeqModeSet(u32 mode)
{
    if (mode > MGSEQ_MODE_CLOSE) {
        mode = MGSEQ_MODE_CLOSE;
    }
    seqWork.mode = mode;
    seqWork.stat |= MGSEQ_STAT_MODENEXT;
    return seqWork.mode;
}

u16 MgSeqModeChangeOff(void)
{
    seqWork.stat |= MGSEQ_STAT_MODECHANGE_OFF;
    return seqWork.stat;
}

u16 MgSeqModeChangeOn(void)
{
    seqWork.stat &= ~MGSEQ_STAT_MODECHANGE_OFF;
    return seqWork.stat;
}

s32 MgSeqTimerValueGet(void)
{
    if (!seqWork.timer) {
        return seqWork.param.maxTime * 60;
    } else {
        return MgTimerValueGet(seqWork.timer);
    }
}

void MgSeqRecordSet(int recordVal)
{
    if (!_CheckFlag(FLAG_MG_PRACTICE)) {
        seqWork.stat |= MGSEQ_STAT_RECORD;
        seqWork.recordVal = recordVal;
    }
}

u16 MgSeqStatGet(void)
{
    return seqWork.stat;
}

u16 MgSeqWinnerSet(s16 charNo1, s16 charNo2, s16 charNo3, s16 charNo4)
{
    seqWork.winner[0] = charNo1;
    seqWork.winner[1] = charNo2;
    seqWork.winner[2] = charNo3;
    seqWork.winner[3] = charNo4;
    seqWork.stat |= MGSEQ_STAT_WINNER;
    return seqWork.stat;
}

u16 MgSeqDrawSet(void)
{
    seqWork.winner[0] = seqWork.winner[1] = seqWork.winner[2] = seqWork.winner[3] = -1;
    seqWork.stat |= MGSEQ_STAT_WINNER;
    return seqWork.stat;
}

void fn_80071CCC(void)
{
    seqWork.stat |= MGSEQ_STAT_UNKBIT;
}

void MgSeqModeDelaySet(s16 mode, s16 delay)
{
    if (mode < 0 || mode >= MGSEQ_MODE_MAX) {
        return;
    }
    seqWork.modeDelay[mode] = delay;
}

u32 MgSeqModeHookAdd(s16 mode, MGSEQ_FUNC func)
{
    if (mode < 0 || mode >= MGSEQ_MODE_MAX || seqWork.hookNum >= MGSEQ_MODEHOOK_MAX) {
        return -1;
    }
    seqWork.hook[seqWork.hookNum].mode = mode;
    seqWork.hook[seqWork.hookNum].hook = func;
    seqWork.hookNum++;
    return seqWork.hookNum - 1;
}

void MgSeqModeHookReset(s16 mode)
{
    seqWork.hook[mode].mode = 0xFFFF;
}

u32 MgSeqFrameNoGet(void)
{
    return seqFrameNo;
}

void MgSeqFrameNoSet(u32 frameNo)
{
    seqFrameNo = frameNo;
}

s16 MgSeqGameMesIdGet(void)
{
    return seqGameMesId;
}

void MgSeqTimerKill(s16 maxTime)
{
    if (seqWork.timer) {
        MgTimerKill(seqWork.timer);
    }
    seqWork.timer = NULL;
    seqWork.param.maxTime = maxTime;
}

void MgSeqStatBitSet(u16 bit)
{
    seqWork.stat |= bit;
}

void MgSeqStatBitReset(u16 bit)
{
    seqWork.stat &= ~bit;
}
