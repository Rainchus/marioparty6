#include <string.h>

#include <dolphin/exi.h>
#include <dolphin/mic.h>
#include <dolphin/os.h>

#pragma force_active on

#define MIC_EXI_ID 0x0A000000

#define MIC_DMA_DATA 0x20
#define MIC_READ_STATUS 0x40
#define MIC_WRITE_STATUS 0x80
#define MIC_RESET 0xFF

#define MIC_STATUS_BUFOVRFLW 0x0200
#define MIC_STATUS_GAIN15 0x0400
#define MIC_STATUS_22050HZ 0x0800
#define MIC_STATUS_44100HZ 0x1000
#define MIC_STATUS_64BYTES 0x2000
#define MIC_STATUS_128BYTES 0x4000
#define MIC_STATUS_ACTIVE 0x8000

typedef struct MICControlBlock {
    s32 result_code;
    BOOL is_attached;
    BOOL is_active;
    OSThreadQueue thread_queue;
    MICCallback exi_callback;
    MICCallback tx_callback;
    MICCallback detach_callback;
    MICCallback attach_callback;
    MICCallback mount_callback;
    MICCallback set_callback;
    u32 last_status;
    u32 status;
    u32 field_34;
    u32 error_count;
    s32 hw_buff_size;
    u32 sample_rate;
    u32 gain;
    s16* buff_ring_base;
    s32 buff_size;
    s32 buff_ring_size;
    s32 buff_ring_cur;
    u32 button;
    u32 last_button;
    u32 button_time_delta;
    OSTick button_time_last;
    s32 timeout;
} MICControlBlock;

static s32 __MICDoMount(s32 chan);
static void __MICDoUnmount(s32 chan, s32 result);
static void __MICUnlockedCallback(s32 chan, OSContext* context);
static void __MICMountCallback(s32 chan, s32 result);
static void __MICSetCallback(s32 chan, s32 result);
static s32 __MICSync(s32 chan);
static void __MICSyncCallback(s32 chan, s32 result);
static void __MICExtHandler(s32 chan, OSContext* context);
static void __MICExiHandler(s32 chan, OSContext* context);
static void __MICTxHandler(s32 chan, OSContext* context);
static void __MICAlarmCallback(OSAlarm* alarm, OSContext* context);
static void __MICTimeoutCallback(OSAlarm* alarm, OSContext* context);
static s32 __MICRawReset(s32 chan);
static s32 __MICRawReadStatus(s32 chan, u32* status);
static s32 __MICRawWriteStatus(s32 chan, u32 status);
static s32 __MICRawReadDataAsync(s32 chan, s16* data, u32 len, EXICallback callback);
static s32 __MICGetControlBlock(s32 chan, BOOL skip_active_check, MICControlBlock** micblock);
static void __MICPutControlBlock(MICControlBlock* micblock, s32 result);
static BOOL __MICUpdateStatus(s32 chan, u32 status, BOOL check_change);
static void __MICUpdateButton(s32 chan);

static MICControlBlock __MICBlock[2];
static OSAlarm __alarm;
static OSAlarm __timeout[2];
static BOOL __init = FALSE;

static u32 __MICTimeoutTable[4][4] = {
    {0xE58D, 0x1CB1B, 0x39636, 0x39636},
    {0x72B2, 0xE58D, 0x1CB1B, 0x1CB1B},
    {0x3945, 0x72B2, 0xE58D, 0xE58D},
    {0x3945, 0x72B2, 0xE58D, 0xE58D},
};

void MICInit(void) {
    if (__init == FALSE) {
        s32 i;

        for (i = 0; i < 2; i++) {
            __MICBlock[i].result_code = MIC_RESULT_NOCARD;
            __MICBlock[i].is_attached = FALSE;
            __MICBlock[i].is_active = FALSE;
            __MICBlock[i].exi_callback = NULL;
            __MICBlock[i].tx_callback = NULL;
            __MICBlock[i].detach_callback = NULL;
            __MICBlock[i].attach_callback = NULL;
            __MICBlock[i].mount_callback = NULL;
            __MICBlock[i].set_callback = NULL;
            __MICBlock[i].error_count = 0;
            OSInitThreadQueue(&__MICBlock[i].thread_queue);
        }

        OSCreateAlarm(&__alarm);
        OSSetPeriodicAlarm(&__alarm, OSGetTime(), OSMicrosecondsToTicks(5000),
                           __MICAlarmCallback);
        OSCreateAlarm(&__timeout[0]);
        OSCreateAlarm(&__timeout[1]);
        __init = TRUE;
    }
}

s32 MICProbeEx(s32 chan) {
    BOOL enabled;
    s32 probe;
    s32 result;

    if (chan < 0 || chan >= 2) {
        return MIC_RESULT_FATAL_ERROR;
    }

    enabled = OSDisableInterrupts();
    probe = EXIProbeEx(chan);

    if (probe == -1) {
        result = MIC_RESULT_NOCARD;
    } else if (probe == 0) {
        result = MIC_RESULT_BUSY;
    } else if (__MICBlock[chan].is_attached) {
        result = MIC_RESULT_READY;
    } else if (EXIGetState(chan) & EXI_STATE_ATTACHED) {
        result = MIC_RESULT_WRONGDEVICE;
    } else {
        BOOL id_matches;
        u32 id;

        if (!EXIGetID(chan, 0, &id)) {
            result = MIC_RESULT_BUSY;
        } else {
            if (id == MIC_EXI_ID) {
                id_matches = TRUE;
            } else {
                id_matches = FALSE;
            }
            if (id_matches) {
                result = MIC_RESULT_READY;
            } else {
                result = MIC_RESULT_WRONGDEVICE;
            }
        }
    }

    OSRestoreInterrupts(enabled);
    return result;
}

s32 MICMountAsync(s32 chan, s16* buffer, s32 size, MICCallback detachCallback,
                  MICCallback attachCallback) {
    MICControlBlock* cb;
    BOOL enabled;

    if (!__init || chan < 0 || chan >= 2) {
        return MIC_RESULT_FATAL_ERROR;
    }

    cb = &__MICBlock[chan];
    enabled = OSDisableInterrupts();
    if (cb->result_code == MIC_RESULT_BUSY) {
        OSRestoreInterrupts(enabled);
        return MIC_RESULT_BUSY;
    }

    if (cb->is_attached) {
        cb->result_code = MIC_RESULT_INVALID_STATE;
        OSRestoreInterrupts(enabled);
        return MIC_RESULT_INVALID_STATE;
    }

    if (!cb->is_attached && (EXIGetState(chan) & EXI_STATE_ATTACHED)) {
        cb->result_code = MIC_RESULT_WRONGDEVICE;
        OSRestoreInterrupts(enabled);
        return MIC_RESULT_WRONGDEVICE;
    }

    if (!cb->is_attached && !EXIAttach(chan, __MICExtHandler)) {
        cb->result_code = MIC_RESULT_NOCARD;
        OSRestoreInterrupts(enabled);
        return MIC_RESULT_NOCARD;
    }

    cb->result_code = MIC_RESULT_BUSY;
    cb->is_active = FALSE;
    cb->exi_callback = NULL;
    cb->tx_callback = NULL;
    cb->detach_callback = detachCallback;
    cb->attach_callback = attachCallback;
    cb->mount_callback = NULL;
    cb->set_callback = NULL;
    cb->buff_ring_base = (s16*)(((u32)buffer + 31) & ~31);
    cb->buff_size = size - ((u32)cb->buff_ring_base - (u32)buffer);
    cb->buff_ring_cur = 0;

    EXISetExiCallback(chan, NULL);
    OSRestoreInterrupts(enabled);

    cb->mount_callback = __MICMountCallback;
    if (!EXILock(chan, 0, __MICUnlockedCallback)) {
        return MIC_RESULT_READY;
    }
    cb->mount_callback = NULL;
    return __MICDoMount(chan);
}

s32 MICMount(s32 chan, s16* buffer, s32 size, MICCallback detachCallback) {
    s32 result = MICMountAsync(chan, buffer, size, detachCallback, __MICSyncCallback);

    if (result < MIC_RESULT_READY) {
        return result;
    }
    return __MICSync(chan);
}

s32 MICUnmount(s32 chan) {
    s32 result;
    MICControlBlock* cb;

    if (!__init || chan < 0 || chan >= 2) {
        return MIC_RESULT_FATAL_ERROR;
    }
    result = __MICGetControlBlock(chan, FALSE, &cb);
    if (result < MIC_RESULT_READY) {
        return result;
    }
    __MICDoUnmount(chan, MIC_RESULT_NOCARD);
    return MIC_RESULT_READY;
}

s32 MICGetRingbuffsize(s32 chan, s32* size) {
    MICControlBlock* cb;
    BOOL enabled;

    if (!__init || chan < 0 || chan >= 2 || size == NULL) {
        return MIC_RESULT_FATAL_ERROR;
    }

    cb = &__MICBlock[chan];
    enabled = OSDisableInterrupts();
    if (!cb->is_attached) {
        OSRestoreInterrupts(enabled);
        return MIC_RESULT_NOCARD;
    }

    *size = cb->buff_ring_size;
    OSRestoreInterrupts(enabled);
    return MIC_RESULT_READY;
}

s32 MICGetSampleRate(s32 chan, s32* rate) {
    MICControlBlock* cb;
    BOOL enabled;

    if (!__init || chan < 0 || chan >= 2 || rate == NULL) {
        return MIC_RESULT_FATAL_ERROR;
    }

    cb = &__MICBlock[chan];
    enabled = OSDisableInterrupts();
    if (!cb->is_attached) {
        OSRestoreInterrupts(enabled);
        return MIC_RESULT_NOCARD;
    }

    *rate = cb->sample_rate;
    OSRestoreInterrupts(enabled);
    return MIC_RESULT_READY;
}

s32 MICSetGain(s32 chan, s32 gain) {
    s32 result = MICSetGainAsync(chan, gain, __MICSyncCallback);

    if (result < MIC_RESULT_READY) {
        return result;
    }
    return __MICSync(chan);
}

s32 MICSetGainAsync(s32 chan, s32 gain, MICCallback callback) {
    MICControlBlock* cb;
    BOOL enabled;
    s32 result;
    u32 status;

    if (!__init || chan < 0 || chan >= 2 || (gain != 0 && gain != 15)) {
        return MIC_RESULT_FATAL_ERROR;
    }

    enabled = OSDisableInterrupts();
    result = __MICGetControlBlock(chan, FALSE, &cb);
    if (result < MIC_RESULT_READY) {
        OSRestoreInterrupts(enabled);
        return result;
    }

    status = cb->last_status & ~MIC_STATUS_GAIN15;
    switch (gain) {
        case 15:
            status |= MIC_STATUS_GAIN15;
            break;
        case 0:
            break;
    }

    cb->status = status;
    cb->attach_callback = callback;
    cb->set_callback = __MICSetCallback;
    OSRestoreInterrupts(enabled);
    return MIC_RESULT_READY;
}

s32 MICGetButton(s32 chan, u32* button) {
    MICControlBlock* cb;
    BOOL enabled;

    if (!__init || chan < 0 || chan >= 2 || button == NULL) {
        return MIC_RESULT_FATAL_ERROR;
    }

    cb = &__MICBlock[chan];
    enabled = OSDisableInterrupts();
    if (!cb->is_attached) {
        OSRestoreInterrupts(enabled);
        return MIC_RESULT_NOCARD;
    }

    *button = cb->button;
    OSRestoreInterrupts(enabled);
    return MIC_RESULT_READY;
}

s32 MICGetDeviceID(s32 chan, u32* id) {
    MICControlBlock* cb;
    BOOL enabled;

    if (!__init || chan < 0 || chan >= 2 || id == NULL) {
        return MIC_RESULT_FATAL_ERROR;
    }

    cb = &__MICBlock[chan];
    enabled = OSDisableInterrupts();
    if (!cb->is_attached) {
        OSRestoreInterrupts(enabled);
        return MIC_RESULT_NOCARD;
    }

    *id = (cb->last_status & 0x10) >> 4;
    OSRestoreInterrupts(enabled);
    return MIC_RESULT_READY;
}

s32 MICStart(s32 chan) {
    s32 result = MICStartAsync(chan, __MICSyncCallback);

    if (result < MIC_RESULT_READY) {
        return result;
    }
    return __MICSync(chan);
}

s32 MICStartAsync(s32 chan, MICCallback callback) {
    MICControlBlock* cb;
    BOOL enabled;
    s32 result;
    s32 rate;
    s32 size;

    if (!__init || chan < 0 || chan >= 2) {
        return MIC_RESULT_FATAL_ERROR;
    }

    enabled = OSDisableInterrupts();
    if ((result = __MICGetControlBlock(chan, FALSE, &cb)) < MIC_RESULT_READY) {
        OSRestoreInterrupts(enabled);
        return result;
    }

    cb->status = cb->last_status | MIC_STATUS_ACTIVE;
    cb->attach_callback = callback;
    cb->set_callback = __MICSetCallback;
    cb->error_count = 0;
    cb->buff_ring_cur = 0;

    rate = (cb->last_status >> 11) & 3;
    size = (cb->last_status >> 13) & 3;
    cb->timeout = __MICTimeoutTable[rate][size] * 92;

    memset(cb->buff_ring_base, 0, cb->buff_ring_size);
    OSRestoreInterrupts(enabled);
    return MIC_RESULT_READY;
}

s32 MICStop(s32 chan) {
    s32 result = MICStopAsync(chan, __MICSyncCallback);

    if (result < MIC_RESULT_READY) {
        return result;
    }
    return __MICSync(chan);
}

s32 MICStopAsync(s32 chan, MICCallback callback) {
    MICControlBlock* cb;
    BOOL enabled;
    s32 result;

    if (!__init || chan < 0 || chan >= 2) {
        return MIC_RESULT_FATAL_ERROR;
    }

    enabled = OSDisableInterrupts();
    if ((result = __MICGetControlBlock(chan, TRUE, &cb)) < MIC_RESULT_READY) {
        OSRestoreInterrupts(enabled);
        return result;
    }

    cb->status = cb->last_status & ~MIC_STATUS_ACTIVE;
    cb->attach_callback = callback;
    cb->set_callback = __MICSetCallback;
    OSRestoreInterrupts(enabled);
    return MIC_RESULT_READY;
}

s32 MICGetCurrentTop(s32 chan) {
    MICControlBlock* cb;
    BOOL enabled;
    s32 result;

    if (!__init || chan < 0 || chan >= 2) {
        return MIC_RESULT_BUSY;
    }

    cb = &__MICBlock[chan];
    enabled = OSDisableInterrupts();
    if (!cb->is_attached) {
        OSRestoreInterrupts(enabled);
        return MIC_RESULT_BUSY;
    }

    result = cb->buff_ring_cur >> 1;
    OSRestoreInterrupts(enabled);
    return result;
}

s32 MICUpdateIndex(s32 chan, s32 index, s32 samples) {
    MICControlBlock* cb;
    BOOL enabled;
    s32 samples_in_ring;

    if (!__init || chan < 0 || chan >= 2 || index < 0 || samples < 0) {
        return MIC_RESULT_BUSY;
    }

    cb = &__MICBlock[chan];
    enabled = OSDisableInterrupts();
    if (!cb->is_attached) {
        OSRestoreInterrupts(enabled);
        return MIC_RESULT_BUSY;
    }

    index += samples;
    samples_in_ring = cb->buff_ring_size >> 1;
    if (index >= samples_in_ring) {
        index -= samples_in_ring;
    }

    OSRestoreInterrupts(enabled);
    return index;
}

s32 MICGetSamplesLeft(s32 chan, s32 index) {
    MICControlBlock* cb;
    BOOL enabled;
    s32 current;
    s32 result;

    if (!__init || chan < 0 || chan >= 2 || index < 0) {
        return MIC_RESULT_BUSY;
    }

    cb = &__MICBlock[chan];
    enabled = OSDisableInterrupts();
    if (!cb->is_attached) {
        OSRestoreInterrupts(enabled);
        return MIC_RESULT_BUSY;
    }

    current = cb->buff_ring_cur >> 1;
    if (current >= index) {
        result = current - index;
    } else {
        result = (cb->buff_ring_size >> 1) - (index - current);
    }

    OSRestoreInterrupts(enabled);
    return result;
}

s32 MICGetSamples(s32 chan, s16* buffer, s32 index, s32 samples) {
    MICControlBlock* cb;
    BOOL enabled;
    s16* destination;
    s32 i;

    if (!__init || chan < 0 || chan >= 2 || buffer == NULL || index < 0 ||
        samples < 0) {
        return MIC_RESULT_BUSY;
    }

    cb = &__MICBlock[chan];
    enabled = OSDisableInterrupts();
    if (!cb->is_attached) {
        OSRestoreInterrupts(enabled);
        return MIC_RESULT_BUSY;
    }

    destination = buffer;
    index *= sizeof(s16);
    for (i = 0; i < samples; i++) {
        if (index >= cb->buff_ring_size) {
            index = 0;
        }
        if (index == cb->buff_ring_cur) {
            break;
        }
        *destination++ = *(s16*)((u8*)cb->buff_ring_base + index);
        index += sizeof(s16);
    }

    OSRestoreInterrupts(enabled);
    return i;
}

BOOL MICIsActive(s32 chan) {
    if (!__init || chan < 0 || chan >= 2) {
        return FALSE;
    }
    return __MICBlock[chan].is_active;
}

BOOL MICIsAttached(s32 chan) {
    if (!__init || chan < 0 || chan >= 2) {
        return FALSE;
    }
    return __MICBlock[chan].is_attached;
}

static s32 __MICDoMount(s32 chan) {
    s32 result;
    u32 id;
    u32 status;
    MICControlBlock* cb = &__MICBlock[chan];
    BOOL enabled = OSDisableInterrupts();

    cb->is_attached = TRUE;

    if (!EXIGetID(chan, 0, &id)) {
        result = MIC_RESULT_NOCARD;
    } else if (id != MIC_EXI_ID) {
        result = MIC_RESULT_WRONGDEVICE;
    } else {
        result = MIC_RESULT_READY;
    }

    if (result >= MIC_RESULT_READY &&
        (result = __MICRawReset(chan)) >= MIC_RESULT_READY &&
        (result = __MICRawReadStatus(chan, &status)) >= MIC_RESULT_READY) {
        __MICUpdateStatus(chan, status, FALSE);
        cb->button = 0;
        cb->last_button = 0;
        cb->button_time_delta = 0;
        cb->button_time_last = OSGetTick();

        if (EXIProbe(chan)) {
            EXISetExiCallback(chan, __MICExiHandler);
            EXIUnlock(chan);
            OSRestoreInterrupts(enabled);

            if (cb->attach_callback) {
                MICCallback attach = cb->attach_callback;
                cb->attach_callback = NULL;
                attach(chan, result);
            }
            __MICPutControlBlock(cb, result);
            return MIC_RESULT_READY;
        }
        result = MIC_RESULT_NOCARD;
    }

    EXIUnlock(chan);
    __MICDoUnmount(chan, result);
    OSRestoreInterrupts(enabled);
    return result;
}

static void __MICDoUnmount(s32 chan, s32 result) {
    MICControlBlock* cb = &__MICBlock[chan];
    MICCallback callback;
    BOOL enabled = OSDisableInterrupts();

    if (cb->is_attached) {
        EXISetExiCallback(chan, NULL);
        EXIDetach(chan);
        cb->result_code = result;
        cb->is_attached = FALSE;
        cb->error_count = 0;
    } else {
        cb->result_code = MIC_RESULT_NOCARD;
    }
    OSRestoreInterrupts(enabled);

    callback = cb->attach_callback;
    if (callback) {
        cb->attach_callback = NULL;
        callback(chan, result);
    }
}

static void __MICUnlockedCallback(s32 chan, OSContext* context) {
    MICControlBlock* cb = &__MICBlock[chan];
    MICCallback callback = cb->mount_callback;

    if (callback) {
        cb->mount_callback = NULL;
        callback(chan, EXIProbe(chan) ? MIC_RESULT_UNLOCKED : MIC_RESULT_NOCARD);
    }
}

static void __MICMountCallback(s32 chan, s32 result) {
    MICControlBlock* cb = &__MICBlock[chan];

    switch (result) {
    case MIC_RESULT_UNLOCKED:
        cb->mount_callback = __MICMountCallback;
        if (EXILock(chan, 0, __MICUnlockedCallback)) {
            cb->mount_callback = NULL;
            __MICDoMount(chan);
        }
        break;
    case MIC_RESULT_NOCARD:
        __MICDoUnmount(chan, result);
        break;
    }
}

static void __MICSetCallback(s32 chan, s32 result) {
    MICControlBlock* cb = &__MICBlock[chan];
    u32 status = cb->status;

    if (__MICRawWriteStatus(chan, status) >= MIC_RESULT_READY) {
        __MICUpdateStatus(chan, status, FALSE);
    }
}

static s32 __MICSync(s32 chan) {
    MICControlBlock* cb = &__MICBlock[chan];
    BOOL enabled = OSDisableInterrupts();

    while (cb->result_code == MIC_RESULT_BUSY) {
        OSSleepThread(&cb->thread_queue);
    }
    OSRestoreInterrupts(enabled);
    return cb->result_code;
}

static void __MICSyncCallback(s32 chan, s32 result) {
    OSWakeupThread(&__MICBlock[chan].thread_queue);
}

static void __MICExtHandler(s32 chan, OSContext* context) {
    MICControlBlock* cb = &__MICBlock[chan];

    if (cb->is_attached) {
        MICCallback callback;

        EXISetExiCallback(chan, NULL);
        cb->result_code = MIC_RESULT_NOCARD;
        cb->is_attached = FALSE;
        cb->is_active = FALSE;
        cb->error_count = 0;

        callback = cb->attach_callback;
        if (callback) {
            cb->attach_callback = NULL;
            callback(chan, MIC_RESULT_NOCARD);
        }

        callback = cb->detach_callback;
        if (callback) {
            cb->detach_callback = NULL;
            callback(chan, MIC_RESULT_NOCARD);
        }
    }
}

static void __MICExiHandler(s32 chan, OSContext* context) {
    MICControlBlock* cb;
    s32 result = MIC_RESULT_FATAL_ERROR;

    OSCancelAlarm(&__timeout[chan]);
    cb = &__MICBlock[chan];

    if (cb->is_attached && cb->is_active) {
        if (EXILock(chan, 0, NULL)) {
            u32 status;

            if ((result = __MICRawReadStatus(chan, &status)) >= MIC_RESULT_READY &&
                __MICUpdateStatus(chan, status, TRUE)) {
                if (status & MIC_STATUS_BUFOVRFLW) {
                    cb->error_count++;
                    status &= ~MIC_STATUS_BUFOVRFLW;
                }

                __MICUpdateButton(chan);
                if (cb->set_callback) {
                    cb->set_callback(chan, MIC_RESULT_READY);
                }

                if (cb->is_active) {
                    result = __MICRawReadDataAsync(
                        chan, (s16*)((u8*)cb->buff_ring_base + cb->buff_ring_cur),
                        cb->hw_buff_size, __MICTxHandler);
                    if (result >= MIC_RESULT_READY) {
                        goto skip_unlock;
                    }
                } else if (!EXIProbe(chan)) {
                    result = MIC_RESULT_NOCARD;
                }
            }
            EXIUnlock(chan);
        }

    skip_unlock:
        if (cb->exi_callback) {
            cb->exi_callback(chan, result);
        }

        if (cb->set_callback) {
            cb->set_callback = NULL;
            if (cb->attach_callback) {
                cb->attach_callback(chan, result);
                cb->attach_callback = NULL;
            }
            __MICPutControlBlock(cb, result);
        }
    }
}

static void __MICTxHandler(s32 chan, OSContext* context) {
    s32 result;
    MICControlBlock* cb = &__MICBlock[chan];
    MICCallback callback;

    cb->buff_ring_cur += cb->hw_buff_size;
    if (cb->buff_ring_cur >= cb->buff_ring_size) {
        cb->buff_ring_cur = 0;
    }

    if (!EXIDeselect(chan)) {
        result = MIC_RESULT_NOCARD;
        goto unlock;
    }

    {
        u32 status;

        result = __MICRawReadStatus(chan, &status);
        if (result < MIC_RESULT_READY) {
            goto unlock;
        }
        if (!__MICUpdateStatus(chan, status, TRUE)) {
            goto unlock;
        }
        if (status & MIC_STATUS_BUFOVRFLW) {
            cb->error_count++;
            status &= ~MIC_STATUS_BUFOVRFLW;
        }

        __MICUpdateButton(chan);
        if (!EXIProbe(chan)) {
            result = MIC_RESULT_NOCARD;
            goto unlock;
        }
        OSSetAlarm(&__timeout[chan], cb->timeout, __MICTimeoutCallback);
        result = MIC_RESULT_READY;
    }

unlock:
    EXIUnlock(chan);
    callback = cb->tx_callback;
    if (callback) {
        callback(chan, result);
    }
}

static void __MICAlarmCallback(OSAlarm* alarm, OSContext* context) {
    s32 chan;

    for (chan = 0; chan < 2; chan++) {
        MICControlBlock* cb = &__MICBlock[chan];

        if (cb->is_attached && !cb->is_active && EXILock(chan, 0, NULL)) {
            u32 status;
            s32 result;

            if ((result = __MICRawReadStatus(chan, &status)) >= MIC_RESULT_READY &&
                __MICUpdateStatus(chan, status, TRUE)) {
                __MICUpdateButton(chan);
                if (cb->set_callback) {
                    cb->set_callback(chan, result);
                }
            }

            EXIUnlock(chan);
            if (cb->set_callback) {
                cb->set_callback = NULL;
                if (cb->attach_callback) {
                    cb->attach_callback(chan, result);
                    cb->attach_callback = NULL;
                }
                __MICPutControlBlock(cb, result);
            }
        }
    }
}

static void __MICTimeoutCallback(OSAlarm* alarm, OSContext* context) {
    s32 chan;
    MICControlBlock* cb;

    if (alarm == &__timeout[0]) {
        chan = 0;
    } else {
        chan = 1;
    }
    cb = &__MICBlock[chan];

    if (cb->is_attached) {
        u32 status;

        if (!EXILock(chan, 0, NULL)) {
            OSSetAlarm(&__timeout[chan], cb->timeout, __MICTimeoutCallback);
            return;
        }

        cb->is_active = FALSE;
        __MICRawReset(chan);
        __MICRawReadStatus(chan, &status);
        __MICUpdateStatus(chan, status, FALSE);
        EXIUnlock(chan);
    }
}

static s32 __MICRawReset(s32 chan) {
    BOOL enabled = OSDisableInterrupts();
    BOOL failed;
    u16 command;

    if (!EXISelect(chan, 0, EXI_FREQ_16M)) {
        OSRestoreInterrupts(enabled);
        return MIC_RESULT_NOCARD;
    }

    failed = FALSE;
    command = MIC_RESET << 8;
    failed |= !EXIImm(chan, &command, 1, EXI_WRITE, NULL);
    failed |= !EXISync(chan);
    failed |= !EXIDeselect(chan);
    OSRestoreInterrupts(enabled);

    if (failed) {
        return MIC_RESULT_NOCARD;
    }
    return MIC_RESULT_READY;
}

static s32 __MICRawReadStatus(s32 chan, u32* status) {
    BOOL enabled = OSDisableInterrupts();
    BOOL failed;
    u16 command;
    u16 data;

    if (!EXISelect(chan, 0, EXI_FREQ_16M)) {
        OSRestoreInterrupts(enabled);
        return MIC_RESULT_NOCARD;
    }

    failed = FALSE;
    command = MIC_READ_STATUS << 8;
    failed |= !EXIImm(chan, &command, 1, EXI_WRITE, NULL);
    failed |= !EXISync(chan);
    failed |= !EXIImm(chan, &data, 2, EXI_READ, NULL);
    failed |= !EXISync(chan);
    failed |= !EXIDeselect(chan);
    *status = data;
    OSRestoreInterrupts(enabled);

    if (failed) {
        return MIC_RESULT_NOCARD;
    }
    return MIC_RESULT_READY;
}

static s32 __MICRawWriteStatus(s32 chan, u32 status) {
    BOOL enabled = OSDisableInterrupts();
    BOOL failed;
    u32 data;

    if (!EXISelect(chan, 0, EXI_FREQ_16M)) {
        OSRestoreInterrupts(enabled);
        return MIC_RESULT_NOCARD;
    }

    failed = FALSE;
    data = MIC_WRITE_STATUS << 24;
    data |= (status & 0xFFFF) << 8;
    failed |= !EXIImm(chan, &data, 3, EXI_WRITE, NULL);
    failed |= !EXISync(chan);
    failed |= !EXIDeselect(chan);
    OSRestoreInterrupts(enabled);

    if (failed) {
        return MIC_RESULT_NOCARD;
    }
    return MIC_RESULT_READY;
}

static s32 __MICRawReadDataAsync(s32 chan, s16* data, u32 len, EXICallback callback) {
    BOOL enabled;
    BOOL failed;
    u16 command;

    DCInvalidateRange(data, len);
    enabled = OSDisableInterrupts();

    if (!EXISelect(chan, 0, EXI_FREQ_16M)) {
        OSRestoreInterrupts(enabled);
        return MIC_RESULT_NOCARD;
    }

    failed = FALSE;
    command = MIC_DMA_DATA << 8;
    failed |= !EXIImm(chan, &command, 1, EXI_WRITE, NULL);
    failed |= !EXISync(chan);
    failed |= !EXIDma(chan, data, len, EXI_READ, callback);

    if (failed) {
        EXIDeselect(chan);
        OSRestoreInterrupts(enabled);
        return MIC_RESULT_NOCARD;
    }

    OSRestoreInterrupts(enabled);
    return MIC_RESULT_READY;
}

static s32 __MICGetControlBlock(s32 chan, BOOL skip_active_check, MICControlBlock** micblock) {
    s32 result;
    MICControlBlock* cb = &__MICBlock[chan];
    BOOL enabled = OSDisableInterrupts();

    if (!cb->is_attached) {
        result = MIC_RESULT_NOCARD;
    } else if (!skip_active_check && cb->is_active) {
        result = MIC_RESULT_INVALID_STATE;
    } else if (cb->result_code == MIC_RESULT_BUSY) {
        result = MIC_RESULT_BUSY;
    } else {
        cb->result_code = MIC_RESULT_BUSY;
        cb->attach_callback = NULL;
        result = MIC_RESULT_READY;
        *micblock = cb;
    }

    OSRestoreInterrupts(enabled);
    return result;
}

static void __MICPutControlBlock(MICControlBlock* micblock, s32 result) {
    BOOL enabled = OSDisableInterrupts();

    if (micblock->is_attached) {
        micblock->result_code = result;
    } else if (micblock->result_code == MIC_RESULT_BUSY) {
        micblock->result_code = result;
    }
    OSRestoreInterrupts(enabled);
}

static BOOL __MICUpdateStatus(s32 chan, u32 status, BOOL check_change) {
    MICControlBlock* cb = &__MICBlock[chan];
    BOOL enabled = OSDisableInterrupts();

    switch (status & (MIC_STATUS_64BYTES | MIC_STATUS_128BYTES)) {
        case 0:
            cb->hw_buff_size = 32;
            break;
        case MIC_STATUS_64BYTES:
            cb->hw_buff_size = 64;
            break;
        default:
            cb->hw_buff_size = 128;
            break;
    }

    switch (status & (MIC_STATUS_22050HZ | MIC_STATUS_44100HZ)) {
        case 0:
            cb->sample_rate = 11025;
            break;
        case MIC_STATUS_22050HZ:
            cb->sample_rate = 22050;
            break;
        default:
            cb->sample_rate = 44100;
            break;
    }

    if (status & MIC_STATUS_GAIN15) {
        cb->gain = 15;
    } else {
        cb->gain = 0;
    }
    cb->buff_ring_size = cb->hw_buff_size * (cb->buff_size / cb->hw_buff_size);

    if (status & MIC_STATUS_ACTIVE) {
        if (!cb->is_active) {
            OSSetAlarm(&__timeout[chan], cb->timeout, __MICTimeoutCallback);
        }
        cb->is_active = TRUE;
    } else {
        cb->is_active = FALSE;
    }

    if (check_change && ((cb->last_status ^ status) & 0xFC0F)) {
        cb->is_active = FALSE;
        __MICRawReset(chan);
        __MICRawReadStatus(chan, &status);
        __MICUpdateStatus(chan, status, FALSE);
        OSRestoreInterrupts(enabled);
        return FALSE;
    }

    cb->last_status = status;
    OSRestoreInterrupts(enabled);
    return TRUE;
}

static void __MICUpdateButton(s32 chan) {
    MICControlBlock* cb;
    BOOL enabled;
    OSTick ticks;
    OSTick delta;
    u32 button_bits;
    u32 button;
    u32 button_changed;
    u32 new_button;

    cb = &__MICBlock[chan];
    enabled = OSDisableInterrupts();
    ticks = OSGetTick();
    delta = ticks - cb->button_time_last + cb->button_time_delta;

    if (delta >= 10000) {
        button_bits = (cb->last_status >> 4) & 0x1F;

        button_bits &= ~1;
        button = cb->button;
        button_changed = cb->last_button ^ button;
        new_button =
            (button_bits & ~button_changed) | (button_changed & button);
        cb->button = new_button;
        cb->last_button = button_bits;
        delta = 0;
    }

    cb->button_time_last = ticks;
    cb->button_time_delta = delta;
    OSRestoreInterrupts(enabled);
}

#pragma force_active reset
