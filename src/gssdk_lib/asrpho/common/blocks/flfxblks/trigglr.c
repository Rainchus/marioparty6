#include <stdlib.h>

#include "gssdk/triggerlr.h"

extern void *heap_Calloc(void *heap, u32 count, u32 size);
extern void heap_Free(void *heap, void *ptr);
extern f32 logf_check(f32 value);
extern u8 qQueueControl(
    TosQueue *queue, u32 command, u32 elementSize, void *argument);
extern u8 _tosControl(
    TosBaseBlock *block, u32 command, u32 argument0, u32 argument1);
extern u8 tosBaseBlockStartInputQueues(TosBaseBlock *block, u32 reader);
extern u8 tosBaseBlockDisableInputQueues(TosBaseBlock *block);

static s32 TriggerLR_FindSpeech(TriggerLR *block, void **inputs);
static void ProcessTriggerLR(
    TosBaseBlock *baseBlock, void **inputs, s32 inputCount);
static u32 ControlTriggerLR(
    TosBaseBlock *baseBlock, u32 command, void *argument, u32 argumentSize);
static u32 InitTriggerLR(TosBaseBlock *baseBlock);

static s32 TriggerLR_FindSpeech(TriggerLR *block, void **inputs)
{
    f32 *featureValues = (f32 *)inputs[1];
    f32 activity = featureValues[0];
    f32 onsetMetric = *(f32 *)inputs[2];
    f32 peakMetric = *(f32 *)inputs[3];
    f32 histogramThreshold =
        featureValues[3] + block->noiseFloorOffsetLog;
    f32 energyThreshold;

    if (histogramThreshold < block->energyFloorLog) {
        histogramThreshold = block->energyFloorLog;
    }
    if (!block->histogramValid) {
        block->histogramLowerQuantile =
            histogramThreshold + block->histogramThresholdOffsetLog;
    }

    energyThreshold =
        block->histogramLowerQuantile - block->histogramThresholdOffsetLog;
    if (energyThreshold < histogramThreshold) {
        energyThreshold = histogramThreshold;
    }

    switch (block->speechState) {
    case 0:
        if (block->triggerLookbackFrames > block->triggerLookbackLimit) {
            block->triggerLookbackFrames = block->triggerLookbackLimit;
        }

        if (onsetMetric > block->onsetThresholdLog) {
            block->speechState = 1;
            block->histogramUpdated = 0;
            block->energyCriterionMet = 0;
            block->consecutiveCriterionMet = 0;
            block->activitySeen = 0;
            block->candidateFrames = 1;
            block->silenceFrames = 0;
            block->consecutiveEnergeticFrames =
                activity >= energyThreshold ? 1 : 0;
            block->histogramCandidatePeak = peakMetric;
            block->peakActivityLog = onsetMetric;

            if (block->processMode == 0) {
                block->voicingDecisionHistory[0] = -2147483600.0f;
                block->voicingDecisionHistory[1] = -2147483600.0f;
                block->voicingDecisionHistory[2] = -2147483600.0f;
                block->voicingDecisionHistoryWrite =
                    block->voicingDecisionHistory;

                if (activity >= energyThreshold) {
                    *block->voicingDecisionHistoryWrite =
                        Voicing_GetMaxVoicing(block);
                }
                block->voicingCriterionMet = 0;
            }
        } else {
            if (block->speechActive &&
                block->holdInputQueuesAfterTrigger) {
                block->silenceFrames++;
            }
            if (block->processMode == 0) {
                Voicing_MaintainNoiseEner(block);
            }
        }
        break;

    case 1:
        if (!block->voicingCriterionMet) {
            block->voicingDecisionHistoryWrite++;
            if (block->voicingDecisionHistoryWrite ==
                block->voicingDecisionHistoryEnd) {
                block->voicingDecisionHistoryWrite =
                    block->voicingDecisionHistory;
            }
            *block->voicingDecisionHistoryWrite = -2147483600.0f;
        }

        if (onsetMetric > block->activityThresholdLog) {
            if (block->histogramCandidatePeak < peakMetric) {
                block->histogramCandidatePeak = peakMetric;
            }
            if (block->peakActivityLog < onsetMetric) {
                block->peakActivityLog = onsetMetric;
            }
            block->candidateFrames++;

            if (activity >= energyThreshold) {
                block->consecutiveEnergeticFrames++;
                block->activitySeen = 1;
            } else {
                block->consecutiveEnergeticFrames = 0;
            }

            if (!block->consecutiveCriterionMet &&
                block->consecutiveEnergeticFrames >=
                    block->consecutiveEnergeticLimit) {
                block->consecutiveCriterionMet = 1;
            }

            if (activity >= energyThreshold &&
                !block->voicingCriterionMet) {
                f32 first;
                f32 second;
                f32 third;
                f32 low;
                f32 high;
                f32 median;

                *block->voicingDecisionHistoryWrite =
                    Voicing_GetMaxVoicing(block);
                first = block->voicingDecisionHistory[0];
                second = block->voicingDecisionHistory[1];
                third = block->voicingDecisionHistory[2];
                if (first < second) {
                    low = first;
                    high = second;
                } else {
                    low = second;
                    high = first;
                }
                if (third > high) {
                    median = high;
                } else if (third > low) {
                    median = third;
                } else {
                    median = low;
                }
                if (median > block->voicingDecisionThreshold) {
                    block->voicingCriterionMet = 1;
                }
            }

            if (!block->energyCriterionMet &&
                block->candidateFrames >= block->minimumSpeechFrames &&
                block->peakActivityLog >
                    block->peakActivityThresholdLog &&
                block->histogramCandidatePeak >= histogramThreshold) {
                block->energyCriterionMet = 1;
            }

            if (block->candidateFrames >= block->histogramUpdateFrames &&
                block->energyCriterionMet) {
                SlidingHisto_NewItem(block, block->histogramCandidatePeak);
                block->histogramUpdated = 1;
                block->histogramValid = 1;
                block->histogramLowerQuantile =
                    SlidingHisto_LowerQuantile(
                        block, block->histogramQuantile);
                block->candidateFrames = block->minimumSpeechFrames;
                block->histogramCandidatePeak = peakMetric;
            }

            if (block->energyCriterionMet &&
                block->consecutiveCriterionMet &&
                block->voicingCriterionMet) {
                block->speechState = 3;
                block->speechActive = 1;
            }
        } else {
            block->speechState = 2;
            block->silenceFrames = 1;
        }
        break;

    case 2:
        if (!block->voicingCriterionMet) {
            block->voicingDecisionHistoryWrite++;
            if (block->voicingDecisionHistoryWrite ==
                block->voicingDecisionHistoryEnd) {
                block->voicingDecisionHistoryWrite =
                    block->voicingDecisionHistory;
            }
            *block->voicingDecisionHistoryWrite = -2147483600.0f;
        }

        if (onsetMetric > block->onsetThresholdLog) {
            block->speechState = 1;
            block->candidateFrames = 1;
            if (block->histogramCandidatePeak < peakMetric) {
                block->histogramCandidatePeak = peakMetric;
            }
            block->peakActivityLog = onsetMetric;
            block->energyCriterionMet = 0;
            block->consecutiveCriterionMet = 0;
            block->consecutiveEnergeticFrames =
                activity >= energyThreshold ? 1 : 0;

            if (activity >= energyThreshold &&
                !block->voicingCriterionMet) {
                f32 first;
                f32 second;
                f32 third;
                f32 low;
                f32 high;
                f32 median;

                *block->voicingDecisionHistoryWrite =
                    Voicing_GetMaxVoicing(block);
                first = block->voicingDecisionHistory[0];
                second = block->voicingDecisionHistory[1];
                third = block->voicingDecisionHistory[2];
                if (first < second) {
                    low = first;
                    high = second;
                } else {
                    low = second;
                    high = first;
                }
                if (third > high) {
                    median = high;
                } else if (third > low) {
                    median = third;
                } else {
                    median = low;
                }
                if (median > block->voicingDecisionThreshold) {
                    block->voicingCriterionMet = 1;
                }
            }
        } else {
            block->silenceFrames++;
            if (block->silenceFrames > block->silenceFrameLimit) {
                block->speechState = 0;
                if (block->activitySeen &&
                    _tosControl(
                        &block->base, 0xCA, block->frameCount, 70)) {
                    return 1;
                }
                if (!block->histogramUpdated &&
                    block->energyCriterionMet) {
                    SlidingHisto_NewItem(
                        block, block->histogramCandidatePeak);
                    block->histogramUpdated = 1;
                    block->histogramValid = 1;
                    block->histogramLowerQuantile =
                        SlidingHisto_LowerQuantile(
                            block, block->histogramQuantile);
                }
            }
        }
        break;

    case 3:
        if (onsetMetric > block->activityThresholdLog) {
            if (block->histogramCandidatePeak < peakMetric) {
                block->histogramCandidatePeak = peakMetric;
            }
            block->candidateFrames++;
            if (block->candidateFrames >= block->histogramUpdateFrames) {
                SlidingHisto_NewItem(block, block->histogramCandidatePeak);
                block->histogramUpdated = 1;
                block->histogramValid = 1;
                block->histogramLowerQuantile =
                    SlidingHisto_LowerQuantile(
                        block, block->histogramQuantile);
                block->candidateFrames = 0;
                block->histogramCandidatePeak = peakMetric;
            }
        } else {
            block->speechState = 4;
            block->silenceFrames = 1;
        }
        break;

    case 4:
        if (onsetMetric > block->onsetThresholdLog) {
            block->speechState = 3;
            block->candidateFrames++;
            if (block->histogramCandidatePeak < peakMetric) {
                block->histogramCandidatePeak = peakMetric;
            }
        } else {
            block->silenceFrames++;
            if (block->silenceFrames > block->silenceFrameLimit) {
                block->speechState = 0;
                if (!block->histogramUpdated) {
                    SlidingHisto_NewItem(
                        block, block->histogramCandidatePeak);
                    block->histogramUpdated = 1;
                    block->histogramValid = 1;
                    block->histogramLowerQuantile =
                        SlidingHisto_LowerQuantile(
                            block, block->histogramQuantile);
                }
            }
        }
        break;
    }

    return 0;
}

static void ProcessTriggerLR(
    TosBaseBlock *baseBlock, void **inputs, s32 inputCount)
{
    TriggerLR *block = (TriggerLR *)baseBlock;

    (void)inputCount;
    if (inputs[0] == NULL) {
        return;
    }

    block->frameCount++;
    if (block->processMode == 0) {
        if (Voicing_AddSignal(block, (s16 *)inputs[0])) {
            return;
        }
        block->triggerLookbackFrames++;
        if (block->triggerLookbackFrames > block->triggerLookbackLimit) {
            block->triggerLookbackFrames = block->triggerLookbackLimit;
        }
    }

    if (TriggerLR_FindSpeech(block, inputs)) {
        return;
    }

    if (block->processMode == 0) {
        if (block->speechState != 3) {
            return;
        }

        block->speechStartFrame =
            block->frameCount - block->triggerLookbackFrames;
        if (!block->holdInputQueuesAfterTrigger &&
            !qQueueControl(
                block->base.input[0].queue, 8,
                block->base.input[0].outputSize, NULL)) {
            return;
        }

        if (_tosControl(
                baseBlock, block->triggerEventMode ? 0xCB : 0xC8,
                block->triggerLookbackFrames, block->frameCount)) {
            return;
        }

        if (block->holdInputQueuesAfterTrigger) {
            block->processMode = 1;
        } else {
            tosBaseBlockDisableInputQueues(baseBlock);
        }
    } else if (block->processMode == 1 && block->speechActive &&
        block->silenceFrames > block->endSilenceFrames) {
        block->speechActive = 0;
        if (_tosControl(baseBlock, 0xC9, block->frameCount, 0)) {
            return;
        }
        block->processMode = 2;
    }
}

static u32 ControlTriggerLR(
    TosBaseBlock *baseBlock, u32 command, void *argument, u32 argumentSize)
{
    TriggerLR *block = (TriggerLR *)baseBlock;
    TriggerLRControlData *controlData;
    u32 *sessionOffset;
    u8 *sessionBuffer;
    u32 queueControlFailed;
    f32 sensitivityScale;

    switch ((u8)command) {
    case 0x64:
        block->frameCount = 0;
        block->speechStartFrame = 0;
        block->triggerLookbackFrames = 0;
        block->candidateFrames = 0;
        block->silenceFrames = 0;
        block->speechActive = 0;
        block->voicingCriterionMet = 0;
        block->speechState = 0;
        Voicing_Reset(block);

        if (block->resetStartsInputQueues) {
            block->processMode = 0;
            if (tosBaseBlockStartInputQueues(baseBlock, 0)) {
                return 0;
            }
        } else if (block->holdInputQueuesAfterTrigger) {
            block->processMode = 1;
            queueControlFailed = !qQueueControl(
                baseBlock->input[1].queue, 5,
                baseBlock->input[1].outputSize, NULL);
            queueControlFailed |= !qQueueControl(
                baseBlock->input[2].queue, 5,
                baseBlock->input[2].outputSize, NULL);
            queueControlFailed |= !qQueueControl(
                baseBlock->input[3].queue, 5,
                baseBlock->input[3].outputSize, NULL);
            if (queueControlFailed) {
                return 0;
            }
        } else {
            block->processMode = 2;
            if (tosBaseBlockDisableInputQueues(baseBlock)) {
                return 0;
            }
        }
        return 1;

    case 0x65:
        *(u32 **)argument = &block->speechStartFrame;
        return 1;

    case 0x66:
        controlData = (TriggerLRControlData *)argument;
        controlData->resetStartsInputQueues =
            block->resetStartsInputQueues;
        controlData->holdInputQueuesAfterTrigger =
            block->holdInputQueuesAfterTrigger;
        controlData->endSilenceMilliseconds =
            block->endSilenceMilliseconds;
        controlData->sensitivity = block->sensitivity;
        controlData->minimumSpeechMilliseconds =
            block->minimumSpeechMilliseconds;
        controlData->energyFloorDbHundredths =
            block->energyFloorDbHundredths;
        return 1;

    case 0x67:
        controlData = (TriggerLRControlData *)argument;
        block->resetStartsInputQueues =
            controlData->resetStartsInputQueues;
        block->holdInputQueuesAfterTrigger =
            controlData->holdInputQueuesAfterTrigger;
        block->endSilenceMilliseconds =
            controlData->endSilenceMilliseconds;
        block->sensitivity = controlData->sensitivity;
        block->minimumSpeechMilliseconds =
            controlData->minimumSpeechMilliseconds;
        block->energyFloorDbHundredths =
            controlData->energyFloorDbHundredths;

        sensitivityScale =
            (f32)(block->sensitivity - block->sensitivityMinimum) /
            (f32)(block->sensitivityMaximum - block->sensitivityMinimum);
        block->histogramThresholdOffsetLog =
            ((f32)block->histogramOffsetMinimum +
                sensitivityScale *
                    (f32)(block->histogramOffsetMaximum -
                        block->histogramOffsetMinimum)) *
            0.23025851f;
        block->voicingDecisionThreshold = logf_check(
            block->voicingThresholdMinimum +
            sensitivityScale *
                (block->voicingThresholdMaximum -
                    block->voicingThresholdMinimum));
        block->endSilenceFrames = block->endSilenceMilliseconds / 10;
        block->minimumSpeechFrames =
            block->minimumSpeechMilliseconds / 10;
        block->energyFloorLog =
            27.37f +
            (f32)block->energyFloorDbHundredths / 4.3429446f / 100.0f;
        return 1;

    case 0x68:
        block->triggerEventMode = (u32)argument;
        return 1;

    case 0x69:
        sessionOffset = (u32 *)argumentSize;
        sessionBuffer = (u8 *)argument;
        SlidingHisto_Clear(block);
        Voicing_Reset(block);
        block->histogramValid = 0;
        if (sessionBuffer != NULL) {
            sessionBuffer += *sessionOffset;
            block->histogramValid = *(u32 *)sessionBuffer;
            SlidingHisto_PutSession(
                block, (SlidingHistoSessionData *)(sessionBuffer + 4));
            if (block->histogramValid) {
                block->histogramLowerQuantile =
                    SlidingHisto_LowerQuantile(
                        block, block->histogramQuantile);
            }
        }
        *sessionOffset += 4 + SlidingHisto_sizeof_SessionData(block);
        return 1;

    case 0x6A:
        sessionOffset = (u32 *)argumentSize;
        sessionBuffer = (u8 *)argument;
        if (sessionBuffer != NULL) {
            sessionBuffer += *sessionOffset;
            *(u32 *)sessionBuffer = block->histogramValid;
            SlidingHisto_GetSession(
                block, (SlidingHistoSessionData *)(sessionBuffer + 4));
        }
        *sessionOffset += 4 + SlidingHisto_sizeof_SessionData(block);
        return 1;

    case 0xFF:
        if (block->voicingDecisionHistory != NULL) {
            heap_Free(
                block->base.context->heap,
                block->voicingDecisionHistory);
        }
        SlidingHisto_Free(block);
        Voicing_Free(block);
        tosBaseBlockDestruct(block);
        return 1;
    }

    return 0;
}

static u32 InitTriggerLR(TosBaseBlock *baseBlock)
{
    TriggerLR *block = (TriggerLR *)baseBlock;
    TosContext *context = baseBlock->context;
    f32 sensitivityScale;

    baseBlock->input[1].inputSize = 0x18;
    baseBlock->input[2].inputSize = sizeof(f32);
    baseBlock->input[3].inputSize = sizeof(f32);

    block->minimumSpeechFrames = _tosGetProfileU32(block, 1, 6);
    block->minimumSpeechMilliseconds = block->minimumSpeechFrames * 10;
    block->consecutiveEnergeticLimit = _tosGetProfileU32(block, 2, 3);
    block->silenceFrameLimit = _tosGetProfileU32(block, 3, 15);
    block->triggerLookbackLimit = _tosGetProfileU32(block, 4, 45);
    block->energyFloorDbHundredths =
        (s32)_tosGetProfileU32(block, 5, -7200);
    block->triggerEventMode = _tosGetProfileU32(block, 31, 0);
    block->energyFloorLog =
        27.37f +
        (f32)block->energyFloorDbHundredths / 4.3429446f / 100.0f;
    block->activityThresholdLog =
        logf_check(_tosGetProfileFloat(block, 8, 1.0f));
    block->onsetThresholdLog =
        logf_check(_tosGetProfileFloat(block, 9, 5.0f));
    block->peakActivityThresholdLog =
        logf_check(_tosGetProfileFloat(block, 10, 100.0f));
    block->noiseFloorOffsetLog =
        (f32)abs((s32)_tosGetProfileU32(block, 6, 17)) * 0.23025851f;
    block->sensitivityMinimum = _tosGetProfileU32(block, 18, 0);
    block->sensitivityMaximum = _tosGetProfileU32(block, 19, 100);
    block->histogramOffsetMinimum =
        (s32)_tosGetProfileU32(block, 20, 5);
    block->histogramOffsetMaximum =
        (s32)_tosGetProfileU32(block, 21, 25);
    block->voicingThresholdMinimum =
        _tosGetProfileFloat(block, 22, 0.75f);
    block->voicingThresholdMaximum =
        _tosGetProfileFloat(block, 23, 0.65f);
    block->sensitivity = _tosGetProfileU32(block, 7, 50);

    sensitivityScale =
        (f32)(block->sensitivity - block->sensitivityMinimum) /
        (f32)(block->sensitivityMaximum - block->sensitivityMinimum);
    block->histogramThresholdOffsetLog =
        ((f32)block->histogramOffsetMinimum +
            sensitivityScale *
                (f32)(block->histogramOffsetMaximum -
                    block->histogramOffsetMinimum)) *
        0.23025851f;
    block->voicingDecisionThreshold = logf_check(
        block->voicingThresholdMinimum +
        sensitivityScale *
            (block->voicingThresholdMaximum -
                block->voicingThresholdMinimum));
    block->histogramQuantile =
        _tosGetProfileFloat(block, 15, 0.5f);
    block->histogramUpdateFrames = _tosGetProfileU32(block, 16, 100);
    block->histogramValid = 0;

    if (SlidingHisto_Init(block)) {
        return 1;
    }

    block->voicingDecisionHistory =
        heap_Calloc(context->heap, 3, sizeof(f32));
    if (block->voicingDecisionHistory == NULL) {
        _tosErrorLog(block, 2);
        return 1;
    }
    block->voicingDecisionHistoryWrite =
        block->voicingDecisionHistory;
    block->voicingDecisionHistoryEnd =
        block->voicingDecisionHistory + 3;

    if (InitVoicing(block)) {
        return 1;
    }

    baseBlock->input[0].inputSize =
        (u16)(block->subsamplerInputCount * sizeof(s16));
    block->resetStartsInputQueues = 1;
    block->holdInputQueuesAfterTrigger = 1;
    block->endSilenceFrames = _tosGetProfileU32(block, 17, 50);
    block->endSilenceMilliseconds = block->endSilenceFrames * 10;
    return 0;
}

TosBaseBlock *ConstructTriggerLR(TosContext *context, u32 blockIndex)
{
    return tosBaseBlockConstruct(
        context, blockIndex, 4, 0, ProcessTriggerLR, InitTriggerLR,
        ControlTriggerLR, sizeof(TriggerLR));
}
