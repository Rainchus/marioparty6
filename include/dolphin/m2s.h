#ifndef _DOLPHIN_M2S_H_
#define _DOLPHIN_M2S_H_

#include <dolphin/types.h>

#ifdef __cplusplus
extern "C" {
#endif

void M2SInit(void);
BOOL M2SSetBuffer(s16* buffer);
BOOL M2SSetActiveChannel(s32 chan);
BOOL M2SSetMode(s32 mode);
BOOL M2SSetPrerecordSamples(s32 milliseconds);
BOOL M2SSetShifts(s32 shifts);

void M2SOpen(void);
void M2SClose(void);
void M2SStart(void);
void M2SStop(void);

s32 M2SGetSamples(s16** buffer, s32 samples);
s32 M2SGetSamplesLeft(void);
void M2SAdvanceBuffer(s32 samples);

#ifdef __cplusplus
}
#endif

#endif
