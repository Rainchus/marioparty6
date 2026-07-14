#include "gssdk/tos.h"

#include <string.h>

typedef struct Combiner {
    TosBaseBlock base;
    u32 frameCount;
    u16 historyIndex;
    u16 bandCount;
    u16 vectorLength;
    f32 *history[8];
    f32 *historyAllocation;
    u8 flushing;
} Combiner;

extern void *heap_Calloc(void *heap, u32 count, u32 size);
extern void heap_Free(void *heap, void *ptr);
extern u8 qCheckInputQueues(TosQueuePort *input, u32 count);

static u32 CombinerInit(TosBaseBlock *baseBlock)
{
    Combiner *block = (Combiner *)baseBlock;
    TosContext *context = baseBlock->context;
    f32 *history;

    block->bandCount = (u16)_tosGetProfileU32(block, 2, 12);
    block->vectorLength = block->bandCount * 3 + 3;
    block->base.input->inputSize =
        (block->bandCount + 2) * sizeof(f32);
    block->base.output->outputSize =
        (block->vectorLength - 1) * sizeof(f32);
    block->historyAllocation = heap_Calloc(
        context->heap, block->vectorLength * 8, sizeof(f32));
    if (block->historyAllocation == NULL) {
        return 2;
    }

    history = block->historyAllocation;
    block->history[0] = history;
    history += block->vectorLength;
    block->history[1] = history;
    history += block->vectorLength;
    block->history[2] = history;
    history += block->vectorLength;
    block->history[3] = history;
    history += block->vectorLength;
    block->history[4] = history;
    history += block->vectorLength;
    block->history[5] = history;
    history += block->vectorLength;
    block->history[6] = history;
    history += block->vectorLength;
    block->history[7] = history;
    block->historyIndex = 0;
    return 0;
}

static void CombinerProcess(
    TosBaseBlock *baseBlock, void **inputs, s32 inputCount)
{
    Combiner *block = (Combiner *)baseBlock;
    f32 *input = inputs[0];
    f32 *outputSource = NULL;
    TosQueue *outputQueue = baseBlock->output->queue;

    if (input != NULL) {
        u16 index = block->historyIndex;
        f32 *current = block->history[index & 7];
        f32 *previous1 = block->history[(index + 7) & 7];
        f32 *previous2 = block->history[(index + 6) & 7];
        f32 *previous3 = block->history[(index + 5) & 7];
        f32 *previous4 = block->history[(index + 4) & 7];
        f32 *delta = previous2 + block->bandCount + 3;
        s32 i;

        block->frameCount++;
        current[0] = input[0];
        outputSource = previous3 + 1;
        previous2[1] = 0.2f *
                       (2.0f * input[0] + previous1[0] - previous3[0] -
                        2.0f * previous4[0]);
        previous3[2] = previous2[1] - previous4[1];

        current += 3;
        previous1 += 3;
        previous3 += 3;
        previous4 += 3;
        input += 2;
        for (i = 0; i < block->bandCount; i++) {
            *current++ = *input;
            *delta++ = 0.375f *
                       (2.0f * *input++ + *previous1++ - *previous3++ -
                        2.0f * *previous4++);
        }

        delta -= block->bandCount;
        previous3 += block->bandCount;
        for (i = 0; i < block->bandCount; i++) {
            *previous3++ = *delta++ - *previous4++;
        }
    }

    if (block->flushing != 0) {
        block->frameCount--;
        if (block->frameCount == 4) {
            block->flushing = 0;
        }
        outputSource = block->history[(block->historyIndex + 5) & 7] + 1;
    }

    if (outputSource != NULL && block->frameCount > 3) {
        f32 *output = qEnQueueOne(outputQueue);
        s32 i;

        if (block->frameCount > 6) {
            for (i = 0; i < block->vectorLength - 1; i++) {
                *output++ = *outputSource++;
            }
        } else if (block->frameCount == 6) {
            *output++ = *outputSource;
            outputSource += 2;
            *output++ = 0.0f;
            for (i = 0; i < block->bandCount * 2; i++) {
                *output++ = *outputSource++;
            }
            for (i = 0; i < block->bandCount; i++) {
                *output++ = 0.0f;
            }
        } else {
            *output++ = 0.0f;
            *output++ = 0.0f;
            outputSource += 2;
            for (i = 0; i < block->bandCount; i++) {
                *output++ = *outputSource++;
            }
            for (i = 0; i < block->bandCount * 2; i++) {
                *output++ = 0.0f;
            }
        }
    }

    if (outputSource != NULL) {
        block->historyIndex = (block->historyIndex + 1) & 7;
    }
}

static u32 CombinerControl(
    TosBaseBlock *baseBlock, u32 command, void *argument, u32 argumentSize)
{
    Combiner *block = (Combiner *)baseBlock;

    switch ((u8)command) {
    case 2:
        if (block->frameCount == 0) {
            if (argument == NULL ||
                qCheckInputQueues(block->base.input, 1) == 0) {
                break;
            }
        }
        block->flushing = 1;
        if (block->frameCount < 3) {
            block->frameCount += 4;
        } else {
            block->frameCount = 7;
        }
        break;
    case 1:
        block->frameCount = 0;
        memset(
            block->historyAllocation, 0,
            block->vectorLength * 8 * sizeof(f32));
        break;
    case 255:
        heap_Free(block->base.context->heap, block->historyAllocation);
        tosBaseBlockDestruct(block);
        break;
    default:
        return 0;
    }
    return 1;
}

void *ConstructCombiner(TosContext *context, u32 blockIndex)
{
    return tosBaseBlockConstruct(
        context, blockIndex, 1, 1, CombinerProcess, CombinerInit,
        CombinerControl, sizeof(Combiner));
}
