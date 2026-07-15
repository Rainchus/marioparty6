#include "types.h"

#include <float.h>

#include "gssdk/langdata.h"
#include "gssdk/tos.h"

typedef struct VqCodeBook {
    u8 dimension;
    u8 firstCodeBookSize;
    u8 secondSearchCount;
    u8 secondCodeBookSize;
    u16 compressedStart;
    u16 reserved06;
    f32 *firstCodeBook;
    u32 *secondCodeBookIndices;
    f32 *secondCodeBook;
} VqCodeBook;

typedef struct Vq1500 {
    TosBaseBlock base;
    u8 codeBookCount;
    u8 reserved29[3];
    VqCodeBook *codeBooks;
} Vq1500;

extern void *heap_Alloc(void *heap, u32 size);
extern void heap_Free(void *heap, void *ptr);

static void FreeCodeBooks(TosContext *context, Vq1500 *block);
static void FreeVq1500(Vq1500 *block);

static u32 InitVq1500Process(TosBaseBlock *baseBlock)
{
    baseBlock->input->inputSize =
        _tosGetProfileU32(baseBlock, 1, 0x26) * sizeof(f32);
    baseBlock->output->outputSize =
        (u8)_tosGetProfileU32(baseBlock, 2, 4);
    return 0;
}

static u32 LoadCodeBooks(Vq1500 *block, LanguageData *language)
{
    TosContext *context = block->base.context;
    VqCodeBook *destination;
    u32 i;
    CodeBookData *source;

    block->codeBookCount = language->getNbrCodeBook(language);
    FreeCodeBooks(context, block);
    block->codeBooks =
        heap_Alloc(context->heap, block->codeBookCount * sizeof(VqCodeBook));
    if (block->codeBooks == NULL) {
        return _tosErrorLog(block, 2);
    }

    destination = block->codeBooks;
    for (i = 0; i < block->codeBookCount; destination++, i++) {
        source = language->getpCodeBook(language, i);
        destination->dimension = language->getCodeBookDim(source);
        destination->firstCodeBookSize = language->getFirstCdbSize(source);
        destination->secondSearchCount = language->getNbrInSecSearch(source);
        destination->secondCodeBookSize = language->getSecondCdbSize(source);
        destination->compressedStart = (u8)language->getCompStart(source);
        destination->firstCodeBook = language->getpFirstCdb(source);
        destination->secondCodeBookIndices =
            language->getpIndexInSecCdb(source);
        destination->secondCodeBook = language->getpSecondCdb(source);
    }
    return 0;
}

static u8 GetLabel(VqCodeBook *codeBook, f32 *input)
{
    f32 minimum;
    f32 distance;
    f32 difference;
    u8 label;
    s32 i;
    s32 j;

    minimum = FLT_MAX;
    label = 0;
    {
        f32 *vector;
        f32 *inputValue;

        for (i = 0; i < codeBook->firstCodeBookSize; i++) {
            vector = codeBook->firstCodeBook + i * codeBook->dimension;
            inputValue = input;
            difference = *inputValue++ - *vector++;
            distance = difference * difference;
            for (j = 1; j < codeBook->dimension; j++) {
                difference = *inputValue++ - *vector++;
                distance += difference * difference;
                if (distance > minimum) {
                    break;
                }
            }
            if (distance < minimum) {
                minimum = distance;
                label = i;
            }
        }
    }

    {
        u32 *indices;
        f32 *vector;
        f32 *inputValue;

        indices = codeBook->secondCodeBookIndices +
                  codeBook->secondSearchCount * label;
        minimum = FLT_MAX;
        for (i = 0; i < codeBook->secondSearchCount; i++) {
            vector = codeBook->secondCodeBook + *indices++;
            inputValue = input;
            difference = *inputValue++ - *vector++;
            distance = difference * difference;
            for (j = 1; j < codeBook->dimension; j++) {
                difference = *inputValue++ - *vector++;
                distance += difference * difference;
                if (distance > minimum) {
                    break;
                }
            }
            if (distance < minimum) {
                minimum = distance;
                label = *(u32 *)vector;
            }
        }
    }
    return label;
}

static void IdleVq1500Process(
    TosBaseBlock *baseBlock, void **inputs, s32 inputCount)
{
}

static void Vq1500Process(
    TosBaseBlock *baseBlock, void **inputs, s32 inputCount)
{
    Vq1500 *block = (Vq1500 *)baseBlock;
    u8 *output;
    VqCodeBook *codeBook;
    f32 *input;
    s32 i;

    input = inputs[0];
    if (input == NULL) {
        return;
    }

    output = qEnQueueOne(block->base.output->queue);
    codeBook = block->codeBooks;
    for (i = 0; i < block->codeBookCount; i++, codeBook++) {
        *output++ = GetLabel(
            codeBook, input + codeBook->compressedStart);
    }
}

static u32 ControlVq1500Process(
    TosBaseBlock *baseBlock, u32 command, void *argument, u32 argumentSize)
{
    Vq1500 *block = (Vq1500 *)baseBlock;

    switch ((u8)command) {
    case 1:
        if ((u8)LoadCodeBooks(block, argument) != 0) {
            block->base.process = IdleVq1500Process;
        } else {
            block->base.process = Vq1500Process;
        }
        break;
    case 255:
        FreeVq1500(block);
        break;
    default:
        return 0;
    }
    return 1;
}

static void FreeCodeBooks(TosContext *context, Vq1500 *block)
{
    if (block->codeBooks != NULL) {
        heap_Free(context->heap, block->codeBooks);
        block->codeBooks = NULL;
    }
}

static void FreeVq1500(Vq1500 *block)
{
    FreeCodeBooks(block->base.context, block);
    tosBaseBlockDestruct(block);
}

void *ConstructVq(TosContext *context, u32 blockIndex)
{
    return tosBaseBlockConstruct(
        context, blockIndex, 1, 1, IdleVq1500Process, InitVq1500Process,
        ControlVq1500Process, sizeof(Vq1500));
}
