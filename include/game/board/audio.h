#ifndef _BOARD_AUDIO_H
#define _BOARD_AUDIO_H

#include "game/audio.h"

#define MB_AUD_FX_OBJ_MAX 16
#define MB_AUD_FX_DELAY_MAX 32
#define MB_AUD_FXNO_OFF (-99)

#define MB_MUS_CHAN_BG 0
#define MB_MUS_CHAN_FG 1
#define MB_MUS_CHAN_MAX 2

void mbAudInit(void);
void mbAudClose(void);

void mbMusBoardPlay(void);
void mbMusPlay(int chan, int id, s8 vol, u16 fadeSpeed);
void mbMusFadeOut(int chan);
void mbMusStop(int chan, BOOL unused);
void mbMusFadeOutSpeed(int chan, u16 speed);
void mbMusStub(void);
void mbMusParamSet(int chan, s8 vol, u16 fadeSpeed);
void mbMusPauseFadeOut(int chan, BOOL pauseF, int speed);
s32 mbMusStatGet(int chan);
BOOL mbMusCheck(int chan);
BOOL mbMusStopCheck(int chan);
BOOL mbMusFadeCheck(int chan);
BOOL mbMusEndCheck(int chan);
int mbMusJinglePlay(s16 id);
void mbMusJingleWait(int streamNo);
s32 mbMusJingleStatGet(int streamNo);
BOOL mbMusBoardFadeOut(int chan, int nextChan, int speed, int fadeSpeed, int musId, BOOL pauseF);
BOOL mbMusBoardFadeCheck(void);
void mbMusPauseSet(BOOL pauseF);

void mbAudStub1(void);
void mbAudStub2(void);
int mbAudFXObjSet(int seId);
int mbAudFXObjCreate(int seId, BOOL multiF);
void mbAudFXObjVolSet(int seNo, s16 vol);
void mbAudFXObjKill(int seNo);
void mbAudFXObjClose(void);
int mbAudFXObjEmitterSet(int seId, Vec *pos);
int mbAudFXObjEmitterCreate(int seId, Vec *pos, BOOL multiF);
void mbAudFXObjEmitterUpdate(int seNo, Vec *pos);
void mbAudFXObjEmiterPosSet(int seId, Vec *pos);
void mbAudFXObjRefSet(int seId, int *fxRef);

int mbAudFXPlay(s16 seId);
void mbAudFXStop(int seNo);
void mbAudFXStopAll(int speed);
int mbAudFXPosPlay(s16 seId, Vec *pos);
u8 mbAudFXPosPanGet(Vec *pos);
int mbAudFXEmitterPlay(int seId, Vec *pos);
void mbAudFXVolSet(int seNo, s16 vol);
void mbAudFXPanning(int seNo, s16 pan);
void mbAudFXPosPanning(int seNo, Vec *pos);
void mbAudFXDelaySet(int delay);
int mbAudGuidePlay(s16 seId);

#endif
