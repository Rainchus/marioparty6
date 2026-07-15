#include "gssdk/triggerlr.h"

#include <math.h>

extern void *heap_Calloc(void *heap, u32 count, u32 size);
extern void heap_Free(void *heap, void *ptr);
extern f32 expf(f32 value);
extern f32 logf_check(f32 value);

s32 Voicing_AddSignal(TriggerLR *block, s16 *input)
{
    s16 *output = block->subsamplerOutput;
    s16 *outputEnd = output + block->frameLength;
    s32 result = 0;

    if (Subsampler_Process(block, input) != 0) {
        result = 1;
    } else {
        while (output < outputEnd) {
            s16 sample = *output;

            *block->signalWrite++ = sample;
            *block->energyWrite++ = sample * sample;
            if (block->signalWrite >= block->signalEnd) {
                block->signalWrite = block->signalHistory;
                block->energyWrite = block->energyHistory;
            }
            output++;
        }
    }
    return result;
}

void Voicing_MaintainNoiseEner(TriggerLR *block)
{
    u32 *energy = block->energyWrite - 1;
    f32 energySum = 0.0f;
    u32 i;

    if (energy < block->energyHistory) {
        energy += block->historyLength;
    }
    for (i = 0; i < block->frameLength; i++) {
        energySum += *energy--;
        if (energy < block->energyHistory) {
            energy += block->historyLength;
        }
    }

    *block->noiseEnergyWrite++ = logf_check(energySum);
    if (block->noiseEnergyWrite >= block->noiseEnergyEnd) {
        block->noiseEnergyWrite = block->noiseEnergyHistory;
    }
    if (block->noiseEnergyCount < block->noiseEnergyCapacity) {
        block->noiseEnergyCount++;
    }
}

f32 Voicing_GetMaxVoicing(TriggerLR *block)
{
    f32 *noiseEnergy;
    f32 noiseMean = 0.0f;
    f32 noiseLog;
    f32 currentEnergy;
    f32 laggedEnergy;
    f32 maximumCorrelation = 0.0f;
    f32 maximumVoicing = 0.0f;
    u32 bestLag = 0;
    u32 lag;
    u32 i;

    for (noiseEnergy = block->noiseEnergyHistory;
         noiseEnergy < block->noiseEnergyHistory + block->noiseEnergyCount;
         noiseEnergy++) {
        noiseMean += expf(*noiseEnergy);
    }
    if (block->noiseEnergyCount != 0) {
        noiseMean /= block->noiseEnergyCount;
    }
    noiseLog = logf_check(noiseMean);

    {
        u32 *energy = block->energyWrite - 1;
        f32 energySum = 0.0f;

        if (energy < block->energyHistory) {
            energy += block->historyLength;
        }
        for (i = 0; i < block->frameLength; i++) {
            energySum += *energy--;
            if (energy < block->energyHistory) {
                energy += block->historyLength;
            }
        }
        currentEnergy = logf_check(energySum);
    }

    if (currentEnergy >= noiseLog + block->voicingThreshold &&
        currentEnergy >= block->minimumLogEnergy) {
        for (lag = block->minimumLag; lag <= block->maximumLag; lag++) {
            f32 correlation = 0.0f;
            u32 phase;

            for (phase = 0; phase < 4; phase++) {
                s16 *first = block->signalWrite - 1 - phase;
                s16 *second = block->signalWrite - 1 - phase - lag;

                if (first < block->signalHistory) {
                    first += block->historyLength;
                }
                if (second < block->signalHistory) {
                    second += block->historyLength;
                }

                for (i = phase; i < block->frameLength; i += 4) {
                    correlation += (f32)(*first * *second);
                    first -= 4;
                    if (first < block->signalHistory) {
                        first += block->historyLength;
                    }
                    second -= 4;
                    if (second < block->signalHistory) {
                        second += block->historyLength;
                    }
                }
                if (correlation < 0.0f) {
                    break;
                }
            }

            if (correlation > maximumCorrelation) {
                maximumCorrelation = correlation;
                bestLag = lag;
            }
        }

        {
            u32 *energy = block->energyWrite - 1 - bestLag;
            f32 energySum = 0.0f;

            if (energy < block->energyHistory) {
                energy += block->historyLength;
            }
            for (i = 0; i < block->frameLength; i++) {
                energySum += *energy--;
                if (energy < block->energyHistory) {
                    energy += block->historyLength;
                }
            }
            laggedEnergy = logf_check(energySum);
        }

        if (laggedEnergy >= noiseLog + block->voicingThreshold &&
            laggedEnergy >= block->minimumLogEnergy) {
            f32 laggedSignalEnergy =
                0.5f * logf_check(expf(laggedEnergy) - noiseMean);
            f32 currentSignalEnergy =
                0.5f * logf_check(expf(currentEnergy) - noiseMean);

            maximumVoicing = logf_check(maximumCorrelation) -
                             currentSignalEnergy - laggedSignalEnergy;
        }
    }
    return maximumVoicing;
}

void Voicing_Reset(TriggerLR *block)
{
    block->signalWrite = block->signalHistory;
    while (block->signalWrite < block->signalEnd) {
        *block->signalWrite++ = 0;
    }
    block->signalWrite = block->signalHistory;

    block->energyWrite = block->energyHistory;
    while (block->energyWrite < block->energyEnd) {
        *block->energyWrite++ = 0;
    }
    block->energyWrite = block->energyHistory;

    block->noiseEnergyWrite = block->noiseEnergyHistory;
    block->noiseEnergyCount = 0;
    Subsampler_Reset(block);
}

s32 InitVoicing(TriggerLR *block)
{
    TosContext *context = block->base.context;
    f32 minimumPeriod;
    f32 maximumPeriod;
    f32 frameDuration;
    f32 sampleRate;
    f32 minimumLogEnergy;
    u32 result = 0;

    block->frameLength = block->maximumLag = block->minimumLag = 0;
    block->signalEnd = block->signalWrite = block->signalHistory = NULL;
    block->energyEnd = block->energyWrite = block->energyHistory = NULL;
    block->noiseEnergyEnd = block->noiseEnergyHistory =
        block->noiseEnergyWrite = NULL;
    block->noiseEnergyCapacity = block->noiseEnergyCount = 0;

    minimumPeriod = _tosGetProfileFloat(block, 24, 0.005f);
    maximumPeriod = _tosGetProfileFloat(block, 25, 0.014f);
    frameDuration = _tosGetProfileFloat(block, 26, 0.01f);
    sampleRate = _tosGetProfileFloat(block, 27, 11000.0f);
    minimumLogEnergy = _tosGetProfileFloat(block, 29, -1000.0f);

    block->minimumLogEnergy = minimumLogEnergy;
    block->voicingThreshold =
        _tosGetProfileFloat(block, 28, 6.0f) * 0.23025851f;
    block->minimumLag =
        (u32)floorf(minimumPeriod * sampleRate * 0.5f);
    block->maximumLag =
        (u32)ceilf(maximumPeriod * sampleRate * 0.5f);
    block->frameLength =
        (u32)ceilf(frameDuration * sampleRate * 0.5f);
    block->historyLength = block->maximumLag + block->frameLength;

    block->signalHistory =
        heap_Calloc(context->heap, block->historyLength, sizeof(s16));
    block->signalWrite = block->signalHistory;
    block->signalEnd = block->signalHistory + block->historyLength;

    block->energyHistory =
        heap_Calloc(context->heap, block->historyLength, sizeof(u32));
    block->energyWrite = block->energyHistory;
    block->energyEnd = block->energyHistory + block->historyLength;

    block->noiseEnergyCapacity = 16;
    block->noiseEnergyHistory = heap_Calloc(
        context->heap, block->noiseEnergyCapacity, sizeof(f32));
    block->noiseEnergyEnd =
        block->noiseEnergyHistory + block->noiseEnergyCapacity;

    if (block->signalHistory == NULL || block->energyHistory == NULL ||
        block->noiseEnergyHistory == NULL) {
        _tosErrorLog(block, 2);
        result = 1;
    } else {
        Voicing_Reset(block);
        if (Subsampler_Init(block) != 0) {
            result = 1;
        }
    }
    return result;
}

void Voicing_Free(TriggerLR *block)
{
    TosContext *context = block->base.context;

    if (block->signalHistory != NULL) {
        heap_Free(context->heap, block->signalHistory);
    }
    if (block->energyHistory != NULL) {
        heap_Free(context->heap, block->energyHistory);
    }
    if (block->noiseEnergyHistory != NULL) {
        heap_Free(context->heap, block->noiseEnergyHistory);
    }
    Subsampler_Free(block);
}
