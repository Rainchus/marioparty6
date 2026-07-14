#include "types.h"

#include "dolphin/m2s.h"

typedef struct SidState {
    u16 recording;
} SidState;

extern void *heap_Alloc(void *heap, u32 size);
extern void heap_Free(void *heap, void *ptr);

s32 SidInit(void *heap, void *unused, SidState **sid)
{
    SidState *state = heap_Alloc(heap, sizeof(SidState));

    M2SOpen();
    state->recording = FALSE;
    *sid = state;
    return 0;
}

u16 SidIsRecording(SidState *sid)
{
    return sid->recording;
}

s32 SidClose(void *heap, SidState *sid)
{
    M2SClose();
    heap_Free(heap, sid);
    return 0;
}

s32 SidStart(SidState *sid)
{
    M2SStart();
    sid->recording = TRUE;
    return 0;
}

s32 SidStop(SidState *sid)
{
    M2SStop();
    sid->recording = FALSE;
    return 0;
}

s32 SidGetSamples(SidState *sid, s16 **buffer, s32 samples)
{
    if (!sid->recording) {
        return 0;
    }
    return M2SGetSamples(buffer, samples);
}

s32 SidGetSamplesLeft(SidState *sid)
{
    if (sid == NULL) {
        return 0;
    }
    if (!sid->recording) {
        return 0;
    }
    return M2SGetSamplesLeft();
}

s32 SidAdvanceBuffer(SidState *sid, s32 samples)
{
    if (sid->recording) {
        M2SAdvanceBuffer(samples);
    }
    return 0;
}
