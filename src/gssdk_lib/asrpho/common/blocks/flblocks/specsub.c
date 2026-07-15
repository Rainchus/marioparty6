#include "types.h"

#include <math.h>
#include <stddef.h>
#include <string.h>

#include "gssdk/mqueue.h"

extern void *heap_Calloc(void *heap, u32 count, u32 size);
extern void heap_Free(void *heap, void *ptr);
extern u8 qQueueControl(
    TosQueue *queue, u32 command, u16 elementSize, void *argument);
extern f32 logf_check(f32 value);

typedef struct SpecSub {
    TosBaseBlock base;
    u16 frameCount;
    u16 noiseUpdateCount;
    f32 *noisePower;
    f32 *noiseLogAmplitude;
    f32 *priorSnr;
    f32 *posteriorSnr;
    f32 *history;
    f32 *historyEnd;
    f32 *historyWrite;
    f32 *sessionHistoryCursor;
    u16 historyLength;
    f32 *gain;
    u32 spectrumSize;
    u16 outputStart;
    f32 noiseSmoothing;
    f32 noiseUpdateLimit;
    /* Exact four-byte ABI reserve; semantic role unrecovered, never accessed. */
    u32 reserved64;
    f32 noiseScale;
    f32 gainFloor;
    u32 segmentCount;
    u32 *segmentEnds;
    f32 exponentBias;
    f32 exponentScale;
    f32 normalizationState;
    f32 exponentClamp;
    f32 denominatorConstant;
    f32 numeratorConstant;
    f32 denominatorFeedback;
    f32 numeratorFeedback;
} SpecSub;

typedef char SpecSubHistoryOffset[
    (offsetof(SpecSub, history) == 0x3C) ? 1 : -1];
typedef char SpecSubReserved64Offset[
    (offsetof(SpecSub, reserved64) == 0x64) ? 1 : -1];
typedef char SpecSubNoiseScaleOffset[
    (offsetof(SpecSub, noiseScale) == 0x68) ? 1 : -1];
typedef char SpecSubSegmentEndsOffset[
    (offsetof(SpecSub, segmentEnds) == 0x74) ? 1 : -1];
typedef char SpecSubNormalizationStateOffset[
    (offsetof(SpecSub, normalizationState) == 0x80) ? 1 : -1];
typedef char SpecSubNumeratorFeedbackOffset[
    (offsetof(SpecSub, numeratorFeedback) == 0x94) ? 1 : -1];
typedef char SpecSubSize[(sizeof(SpecSub) == 0x98) ? 1 : -1];

static void Reset(SpecSub *block);
static void SpecSubPutSession(SpecSub *block, const void *sessionData);
static void SpecSubGetSession(SpecSub *block, void *sessionData);
static void SpectralSubtraction(
    SpecSub *block, u8 inhibitNoiseUpdate, const f32 *input,
    f32 *outputSpectrum, f32 *outputEnergy);
static void ProcessSpecSub(
    TosBaseBlock *baseBlock, void **inputs, s32 inputCount);
static u32 ControlSpecSub(
    TosBaseBlock *baseBlock, u32 command, void *argument, u32 argumentSize);
static u32 InitSpecSub(TosBaseBlock *baseBlock);

static void Reset(SpecSub *block)
{
    f32 *value;
    f32 *end;

    value = block->posteriorSnr;
    end = value + block->spectrumSize;
    while (value < end) {
        *value++ = 1.0f;
    }

    value = block->priorSnr;
    end = value + block->spectrumSize;
    while (value < end) {
        *value++ = 1.0f;
    }

    block->frameCount = 0;
    block->normalizationState = 1.0f;
}

static void SpecSubPutSession(SpecSub *block, const void *sessionData)
{
    const u8 *session = (const u8 *)sessionData;
    u32 spectrumSize = block->spectrumSize;
    u32 arraySize = spectrumSize * sizeof(f32);
    u32 i;

    block->frameCount = *(const u32 *)(session + 0);
    block->noiseUpdateCount = *(const u32 *)(session + 4);
    block->normalizationState = *(const f32 *)(session + 8);
    session += 12;

    memcpy(block->noisePower, session, arraySize);
    session += arraySize;
    memcpy(block->priorSnr, session, arraySize);
    session += arraySize;
    memcpy(block->posteriorSnr, session, arraySize);
    session += arraySize;
    memcpy(block->gain, session, arraySize);
    session += arraySize;

    for (i = 0; i < spectrumSize; i++) {
        block->noiseLogAmplitude[i] =
            0.5f * logf_check(block->noisePower[i]);
    }

    block->historyWrite = block->history + *(const u32 *)(session + 0);
    block->sessionHistoryCursor =
        block->history + *(const u32 *)(session + 4);
    session += 8;
    memcpy(
        block->history, session,
        spectrumSize * (block->historyLength + 1) * sizeof(f32));
}

static void SpecSubGetSession(SpecSub *block, void *sessionData)
{
    u8 *session = (u8 *)sessionData;
    u32 spectrumSize = block->spectrumSize;
    u32 arraySize = spectrumSize * sizeof(f32);

    *(u32 *)(session + 0) = block->frameCount;
    *(u32 *)(session + 4) = block->noiseUpdateCount;
    *(f32 *)(session + 8) = block->normalizationState;
    session += 12;

    memcpy(session, block->noisePower, arraySize);
    session += arraySize;
    memcpy(session, block->priorSnr, arraySize);
    session += arraySize;
    memcpy(session, block->posteriorSnr, arraySize);
    session += arraySize;
    memcpy(session, block->gain, arraySize);
    session += arraySize;

    *(u32 *)(session + 0) = block->historyWrite - block->history;
    *(u32 *)(session + 4) = block->sessionHistoryCursor - block->history;
    session += 8;
    memcpy(
        session, block->history,
        spectrumSize * (block->historyLength + 1) * sizeof(f32));
}

static void SpectralSubtraction(
    SpecSub *block, u8 inhibitNoiseUpdate, const f32 *input,
    f32 *outputSpectrum, f32 *outputEnergy)
{
    u32 spectrumSize = block->spectrumSize;
    u32 arraySize = spectrumSize * sizeof(f32);
    f32 *historyWrite = block->historyWrite;
    u16 historyLength = block->historyLength;
    u16 noiseUpdateCount = block->noiseUpdateCount;
    u16 frameCount = block->frameCount;
    f32 smoothing = block->noiseSmoothing;
    f32 *noisePower = block->noisePower;
    f32 maximumAverage;
    f32 exponent;
    f32 total;
    f32 value;
    f32 difference;
    f32 alpha;
    f32 *cursor;
    f32 *end;
    u32 count;
    u32 i;
    u32 segment;

    memcpy(historyWrite, input, arraySize);
    historyWrite += spectrumSize;
    if (historyWrite == block->historyEnd) {
        historyWrite = block->history;
    }

    if (inhibitNoiseUpdate != 0) {
        frameCount = 0;
    } else {
        if (noiseUpdateCount == 0) {
            for (i = 0; i < spectrumSize; i++) {
                noisePower[i] = input[i];
                block->noiseLogAmplitude[i] =
                    0.5f * logf_check(input[i]);
            }
        }

        if (frameCount <= historyLength) {
            frameCount++;
        }

        if (frameCount == historyLength + 1) {
            if ((f32)noiseUpdateCount >= block->noiseUpdateLimit) {
                alpha = 1.0f / block->noiseUpdateLimit;
            } else {
                noiseUpdateCount++;
                alpha = 1.0f / (f32)noiseUpdateCount;
            }

            for (i = 0; i < spectrumSize; i++) {
                difference = historyWrite[i] - noisePower[i];
                if (difference > noisePower[i]) {
                    difference = noisePower[i];
                }
                noisePower[i] += alpha * difference;
                block->noiseLogAmplitude[i] =
                    0.5f * logf_check(noisePower[i]);
            }
        }
    }

    if (noiseUpdateCount != 0) {
        for (i = 0; i < spectrumSize; i++) {
            block->priorSnr[i] = smoothing *
                (1.0f + block->posteriorSnr[i] * block->gain[i]);
        }

        for (i = 0; i < spectrumSize; i++) {
            block->posteriorSnr[i] =
                (input[i] + 0.000001f) /
                (block->noiseScale * noisePower[i] + 0.000001f);
        }

        for (i = 0; i < spectrumSize; i++) {
            block->priorSnr[i] +=
                (1.0f - smoothing) * block->posteriorSnr[i];
        }
    }

    maximumAverage = 0.0f;
    cursor = block->posteriorSnr;
    for (segment = 0; segment < block->segmentCount; segment++) {
        end = block->posteriorSnr + block->segmentEnds[segment];
        total = 0.0f;
        count = 0;
        while (cursor < end) {
            total += *cursor++;
            count++;
        }
        value = total / (f32)count;
        if (value > maximumAverage) {
            maximumAverage = value;
        }
    }

    exponent = block->exponentBias +
               maximumAverage * block->exponentScale;
    exponent = exponent <= block->exponentClamp
                   ? exponent
                   : block->exponentClamp;
    value = expf(exponent);

    block->normalizationState =
        value *
        ((block->numeratorConstant +
          block->numeratorFeedback * block->normalizationState) /
         (block->denominatorConstant +
          block->denominatorFeedback * block->normalizationState));

    if (noiseUpdateCount == 0) {
        for (i = 0; i < spectrumSize; i++) {
            block->gain[i] = 1.0f;
        }
    } else {
        for (i = 0; i < spectrumSize; i++) {
            if (block->priorSnr[i] <= 1.0f) {
                value = 0.0f;
            } else {
                value = 1.0f - 1.0f / block->priorSnr[i];
            }
            value *= value;
            if (value <= block->gainFloor) {
                value = block->gainFloor;
            }
            block->gain[i] = value;
        }
    }

    for (i = 0; i < block->outputStart; i++) {
        value = block->gain[i] * input[i];
        outputSpectrum[i] = 0.5f * logf_check(value);
    }

    total = 0.0f;
    for (; i < spectrumSize; i++) {
        value = block->gain[i] * input[i];
        total += value;
        outputSpectrum[i] = 0.5f * logf_check(value);
    }
    *outputEnergy = logf_check(total);

    block->historyLength = historyLength;
    block->noiseUpdateCount = noiseUpdateCount;
    block->frameCount = frameCount;
    block->historyWrite = historyWrite;
}

static void ProcessSpecSub(
    TosBaseBlock *baseBlock, void **inputs, s32 inputCount)
{
    SpecSub *block = (SpecSub *)baseBlock;
    f32 *outputEnergy;
    f32 *outputSpectrum;
    f32 *outputNoise;
    f32 *outputNormalization;

    if (inputs[0] == NULL) {
        return;
    }

    outputEnergy = qEnQueueOne(baseBlock->output[0].queue);
    outputSpectrum = qEnQueueOne(baseBlock->output[1].queue);
    outputNoise = qEnQueueOne(baseBlock->output[2].queue);
    outputNormalization = qEnQueueOne(baseBlock->output[3].queue);

    SpectralSubtraction(
        block, *(u8 *)inputs[1], inputs[0], outputSpectrum, outputEnergy);
    memcpy(
        outputNoise, block->noiseLogAmplitude,
        block->spectrumSize * sizeof(f32));
    *outputNormalization = logf_check(block->normalizationState);
}

static u32 ControlSpecSub(
    TosBaseBlock *baseBlock, u32 command, void *argument, u32 argumentSize)
{
    SpecSub *block = (SpecSub *)baseBlock;
    u32 *sessionOffset = (u32 *)argumentSize;
    u32 sessionSize;
    f32 *value;
    f32 *end;
    u32 result = 1;

    switch ((u8)command) {
    case 1:
        if (argument != NULL) {
            SpecSubGetSession(
                block, (u8 *)argument + *sessionOffset);
        }
        sessionSize =
            (((block->historyLength + 5) * block->spectrumSize) + 5) *
            sizeof(u32);
        *sessionOffset += sessionSize;
        break;
    case 2:
        if (argument != NULL) {
            SpecSubPutSession(
                block, (const u8 *)argument + *sessionOffset);
            sessionSize =
                (((block->historyLength + 5) * block->spectrumSize) + 5) *
                sizeof(u32);
            *sessionOffset += sessionSize;
        } else {
            value = block->noisePower;
            end = value + block->spectrumSize * 2;
            while (value < end) {
                *value++ = 1.0f;
            }
            block->noiseUpdateCount = 0;
            Reset(block);
        }
        break;
    case 3:
        result = qQueueControl(
            baseBlock->input[1].queue, 5,
            baseBlock->input[1].outputSize, NULL);
        break;
    case 4:
        result = qQueueControl(
            baseBlock->input[1].queue, 6,
            baseBlock->input[1].outputSize, NULL);
        break;
    case 255:
        heap_Free(baseBlock->context->heap, block->history);
        heap_Free(baseBlock->context->heap, block->segmentEnds);
        tosBaseBlockDestruct(block);
        break;
    default:
        return 0;
    }
    return result;
}

static u32 InitSpecSub(TosBaseBlock *baseBlock)
{
    SpecSub *block = (SpecSub *)baseBlock;
    TosContext *context = baseBlock->context;
    u32 spectrumSize;
    u32 arraySize;
    u32 endpoint;
    u32 segment;
    f32 ratio1;
    f32 ratio2;
    f32 maximumRatio;
    f32 *value;
    f32 *end;

    spectrumSize = _tosGetProfileU32(block, 8, 21);
    block->spectrumSize = spectrumSize;
    arraySize = spectrumSize * sizeof(f32);

    baseBlock->input[0].inputSize = arraySize;
    baseBlock->input[1].inputSize = 1;
    baseBlock->output[0].outputSize = sizeof(f32);
    baseBlock->output[1].outputSize = arraySize;
    baseBlock->output[2].outputSize = arraySize;
    baseBlock->output[3].outputSize = sizeof(f32);

    block->outputStart =
        (u16)_tosGetProfileU32(block, 6, 1) - 1;
    block->historyLength = _tosGetProfileU32(block, 1, 3);
    block->gainFloor = _tosGetProfileFloat(block, 2, 0.01f);
    block->noiseScale = _tosGetProfileFloat(block, 3, 1.0f);
    block->noiseUpdateLimit =
        _tosGetProfileFloat(block, 4, 40.0f);
    block->noiseSmoothing =
        _tosGetProfileFloat(block, 7, 0.99f);

    block->history = heap_Calloc(
        context->heap,
        spectrumSize * (block->historyLength + 6), sizeof(f32));
    block->historyWrite = block->history;
    block->historyEnd =
        block->history + spectrumSize * (block->historyLength + 1);
    block->noisePower = block->historyEnd;
    block->noiseLogAmplitude = block->noisePower + spectrumSize;
    block->priorSnr = block->noiseLogAmplitude + spectrumSize;
    block->posteriorSnr = block->priorSnr + spectrumSize;
    block->gain = block->posteriorSnr + spectrumSize;

    block->denominatorConstant = 0.8f;
    block->numeratorConstant = 0.2f;
    block->denominatorFeedback = 0.1f;
    block->numeratorFeedback = 0.9f;

    ratio1 = block->numeratorFeedback / block->denominatorFeedback;
    ratio2 = block->numeratorConstant / block->denominatorConstant;
    maximumRatio = ratio1 > ratio2 ? ratio1 : ratio2;
    block->exponentClamp = logf_check(3.4028e38f / maximumRatio);
    block->normalizationState = 1.0f;
    block->exponentBias = -logf_check(11.0f);
    block->exponentScale = 0.90909094f;

    block->segmentCount =
        (u16)_tosGetProfileU32(block, 30, 1);
    block->segmentEnds = heap_Calloc(
        context->heap, block->segmentCount, sizeof(u32));

    for (segment = 0; segment < block->segmentCount; segment++) {
        switch (segment) {
        case 0:
            endpoint =
                (u16)_tosGetProfileU32(block, 31, spectrumSize);
            break;
        case 1:
            endpoint =
                (u16)_tosGetProfileU32(block, 32, spectrumSize);
            break;
        case 2:
            endpoint =
                (u16)_tosGetProfileU32(block, 33, spectrumSize);
            break;
        default:
            _tosErrorLog(block, 0x67);
            return 1;
        }
        if (endpoint > spectrumSize) {
            endpoint = spectrumSize;
        }
        block->segmentEnds[segment] = endpoint;
    }

    if (block->history == NULL) {
        return _tosErrorLog(block, 0x66);
    }

    value = block->noisePower;
    end = value + spectrumSize * 2;
    while (value < end) {
        *value++ = 1.0f;
    }
    block->noiseUpdateCount = 0;
    Reset(block);
    return 0;
}

void *ConstructSpecSub(TosContext *context, u32 blockIndex)
{
    return tosBaseBlockConstruct(
        context, blockIndex, 2, 4, ProcessSpecSub, InitSpecSub,
        ControlSpecSub, sizeof(SpecSub));
}
