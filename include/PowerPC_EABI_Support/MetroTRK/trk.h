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

typedef enum DSFileHandle {
    DS_Stdin = 0,
    DS_Stdout = 1,
    DS_Stderr = 2,
} DSFileHandle;

typedef enum DSIOResult {
    DS_IONoError = 0,
    DS_IOError = 1,
    DS_IOEOF = 2,
} DSIOResult;

typedef enum MessageCommandID {
    DSMSG_NotifyStopped = 0x90,
    DSMSG_NotifyException = 0x91,
    DSMSG_WriteFile = 0xD0,
    DSMSG_ReadFile = 0xD1,
} MessageCommandID;

typedef int MessageBufferID;

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
