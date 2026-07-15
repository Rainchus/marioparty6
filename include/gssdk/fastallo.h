#ifndef GSSDK_FASTALLO_H
#define GSSDK_FASTALLO_H

#include "types.h"

typedef struct FastAllocator {
    void *heap;
    u16 chunkBytes;
    u16 wordsLeft;
    u32 chunkCount;
    void **chunks;
    void *current;
    void *freeLists[65];
} FastAllocator;

extern void *fastallo_AllocateMemoryFromChunk(
    FastAllocator *allocator, u32 words);
extern void fastallo_Init(
    FastAllocator *allocator, void *heap, u16 chunkBytes);
extern void fastallo_Terminate(FastAllocator *allocator);

#endif
