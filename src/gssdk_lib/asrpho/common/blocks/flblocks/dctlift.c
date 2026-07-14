#include "types.h"

#include "gssdk/tos.h"

#define M_PI 3.141592653589793

typedef struct DCTLift {
    TosBaseBlock base;
    u32 outputCount;
    u32 inputEnd;
    u32 inputStart;
    f32 *coefficients;
    f32 scale;
} DCTLift;

extern void *heap_Alloc(void *heap, u32 size);
extern void heap_Free(void *heap, void *ptr);
extern f32 sinf(f32 value);
extern f32 cosf(f32 value);

static void ProcessDCTLift(
    TosBaseBlock *baseBlock, void **inputs, s32 inputCount)
{
    DCTLift *block = (DCTLift *)baseBlock;
    f32 *input = inputs[0];
    f32 *inputEnd;
    f32 *inputCursor;
    f32 *output;
    f32 *outputEnd;
    f32 *coefficients;
    f32 scale;

    if (input == NULL) {
        return;
    }

    output = qEnQueueOne(block->base.output->queue);
    coefficients = block->coefficients;
    scale = block->scale;
    *output++ = *input++;
    inputEnd = input + block->inputEnd;
    outputEnd = output + block->outputCount;
    input += block->inputStart;

    while (output < outputEnd) {
        inputCursor = input;
        *output = 0.0f;
        while (inputCursor < inputEnd) {
            *output += *coefficients++ * *inputCursor++;
        }
        *output++ *= scale;
    }

    output -= block->outputCount;
    *output *= 0.5f;
}

static u32 ControlDCTLift(
    TosBaseBlock *baseBlock, u32 command, void *argument, u32 argumentSize)
{
    DCTLift *block = (DCTLift *)baseBlock;

    switch ((u8)command) {
    case 255:
        heap_Free(block->base.context->heap, block->coefficients);
        tosBaseBlockDestruct(block);
        break;
    default:
        return 0;
    }
    return 1;
}

static u32 InitDCTLift(TosBaseBlock *baseBlock)
{
    DCTLift *block = (DCTLift *)baseBlock;
    u32 outputCount;
    s16 liftPeriod;
    u32 inputSpan;
    u32 i;
    u32 j;
    f32 lift;
    f32 *coefficient;

    outputCount = (u16)_tosGetProfileU32(block, 2, 13);
    block->outputCount = outputCount;
    liftPeriod = (s16)_tosGetProfileU32(block, 3, 16);
    block->inputEnd = (u16)_tosGetProfileU32(block, 1, 21);
    block->inputStart = (u16)_tosGetProfileU32(block, 4, 0);

    block->base.input->inputSize =
        (block->inputEnd + 1) * sizeof(f32);
    block->base.output->outputSize =
        (outputCount + 1) * sizeof(f32);

    inputSpan = block->inputEnd - block->inputStart;
    block->scale = 2.0 / inputSpan;
    block->coefficients = heap_Alloc(
        block->base.context->heap,
        outputCount * inputSpan * sizeof(f32));

    coefficient = block->coefficients;
    for (i = 0; i < outputCount; i++) {
        lift = 1.0f + 6.0f * sinf(M_PI * i / liftPeriod);
        for (j = 0; j < inputSpan; j++) {
            *coefficient++ =
                lift * cosf(M_PI * (j + 0.5) * i / inputSpan);
        }
    }
    return 0;
}

void *ConstructDCTLift(TosContext *context, u32 blockIndex)
{
    return tosBaseBlockConstruct(
        context, blockIndex, 1, 1, ProcessDCTLift, InitDCTLift,
        ControlDCTLift, sizeof(DCTLift));
}
