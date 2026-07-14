#ifndef GSSDK_GSAPI_H
#define GSSDK_GSAPI_H

#include "types.h"

typedef s32 (*GSNotifyCallback)(void *context, u32 event, u32 value,
                               u32 auxiliary);
typedef s32 (*GSGetSamplesCallback)(void *source, void *samples,
                                   u32 *sampleCount);

typedef struct GSAPIState {
    void *heap;
    u8 reserved04[0x18];
    GSNotifyCallback notify;
    GSGetSamplesCallback getSamples;
    u8 reserved24[0x6C];
    u32 sessionDataCount;
} GSAPIState;

typedef struct GSCallbackContext {
    u8 reserved00[0x44];
    u32 value44;
    u8 reserved48[0xC];
    u32 callbackActive;
} GSCallbackContext;

extern GSAPIState gGSAPI;

#endif
