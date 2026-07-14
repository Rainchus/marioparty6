#include "types.h"

#include "gssdk/tos.h"

typedef struct MedianNode {
    struct MedianNode *higher;
    struct MedianNode *lower;
    f32 value;
} MedianNode;

typedef struct Median {
    TosBaseBlock base;
    u32 capacity;
    u32 count;
    u32 ringIndex;
    MedianNode *nodes;
    MedianNode *minimum;
    MedianNode *maximum;
    MedianNode *median;
} Median;

extern void *heap_Calloc(void *heap, u32 count, u32 size);
extern void heap_Free(void *heap, void *ptr);

static void ProcessMedian(
    TosBaseBlock *baseBlock, void **input, s32 inputCount)
{
    Median *block = (Median *)baseBlock;
    MedianNode *node;
    MedianNode *cursor;
    f32 sample;
    f32 *output;
    u32 i;

    if (*input != NULL) {
        node = &block->nodes[block->ringIndex];
        sample = *(f32 *)*input;

        if (block->count == block->capacity) {
            if (node->higher != NULL) {
                node->higher->lower = node->lower;
            } else {
                block->maximum = node->lower;
            }

            if (node->lower != NULL) {
                node->lower->higher = node->higher;
            } else {
                block->minimum = node->higher;
            }

            block->count--;

            if (node == block->median) {
                if (node->higher != NULL) {
                    block->median = node->higher;
                } else if (node->lower != NULL) {
                    block->median = node->lower;
                } else {
                    block->median = NULL;
                }
            }
        }

        node->value = sample;

        if (block->count == 0) {
            block->median = node;
            block->maximum = node;
            block->minimum = node;
            node->lower = NULL;
            node->higher = NULL;
        } else if (sample < block->median->value) {
            cursor = block->median->lower;
            while (cursor != NULL && sample < cursor->value) {
                cursor = cursor->lower;
            }

            if (cursor != NULL) {
                node->higher = cursor->higher;
                cursor->higher = node;
                node->lower = cursor;
                node->higher->lower = node;
            } else {
                node->higher = block->minimum;
                node->lower = NULL;
                block->minimum->lower = node;
                block->minimum = node;
            }
        } else {
            cursor = block->median->higher;
            while (cursor != NULL && sample >= cursor->value) {
                cursor = cursor->higher;
            }

            if (cursor != NULL) {
                node->higher = cursor;
                node->lower = cursor->lower;
                cursor->lower = node;
                node->lower->higher = node;
            } else {
                node->higher = NULL;
                node->lower = block->maximum;
                block->maximum->higher = node;
                block->maximum = node;
            }
        }

        block->ringIndex++;
        if (block->ringIndex == block->capacity) {
            block->ringIndex = 0;
        }

        block->count++;
        block->median = block->minimum;
        for (i = 0; i < (block->count >> 1); i++) {
            block->median = block->median->higher;
        }

        output = qEnQueueOne(block->base.output->queue);
        if (output != NULL) {
            *output = block->median->value;
        }
    }
}

static u32 ControlMedian(
    TosBaseBlock *baseBlock, u32 command, void *argument, u32 argumentSize)
{
    Median *block = (Median *)baseBlock;

    switch ((u8)command) {
    case 255:
        if (block->nodes != NULL) {
            heap_Free(block->base.context->heap, block->nodes);
        }
        tosBaseBlockDestruct(block);
        break;
    default:
        return 0;
    }

    return 1;
}

u32 InitMedian(TosBaseBlock *baseBlock)
{
    Median *block = (Median *)baseBlock;
    u32 result = 0;

    block->capacity = 0;
    block->count = 0;
    block->ringIndex = 0;
    block->nodes = NULL;
    block->minimum = NULL;
    block->maximum = NULL;
    block->median = NULL;

    block->base.input->inputSize = 0x18;
    block->base.output->outputSize = sizeof(f32);

    block->capacity = _tosGetProfileU32(block, 1, 1);
    if ((block->capacity & 1) == 0) {
        _tosErrorLog(block, 200);
        result = 1;
    } else {
        block->nodes = heap_Calloc(
            block->base.context->heap,
            block->capacity,
            sizeof(MedianNode));

        block->median = NULL;
        block->maximum = NULL;
        block->minimum = NULL;
        block->ringIndex = 0;
        block->count = 0;
    }

    return result;
}

void *ConstructMedian(TosContext *context, u32 blockIndex)
{
    return tosBaseBlockConstruct(
        context, blockIndex, 1, 1,
        (TosProcessFunction)ProcessMedian,
        (TosInitFunction)InitMedian,
        (TosControlFunction)ControlMedian,
        sizeof(Median));
}
