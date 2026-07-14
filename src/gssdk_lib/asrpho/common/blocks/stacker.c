#include "types.h"

#include <string.h>

#include "gssdk/tos.h"

typedef struct Stacker {
TosBaseBlock base;
u32 reserved28;
u16 numberInputs;
u16 *elementSizes;
u8 *enabled;
} Stacker;

extern void *heap_Calloc(void *heap, u32 count, u32 size);
extern void heap_Free(void *heap, void *ptr);

static void ProcessStacker(
    TosBaseBlock *baseBlock, void **inputs, s32 inputCount)
{
Stacker *block = (Stacker *)baseBlock;
u16 i;
u8 *output;

for (i = 0; i < inputCount; i++) {
    if (inputs[i] != NULL) {
        break;
    }
}
if (i == inputCount) {
    return;
}

output = qEnQueueOne(block->base.output->queue);
for (i = 0; i < inputCount; i++) {
    if (block->enabled[i] != 0) {
        memcpy(output, inputs[i], block->elementSizes[i]);
    }
    output += block->elementSizes[i];
}
}

static u32 ControlStacker(
    TosBaseBlock *baseBlock, u32 command, void *argument, u32 argumentSize)
{
Stacker *block = (Stacker *)baseBlock;

switch ((u8)command) {
case 100:
    block->enabled[0] = 0;
    break;
case 101:
    block->enabled[1] = 0;
    break;
case 150:
    block->enabled[0] = 1;
    break;
case 151:
    block->enabled[1] = 1;
    break;
case 255:
    heap_Free(block->base.context->heap, block->elementSizes);
    heap_Free(block->base.context->heap, block->enabled);
    tosBaseBlockDestruct(block);
    break;
default:
    return 0;
}
return 1;
}

static u32 InitStacker(TosBaseBlock *baseBlock)
{
Stacker *block = (Stacker *)baseBlock;
TosContext *context = baseBlock->context;
u16 i;

block->numberInputs = _tosGetProfileU32(block, 3, 2);
block->elementSizes = heap_Calloc(
    context->heap, block->numberInputs, sizeof(u16));
block->enabled = heap_Calloc(
    context->heap, block->numberInputs, sizeof(u8));
if (block->elementSizes == NULL || block->enabled == NULL) {
    _tosErrorLog(block, 2);
    return 1;
}

for (i = 0; i < block->numberInputs; i++) {
    switch (i) {
    case 0:
        block->elementSizes[0] = _tosGetProfileU32(block, 1, 152);
        break;
    case 1:
        block->elementSizes[1] = _tosGetProfileU32(block, 2, 20);
        break;
    default:
        _tosErrorLog(block, 200);
        return 1;
    }
    block->enabled[i] = 1;
}

for (i = 0; i < block->numberInputs; i++) {
    block->base.input[i].inputSize = block->elementSizes[i];
    block->base.output->outputSize =
        block->base.output->outputSize + block->base.input[i].inputSize;
}
return 0;
}

void *ConstructStacker(TosContext *context, u32 blockIndex)
{
return tosBaseBlockConstruct(
    context, blockIndex, 2, 1, ProcessStacker, InitStacker,
    ControlStacker, sizeof(Stacker));
}
