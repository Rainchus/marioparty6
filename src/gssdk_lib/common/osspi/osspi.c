#include "types.h"

#define OSSPI_INVALID_POINTER 0x80060183

u32 osspi_IsGoodReadPtr(const void *ptr, u32 size)
{
    if (ptr == NULL) {
        return OSSPI_INVALID_POINTER;
    }
    return 0;
}

u32 osspi_IsGoodWritePtr(void *ptr, u32 size)
{
    if (ptr == NULL) {
        return OSSPI_INVALID_POINTER;
    }
    return 0;
}
