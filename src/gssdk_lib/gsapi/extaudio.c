#include "gssdk/gsapi.h"

typedef struct ExtAudioState {
    u32 active;
} ExtAudioState;

extern void *heap_Alloc(void *heap, u32 size);
extern void heap_Free(void *heap, void *ptr);

s32 ExtAudioInit(void *heap, ExtAudioState **audio)
{
    ExtAudioState *state = heap_Alloc(heap, sizeof(ExtAudioState));

    state->active = FALSE;
    *audio = state;
    return 0;
}

s32 ExtAudioClose(void *heap, ExtAudioState *audio)
{
    heap_Free(heap, audio);
    return 0;
}

s32 ExtAudioStart(ExtAudioState *audio)
{
    audio->active = TRUE;
    return 0;
}

s32 ExtAudioStop(ExtAudioState *audio)
{
    audio->active = FALSE;
    return 0;
}

u32 ExtAudioGetSamples(void *source, void *samples, u32 sampleCount)
{
    gGSAPI.getSamples(source, samples, &sampleCount);
    return sampleCount;
}

u32 ExtAudioGetSamplesLeft(ExtAudioState *audio)
{
    return 0;
}
