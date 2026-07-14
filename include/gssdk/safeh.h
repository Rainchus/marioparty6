#ifndef GSSDK_SAFEH_H
#define GSSDK_SAFEH_H

#include "types.h"

typedef struct SafeHandle {
    void *address;
    u32 type;
} SafeHandle;

extern const SafeHandle g_lhNullHandle;

u32 safeh_HandleCheck(SafeHandle handle, u32 type, u32 size);
BOOL safeh_HandlesEqual(SafeHandle left, SafeHandle right);

#endif
