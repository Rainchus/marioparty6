#include "types.h"

#include <string.h>

#include "gssdk/tos.h"

typedef struct Window {
    TosBaseBlock base;
    f32 *history;
    f32 *historyWrite;
    f32 *historyEnd;
    u16 windowLength;
    u16 frameLength;
    u16 reserved38;
    u16 overlapLength;
    u16 frameCount;
    f32 previousSample;
    u8 firstFrame;
    u8 flushed;
    s16 firstSample;
    s16 lastSample;
    u8 flushPending;
    f32 *window;
} Window;

extern void *heap_Calloc(void *heap, u32 count, u32 size);
extern void heap_Free(void *heap, void *ptr);
extern f32 cosf(f32 value);

static void WindowFlush(Window *block)
{
    u16 frameLength = block->frameLength;
    f32 *history = block->historyWrite - frameLength;
    f32 *window = block->window;
    f32 *output = qEnQueueOne(block->base.output->queue);
    f32 *windowEnd;

    if (history < block->history) {
        history = block->historyEnd - frameLength;
    }
    memcpy(block->historyWrite, history, frameLength * sizeof(f32));
    *block->historyWrite = (f32)block->firstSample - (f32)block->lastSample;

    block->historyWrite += frameLength;
    if (block->historyWrite == block->historyEnd) {
        block->historyWrite = block->history;
    }

    history = block->historyWrite + frameLength - block->overlapLength;
    windowEnd = window + block->overlapLength;
    while (window < windowEnd) {
        *output++ = *window++ * *history++;
    }
    if (history == block->historyEnd) {
        history = block->history;
    }

    windowEnd = window + frameLength;
    while (window < windowEnd) {
        *output++ = *window++ * *history++;
    }
    if (history == block->historyEnd) {
        history = block->history;
    }

    windowEnd = window + block->overlapLength;
    while (window < windowEnd) {
        *output++ = *window++ * *history++;
    }

    block->flushed = 1;
    block->flushPending = 0;
}

static void ProcessWindow(
    TosBaseBlock *baseBlock, void **inputs, s32 inputCount)
{
    Window *block = (Window *)baseBlock;
    s16 *input = inputs[0];
    f32 *history;
    f32 *window;
    f32 *output;
    f32 *windowEnd;
    f32 previous;
    f32 sample;
    u16 frameLength;
    s32 i;

    if (input != NULL) {
        frameLength = block->frameLength;
        window = block->window;
        history = block->historyWrite;
        block->firstSample = *input;
        previous = block->previousSample;

        for (i = 0; i < frameLength; i++) {
            sample = (f32)*input++;
            *history++ = sample - previous;
            previous = sample;
        }
        block->lastSample = input[-1];
        block->previousSample = sample;

        if (history == block->historyEnd) {
            history = block->history;
        }

        if (block->firstFrame != 0) {
            *block->historyWrite = 0.0f;
            memcpy(history, block->historyWrite, frameLength * sizeof(f32));
            history += frameLength;
            if (history == block->historyEnd) {
                history = block->history;
            }
            block->firstFrame = 0;
            block->flushed = 0;
            block->historyWrite = history;
        } else {
            output = qEnQueueOne(block->base.output->queue);
            block->historyWrite = history;
            history += frameLength - block->overlapLength;

            windowEnd = window + block->overlapLength;
            while (window < windowEnd) {
                *output++ = *window++ * *history++;
            }
            if (history == block->historyEnd) {
                history = block->history;
            }

            windowEnd = window + frameLength;
            while (window < windowEnd) {
                *output++ = *window++ * *history++;
            }
            if (history == block->historyEnd) {
                history = block->history;
            }

            windowEnd = window + block->overlapLength;
            while (window < windowEnd) {
                *output++ = *window++ * *history++;
            }
        }
    }

    if (block->flushPending != 0) {
        WindowFlush(block);
    }
}

static u32 ControlWindow(
    TosBaseBlock *baseBlock, u32 command, void *argument, u32 argumentSize)
{
    Window *block = (Window *)baseBlock;

    switch ((u8)command) {
    case 1:
        block->previousSample = 0.0f;
        block->firstFrame = 1;
        block->flushed = 1;
        break;
    case 2:
        if (block->flushed == 0) {
            block->flushPending = 1;
        }
        break;
    case 255:
        heap_Free(block->base.context->heap, block->history);
        heap_Free(block->base.context->heap, block->window);
        tosBaseBlockDestruct(block);
        break;
    default:
        return 0;
    }
    return 1;
}

static u32 InitWindow(TosBaseBlock *baseBlock)
{
    Window *block = (Window *)baseBlock;
    TosContext *context = baseBlock->context;
    u16 frameLength;
    u16 windowLength;
    s32 i;

    frameLength = (u16)_tosGetProfileU32(block, 1, 110);
    block->frameLength = frameLength;
    windowLength = (u16)_tosGetProfileU32(block, 2, 330);
    block->windowLength = windowLength;
    block->overlapLength = (windowLength - frameLength) / 2;

    if (frameLength > windowLength || windowLength > frameLength * 3) {
        return 1;
    }

    block->base.input->inputSize = frameLength * sizeof(s16);
    block->base.output->outputSize = windowLength * sizeof(f32);
    block->frameCount = 3;
    block->history = heap_Calloc(
        context->heap, frameLength * 3, sizeof(f32));
    block->window = heap_Calloc(context->heap, windowLength, sizeof(f32));
    if (block->history == NULL || block->window == NULL) {
        _tosErrorLog(block, 2);
        return 1;
    }

    block->historyWrite = block->history;
    block->historyEnd = block->history + frameLength * 3;
    for (i = 0; i < windowLength; i++) {
        block->window[i] =
            0.54f -
            0.46f * cosf((6.283185308 * i) / (windowLength - 1));
    }

    block->previousSample = 0.0f;
    block->firstFrame = 1;
    block->flushed = 1;
    block->flushPending = 0;
    return 0;
}

void *ConstructWindow(TosContext *context, u32 blockIndex)
{
    return tosBaseBlockConstruct(
        context, blockIndex, 1, 1, ProcessWindow, InitWindow, ControlWindow,
        sizeof(Window));
}
