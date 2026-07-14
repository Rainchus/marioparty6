#define _MATH_H

#include "game/board/audio.h"
#include "game/board/main.h"
#include "game/charman.h"
#include "game/disp.h"
#include "game/memory.h"

#include <string.h>

typedef struct MBAudFXObj_s {
    int seId;
    int seNo;
    int vol;
    BOOL emitterF;
    BOOL pauseF;
    BOOL lockF;
    Vec pos;
    int *fxRef;
} MBAUDFXOBJ;

typedef struct MBAudFXData_s {
    int seId;
    int delay;
    int type;
    s8 pan;
    Vec pos;
} MBAUDFXDATA;

typedef struct MBMusData_s {
    int streamNo;
    s16 id;
    s16 vol;
    s16 closeTime;
    s16 stopTime;
    s16 fadeTime;
    s16 pauseF;
    s16 stopF;
    HUPROCESS *proc;
} MBMUSDATA;

typedef struct BoardMusData_s {
    u32 boardNo;
    s32 dayMusId;
    s32 nightMusId;
    BOOL timeF;
} BOARDMUSDATA;

typedef struct MusBoardFadeData_s {
    int chan;
    int nextChan;
    int speed;
    int fadeSpeed;
    int musId;
    BOOL pauseF;
} MUSBOARDFADEDATA;

enum {
    MB_AUD_FX_TYPE_NORMAL,
    MB_AUD_FX_TYPE_PAN,
    MB_AUD_FX_TYPE_EMITTER
};

static BOARDMUSDATA boardMusData[] = {
    { 0, 13, 14, TRUE },
    { 1, 15, 16, TRUE },
    { 2, 17, 18, TRUE },
    { 3, 19, 20, TRUE },
    { 4, 21, 22, TRUE },
    { 5, 23, 24, TRUE },
    { 6, 45, 45, FALSE },
    { 7, 46, 46, FALSE },
    { 8, 47, 47, FALSE },
    { 9, 37, 38, TRUE },
    { 10, 44, 44, FALSE },
    { -1, 0, 0, FALSE }
};

static u32 guideFxTbl[][2] = {
    { 948, 940 },
    { 949, 941 },
    { 950, 942 },
    { 951, 943 },
    { 952, 944 },
    { 953, 945 },
    { 954, 946 },
    { 955, 947 },
    { -1, -1 }
};

static MBAUDFXOBJ audFXObjData[MB_AUD_FX_OBJ_MAX];
static MBAUDFXDATA audFXData[MB_AUD_FX_DELAY_MAX];
static MBMUSDATA musData[MB_MUS_CHAN_MAX];

static int dummyId = -1;

static HUPROCESS *musBoardFadeProc;
static HUPROCESS *audFXProc;
static BOOL audFXObjOnF;
static int audFXObjOnTimer;
static int audFXDelay;
static int musChanId[MB_MUS_CHAN_MAX];

extern HUPROCESS *mbMainProc;
extern void mbPlayerPosGet(int playerNo, Vec *pos);
extern void Hu3D3Dto2D(Vec *src, s16 cameraBit, Vec *dst);

static inline int BoardNoGet(void)
{
    return GwSystem.boardNo;
}

static void MusPlay(void);
static void MusPlayKill(void);
static void MusBoardFade(void);
static void AudFXMain(void);
static void AudFXMainDestroy(void);

static inline void AudFXObjPauseSet(int seNo, BOOL pauseF)
{
    MBAUDFXOBJ *audFx = &audFXObjData[0];
    int i;

    for (i = 0, audFx = &audFXObjData[0]; i < MB_AUD_FX_OBJ_MAX; i++, audFx++) {
        if (audFx->seNo == seNo) {
            break;
        }
    }
    if (i >= MB_AUD_FX_OBJ_MAX) {
        return;
    }
    if (pauseF == FALSE) {
        if (audFx->pauseF != FALSE) {
            (void)i;
        } else {
            (void)i;
            return;
        }
        audFx->pauseF = FALSE;
        msmSePause(audFx->seNo, FALSE, 1000);
    } else {
        if (audFx->pauseF != FALSE) {
            (void)audFx;
        } else {
            audFx->pauseF = TRUE;
            msmSePause(audFx->seNo, TRUE, 1000);
            return;
        }
    }
}

static inline void AudFXPosSet(MBAUDFXDATA *audFx, Vec *pos)
{
    if (pos != NULL) {
        audFx->pos = *pos;
    }
}

void mbAudInit(void)
{
    MBAUDFXOBJ *audFx = &audFXObjData[0];
    MBAUDFXDATA *delayFx = &audFXData[0];
    int i;

    for (i = 0; i < MB_MUS_CHAN_MAX; i++) {
        musData[i].streamNo = MSM_STREAMNO_NONE;
        musData[i].id = MSM_STREAM_NONE;
        musData[i].vol = 0;
        musData[i].closeTime = 0;
        musData[i].stopTime = 0;
        musData[i].stopF = FALSE;
        musData[i].fadeTime = 0;
        musData[i].pauseF = FALSE;
        musData[i].proc = NULL;
    }
    musBoardFadeProc = NULL;
    dummyId = -1;

    memset(&audFXObjData[0], 0, sizeof(audFXObjData));
    for (i = 0, audFx = &audFXObjData[0]; i < MB_AUD_FX_OBJ_MAX; i++, audFx++) {
        audFx->seId = MSM_SE_NONE;
        audFx->seNo = MSM_SENO_NONE;
    }

    audFXProc = HuPrcChildCreate(AudFXMain, 0x2001, 0x4000, 0, mbMainProc);
    HuPrcDestructorSet2(audFXProc, AudFXMainDestroy);
    if (_CheckFlag(FLAG_BOARD_STAR_RESET)) {
        audFXObjOnF = TRUE;
        audFXObjOnTimer = 1;
    } else {
        audFXObjOnF = FALSE;
        audFXObjOnTimer = 0;
    }

    audFXDelay = 0;
    memset(&audFXData[0], 0, sizeof(audFXData));
    for (i = 0; i < MB_AUD_FX_DELAY_MAX; i++, delayFx++) {
        delayFx->seId = MSM_SE_NONE;
    }
    mbAudFXPlay(0x452);
}

void mbMusBoardPlay(void)
{
    int i;
    int musId = boardMusData[0].dayMusId;

    for (i = 0; boardMusData[i].boardNo != 0xFFFFFFFF; i++) {
        if (boardMusData[i].boardNo == BoardNoGet()) {
            if (boardMusData[i].timeF) {
                if (GwSystem.curTime == FALSE) {
                    musId = boardMusData[i].dayMusId;
                } else {
                    musId = boardMusData[i].nightMusId;
                }
            } else {
                musId = boardMusData[i].dayMusId;
            }
            break;
        }
    }
    mbMusPlay(MB_MUS_CHAN_BG, musId, MSM_VOL_MAX, 0);
}

void mbMusPlay(int chan, int id, s8 vol, u16 fadeSpeed)
{
    MBMUSDATA *musP = &musData[chan];
    int streamNo;
    int chanOther;
    MSM_STREAMPARAM streamParam;

    if (musicOffF || omSysExitReq) {
        return;
    }

    streamParam.flag = MSM_STREAMPARAM_CHAN | MSM_STREAMPARAM_VOL;
    chanOther = chan ^ 1;
    if (musData[chan].streamNo != MSM_STREAMNO_NONE && id == musData[chan].id) {
        if (mbMusStatGet(chan) == MSM_STREAM_PLAY || mbMusStatGet(chan) == MSM_STREAM_PAUSEOUT) {
            return;
        }
    }
    if (musData[chan].streamNo != MSM_STREAMNO_NONE && id == musData[chan].id && mbMusStopCheck(chan)) {
        mbMusPauseFadeOut(chan, FALSE, 0);
        return;
    }
    if (mbMusCheck(chan)) {
        if (mbMusEndCheck(chan) == FALSE) {
            mbMusFadeOut(chan);
        }
        while (mbMusCheck(chan)) {
            HuPrcVSleep();
        }
    }
    mbMusStop(chan, FALSE);

    if (fadeSpeed) {
        streamParam.flag |= MSM_STREAMPARAM_FADESPEED;
        streamParam.fadeSpeed = fadeSpeed;
    }
    streamParam.chan = chan * 2;
    streamParam.vol = vol;
    streamNo = HuAudSStreamChanPlay(id, chan * 2);
    if (streamNo >= 0) {
        int *chanP;

        musP->streamNo = streamNo;
        musP->id = id;
        musP->vol = vol;
        musP->closeTime = 3;
        musP->stopTime = 60;
        musP->stopF = FALSE;
        musP->proc = HuPrcChildCreate(MusPlay, 0x2001, 0x2000, 0, mbMainProc);
        HuPrcSetStat(musP->proc, HU_PRC_STAT_PAUSE_ON | HU_PRC_STAT_UPAUSE_ON);
        musP->proc->property = chanP = &musChanId[chan];
        *chanP = chan;
        HuPrcDestructorSet2(musP->proc, MusPlayKill);
    }
}

void mbMusFadeOut(int chan)
{
    MBMUSDATA *musP = &musData[chan];

    mbMusFadeOutSpeed(chan, 1000);
}

void mbMusStop(int chan, BOOL unused)
{
    MBMUSDATA *musP = &musData[chan];

    HuAudSStreamStop(musP->streamNo);
    musP->streamNo = MSM_STREAMNO_NONE;
    musP->id = MSM_STREAM_NONE;
    musP->closeTime = 0;
    musP->stopTime = 0;
    musP->stopF = FALSE;
    musP->fadeTime = 0;
    musP->pauseF = FALSE;
}

void mbMusFadeOutSpeed(int chan, u16 speed)
{
    MBMUSDATA *musP = &musData[chan];

    if (musP->streamNo == MSM_STREAMNO_NONE) {
        return;
    }
    if (mbMusEndCheck(chan)) {
        return;
    }
    HuAudSStreamFadeOut(musP->streamNo, speed);
    musP->stopTime = (s16)((speed * 60) / 1000) + 5;
    musP->stopF = TRUE;
}

void mbMusStub(void)
{
}

void mbMusParamSet(int chan, s8 vol, u16 fadeSpeed)
{
    MBMUSDATA *musP = &musData[chan];
    MSM_STREAMPARAM streamParam;

    if (musP->streamNo == MSM_STREAMNO_NONE) {
        return;
    }
    streamParam.flag = MSM_STREAMPARAM_CHAN | MSM_STREAMPARAM_VOL;
    if (fadeSpeed) {
        streamParam.flag |= MSM_STREAMPARAM_FADESPEED;
        streamParam.fadeSpeed = fadeSpeed;
    }
    if (vol < 0) {
        vol = MSM_VOL_MAX;
    }
    streamParam.vol = vol;
    musP->vol = vol;
    HuAudSStreamParamSet(musP->streamNo, vol, fadeSpeed);
}

void mbMusPauseFadeOut(int chan, BOOL pauseF, int speed)
{
    MBMUSDATA *musP = &musData[chan];

    if (musP->streamNo == MSM_STREAMNO_NONE) {
        return;
    }
    if (pauseF) {
        if (mbMusStopCheck(chan)) {
            return;
        }
    } else if (mbMusStopCheck(chan) == FALSE) {
        return;
    }

    musData[chan].pauseF = pauseF;
    if (speed > 0) {
        HuAudSStreamPauseFadeOut(musP->streamNo, pauseF, speed);
        musData[chan].fadeTime = 2.0f + ((speed * 60) / 1000.0f);
    } else {
        HuAudSStreamPauseFadeOut(musP->streamNo, pauseF, 1000);
        musData[chan].fadeTime = 62;
    }
}

s32 mbMusStatGet(int chan)
{
    MBMUSDATA *musP = &musData[chan];

    if (musP->streamNo == MSM_STREAMNO_NONE) {
        return MSM_STREAM_DONE;
    }
    if (musP->stopF) {
        return MSM_STREAM_STOP;
    }
    if (musP->pauseF) {
        return MSM_STREAM_PAUSEIN;
    }
    return MSM_STREAM_PLAY;
}

void mbAudClose(void)
{
    int i;

    for (i = 0; i < MB_MUS_CHAN_MAX; i++) {
        MBMUSDATA *musP = &musData[i];

        mbMusFadeOutSpeed(i, 1000);
    }
    mbAudFXObjClose();
}

BOOL mbMusCheck(int chan)
{
    if (musData[chan].streamNo != MSM_STREAMNO_NONE) {
        return TRUE;
    }
    return FALSE;
}

BOOL mbMusStopCheck(int chan)
{
    if (mbMusCheck(chan) == FALSE) {
        return FALSE;
    }
    if (mbMusStatGet(chan) == MSM_STREAM_PAUSEIN) {
        return TRUE;
    }
    return FALSE;
}

BOOL mbMusFadeCheck(int chan)
{
    if (mbMusCheck(chan) == FALSE) {
        return FALSE;
    }
    if (musData[chan].fadeTime > 0) {
        return TRUE;
    }
    return FALSE;
}

BOOL mbMusEndCheck(int chan)
{
    if (mbMusCheck(chan) == FALSE) {
        return TRUE;
    }
    if (mbMusStatGet(chan) == MSM_STREAM_STOP || mbMusStatGet(chan) == MSM_STREAM_DONE) {
        return TRUE;
    }
    return FALSE;
}

int mbMusJinglePlay(s16 id)
{
    int streamNo = HuAudSStreamChanPlay(id, MB_MUS_CHAN_FG * 2);

    return streamNo;
}

void mbMusJingleWait(int streamNo)
{
    int time;

    if (streamNo < 0) {
        return;
    }
    for (time = 0; time < 600; time++) {
        if (mbMusJingleStatGet(streamNo) == MSM_STREAM_DONE) {
            break;
        }
        HuPrcVSleep();
    }
    if (time >= 600) {
        HuAudSStreamFadeOut(streamNo, 10);
        HuPrcSleep(11);
    }
}

s32 mbMusJingleStatGet(int streamNo)
{
    if (streamNo < 0) {
        return MSM_STREAM_DONE;
    }
    return HuAudSStreamStatGet(streamNo);
}

static void MusPlay(void)
{
    MBMUSDATA *musP;

    do {
        HUPROCESS *proc = HuPrcCurrentGet();
        int *chanP = proc->property;

        musP = &musData[*chanP];
        if (musP->fadeTime > 0) {
            musP->fadeTime--;
        }
        if (mbExitCheck()) {
            if (mbMusEndCheck(*chanP) == FALSE) {
                mbMusStop(*chanP, FALSE);
            }
            musP->streamNo = MSM_STREAMNO_NONE;
            musP->id = MSM_STREAM_NONE;
            musP->closeTime = 0;
            musP->stopTime = 0;
            musP->stopF = FALSE;
            musP->fadeTime = 0;
            musP->pauseF = FALSE;
            musP->proc = NULL;
            break;
        }
        if (musP->stopF) {
            if (--musP->stopTime < 0) {
                mbMusStop(*chanP, FALSE);
                musP->streamNo = MSM_STREAMNO_NONE;
                musP->id = MSM_STREAM_NONE;
                musP->closeTime = 0;
                musP->stopTime = 0;
                musP->stopF = FALSE;
                musP->fadeTime = 0;
                musP->pauseF = FALSE;
                musP->proc = NULL;
                break;
            }
        }
        if (musP->streamNo != MSM_STREAMNO_NONE) {
            switch (HuAudSStreamStatGet(musP->streamNo)) {
                case MSM_STREAM_DVDERROR:
                    mbMusStop(*chanP, FALSE);
                    musP->closeTime = 0;
                case MSM_STREAM_DONE:
                    if (--musP->closeTime <= 0) {
                        musP->streamNo = MSM_STREAMNO_NONE;
                        musP->id = MSM_STREAM_NONE;
                        musP->closeTime = 0;
                        musP->stopTime = 0;
                        musP->stopF = FALSE;
                        musP->fadeTime = 0;
                        musP->pauseF = FALSE;
                        musP->proc = NULL;
                    }
                    break;
            }
            HuPrcVSleep();
        }
        if (musP == NULL) {
            break;
        }
    } while (musP->streamNo != MSM_STREAMNO_NONE);

    if (musP) {
        musP->proc = NULL;
    }
    HuPrcEnd();
}

static void MusPlayKill(void)
{
    HUPROCESS *proc = HuPrcCurrentGet();

    if (proc->property) {
        proc->property = NULL;
    }
}

BOOL mbMusBoardFadeOut(int chan, int nextChan, int speed, int fadeSpeed, int musId, BOOL pauseF)
{
    MUSBOARDFADEDATA *work;
    void *workP;

    if (musBoardFadeProc) {
        return FALSE;
    }
    musBoardFadeProc = HuPrcChildCreate(MusBoardFade, 0x2001, 0x2000, 0, mbMainProc);
    workP = HuMemDirectMallocNum(HEAP_HEAP, sizeof(MUSBOARDFADEDATA), HU_MEMNUM_OVL);
    work = workP;
    musBoardFadeProc->property = work;
    memset(work, 0, sizeof(MUSBOARDFADEDATA));
    work->chan = chan;
    work->nextChan = nextChan;
    work->speed = speed;
    work->fadeSpeed = fadeSpeed;
    work->musId = musId;
    work->pauseF = pauseF;

    if (work->pauseF) {
        mbMusPauseFadeOut(chan, TRUE, work->speed);
    } else {
        mbMusFadeOutSpeed(chan, (u16)work->speed);
    }
    return TRUE;
}

BOOL mbMusBoardFadeCheck(void)
{
    if (musBoardFadeProc == NULL) {
        return FALSE;
    }
    return TRUE;
}

static void MusBoardFade(void)
{
    MUSBOARDFADEDATA *work = HuPrcCurrentGet()->property;
    int waitTime;
    int time;

    waitTime = ((work->speed * 60) / 1000) + 10;
    for (time = 0; time < waitTime; time++) {
        HuPrcVSleep();
    }
    HuPrcSleep(5);

    if (work->musId == MSM_STREAM_NONE) {
        int i;
        int musId = boardMusData[0].dayMusId;

        for (i = 0; boardMusData[i].boardNo != 0xFFFFFFFF; i++) {
            if (boardMusData[i].boardNo == BoardNoGet()) {
                if (boardMusData[i].timeF) {
                    if (GwSystem.curTime == FALSE) {
                        musId = boardMusData[i].dayMusId;
                    } else {
                        musId = boardMusData[i].nightMusId;
                    }
                } else {
                    musId = boardMusData[i].dayMusId;
                }
                break;
            }
        }
        mbMusPlay(MB_MUS_CHAN_BG, musId, MSM_VOL_MAX, 0);
    } else {
        mbMusPlay(work->nextChan, work->musId, MSM_VOL_MAX, (u16)work->fadeSpeed);
    }
    HuMemDirectFree(work);
    musBoardFadeProc = NULL;
    HuPrcEnd();
}

void mbMusPauseSet(BOOL pauseF)
{
    s32 pauseStatus;
    s32 unpauseStatus;

    if (pauseF) {
        if (musData[MB_MUS_CHAN_BG].streamNo != MSM_STREAMNO_NONE) {
            HuAudSStreamPauseFadeOut(musData[MB_MUS_CHAN_BG].streamNo, TRUE, 5);
        }
        if (musData[MB_MUS_CHAN_FG].streamNo != MSM_STREAMNO_NONE) {
            HuAudSStreamPauseFadeOut(musData[MB_MUS_CHAN_FG].streamNo, TRUE, 5);
        }
        pauseStatus = HuAudSStreamStatGet(2);
        if (pauseStatus == MSM_STREAM_PLAY) {
            HuAudSStreamPauseFadeOut(2, TRUE, 5);
        }
    } else {
        if (musData[MB_MUS_CHAN_BG].streamNo != MSM_STREAMNO_NONE && musData[MB_MUS_CHAN_BG].pauseF == FALSE) {
            HuAudSStreamPauseFadeOut(musData[MB_MUS_CHAN_BG].streamNo, FALSE, 5);
        }
        if (musData[MB_MUS_CHAN_FG].streamNo != MSM_STREAMNO_NONE && musData[MB_MUS_CHAN_BG].pauseF == FALSE) {
            HuAudSStreamPauseFadeOut(musData[MB_MUS_CHAN_FG].streamNo, FALSE, 5);
        }
        unpauseStatus = HuAudSStreamStatGet(2);
        if (unpauseStatus == MSM_STREAM_PAUSEIN) {
            HuAudSStreamPauseFadeOut(2, FALSE, 5);
        }
    }
}

void mbAudStub1(void)
{
}

void mbAudStub2(void)
{
}

int mbAudFXObjSet(int seId)
{
    return mbAudFXObjCreate(seId, TRUE);
}

int mbAudFXObjCreate(int seId, BOOL multiF)
{
    MBAUDFXOBJ *audFx = &audFXObjData[0];
    int i;

    if (multiF == FALSE) {
        for (i = 0, audFx = &audFXObjData[0]; i < MB_AUD_FX_OBJ_MAX; i++, audFx++) {
            if (audFx->seId == seId) {
                return MSM_SENO_NONE;
            }
        }
    }
    for (i = 0, audFx = &audFXObjData[0]; i < MB_AUD_FX_OBJ_MAX; i++, audFx++) {
        if (audFx->seId == MSM_SE_NONE) {
            break;
        }
    }
    if (i >= MB_AUD_FX_OBJ_MAX) {
        return MSM_SENO_NONE;
    }
    if (audFXObjOnF) {
        audFx->seNo = MB_AUD_FXNO_OFF;
        audFx->seId = seId;
        audFx->vol = MSM_VOL_MAX;
        audFx->pauseF = FALSE;
        audFx->emitterF = FALSE;
        audFx->lockF = TRUE;
        audFx->fxRef = NULL;
        return MB_AUD_FXNO_OFF;
    }

    audFx->seNo = HuAudFXPlay(seId);
    if (audFx->seNo < 0) {
        audFx->seNo = audFx->seId = MSM_SE_NONE;
        return MSM_SENO_NONE;
    }
    audFx->seId = seId;
    audFx->vol = MSM_VOL_MAX;
    audFx->pauseF = FALSE;
    audFx->emitterF = FALSE;
    audFx->lockF = FALSE;
    audFx->fxRef = NULL;
    return audFx->seNo;
}

void mbAudFXObjVolSet(int seNo, s16 vol)
{
    MBAUDFXOBJ *audFx = &audFXObjData[0];
    int i;

    for (i = 0, audFx = &audFXObjData[0]; i < MB_AUD_FX_OBJ_MAX; i++, audFx++) {
        if (audFx->seNo == seNo) {
            break;
        }
    }
    if (i >= MB_AUD_FX_OBJ_MAX) {
        return;
    }
    audFx->vol = vol;
    HuAudFXVolSet(audFx->seNo, audFx->vol);
}

void mbAudFXObjKill(int seNo)
{
    MBAUDFXOBJ *audFx = &audFXObjData[0];
    int i;

    for (i = 0, audFx = &audFXObjData[0]; i < MB_AUD_FX_OBJ_MAX; i++, audFx++) {
        if (audFx->seNo == seNo) {
            break;
        }
    }
    if (i >= MB_AUD_FX_OBJ_MAX) {
        return;
    }
    audFx->seId = MSM_SE_NONE;
    audFx->seNo = MSM_SENO_NONE;
    HuAudFXFadeOut(seNo, 1000);
}

void mbAudFXObjClose(void)
{
    MBAUDFXOBJ *audFx = &audFXObjData[0];
    int i;

    for (i = 0, audFx = &audFXObjData[0]; i < MB_AUD_FX_OBJ_MAX; i++, audFx++) {
        if (audFx->seNo != MSM_SENO_NONE) {
            mbAudFXObjKill(audFx->seNo);
        }
    }
}

int mbAudFXObjEmitterSet(int seId, Vec *pos)
{
    return mbAudFXObjEmitterCreate(seId, pos, TRUE);
}

int mbAudFXObjEmitterCreate(int seId, Vec *pos, BOOL multiF)
{
    MBAUDFXOBJ *audFx = &audFXObjData[0];
    int i;

    if (multiF == FALSE) {
        for (i = 0, audFx = &audFXObjData[0]; i < MB_AUD_FX_OBJ_MAX; i++, audFx++) {
            if (audFx->seId == seId) {
                return MSM_SENO_NONE;
            }
        }
    }
    for (i = 0, audFx = &audFXObjData[0]; i < MB_AUD_FX_OBJ_MAX; i++, audFx++) {
        if (audFx->seId == MSM_SE_NONE) {
            break;
        }
    }
    if (i >= MB_AUD_FX_OBJ_MAX) {
        return MSM_SENO_NONE;
    }
    if (audFXObjOnF) {
        audFx->seNo = MB_AUD_FXNO_OFF;
        audFx->seId = seId;
        audFx->vol = MSM_VOL_MAX;
        audFx->pauseF = FALSE;
        audFx->emitterF = TRUE;
        audFx->pos = *pos;
        audFx->lockF = TRUE;
        audFx->fxRef = NULL;
        return MB_AUD_FXNO_OFF;
    }

    audFx->seNo = HuAudFXEmiterPlay(seId, pos);
    if (audFx->seNo < 0) {
        audFx->seNo = MSM_SENO_NONE;
        return MSM_SENO_NONE;
    }
    audFx->seId = seId;
    audFx->vol = MSM_VOL_MAX;
    audFx->pauseF = FALSE;
    audFx->emitterF = TRUE;
    audFx->pos = *pos;
    audFx->lockF = FALSE;
    audFx->fxRef = NULL;
    return audFx->seNo;
}

void mbAudFXObjEmitterUpdate(int seNo, Vec *pos)
{
    MBAUDFXOBJ *audFx = &audFXObjData[0];
    int i;

    for (i = 0, audFx = &audFXObjData[0]; i < MB_AUD_FX_OBJ_MAX; i++, audFx++) {
        if (audFx->seNo == seNo && audFx->emitterF) {
            break;
        }
    }
    if (i >= MB_AUD_FX_OBJ_MAX) {
        return;
    }
    audFx->pos = *pos;
    if (audFXObjOnF == FALSE) {
        HuAudFXEmiterUpDate(audFx->seNo, &audFx->pos);
    }
}

void mbAudFXObjEmiterPosSet(int seId, Vec *pos)
{
    MBAUDFXOBJ *audFx = &audFXObjData[0];
    int i;

    for (i = 0, audFx = &audFXObjData[0]; i < MB_AUD_FX_OBJ_MAX; i++, audFx++) {
        if (audFx->seId == seId && audFx->emitterF && audFx->lockF) {
            break;
        }
    }
    if (i >= MB_AUD_FX_OBJ_MAX) {
        return;
    }
    audFx->pos = *pos;
}

void mbAudFXObjRefSet(int seId, int *fxRef)
{
    MBAUDFXOBJ *audFx = &audFXObjData[0];
    int i;

    for (i = 0, audFx = &audFXObjData[0]; i < MB_AUD_FX_OBJ_MAX; i++, audFx++) {
        if (audFx->seId == seId && audFx->lockF) {
            break;
        }
    }
    if (i >= MB_AUD_FX_OBJ_MAX) {
        return;
    }
    audFx->fxRef = fxRef;
}

static void AudFXMain(void)
{
    MBAUDFXOBJ *audFx = &audFXObjData[0];
    MBAUDFXDATA *delayFx = &audFXData[0];
    int i;

    do {
        delayFx = &audFXData[0];
        for (i = 0; i < MB_AUD_FX_DELAY_MAX; i++, delayFx++) {
            if (delayFx->seId >= 0 && --delayFx->delay <= 0) {
                switch (delayFx->type) {
                    case MB_AUD_FX_TYPE_NORMAL:
                        HuAudFXPlay(delayFx->seId);
                        break;
                    case MB_AUD_FX_TYPE_PAN:
                        HuAudFXPlayPan(delayFx->seId, delayFx->pan);
                        break;
                    case MB_AUD_FX_TYPE_EMITTER:
                        HuAudFXEmiterPlay(delayFx->seId, &delayFx->pos);
                        break;
                }
                delayFx->seId = MSM_SE_NONE;
                delayFx->delay = -1;
                delayFx->type = MB_AUD_FX_TYPE_NORMAL;
            }
        }

        for (i = 0, audFx = &audFXObjData[0]; i < MB_AUD_FX_OBJ_MAX; i++, audFx++) {
            if (audFx->seId != MSM_SE_NONE && audFx->seNo != MSM_SENO_NONE && audFx->lockF == FALSE
                && HuAudFXStatusGet(audFx->seNo) == MSM_SE_DONE) {
                audFx->seId = MSM_SE_NONE;
                audFx->seNo = MSM_SENO_NONE;
            }
        }

        if (_CheckFlag(FLAG_BOARD_STAR_RESET) == FALSE) {
            if (audFXObjOnF) {
                if (--audFXObjOnTimer <= 0) {
                    for (i = 0, audFx = &audFXObjData[0]; i < MB_AUD_FX_OBJ_MAX; i++, audFx++) {
                        if (audFx->lockF && audFx->seNo == MB_AUD_FXNO_OFF) {
                            if (audFx->emitterF) {
                                audFx->seNo = HuAudFXEmiterPlay(audFx->seId, &audFx->pos);
                                if (audFx->seNo < 0) {
                                    audFx->seNo = MSM_SENO_NONE;
                                    audFx->seId = MSM_SE_NONE;
                                }
                            } else {
                                audFx->seNo = HuAudFXPlay(audFx->seId);
                                if (audFx->seNo < 0) {
                                    audFx->seNo = MSM_SENO_NONE;
                                    audFx->seId = MSM_SE_NONE;
                                }
                            }
                            if (audFx->fxRef != NULL) {
                                *audFx->fxRef = audFx->seNo;
                            }
                            audFx->lockF = FALSE;
                        }
                        if (audFx->seId != MSM_SE_NONE && audFx->seNo != MSM_SENO_NONE) {
                            AudFXObjPauseSet(audFx->seNo, FALSE);
                        }
                    }
                }
            }
            if (audFXObjOnTimer <= 0) {
                audFXObjOnF = FALSE;
            }
        } else {
            if (audFXObjOnF == FALSE) {
                for (i = 0, audFx = &audFXObjData[0]; i < MB_AUD_FX_OBJ_MAX; i++, audFx++) {
                    if (audFx->seId != MSM_SE_NONE && audFx->seNo != MSM_SENO_NONE) {
                        AudFXObjPauseSet(audFx->seNo, TRUE);
                    }
                }
            }
            audFXObjOnF = TRUE;
            audFXObjOnTimer = 1;
        }

        for (i = 0; i < GW_PLAYER_MAX; i++) {
            Vec pos;
            int pan;

            mbPlayerPosGet(i, &pos);
            pan = mbAudFXPosPanGet(&pos);
            CharModelVoicePanSet(GwPlayer[i].charNo, pan);
        }
        HuPrcVSleep();
    } while (mbExitCheck() == FALSE);

    HuPrcEnd();
}

static void AudFXMainDestroy(void)
{
    mbAudFXObjClose();
    audFXProc = NULL;
}

int mbAudFXPlay(s16 seId)
{
    int seNo = MSM_SENO_NONE;

    if (audFXDelay > 0) {
        int delay = audFXDelay;
        MBAUDFXDATA *audFx = &audFXData[0];
        int i;

        for (i = 0; i < MB_AUD_FX_DELAY_MAX; i++, audFx++) {
            if (audFx->seId < 0) {
                break;
            }
        }
        if (i < MB_AUD_FX_DELAY_MAX) {
            audFx->seId = seId;
            audFx->delay = delay;
            audFx->type = MB_AUD_FX_TYPE_NORMAL;
            audFx->pan = 0;
            AudFXPosSet(audFx, NULL);
        }
    } else {
        seNo = HuAudFXPlay(seId);
    }
    audFXDelay = 0;
    return seNo;
}

void mbAudFXStop(int seNo)
{
    HuAudFXStop(seNo);
}

void mbAudFXStopAll(int speed)
{
    MBAUDFXOBJ *audFx = &audFXObjData[0];
    MBAUDFXDATA *delayFx = &audFXData[0];
    int i;

    if (speed <= 0) {
        speed = 1000;
    }
    msmSeStopAll(FALSE, speed);

    memset(&audFXObjData[0], 0, sizeof(audFXObjData));
    for (i = 0, audFx = &audFXObjData[0]; i < MB_AUD_FX_OBJ_MAX; i++, audFx++) {
        audFx->seId = MSM_SE_NONE;
        audFx->seNo = MSM_SENO_NONE;
    }

    memset(&audFXData[0], 0, sizeof(audFXData));
    for (i = 0; i < MB_AUD_FX_DELAY_MAX; i++, delayFx++) {
        delayFx->seId = MSM_SE_NONE;
    }
}

int mbAudFXPosPlay(s16 seId, Vec *pos)
{
    int pan;
    int seNo = MSM_SENO_NONE;

    pan = mbAudFXPosPanGet(pos);

    if (audFXDelay > 0) {
        int delay = audFXDelay;
        MBAUDFXDATA *audFx = &audFXData[0];
        int i;

        for (i = 0; i < MB_AUD_FX_DELAY_MAX; i++, audFx++) {
            if (audFx->seId < 0) {
                break;
            }
        }
        if (i < MB_AUD_FX_DELAY_MAX) {
            audFx->seId = seId;
            audFx->delay = delay;
            audFx->type = MB_AUD_FX_TYPE_PAN;
            audFx->pan = pan;
            AudFXPosSet(audFx, NULL);
        }
    } else {
        seNo = HuAudFXPlayPan(seId, pan);
    }
    audFXDelay = 0;
    return seNo;
}

u8 mbAudFXPosPanGet(Vec *pos)
{
    Vec pos2D;
    int pan;

    Hu3D3Dto2D(pos, HU3D_CAM0, &pos2D);
    pan = (int)((pos2D.x * (1.0f / HU_DISP_WIDTH)) * MSM_PAN_WIDTH) + MSM_PAN_LEFT;
    if (pan < MSM_PAN_LEFT) {
        pan = MSM_PAN_LEFT;
    } else if (pan > MSM_PAN_RIGHT) {
        pan = MSM_PAN_RIGHT;
    }
    return pan;
}

int mbAudFXEmitterPlay(int seId, Vec *pos)
{
    int seNo = MSM_SENO_NONE;

    if (audFXDelay > 0) {
        int delay = audFXDelay;
        MBAUDFXDATA *audFx = &audFXData[0];
        int i;

        for (i = 0; i < MB_AUD_FX_DELAY_MAX; i++, audFx++) {
            if (audFx->seId < 0) {
                break;
            }
        }
        if (i < MB_AUD_FX_DELAY_MAX) {
            audFx->seId = (s16)seId;
            audFx->delay = delay;
            audFx->type = MB_AUD_FX_TYPE_EMITTER;
            audFx->pan = 0;
            AudFXPosSet(audFx, pos);
        }
    } else {
        seNo = HuAudFXEmiterPlay(seId, pos);
    }
    audFXDelay = 0;
    return seNo;
}

void mbAudFXVolSet(int seNo, s16 vol)
{
    HuAudFXVolSet(seNo, vol);
}

void mbAudFXPanning(int seNo, s16 pan)
{
    HuAudFXPanning(seNo, pan);
}

void mbAudFXPosPanning(int seNo, Vec *pos)
{
    s16 pan = mbAudFXPosPanGet(pos);

    mbAudFXPanning(seNo, pan);
}

void mbAudFXDelaySet(int delay)
{
    audFXDelay = delay;
}

int mbAudGuidePlay(s16 seId)
{
    int delay1;
    int delay2;
    BOOL partyF;
    int timeNo;
    int i;

    if (GwSystem.curTime) {
        partyF = GwSystem.partyF;

        if (partyF != FALSE) {
            goto time_one;
        }
    }
    timeNo = 0;
    goto time_set;
time_one:
    timeNo = 1;
time_set:

    for (i = 0; guideFxTbl[i][0] != -1; i++) {
        if (seId == guideFxTbl[i][0] || seId == guideFxTbl[i][1]) {
            break;
        }
    }
    if (guideFxTbl[i][timeNo] != -1) {
        int seNo;
        s16 guideSeId = guideFxTbl[i][timeNo];

        seNo = MSM_SENO_NONE;

        if (audFXDelay > 0) {
            MBAUDFXDATA *audFx;
            int j;

            delay1 = audFXDelay;
            audFx = &audFXData[0];
            for (j = 0; j < MB_AUD_FX_DELAY_MAX; j++, audFx++) {
                if (audFx->seId < 0) {
                    break;
                }
            }
            if (j < MB_AUD_FX_DELAY_MAX) {
                audFx->seId = guideSeId;
                audFx->delay = delay1;
                audFx->type = MB_AUD_FX_TYPE_NORMAL;
                audFx->pan = 0;
                AudFXPosSet(audFx, NULL);
            }
        } else {
            seNo = HuAudFXPlay(guideSeId);
        }
        audFXDelay = 0;
        return seNo;
    } else {
        int seNo = MSM_SENO_NONE;

        if (audFXDelay > 0) {
            MBAUDFXDATA *audFx;
            int j;

            delay2 = audFXDelay;
            audFx = &audFXData[0];
            for (j = 0; j < MB_AUD_FX_DELAY_MAX; j++, audFx++) {
                if (audFx->seId < 0) {
                    break;
                }
            }
            if (j < MB_AUD_FX_DELAY_MAX) {
                audFx->seId = seId;
                audFx->delay = delay2;
                audFx->type = MB_AUD_FX_TYPE_NORMAL;
                audFx->pan = 0;
                AudFXPosSet(audFx, NULL);
            }
        } else {
            seNo = HuAudFXPlay(seId);
        }
        audFXDelay = 0;
        return seNo;
    }
}
