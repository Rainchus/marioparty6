#include "types.h"

#include <float.h>
#include <math.h>
#include <string.h>

#include "gssdk/langdata.h"
#include "gssdk/tos.h"

typedef struct GenderCodeBook {
    u8 dimension;
    u8 firstCodeBookSize;
    u8 secondSearchCount;
    u8 secondCodeBookSize;
    u16 compressedStart;
    f32 *firstCodeBook;
    u32 *secondCodeBookIndices;
    f32 *secondCodeBook;
} GenderCodeBook;

typedef struct Gender {
    TosBaseBlock base;
    s32 state;
    u32 genderMask;
    u16 selectedGender;
    u8 fixedGender;
    f32 *frameBuffer;
    f32 *frameBufferEnd;
    f32 *outputFrame;
    f32 *inputFrame;
    f32 *scores;
    u32 bufferedFrameCount;
    f32 totalWeight;
    u32 scoreFrameCount;
    f32 outputWeight;
    GenderCodeBook codeBook;
    u8 *enabledGenders;
    u32 inputEnd;
    u32 inputStart;
    u32 genderCount;
    u32 frameBufferCapacity;
    u32 frameSize;
    s32 scoreFrameLimit;
    s32 outputWeightThreshold;
    s32 fixedGenderThreshold;
    LanguageData *language;
} Gender;

typedef struct GenderSession {
    u32 state;
    u32 selectedGender;
    f32 scores[];
} GenderSession;

typedef struct GenderSessionTail {
    u32 bufferedFrameCount;
    f32 totalWeight;
    u32 scoreFrameCount;
    f32 outputWeight;
} GenderSessionTail;

static const u8 ControlID_Disable[] = { 0xD, 0xE, 0xF };

extern void *heap_Alloc(void *heap, u32 size);
extern void *heap_Calloc(void *heap, u32 count, u32 size);
extern void heap_Free(void *heap, void *ptr);

static void ResetGender(Gender *block);

static void GenderGetSession(Gender *block, GenderSession *session)
{
    GenderSessionTail *tail;
    f32 *score;
    u32 i;

    session->state = block->state;
    session->selectedGender = block->selectedGender;
    score = session->scores;
    for (i = 0; i < block->genderCount; i++) {
        *score++ = block->scores[i];
    }

    tail = (GenderSessionTail *)score;
    tail->bufferedFrameCount = block->bufferedFrameCount;
    tail->totalWeight = block->totalWeight;
    tail->scoreFrameCount = block->scoreFrameCount;
    tail->outputWeight = block->outputWeight;
}

static void GenderPutSession(Gender *block, const GenderSession *session)
{
    const GenderSessionTail *tail;
    const f32 *score;
    u32 i;

    block->state = session->state;
    block->selectedGender = session->selectedGender;
    score = session->scores;
    for (i = 0; i < block->genderCount; i++) {
        block->scores[i] = *score++;
    }

    tail = (const GenderSessionTail *)score;
    block->bufferedFrameCount = tail->bufferedFrameCount;
    block->totalWeight = tail->totalWeight;
    block->scoreFrameCount = tail->scoreFrameCount;
    block->outputWeight = tail->outputWeight;
}

static u32 LoadCodeBook(Gender *block, LanguageData *language)
{
    CodeBookData *source;

    if (language->checkBitField(language, 4)) {
        source = language->getpCodeBook(
            language, language->getNbrCodeBook(language));
    } else {
        source = language->getpCodeBook(language, 1);
    }

    block->codeBook.dimension = language->getCodeBookDim(source);
    block->codeBook.firstCodeBookSize = language->getFirstCdbSize(source);
    block->codeBook.secondSearchCount =
        language->getNbrInSecSearch(source);
    block->codeBook.secondCodeBookSize =
        language->getSecondCdbSize(source);
    block->codeBook.compressedStart =
        (u8)language->getCompStart(source);
    block->codeBook.firstCodeBook = language->getpFirstCdb(source);
    block->codeBook.secondCodeBookIndices =
        language->getpIndexInSecCdb(source);
    block->codeBook.secondCodeBook = language->getpSecondCdb(source);

    if (block->codeBook.dimension > block->inputEnd - block->inputStart) {
        return _tosErrorLog(block, 0x66);
    }
    block->language = language;
    return 0;
}

static void GenderFixedGender(Gender *block, u32 genderMask)
{
    u32 *warpFactor;
    u32 matchingGenderCount;
    u16 matchingGender;
    u32 i;

    warpFactor = block->language->getpWarpFactors(block->language);
    matchingGenderCount = 0;
    matchingGender = 0;
    for (i = 0; i < block->genderCount; i++, warpFactor += 2) {
        if (genderMask & *warpFactor) {
            matchingGender = i;
            matchingGenderCount++;
        }
    }

    if ((matchingGenderCount == 1) != block->fixedGender) {
        ResetGender(block);
    }
    if (matchingGenderCount == 1) {
        block->selectedGender = matchingGender + 1;
        if (block->state == 0) {
            block->state = 1;
        }
    }
    block->fixedGender = matchingGenderCount == 1;
    block->genderMask = genderMask;
}

static u32 ControlGender(
    TosBaseBlock *baseBlock, u32 command, void *argument, u32 argumentSize)
{
    Gender *block = (Gender *)baseBlock;
    u32 *sessionOffset = (u32 *)argumentSize;
    u32 result = 1;

    switch ((u8)command) {
    case 1:
        result = (u8)LoadCodeBook(block, argument) == 0;
        break;
    case 2:
        block->enabledGenders[(u32)argument + 1] = 1;
        break;
    case 3:
        if (block->bufferedFrameCount == 0 && argument == NULL) {
            break;
        }
        if (block->selectedGender == 0) {
            block->outputWeight = 1.0f + block->outputWeightThreshold;
        }
        block->state = 1;
        break;
    case 4:
        result = qQueueControl(
            baseBlock->input->queue, 7, baseBlock->input->outputSize,
            argument);
        break;
    case 5:
        result = qQueueControl(
            baseBlock->input->queue, 6, baseBlock->input->outputSize,
            argument);
        break;
    case 6:
        if (argument != NULL) {
            GenderPutSession(
                block, (GenderSession *)((u8 *)argument + *sessionOffset));
            *sessionOffset += (block->genderCount + 6) * sizeof(u32);
        } else {
            ResetGender(block);
        }
        break;
    case 7:
        if (argument != NULL) {
            GenderGetSession(
                block, (GenderSession *)((u8 *)argument + *sessionOffset));
        }
        *sessionOffset += (block->genderCount + 6) * sizeof(u32);
        break;
    case 8:
        GenderFixedGender(block, *(u32 *)argument);
        break;
    case 9:
        *(u32 *)argument = block->genderMask;
        break;
    case 10:
        block->enabledGenders[(u32)argument + 1] = 0;
        break;
    case 0xFF:
        heap_Free(block->base.context->heap, block->scores);
        heap_Free(block->base.context->heap, block->frameBuffer);
        heap_Free(block->base.context->heap, block->enabledGenders);
        tosBaseBlockDestruct(block);
        break;
    default:
        result = 0;
        break;
    }
    return result;
}

static void ResetGender(Gender *block)
{
    u32 i;

    block->state = 0;
    if (block->fixedGender) {
        block->state = 1;
    } else {
        block->selectedGender = 0;
    }
    block->inputFrame = block->frameBuffer;
    block->outputFrame = block->frameBuffer;
    for (i = 0; i < block->genderCount; i++) {
        block->scores[i] = 0.0f;
    }
    block->bufferedFrameCount = 0;
    block->totalWeight = 0.0f;
    block->scoreFrameCount = 0;
    block->outputWeight = 0.0f;
}

static void GenderDataCompute(Gender *block, f32 *input, f32 weight)
{
    f32 *inputStart;
    f32 *inputEnd;
    f32 *codeBookValue;
    f32 *inputValue;
    f32 distance;
    f32 minimum;
    f32 difference;
    f32 smoothing;
    u32 *indices;
    s32 firstCode;
    u32 gender;
    u32 code;

    block->totalWeight += weight;
    block->outputWeight += weight;
    if (block->scoreFrameCount < block->scoreFrameLimit) {
        block->scoreFrameCount++;
    }
    smoothing = 1.0f / block->scoreFrameCount;

    inputStart = input + block->inputStart;
    for (gender = 0; gender < block->genderCount; gender++) {
        if (block->enabledGenders[gender + 1]) {
            minimum = FLT_MAX;
            firstCode = -1;
            inputEnd = inputStart + block->inputEnd - block->inputStart;
            codeBookValue = block->codeBook.firstCodeBook;
            for (code = 0; code < block->codeBook.firstCodeBookSize; code++) {
                distance = 0.0f;
                inputValue = inputStart;
                while (inputValue < inputEnd) {
                    difference = *inputValue++ - *codeBookValue++;
                    distance += difference * difference;
                    if (distance > minimum) {
                        break;
                    }
                }
                if (inputValue == inputEnd) {
                    minimum = distance;
                    firstCode = code;
                } else {
                    codeBookValue += inputEnd - inputValue;
                }
            }

            if (block->codeBook.secondSearchCount != 0) {
                indices = block->codeBook.secondCodeBookIndices +
                    firstCode * block->codeBook.secondSearchCount;
                minimum = FLT_MAX;
                for (code = 0; code < block->codeBook.secondSearchCount;
                     code++) {
                    codeBookValue =
                        block->codeBook.secondCodeBook + *indices++;
                    inputValue = inputStart;
                    distance = 0.0f;
                    while (inputValue < inputEnd) {
                        difference = *inputValue++ - *codeBookValue++;
                        distance += difference * difference;
                        if (distance > minimum) {
                            break;
                        }
                    }
                    if (inputValue == inputEnd) {
                        minimum = distance;
                    }
                }
            }

            block->scores[gender] += smoothing *
                (weight * sqrtf(minimum) - block->scores[gender]);
        }
        inputStart += block->inputEnd;
    }
}

static void ReadFrame(Gender *block, void **inputs, s32 inputCount)
{
    f32 *frame;
    f32 activity;
    u32 input;

    if (inputs[0] == NULL) {
        return;
    }

    frame = block->inputFrame;
    for (input = 1; input <= block->genderCount; input++) {
        if (block->enabledGenders[input]) {
            memcpy(frame, inputs[input], block->inputEnd * sizeof(f32));
        }
        frame += block->inputEnd;
    }

    activity = *(f32 *)inputs[0];
    if (!block->fixedGender && activity > 0.001f) {
        GenderDataCompute(block, block->inputFrame, activity);
    }
    block->bufferedFrameCount++;
    block->inputFrame = frame == block->frameBufferEnd
        ? block->frameBuffer : frame;
}

static u32 WriteFrameToOutput(Gender *block)
{
    u32 bestGender;
    f32 minimum;
    u32 gender;

    bestGender = 0;
    minimum = FLT_MAX;
    for (gender = 0; gender < block->genderCount; gender++) {
        if (block->enabledGenders[gender + 1] &&
            block->scores[gender] < minimum) {
            minimum = block->scores[gender];
            bestGender = gender;
        }
    }

    if (block->totalWeight > block->fixedGenderThreshold &&
        block->totalWeight > 0.001f &&
        (block->genderMask & 0x80000000) != 0) {
        for (gender = 0; gender < block->genderCount; gender++) {
            if (gender != bestGender) {
                _tosControl(
                    block, ControlID_Disable[gender], gender, bestGender);
            }
        }
        switch (bestGender + 1) {
        case 1:
            block->genderMask = 1;
            break;
        case 2:
            block->genderMask = 2;
            break;
        case 3:
            block->genderMask = 4;
            break;
        default:
            return 0;
        }
        block->fixedGender = 1;
        block->outputWeight = 0.0f;
        block->state = 1;
    } else {
        block->outputWeight = 0.0f;
    }
    return bestGender + 1;
}

static void OutputFrame(Gender *block)
{
    f32 *output;

    output = qEnQueueOne(block->base.output->queue);
    if (output == NULL) {
        return;
    }
    if (!block->fixedGender &&
        (block->state == 0 ||
         block->outputWeight > block->outputWeightThreshold)) {
        block->selectedGender = WriteFrameToOutput(block);
    }
    memcpy(
        output,
        block->outputFrame +
            (block->selectedGender - 1) * block->inputEnd,
        block->inputEnd * sizeof(f32));
    block->outputFrame += block->frameSize;
    if (block->outputFrame == block->frameBufferEnd) {
        block->outputFrame = block->frameBuffer;
    }
    block->bufferedFrameCount--;
}

static u32 InitGender(TosBaseBlock *baseBlock)
{
    Gender *block = (Gender *)baseBlock;
    TosContext *context = block->base.context;

    block->genderCount = block->base.inputCount - 1;
    block->inputStart = (u16)_tosGetProfileU32(block, 5, 1);
    block->inputEnd = (u16)_tosGetProfileU32(block, 6, 0);
    block->frameBufferCapacity =
        (u16)_tosGetProfileU32(block, 1, 50);
    block->scoreFrameLimit =
        (s16)_tosGetProfileU32(block, 2, 600);
    block->outputWeightThreshold =
        (s16)_tosGetProfileU32(block, 3, 10);
    block->fixedGenderThreshold =
        (s16)_tosGetProfileU32(block, 4, 20);
    block->frameSize = block->genderCount * block->inputEnd;

    block->frameBuffer = heap_Calloc(
        context->heap,
        block->frameBufferCapacity * block->frameSize, sizeof(f32));
    block->frameBufferEnd = block->frameBuffer +
        block->frameBufferCapacity * block->frameSize;
    block->scores = heap_Alloc(
        context->heap, block->genderCount * sizeof(f32));
    block->enabledGenders = heap_Alloc(
        context->heap, block->genderCount + 1);
    if (block->frameBuffer == NULL || block->scores == NULL ||
        block->enabledGenders == NULL) {
        return 1;
    }

    memset(block->enabledGenders, 1, block->genderCount + 1);
    block->base.input[0].inputSize = sizeof(f32);
    block->base.input[1].inputSize = block->inputEnd * sizeof(f32);
    block->base.input[2].inputSize = block->inputEnd * sizeof(f32);
    block->base.input[3].inputSize = block->inputEnd * sizeof(f32);
    block->base.output->outputSize = block->inputEnd * sizeof(f32);
    block->genderMask = 0x7FFFFFFF;
    ResetGender(block);
    return 0;
}

static void ProcessGender(
    TosBaseBlock *baseBlock, void **inputs, s32 inputCount)
{
    Gender *block = (Gender *)baseBlock;

    switch (block->state) {
    case 0:
        if (block->bufferedFrameCount == block->frameBufferCapacity &&
            block->bufferedFrameCount != 0) {
            OutputFrame(block);
        }
        if (block->bufferedFrameCount < block->frameBufferCapacity) {
            ReadFrame(block, inputs, inputCount);
        }
        if (block->totalWeight > block->fixedGenderThreshold) {
            block->state = 1;
        }
        break;
    case 1:
        if (block->bufferedFrameCount != 0) {
            OutputFrame(block);
        }
        if (inputs[0] != NULL) {
            ReadFrame(block, inputs, inputCount);
        }
        if (block->bufferedFrameCount == 0) {
            block->state = 2;
        }
        break;
    case 2:
        if (block->bufferedFrameCount < block->frameBufferCapacity) {
            ReadFrame(block, inputs, inputCount);
        }
        if (block->bufferedFrameCount != 0) {
            OutputFrame(block);
        }
        break;
    }
}

void *ConstructGender(TosContext *context, u32 blockIndex)
{
    return tosBaseBlockConstruct(
        context, blockIndex, 4, 1, ProcessGender, InitGender,
        ControlGender, sizeof(Gender));
}
