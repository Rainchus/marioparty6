#include "types.h"

#include <math.h>
#include <string.h>

#include "gssdk/fft.h"
#include "gssdk/mqueue.h"
#include "gssdk/tos.h"

typedef struct FFTMod {
    TosBaseBlock base;
    u32 inputLength;
    u32 fftLength;
    u32 firstPowerBin;
    u32 lastPowerBin;
    f32 powerFloor;
    u8 padInput;
    u32 paddingBytes;
    f32 *buffer;
    s16 powerLevel;
    u8 powerSpectrum;
} FFTMod;

extern void *heap_Alloc(void *heap, u32 size);
extern void heap_Free(void *heap, void *ptr);
extern f32 logf_check(f32 value);

static f32 CalcPowerSpec(FFTMod *block, f32 *input, f32 *output)
{
    f32 power = block->powerFloor;
    f32 *real = input;
    f32 *imaginary = input + block->fftLength - 1;
    f32 *powerStart = output + block->firstPowerBin;
    f32 *powerEnd = output + block->lastPowerBin;
    f32 *spectrumEnd = output + block->fftLength / 2;

    *output = *real * *real;
    if (output == powerStart) {
        power += *powerStart++;
    }
    real++;
    output++;

    while (output < powerStart) {
        *output++ = *real * *real + *imaginary * *imaginary;
        real++;
        imaginary--;
    }
    while (output <= powerEnd) {
        *output = *real * *real + *imaginary * *imaginary;
        power += *output++;
        real++;
        imaginary--;
    }
    while (output < spectrumEnd) {
        *output++ = *real * *real + *imaginary * *imaginary;
        real++;
        imaginary--;
    }
    *output = *real * *real;
    return power;
}

static void CalcAmplitudeSpec(FFTMod *block, f32 *input, f32 *output)
{
    f32 *real = input;
    f32 *imaginary = input + block->fftLength - 1;
    f32 *powerStart = output + block->firstPowerBin;
    f32 *powerEnd = output + block->lastPowerBin;
    f32 *spectrumEnd = output + block->fftLength / 2;

    *output = sqrtf(*real * *real);
    if (output == powerStart) {
        powerStart++;
    }
    real++;
    output++;

    while (output < powerStart) {
        *output++ = sqrtf(*real * *real + *imaginary * *imaginary);
        real++;
        imaginary--;
    }
    while (output <= powerEnd) {
        *output++ = sqrtf(*real * *real + *imaginary * *imaginary);
        real++;
        imaginary--;
    }
    while (output < spectrumEnd) {
        *output++ = sqrtf(*real * *real + *imaginary * *imaginary);
        real++;
        imaginary--;
    }
    *output = sqrtf(*real * *real);
}

static void ProcessFFTMod(
    TosBaseBlock *baseBlock, void **inputs, s32 inputCount)
{
    FFTMod *block = (FFTMod *)baseBlock;
    f32 *input;
    f32 *spectrum;

    if (block->padInput != 0) {
        if (inputs[0] != NULL) {
            memcpy(block->buffer, inputs[0], block->base.input->inputSize);
            input = block->buffer;
            memset(
                input + block->inputLength, 0, block->paddingBytes);
        } else {
            input = NULL;
        }
    } else {
        input = inputs[0];
    }

    if (input != NULL) {
        if (block->powerSpectrum != 0) {
            spectrum = qEnQueueOne(block->base.output[1].queue);
        } else {
            spectrum = qEnQueueOne(block->base.output->queue);
        }

        realfft(block->fftLength, input);
        if (block->powerSpectrum != 0) {
            f32 logPower = logf_check(CalcPowerSpec(block, input, spectrum));
            s16 powerLevel;

            qEnQueue(block->base.output->queue, &logPower, 1);
            powerLevel =
                (s16)(100.0 * 4.3429446f * (logPower - 27.37f));
            if (powerLevel > 1800) {
                powerLevel = 1800;
            }

            if (powerLevel > block->powerLevel) {
                block->powerLevel = powerLevel;
            } else {
                block->powerLevel =
                    (((block->powerLevel + 7200) * 64000) >> 16) - 7200;
            }
        } else {
            CalcAmplitudeSpec(block, input, spectrum);
        }
    }
}

static u32 ControlFFTMod(
    TosBaseBlock *baseBlock, u32 command, void *argument, u32 argumentSize)
{
    FFTMod *block = (FFTMod *)baseBlock;
    TosContext *context = block->base.context;

    switch ((u8)command) {
    case 1:
        if (block->powerSpectrum == 0) {
            _tosErrorLog(block, 102);
            return 1;
        }
        *(s16 **)argument = &block->powerLevel;
        break;
    case 255:
        if (block->padInput != 0) {
            heap_Free(context->heap, block->buffer);
        }
        tosBaseBlockDestruct(block);
        break;
    default:
        return 0;
    }
    return 1;
}

static u32 InitFFTMod(TosBaseBlock *baseBlock)
{
    FFTMod *block = (FFTMod *)baseBlock;
    TosContext *context = baseBlock->context;
    s16 sampleRate;
    f32 frequency;
    f32 scale;

    sampleRate = (s16)_tosGetProfileU32(block, 3, 5500);
    block->fftLength = (u16)_tosGetProfileU32(block, 4, 512);
    block->inputLength = (u16)_tosGetProfileU32(block, 6, 512);
    if (block->inputLength > block->fftLength) {
        return 1;
    }

    if (block->fftLength > block->inputLength) {
        block->padInput = 1;
        block->paddingBytes =
            (block->fftLength - block->inputLength) * sizeof(f32);
        block->buffer =
            heap_Alloc(context->heap, block->fftLength * sizeof(f32));
    }

    block->base.input->inputSize = block->inputLength * sizeof(f32);
    if (block->powerSpectrum == 0) {
        block->base.output->outputSize =
            (block->fftLength / 2 + 1) * sizeof(f32);
    } else {
        block->base.output->outputSize = sizeof(f32);
        block->base.output[1].outputSize =
            (block->fftLength / 2 + 1) * sizeof(f32);
    }

    frequency = _tosGetProfileFloat(block, 1, 300.0f);
    scale = 2.0f * block->fftLength;
    block->firstPowerBin =
        (u32)floorf((sampleRate * frequency) / scale);
    if (block->firstPowerBin == 0) {
        block->firstPowerBin = 0;
    }
    if (block->firstPowerBin >= block->fftLength / 2 - 1) {
        block->firstPowerBin = block->fftLength / 2 - 2;
    }

    frequency = _tosGetProfileFloat(block, 2, 4500.0f);
    block->lastPowerBin =
        (u32)floorf(
            0.9999f + (block->fftLength * frequency) / scale);
    if (block->lastPowerBin == 0) {
        block->lastPowerBin = 0;
    }
    if (block->lastPowerBin >= block->fftLength / 2 - 1) {
        block->lastPowerBin = block->fftLength / 2 - 2;
    }
    if (block->lastPowerBin <= block->firstPowerBin) {
        block->lastPowerBin = block->firstPowerBin;
    }

    block->powerFloor = _tosGetProfileFloat(block, 5, 0.0f);
    return 0;
}

void *ConstructFFTMod(TosContext *context, u32 blockIndex)
{
    struct TosProfileBlock {
        TosContext *context;
        u8 blockIndex;
    } profileBlock;
    FFTMod *block;
    u8 powerSpectrum;

    profileBlock.context = context;
    profileBlock.blockIndex = blockIndex;
    powerSpectrum =
        (u8)_tosGetProfileU32(&profileBlock, 7, 1);
    block = (FFTMod *)tosBaseBlockConstruct(
        context, blockIndex, 1, powerSpectrum != 0 ? 2 : 1,
        ProcessFFTMod, InitFFTMod, ControlFFTMod, sizeof(FFTMod));
    block->powerSpectrum = powerSpectrum;
    return block;
}
