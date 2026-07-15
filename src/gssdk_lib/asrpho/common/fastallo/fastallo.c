#include "types.h"

#include <string.h>

#include "gssdk/fastallo.h"

extern void *heap_Realloc(void *heap, void *ptr, u32 size);
extern void *heap_Alloc(void *heap, u32 size);
extern void heap_Free(void *heap, void *ptr);

void *fastallo_AllocateMemoryFromChunk(FastAllocator *allocator, u32 words)
{
    void *result = NULL;
    u32 chunk;

    if ((u16)words == 0 || (u16)words > 64) {
        goto done;
    }

    if (allocator->wordsLeft < (u16)words) {
        void **chunks;
        void *memory;

        chunk = allocator->chunkCount;
        if (allocator->wordsLeft != 0) {
            void **freeList = &allocator->freeLists[
                ((u32)allocator->wordsLeft + 3) >> 2];

            *(void **)allocator->current = *freeList;
            *freeList = allocator->current;
        }

        chunks = heap_Realloc(allocator->heap, allocator->chunks,
                              (chunk + 1) * sizeof(void *));
        if (chunks == NULL) {
            goto done;
        }
        allocator->chunks = chunks;

        memory = heap_Alloc(allocator->heap, allocator->chunkBytes);
        if (memory == NULL) {
            goto done;
        }
        allocator->chunks[chunk] = memory;
        allocator->chunkCount = ++chunk;
        allocator->current = memory;
        allocator->wordsLeft = (u32)allocator->chunkBytes >> 2;
    }

    result = allocator->current;
    allocator->wordsLeft -= (u16)words;
    allocator->current = (u8 *)allocator->current + ((u16)words * sizeof(u32));

done:
    return result;
}

void fastallo_Init(FastAllocator *allocator, void *heap, u16 chunkBytes)
{
    memset(allocator, 0, sizeof(FastAllocator));
    allocator->chunkBytes = chunkBytes;
    allocator->heap = heap;
}

void fastallo_Terminate(FastAllocator *allocator)
{
    u32 i;
    u32 chunkCount = allocator->chunkCount;

    for (i = 0; i < chunkCount; i++) {
        heap_Free(allocator->heap, allocator->chunks[i]);
    }
    if (allocator->chunks != NULL) {
        heap_Free(allocator->heap, allocator->chunks);
    }
}
