#ifndef METROTRK_PORTABLE_NUBEVENT_H
#define METROTRK_PORTABLE_NUBEVENT_H

#include "PowerPC_EABI_Support/MetroTRK/trk.h"

typedef enum NubEventType {
    NUBEVENT_Null = 0,
    NUBEVENT_Shutdown = 1,
    NUBEVENT_Request = 2,
    NUBEVENT_Breakpoint = 3,
    NUBEVENT_Exception = 4,
    NUBEVENT_Support = 5,
} NubEventType;

typedef u32 NubEventID;

typedef struct TRKEvent {
    NubEventType eventType;
    NubEventID eventID;
    MessageBufferID msgBufID;
} TRKEvent;

BOOL TRKGetNextEvent(TRKEvent* event);
void TRKDestructEvent(TRKEvent* event);
void TRKConstructEvent(TRKEvent* event, NubEventType eventType);
DSError TRKPostEvent(TRKEvent* event);
DSError TRKInitializeEventQueue(void);

#endif
