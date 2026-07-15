#include "gssdk/triggerlr.h"

#include "stdlib.h"

extern void *heap_Calloc(void *heap, u32 count, u32 size);
extern void heap_Free(void *heap, void *ptr);

s32 Subsampler_Process(TriggerLR *block, const s16 *input)
{
    if (input != NULL) {
        const s16 *inputEnd = input + block->subsamplerInputCount;
        s16 *output = block->subsamplerOutput;

        while (input < inputEnd) {
            s16 sample = *input;

            if (block->subsamplerPhase == 1) {
                *block->delayWrite++ = sample << block->sampleShift;
                if (block->delayWrite >= block->delayEnd) {
                    block->delayWrite = block->delay;
                }

                block->delayRead++;
                if (block->delayRead >= block->delayEnd) {
                    block->delayRead = block->delay;
                }
                block->subsamplerPhase = 0;
            } else {
                s32 *product = *block->productRow;
                s32 *symmetricProduct =
                    product + block->halfImpulseLengthPlusOne;
                s32 *coefficient = block->coefficients;
                s32 sum = 0;
                s32 **row;
                u32 column;

                while (coefficient < block->coefficientsEnd) {
                    s32 value = sample * *coefficient++;

                    *--symmetricProduct = value;
                    *product++ = value;
                }

                block->productRow++;
                if (block->productRow >= block->productRowsEnd) {
                    block->productRow = block->productRows;
                }

                row = block->productRow;
                for (column = 0;
                     column < block->halfImpulseLengthPlusOne; column++) {
                    sum += (*row)[column];
                    row++;
                    if (row >= block->productRowsEnd) {
                        row = block->productRows;
                    }
                }

                sum += *block->delayRead;
                *output++ = (s16)(sum >> block->coefficientShift);
                block->subsamplerPhase = 1;
            }
            input++;
        }
    }
    return 0;
}

static s32 Subsampler_CheckImpulseResponse(
    TriggerLR *block, const f64 *impulseResponse, u32 impulseLength)
{
    const f64 tolerance = 1e-15;
    const f64 *left;
    const f64 *right;
    u32 invalid = 0;

    if ((impulseLength & 1) == 0) {
        invalid = 1;
        goto done;
    }

    block->halfImpulseLength = impulseLength >> 1;
    block->halfImpulseLengthPlusOne = block->halfImpulseLength + 1;
    if ((block->halfImpulseLength & 1) == 0) {
        invalid = 1;
        goto done;
    }
    if ((block->halfImpulseLengthPlusOne & 1) == 1) {
        invalid = 1;
        goto done;
    }

    left = impulseResponse;
    right = impulseResponse + impulseLength - 1;
    while (left < right) {
        if (abs(*left - *right) > tolerance) {
            invalid = 1;
            goto done;
        }
        left++;
        right--;
    }

    if (abs(*left - 0.5) > tolerance) {
        invalid = 1;
        goto done;
    }

    left = impulseResponse + 1;
    while (left < right) {
        if (abs(*left) > tolerance) {
            invalid = 1;
            goto done;
        }
        left += 2;
    }

done:
    if (invalid != 0) {
        _tosErrorLog(block, 300);
    }
    return invalid;
}

void Subsampler_Reset(TriggerLR *block)
{
    s32 *delay;
    s32 **row;

    for (delay = block->delay; delay < block->delayEnd; delay++) {
        *delay = 0;
    }
    block->delayWrite = block->delay;
    block->delayRead = block->delay + block->halfImpulseLength / 2;

    for (row = block->productRows; row < block->productRowsEnd; row++) {
        s32 *value;

        for (value = *row;
             value < *row + block->halfImpulseLengthPlusOne; value++) {
            *value = 0;
        }
    }
    block->productRow = block->productRows;
    block->subsamplerPhase = 0;
}

s32 Subsampler_Init(TriggerLR *block)
{
    f64 impulseResponse[23] = {
        -0.0044691990096899963,
        -2.8698779710312522e-16,
        0.011551957698517620,
        3.9956647417786528e-16,
        -0.024138391579451441,
        -5.5259703582695518e-16,
        0.046702085726844770,
        6.5110922026792407e-16,
        -0.095170806790360677,
        -7.6770895142885015e-16,
        0.31453233358223820,
        0.50000000000000078,
        0.31453233358223820,
        -7.6770895142885015e-16,
        -0.095170806790360677,
        6.5110922026792407e-16,
        0.046702085726844770,
        -5.5259703582695518e-16,
        -0.024138391579451441,
        3.9956647417786528e-16,
        0.011551957698517620,
        -2.8698779710312522e-16,
        -0.0044691990096899963,
    };
    TosContext *context = block->base.context;
    s32 **row;
    f64 scale;
    f64 *impulse;
    s32 *coefficient;
    u32 result = 0;

    block->impulseLength = 23;
    if (Subsampler_CheckImpulseResponse(
            block, impulseResponse, block->impulseLength) != 0) {
        return 1;
    }

    block->subsamplerInputCount =
        (u16)_tosGetProfileU32(block, 30, 110);
    block->subsamplerOutput =
        heap_Calloc(context->heap, block->frameLength, sizeof(s16));
    block->coefficientShift = 12;
    block->delayWrite = NULL;
    block->delayRead = NULL;
    block->productRow = NULL;
    block->productRowsEnd = NULL;

    block->delay = heap_Calloc(
        context->heap, block->halfImpulseLength, sizeof(s32));
    block->delayEnd = block->delay + block->halfImpulseLength;

    block->productRows = heap_Calloc(
        context->heap, block->halfImpulseLengthPlusOne, sizeof(s32 *));
    block->productRowsEnd =
        block->productRows + block->halfImpulseLengthPlusOne;

    if (block->subsamplerOutput == NULL || block->delay == NULL ||
        block->productRows == NULL) {
        _tosErrorLog(block, 2);
        return 1;
    }

    row = block->productRows;
    while (row < block->productRowsEnd) {
        *row = heap_Calloc(
            context->heap, block->halfImpulseLengthPlusOne, sizeof(s32));
        if (row == NULL) {
            _tosErrorLog(block, 2);
            result = 1;
            goto done;
        }
        row++;
    }

    block->coefficients = heap_Calloc(
        context->heap, block->halfImpulseLengthPlusOne / 2, sizeof(s32));
    block->coefficientsEnd =
        block->coefficients + block->halfImpulseLengthPlusOne / 2;
    if (block->coefficients == NULL) {
        _tosErrorLog(block, 2);
        result = 1;
        goto done;
    }

    scale = 1 << block->coefficientShift;
    impulse = impulseResponse;
    coefficient = block->coefficients;
    while (coefficient < block->coefficientsEnd) {
        *coefficient++ = (s32)(*impulse * scale);
        impulse += 2;
    }
    block->sampleShift = block->coefficientShift - 1;
    Subsampler_Reset(block);

done:
    return result;
}

void Subsampler_Free(TriggerLR *block)
{
    TosContext *context = block->base.context;
    s32 **row;

    if (block->coefficients != NULL) {
        heap_Free(context->heap, block->coefficients);
    }
    if (block->delay != NULL) {
        heap_Free(context->heap, block->delay);
    }
    for (row = block->productRows; row < block->productRowsEnd; row++) {
        heap_Free(context->heap, *row);
    }
    if (block->productRows != NULL) {
        heap_Free(context->heap, block->productRows);
    }
    if (block->subsamplerOutput != NULL) {
        heap_Free(context->heap, block->subsamplerOutput);
    }
}
