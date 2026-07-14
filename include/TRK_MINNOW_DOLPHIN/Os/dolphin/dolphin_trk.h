#ifndef OS_DOLPHIN_DOLPHIN_TRK_H
#define OS_DOLPHIN_DOLPHIN_TRK_H

#include "PowerPC_EABI_Support/MetroTRK/trk.h"

DSError TRKInitializeTarget(void);
void __TRK_copy_vectors(void);

#endif
