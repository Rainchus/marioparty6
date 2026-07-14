#ifndef OS_DOLPHIN_DOLPHIN_TRK_GLUE_H
#define OS_DOLPHIN_DOLPHIN_TRK_GLUE_H

#include "PowerPC_EABI_Support/MetroTRK/trk.h"

DSError TRKInitializeIntDrivenUART(u32, u32, u32, volatile u8**);
void UnreserveEXI2Port(void);
void ReserveEXI2Port(void);
void TRK_board_display(char*);

#endif
