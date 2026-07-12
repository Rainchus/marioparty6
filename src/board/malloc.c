#include "game/memory.h"
#include "dolphin/os/OSCache.h"
#include "string.h"

void *mbMalloc(s32 size)
{
    void *ptr;

    ptr = HuMemDirectMallocNum(HEAP_HEAP, size, HU_MEMNUM_OVL);
    memset(ptr, 0, size);
    return ptr;
}

void *mbMallocNum(s32 size, u32 num)
{
    void *ptr;

    ptr = HuMemDirectMallocNum(HEAP_MODEL, size, num);
    memset(ptr, 0, size);
    return ptr;
}

void *mbMallocFlush(s32 size)
{
    void *ptr;

    ptr = HuMemDirectMallocNum(HEAP_HEAP, size, HU_MEMNUM_OVL);
    DCFlushRange(ptr, size);
    return ptr;
}

void *mbMallocFlushModel(s32 size)
{
    void *ptr;

    ptr = HuMemDirectMallocNum(HEAP_MODEL, size, HU_MEMNUM_OVL);
    DCFlushRange(ptr, size);
    return ptr;
}

void *mbMallocFlushModelNum(s32 size, u32 num)
{
    void *ptr;

    ptr = HuMemDirectMallocNum(HEAP_MODEL, size, num);
    DCFlushRange(ptr, size);
    return ptr;
}
