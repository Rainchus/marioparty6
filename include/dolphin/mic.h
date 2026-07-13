#ifndef _DOLPHIN_MIC_H_
#define _DOLPHIN_MIC_H_

#include <dolphin/types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MIC_RINGBUFF_SIZE (12 * 1024)

#define MIC_RESULT_UNLOCKED 1
#define MIC_RESULT_READY 0
#define MIC_RESULT_BUSY -1
#define MIC_RESULT_WRONGDEVICE -2
#define MIC_RESULT_NOCARD -3
#define MIC_RESULT_INVALID_STATE -4
#define MIC_RESULT_FATAL_ERROR -128

#define MIC_BUTTON_0 0x0001
#define MIC_BUTTON_1 0x0002
#define MIC_BUTTON_2 0x0004
#define MIC_BUTTON_3 0x0008
#define MIC_BUTTON_4 0x0010
#define MIC_BUTTON_TALK MIC_BUTTON_4

typedef void (*MICCallback)(s32 chan, s32 result);

void MICInit(void);
s32 MICProbeEx(s32 chan);

s32 MICMountAsync(s32 chan, s16* buffer, s32 size, MICCallback detachCallback,
                  MICCallback attachCallback);
s32 MICMount(s32 chan, s16* buffer, s32 size, MICCallback detachCallback);
s32 MICUnmount(s32 chan);
s32 MICGetRingbuffsize(s32 chan, s32* size);
s32 MICGetSampleRate(s32 chan, s32* rate);

s32 MICSetGainAsync(s32 chan, s32 gain, MICCallback callback);
s32 MICSetGain(s32 chan, s32 gain);
s32 MICGetButton(s32 chan, u32* button);
s32 MICGetDeviceID(s32 chan, u32* id);

s32 MICStartAsync(s32 chan, MICCallback callback);
s32 MICStart(s32 chan);
s32 MICStopAsync(s32 chan, MICCallback callback);
s32 MICStop(s32 chan);

s32 MICGetCurrentTop(s32 chan);
s32 MICUpdateIndex(s32 chan, s32 index, s32 samples);
s32 MICGetSamplesLeft(s32 chan, s32 index);
s32 MICGetSamples(s32 chan, s16* buffer, s32 index, s32 samples);

BOOL MICIsActive(s32 chan);
BOOL MICIsAttached(s32 chan);

#ifdef __cplusplus
}
#endif

#endif
