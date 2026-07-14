#ifndef METROTRK_PORTABLE_SERPOLL_H
#define METROTRK_PORTABLE_SERPOLL_H

#include "PowerPC_EABI_Support/MetroTRK/trk.h"

DSError TRKInitializeSerialHandler(void);
DSError TRKTerminateSerialHandler(void);
void TRKGetInput(void);

extern void* gTRKInputPendingPtr;

#endif
