#include <stddef.h>
#include <string.h>

#include "gssdk/mqueue.h"
#include "gssdk/smoother.h"

extern void *heap_Calloc(void *heap, u32 count, u32 size);
extern void heap_Free(void *heap, void *ptr);
extern TosQueue *qQueueInitEx(
    TosQueue *queue, u16 elementSize, u8 initializeReaders,
    u16 maxElements);
extern u8 qQueueControl(
    TosQueue *queue, u32 command, u16 elementSize, void *argument);
extern void qQueueReset(TosQueue *queue);
extern void qFreeUnusedElemements(TosQueue *queue);
extern f32 HLnOnePlusExpHFloat(f32 value);
extern f32 LogAdd(f32 value1, f32 value2);

typedef struct Acne {
    TosBaseBlock base;
    TosQueue *queue;
    u16 queueDepthLimit;
    s32 state;
    u8 drainingQueue;
    u8 smoothedMeanValid;
    f32 *smoothedMean;
    f32 *adaptiveMean;
    f32 *noiseEstimate;
    f32 inputReference;
    f32 accumulatedWeight;
    s32 zeroFrameCount;
    f32 cumulativeWeight;
    f32 smoothingProgress;
    u32 frameCount;
    s32 consecutiveZeroFrames;
    u32 bandCount;
    f32 smoothingThreshold;
    s32 maximumConsecutiveZeroFrames;
    f32 maximumAccumulatedWeight;
    f32 scalarFloor;
    f32 adaptationRatio;
    f32 noiseFloorOffset;
    f32 minimumBandDifference;
    /* Exact four-byte ABI reserve; semantic role unrecovered, never accessed. */
    u32 reserved80;
    f32 initialWeight;
    const f32 *noiseFloors;
    const f32 *initialMean;
    const Smoother1600Info *smootherInfo;
    Smoother *smoother;
} Acne;

typedef struct AcneSessionScalars {
    f32 inputReference;
    f32 accumulatedWeight;
    s32 zeroFrameCount;
    f32 cumulativeWeight;
    f32 smoothingProgress;
    u32 frameCount;
    s32 consecutiveZeroFrames;
} AcneSessionScalars;

typedef char AcneMinimumBandDifferenceOffset[
    (offsetof(Acne, minimumBandDifference) == 0x7C) ? 1 : -1];
typedef char AcneReserved80Offset[
    (offsetof(Acne, reserved80) == 0x80) ? 1 : -1];
typedef char AcneInitialWeightOffset[
    (offsetof(Acne, initialWeight) == 0x84) ? 1 : -1];
typedef char AcneSmootherOffset[
    (offsetof(Acne, smoother) == 0x94) ? 1 : -1];
typedef char AcneSize[(sizeof(Acne) == 0x98) ? 1 : -1];
typedef char AcneSessionScalarsSize[
    (sizeof(AcneSessionScalars) == 0x1C) ? 1 : -1];

static const f32 g_SpeechMeanInit11kHz[21] = {
    7.5637002f,  8.22809982f, 8.59599972f, 8.69009972f,
    8.71949959f, 8.73530006f, 8.74090004f, 8.7439003f,
    8.74450016f, 8.74419975f, 8.74380016f, 8.74300003f,
    8.74139977f, 8.73900032f, 8.73379993f, 8.72389984f,
    8.70829964f, 8.67770004f, 8.62969971f, 8.51099968f,
    8.2116003f,
};

static const f32 g_NoiseFloors11kHz[21] = {
    -1.67426932f, -2.18509817f, -2.52157259f, -2.77288866f,
    -2.87824988f, -2.87824988f, -2.87824988f, -2.87824988f,
    -2.87824988f, -2.87824988f, -2.87824988f, -2.87824988f,
    -2.87824988f, -2.87824988f, -2.87824988f, -2.87824988f,
    -2.87824988f, -2.87824988f, -2.87824988f, -2.98695707f,
    -3.05103707f,
};

static u8 Reset(Acne *block);
static void AcneGetSession(Acne *block, void *sessionData);
static void AcnePutSession(Acne *block, const void *sessionData);
static u8 NormalizeAndEnQ(Acne *block);
static void ProcessAcne(
    TosBaseBlock *baseBlock, void **inputs, s32 inputCount);
static u32 ControlAcne(
    TosBaseBlock *baseBlock, u32 command, void *argument, u32 argumentSize);
static u32 InitAcne(TosBaseBlock *baseBlock);

static u8 Reset(Acne *block)
{
    qQueueReset(block->queue);
    block->state = 2;
    block->drainingQueue = 0;
    if (block->initialWeight > 0.0f) {
        memcpy(
            block->adaptiveMean, block->initialMean,
            block->bandCount * sizeof(f32));
        memcpy(
            block->smoothedMean, block->initialMean,
            block->bandCount * sizeof(f32));
        if ((u8)Smoothing(block->smoother, block->smoothedMean)) {
            return 1;
        }
        block->smoothedMeanValid = 1;
    } else {
        memset(
            block->adaptiveMean, 0,
            block->bandCount * sizeof(f32));
        block->smoothedMeanValid = 0;
    }

    memset(
        block->noiseEstimate, 0,
        block->bandCount * sizeof(f32));
    block->inputReference = 100000.0f;
    block->accumulatedWeight = block->initialWeight;
    block->zeroFrameCount = 0;
    block->cumulativeWeight = 0.5f;
    block->smoothingProgress = 0.0f;
    block->frameCount = 0;
    block->consecutiveZeroFrames = 0;
    return 0;
}

static void AcneGetSession(Acne *block, void *sessionData)
{
    u8 *session = (u8 *)sessionData;
    u32 arraySize = block->bandCount * sizeof(f32);
    AcneSessionScalars *scalars;

    *(u32 *)(session + 0) = block->state;
    session[4] = block->drainingQueue;
    session[8] = block->smoothedMeanValid;
    session += 12;

    memcpy(session, block->smoothedMean, arraySize);
    session += arraySize;
    memcpy(session, block->adaptiveMean, arraySize);
    session += arraySize;
    memcpy(session, block->noiseEstimate, arraySize);
    session += arraySize;

    scalars = (AcneSessionScalars *)session;
    scalars->inputReference = block->inputReference;
    scalars->accumulatedWeight = block->accumulatedWeight;
    scalars->zeroFrameCount = block->zeroFrameCount;
    scalars->cumulativeWeight = block->cumulativeWeight;
    scalars->smoothingProgress = block->smoothingProgress;
    scalars->frameCount = block->frameCount;
    scalars->consecutiveZeroFrames = block->consecutiveZeroFrames;
}

static void AcnePutSession(Acne *block, const void *sessionData)
{
    const u8 *session = (const u8 *)sessionData;
    u32 arraySize = block->bandCount * sizeof(f32);
    const AcneSessionScalars *scalars;

    block->state = *(const u32 *)(session + 0);
    block->drainingQueue = session[4];
    block->smoothedMeanValid = session[8];
    session += 12;

    memcpy(block->smoothedMean, session, arraySize);
    session += arraySize;
    memcpy(block->adaptiveMean, session, arraySize);
    session += arraySize;
    memcpy(block->noiseEstimate, session, arraySize);
    session += arraySize;

    scalars = (const AcneSessionScalars *)session;
    block->inputReference = scalars->inputReference;
    block->accumulatedWeight = scalars->accumulatedWeight;
    block->zeroFrameCount = scalars->zeroFrameCount;
    block->cumulativeWeight = scalars->cumulativeWeight;
    block->smoothingProgress = scalars->smoothingProgress;
    block->frameCount = scalars->frameCount;
    block->consecutiveZeroFrames = scalars->consecutiveZeroFrames;
}

static u8 NormalizeAndEnQ(Acne *block)
{
    f32 *input;
    f32 *output;
    f32 *outputEnd;
    f32 *mean;
    const f32 *noiseFloor;
    f32 *outputElement;

    input = qDeQueueOne(block->queue, 0);
    if (input == NULL) {
        return 0;
    }

    if (block->smoothingProgress > block->smoothingThreshold) {
        block->smoothingProgress = 0.0f;
        memcpy(
            block->smoothedMean, block->adaptiveMean,
            block->bandCount * sizeof(f32));
        if ((u8)Smoothing(block->smoother, block->smoothedMean)) {
            return 0;
        }
        block->smoothedMeanValid = 1;
    }

    outputElement = qEnQueueOne(block->base.output[0].queue);
    output = outputElement + 1;
    if (block->smoothedMeanValid) {
        memcpy(output, input, block->bandCount * sizeof(f32));
        outputEnd = output + block->bandCount;
        mean = block->smoothedMean;
        noiseFloor = block->noiseFloors;

        while (output < outputEnd) {
            *output -= *mean++;
            if (*noiseFloor > *output) {
                *output = *noiseFloor +
                    HLnOnePlusExpHFloat(*output - *noiseFloor);
            } else {
                *output +=
                    HLnOnePlusExpHFloat(*noiseFloor - *output);
            }
            output++;
            noiseFloor++;
        }
    } else {
        memcpy(
            output, block->noiseFloors,
            block->bandCount * sizeof(f32));
    }

    outputElement[0] = LogAdd(
        input[block->bandCount] - block->inputReference,
        block->scalarFloor);
    qFreeUnusedElemements(block->queue);
    return 1;
}

static void ProcessAcne(
    TosBaseBlock *baseBlock, void **inputs, s32 inputCount)
{
    Acne *block = (Acne *)baseBlock;
    f32 *input;
    f32 *noiseInput;
    f32 *featureInput;
    f32 *queuedInput;
    f32 activity;
    f32 scalarInput;
    u32 arraySize;

    (void)inputCount;
    if (block->accumulatedWeight >= block->maximumAccumulatedWeight) {
        block->state = 0;
        block->drainingQueue = 1;
        while (qQueueNbrElements(block->queue) != 0) {
            NormalizeAndEnQ(block);
        }
    }

    if (block->drainingQueue &&
        qQueueNbrElements(block->queue) == 0) {
        block->drainingQueue = 0;
    }
    if (block->drainingQueue) {
        NormalizeAndEnQ(block);
        return;
    }

    input = (f32 *)inputs[0];
    if (input == NULL) {
        return;
    }

    noiseInput = (f32 *)inputs[1];
    activity = *(f32 *)inputs[2];
    featureInput = (f32 *)inputs[3];
    scalarInput = featureInput[0];
    block->inputReference = featureInput[1];
    arraySize = block->bandCount * sizeof(f32);
    memcpy(block->noiseEstimate, noiseInput, arraySize);

    queuedInput = qEnQueueOne(block->queue);
    memcpy(queuedInput, input, arraySize);
    queuedInput[block->bandCount] = scalarInput;
    block->frameCount++;

    if (activity > 0.001f) {
        f32 weight;
        f32 positiveRate;
        f32 negativeRate;
        f32 meanCorrection = 0.0f;
        f32 *mean = block->adaptiveMean;
        f32 *meanEnd = mean + block->bandCount;
        f32 *noise = block->noiseEstimate;
        f32 *queued = queuedInput;

        block->accumulatedWeight += activity;
        if (block->accumulatedWeight >=
            block->maximumAccumulatedWeight) {
            block->accumulatedWeight =
                block->maximumAccumulatedWeight;
        }

        weight = activity / block->accumulatedWeight;
        block->smoothingProgress += weight;
        positiveRate = weight * block->adaptationRatio;
        if (positiveRate > 1.0f) {
            positiveRate = 1.0f;
        }
        negativeRate = weight / block->adaptationRatio;
        if (negativeRate > 1.0f) {
            negativeRate = 1.0f;
        }

        while (mean < meanEnd) {
            f32 adjustedInput =
                *noise++ + block->noiseFloorOffset;
            f32 difference;
            f32 update;
            f32 residual;

            if (*queued > adjustedInput) {
                adjustedInput = *queued;
            }
            difference = adjustedInput - *mean;
            if (difference < block->minimumBandDifference) {
                difference = block->minimumBandDifference;
            }
            if (difference > 0.0f) {
                update = difference * positiveRate;
            } else {
                update = difference * negativeRate;
            }
            *mean += update;

            residual = adjustedInput - *mean;
            if (residual > 0.0f) {
                meanCorrection +=
                    residual * block->adaptationRatio;
            } else {
                meanCorrection +=
                    residual / block->adaptationRatio;
            }
            mean++;
            queued++;
        }

        block->cumulativeWeight += activity;
        if (block->cumulativeWeight > 200.0f) {
            block->cumulativeWeight = 200.0f;
        }
        weight = activity / block->cumulativeWeight;
        block->smoothingProgress += weight;
        meanCorrection *= weight / (f32)block->bandCount;
        if (meanCorrection > 0.7f) {
            meanCorrection = 0.7f;
        }
        if (meanCorrection < -0.7f) {
            meanCorrection = -0.7f;
        }

        mean = block->adaptiveMean;
        while (mean < meanEnd) {
            *mean++ += meanCorrection;
        }
    }

    if (activity == 0.0f && block->frameCount != 0) {
        block->consecutiveZeroFrames++;
        block->zeroFrameCount++;
        if (block->zeroFrameCount > 50) {
            block->zeroFrameCount = 50;
        }
    } else {
        block->consecutiveZeroFrames = 0;
    }

    switch (block->state) {
    case 0:
        NormalizeAndEnQ(block);
        break;
    case 1:
        if (block->consecutiveZeroFrames >
            block->maximumConsecutiveZeroFrames) {
            block->state = 2;
            while (qQueueNbrElements(block->queue) != 0) {
                NormalizeAndEnQ(block);
            }
        } else if (qQueueNbrElements(block->queue) ==
            block->queueDepthLimit) {
            NormalizeAndEnQ(block);
        }
        break;
    case 2:
        if (activity > 0.001f) {
            block->state = 1;
        } else {
            NormalizeAndEnQ(block);
        }
        break;
    }
}

static u32 ControlAcne(
    TosBaseBlock *baseBlock, u32 command, void *argument, u32 argumentSize)
{
    Acne *block = (Acne *)baseBlock;
    u32 *sessionOffset = (u32 *)argumentSize;
    u32 sessionSize =
        (block->bandCount * 3 + 10) * sizeof(u32);

    switch ((u8)command) {
    case 2:
        block->drainingQueue = 1;
        return 1;

    case 3:
        if (argument != NULL) {
            AcneGetSession(
                block, (u8 *)argument + *sessionOffset);
        }
        *sessionOffset += sessionSize;
        return 1;

    case 4:
        if (argument != NULL) {
            AcnePutSession(
                block, (u8 *)argument + *sessionOffset);
            *sessionOffset += sessionSize;
        } else if (Reset(block)) {
            return 0;
        }
        return 1;

    case 0xFF: {
        TosContext *context = block->base.context;

        qQueueControl(block->queue, 0xFF, 0, NULL);
        heap_Free(context->heap, block->adaptiveMean);
        smtDestruct(block->smoother, context);
        tosBaseBlockDestruct(block);
        return 1;
    }
    }

    return 0;
}

static u32 InitAcne(TosBaseBlock *baseBlock)
{
    Acne *block = (Acne *)baseBlock;
    TosContext *context = baseBlock->context;
    u16 bandCount;
    u32 arraySize;
    u32 queueElementSize;

    bandCount = (u16)_tosGetProfileU32(block, 9, 21);
    block->bandCount = bandCount;
    arraySize = bandCount * sizeof(f32);
    queueElementSize = (bandCount + 1) * sizeof(f32);
    baseBlock->input[0].inputSize = (u16)arraySize;
    baseBlock->input[1].inputSize = (u16)arraySize;
    baseBlock->input[2].inputSize = sizeof(f32);
    baseBlock->input[3].inputSize = 0x18;
    baseBlock->output[0].outputSize = (u16)queueElementSize;

    block->queueDepthLimit =
        (u16)_tosGetProfileU32(block, 2, 100);
    block->queue = qQueueConstruct(context, 0xFF, 1);
    if (block->queue == NULL) {
        return 1;
    }
    if (qQueueInitEx(
            block->queue, (u16)queueElementSize, 1, 0) == NULL) {
        return 1;
    }

    block->adaptiveMean =
        heap_Calloc(context->heap, bandCount * 3, sizeof(f32));
    block->noiseEstimate = block->adaptiveMean + bandCount;
    block->smoothedMean = block->noiseEstimate + bandCount;

    block->noiseFloorOffset =
        0.11513f * _tosGetProfileFloat(block, 3, -1000.0f);
    block->minimumBandDifference =
        0.11513f * _tosGetProfileFloat(block, 4, -1000.0f);
    block->adaptationRatio = 4.0f;
    block->maximumAccumulatedWeight =
        _tosGetProfileFloat(block, 5, 50.0f);
    block->smoothingThreshold =
        10.0f / block->maximumAccumulatedWeight;
    block->maximumConsecutiveZeroFrames =
        (s16)_tosGetProfileU32(block, 6, 15);
    block->scalarFloor =
        0.23026f * _tosGetProfileFloat(block, 7, -30.0f);

    block->initialWeight = 5.0f;
    block->noiseFloors = g_NoiseFloors11kHz;
    block->initialMean = g_SpeechMeanInit11kHz;
    block->smootherInfo = &Smoother1600Info11kHz;
    block->smoother = smtConstruct(
        context, bandCount,
        _tosGetProfileFloat(block, 8, 2.6f), 0.0f,
        block->smootherInfo);

    if (Reset(block)) {
        return 1;
    }
    return 0;
}

TosBaseBlock *ConstructAcne(TosContext *context, u32 blockIndex)
{
    return tosBaseBlockConstruct(
        context, blockIndex, 4, 1, ProcessAcne, InitAcne,
        ControlAcne, sizeof(Acne));
}
