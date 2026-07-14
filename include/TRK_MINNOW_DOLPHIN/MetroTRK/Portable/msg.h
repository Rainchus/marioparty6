#ifndef METROTRK_PORTABLE_MSG_H
#define METROTRK_PORTABLE_MSG_H

#include "PowerPC_EABI_Support/MetroTRK/trk.h"

typedef struct TRK_Msg {
    u32 mutex;
    BOOL isInUse;
    u32 m_msgLength;
    u32 position;
    u8 m_msg[TRKMSGBUF_SIZE];
} TRK_Msg;

DSError TRKMessageSend(TRK_Msg* msg);

#endif
