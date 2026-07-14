#include "types.h"

#include "gssdk/tos.h"

typedef struct Undersampler {
TosBaseBlock base;
u16 inputSamples;
u8 factor;
u8 reserved2B;
} Undersampler;

static void ProcessUndersampler(
    TosBaseBlock *baseBlock, void **inputs, s32 inputCount)
{
Undersampler *block = (Undersampler *)baseBlock;
s16 *input = inputs[0];
u8 factor = block->factor;
s32 *output;
s16 *cursor;
s16 *groupEnd;
s16 *inputEnd;
s32 sum;

if (input != NULL) {
    output = qEnQueueOne(baseBlock->output->queue);
    inputEnd = input + block->inputSamples;
    cursor = input;
    while (cursor < inputEnd) {
        groupEnd = cursor + factor;
        sum = *cursor++;
        while (cursor < groupEnd) {
            sum += *cursor++;
        }
        *output++ = sum;
    }
}
}

static u32 ControlUndersampler(
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

static u32 InitUndersampler(TosBaseBlock *baseBlock)
{
u16 inputSamples;
Undersampler *block = (Undersampler *)baseBlock;
u16 inputRate;
s32 outputSamples;
u16 outputRate;

inputSamples = (u16)_tosGetProfileU32(block, 1, 110);
block->inputSamples = inputSamples;
inputRate = (u16)_tosGetProfileU32(block, 2, 11000);
outputRate = (u16)_tosGetProfileU32(block, 3, 2000);
block->factor = inputRate / outputRate;
outputSamples = inputSamples / block->factor;
if (inputSamples % block->factor != 0) {
    _tosErrorLog(block, 100);
    return 1;
}
block->base.input->inputSize = inputSamples * sizeof(s16);
block->base.output->outputSize = outputSamples * sizeof(s32);
return 0;
}

void *ConstructUndersampler(TosContext *context, u32 blockIndex)
{
return tosBaseBlockConstruct(
    context, blockIndex, 1, 1, (TosProcessFunction)ProcessUndersampler,
    (TosInitFunction)InitUndersampler,
    (TosControlFunction)ControlUndersampler, sizeof(Undersampler));
}
