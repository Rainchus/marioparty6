#include "gssdk/tos.h"

typedef struct SHS_VUV {
    TosBaseBlock base;
    u16 fftLength;
    u16 sampleRate;
    u16 minimumFrequency;
    u16 logBinsPerOctave;
    u8 octaveCount;
    u8 harmonicCount;
    f32 harmonicDecay;
    u32 voicingThreshold;
    u16 logSpectrumLength;
    f32 *peakSpectrum;
    f32 *linearSpectrum;
    f32 **splineWork;
    f32 *splineDerivatives;
    f32 *logSpectrum;
    f32 *weightedLogSpectrum;
    f32 *frequencyWeights;
    u32 *harmonicOffsets;
} SHS_VUV;

enum {
    SPLINE_DIFFERENCE,
    SPLINE_DIAGONAL,
    SPLINE_FACTOR,
    SPLINE_RIGHT_HAND_SIDE,
    SPLINE_FORWARD_SOLUTION,
    SPLINE_WORK_COUNT
};

extern void *heap_Calloc(void *heap, u32 count, u32 size);
extern void heap_Free(void *heap, void *ptr);
extern f32 logf_check(f32 value);
extern f32 powf(f32 base, f32 exponent);
extern f32 atanf(f32 value);

static void PeakEnhancement(SHS_VUV *block, void **inputs)
{
    f32 *input;
    f32 *output = block->peakSpectrum;
    f32 *outputPosition;
    f32 *outputEnd;
    f32 *inputPosition;
    f32 *inputEnd;
    s32 halfLength = block->fftLength / 2;
    s32 firstBin;

    outputPosition = output;
    outputEnd = output + halfLength + 1;
    while (outputPosition < outputEnd) {
        *outputPosition++ = 0.0f;
    }

    input = (f32 *)inputs[1];
    firstBin = (s32)((f32)block->minimumFrequency /
                     ((f32)block->sampleRate / (f32)block->fftLength));
    inputPosition = input + firstBin;
    inputEnd = input + halfLength - 1;
    for (;;) {
        if (inputPosition[-1] <= inputPosition[0] &&
            inputPosition[1] <= inputPosition[0]) {
            outputPosition =
                output + (inputPosition - input) - 2;
            outputPosition[0] = inputPosition[-2];
            outputPosition[1] = inputPosition[-1];
            outputPosition[2] = inputPosition[0];
            outputPosition[3] = inputPosition[1];
            if (inputPosition != inputEnd) {
                outputPosition[4] = inputPosition[2];
            }
        }
        if (inputPosition == inputEnd) {
            break;
        }
        inputPosition++;
    }
}

static void spline2(f32 *input, f32 *derivative, SHS_VUV *block)
{
    f32 *difference = block->splineWork[SPLINE_DIFFERENCE];
    f32 *diagonal = block->splineWork[SPLINE_DIAGONAL];
    f32 *factor = block->splineWork[SPLINE_FACTOR];
    f32 *rightHandSide = block->splineWork[SPLINE_RIGHT_HAND_SIDE];
    f32 *forward = block->splineWork[SPLINE_FORWARD_SOLUTION];
    s32 pointCount = block->fftLength / 2 + 1;
    f32 *inputPosition = input;
    f32 *inputNext = input + 1;
    f32 *inputEnd = input + pointCount;
    f32 *differencePosition = difference + 1;
    f32 *diagonalPosition;
    f32 *diagonalPrevious;
    f32 *factorPosition;
    f32 *rightHandSidePosition;
    f32 *forwardPosition;
    f32 *forwardPrevious;
    f32 *derivativePosition;

    while (inputNext < inputEnd) {
        *differencePosition++ = *inputNext++ - *inputPosition++;
    }

    diagonal[0] = 2.0f;
    diagonalPrevious = diagonal;
    diagonalPosition = diagonal + 1;
    factorPosition = factor + 1;
    while (diagonalPosition < diagonal + pointCount - 1) {
        *factorPosition = (f32)(1.0 / *diagonalPrevious++);
        *diagonalPosition++ = 4.0f - *factorPosition++;
    }
    *factorPosition = (f32)(1.0 / *diagonalPrevious);
    *diagonalPosition = 2.0f - *diagonalPrevious;

    rightHandSide[0] = 3.0f * difference[1];
    differencePosition = difference + 1;
    rightHandSidePosition = rightHandSide + 1;
    while (rightHandSidePosition < rightHandSide + pointCount - 1) {
        *rightHandSidePosition++ =
            3.0f * (*differencePosition + differencePosition[1]);
        differencePosition++;
    }
    *rightHandSidePosition = 3.0f * *differencePosition;

    forward[0] = rightHandSide[0];
    factorPosition = factor + 1;
    rightHandSidePosition = rightHandSide + 1;
    forwardPrevious = forward;
    forwardPosition = forward + 1;
    while (forwardPosition < forward + pointCount) {
        *forwardPosition++ =
            *rightHandSidePosition++ -
            *factorPosition++ * *forwardPrevious++;
    }

    diagonalPosition = diagonal + pointCount - 1;
    forwardPosition = forward + pointCount - 1;
    derivativePosition = derivative + pointCount - 1;
    *derivativePosition = *forwardPosition / *diagonalPosition;
    while (derivativePosition > derivative) {
        diagonalPosition--;
        forwardPosition--;
        derivativePosition--;
        *derivativePosition =
            (*forwardPosition - derivativePosition[1]) /
            *diagonalPosition;
    }
}

static void Lin_2_Log(SHS_VUV *block)
{
    f32 halfLength = (f32)(block->fftLength * 0.5);
    f32 binScale;
    f32 frequency;
    f32 frequencyStep;
    s32 octave;
    s32 bin;

    spline2(block->linearSpectrum, block->splineDerivatives, block);
    frequencyStep =
        powf(2.0f, (f32)(1.0 / block->logBinsPerOctave));
    binScale = (f32)block->fftLength / (f32)block->sampleRate;
    frequency = (f32)block->minimumFrequency;

    for (octave = 0; octave < block->octaveCount; octave++) {
        for (bin = 0; bin < block->logBinsPerOctave; bin++) {
            f32 linearBin = frequency * binScale;
            f32 value;

            if (linearBin < halfLength) {
                s32 index = (s32)linearBin;
                f32 fraction = linearBin - (f32)index;
                f32 inverseFraction = 1.0f - fraction;
                f32 lowerValue = block->linearSpectrum[index];
                f32 upperValue = block->linearSpectrum[index + 1];
                f32 difference = upperValue - lowerValue;
                f32 lowerCorrection =
                    block->splineDerivatives[index] - difference;
                f32 upperCorrection =
                    block->splineDerivatives[index + 1] - difference;

                value =
                    fraction * upperValue + inverseFraction * lowerValue;
                value += fraction * inverseFraction *
                         (inverseFraction * lowerCorrection -
                          fraction * upperCorrection);
            } else {
                value = 0.0f;
            }

            block->logSpectrum[
                octave * block->logBinsPerOctave + bin] = value;
            frequency *= frequencyStep;
        }
    }
}

static void SubHarmonicSummation_and_VUVDetermination(
    SHS_VUV *block, void **inputs)
{
    f32 attenuation = block->harmonicDecay;
    f32 harmonicScale = attenuation;
    f32 *sum = qEnQueueOne(block->base.output[0].queue);
    f32 *sumEnd = sum + block->logSpectrumLength;
    u32 *offset = block->harmonicOffsets;
    u32 *offsetEnd = offset + block->harmonicCount;
    f32 maximum;
    f32 ratio;
    u8 *voiced;

    {
        f32 *destination = sum;
        f32 *source = block->weightedLogSpectrum;

        while (destination < sumEnd) {
            *destination++ = *source++;
        }
    }

    while (offset < offsetEnd) {
        f32 *destination = sum;
        f32 *destinationEnd = sumEnd - *offset;
        f32 *source = block->weightedLogSpectrum + *offset;

        while (destination < destinationEnd) {
            *destination += harmonicScale * *source;
            destination++;
            source++;
        }
        offset++;
        harmonicScale *= attenuation;
    }

    voiced = qEnQueueOne(block->base.output[1].queue);
    maximum = sum[0];
    {
        f32 *value = sum + 1;

        while (value < sumEnd) {
            if (*value > maximum) {
                maximum = *value;
            }
            value++;
        }
    }

    ratio = maximum / ((f32 *)inputs[0])[1];
    if (ratio < (f32)block->voicingThreshold) {
        *voiced = 0;
    } else {
        *voiced = 1;
    }
}

static void ProcessSHS_VUV(
    TosBaseBlock *baseBlock, void **inputs, s32 inputCount)
{
    SHS_VUV *block = (SHS_VUV *)baseBlock;

    if (inputs[0] != NULL) {
        s32 halfLength;
        f32 *input;
        f32 *output;
        f32 *outputEnd;
        f32 *logValue;
        f32 *weight;

        PeakEnhancement(block, inputs);

        halfLength = block->fftLength / 2;
        input = block->peakSpectrum + 1;
        output = block->linearSpectrum;
        outputEnd = output + halfLength;
        *output++ = 0.0f;
        while (output < outputEnd) {
            *output++ =
                (f32)(0.25 *
                      (input[-1] + input[0] + input[0] + input[1]));
            input++;
        }
        *output = (f32)(0.5 * (input[-1] + input[0]));

        Lin_2_Log(block);
        logValue = block->logSpectrum;
        weight = block->frequencyWeights;
        output = block->weightedLogSpectrum;
        outputEnd = output + block->logSpectrumLength;
        while (output < outputEnd) {
            if (*logValue > 0.0f) {
                *output = *logValue * *weight;
            } else {
                *output = 0.0f;
            }
            logValue++;
            weight++;
            output++;
        }

        SubHarmonicSummation_and_VUVDetermination(block, inputs);
    }
}

static u32 ControlSHS_VUV(
    TosBaseBlock *baseBlock, u32 command, void *argument, u32 argumentSize)
{
    SHS_VUV *block = (SHS_VUV *)baseBlock;

    switch ((u8)command) {
    case 255: {
        TosContext *context = block->base.context;

        heap_Free(context->heap, block->peakSpectrum);
        heap_Free(context->heap, block->linearSpectrum);
        heap_Free(context->heap, block->splineWork[SPLINE_DIFFERENCE]);
        heap_Free(context->heap, block->splineWork[SPLINE_DIAGONAL]);
        heap_Free(context->heap, block->splineWork[SPLINE_FACTOR]);
        heap_Free(context->heap, block->splineWork[SPLINE_RIGHT_HAND_SIDE]);
        heap_Free(context->heap, block->splineWork[SPLINE_FORWARD_SOLUTION]);
        heap_Free(context->heap, block->splineWork);
        heap_Free(context->heap, block->splineDerivatives);
        heap_Free(context->heap, block->logSpectrum);
        heap_Free(context->heap, block->weightedLogSpectrum);
        heap_Free(context->heap, block->frequencyWeights);
        heap_Free(context->heap, block->harmonicOffsets);
        tosBaseBlockDestruct(block);
        break;
    }
    default:
        return 0;
    }
    return 1;
}

static u32 InitSHS_VUV(TosBaseBlock *baseBlock)
{
    SHS_VUV *block = (SHS_VUV *)baseBlock;
    TosContext *context = block->base.context;
    u16 linearBinCount;
    f32 minimumFrequencyOffset;
    s32 i;

    linearBinCount = (u16)_tosGetProfileU32(block, 1, 65);
    block->fftLength = (linearBinCount - 1) * 2;
    block->sampleRate = (u16)_tosGetProfileU32(block, 2, 2200);
    block->minimumFrequency =
        (u16)_tosGetProfileU32(block, 3, 50);
    block->logBinsPerOctave =
        (u16)_tosGetProfileU32(block, 4, 50);
    block->octaveCount = (u8)_tosGetProfileU32(block, 5, 5);
    block->logSpectrumLength =
        block->octaveCount * block->logBinsPerOctave;
    block->voicingThreshold =
        _tosGetProfileU32(block, 8, 5000);

    block->base.input[0].inputSize = 6 * sizeof(f32);
    block->base.input[1].inputSize = linearBinCount * sizeof(f32);
    block->base.output[0].outputSize =
        block->logSpectrumLength * sizeof(f32);
    block->base.output[1].outputSize = sizeof(u8);

    block->peakSpectrum =
        heap_Calloc(context->heap, linearBinCount, sizeof(f32));
    block->linearSpectrum =
        heap_Calloc(context->heap, linearBinCount, sizeof(f32));
    block->splineWork =
        heap_Calloc(context->heap, SPLINE_WORK_COUNT, sizeof(f32 *));
    if (block->peakSpectrum == NULL ||
        block->linearSpectrum == NULL ||
        block->splineWork == NULL) {
        _tosErrorLog(block, 2);
        return 1;
    }

    block->splineWork[SPLINE_DIFFERENCE] =
        heap_Calloc(context->heap, linearBinCount, sizeof(f32));
    block->splineWork[SPLINE_DIAGONAL] =
        heap_Calloc(context->heap, linearBinCount, sizeof(f32));
    block->splineWork[SPLINE_FACTOR] =
        heap_Calloc(context->heap, linearBinCount, sizeof(f32));
    block->splineWork[SPLINE_RIGHT_HAND_SIDE] =
        heap_Calloc(context->heap, linearBinCount, sizeof(f32));
    block->splineWork[SPLINE_FORWARD_SOLUTION] =
        heap_Calloc(context->heap, linearBinCount, sizeof(f32));
    block->splineDerivatives =
        heap_Calloc(context->heap, linearBinCount, sizeof(f32));
    block->logSpectrum = heap_Calloc(
        context->heap, block->logSpectrumLength, sizeof(f32));
    block->weightedLogSpectrum = heap_Calloc(
        context->heap, block->logSpectrumLength, sizeof(f32));
    block->frequencyWeights = heap_Calloc(
        context->heap, block->logSpectrumLength, sizeof(f32));

    minimumFrequencyOffset =
        (f32)block->logBinsPerOctave *
        logf_check((f32)(65.0 / block->minimumFrequency)) /
        0.6931472f;
    for (i = 0; i < block->logSpectrumLength; i++) {
        f32 angle =
            (f32)(3.0 * ((f32)i - minimumFrequencyOffset) /
                  block->logBinsPerOctave);

        block->frequencyWeights[i] =
            (f32)(0.5 + atanf(angle) / 3.141592653589793);
        if (block->frequencyWeights[i] < 0.0f) {
            block->frequencyWeights[i] = 0.0f;
        }
    }

    block->harmonicCount = (u8)_tosGetProfileU32(block, 6, 15);
    block->harmonicOffsets =
        heap_Calloc(context->heap, block->harmonicCount, sizeof(u32));
    for (i = 0; i < block->harmonicCount; i++) {
        f32 harmonicLog = logf_check((f32)(2.0 + i));

        block->harmonicOffsets[i] =
            (u32)(0.5 +
                  (f32)block->logBinsPerOctave * harmonicLog /
                      0.6931472f);
    }

    block->harmonicDecay =
        _tosGetProfileFloat(block, 7, 0.84f);

    if (block->splineWork[SPLINE_DIFFERENCE] == NULL ||
        block->splineWork[SPLINE_DIAGONAL] == NULL ||
        block->splineWork[SPLINE_FACTOR] == NULL ||
        block->splineWork[SPLINE_RIGHT_HAND_SIDE] == NULL ||
        block->splineWork[SPLINE_FORWARD_SOLUTION] == NULL ||
        block->splineDerivatives == NULL ||
        block->logSpectrum == NULL ||
        block->weightedLogSpectrum == NULL ||
        block->frequencyWeights == NULL ||
        block->harmonicOffsets == NULL) {
        _tosErrorLog(block, 2);
        return 1;
    }
    return 0;
}

void *ConstructSHS_VUV(TosContext *context, u32 blockIndex)
{
    return tosBaseBlockConstruct(
        context, blockIndex, 2, 2, ProcessSHS_VUV, InitSHS_VUV,
        ControlSHS_VUV, sizeof(SHS_VUV));
}
