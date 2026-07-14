#include "types.h"

#include "gssdk/tos.h"

typedef s16 (*ExtraEventStateScoreFunction)(void *context);
typedef s32 (*ExtraEventFrameScoreFunction)(void *context);
typedef s16 *(*ExtraEventScoreCacheFunction)(void *context);

typedef struct ExtraEventCallbacks {
void *context;
ExtraEventStateScoreFunction readStateScore;
void *reserved08;
ExtraEventScoreCacheFunction getScoreCache;
ExtraEventFrameScoreFunction readFrameScore;
} ExtraEventCallbacks;

typedef struct ExtraEventContextInfo {
u32 stateCount;
u16 *stateIds;
u16 *transitionBounds;
u32 transitionCount;
s32 transitionScore;
s32 initialScore;
} ExtraEventContextInfo;

typedef struct ExtraEventState {
s32 score;
u32 stateId;
} ExtraEventState;

typedef struct ExtraEventDP {
TosBaseBlock base;
TosQueuePort *input;
void *callbackContext;
ExtraEventStateScoreFunction readStateScore;
ExtraEventFrameScoreFunction readFrameScore;
ExtraEventScoreCacheFunction getScoreCache;
s16 *scoreCache;
u32 stateCount;
u16 *stateIds;
s32 bestScore;
s32 accumulatedScore;
u16 frameCount;
u16 reserved52;
s32 transitionScore;
s32 initialScore;
u32 reserved5C;
u32 transitionCount;
u16 *transitionBounds;
ExtraEventState *states;
} ExtraEventDP;

extern void *heap_Alloc(void *heap, u32 size);
extern void heap_Free(void *heap, void *ptr);

static void InitViterbi(ExtraEventDP *block)
{
ExtraEventState *state;
u16 i;

block->bestScore = 0;
block->accumulatedScore = 0;
block->frameCount = 0xFFFF;

state = block->states;
state->score = block->initialScore;
state->stateId = block->stateIds[0];
state++;

for (i = 1; i < block->stateCount; i++) {
    state->score = 0x7FFFF7FF;
    state->stateId = block->stateIds[i];
    state++;
}
}

static void DynProgExtraEventsProcess(ExtraEventDP *block)
{
ExtraEventState *states;
ExtraEventState *lastState;
ExtraEventState *state;
ExtraEventState *endState;
s16 *scoreCache;
s32 previousLastScore;
s32 nextScore;
s32 score;
u16 transition;
u16 stateIndex;

states = block->states;
lastState = &states[block->stateCount - 1];
scoreCache = block->scoreCache;
previousLastScore = lastState->score;
nextScore = 0x7FFFFFFF;

for (transition = 1; transition <= block->transitionCount; transition++) {
    endState = &states[block->transitionBounds[transition + 1] - 1];
    state = &states[block->transitionBounds[transition]];
    score = states[0].score + block->transitionScore;

    while (state < endState) {
        s32 currentScore = state->score;

        nextScore = score;
        if (nextScore < currentScore) {
            state->score = nextScore;
        }
        score = currentScore;
        state++;
    }

    if (state->score < previousLastScore) {
        lastState->score = state->score;
    }
    if (nextScore < state->score) {
        state->score = nextScore;
    }
}

if (previousLastScore < states[0].score) {
    states[0].score = lastState->score;
}

state = states;
for (stateIndex = 0; stateIndex < block->stateCount; stateIndex++) {
    score = scoreCache[state->stateId];
    if (score == 0x7FFF) {
        score = block->readStateScore(block->callbackContext);
        scoreCache[state->stateId] = score;
    }

    state->score += (s16)score;
    if (state->score > 0x7FFFF7FF) {
        state->score = 0x7FFFF7FF;
    }
    state++;
}

if (states[0].score < lastState->score) {
    block->bestScore = states[0].score;
} else {
    block->bestScore = lastState->score;
}
}

static u32 ProcessExtraEventDP(
TosBaseBlock *baseBlock, void **inputs, s32 inputCount)
{
ExtraEventDP *block = (ExtraEventDP *)baseBlock;

if (inputs[0] != NULL) {
    block->frameCount++;
    block->accumulatedScore += block->readFrameScore(block->callbackContext);
    DynProgExtraEventsProcess(block);
}

return 0;
}

static u32 InitExtraEventDP(TosBaseBlock *baseBlock)
{
ExtraEventDP *block = (ExtraEventDP *)baseBlock;

block->input = baseBlock->input;
block->input->inputSize = (u16)_tosGetProfileU32(baseBlock, 0x0F, 4);
return 0;
}

static u32 ControlExtraEventDP(
TosBaseBlock *baseBlock, u32 command, void *argument, u32 argumentSize)
{
ExtraEventDP *block = (ExtraEventDP *)baseBlock;
ExtraEventCallbacks *callbacks = (ExtraEventCallbacks *)argument;
ExtraEventContextInfo *contextInfo = (ExtraEventContextInfo *)argument;
TosContext *context;

switch ((u8)command) {
case 1:
    if (argument == NULL) {
        return 1;
    }
    *(s32 *)argument = block->bestScore + block->accumulatedScore;
    break;
case 2:
    InitViterbi(block);
    block->base.process = (TosProcessFunction)ProcessExtraEventDP;
    break;
case 3:
    break;
case 6:
    block->readStateScore = callbacks->readStateScore;
    block->getScoreCache = callbacks->getScoreCache;
    block->readFrameScore = callbacks->readFrameScore;
    block->callbackContext = callbacks->context;
    break;
case 7:
    break;
case 8:
    if (block->states != NULL) {
        heap_Free(block->base.context->heap, block->states);
        block->states = NULL;
    }

    context = block->base.context;
    block->transitionCount = contextInfo->transitionCount;
    block->transitionScore = contextInfo->transitionScore;
    block->initialScore = contextInfo->initialScore;
    block->stateCount = contextInfo->stateCount;
    block->stateIds = contextInfo->stateIds;
    block->transitionBounds = contextInfo->transitionBounds;
    block->scoreCache = block->getScoreCache(block->callbackContext);
    block->states = heap_Alloc(
        context->heap, block->stateCount * sizeof(ExtraEventState));
    if (block->states == NULL) {
        _tosErrorLog(block, 1);
    }
    break;
case 0xFF:
    if (block->states != NULL) {
        heap_Free(block->base.context->heap, block->states);
        block->states = NULL;
    }
    tosBaseBlockDestruct(block);
    break;
default:
    return 0;
}

return 1;
}

void *ConstructExtraEventDp(TosContext *context, u32 blockIndex)
{
return tosBaseBlockConstruct(
    context, blockIndex, 1, 0, (TosProcessFunction)ProcessExtraEventDP,
    (TosInitFunction)InitExtraEventDP,
    (TosControlFunction)ControlExtraEventDP, sizeof(ExtraEventDP));
}
