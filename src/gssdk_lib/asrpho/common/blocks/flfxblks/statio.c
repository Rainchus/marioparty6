#include "types.h"

#include "gssdk/tos.h"

typedef struct Stationarity {
TosBaseBlock base;
u32 drain;
u32 queued;
u32 span;
f32 threshold;
f32 *history;
f32 *historyEnd;
f32 *historyWrite;
f32 *delayed;
f32 *delayedEnd;
f32 *delayedWrite;
} Stationarity;

extern void *heap_Calloc(void *heap, u32 count, u32 size);
extern void heap_Free(void *heap, void *ptr);
extern f32 _tosGetProfileFloat(void *block, u32 key, f32 defaultValue);

static void ProcessStationarity(
    TosBaseBlock *baseBlock, void **input, s32 inputCount)
{
Stationarity *block = (Stationarity *)baseBlock;
f32 sample;
f32 incoming = 0.0f;
f32 delayed;
f32 *cursor;
f32 *output;

if (input[0] != NULL) {
    sample = *(f32 *)input[0];
    incoming = *(f32 *)input[1];
    if (block->queued == 0) {
        cursor = block->history;
        while (cursor < block->historyEnd) {
            *cursor = sample;
            cursor++;
        }
        block->historyWrite = block->history;
    } else {
        *block->historyWrite++ = sample;
        if (block->historyWrite == block->historyEnd) {
            block->historyWrite = block->history;
        }
    }
    block->queued++;
} else if (block->drain == 0) {
    return;
}

delayed = *block->delayedWrite;
*block->delayedWrite++ = incoming;
if (block->delayedWrite == block->delayedEnd) {
    block->delayedWrite = block->delayed;
}
if (block->drain != 0 && block->queued == 0) {
    block->drain = 0;
}
if (block->queued <= block->span && block->drain == 0) {
    return;
}

output = qEnQueueOne(block->base.output->queue);
block->queued--;
if (delayed > 0.0f) {
    cursor = block->history;
    incoming = *cursor;
    sample = incoming;
    while (++cursor < block->historyEnd) {
        f32 value = *cursor;

        if (value < incoming) {
            incoming = value;
        }
        if (value > sample) {
            sample = value;
        }
    }
    if (!(sample - incoming < block->threshold)) {
        delayed *= 0.1f;
    }
}
*output = delayed;
}

static u32 ControlStationarity(
    TosBaseBlock *baseBlock, u32 command, void *argument, u32 argumentSize)
{
Stationarity *block = (Stationarity *)baseBlock;
TosContext *context = baseBlock->context;

switch ((u8)command) {
case 1:
    block->drain = 1;
    break;
case 255:
    heap_Free(context->heap, block->history);
    heap_Free(context->heap, block->delayed);
    tosBaseBlockDestruct(block);
    break;
default:
    return 0;
}
return 1;
}

static u32 InitStationarity(Stationarity *block)
{
TosContext *context = block->base.context;
u32 i;

block->base.input[0].inputSize = 24;
block->base.input[1].inputSize = sizeof(f32);
block->base.output->outputSize = sizeof(f32);
block->span = (u8)_tosGetProfileU32(block, 1, 2);
block->threshold = _tosGetProfileFloat(block, 2, 2.0f);
block->history = heap_Calloc(
    context->heap, block->span * 2 + 1, sizeof(f32));
block->delayed = heap_Calloc(context->heap, block->span, sizeof(f32));
if (block->history == NULL || block->delayed == NULL) {
    return _tosErrorLog(block, 2);
}
for (i = 0; i < block->span * 2 + 1; i++) {
    block->history[i] = 0.0f;
}
block->historyEnd = block->history + block->span * 2 + 1;
block->historyWrite = block->history;
block->delayedEnd = block->delayed + block->span;
block->delayedWrite = block->delayed;
return 0;
}

void *ConstructStationarity(TosContext *context, u32 blockIndex)
{
return tosBaseBlockConstruct(
    context, blockIndex, 2, 1, (TosProcessFunction)ProcessStationarity,
    (TosInitFunction)InitStationarity,
    (TosControlFunction)ControlStationarity, sizeof(Stationarity));
}
