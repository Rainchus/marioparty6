#include "types.h"

#include <string.h>

#include "gssdk/tos.h"

typedef struct GenderFilter {
TosBaseBlock base;
u32 numberInputs;
u16 elementSize;
u16 reserved2E;
} GenderFilter;

static void ProcessGenderFilter(
    TosBaseBlock *baseBlock, void **inputs, s32 inputCount)
{
GenderFilter *block = (GenderFilter *)baseBlock;
void **inputCursor;
s32 i;
void *value;
void *output;

inputCursor = inputs + 1;
value = inputs[0];
i = 1;
while (value == NULL && i < inputCount) {
    value = *inputCursor++;
    i++;
}
if (value != NULL) {
    output = qEnQueueOne(baseBlock->output->queue);
    if (output != NULL) {
        memcpy(output, value, block->elementSize);
    }
}
}

static u32 ControlGenderFilter(
    TosBaseBlock *baseBlock, u32 command, void *argument, u32 argumentSize)
{
switch ((u8)command) {
case 255:
    tosBaseBlockDestruct(baseBlock);
    break;
default:
    return 0;
}
return 1;
}

static u32 InitGenderFilter(TosBaseBlock *baseBlock)
{
GenderFilter *block = (GenderFilter *)baseBlock;
u32 i;

block->numberInputs = (u16)_tosGetProfileU32(block, 1, 3);
block->elementSize = _tosGetProfileU32(block, 2, 4);
for (i = 0; i < block->numberInputs; i++) {
    baseBlock->input[i].inputSize = block->elementSize;
}
baseBlock->output->outputSize = block->elementSize;
return 0;
}

void *ConstructGenderFilter(TosContext *context, u32 blockIndex)
{
struct TosProfileBlock {
    TosContext *context;
    u8 blockIndex;
} profileBlock;

profileBlock.context = context;
profileBlock.blockIndex = blockIndex;
return tosBaseBlockConstruct(
    context, blockIndex, (u8)_tosGetProfileU32(&profileBlock, 1, 3), 1,
    (TosProcessFunction)ProcessGenderFilter,
    (TosInitFunction)InitGenderFilter,
    (TosControlFunction)ControlGenderFilter, sizeof(GenderFilter));
}
