#include "gssdk/gsapi.h"

typedef struct GSContextParamState {
    u8 reserved00[8];
    u32 validParameters;
    u32 values[];
} GSContextParamState;

typedef struct GSContext {
    void *engine;
    u8 reserved04[8];
    GSContextParamState *parameters;
    u8 reserved10[0x20];
    u32 data30;
    u8 reserved34[0xC];
    u32 data40;
} GSContext;

typedef struct GSActiveWords {
    u8 reserved00[0x24];
    void *words;
} GSActiveWords;

extern void heap_Free(void *heap, void *ptr);
extern s32 asrspi_ActivateWords(void *engine, void *words, s32 index);

s32 ContextAPIDeActivate(GSContext *context)
{
    context->data30 = 0;
    context->data40 = 0;
    return 0;
}

s32 ContextSetCtxData(void **contextData, void *data)
{
    *contextData = data;
    return 0;
}

s32 ContextGetParam(GSContext *context, u32 parameter, u32 *value)
{
    GSContextParamState *parameters = context->parameters;

    if (parameters == 0 ||
        (parameters->validParameters & (1U << parameter)) == 0) {
        return 0x80CC0008;
    }

    *value = parameters->values[parameter];
    return 0;
}

s32 ContextSetActiveWords(GSContext *context, GSActiveWords *activeWords)
{
    if (activeWords->words == 0) {
        return 0;
    }

    return asrspi_ActivateWords(context->engine, activeWords->words, -1);
}

s32 SessionDataFree(void *sessionData)
{
    if (sessionData != 0) {
        heap_Free(gGSAPI.heap, sessionData);
        gGSAPI.sessionDataCount--;
    }

    return 0;
}
