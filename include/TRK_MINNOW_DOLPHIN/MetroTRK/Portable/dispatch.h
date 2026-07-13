#ifndef METROTRK_PORTABLE_DISPATCH_H
#define METROTRK_PORTABLE_DISPATCH_H

#include "PowerPC_EABI_Support/MetroTRK/trk.h"

DSError TRKInitializeDispatcher(void);
DSError TRKDispatchMessage(TRKBuffer* buffer);

#endif
