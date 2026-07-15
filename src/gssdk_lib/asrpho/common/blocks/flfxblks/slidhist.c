#include "types.h"

#include <string.h>

#include "gssdk/triggerlr.h"

extern void *heap_Calloc(void *heap, u32 count, u32 size);
extern void heap_Free(void *heap, void *ptr);
extern f32 floorf(f32 value);
extern f32 ceilf(f32 value);

void SlidingHisto_NewItem(TriggerLR *block, f32 item)
{
    s32 bin;
    s32 oldBin;

    if (item > block->histogramMinimum + block->histogramRange) {
        bin = block->histogramLastBin;
    } else if (item < block->histogramMinimum) {
        bin = 0;
    } else {
        bin = (s32)floorf(
            (item - block->histogramMinimum +
             0.5f * block->histogramBinWidth) /
            block->histogramBinWidth);
    }

    block->histogramBins[bin]++;
    block->histogramItemCount++;

    oldBin = *block->histogramHistoryWrite;
    *block->histogramHistoryWrite = bin;
    block->histogramHistoryWrite++;
    if (block->histogramHistoryWrite == block->histogramHistoryEnd) {
        block->histogramHistoryWrite = block->histogramHistory;
    }

    if (oldBin >= 0) {
        block->histogramBins[oldBin]--;
        block->histogramItemCount--;
    }
}

f32 SlidingHisto_LowerQuantile(TriggerLR *block, f32 quantile)
{
    u32 count = 0;
    u32 target;
    u32 *bin = block->histogramBins;
    s32 binIndex;

    target = (u32)ceilf(
        (f32)(0.001 + quantile * block->histogramItemCount));
    do {
        count += *bin++;
    } while (count < target);

    binIndex = (s32)(bin - block->histogramBins) - 1;
    return block->histogramMinimum + block->histogramBinWidth * binIndex;
}

void SlidingHisto_Clear(TriggerLR *block)
{
    u32 *bin = block->histogramBins + block->histogramBinCount;
    s32 *history;

    while (bin > block->histogramBins) {
        *--bin = 0;
    }
    block->histogramItemCount = 0;

    history = block->histogramHistory;
    while (history < block->histogramHistoryEnd) {
        *history++ = -1;
    }
}

u32 SlidingHisto_Init(TriggerLR *block)
{
    TosContext *context = block->base.context;
    f32 maximum;
    u32 *bin;
    s32 *history;
    u32 result = 0;

    block->histogramMinimum = _tosGetProfileFloat(block, 12, 6.5f);
    maximum = _tosGetProfileFloat(block, 13, 27.0f);
    block->histogramRange = maximum - block->histogramMinimum;

    block->histogramBinCount = _tosGetProfileU32(block, 14, 150);
    block->histogramBins = heap_Calloc(
        context->heap, block->histogramBinCount, sizeof(u32));
    block->histogramLastBin = block->histogramBinCount - 1;
    block->histogramBinWidth =
        block->histogramRange / block->histogramLastBin;

    block->histogramHistoryLength = _tosGetProfileU32(block, 11, 20);
    block->histogramHistory = heap_Calloc(
        context->heap, block->histogramHistoryLength, sizeof(s32));
    block->histogramHistoryEnd =
        block->histogramHistory + block->histogramHistoryLength;
    block->histogramHistoryWrite = block->histogramHistory;

    if (block->histogramBins == NULL || block->histogramHistory == NULL) {
        _tosErrorLog(block, 2);
        result = 1;
    } else {
        bin = block->histogramBins + block->histogramBinCount;
        while (bin > block->histogramBins) {
            *--bin = 0;
        }
        block->histogramItemCount = 0;

        history = block->histogramHistory;
        while (history < block->histogramHistoryEnd) {
            *history++ = -1;
        }
    }
    return result;
}

void SlidingHisto_Free(TriggerLR *block)
{
    TosContext *context = block->base.context;

    if (block->histogramBins != NULL) {
        heap_Free(context->heap, block->histogramBins);
    }
    if (block->histogramHistory != NULL) {
        heap_Free(context->heap, block->histogramHistory);
    }
}

u32 SlidingHisto_sizeof_SessionData(TriggerLR *block)
{
    return block->histogramHistoryLength * sizeof(s32) + sizeof(u32);
}

void SlidingHisto_PutSession(
    TriggerLR *block, const SlidingHistoSessionData *session)
{
    u32 *bin = block->histogramBins + block->histogramBinCount;
    s32 *history;
    s32 historyBin;

    while (bin > block->histogramBins) {
        *--bin = 0;
    }
    block->histogramItemCount = 0;

    history = block->histogramHistory;
    while (history < block->histogramHistoryEnd) {
        *history++ = -1;
    }

    block->histogramHistoryWrite =
        block->histogramHistory + session->historyWriteIndex;
    memcpy(
        block->histogramHistory, session->history,
        block->histogramHistoryLength * sizeof(s32));

    history = block->histogramHistory;
    while (history < block->histogramHistoryEnd) {
        historyBin = *history;
        if (historyBin >= 0) {
            block->histogramBins[historyBin]++;
            block->histogramItemCount++;
        }
        history++;
    }
}

void SlidingHisto_GetSession(
    TriggerLR *block, SlidingHistoSessionData *session)
{
    session->historyWriteIndex =
        block->histogramHistoryWrite - block->histogramHistory;
    memcpy(
        session->history, block->histogramHistory,
        block->histogramHistoryLength * sizeof(s32));
}
