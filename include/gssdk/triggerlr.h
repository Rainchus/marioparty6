#ifndef GSSDK_TRIGGERLR_H
#define GSSDK_TRIGGERLR_H

#include <stddef.h>

#include "types.h"

#include "gssdk/tos.h"

typedef struct TriggerLR {
    TosBaseBlock base;
    u32 frameCount;
    u32 speechStartFrame;
    u32 triggerEventMode;
    u32 resetStartsInputQueues;
    u32 holdInputQueuesAfterTrigger;
    u32 speechActive;
    u32 activitySeen;
    u32 endSilenceFrames;
    u32 endSilenceMilliseconds;
    u32 triggerLookbackLimit;
    u32 silenceFrames;
    u32 silenceFrameLimit;
    u32 minimumSpeechFrames;
    u32 minimumSpeechMilliseconds;
    u32 candidateFrames;
    u32 consecutiveEnergeticFrames;
    u32 consecutiveEnergeticLimit;
    u32 triggerLookbackFrames;
    s32 energyFloorDbHundredths;
    f32 energyFloorLog;
    f32 activityThresholdLog;
    f32 onsetThresholdLog;
    f32 peakActivityThresholdLog;
    f32 peakActivityLog;
    f32 noiseFloorOffsetLog;
    f32 histogramThresholdOffsetLog;
    u32 processMode;
    u32 speechState;
    f32 histogramQuantile;
    f32 histogramCandidatePeak;
    f32 histogramLowerQuantile;
    u32 histogramUpdateFrames;
    u32 histogramUpdated;
    u32 histogramValid;
    u32 energyCriterionMet;
    u32 consecutiveCriterionMet;
    u32 voicingCriterionMet;
    f32 *voicingDecisionHistory;
    f32 *voicingDecisionHistoryEnd;
    f32 *voicingDecisionHistoryWrite;
    f32 voicingDecisionThreshold;
    u32 sensitivity;
    u32 sensitivityMinimum;
    u32 sensitivityMaximum;
    s32 histogramOffsetMinimum;
    s32 histogramOffsetMaximum;
    f32 voicingThresholdMinimum;
    f32 voicingThresholdMaximum;

    f32 histogramMinimum;
    f32 histogramRange;
    u32 histogramBinCount;
    s32 histogramLastBin;
    f32 histogramBinWidth;
    u32 *histogramBins;
    u32 histogramItemCount;
    u32 histogramHistoryLength;
    s32 *histogramHistory;
    s32 *histogramHistoryEnd;
    s32 *histogramHistoryWrite;

    u32 minimumLag;
    u32 maximumLag;
    u32 frameLength;
    u32 historyLength;
    s16 *signalHistory;
    s16 *signalWrite;
    s16 *signalEnd;
    u32 *energyHistory;
    u32 *energyWrite;
    u32 *energyEnd;
    f32 *noiseEnergyWrite;
    f32 *noiseEnergyHistory;
    f32 *noiseEnergyEnd;
    u32 noiseEnergyCount;
    u32 noiseEnergyCapacity;
    f32 voicingThreshold;
    f32 minimumLogEnergy;

    u32 subsamplerInputCount;
    s16 *subsamplerOutput;
    u32 impulseLength;
    u32 halfImpulseLengthPlusOne;
    u32 halfImpulseLength;
    s32 *coefficients;
    s32 *coefficientsEnd;
    u32 coefficientShift;
    u32 sampleShift;
    s32 *delay;
    s32 *delayEnd;
    s32 *delayWrite;
    s32 *delayRead;
    s32 **productRows;
    s32 **productRow;
    s32 **productRowsEnd;
    s32 subsamplerPhase;
} TriggerLR;

typedef struct TriggerLRControlData {
    u32 resetStartsInputQueues;
    u32 holdInputQueuesAfterTrigger;
    u32 endSilenceMilliseconds;
    u32 sensitivity;
    u32 minimumSpeechMilliseconds;
    s32 energyFloorDbHundredths;
} TriggerLRControlData;

typedef struct SlidingHistoSessionData {
    u32 historyWriteIndex;
    s32 history[1];
} SlidingHistoSessionData;

#define TRIGGERLR_LAYOUT_ASSERT(name, condition) \
    typedef char name[(condition) ? 1 : -1]

TRIGGERLR_LAYOUT_ASSERT(
    TriggerLRHistogramOffset, offsetof(TriggerLR, histogramMinimum) == 0xE8);
TRIGGERLR_LAYOUT_ASSERT(
    TriggerLRVoicingOffset, offsetof(TriggerLR, minimumLag) == 0x114);
TRIGGERLR_LAYOUT_ASSERT(
    TriggerLRSubsamplerOffset,
    offsetof(TriggerLR, subsamplerInputCount) == 0x158);
TRIGGERLR_LAYOUT_ASSERT(TriggerLRSize, sizeof(TriggerLR) == 0x19C);
TRIGGERLR_LAYOUT_ASSERT(
    TriggerLRControlDataSize, sizeof(TriggerLRControlData) == 0x18);

#undef TRIGGERLR_LAYOUT_ASSERT

void SlidingHisto_NewItem(TriggerLR *block, f32 item);
f32 SlidingHisto_LowerQuantile(TriggerLR *block, f32 quantile);
void SlidingHisto_Clear(TriggerLR *block);
u32 SlidingHisto_Init(TriggerLR *block);
void SlidingHisto_Free(TriggerLR *block);
u32 SlidingHisto_sizeof_SessionData(TriggerLR *block);
void SlidingHisto_PutSession(
    TriggerLR *block, const SlidingHistoSessionData *session);
void SlidingHisto_GetSession(
    TriggerLR *block, SlidingHistoSessionData *session);

s32 Voicing_AddSignal(TriggerLR *block, s16 *input);
void Voicing_MaintainNoiseEner(TriggerLR *block);
f32 Voicing_GetMaxVoicing(TriggerLR *block);
void Voicing_Reset(TriggerLR *block);
s32 InitVoicing(TriggerLR *block);
void Voicing_Free(TriggerLR *block);

s32 Subsampler_Process(TriggerLR *block, const s16 *input);
void Subsampler_Reset(TriggerLR *block);
s32 Subsampler_Init(TriggerLR *block);
void Subsampler_Free(TriggerLR *block);

#endif
