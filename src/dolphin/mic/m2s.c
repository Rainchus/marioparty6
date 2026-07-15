#include <string.h>

#include <dolphin/m2s.h>
#include <dolphin/mic.h>
#include <dolphin/os.h>

#pragma force_active on

#define M2S_CHANNEL_COUNT 2
#define M2S_BUFFER_SAMPLES 128
#define M2S_CALIBRATION_SAMPLES 44
#define M2S_CALIBRATION_HISTORY_BYTES (M2S_CALIBRATION_SAMPLES * sizeof(s16))
#define M2S_RESERVED_RING_SAMPLES 1152

typedef struct M2SControlBlock {
    BOOL is_attached;
    BOOL is_active;
    s32 sample_index;
    s32 shifts;
    s32 calibration_threshold;
    BOOL calibrated;
    s32 calibration_accumulator;
    s32 calibration_history_index;
    u32 calibration_history[M2S_CALIBRATION_SAMPLES];
} M2SControlBlock;

typedef struct M2SDebugControl {
    s16* buffer;
    s32 samples;
    s32 index;
} M2SDebugControl;

static void __M2SCalibration(s16* buffer, s32 samples);

static M2SControlBlock __M2SBlock[M2S_CHANNEL_COUNT];
static u32 __M2STmpHistory[M2S_CALIBRATION_SAMPLES];
static M2SDebugControl __M2SDebug;

static BOOL __init = FALSE;
static BOOL __open = FALSE;
static s32 __M2SChannel = 0;
static s16* __M2SBuffer = NULL;
static s32 __M2SMode = 0;
static s32 __M2SPrerecordingSamples = 0;
static s32 __M2STmpAcc = 0;
static s32 __M2STmpPtr = 0;

void M2SInit(void) {
    if (__init) {
        return;
    }

    __M2SBlock[0].is_attached = __M2SBlock[1].is_attached = FALSE;
    __M2SBlock[0].is_active = __M2SBlock[1].is_active = FALSE;
    __M2SBlock[0].shifts = __M2SBlock[1].shifts = 0;
    __M2SBlock[0].calibration_threshold = __M2SBlock[1].calibration_threshold = 0;

    __open = FALSE;
    __M2SChannel = 0;
    __M2SBuffer = NULL;
    __M2SMode = 0;
    __M2SPrerecordingSamples = 0;
    __M2SDebug.buffer = NULL;
    __init = TRUE;
}

BOOL M2SSetBuffer(s16* buffer) {
    if (!__init) {
        return FALSE;
    }
    if (buffer == NULL) {
        return FALSE;
    }

    if (__M2SBlock[__M2SChannel].is_active) {
        return FALSE;
    }

    __M2SBuffer = buffer;
    return TRUE;
}

BOOL M2SSetActiveChannel(s32 chan) {
    M2SControlBlock* cb;
    BOOL enabled;

    if (!__init) {
        return FALSE;
    }
    if (chan < 0 || chan >= M2S_CHANNEL_COUNT) {
        return FALSE;
    }

    enabled = OSDisableInterrupts();
    cb = &__M2SBlock[__M2SChannel];
    if (cb->is_active) {
        OSRestoreInterrupts(enabled);
        return FALSE;
    }

    if (!MICIsAttached(__M2SChannel)) {
        cb->is_attached = FALSE;
    }

    cb = &__M2SBlock[chan];
    if (__open) {
        if (MICIsAttached(chan)) {
            cb->is_attached = TRUE;
        } else {
            cb->is_attached = FALSE;
        }
    }

    __M2SChannel = chan;
    OSRestoreInterrupts(enabled);
    return TRUE;
}

BOOL M2SSetMode(s32 mode) {
    if (!__init) {
        return FALSE;
    }
    if ((mode & ~3) != 0) {
        return FALSE;
    }

    if (__M2SBlock[__M2SChannel].is_active) {
        return FALSE;
    }

    __M2SMode = mode;
    return TRUE;
}

BOOL M2SSetPrerecordSamples(s32 milliseconds) {
    BOOL enabled;
    s32 ring_size;
    s32 sample_rate;
    s32 max_milliseconds;

    if (!__init) {
        return FALSE;
    }

    enabled = OSDisableInterrupts();
    if (__M2SBlock[__M2SChannel].is_active) {
        OSRestoreInterrupts(enabled);
        return FALSE;
    }
    if (!MICIsAttached(__M2SChannel)) {
        OSRestoreInterrupts(enabled);
        return FALSE;
    }

    MICGetRingbuffsize(__M2SChannel, &ring_size);
    MICGetSampleRate(__M2SChannel, &sample_rate);
    max_milliseconds = (((ring_size >> 1) - M2S_RESERVED_RING_SAMPLES) * 1000) / sample_rate;
    if (max_milliseconds < 0) {
        OSRestoreInterrupts(enabled);
        return FALSE;
    }
    if (milliseconds < 0 || max_milliseconds < milliseconds) {
        OSRestoreInterrupts(enabled);
        return FALSE;
    }

    __M2SPrerecordingSamples = (milliseconds * sample_rate) / 1000;
    OSRestoreInterrupts(enabled);
    return TRUE;
}

BOOL M2SSetShifts(s32 shifts) {
    M2SControlBlock* cb;

    if (!__init) {
        return FALSE;
    }
    if (shifts < -16 || shifts > 16) {
        return FALSE;
    }

    cb = &__M2SBlock[__M2SChannel];
    if (cb->is_active) {
        return FALSE;
    }

    cb->shifts = shifts;
    return TRUE;
}

void M2SOpen(void) {
    M2SControlBlock* cb;
    BOOL enabled = OSDisableInterrupts();

    if (!__init || __open) {
        OSRestoreInterrupts(enabled);
        return;
    }

    cb = &__M2SBlock[__M2SChannel];
    if (!MICIsAttached(__M2SChannel)) {
        cb->is_attached = FALSE;
        OSRestoreInterrupts(enabled);
        return;
    }

    cb->is_attached = TRUE;
    __open = TRUE;
    OSRestoreInterrupts(enabled);
}

void M2SClose(void) {
    s32 chan;

    if (!__init || !__open) {
        return;
    }

    for (chan = 0; chan < M2S_CHANNEL_COUNT; chan++) {
        M2SControlBlock* cb = &__M2SBlock[chan];

        if (cb->is_active) {
            MICStop(chan);
            cb->is_active = FALSE;
        }
        cb->is_attached = FALSE;
    }
    __open = FALSE;
}

void M2SStart(void) {
    M2SControlBlock* cb;
    BOOL enabled;

    if (!__init || !__open) {
        return;
    }

    enabled = OSDisableInterrupts();
    cb = &__M2SBlock[__M2SChannel];
    if (!cb->is_attached || cb->is_active) {
        OSRestoreInterrupts(enabled);
        return;
    }

    if (!MICIsAttached(__M2SChannel)) {
        cb->is_attached = FALSE;
        OSRestoreInterrupts(enabled);
        return;
    }

    if ((__M2SMode & 1) == 0) {
        if (MICStart(__M2SChannel) != MIC_RESULT_READY) {
            OSRestoreInterrupts(enabled);
            return;
        }
        cb->sample_index = MICGetCurrentTop(__M2SChannel);
    } else {
        s32 ring_samples;
        s32 advance_samples;

        MICGetRingbuffsize(__M2SChannel, &ring_samples);
        ring_samples >>= 1;
        advance_samples = ring_samples - __M2SPrerecordingSamples;
        if (advance_samples < M2S_RESERVED_RING_SAMPLES) {
            OSRestoreInterrupts(enabled);
            return;
        }

        cb->sample_index = MICGetCurrentTop(__M2SChannel);
        cb->sample_index = MICUpdateIndex(__M2SChannel, cb->sample_index, advance_samples);
    }

    cb->is_active = TRUE;
    cb->calibrated = FALSE;
    cb->calibration_accumulator = 0;
    cb->calibration_history_index = 0;
    memset(cb->calibration_history, 0, M2S_CALIBRATION_HISTORY_BYTES);
    __M2SDebug.index = 0;
    OSRestoreInterrupts(enabled);
}

void M2SStop(void) {
    M2SControlBlock* cb;

    if (!__init || !__open) {
        return;
    }

    cb = &__M2SBlock[__M2SChannel];
    if (!cb->is_attached || !cb->is_active) {
        return;
    }

    if (!MICIsAttached(__M2SChannel)) {
        cb->is_active = FALSE;
        cb->is_attached = FALSE;
        return;
    }

    if ((__M2SMode & 2) == 0 && MICStop(__M2SChannel) != MIC_RESULT_READY) {
        return;
    }
    cb->is_active = FALSE;
}

s32 M2SGetSamples(s16** buffer, s32 samples) {
    M2SControlBlock* cb;

    if (!__init || !__open) {
        return -1;
    }
    if (buffer == NULL || samples < 0) {
        return -1;
    }

    cb = &__M2SBlock[__M2SChannel];
    if (!cb->is_attached || !cb->is_active) {
        return -1;
    }

    if (!MICIsAttached(__M2SChannel)) {
        cb->is_active = cb->is_attached = FALSE;
        return -1;
    }

    if (__M2SBuffer == NULL) {
        return -1;
    }

    if (samples > M2S_BUFFER_SAMPLES) {
        samples = M2S_BUFFER_SAMPLES;
    }
    samples = MICGetSamples(__M2SChannel, __M2SBuffer, cb->sample_index, samples);
    __M2SCalibration(__M2SBuffer, samples);
    *buffer = __M2SBuffer;
    return samples;
}

s32 M2SGetSamplesLeft(void) {
    M2SControlBlock* cb;

    if (!__init || !__open) {
        return -1;
    }

    cb = &__M2SBlock[__M2SChannel];
    if (!cb->is_attached || !cb->is_active) {
        return -1;
    }

    if (!MICIsAttached(__M2SChannel)) {
        cb->is_attached = FALSE;
        cb->is_active = FALSE;
        return -1;
    }

    return MICGetSamplesLeft(__M2SChannel, cb->sample_index);
}

void M2SAdvanceBuffer(s32 samples) {
    M2SControlBlock* cb;
    s32 index;
    s16* debug_buffer;

    if (!__init || !__open) {
        return;
    }
    if (samples < 0) {
        return;
    }

    cb = &__M2SBlock[__M2SChannel];
    if (!cb->is_attached || !cb->is_active) {
        return;
    }

    if (!MICIsAttached(__M2SChannel)) {
        cb->is_attached = FALSE;
        cb->is_active = FALSE;
        return;
    }

    cb->sample_index = MICUpdateIndex(__M2SChannel, cb->sample_index, samples);

    if (__M2SDebug.buffer != NULL) {
        s32 i;
        s32 debug_samples;

        index = __M2SDebug.index;
        debug_samples = __M2SDebug.samples;
        debug_buffer = &__M2SDebug.buffer[index];

        for (i = 0; i < samples; i++) {
            if (index >= debug_samples) {
                break;
            }
            *debug_buffer++ = __M2SBuffer[i];
            index++;
        }
        __M2SDebug.index = index;
    }

    if (!cb->calibrated) {
        cb->calibration_accumulator = __M2STmpAcc;
        cb->calibration_history_index = __M2STmpPtr;
        memcpy(cb->calibration_history, __M2STmpHistory,
               M2S_CALIBRATION_HISTORY_BYTES);
    }
}

static void __M2SCalibration(s16* buffer, s32 samples) {
    M2SControlBlock* cb = &__M2SBlock[__M2SChannel];
    s32 i;

    if (cb->shifts > 0) {
        for (i = 0; i < samples; i++) {
            buffer[i] <<= cb->shifts;
        }
    } else if (cb->shifts < 0) {
        for (i = 0; i < samples; i++) {
            buffer[i] >>= -cb->shifts;
        }
    }

    if (!cb->calibrated) {
        s32 threshold = cb->calibration_threshold;
        s32 accumulator = cb->calibration_accumulator;
        s32 history_index = cb->calibration_history_index;

        memcpy(__M2STmpHistory, cb->calibration_history,
               M2S_CALIBRATION_HISTORY_BYTES);
        for (i = 0; i < samples; i++) {
            s32 sample = buffer[i];
            s32 average = (sample * sample) / M2S_CALIBRATION_SAMPLES;

            accumulator -= __M2STmpHistory[history_index];
            accumulator += average;
            if (accumulator >= threshold) {
                cb->calibrated = TRUE;
                return;
            }

            buffer[i] = 0;
            __M2STmpHistory[history_index] = average;
            history_index++;
            if (history_index >= M2S_CALIBRATION_SAMPLES) {
                history_index = 0;
            }
        }

        __M2STmpAcc = accumulator;
        __M2STmpPtr = history_index;
    }
}

#pragma force_active reset
