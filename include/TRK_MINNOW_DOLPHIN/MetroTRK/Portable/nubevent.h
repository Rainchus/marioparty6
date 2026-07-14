#ifndef METROTRK_PORTABLE_NUBEVENT_H
#define METROTRK_PORTABLE_NUBEVENT_H

#include "PowerPC_EABI_Support/MetroTRK/trk.h"

typedef u32 NubEventID;

typedef struct TRKEvent {
    NubEventType eventType;
    NubEventID eventID;
    MessageBufferID msgBufID;
} TRKEvent;

typedef struct TRKEventQueue {
    int mutex;
    int count;
    int next;
    TRKEvent events[2];
    NubEventID eventID;
} TRKEventQueue;

extern TRKEventQueue gTRKEventQueue;

BOOL TRKGetNextEvent(TRKEvent* event);
void TRKDestructEvent(TRKEvent* event);
void TRKConstructEvent(TRKEvent* event, NubEventType eventType);
DSError TRKPostEvent(TRKEvent* event);
DSError TRKInitializeEventQueue(void);

#endif
