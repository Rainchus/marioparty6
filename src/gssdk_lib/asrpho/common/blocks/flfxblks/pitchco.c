#include "types.h"

#include "gssdk/tos.h"

typedef struct PitchCombiner {
    TosBaseBlock base;
    f32 *normEnergyHistory;
    f32 *normEnergyOutput;
    f32 *normEnergyWrite;
    f32 *normEnergyEnd;
    f32 *normEnergyFilter;
    f32 *logPitchHistory;
    f32 *logPitchOutput;
    f32 *logPitchWrite;
    f32 *logPitchEnd;
    f32 *logPitchFilter;
    f32 *voicingHistory;
    f32 *voicingOutput;
    f32 *voicingWrite;
    f32 *voicingEnd;
    f32 *pitchSlopeHistory;
    f32 *pitchSlopeOutput;
    f32 *pitchSlopeWrite;
    f32 *pitchSlopeEnd;
    f32 *meanPitchHistory;
    f32 *meanPitchOutput;
    f32 *meanPitchWrite;
    f32 *meanPitchEnd;
    f32 *normEnergySlopeHistory;
    f32 *normEnergySlopeOutput;
    f32 *normEnergySlopeWrite;
    f32 *normEnergySlopeEnd;
    f32 pitchDelta;
    f32 *pitchCoefficients;
    f32 *normEnergyCoefficients;
    u8 pitchWindowLength;
    u8 normEnergyWindowLength;
    u8 delayLength;
    u8 voicedFrameCount;
    u8 queuedFrameCount;
    u8 voiced;
    u8 previousVoiced;
    u8 beginVoiced;
    u8 endVoiced;
    u8 initializeNormEnergy;
    u16 pitchCoefficientDivisor;
    u16 normEnergyCoefficientDivisor;
    f32 lastMeanPitch;
    f32 lastPitchSlope;
    u8 unused;
    u8 outputFeatureCount;
    u8 flushing;
    u8 flushPitch;
    u8 flushNormEnergy;
    u8 pitchFilterReady;
    u16 bandCount;
    f32 voicedOutput;
    f32 unvoicedOutput;
} PitchCombiner;

extern void *heap_Calloc(void *heap, u32 count, u32 size);
extern void heap_Free(void *heap, void *ptr);
extern f32 logf_check(f32 value);

#pragma dont_inline on
static f32 MaxFloatInArray(f32 *values, u16 count)
{
    f32 maximum = values[0];
    f32 *value = values + 1;
    u16 i;

    for (i = 1; i < count; i++, value++) {
        if (*value > maximum) {
            maximum = *value;
        }
    }
    return maximum;
}
#pragma dont_inline reset

static void StatusProcessing(PitchCombiner *block)
{
    block->previousVoiced = block->voiced;
    if (*block->voicingWrite == 1.0f) {
        if (block->voiced == 0) {
            block->voiced = 1;
            block->beginVoiced = 1;
        }
        block->voicedFrameCount++;
    } else if (block->voiced == 1) {
        if (block->voicedFrameCount == block->pitchWindowLength - 1) {
            block->endVoiced = 1;
        }
        block->voiced = 0;
        block->voicedFrameCount = 0;
    }

    block->voicingWrite++;
    if (block->voicingWrite == block->voicingEnd) {
        block->voicingWrite = block->voicingHistory;
    }
}

static void CheckBeginVoicedFlag(PitchCombiner *block)
{
    f32 *destination;
    f32 *source;
    u8 i;

    if (block->beginVoiced != 1) {
        return;
    }

    source = block->meanPitchWrite - 1;
    if (source < block->meanPitchHistory) {
        source = block->meanPitchEnd - 1;
    }
    destination = source - 1;
    if (destination < block->meanPitchHistory) {
        destination = block->meanPitchEnd - 1;
    }
    for (i = 0; i < block->pitchWindowLength / 2; i++) {
        *destination = *source;
        destination--;
        if (destination < block->meanPitchHistory) {
            destination = block->meanPitchEnd - 1;
        }
    }

    source = block->pitchSlopeWrite - 1;
    if (source < block->pitchSlopeHistory) {
        source = block->pitchSlopeEnd - 1;
    }
    destination = source - 1;
    if (destination < block->pitchSlopeHistory) {
        destination = block->pitchSlopeEnd - 1;
    }
    for (i = 0; i < block->pitchWindowLength / 2; i++) {
        *destination = *source;
        destination--;
        if (destination < block->pitchSlopeHistory) {
            destination = block->pitchSlopeEnd - 1;
        }
    }
    block->beginVoiced = 0;
}

static void CheckEndVoicedFlag(PitchCombiner *block)
{
    f32 *destination;
    u8 i;

    if (block->endVoiced != 1) {
        return;
    }

    destination = block->meanPitchWrite - 1;
    if (destination < block->meanPitchHistory) {
        destination = block->meanPitchEnd - 1;
    }
    for (i = 0; i < block->pitchWindowLength / 2; i++) {
        *destination = block->lastMeanPitch;
        destination++;
        if (destination == block->meanPitchEnd) {
            destination = block->meanPitchHistory;
        }
    }

    destination = block->pitchSlopeWrite - 1;
    if (destination < block->pitchSlopeHistory) {
        destination = block->pitchSlopeEnd - 1;
    }
    for (i = 0; i < block->pitchWindowLength / 2; i++) {
        *destination = block->lastPitchSlope;
        destination++;
        if (destination == block->pitchSlopeEnd) {
            destination = block->pitchSlopeHistory;
        }
    }
    block->endVoiced = 0;
}

static void LogPitchProcessing(PitchCombiner *block)
{
    if (block->voicedFrameCount == block->pitchWindowLength) {
        f32 *coefficient;
        f32 *pitch;
        f32 result;
        u8 i;
        u8 pitchWindowLength;
        u16 j;

        block->pitchFilterReady = 1;
        i = 0;
        pitch = block->logPitchFilter;
        coefficient = block->pitchCoefficients;
        result = 0.0f;
        pitchWindowLength = block->pitchWindowLength;
        for (; i < pitchWindowLength; i++) {
            result += *pitch++ * *coefficient++;
            if (pitch == block->logPitchEnd) {
                pitch = block->logPitchHistory;
            }
        }
        result /= block->pitchCoefficientDivisor;
        *block->pitchSlopeWrite = result;
        block->lastPitchSlope = *block->pitchSlopeWrite++;
        if (block->pitchSlopeWrite == block->pitchSlopeEnd) {
            block->pitchSlopeWrite = block->pitchSlopeHistory;
        }

        pitch = block->logPitchFilter;
        j = 0;
        result = 0.0f;
        pitchWindowLength = block->pitchWindowLength;
        for (; j < pitchWindowLength; j++) {
            result += *pitch++;
            if (pitch == block->logPitchEnd) {
                pitch = block->logPitchHistory;
            }
        }
        result /= pitchWindowLength;
        *block->meanPitchWrite = result;
        block->lastMeanPitch = *block->meanPitchWrite++;
        if (block->meanPitchWrite == block->meanPitchEnd) {
            block->meanPitchWrite = block->meanPitchHistory;
        }

        CheckBeginVoicedFlag(block);
        block->voicedFrameCount--;
    } else {
        block->pitchSlopeWrite++;
        if (block->pitchSlopeWrite == block->pitchSlopeEnd) {
            block->pitchSlopeWrite = block->pitchSlopeHistory;
        }
        block->meanPitchWrite++;
        if (block->meanPitchWrite == block->meanPitchEnd) {
            block->meanPitchWrite = block->meanPitchHistory;
        }
        CheckEndVoicedFlag(block);
    }
}

static void NormE_Processing(PitchCombiner *block)
{
    f32 *energy = block->normEnergyFilter;
    f32 *coefficient = block->normEnergyCoefficients;
    f32 result = 0.0f;
    u8 i;

    for (i = 0; i < block->normEnergyWindowLength; i++) {
        result += *energy++ * *coefficient++;
        if (energy == block->normEnergyEnd) {
            energy = block->normEnergyHistory;
        }
    }
    result /= block->normEnergyCoefficientDivisor;
    *block->normEnergySlopeWrite++ = result;
    if (block->normEnergySlopeWrite == block->normEnergySlopeEnd) {
        block->normEnergySlopeWrite = block->normEnergySlopeHistory;
    }

    block->normEnergyFilter++;
    if (block->normEnergyFilter == block->normEnergyEnd) {
        block->normEnergyFilter = block->normEnergyHistory;
    }

    if (block->initializeNormEnergy != 0) {
        f32 latest;
        f32 *destination = block->normEnergySlopeHistory;

        if (block->normEnergySlopeWrite == block->normEnergySlopeHistory) {
            latest = block->normEnergySlopeEnd[-1];
        } else {
            latest = block->normEnergySlopeWrite[-1];
        }
        for (i = 0; i < block->normEnergyWindowLength / 2; i++) {
            *destination++ = latest;
        }
        block->initializeNormEnergy = 0;
    }
}

static void OutputProcedure(PitchCombiner *block)
{
    block->logPitchOutput++;
    if (block->logPitchOutput == block->logPitchEnd) {
        block->logPitchOutput = block->logPitchHistory;
    }
    block->meanPitchOutput++;
    if (block->meanPitchOutput == block->meanPitchEnd) {
        block->meanPitchOutput = block->meanPitchHistory;
    }
    block->pitchSlopeOutput++;
    if (block->pitchSlopeOutput == block->pitchSlopeEnd) {
        block->pitchSlopeOutput = block->pitchSlopeHistory;
    }
    block->normEnergyOutput++;
    if (block->normEnergyOutput == block->normEnergyEnd) {
        block->normEnergyOutput = block->normEnergyHistory;
    }
    block->normEnergySlopeOutput++;
    if (block->normEnergySlopeOutput == block->normEnergySlopeEnd) {
        block->normEnergySlopeOutput = block->normEnergySlopeHistory;
    }
    block->voicingOutput++;
    if (block->voicingOutput == block->voicingEnd) {
        block->voicingOutput = block->voicingHistory;
    }
}

static u8 FlushPitchCombiner(PitchCombiner *block)
{
    if (block->queuedFrameCount != 0) {
        f32 *source;
        f32 *destination;
        f32 *output;
        u16 i;
        u8 halfLength;

        if (block->flushPitch != 0) {
            if (block->queuedFrameCount >=
                (u8)(block->pitchWindowLength - 1)) {
                halfLength = block->pitchWindowLength / 2;
                source = block->pitchSlopeWrite - 1;
                if (source < block->pitchSlopeHistory) {
                    source = block->pitchSlopeEnd - 1;
                }
                for (i = 0; i < halfLength; i++) {
                    *block->pitchSlopeWrite++ = *source;
                    if (block->pitchSlopeWrite == block->pitchSlopeEnd) {
                        block->pitchSlopeWrite = block->pitchSlopeHistory;
                    }
                }

                source = block->meanPitchWrite - 1;
                if (source < block->meanPitchHistory) {
                    source = block->meanPitchEnd - 1;
                }
                for (i = 0; i < halfLength; i++) {
                    *block->meanPitchWrite++ = *source;
                    if (block->meanPitchWrite == block->meanPitchEnd) {
                        block->meanPitchWrite = block->meanPitchHistory;
                    }
                }
            }
            block->flushPitch = 0;
        }

        if (block->flushNormEnergy != 0) {
            source = block->normEnergySlopeWrite - 1;
            destination = block->normEnergySlopeWrite;
            halfLength = block->normEnergyWindowLength / 2;
            if (source < block->normEnergySlopeHistory) {
                source = block->normEnergySlopeEnd - 1;
            }
            for (i = 0; i < halfLength; i++) {
                *destination++ = *source;
                if (destination == block->normEnergySlopeEnd) {
                    destination = block->normEnergySlopeHistory;
                }
            }
            block->flushNormEnergy = 0;
        }

        block->pitchDelta =
            *block->logPitchOutput - *block->meanPitchOutput;
        output = qEnQueueOne(block->base.output->queue);
        *output++ = 32.0f * block->pitchDelta;
        *output++ = 53.0f * *block->pitchSlopeOutput;
        *output++ = 1.107 * *block->normEnergyOutput;
        *output++ = 0.832 * *block->normEnergySlopeOutput;
        if (*block->voicingOutput == 1.0f) {
            *output = block->voicedOutput;
        } else {
            *output = block->unvoicedOutput;
        }

        OutputProcedure(block);
        block->queuedFrameCount--;
        if (block->queuedFrameCount != 0) {
            return 1;
        }
    }

    block->flushing = 0;
    return 0;
}

static void ProcessPitchCombiner(
    TosBaseBlock *baseBlock, void **inputs, s32 inputCount)
{
    PitchCombiner *block = (PitchCombiner *)baseBlock;

    if (inputs[0] != NULL) {
        f32 *output;

        *block->normEnergyWrite++ =
            MaxFloatInArray((f32 *)inputs[2] + 1, block->bandCount);
        if (block->normEnergyWrite == block->normEnergyEnd) {
            block->normEnergyWrite = block->normEnergyHistory;
        }

        *block->logPitchWrite++ = logf_check(*(f32 *)inputs[0]);
        if (block->logPitchWrite == block->logPitchEnd) {
            block->logPitchWrite = block->logPitchHistory;
        }

        *block->voicingWrite = *(u8 *)inputs[1];
        block->queuedFrameCount++;
        StatusProcessing(block);

        if (block->queuedFrameCount >= block->pitchWindowLength) {
            LogPitchProcessing(block);
        }
        if (block->voiced == 0) {
            block->logPitchFilter = block->logPitchWrite;
        } else if (block->pitchFilterReady == 1) {
            block->logPitchFilter++;
            if (block->logPitchFilter == block->logPitchEnd) {
                block->logPitchFilter = block->logPitchHistory;
            }
            block->pitchFilterReady = 0;
        }

        if (block->queuedFrameCount >= block->normEnergyWindowLength) {
            NormE_Processing(block);
        }

        if (block->queuedFrameCount == block->delayLength) {
            block->pitchDelta =
                *block->logPitchOutput - *block->meanPitchOutput;
            output = qEnQueueOne(block->base.output->queue);
            *output++ = 32.0f * block->pitchDelta;
            *output++ = 53.0f * *block->pitchSlopeOutput;
            *output++ = 1.107 * *block->normEnergyOutput;
            *output++ = 0.832 * *block->normEnergySlopeOutput;
            if (*block->voicingOutput == 1.0f) {
                *output = block->voicedOutput;
            } else {
                *output = block->unvoicedOutput;
            }

            OutputProcedure(block);
            block->queuedFrameCount--;
        }
    } else if (block->flushing == 1) {
        FlushPitchCombiner(block);
    }
}

static void PitchCombinerReset(PitchCombiner *block)
{
    {
        u8 length = block->delayLength;
        u16 i = 0;
        f32 *history = block->voicingHistory;

        block->voicingWrite = history;
        block->voicingOutput = history;
        block->voicingEnd = history + length;
        for (; i < length; i++) {
            block->voicingHistory[i] = 0.0f;
        }
    }
    {
        u8 length = block->delayLength;
        u16 i = 0;
        f32 *history = block->normEnergyHistory;

        block->normEnergyWrite = history;
        block->normEnergyOutput = history;
        block->normEnergyEnd = history + length;
        for (; i < length; i++) {
            block->normEnergyHistory[i] = 0.0f;
        }
    }
    {
        u8 length = block->delayLength;
        u16 i = 0;
        f32 *history = block->logPitchHistory;

        block->logPitchWrite = history;
        block->logPitchOutput = history;
        block->logPitchEnd = history + length;
        for (; i < length; i++) {
            block->logPitchHistory[i] = 0.0f;
        }
    }
    {
        u8 length = block->delayLength;
        u16 i = 0;
        f32 *history = block->meanPitchHistory;

        block->meanPitchWrite = history;
        block->meanPitchOutput = history;
        block->meanPitchEnd = history + length;
        for (; i < length; i++) {
            block->meanPitchHistory[i] = 0.0f;
        }
    }
    {
        u8 length = block->delayLength;
        u16 i = 0;
        f32 *history = block->pitchSlopeHistory;

        block->pitchSlopeWrite = history;
        block->pitchSlopeOutput = history;
        block->pitchSlopeEnd = history + length;
        for (; i < length; i++) {
            block->pitchSlopeHistory[i] = 0.0f;
        }
    }
    {
        u8 length = block->delayLength;
        u16 i = 0;
        f32 *history = block->normEnergySlopeHistory;

        block->normEnergySlopeWrite = history;
        block->normEnergySlopeOutput = history;
        block->normEnergySlopeEnd = history + length;
        for (; i < length; i++) {
            block->normEnergySlopeHistory[i] = 0.0f;
        }
    }

    block->meanPitchWrite =
        block->meanPitchHistory + block->pitchWindowLength / 2;
    block->pitchSlopeWrite =
        block->pitchSlopeHistory + block->pitchWindowLength / 2;
    block->normEnergySlopeWrite =
        block->normEnergySlopeHistory + block->normEnergyWindowLength / 2;
    block->logPitchFilter = block->logPitchOutput;
    block->normEnergyFilter = block->normEnergyOutput;

    block->pitchDelta = 0.0f;
    block->lastMeanPitch = 0.0f;
    block->voicedFrameCount = 0;
    block->voiced = 0;
    block->previousVoiced = 0;
    block->queuedFrameCount = 0;
    block->beginVoiced = 0;
    block->endVoiced = 0;
    block->initializeNormEnergy = 1;
    block->flushing = 0;
    block->flushPitch = 1;
    block->flushNormEnergy = 1;
    block->pitchFilterReady = 0;
}

static u32 ControlPitchCombiner(
    TosBaseBlock *baseBlock, u32 command, void *argument, u32 argumentSize)
{
    PitchCombiner *block = (PitchCombiner *)baseBlock;

    switch ((u8)command) {
    case 200:
        PitchCombinerReset(block);
        break;
    case 201:
        block->flushing = 1;
        break;
    case 255:
        heap_Free(block->base.context->heap, block->normEnergyHistory);
        heap_Free(block->base.context->heap, block->logPitchHistory);
        heap_Free(block->base.context->heap, block->voicingHistory);
        heap_Free(block->base.context->heap, block->pitchSlopeHistory);
        heap_Free(block->base.context->heap, block->meanPitchHistory);
        heap_Free(block->base.context->heap, block->normEnergySlopeHistory);
        heap_Free(block->base.context->heap, block->pitchCoefficients);
        heap_Free(block->base.context->heap, block->normEnergyCoefficients);
        tosBaseBlockDestruct(block);
        break;
    default:
        return 0;
    }
    return 1;
}

static u32 InitPitchCombiner(TosBaseBlock *baseBlock)
{
    PitchCombiner *block = (PitchCombiner *)baseBlock;
    TosContext *context = baseBlock->context;
    u32 bandCount;

    bandCount = _tosGetProfileU32(block, 1, 21);
    block->bandCount = (u16)bandCount;
    block->lastMeanPitch = 0.0f;
    block->outputFeatureCount = 5;
    block->base.input[0].inputSize = sizeof(f32);
    block->base.input[1].inputSize = sizeof(u8);
    block->base.input[2].inputSize =
        ((u16)bandCount + 1) * sizeof(f32);
    block->base.output->outputSize =
        block->outputFeatureCount * sizeof(f32);

    block->pitchWindowLength = 7;
    block->normEnergyWindowLength = 5;
    block->delayLength =
        block->pitchWindowLength > block->normEnergyWindowLength
            ? block->pitchWindowLength
            : block->normEnergyWindowLength;

    block->normEnergyHistory =
        heap_Calloc(context->heap, block->delayLength, sizeof(f32));
    block->logPitchHistory =
        heap_Calloc(context->heap, block->delayLength, sizeof(f32));
    block->voicingHistory =
        heap_Calloc(context->heap, block->delayLength, sizeof(f32));
    block->pitchSlopeHistory =
        heap_Calloc(context->heap, block->delayLength, sizeof(f32));
    block->meanPitchHistory =
        heap_Calloc(context->heap, block->delayLength, sizeof(f32));
    block->normEnergySlopeHistory =
        heap_Calloc(context->heap, block->delayLength, sizeof(f32));
    block->pitchCoefficients =
        heap_Calloc(context->heap, block->pitchWindowLength, sizeof(f32));
    block->normEnergyCoefficients = heap_Calloc(
        context->heap, block->normEnergyWindowLength, sizeof(f32));

    if (block->normEnergyHistory == NULL ||
        block->logPitchHistory == NULL ||
        block->voicingHistory == NULL ||
        block->pitchSlopeHistory == NULL ||
        block->meanPitchHistory == NULL ||
        block->normEnergySlopeHistory == NULL ||
        block->pitchCoefficients == NULL ||
        block->normEnergyCoefficients == NULL) {
        _tosErrorLog(block, 2);
        return 1;
    }

    {
        u8 length = block->delayLength;
        u16 i = 0;
        f32 *history = block->voicingHistory;

        block->voicingWrite = history;
        block->voicingOutput = history;
        block->voicingEnd = history + length;
        for (; i < length; i++) {
            block->voicingHistory[i] = 0.0f;
        }
    }
    {
        u8 length = block->delayLength;
        u16 i = 0;
        f32 *history = block->normEnergyHistory;

        block->normEnergyWrite = history;
        block->normEnergyOutput = history;
        block->normEnergyEnd = history + length;
        for (; i < length; i++) {
            block->normEnergyHistory[i] = 0.0f;
        }
    }
    {
        u8 length = block->delayLength;
        u16 i = 0;
        f32 *history = block->logPitchHistory;

        block->logPitchWrite = history;
        block->logPitchOutput = history;
        block->logPitchEnd = history + length;
        for (; i < length; i++) {
            block->logPitchHistory[i] = 0.0f;
        }
    }
    {
        u8 length = block->delayLength;
        u16 i = 0;
        f32 *history = block->meanPitchHistory;

        block->meanPitchWrite = history;
        block->meanPitchOutput = history;
        block->meanPitchEnd = history + length;
        for (; i < length; i++) {
            block->meanPitchHistory[i] = 0.0f;
        }
    }
    {
        u8 length = block->delayLength;
        u16 i = 0;
        f32 *history = block->pitchSlopeHistory;

        block->pitchSlopeWrite = history;
        block->pitchSlopeOutput = history;
        block->pitchSlopeEnd = history + length;
        for (; i < length; i++) {
            block->pitchSlopeHistory[i] = 0.0f;
        }
    }
    {
        u8 length = block->delayLength;
        u16 i = 0;
        f32 *history = block->normEnergySlopeHistory;

        block->normEnergySlopeWrite = history;
        block->normEnergySlopeOutput = history;
        block->normEnergySlopeEnd = history + length;
        for (; i < length; i++) {
            block->normEnergySlopeHistory[i] = 0.0f;
        }
    }

    block->meanPitchWrite =
        block->meanPitchHistory + block->pitchWindowLength / 2;
    block->pitchSlopeWrite =
        block->pitchSlopeHistory + block->pitchWindowLength / 2;
    block->normEnergySlopeWrite =
        block->normEnergySlopeHistory + block->normEnergyWindowLength / 2;
    block->logPitchFilter = block->logPitchOutput;
    block->normEnergyFilter = block->normEnergyOutput;
    block->pitchDelta = 0.0f;

    block->pitchCoefficients[0] = -3.0f;
    block->pitchCoefficients[1] = -2.0f;
    block->pitchCoefficients[2] = -1.0f;
    block->pitchCoefficients[3] = 0.0f;
    block->pitchCoefficients[4] = 1.0f;
    block->pitchCoefficients[5] = 2.0f;
    block->pitchCoefficients[6] = 3.0f;
    block->pitchCoefficientDivisor = 28;

    block->normEnergyCoefficients[0] = -2.0f;
    block->normEnergyCoefficients[1] = -1.0f;
    block->normEnergyCoefficients[2] = 0.0f;
    block->normEnergyCoefficients[3] = 1.0f;
    block->normEnergyCoefficients[4] = 2.0f;
    block->normEnergyCoefficientDivisor = 10;

    block->voicedFrameCount = 0;
    block->voiced = 0;
    block->previousVoiced = 0;
    block->queuedFrameCount = 0;
    block->beginVoiced = 0;
    block->endVoiced = 0;
    block->initializeNormEnergy = 1;
    block->flushing = 0;
    block->flushPitch = 1;
    block->flushNormEnergy = 1;
    block->pitchFilterReady = 0;

    block->voicedOutput = _tosGetProfileFloat(block, 2, 0.0f);
    block->unvoicedOutput = _tosGetProfileFloat(block, 3, 100000.0f);
    return 0;
}

void *ConstructPitchCombiner(TosContext *context, u32 blockIndex)
{
    return tosBaseBlockConstruct(
        context, blockIndex, 3, 1,
        ProcessPitchCombiner, InitPitchCombiner, ControlPitchCombiner,
        sizeof(PitchCombiner));
}
