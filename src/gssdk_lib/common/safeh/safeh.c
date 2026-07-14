#include "gssdk/safeh.h"

#include <stddef.h>

#define SAFEH_INVALID_HANDLE 0x80170189

extern u32 osspi_IsGoodReadPtr(void *address, u32 size);
extern u32 osspi_IsGoodWritePtr(void *address, u32 size);

const SafeHandle g_lhNullHandle = { NULL, 0 };

static inline BOOL IsError(u32 result)
{
    return result >= 0x80000000 ? TRUE : FALSE;
}

u32 safeh_HandleCheck(SafeHandle handle, u32 type, u32 size)
{
    u32 result;

    if (handle.type != type) {
        return SAFEH_INVALID_HANDLE;
    }
    if (handle.address == NULL) {
        return SAFEH_INVALID_HANDLE;
    }

    result = osspi_IsGoodReadPtr(handle.address, size);
    if (IsError(result)) {
        return result;
    }
    return osspi_IsGoodWritePtr(handle.address, size);
}

BOOL safeh_HandlesEqual(SafeHandle left, SafeHandle right)
{
    if (left.type != right.type) {
        return FALSE;
    }
    if (left.address != right.address) {
        return FALSE;
    }
    return TRUE;
}
