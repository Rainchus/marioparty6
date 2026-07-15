#include "types.h"

#include <stddef.h>
#include <string.h>

#include "gssdk/langdata.h"
#include "gssdk/mqueue.h"
#include "gssdk/tos.h"

enum {
    DPGENUW_WORD_STATE_COUNT = 20,
    DPGENUW_STATE_COUNT = 22,
    DPGENUW_MAX_SCORE = 31622
};

typedef struct DpGenUwBacktrace DpGenUwBacktrace;

struct DpGenUwBacktrace {
    DpGenUwBacktrace *next;
    u16 distribution;
    s16 references;
    u32 frame;
};

typedef struct DpGenUwState {
    u16 distribution;
    s16 score;
    DpGenUwBacktrace *backtrace;
} DpGenUwState;

typedef struct DpGenUwLanguageInfo {
    u32 distributionCount;
    u16 *distributions;
    u16 silenceDistribution;
} DpGenUwLanguageInfo;

typedef struct DpGenUwResult {
    f32 averageScore;
    u32 silenceFrameCount;
    u32 distributionCount;
    u16 distributions[1];
} DpGenUwResult;

typedef struct DpGenUw DpGenUw;
typedef u16 (*DpGenUwActiveDistributionsFunction)(
    DpGenUw *block, u16 *distributions, u8 *active);
typedef void (*DpGenUwResultFunction)(
    DpGenUw *block, void *argument, u32 argumentSize);

struct DpGenUw {
    TosBaseBlock base;
    TosQueuePort *input;
    TosQueuePort *output;
    u16 field30;
    u16 phenomeCount16;
    s16 silenceTransitionPenalty;
    s16 wordTransitionPenalty;
    DpGenUwState *states;
    DpGenUwBacktrace **backtraceChunks;
    DpGenUwBacktrace *freeBacktraces;
    u32 profile2Scale;
    u32 field48;
    u16 ergodicDistributionCount;
    void *ergodicPenalty;
    s16 ergodicDistributionOffset;
    u32 phenomeCount;
    u32 userWordTrainingPhenomeCount;
    u16 userWordDistributionCount;
    u16 *stateDistributions;
    u16 silenceDistribution;
    u32 accumulatedScore;
    u32 finalScore;
    u32 field74;
    s16 bestScore;
    u32 field7C;
    u8 initialized;
    u32 frame;
    u32 maximumFrameCount;
    DpGenUwActiveDistributionsFunction makeActiveDistributions;
    void *field90;
    DpGenUwResultFunction sendResult;
    u8 field98[0x50];
    u32 activeBacktraceCount;
    u32 fieldEC;
    u16 backtraceChunkCount;
    u16 backtraceChunkBytes;
};

extern void *heap_Alloc(void *heap, u32 size);
extern void heap_Free(void *heap, void *ptr);

static DpGenUwBacktrace *AllocateBacktrace(
    DpGenUw *block, DpGenUwBacktrace *lastBacktrace)
{
    u32 oldChunkCount = block->backtraceChunkCount;
    TosContext *context = block->base.context;
    DpGenUwBacktrace **chunks = heap_Alloc(
        context->heap,
        (oldChunkCount + 1) * sizeof(DpGenUwBacktrace *));
    DpGenUwBacktrace *backtraces;
    u32 backtracesPerChunk;

    if (chunks == NULL) {
        _tosErrorLog(block, 2);
        return NULL;
    }

    if (oldChunkCount != 0) {
        memcpy(
            chunks, block->backtraceChunks,
            oldChunkCount * sizeof(DpGenUwBacktrace *));
        heap_Free(context->heap, block->backtraceChunks);
    }
    block->backtraceChunks = chunks;

    backtracesPerChunk =
        block->backtraceChunkBytes / sizeof(DpGenUwBacktrace);
    backtraces = heap_Alloc(
        context->heap,
        backtracesPerChunk * sizeof(DpGenUwBacktrace));
    if (backtraces == NULL) {
        _tosErrorLog(block, 2);
        return NULL;
    }

    if (lastBacktrace != NULL) {
        u32 backtraceIndex;

        lastBacktrace->next = backtraces;
        for (backtraceIndex = 0;
             backtraceIndex < backtracesPerChunk - 1;
             backtraceIndex++) {
            backtraces[backtraceIndex].next =
                &backtraces[backtraceIndex + 1];
        }
        backtraces[backtraceIndex].next = NULL;
    }

    block->backtraceChunks[oldChunkCount] = backtraces;
    block->backtraceChunkCount = oldChunkCount + 1;
    return backtraces;
}

static void FreeBacktrace(DpGenUw *block)
{
    TosContext *context = block->base.context;
    u32 chunkIndex;

    for (chunkIndex = 0; chunkIndex < block->backtraceChunkCount;
         chunkIndex++) {
        heap_Free(context->heap, block->backtraceChunks[chunkIndex]);
    }

    if (block->backtraceChunks != NULL) {
        heap_Free(context->heap, block->backtraceChunks);
    }
    block->backtraceChunks = NULL;
    block->backtraceChunkCount = 0;
}

static DpGenUwBacktrace *InitBacktrace(DpGenUw *block)
{
    u32 backtracesPerChunk =
        block->backtraceChunkBytes / sizeof(DpGenUwBacktrace);
    u32 lastBacktraceIndex = backtracesPerChunk - 1;
    DpGenUwBacktrace *lastBacktrace = NULL;
    u16 chunkIndex;

    for (chunkIndex = 0; chunkIndex < block->backtraceChunkCount;
         chunkIndex++) {
        DpGenUwBacktrace *backtraces =
            block->backtraceChunks[chunkIndex];
        u32 backtraceIndex;

        if (lastBacktrace != NULL) {
            lastBacktrace->next = backtraces;
        }

        for (backtraceIndex = 0; backtraceIndex < lastBacktraceIndex;
             backtraceIndex++) {
            backtraces[backtraceIndex].next =
                &backtraces[backtraceIndex + 1];
        }
        lastBacktrace = &backtraces[backtraceIndex];
    }

    if (lastBacktrace != NULL) {
        lastBacktrace->next = NULL;
    }

    lastBacktrace = block->backtraceChunks[0];
    lastBacktrace->references = 0;
    lastBacktrace->distribution = 0xFFFF;
    block->freeBacktraces = lastBacktrace->next;
    return lastBacktrace;
}

static void InitViterbi(DpGenUw *block)
{
    DpGenUwBacktrace *backtrace;
    DpGenUwState *state;
    u32 stateIndex;

    block->field30 = 0;
    block->field48 = 0;
    block->frame = -1;
    block->bestScore = 0;

    backtrace = InitBacktrace(block);
    backtrace->next = NULL;
    backtrace->distribution = 0;
    backtrace->frame = 0;

    state = block->states;
    for (stateIndex = 0; stateIndex < DPGENUW_WORD_STATE_COUNT;
         stateIndex++) {
        state->score = DPGENUW_MAX_SCORE;
        state->distribution = block->stateDistributions[stateIndex];
        state->backtrace = backtrace;
        state++;
    }

    state->score = 0;
    state->distribution = block->silenceDistribution;
    state->backtrace = backtrace;
    state++;

    state->score = DPGENUW_MAX_SCORE;
    state->distribution = block->silenceDistribution;
    state->backtrace = backtrace;

    backtrace->references = DPGENUW_STATE_COUNT;
    block->accumulatedScore = 0;
    block->activeBacktraceCount = 0;
}

static u32 InitDpGenUw(TosBaseBlock *baseBlock)
{
    DpGenUw *block = (DpGenUw *)baseBlock;
    u16 distributionCount;

    block->profile2Scale =
        (u16)((s32)_tosGetProfileU32(baseBlock, 2, 1) / 10);

    distributionCount = _tosGetProfileU32(baseBlock, 0x13, 1);
    block->input = baseBlock->input;
    block->output = baseBlock->output;
    block->input->inputSize = distributionCount * sizeof(s16);
    block->output->outputSize = distributionCount * 3;

    block->backtraceChunkBytes =
        _tosGetProfileU32(baseBlock, 0x11, 1500);
    if (block->backtraceChunkBytes > 65000) {
        block->backtraceChunkBytes = 65000;
    } else if (block->backtraceChunkBytes < 200) {
        block->backtraceChunkBytes = 200;
    }

    block->maximumFrameCount =
        (u16)_tosGetProfileU32(baseBlock, 0x0B, 0) * 100;
    block->wordTransitionPenalty =
        (s16)_tosGetProfileU32(baseBlock, 0x14, 120);
    block->silenceTransitionPenalty =
        (s16)_tosGetProfileU32(baseBlock, 0x23, 900);
    block->backtraceChunkCount = 0;
    return 0;
}

static void SendResultUserWords(
    DpGenUw *block, void *argument, u32 argumentSize)
{
    DpGenUwBacktrace *backtrace =
        block->states[DPGENUW_STATE_COUNT - 1].backtrace;
    DpGenUwBacktrace *currentBacktrace = backtrace;
    u32 lastFrame = backtrace->frame;
    u32 silenceFrameCount = 0;
    u16 backtraceCount = 0;
    s32 backtraceIndex;
    DpGenUwResult *result;

    while (currentBacktrace->next != NULL) {
        DpGenUwBacktrace *nextBacktrace = currentBacktrace->next;

        if (currentBacktrace->distribution ==
            block->silenceDistribution) {
            silenceFrameCount += lastFrame - currentBacktrace->frame;
            lastFrame = 0;
        } else if (lastFrame == 0) {
            lastFrame = currentBacktrace->frame;
        }

        currentBacktrace = nextBacktrace;
        backtraceCount++;
    }

    if (backtraceCount != 0) {
        backtraceCount--;
    }

    result = heap_Alloc(
        block->base.context->heap,
        sizeof(DpGenUwResult) +
            (backtraceCount - 1) * sizeof(u16));
    if (result == NULL) {
        _tosErrorLog(block, 1);
        return;
    }

    result->silenceFrameCount = silenceFrameCount;
    result->distributionCount = backtraceCount;
    if (block->frame != 0) {
        result->averageScore =
            (f32)(block->accumulatedScore + block->finalScore) /
            (f32)block->frame;
    } else {
        result->averageScore = 0.0f;
    }

    currentBacktrace = backtrace;
    for (backtraceIndex = backtraceCount - 1;
         backtraceIndex >= 0;
         backtraceIndex--) {
        result->distributions[backtraceIndex] =
            currentBacktrace->distribution;
        currentBacktrace = currentBacktrace->next;
    }

    if (_tosControl(block, 0x0C, (u32)result, 1)) {
        _tosErrorLog(block, 1);
    }
    heap_Free(block->base.context->heap, result);
}

static u16 MakeUserWordsActiveDist(
    DpGenUw *block, u16 *distributions, u8 *active)
{
    distributions[0] = block->phenomeCount;
    return distributions[0];
}

static inline void ReleaseBacktrace(
    DpGenUw *block, DpGenUwBacktrace *backtrace)
{
    backtrace->references--;
    if (backtrace->references <= 0) {
        DpGenUwBacktrace *firstBacktrace = backtrace;
        DpGenUwBacktrace *lastBacktrace = backtrace;
        DpGenUwBacktrace *sharedBacktrace = backtrace->next;

        block->activeBacktraceCount--;
        while (sharedBacktrace->references <= 1) {
            lastBacktrace = sharedBacktrace;
            sharedBacktrace = sharedBacktrace->next;
            block->activeBacktraceCount--;
        }
        sharedBacktrace->references--;
        lastBacktrace->next = block->freeBacktraces;
        block->freeBacktraces = firstBacktrace;
    }
}

static u32 DynProgUserWords(DpGenUw *block, s16 *scores)
{
    DpGenUwState *state;
    DpGenUwState *bestWordState;
    DpGenUwState *worstWordState;
    DpGenUwState *entryState;
    DpGenUwBacktrace *transitionBacktrace;
    DpGenUwBacktrace *finalBacktrace;
    DpGenUwBacktrace *newBacktrace;
    u16 bestWordDistribution;
    u16 entryDistribution;
    s16 normalization;
    s16 silenceScore;
    s32 wordEntryScore;
    s32 entryScore;
    u32 stateIndex;

    state = block->states;
    for (stateIndex = 1; stateIndex < DPGENUW_STATE_COUNT;
         stateIndex++) {
        if (state->score > DPGENUW_MAX_SCORE) {
            state->score = DPGENUW_MAX_SCORE;
        }
        state++;
    }

    bestWordState = block->states;
    state = &block->states[1];
    for (stateIndex = 1; stateIndex < DPGENUW_WORD_STATE_COUNT;
         stateIndex++) {
        if (state->score < bestWordState->score) {
            bestWordState = state;
        }
        state++;
    }

    state = &block->states[DPGENUW_WORD_STATE_COUNT];
    normalization = bestWordState->score;
    bestWordDistribution = bestWordState->distribution;
    wordEntryScore =
        bestWordState->score + block->wordTransitionPenalty;

    if (state->score < normalization) {
        normalization = state->score;
    }

    entryScore = state->score + block->silenceTransitionPenalty;
    if (entryScore < wordEntryScore) {
        entryDistribution = state->distribution;
        entryState = state;
    } else {
        entryScore = wordEntryScore;
        entryDistribution = bestWordDistribution;
        entryState = bestWordState;
    }

    silenceScore = scores[state->distribution];
    if (state[1].score < normalization) {
        normalization = state[1].score;
    }
    if (state[1].score + block->silenceTransitionPenalty < entryScore) {
        entryScore =
            state[1].score + block->silenceTransitionPenalty;
        entryDistribution = state[1].distribution;
        entryState = &state[1];
    }

    newBacktrace = block->freeBacktraces;
    if (newBacktrace->next == NULL) {
        block->freeBacktraces =
            AllocateBacktrace(block, newBacktrace);
    } else {
        block->freeBacktraces = newBacktrace->next;
    }
    newBacktrace->next = entryState->backtrace;
    entryState->backtrace->references++;
    newBacktrace->distribution = entryDistribution;
    newBacktrace->references = 0;
    newBacktrace->frame = block->frame;
    block->activeBacktraceCount++;
    newBacktrace->references++;
    transitionBacktrace = newBacktrace;

    if (bestWordDistribution != 0) {
        newBacktrace = block->freeBacktraces;
        if (newBacktrace->next == NULL) {
            block->freeBacktraces =
                AllocateBacktrace(block, newBacktrace);
        } else {
            block->freeBacktraces = newBacktrace->next;
        }
        newBacktrace->next = bestWordState->backtrace;
        bestWordState->backtrace->references++;
        newBacktrace->distribution = bestWordDistribution;
        newBacktrace->references = 0;
        newBacktrace->frame = block->frame;
        block->activeBacktraceCount++;
        newBacktrace->references++;
        finalBacktrace = newBacktrace;
    } else {
        finalBacktrace = transitionBacktrace;
        finalBacktrace->references++;
    }

    state = block->states;
    for (stateIndex = 0; stateIndex < DPGENUW_WORD_STATE_COUNT;
         stateIndex++) {
        if (state->score < entryScore) {
            state->score =
                state->score + scores[state->distribution] -
                normalization;
        } else {
            DpGenUwBacktrace *oldBacktrace = state->backtrace;

            state->score =
                entryScore + scores[state->distribution] -
                normalization;
            state->backtrace = transitionBacktrace;
            transitionBacktrace->references++;
            ReleaseBacktrace(block, oldBacktrace);
        }

        scores[state->distribution] = 1024;
        state++;
    }

    state->score = state->score + silenceScore - normalization;
    if (state[1].score <= wordEntryScore) {
        state[1].score =
            state[1].score + silenceScore - normalization;
    } else {
        DpGenUwBacktrace *oldBacktrace = state[1].backtrace;

        state[1].score =
            wordEntryScore + silenceScore - normalization;
        state[1].backtrace = finalBacktrace;
        finalBacktrace->references++;
        ReleaseBacktrace(block, oldBacktrace);
    }
    block->finalScore = (u16)state[1].score;

    worstWordState = block->states;
    state = &block->states[1];
    for (stateIndex = 1; stateIndex < DPGENUW_WORD_STATE_COUNT;
         stateIndex++) {
        if (state->score >= worstWordState->score) {
            worstWordState = state;
        }
        state++;
    }

    for (stateIndex = 0;
         stateIndex < block->userWordDistributionCount;
         stateIndex++) {
        u16 distribution = block->stateDistributions[stateIndex];
        s32 candidateScore =
            entryScore + scores[distribution] - normalization;

        if (candidateScore < worstWordState->score) {
            DpGenUwBacktrace *oldBacktrace =
                worstWordState->backtrace;

            worstWordState->score = candidateScore;
            worstWordState->distribution = distribution;
            worstWordState->backtrace = transitionBacktrace;
            transitionBacktrace->references++;
            ReleaseBacktrace(block, oldBacktrace);

            worstWordState = block->states;
            state = &block->states[1];
            {
                u32 worstStateIndex;

                for (worstStateIndex = 1;
                     worstStateIndex < DPGENUW_WORD_STATE_COUNT;
                     worstStateIndex++) {
                    if (state->score >= worstWordState->score) {
                        worstWordState = state;
                    }
                    state++;
                }
            }
        }
    }

    ReleaseBacktrace(block, transitionBacktrace);
    ReleaseBacktrace(block, finalBacktrace);
    block->accumulatedScore += (u16)normalization;
    return 0;
}

static void IdleDpGenUwProcess(
    TosBaseBlock *baseBlock, void **inputs, s32 inputCount)
{
}

static void DpGenUwProcess(
    TosBaseBlock *baseBlock, void **inputs, s32 inputCount)
{
    DpGenUw *block = (DpGenUw *)baseBlock;
    s16 *scores = inputs[0];

    if (scores != NULL) {
        s16 *score = scores;
        s32 distributionIndex;

        for (distributionIndex = 0;
             distributionIndex < block->ergodicDistributionCount;
             distributionIndex++) {
            if (distributionIndex != block->silenceDistribution) {
                *score = 1020;
            }
            score++;
        }

        block->frame++;
        if (block->frame == 0) {
            DpGenUwState *state =
                &block->states[DPGENUW_WORD_STATE_COUNT];

            state->score = scores[state->distribution];
            block->bestScore = state->score;
        } else if (DynProgUserWords(block, scores) != 0) {
            _tosErrorLog(block, 0x77);
            block->base.process = IdleDpGenUwProcess;
            _tosControl(block, 0x0B, 0, 0);
        }

        {
            u8 *output = qEnQueueOne(block->output->queue);

            block->makeActiveDistributions(
                block, (u16 *)output,
                output + block->input->inputSize - sizeof(u16));
        }

        if (block->maximumFrameCount != 0 &&
            block->frame > block->maximumFrameCount) {
            block->base.process = IdleDpGenUwProcess;
            if (_tosControl(block, 8, block->frame, 0)) {
                _tosErrorLog(block, 1);
            }
        }
    }
}

static u32 ControlDpGenUw(
    TosBaseBlock *baseBlock, u32 command, void *argument,
    u32 argumentSize)
{
    DpGenUw *block = (DpGenUw *)baseBlock;

    switch ((u8)command) {
    case 3:
        InitViterbi(block);
        qQueueReset(block->output->queue);
        {
            u8 *output = qEnQueueOne(block->output->queue);

            block->makeActiveDistributions(
                block, (u16 *)output,
                output + block->input->inputSize);
        }
        block->base.process = DpGenUwProcess;
        break;
    case 4:
        block->base.process = IdleDpGenUwProcess;
        block->sendResult(block, argument, argumentSize);
        break;
    case 5:
        block->base.process = IdleDpGenUwProcess;
        break;
    case 0x15:
        break;
    case 0x17:
        {
            DpGenUwLanguageInfo *languageInfo = argument;
            TosContext *context = block->base.context;
            u8 error;

            block->userWordDistributionCount =
                languageInfo->distributionCount;
            block->stateDistributions = languageInfo->distributions;
            block->silenceDistribution =
                languageInfo->silenceDistribution;
            block->input->inputSize =
                block->input->queue->elementSize;
            block->phenomeCount16 = block->phenomeCount;

            FreeBacktrace(block);
            if (block->states != NULL) {
                heap_Free(context->heap, block->states);
            }
            block->states = NULL;

            if (AllocateBacktrace(block, NULL) == NULL) {
                error = _tosErrorLog(block, 0x71);
            } else {
                block->states = heap_Alloc(
                    context->heap,
                    DPGENUW_STATE_COUNT * sizeof(DpGenUwState));
                if (block->states == NULL) {
                    error = _tosErrorLog(block, 0x71);
                } else {
                    error = 0;
                }
            }

            if (error) {
                return 0;
            }

            block->initialized = 0;
            InitViterbi(block);
        }
        break;
    case 0x1A:
        block->wordTransitionPenalty = ((s32 *)argument)[3];
        block->silenceTransitionPenalty = ((s32 *)argument)[2];
        break;
    case 0x1B:
        ((s32 *)argument)[3] = block->wordTransitionPenalty;
        ((s32 *)argument)[2] = block->silenceTransitionPenalty;
        break;
    case 0x22:
        {
            LanguageData *language = argument;

            block->ergodicDistributionCount =
                language->getNbrErgodicPhenomes(language);
            block->ergodicPenalty =
                language->getpErgodicPenalty(language);
            block->ergodicDistributionOffset =
                -block->ergodicDistributionCount;
            block->phenomeCount =
                language->getNbrPhenome(language);
            block->userWordTrainingPhenomeCount =
                language->getNbrPhenUserWordTraining(language);
            if (block->output->queue != NULL) {
                block->output->outputSize =
                    block->output->queue->elementSize;
            }
        }
        break;
    case 0xFF:
        {
            TosContext *context = block->base.context;

            FreeBacktrace(block);
            if (block->states != NULL) {
                heap_Free(context->heap, block->states);
            }
            block->states = NULL;
            tosBaseBlockDestruct(block);
        }
        break;
    default:
        return 0;
    }

    return 1;
}

void *ConstructDpGenUw(TosContext *context, u32 blockIndex)
{
    DpGenUw *block = (DpGenUw *)tosBaseBlockConstruct(
        context, blockIndex, 1, 1, IdleDpGenUwProcess,
        InitDpGenUw, ControlDpGenUw, sizeof(DpGenUw));

    block->makeActiveDistributions = MakeUserWordsActiveDist;
    block->sendResult = SendResultUserWords;
    return block;
}
