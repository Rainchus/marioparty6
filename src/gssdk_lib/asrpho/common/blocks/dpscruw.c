#include "types.h"

#include <stddef.h>
#include <string.h>

#include "gssdk/langdata.h"
#include "gssdk/mqueue.h"
#include "gssdk/tos.h"

typedef struct DpScoreUwBacktrace DpScoreUwBacktrace;

struct DpScoreUwBacktrace {
    s32 references;
    DpScoreUwBacktrace *next;
    u32 score;
    u32 garbageScore;
    u16 distribution;
    u16 reserved12;
    u32 frame;
};

typedef struct DpScoreUwState {
    s16 score;
    u16 reserved02;
    DpScoreUwBacktrace *backtrace;
    u16 distribution;
    u16 reserved0A;
} DpScoreUwState;

typedef struct DpScoreUwLanguageInfo {
    u32 wordStateCount;
    u32 silenceStateCount;
    u16 *stateDistributions;
    u16 *silenceDistribution;
} DpScoreUwLanguageInfo;

typedef struct DpScoreUwResult {
    u32 score;
    u32 scoreDelta;
    u32 frameCount;
    f32 averageScore;
    s32 speechScore;
    u32 firstFrame;
    u32 lastFrame;
    u32 backtraceCount;
    DpScoreUwBacktrace *backtrace;
} DpScoreUwResult;

typedef struct DpScoreUw DpScoreUw;
typedef u16 (*DpScoreUwActiveDistributionsFunction)(
    DpScoreUw *block, u16 *distributions, u8 *active);
typedef void (*DpScoreUwResultFunction)(
    DpScoreUw *block, void *argument, u32 argumentSize);

struct DpScoreUw {
    TosBaseBlock base;
    TosQueuePort *input;
    TosQueuePort *output;
    DpScoreUwState *states;
    u16 backtraceChunkBytes;
    u16 reserved36;
    u32 backtraceChunkCount;
    DpScoreUwBacktrace **backtraceChunks;
    DpScoreUwBacktrace *freeBacktraces;
    u16 wordStateCount;
    u16 stateCount;
    u16 silenceStateCount;
    u16 distributionCount;
    u16 ergodicDistributionCount;
    u16 reserved4E;
    u16 *ergodicDistributions;
    u16 *stateDistributions;
    u16 silenceDistribution;
    u16 singleWordGarbageDistribution;
    u32 accumulatedScore;
    u32 accumulatedGarbageScore;
    s16 finalScore;
    s16 bestScore;
    u8 allowSilenceSkip;
    u8 initialized;
    u16 reserved6A;
    u32 frame;
    u32 maximumFrameCount;
    DpScoreUwActiveDistributionsFunction makeActiveDistributions;
    void *reserved78;
    DpScoreUwResultFunction sendResult;
};

extern void *heap_Alloc(void *heap, u32 size);
extern void heap_Free(void *heap, void *ptr);

static DpScoreUwBacktrace *AllocateBacktrace(
    DpScoreUw *block, DpScoreUwBacktrace *lastBacktrace);

static DpScoreUwBacktrace *InitBacktrace(DpScoreUw *block)
{
    u32 backtracesPerChunk =
        block->backtraceChunkBytes / sizeof(DpScoreUwBacktrace);
    u32 lastBacktraceIndex = backtracesPerChunk - 1;
    DpScoreUwBacktrace *lastBacktrace = NULL;
    u32 chunkIndex;

    for (chunkIndex = 0; chunkIndex < block->backtraceChunkCount;
         chunkIndex++) {
        DpScoreUwBacktrace *backtraces =
            block->backtraceChunks[chunkIndex];
        u32 backtraceIndex;

        if (lastBacktrace != NULL) {
            lastBacktrace->next = backtraces;
        }
        lastBacktrace = NULL;

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
    lastBacktrace->score = 0;
    lastBacktrace->frame = 0;
    lastBacktrace->references = 0;
    lastBacktrace->distribution = 0;
    block->freeBacktraces = lastBacktrace->next;
    return lastBacktrace;
}

static DpScoreUwBacktrace *AllocateBacktrace(
    DpScoreUw *block, DpScoreUwBacktrace *lastBacktrace)
{
    u32 oldChunkCount = block->backtraceChunkCount;
    TosContext *context = block->base.context;
    DpScoreUwBacktrace **chunks = heap_Alloc(
        context->heap,
        (oldChunkCount + 1) * sizeof(DpScoreUwBacktrace *));
    DpScoreUwBacktrace *backtraces;
    u32 backtracesPerChunk;

    if (chunks == NULL) {
        _tosErrorLog(block, 2);
        return NULL;
    }

    if (oldChunkCount != 0) {
        memcpy(
            chunks, block->backtraceChunks,
            oldChunkCount * sizeof(DpScoreUwBacktrace *));
        heap_Free(context->heap, block->backtraceChunks);
    }
    block->backtraceChunks = chunks;

    backtracesPerChunk =
        block->backtraceChunkBytes / sizeof(DpScoreUwBacktrace);
    backtraces = heap_Alloc(
        context->heap,
        backtracesPerChunk * sizeof(DpScoreUwBacktrace));
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

static void FreeBacktrace(DpScoreUw *block)
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

static DpScoreUwBacktrace *GetBacktrace(DpScoreUw *block)
{
    DpScoreUwState *state = &block->states[block->stateCount - 1];
    DpScoreUwBacktrace *backtrace = state->backtrace;

    if (block->silenceStateCount == 0) {
        DpScoreUwBacktrace *newBacktrace = block->freeBacktraces;
        s16 finalScore = block->finalScore;

        if (newBacktrace->next == NULL) {
            block->freeBacktraces = AllocateBacktrace(block, newBacktrace);
        } else {
            block->freeBacktraces = newBacktrace->next;
        }

        newBacktrace->next = backtrace;
        backtrace->references++;
        backtrace = newBacktrace;
        newBacktrace->distribution = state->distribution;
        newBacktrace->references = 0;
        newBacktrace->frame = block->frame;
        newBacktrace->score = block->accumulatedScore + finalScore;
        newBacktrace->garbageScore = block->accumulatedGarbageScore;
        newBacktrace->frame++;
    }

    return backtrace;
}

static void InitViterbi(DpScoreUw *block)
{
    DpScoreUwBacktrace *backtrace;
    DpScoreUwState *state;
    u32 stateIndex;
    u32 referenceCount = 0;

    block->frame = -1;
    block->bestScore = 0;
    block->accumulatedScore = 0;
    block->accumulatedGarbageScore = 0;

    backtrace = InitBacktrace(block);
    backtrace->next = NULL;
    backtrace->distribution = 0;
    backtrace->references = 0;

    state = block->states;
    for (stateIndex = 0; stateIndex < block->silenceStateCount;
         stateIndex++) {
        state->distribution = block->silenceDistribution;
        if (stateIndex == 0) {
            state->score = 0;
        } else {
            state->score = 0x77FF;
        }
        state->backtrace = backtrace;
        state++;
        referenceCount++;
    }

    for (stateIndex = 0; stateIndex < block->wordStateCount;
         stateIndex++) {
        state->distribution = block->stateDistributions[stateIndex];
        state->score = 0x77FF;
        state->backtrace = backtrace;
        state++;
        referenceCount++;
    }

    for (stateIndex = 0; stateIndex < block->silenceStateCount;
         stateIndex++) {
        state->distribution = block->silenceDistribution;
        state->score = 0x77FF;
        state->backtrace = backtrace;
        state++;
        referenceCount++;
    }

    if (block->silenceStateCount == 0) {
        block->states[0].score = 0;
    }
    backtrace->references = referenceCount;
}

static u32 InitDpScoreUw(TosBaseBlock *baseBlock)
{
    DpScoreUw *block = (DpScoreUw *)baseBlock;
    u16 distributionCount =
        _tosGetProfileU32(baseBlock, 0x13, 1);

    block->input = baseBlock->input;
    block->output = baseBlock->output;
    block->input->inputSize = distributionCount * sizeof(s16);
    block->output->outputSize = distributionCount * 3;

    block->backtraceChunkBytes =
        _tosGetProfileU32(baseBlock, 0x11, 400);
    if (block->backtraceChunkBytes > 65000) {
        block->backtraceChunkBytes = 65000;
    } else if (block->backtraceChunkBytes < 200) {
        block->backtraceChunkBytes = 200;
    }

    block->maximumFrameCount = 0;
    block->allowSilenceSkip = 0;
    return 0;
}

static void SendResultScoreUserWords(
    DpScoreUw *block, void *argument, u32 argumentSize)
{
    DpScoreUwBacktrace *backtrace = GetBacktrace(block);
    DpScoreUwBacktrace *currentBacktrace = backtrace;
    DpScoreUwBacktrace *lastBacktrace;
    u32 lastFrame;
    u32 lastScore;
    s32 speechScore = 0;
    s32 speechFrames = 0;
    u32 backtraceCount = 0;
    u32 firstFrame = 0;
    BOOL inSilence = FALSE;
    DpScoreUwResult result;

    if (block->silenceStateCount == 0) {
        currentBacktrace = NULL;
        lastFrame = block->frame + 1;
        lastScore = block->accumulatedScore + block->finalScore;
    } else {
        lastFrame = backtrace->frame;
        lastScore = backtrace->score;
        inSilence = TRUE;
        backtraceCount = 1;

        while (currentBacktrace->next != NULL) {
            DpScoreUwBacktrace *nextBacktrace = currentBacktrace->next;

            if (currentBacktrace->distribution ==
                    block->silenceDistribution &&
                !inSilence) {
                inSilence = TRUE;
                speechScore -=
                    currentBacktrace->score -
                    currentBacktrace->garbageScore;
                speechFrames -= currentBacktrace->frame;
            } else if (currentBacktrace->distribution !=
                           block->silenceDistribution &&
                       inSilence) {
                inSilence = FALSE;
                speechScore +=
                    currentBacktrace->score -
                    currentBacktrace->garbageScore;
                speechFrames += currentBacktrace->frame;
            }

            lastBacktrace = currentBacktrace;
            currentBacktrace = nextBacktrace;
            backtraceCount++;
        }

        lastScore -= lastBacktrace->score;
        firstFrame = lastBacktrace->frame;
    }

    result.frameCount = lastFrame - firstFrame;
    result.scoreDelta = lastScore;
    if (result.frameCount != 0) {
        result.averageScore =
            (f32)result.scoreDelta / (f32)result.frameCount;
    } else {
        result.averageScore = 0.0f;
    }

    result.score = block->accumulatedScore + block->finalScore;
    result.firstFrame = firstFrame;
    result.lastFrame = lastFrame;
    result.backtraceCount = backtraceCount;
    result.backtrace = backtrace;
    if (speechFrames == 0) {
        result.speechScore = 100;
    } else {
        result.speechScore = speechScore / speechFrames;
    }

    if (_tosControl(block, 0x0C, (u32)&result, 2)) {
        _tosErrorLog(block, 1);
    }
}

static u16 MakeScoreUserWordsActiveDist(
    DpScoreUw *block, u16 *distributions, u8 *active)
{
    DpScoreUwState *state = block->states;
    u16 activeCount;
    u32 stateIndex;

    memset(active, 0, block->distributionCount);
    activeCount = block->ergodicDistributionCount;
    memset(active, 1, activeCount);

    for (stateIndex = 0; stateIndex < block->stateCount; stateIndex++) {
        u16 distribution = state->distribution;

        if (active[distribution] == 0) {
            active[distribution] = 1;
            activeCount++;
            distributions[activeCount] = distribution;
        }
        state++;
    }

    distributions[0] = activeCount;
    return activeCount;
}

static inline void ReleaseBacktrace(
    DpScoreUw *block, DpScoreUwBacktrace *backtrace)
{
    backtrace->references--;
    if (backtrace->references <= 0) {
        DpScoreUwBacktrace *firstBacktrace = backtrace;
        DpScoreUwBacktrace *lastBacktrace = backtrace;
        DpScoreUwBacktrace *sharedBacktrace = backtrace->next;

        while (sharedBacktrace->references <= 1) {
            lastBacktrace = sharedBacktrace;
            sharedBacktrace = sharedBacktrace->next;
        }
        sharedBacktrace->references--;
        lastBacktrace->next = block->freeBacktraces;
        block->freeBacktraces = firstBacktrace;
    }
}

static s32 DynProgScoreUserWords(DpScoreUw *block, s16 *scores)
{
    DpScoreUwState *state;
    DpScoreUwState *previousState;
    DpScoreUwBacktrace *previousBacktrace;
    DpScoreUwBacktrace *silenceBacktrace;
    s16 previousScore;
    s16 silenceScore;
    s16 bestScore = 0x7FFF;
    s16 normalization = block->bestScore;
    u32 stateIndex;

    block->accumulatedScore += normalization;
    block->accumulatedGarbageScore +=
        scores[block->singleWordGarbageDistribution];

    state = block->states;
    for (stateIndex = 0; stateIndex < block->stateCount; stateIndex++) {
        state->score -= normalization;
        if (state->score > 0x77FF) {
            state->score = 0x77FF;
        }
        state++;
    }

    state = block->states;
    previousScore = state->score;
    previousBacktrace = state->backtrace;
    previousBacktrace->references++;
    state->score += scores[state->distribution];
    if (state->score < bestScore) {
        bestScore = state->score;
    }

    previousState = state;
    silenceScore = state->score;
    silenceBacktrace = state->backtrace;
    state++;

    for (stateIndex = 1; stateIndex < block->stateCount; stateIndex++) {
        s16 transitionScore = previousScore;
        DpScoreUwBacktrace *transitionBacktrace = previousBacktrace;
        s16 oldScore = state->score;
        DpScoreUwBacktrace *oldBacktrace = state->backtrace;
        s16 distributionScore;

        oldBacktrace->references++;
        distributionScore = scores[state->distribution];

        if (block->allowSilenceSkip &&
            state->distribution == block->silenceDistribution &&
            silenceScore < transitionScore &&
            silenceScore < oldScore) {
            state->score = silenceScore + distributionScore;
            if (state->score < bestScore) {
                bestScore = state->score;
            }

            ReleaseBacktrace(block, state->backtrace);
            state->backtrace = silenceBacktrace;
            silenceBacktrace->references++;
        } else if (transitionScore < oldScore) {
            DpScoreUwBacktrace *newBacktrace;

            state->score = transitionScore + distributionScore;
            if (state->score < bestScore) {
                bestScore = state->score;
            }

            newBacktrace = block->freeBacktraces;
            if (newBacktrace->next == NULL) {
                block->freeBacktraces =
                    AllocateBacktrace(block, newBacktrace);
            } else {
                block->freeBacktraces = newBacktrace->next;
            }

            newBacktrace->next = transitionBacktrace;
            transitionBacktrace->references++;
            newBacktrace->distribution = previousState->distribution;
            newBacktrace->references = 0;
            newBacktrace->frame = block->frame;
            newBacktrace->score =
                block->accumulatedScore + transitionScore;
            newBacktrace->garbageScore =
                block->accumulatedGarbageScore;
            newBacktrace->references++;

            ReleaseBacktrace(block, state->backtrace);
            state->backtrace = newBacktrace;
        } else {
            state->score += distributionScore;
            if (state->score < bestScore) {
                bestScore = state->score;
            }
        }

        if (state->distribution == block->silenceDistribution) {
            silenceScore = state->score;
            silenceBacktrace = state->backtrace;
        }

        ReleaseBacktrace(block, transitionBacktrace);
        previousState = state;
        previousScore = oldScore;
        previousBacktrace = oldBacktrace;
        state++;
    }

    ReleaseBacktrace(block, previousBacktrace);
    block->bestScore = bestScore;
    block->finalScore = previousState->score;
    return 0;
}

static void IdleDpScoreUwProcess(
    TosBaseBlock *baseBlock, void **inputs, s32 inputCount)
{
}

static void DpScoreUwProcess(
    TosBaseBlock *baseBlock, void **inputs, s32 inputCount)
{
    DpScoreUw *block = (DpScoreUw *)baseBlock;
    s16 *scores = inputs[0];

    if (scores != NULL) {
        block->frame++;
        if (block->frame == 0) {
            DpScoreUwState *state = block->states;

            state->score = scores[state->distribution];
            block->bestScore = state->score;
        } else if (DynProgScoreUserWords(block, scores) != 0) {
            _tosErrorLog(block, 0x77);
            block->base.process = IdleDpScoreUwProcess;
            if (_tosControl(block, 0x0B, 0, 0)) {
                block->base.process = IdleDpScoreUwProcess;
                return;
            }
        }

        {
            u8 *output = qEnQueueOne(block->output->queue);
            block->makeActiveDistributions(
                block, (u16 *)output,
                output + block->input->inputSize - sizeof(u16));
        }

        if (block->maximumFrameCount != 0 &&
            block->frame > block->maximumFrameCount) {
            block->base.process = IdleDpScoreUwProcess;
            if (_tosControl(block, 8, block->frame, 0)) {
                _tosErrorLog(block, 1);
            }
        }
    }
}

static u32 InitRecognitionLanguage(
    DpScoreUw *block, DpScoreUwLanguageInfo *languageInfo)
{
    TosContext *context;
    u32 error;

    block->wordStateCount = languageInfo->wordStateCount;
    block->stateDistributions = languageInfo->stateDistributions;
    block->silenceStateCount = languageInfo->silenceStateCount;

    if (block->silenceStateCount > 1) {
        _tosErrorLog(block, 1);
        return 1;
    }

    block->frame = -1;
    block->bestScore = 0;
    block->accumulatedScore = 0;
    block->accumulatedGarbageScore = 0;

    if (block->silenceStateCount != 0) {
        block->silenceDistribution =
            languageInfo->silenceDistribution[0];
        block->stateCount = block->wordStateCount + 2;
    } else {
        block->silenceDistribution = 0;
        block->stateCount = block->wordStateCount;
    }

    context = block->base.context;
    FreeBacktrace(block);
    if (block->states != NULL) {
        heap_Free(context->heap, block->states);
    }
    block->states = NULL;

    if (AllocateBacktrace(block, NULL) == NULL) {
        return 1;
    }

    block->states = heap_Alloc(
        context->heap, block->stateCount * sizeof(DpScoreUwState));
    if (block->states == NULL) {
        error = _tosErrorLog(block, 0x71);
    } else {
        error = 0;
    }
    if (error) {
        return 1;
    }

    block->initialized = 0;
    block->input->inputSize = block->input->queue->elementSize;
    block->output->outputSize = block->output->queue->elementSize;
    return 0;
}

static u32 ControlDpScoreUw(
    TosBaseBlock *baseBlock, u32 command, void *argument,
    u32 argumentSize)
{
    DpScoreUw *block = (DpScoreUw *)baseBlock;

    switch ((u8)command) {
    case 1:
        InitViterbi(block);
        qQueueReset(block->output->queue);
        {
            u8 *output = qEnQueueOne(block->output->queue);
            block->makeActiveDistributions(
                block, (u16 *)output,
                output + block->input->inputSize);
        }
        block->base.process = DpScoreUwProcess;
        break;
    case 2:
        block->base.process = IdleDpScoreUwProcess;
        block->sendResult(block, argument, argumentSize);
        break;
    case 3:
        block->base.process = IdleDpScoreUwProcess;
        break;
    case 4:
        if (InitRecognitionLanguage(
                block, (DpScoreUwLanguageInfo *)argument)) {
            return 0;
        }
        break;
    case 5:
        block->allowSilenceSkip = (u32)argument;
        break;
    case 6:
        *(u8 *)argument = block->allowSilenceSkip;
        break;
    case 7:
        {
            LanguageData *language = argument;

            block->distributionCount =
                language->getNbrPhenome(language);
            block->ergodicDistributionCount =
                language->getNbrErgodicPhenomes(language);
            block->ergodicDistributions =
                language->getpErgodicPhenomes(language);
            block->singleWordGarbageDistribution =
                language->getSingleWordGarbagePhenome(language);
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

void *ConstructDpScoreUw(TosContext *context, u32 blockIndex)
{
    DpScoreUw *block = (DpScoreUw *)tosBaseBlockConstruct(
        context, blockIndex, 1, 1, IdleDpScoreUwProcess,
        InitDpScoreUw, ControlDpScoreUw, sizeof(DpScoreUw));

    block->makeActiveDistributions = MakeScoreUserWordsActiveDist;
    block->sendResult = SendResultScoreUserWords;
    return block;
}
