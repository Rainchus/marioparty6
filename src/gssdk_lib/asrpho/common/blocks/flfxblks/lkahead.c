#include "types.h"

#include "gssdk/tos.h"

typedef struct LookAhead {
TosBaseBlock base;
u8 flushActive;
u8 reserved29[3];
u32 lookAhead;
u32 historyLength;
f32 threshold;
f32 *history;
f32 *historyWrite;
f32 *historyEnd;
f32 sum;
u32 queued;
} LookAhead;

extern void *heap_Calloc(void *heap, u32 count, u32 size);
extern void heap_Free(void *heap, void *ptr);

static void ProcessLookAhead(
    TosBaseBlock *baseBlock, void **input, s32 inputCount)
{
LookAhead *block = (LookAhead *)baseBlock;
f32 *sample = input[0];
u8 *output;

if (sample != NULL) {
    block->sum -= *block->historyWrite;
    *block->historyWrite++ = *sample;
    block->sum += *sample;
    if (block->historyWrite == block->historyEnd) {
        block->historyWrite = block->history;
    }
    if (block->queued == block->lookAhead) {
        output = qEnQueueOne(block->base.output->queue);
        *output = block->sum > block->threshold;
    } else {
        block->queued++;
    }
} else if (block->flushActive != 0) {
    while (block->queued != 0) {
        output = qEnQueueOne(block->base.output->queue);
        *output = block->sum > block->threshold;
        block->queued--;
    }
    if (block->queued == 0) {
        block->flushActive = 0;
    }
}
}

static u32 ControlLookAhead(
    TosBaseBlock *baseBlock, u32 command, void *argument, u32 argumentSize)
{
LookAhead *block = (LookAhead *)baseBlock;

switch ((u8)command) {
case 1:
    block->flushActive = 1;
    break;
case 255:
    heap_Free(block->base.context->heap, block->history);
    tosBaseBlockDestruct(block);
    break;
default:
    return 0;
}
return 1;
}

static u32 InitLookAhead(TosBaseBlock *baseBlock)
{
LookAhead *block = (LookAhead *)baseBlock;
TosContext *context = baseBlock->context;

block->lookAhead = (u16)_tosGetProfileU32(block, 1, 0);
block->historyLength = (u16)_tosGetProfileU32(block, 2, 15);
block->threshold = _tosGetProfileFloat(block, 3, 1.0f);
block->base.input->inputSize = sizeof(f32);
block->base.output->outputSize = sizeof(u8);
block->history = heap_Calloc(
    context->heap, block->historyLength, sizeof(f32));
block->historyWrite = block->history;
block->historyEnd = block->history + block->historyLength;
block->sum = 0.0f;
block->queued = 0;
return 0;
}

void *ConstructLookAhead(TosContext *context, u32 blockIndex)
{
return tosBaseBlockConstruct(
    context, blockIndex, 1, 1, (TosProcessFunction)ProcessLookAhead,
    (TosInitFunction)InitLookAhead, (TosControlFunction)ControlLookAhead,
    sizeof(LookAhead));
}
