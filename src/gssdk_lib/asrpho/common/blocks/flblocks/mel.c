#include "types.h"

#include "gssdk/tos.h"

typedef struct Mel {
    TosBaseBlock base;
    u32 bandCount;
    f32 **weights;
    s32 *firstBins;
    u32 *binCounts;
    f32 sampleScale;
} Mel;

typedef union MelControlValue {
    u32 bits;
    f32 value;
} MelControlValue;

extern void *heap_Alloc(void *heap, u32 size);
extern void *heap_Calloc(void *heap, u32 count, u32 size);
extern void heap_Free(void *heap, void *ptr);
extern f32 logf_check(f32 value);
extern f32 powf(f32 base, f32 exponent);

static u8 MelInitBands(Mel *block, f32 sampleScale);

static void ProcessMel(
    TosBaseBlock *baseBlock, void **inputs, s32 inputCount)
{
    Mel *block = (Mel *)baseBlock;
    f32 *input = inputs[0];
    f32 *output;
    u32 band;

    if (input == NULL) {
        return;
    }

    output = qEnQueueOne(block->base.output->queue);
    for (band = 0; band < block->bandCount; band++, output++) {
        f32 *weight = block->weights[band];
        f32 *weightEnd = weight + block->binCounts[band];
        f32 *inputValue = input + block->firstBins[band];

        *output = 10.0f;
        while (weight < weightEnd) {
            *output += *weight++ * *inputValue++;
        }
    }
}

static u32 ControlMel(
    TosBaseBlock *baseBlock, u32 command, void *argument, u32 argumentSize)
{
    Mel *block = (Mel *)baseBlock;
    MelControlValue controlValue;
    u32 result = 1;
    u32 band;

    controlValue.bits = argumentSize;
    switch ((u8)command) {
    case 1:
        if (block->weights != NULL) {
            for (band = 0; band < block->bandCount; band++) {
                heap_Free(block->base.context->heap, block->weights[band]);
            }
            heap_Free(block->base.context->heap, block->weights);
            heap_Free(block->base.context->heap, block->firstBins);
            heap_Free(block->base.context->heap, block->binCounts);
            block->weights = NULL;
        }
        result = MelInitBands(block, controlValue.value) == 0;
        break;

    case 255:
        if (block->weights != NULL) {
            for (band = 0; band < block->bandCount; band++) {
                heap_Free(block->base.context->heap, block->weights[band]);
            }
            heap_Free(block->base.context->heap, block->weights);
            heap_Free(block->base.context->heap, block->firstBins);
            heap_Free(block->base.context->heap, block->binCounts);
            block->weights = NULL;
        }
        tosBaseBlockDestruct(block);
        break;

    default:
        result = 0;
        break;
    }
    return result;
}

static u8 MelInitBands(Mel *block, f32 sampleScale)
{
    TosContext *context = block->base.context;
    f32 maximumFrequency;
    u16 skippedBands;
    u16 binCount;
    f32 *frequencyBins;
    f32 *frequencyBin;
    f32 binWidth;
    f32 frequency;
    f32 linearEnd;
    f32 *centerFrequencies;
    f32 *centerFrequency;
    u32 band;

    maximumFrequency = _tosGetProfileFloat(block, 2, 4000.0f);
    skippedBands = (u16)_tosGetProfileU32(block, 5, 0);
    binCount = (u16)_tosGetProfileU32(block, 3, 0);
    block->sampleScale = sampleScale;

    frequencyBins = heap_Alloc(context->heap, binCount * sizeof(f32));
    if (frequencyBins == NULL) {
        _tosErrorLog(block, 100);
        return 1;
    }

    binWidth = maximumFrequency / (binCount - 1);
    linearEnd = maximumFrequency > 1000.0f ? 1000.0f : maximumFrequency;
    frequency = 0.0f;
    frequencyBin = frequencyBins;
    while (frequency < linearEnd) {
        *frequencyBin++ = frequency / (100.0f * sampleScale);
        frequency += binWidth;
    }
    while (frequency < maximumFrequency + binWidth * 0.5f) {
        *frequencyBin++ =
            10.0f +
            5.0f *
                (logf_check(frequency / (1000.0f * sampleScale)) /
                 0.693147f);
        frequency += binWidth;
    }

    if (maximumFrequency < 1000.0f) {
        block->bandCount =
            (u32)((f64)maximumFrequency / 100.0 - 0.95);
    } else {
        block->bandCount =
            (u32)((f64)(
                      10.0f +
                      5.0f *
                          logf_check((f32)((f64)frequency / 1000.0)) /
                          0.693147f) -
                  0.95);
    }
    block->bandCount -= skippedBands;

    if (block->bandCount != _tosGetProfileU32(block, 4, 21)) {
        return 1;
    }

    block->weights =
        heap_Calloc(context->heap, block->bandCount, sizeof(f32 *));
    if (block->weights == NULL) {
        _tosErrorLog(block, 100);
        return 1;
    }

    block->firstBins =
        heap_Calloc(context->heap, block->bandCount, sizeof(s32));
    if (block->firstBins == NULL) {
        _tosErrorLog(block, 100);
        heap_Free(context->heap, block->weights);
        return 1;
    }

    block->binCounts =
        heap_Calloc(context->heap, block->bandCount, sizeof(u32));
    if (block->binCounts == NULL) {
        _tosErrorLog(block, 100);
        heap_Free(context->heap, block->weights);
        heap_Free(context->heap, block->firstBins);
        return 1;
    }

    centerFrequencies =
        heap_Calloc(context->heap, block->bandCount, sizeof(f32));
    if (centerFrequencies == NULL) {
        _tosErrorLog(block, 100);
        heap_Free(context->heap, block->weights);
        heap_Free(context->heap, block->firstBins);
        heap_Free(context->heap, block->binCounts);
        return 1;
    }

    centerFrequency = centerFrequencies;
    for (band = 0; band < block->bandCount; band++, centerFrequency++) {
        u32 melBand = band + skippedBands;
        f32 bandPosition = (f32)(melBand + 1);
        u32 bin;

        block->binCounts[band] = 0;
        block->firstBins[band] = -1;
        if (melBand < 10) {
            *centerFrequency = 100.0f * bandPosition;
        } else {
            *centerFrequency =
                1000.0f *
                powf(
                    2.0f,
                    (f32)((f64)(bandPosition / 5.0f) - 2.0));
        }

        for (bin = 1; bin < binCount; bin++) {
            f32 value = frequencyBins[bin];
            u32 inBand =
                value > bandPosition - 1.0 &&
                value < bandPosition + 1.0;

            block->binCounts[band] += inBand;
            if (inBand != 0 && block->firstBins[band] == -1) {
                block->firstBins[band] = bin;
            }
        }

        block->weights[band] = heap_Calloc(
            context->heap, block->binCounts[band], sizeof(f32));
        if (block->weights[band] == NULL) {
            _tosErrorLog(block, 100);
            heap_Free(context->heap, centerFrequencies);
            return 1;
        }

        {
            f32 *weight = block->weights[band];

            for (bin = 0; bin < block->binCounts[band]; bin++, weight++) {
                f32 value =
                    frequencyBins[block->firstBins[band] + bin];

                if (value < bandPosition) {
                    *weight = 1.0f - bandPosition + value;
                } else {
                    *weight = 1.0f - value + bandPosition;
                }
            }
        }
    }

    heap_Free(context->heap, centerFrequencies);
    heap_Free(context->heap, frequencyBins);
    return 0;
}

u32 InitMel(TosBaseBlock *baseBlock)
{
    Mel *block = (Mel *)baseBlock;
    u16 binCount = (u16)_tosGetProfileU32(block, 3, 0);
    u16 bandCount = (u16)_tosGetProfileU32(block, 4, 21);

    block->base.input->inputSize = binCount * sizeof(f32);
    block->base.output->outputSize = bandCount * sizeof(f32);
    return 0;
}

void *ConstructMel(TosContext *context, u32 blockIndex)
{
    return tosBaseBlockConstruct(
        context, blockIndex, 1, 1, ProcessMel, InitMel, ControlMel,
        sizeof(Mel));
}
