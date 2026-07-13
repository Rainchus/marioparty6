#ifndef POWERPC_EABI_SUPPORT_METROTRK_TRK_H
#define POWERPC_EABI_SUPPORT_METROTRK_TRK_H

#include "dolphin/types.h"

enum {
    DS_NoError = 0x0,
    DS_NoMessageBufferAvailable = 0x300,
    DS_MessageBufferOverflow = 0x301,
    DS_MessageBufferReadError = 0x302,
    DS_DispatchError = 0x500,
};

typedef int DSError;

#define TRKMSGBUF_SIZE 0x880

typedef struct TRKBuffer {
    u32 mutex;
    BOOL isInUse;
    u32 length;
    u32 position;
    u8 data[TRKMSGBUF_SIZE];
} TRKBuffer;

void SetUseSerialIO(u8 useSerialIO);
u8 GetUseSerialIO(void);

#endif
