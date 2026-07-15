#include "types.h"

#include "gssdk/tos.h"

typedef struct PitchDPCell {
    f32 observation;
    f32 score;
    u32 previousState;
} PitchDPCell;

typedef struct PitchDP {
    TosBaseBlock base;
    u16 stateCount;
    u8 windowLength;
    u8 delayLength;
    u8 inputOffset;
    u32 pruningDivisor;
    u16 maximumJump;
    f32 passNumber;
    u16 sampleRate;
    u16 historyLength;
    f32 jumpScale;
    u16 activeFrameCount;
    u16 inputFrameCount;
    u16 bufferedFrameCount;
    u16 flushFrameCount;
    u16 flushOutputIndex;
    u8 flushing;
    u8 firstFlush;
    u8 initializeScores;
    PitchDPCell *cells;
    PitchDPCell **states;
    f32 *pitchValues;
    f32 **jumpFactors;
    f32 *bestPath;
} PitchDP;

extern void *heap_Calloc(void *heap, u32 count, u32 size);
extern void heap_Free(void *heap, void *ptr);
extern f32 logf_check(f32 value);
extern f32 powf(f32 base, f32 exponent);

static void Actual_DP(PitchDP *block, u16 stateCount, u16 frameCount)
{
    PitchDPCell *cells = block->cells;
    u16 maximumJump = block->maximumJump;
    u16 historyLength = block->historyLength;
    f32 **jumpFactors = block->jumpFactors;
    PitchDPCell **states = block->states;
    u16 frame;

    for (frame = 1; frame < frameCount; frame++) {
        f32 maximumScore = 0.0f;
        f32 pruningScore;
        u16 state;
        PitchDPCell *cell = cells + frame - 1;

        for (state = 0; state < block->stateCount;
             state++, cell += historyLength) {
            if (cell->score > maximumScore) {
                maximumScore = cell->score;
            }
        }

        pruningScore =
            maximumScore - maximumScore / block->pruningDivisor;
        for (state = 0; state < stateCount; state++) {
            PitchDPCell *previous =
                &cells[state * historyLength + frame - 1];

            if (previous->score > pruningScore) {
                u16 firstState;
                u16 lastState;
                u16 nextState;

                if (state < maximumJump) {
                    firstState = 0;
                } else {
                    firstState = state - maximumJump;
                }

                if (stateCount - state < maximumJump) {
                    lastState = stateCount - 1;
                } else {
                    lastState = state + maximumJump;
                }

                for (nextState = firstState;
                     nextState < lastState; nextState++) {
                    f32 jumpFactor =
                        jumpFactors[state][nextState];

                    if (jumpFactor > 0.0f) {
                        PitchDPCell *next =
                            &states[nextState][frame];
                        f32 score =
                            previous->score +
                            next->observation * jumpFactor;

                        if (score > next->score) {
                            next->score = score;
                            next->previousState = state;
                        }
                    }
                }
            }
        }
    }
}

static void BackTracking(PitchDP *block, u16 frameCount)
{
    u16 stateCount = block->stateCount;
    f32 *pitchValues = block->pitchValues;
    PitchDPCell **states = block->states;
    f32 *bestPath = block->bestPath;

    if (frameCount > 1) {
        u16 frame = frameCount - 1;
        f32 maximumScore = states[0][frame].score;
        u16 bestState = 0;
        u16 state;
        PitchDPCell **stateRow = states + 1;
        f32 *pitchValue = pitchValues + 1;

        bestPath[frame] = pitchValues[0];
        for (state = 1; state < stateCount;
             state++, stateRow++, pitchValue++) {
            if ((*stateRow)[frame].score > maximumScore) {
                maximumScore = (*stateRow)[frame].score;
                bestState = state;
                bestPath[frame] = *pitchValue;
            }
        }

        {
            PitchDPCell *cell = &states[bestState][frame];
            f32 *output = bestPath + frame;

            for (; frame != 0; frame--) {
                bestState = cell->previousState;
                cell = &states[bestState][frame - 1];
                *--output = pitchValues[bestState];
            }
        }
    } else {
        f32 maximumScore = states[0][0].observation;
        u16 state;
        PitchDPCell **stateRow = states + 1;
        f32 *pitchValue = pitchValues + 1;

        bestPath[0] = 0.0f;
        for (state = 1; state < stateCount;
             state++, stateRow++, pitchValue++) {
            if ((*stateRow)->observation > maximumScore) {
                maximumScore = (*stateRow)->observation;
                bestPath[0] = *pitchValue;
            }
        }
    }
}

static void Output(PitchDP *block)
{
    u8 windowLength = block->windowLength;
    f32 *bestPath = block->bestPath;
    u8 delayLength = block->delayLength;
    TosBaseBlock baseBlock = block->base;
    u8 frame;

    if (block->passNumber == 1.0) {
        for (frame = 0; frame < windowLength; frame++) {
            f32 *output = qEnQueueOne(baseBlock.output->queue);

            *output = bestPath[frame];
        }
    } else {
        for (frame = 0; frame < windowLength; frame++) {
            f32 *output = qEnQueueOne(baseBlock.output->queue);

            *output = bestPath[delayLength + frame];
        }
    }
}

static void PostDP(PitchDP *block)
{
    u16 stateCount = block->stateCount;
    u8 windowLength = block->windowLength;
    u8 delayLength = block->delayLength;
    PitchDPCell **states = block->states;
    u16 frame;

    if (block->passNumber == 1.0) {
        for (frame = 0; frame < delayLength * 2; frame++) {
            u16 state;

            for (state = 0; state < stateCount; state++) {
                states[state][frame] =
                    states[state][frame + windowLength - delayLength];
            }
        }
    } else {
        for (frame = 0; frame < delayLength * 2; frame++) {
            u16 state;

            for (state = 0; state < stateCount; state++) {
                states[state][frame] =
                    states[state][frame + windowLength];
            }
        }
    }
}

static u8 FlushPitchDP(PitchDP *block)
{
    u8 result = 0;
    u16 stateCount = block->stateCount;

    if (block->firstFlush != 0) {
        u16 state;

        if (block->passNumber == 1.0) {
            block->flushFrameCount = block->bufferedFrameCount;
        } else {
            block->flushFrameCount =
                block->delayLength + block->bufferedFrameCount;
        }

        if (block->flushFrameCount == 0) {
            block->flushing = 0;
            return 0;
        }

        for (state = 0; state < stateCount; state++) {
            block->states[state][0].previousState = 0;
        }
        Actual_DP(block, stateCount, block->flushFrameCount);
        BackTracking(block, block->flushFrameCount);
        if (block->passNumber == 1.0) {
            block->delayLength = 0;
        }
        block->firstFlush = 0;
    }

    {
        f32 *output = qEnQueueOne(block->base.output->queue);

        *output = block->bestPath[
            block->delayLength + block->flushOutputIndex];
    }
    block->flushOutputIndex++;
    if (block->flushOutputIndex == block->flushFrameCount) {
        block->flushing = 0;
    } else {
        result = 1;
    }
    return result;
}

static void ProcessPitchDP(
    TosBaseBlock *baseBlock, void **inputs, s32 inputCount)
{
    PitchDP *block = (PitchDP *)baseBlock;
    f32 *input = inputs[0];
    u16 stateCount = block->stateCount;
    u16 activeFrameCount = block->activeFrameCount;

    if (input != NULL) {
        u16 state;

        if (block->passNumber == 2.0) {
            block->activeFrameCount =
                block->windowLength + block->delayLength * 2;
            block->inputFrameCount = block->windowLength;
            block->inputOffset = block->delayLength * 2;
        }

        for (state = 0; state < stateCount; state++) {
            block->states[state][
                block->bufferedFrameCount + block->inputOffset]
                .observation = input[state];
        }

        if (block->bufferedFrameCount < block->inputFrameCount - 1) {
            block->bufferedFrameCount++;
            return;
        }

        if (block->initializeScores != 0) {
            for (state = 0; state < stateCount; state++) {
                PitchDPCell *first = &block->states[state][0];

                first->score = first->observation;
                first->previousState = 0;
            }
            block->initializeScores = 0;
        }

        Actual_DP(block, stateCount, activeFrameCount);
        BackTracking(block, activeFrameCount);
        Output(block);
        PostDP(block);
        block->bufferedFrameCount = 0;
        block->passNumber += 1.0f;
    } else if (block->flushing == 1) {
        FlushPitchDP(block);
    }
}

static void PitchDPReset(PitchDP *block)
{
    u16 historyLength = block->historyLength;
    u16 stateCount = block->stateCount;
    PitchDPCell *cells = block->cells;
    u16 initialFrameCount;
    u16 frame;

    block->delayLength = (u8)_tosGetProfileU32(block, 5, 6);
    initialFrameCount =
        block->windowLength + block->delayLength;
    block->activeFrameCount = initialFrameCount;
    block->inputFrameCount = initialFrameCount;
    block->bufferedFrameCount = 0;
    block->passNumber = 1.0f;
    block->inputOffset = 0;
    block->flushFrameCount = 0;
    block->flushing = 0;
    block->firstFlush = 1;
    block->initializeScores = 1;
    block->flushOutputIndex = 0;

    for (frame = 0; frame < historyLength; frame++) {
        u16 state;

        for (state = 0; state < stateCount; state++) {
            cells[state * historyLength + frame].score = 0.0f;
        }
    }
}

static u32 ControlPitchDP(
    TosBaseBlock *baseBlock, u32 command, void *argument, u32 argumentSize)
{
    PitchDP *block = (PitchDP *)baseBlock;

    switch ((u8)command) {
    case 201:
        PitchDPReset(block);
        break;
    case 200:
        block->flushing = 1;
        break;
    case 255:
        if (block->cells != NULL) {
            TosContext *context = block->base.context;
            u16 stateCount = block->stateCount;
            f32 **jumpFactors = block->jumpFactors;
            u16 state;

            heap_Free(context->heap, block->cells);
            heap_Free(context->heap, block->states);
            heap_Free(context->heap, block->pitchValues);
            heap_Free(context->heap, block->bestPath);
            for (state = 0; state < stateCount; state++) {
                heap_Free(
                    context->heap, jumpFactors[state]);
            }
            heap_Free(context->heap, jumpFactors);
        }
        tosBaseBlockDestruct(block);
        break;
    default:
        return 0;
    }
    return 1;
}

static void InitJumpFactors(PitchDP *block)
{
    f32 divisor = block->sampleRate * block->jumpScale;
    s32 state;

    for (state = 0; state < block->stateCount; state++) {
        s32 otherState;

        block->jumpFactors[state][state] = 1.0f;
        for (otherState = state - 1; otherState >= 0; otherState--) {
            f32 difference =
                (block->pitchValues[state] -
                 block->pitchValues[otherState]) /
                divisor;

            block->jumpFactors[state][otherState] =
                1.0f - difference * difference;
            if (block->jumpFactors[state][otherState] < 0.0f) {
                block->jumpFactors[state][otherState] = 0.0f;
                break;
            }
        }

        for (otherState = state + 1;
             otherState < block->stateCount; otherState++) {
            f32 difference =
                (block->pitchValues[otherState] -
                 block->pitchValues[state]) /
                divisor;

            block->jumpFactors[state][otherState] =
                1.0f - difference * difference;
            if (block->jumpFactors[state][otherState] < 0.0f) {
                block->jumpFactors[state][otherState] = 0.0f;
                break;
            }
        }
    }
}

static u32 InitPitchDP(TosBaseBlock *baseBlock)
{
    PitchDP *block = (PitchDP *)baseBlock;
    TosContext *context = block->base.context;
    u16 pitchResolution;
    u8 inputPitchRange;
    u16 maximumPitch;
    u16 minimumPitch;
    f32 logTwo;
    u16 initialFrameCount;
    u32 state;

    pitchResolution = (u16)_tosGetProfileU32(block, 1, 50);
    inputPitchRange = (u8)_tosGetProfileU32(block, 10, 5);
    maximumPitch = (u16)_tosGetProfileU32(block, 2, 500);
    minimumPitch = (u16)_tosGetProfileU32(block, 3, 50);

    block->base.input->inputSize =
        inputPitchRange * pitchResolution * sizeof(f32);
    block->base.output->outputSize = sizeof(f32);
    block->pruningDivisor =
        (u16)_tosGetProfileU32(block, 6, 10);
    block->maximumJump =
        (u16)_tosGetProfileU32(block, 7, 10);
    block->sampleRate =
        (u16)_tosGetProfileU32(block, 8, 2000);
    block->windowLength =
        (u8)_tosGetProfileU32(block, 4, 15);
    block->delayLength =
        (u8)_tosGetProfileU32(block, 5, 6);
    block->historyLength =
        block->windowLength + block->delayLength * 2;
    block->jumpScale = _tosGetProfileFloat(block, 9, 0.01f);

    block->passNumber = 1.0f;
    block->flushFrameCount = 0;
    block->flushOutputIndex = 0;
    block->flushing = 0;
    block->firstFlush = 1;
    block->initializeScores = 1;
    initialFrameCount =
        block->windowLength + block->delayLength;
    block->activeFrameCount = initialFrameCount;
    block->inputFrameCount = initialFrameCount;
    block->bufferedFrameCount = 0;
    block->inputOffset = 0;

    logTwo = logf_check(2.0f);
    block->stateCount = (u16)(
        pitchResolution *
        logf_check((f32)maximumPitch / (f32)minimumPitch) /
        logTwo);

    block->cells = heap_Calloc(
        context->heap, block->stateCount * block->historyLength,
        sizeof(PitchDPCell));
    block->states = heap_Calloc(
        context->heap, block->stateCount, sizeof(PitchDPCell *));
    block->pitchValues = heap_Calloc(
        context->heap, block->stateCount, sizeof(f32));
    block->jumpFactors = heap_Calloc(
        context->heap, block->stateCount, sizeof(f32 *));
    block->bestPath = heap_Calloc(
        context->heap,
        block->windowLength + block->delayLength * 2, sizeof(f32));
    if (block->cells == NULL || block->states == NULL ||
        block->jumpFactors == NULL || block->pitchValues == NULL ||
        block->bestPath == NULL) {
        _tosErrorLog(block, 2);
        return 1;
    }

    for (state = 0; state < block->stateCount; state++) {
        block->states[state] =
            block->cells + block->historyLength * state;
        block->pitchValues[state] =
            minimumPitch *
            powf(
                2.0f,
                (f32)state / (f32)pitchResolution);
        block->jumpFactors[state] = heap_Calloc(
            context->heap, block->stateCount, sizeof(f32));
        if (block->jumpFactors[state] == NULL) {
            _tosErrorLog(block, 2);
            return 1;
        }
    }

    InitJumpFactors(block);
    return 0;
}

void *ConstructPitchDP(TosContext *context, u32 blockIndex)
{
    return tosBaseBlockConstruct(
        context, blockIndex, 1, 1, ProcessPitchDP, InitPitchDP,
        ControlPitchDP, sizeof(PitchDP));
}
