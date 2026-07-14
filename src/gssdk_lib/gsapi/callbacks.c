#include "types.h"

#include "gssdk/gsapi.h"

s32 asrspi_cbAbnorm(u32 auxiliary, u32 value, GSCallbackContext *context)
{
    if (gGSAPI.notify == NULL) {
        return 0;
    }

    context->callbackActive = TRUE;
    gGSAPI.notify(context, 4, value, auxiliary);
    context->callbackActive = FALSE;
    return 0;
}

s32 asrspi_cbAgc(void)
{
    return 0;
}

s32 asrspi_cbEnergyLevel(u32 value, GSCallbackContext *context)
{
    if (gGSAPI.notify == NULL) {
        return 0;
    }

    context->callbackActive = TRUE;
    gGSAPI.notify(context, 5, value, context->value44);
    context->callbackActive = FALSE;
    return 0;
}

s32 asrspi_cbSNRLevel(u32 value, GSCallbackContext *context)
{
    if (gGSAPI.notify == NULL) {
        return 0;
    }

    context->callbackActive = TRUE;
    gGSAPI.notify(context, 6, value, context->value44);
    context->callbackActive = FALSE;
    return 0;
}

s32 asrspi_cbSpeechDetected(u32 value, GSCallbackContext *context)
{
    if (gGSAPI.notify == NULL) {
        return 0;
    }

    context->callbackActive = TRUE;
    gGSAPI.notify(context, 7, value, context->value44);
    context->callbackActive = FALSE;
    return 0;
}

s32 asrspi_cbTsDetected(u32 value, GSCallbackContext *context)
{
    if (gGSAPI.notify == NULL) {
        return 0;
    }

    context->callbackActive = TRUE;
    gGSAPI.notify(context, 8, value, context->value44);
    context->callbackActive = FALSE;
    return 0;
}

extern s32 asrspi_cbResult(void);

typedef s32 (*AsrSpiCallback)(void);

typedef struct AsrSpiCallbacks {
    AsrSpiCallback result;
    AsrSpiCallback *signals;
} AsrSpiCallbacks;

AsrSpiCallback AsrSpiSignalCallBacks[] = {
    (AsrSpiCallback)asrspi_cbAbnorm,
    (AsrSpiCallback)asrspi_cbSpeechDetected,
    (AsrSpiCallback)asrspi_cbTsDetected,
    (AsrSpiCallback)asrspi_cbAgc,
    (AsrSpiCallback)asrspi_cbEnergyLevel,
    (AsrSpiCallback)asrspi_cbSNRLevel,
};

AsrSpiCallbacks AsrSpiRecogCallBacks = {
    asrspi_cbResult,
    AsrSpiSignalCallBacks,
};
