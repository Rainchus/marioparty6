#ifndef PPC_GENERIC_TARGIMPL_H
#define PPC_GENERIC_TARGIMPL_H

#include "TRK_MINNOW_DOLPHIN/MetroTRK/Portable/nubevent.h"

void TRKTargetSetInputPendingPtr(void*);
void TRKSwapAndGo(void);
void TRKTargetSetStopped(u32);
DSError TRKTargetInterrupt(TRKEvent*);
DSError TRKTargetSupportRequest(void);
BOOL TRKTargetStopped(void);
DSError TRKTargetAddStopInfo(TRKBuffer*);
DSError TRKTargetAddExceptionInfo(TRKBuffer*);

#endif
