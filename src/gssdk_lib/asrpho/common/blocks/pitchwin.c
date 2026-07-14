#include "types.h"

#include <string.h>

#include "gssdk/tos.h"

#define M_PI 3.141592653589793

typedef struct PitchWindow {
    TosBaseBlock base;
    f32 *window;
    s32 *history;
    s32 *historyRead;
    s32 *historyWrite;
    s32 *historyEnd;
    u16 frameCount;
    u16 frameLength;
    u16 windowLength;
} PitchWindow;

extern void *heap_Calloc(void *heap, u32 count, u32 size);
extern void heap_Free(void *heap, void *ptr);
extern f32 cosf(f32 value);

static void ProcessPitchWindow(
    TosBaseBlock *baseBlock, void **inputs, s32 inputCount)
{
    PitchWindow *block = (PitchWindow *)baseBlock;
    s32 *input = inputs[0];
    f32 *window;
    f32 *output;
    u16 frame;
    u16 sample;

    if (input != NULL) {
        memcpy(block->historyWrite, input, block->frameLength * sizeof(s32));
        block->historyWrite += block->frameLength;
        if (block->historyWrite == block->historyEnd) {
            block->historyWrite = block->history;
        }

        output = qEnQueueOne(block->base.output->queue);
        window = block->window;
        for (frame = 0; frame < block->frameCount; frame++) {
            for (sample = 0; sample < block->frameLength; sample++) {
                *output++ = *block->historyRead++ * *window++;
            }
            if (block->historyRead == block->historyEnd) {
                block->historyRead = block->history;
            }
        }

        block->historyRead += block->frameLength;
        if (block->historyRead == block->historyEnd) {
            block->historyRead = block->history;
        }
    }
}

static u32 ControlPitchWindow(
    TosBaseBlock *baseBlock, u32 command, void *argument, u32 argumentSize)
{
    PitchWindow *block = (PitchWindow *)baseBlock;
    u16 i;

    switch ((u8)command) {
    case 200:
        block->historyWrite = block->history;
        for (i = 0; i < block->windowLength; i++) {
            *block->historyWrite++ = 0;
        }
        block->historyRead = block->history;
        block->historyWrite =
            block->history + (block->frameCount - 1) * block->frameLength;
        block->historyEnd = block->history + block->windowLength;
        break;
    case 255:
        heap_Free(block->base.context->heap, block->window);
        heap_Free(block->base.context->heap, block->history);
        tosBaseBlockDestruct(block);
        break;
    default:
        return 0;
    }
    return 1;
}

#pragma dont_inline on
static void CreateWindow(f32 *window, u16 sampleRate, u16 windowLength)
{
    u16 halfLength;
    s32 i;
    f32 angularFrequency;
    f32 samplePeriod;
    f32 time;

    halfLength = windowLength / 2;
    angularFrequency =
        (2.0 * M_PI) / ((f32)windowLength / (f32)sampleRate);
    window[halfLength] = 1.0f;
    samplePeriod = 1.0f / sampleRate;
    time = (halfLength - 1) * samplePeriod;

    for (i = 1; i < halfLength; i++) {
        window[halfLength - i] =
            0.54 - 0.46 * cosf(angularFrequency * time);
        window[halfLength + i] = window[halfLength - i];
        time -= samplePeriod;
    }
    window[0] = 0.54 - 0.46 * cosf(angularFrequency * time);
}
#pragma dont_inline reset

static u32 InitPitchWindow(TosBaseBlock *baseBlock)
{
    PitchWindow *block = (PitchWindow *)baseBlock;
    TosContext *context = baseBlock->context;
    u16 sampleRate;
    f32 windowDuration;
    u16 inputFrameLength;
    u16 inputSampleRate;
    u16 windowLength;
    f64 inputWindowLength;

    sampleRate = (u16)_tosGetProfileU32(block, 1, 2200);
    windowDuration = _tosGetProfileFloat(block, 2, 0.04f);
    inputFrameLength = (u16)_tosGetProfileU32(block, 3, 110);
    inputSampleRate = (u16)_tosGetProfileU32(block, 4, 11000);

    windowLength = sampleRate * windowDuration + 0.5;
    block->windowLength = windowLength;
    inputWindowLength = 0.5 + windowDuration * inputSampleRate;
    if ((u32)inputWindowLength % inputFrameLength != 0) {
        _tosErrorLog(block, 100);
        return 1;
    }

    block->frameCount = (u32)inputWindowLength / inputFrameLength;
    block->frameLength = windowLength / block->frameCount;
    block->base.input->inputSize = block->frameLength * sizeof(s32);
    block->base.output->outputSize = windowLength * sizeof(f32);

    block->window = heap_Calloc(context->heap, windowLength, sizeof(f32));
    block->history = heap_Calloc(context->heap, windowLength, sizeof(s32));
    if (block->window == NULL || block->history == NULL) {
        _tosErrorLog(block, 2);
        return 1;
    }

    CreateWindow(block->window, sampleRate, windowLength);
    block->historyRead = block->history;
    block->historyWrite =
        block->history + (block->frameCount - 1) * block->frameLength;
    block->historyEnd = block->history + block->windowLength;
    return 0;
}

void *ConstructPitchWindow(TosContext *context, u32 blockIndex)
{
    return tosBaseBlockConstruct(
        context, blockIndex, 1, 1, ProcessPitchWindow, InitPitchWindow,
        ControlPitchWindow, sizeof(PitchWindow));
}
