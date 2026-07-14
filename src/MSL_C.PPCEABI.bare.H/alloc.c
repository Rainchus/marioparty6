#include "PowerPC_EABI_Support/Msl/MSL_C/MSL_Common/alloc.h"

#include <string.h>

typedef struct Block {
    struct Block* prev;
    struct Block* next;
    unsigned long max_size;
    unsigned long size;
} Block;

typedef struct SubBlock {
    unsigned long size;
    Block* block;
    struct SubBlock* prev;
    struct SubBlock* next;
} SubBlock;

struct FixSubBlock;

typedef struct FixBlock {
    struct FixBlock* prev;
    struct FixBlock* next;
    unsigned long client_size;
    struct FixSubBlock* start;
    unsigned long allocated;
} FixBlock;

typedef struct FixSubBlock {
    FixBlock* block;
    struct FixSubBlock* next;
} FixSubBlock;

typedef struct FixStart {
    FixBlock* tail;
    FixBlock* head;
} FixStart;

typedef struct MemPoolObject {
    Block* start;
    FixStart fixed[6];
} MemPoolObject;

typedef struct MemPool {
    void* reserved[14];
} MemPool;

static int initialized;
static const unsigned long fixedPoolSizes[] = {4, 12, 20, 36, 52, 68};

#define initializedFlag (*(unsigned char*)&initialized)
#define SubBlock_size(block) ((block)->size & 0xFFFFFFF8)
#define SubBlock_block(subBlock) \
    ((Block*)((unsigned long)((subBlock)->block) & ~1))
#define Block_size(block) ((block)->size & 0xFFFFFFF8)
#define Block_start(block) \
    (*(SubBlock**)((char*)(block) + Block_size(block) - sizeof(unsigned long)))
#define SubBlock_is_free(block) (!((block)->size & 2))
#define SubBlock_from_pointer(ptr) ((SubBlock*)((char*)(ptr) - 8))
#define FixSubBlock_from_pointer(ptr) ((FixSubBlock*)((char*)(ptr) - 4))
#define classify(ptr) \
    (*(unsigned long*)((char*)(ptr) - sizeof(unsigned long)) & 1)
#define allocation_size(ptr) \
    (!classify(ptr) \
         ? FixSubBlock_from_pointer(ptr)->block->client_size \
         : SubBlock_size(SubBlock_from_pointer(ptr)) - 8)
#define Block_empty(block) \
    (first = (SubBlock*)((char*)(block) + 16)), \
        SubBlock_is_free(first) \
            && SubBlock_size(first) == Block_size(block) - 24

extern void* __sys_alloc(unsigned long size);
extern void __sys_free(void* ptr);

static void SubBlock_merge_next(SubBlock* block, SubBlock** start);
static void Block_link(Block* block, SubBlock* subBlock);
static SubBlock* Block_subBlock(Block* block, unsigned long size);

#define SubBlock_set_size(subBlock, sz)                                \
    (subBlock)->size &= ~0xFFFFFFF8;                                  \
    (subBlock)->size |= (sz) & 0xFFFFFFF8;                            \
    if (SubBlock_is_free(subBlock))                                   \
        *(unsigned long*)((char*)(subBlock) + (sz) -                  \
                          sizeof(unsigned long)) = (sz)

static SubBlock* SubBlock_merge_prev(SubBlock* block, SubBlock** start)
{
    unsigned long previousSize;
    SubBlock* previous;

    if (!(block->size & 4)) {
        previousSize = *(unsigned long*)((char*)block - sizeof(unsigned long));
        if (previousSize & 2) {
            return block;
        }

        previous = (SubBlock*)((char*)block - previousSize);
        SubBlock_set_size(
            previous, previousSize + SubBlock_size(block));

        if (*start == block) {
            *start = (*start)->next;
        }
        block->next->prev = block->prev;
        block->next->prev->next = block->next;
        return previous;
    }
    return block;
}

static void SubBlock_merge_next(SubBlock* block, SubBlock** start)
{
    SubBlock* next;
    unsigned long size;

    next = (SubBlock*)((char*)block + SubBlock_size(block));
    if (!SubBlock_is_free(next)) {
        return;
    }

    size = SubBlock_size(block) + SubBlock_size(next);
    SubBlock_set_size(block, size);

    if (SubBlock_is_free(block)) {
        *(unsigned long*)((char*)block + size) &= ~4;
    } else {
        *(unsigned long*)((char*)block + size) |= 4;
    }

    if (*start == next) {
        *start = (*start)->next;
    }
    if (*start == next) {
        *start = 0;
    }

    next->next->prev = next->prev;
    next->prev->next = next->next;
}

static void Block_link(Block* block, SubBlock* subBlock)
{
    unsigned long size;
    SubBlock** start;

    size = SubBlock_size(subBlock);
    subBlock->size &= ~2;
    *(unsigned long*)((char*)subBlock + size) &= ~4;
    *(unsigned long*)((char*)subBlock + size - sizeof(unsigned long)) = size;

    start = &Block_start(block);
    if (*start != 0) {
        subBlock->prev = (*start)->prev;
        subBlock->prev->next = subBlock;
        subBlock->next = *start;
        (*start)->prev = subBlock;
        *start = subBlock;
        *start = SubBlock_merge_prev(*start, start);
        SubBlock_merge_next(*start, start);
    } else {
        *start = subBlock;
        subBlock->prev = subBlock;
        subBlock->next = subBlock;
    }

    if (block->max_size < SubBlock_size(*start)) {
        block->max_size = SubBlock_size(*start);
    }
}

static void SubBlock_construct(
    SubBlock* block, unsigned long size, Block* owner,
    int previousAllocated, int allocated)
{
    block->block = (Block*)((unsigned long)owner | 1);
    block->size = size;
    if (previousAllocated) {
        block->size |= 4;
    }
    if (allocated) {
        block->size |= 2;
        *(unsigned long*)((char*)block + size) |= 4;
    } else {
        *(unsigned long*)((char*)block + size - sizeof(unsigned long)) = size;
    }
}

static SubBlock* SubBlock_split(SubBlock* block, unsigned long size)
{
    unsigned long oldSize;
    int wasFree;
    int previousAllocated;
    SubBlock* right;
    Block* owner;

    oldSize = SubBlock_size(block);
    wasFree = SubBlock_is_free(block);
    previousAllocated = block->size & 4;
    right = (SubBlock*)((char*)block + size);
    owner = SubBlock_block(block);

    SubBlock_construct(block, size, owner, previousAllocated, !wasFree);
    SubBlock_construct(right, oldSize - size, owner, !wasFree, !wasFree);
    if (wasFree) {
        right->next = block->next;
        right->next->prev = right;
        right->prev = block;
        block->next = right;
    }
    return right;
}

static void Block_unlink(Block* block, SubBlock* subBlock)
{
    unsigned long size;
    SubBlock** start;

    size = subBlock->size;
    subBlock->size = size | 2;
    size &= ~7;
    *(unsigned long*)((char*)subBlock + size) |= 4;

    start = &Block_start(block);
    if (*start == subBlock) {
        *start = subBlock->next;
    }
    if (*start == subBlock) {
        *start = 0;
        block->max_size = 0;
    } else {
        subBlock->next->prev = subBlock->prev;
        subBlock->prev->next = subBlock->next;
    }
}

static SubBlock* Block_subBlock(Block* block, unsigned long size)
{
    SubBlock* subBlock;
    SubBlock* start;
    unsigned long currentSize;
    unsigned long maximumSize;

    start = Block_start(block);
    if (start == 0) {
        block->max_size = 0;
        return 0;
    }

    subBlock = start;
    currentSize = SubBlock_size(subBlock);
    maximumSize = currentSize;
    while (currentSize < size) {
        subBlock = subBlock->next;
        currentSize = SubBlock_size(subBlock);
        if (maximumSize < currentSize) {
            maximumSize = currentSize;
        }
        if (subBlock == start) {
            block->max_size = maximumSize;
            return 0;
        }
    }

    if (currentSize - size >= 0x50) {
        SubBlock_split(subBlock, size);
    }

    Block_start(block) = subBlock->next;
    Block_unlink(block, subBlock);
    return subBlock;
}

static void Block_construct(Block* block, unsigned long size)
{
    SubBlock* subBlock;

    block->size = size | 3;
    *(unsigned long*)((char*)block + size - 8) = block->size;
    subBlock = (SubBlock*)((char*)block + 16);
    SubBlock_construct(subBlock, size - 24, block, 0, 0);
    block->max_size = size - 24;
    Block_start(block) = 0;
    Block_link(block, subBlock);
}

static void link_block(MemPoolObject* pool, Block* block)
{
    if (pool->start != 0) {
        block->prev = pool->start->prev;
        block->prev->next = block;
        block->next = pool->start;
        pool->start->prev = block;
        pool->start = block;
    } else {
        pool->start = block;
        block->prev = block;
        block->next = block;
    }
}

static Block* unlink_block(MemPoolObject* pool, Block* block)
{
    Block* next;

    next = block->next;
    if (next == block) {
        next = 0;
    }
    if (pool->start == block) {
        pool->start = next;
    }
    if (next != 0) {
        next->prev = block->prev;
        next->prev->next = next;
    }
    block->next = 0;
    block->prev = 0;
    return next;
}

static Block* link_new_block(MemPoolObject* pool, unsigned long size)
{
    Block* block;

    size = (size + 0x1F) & ~7;
    if (size < 0x10000) {
        size = 0x10000;
    }

    block = __sys_alloc(size);
    if (block == 0) {
        return 0;
    }

    Block_construct(block, size);
    link_block(pool, block);
    return block;
}

static void* allocate_from_var_pools(
    MemPoolObject* pool, unsigned long size)
{
    Block* block;
    SubBlock* subBlock;

    size = (size + 0xF) & ~7;
    if (size < 0x50) {
        size = 0x50;
    }

    block = pool->start != 0 ? pool->start : link_new_block(pool, size);
    if (block == 0) {
        return 0;
    }

    do {
        if (size <= block->max_size) {
            subBlock = Block_subBlock(block, size);
            if (subBlock != 0) {
                pool->start = block;
                goto allocated;
            }
        }
        block = block->next;
    } while (block != pool->start);

    block = link_new_block(pool, size);
    if (block == 0) {
        return 0;
    }
    subBlock = Block_subBlock(block, size);

allocated:
    return (char*)subBlock + 8;
}

static void* soft_allocate_from_var_pools(
    MemPoolObject* pool, unsigned long size,
    unsigned long* maximumFreeSize)
{
    Block* block;
    SubBlock* subBlock;

    size = (size + 0xF) & ~7;
    if (size < 0x50) {
        size = 0x50;
    }

    *maximumFreeSize = 0;
    block = pool->start;
    if (block == 0) {
        return 0;
    }

    do {
        if (size <= block->max_size) {
            subBlock = Block_subBlock(block, size);
            if (subBlock != 0) {
                pool->start = block;
                goto allocated;
            }
        }
        if (block->max_size > 8
            && *maximumFreeSize < block->max_size - 8) {
            *maximumFreeSize = block->max_size - 8;
        }
        block = block->next;
    } while (block != pool->start);

    return 0;

allocated:
    return (char*)subBlock + 8;
}

static void deallocate_from_var_pools(MemPoolObject* pool, void* ptr)
{
    SubBlock* subBlock;
    SubBlock* first;
    Block* block;

    subBlock = SubBlock_from_pointer(ptr);
    block = SubBlock_block(subBlock);
    Block_link(block, subBlock);

    if (Block_empty(block)) {
        unlink_block(pool, block);
        __sys_free(block);
    }
}

static void FixBlock_construct(
    FixBlock* block, FixBlock* previous, FixBlock* next,
    unsigned long poolIndex, FixSubBlock* first,
    unsigned long bufferSize)
{
    unsigned long entrySize;
    unsigned long count;
    unsigned long i;
    FixSubBlock* entry;
    FixSubBlock* nextEntry;

    block->prev = previous;
    block->next = next;
    previous->next = block;
    next->prev = block;
    block->client_size = fixedPoolSizes[poolIndex];

    entrySize = fixedPoolSizes[poolIndex] + 4;
    count = bufferSize / entrySize;
    entry = first;
    for (i = 0; i < count - 1; i++) {
        nextEntry = (FixSubBlock*)((char*)entry + entrySize);
        entry->block = block;
        entry->next = nextEntry;
        entry = nextEntry;
    }
    entry->block = block;
    entry->next = 0;
    block->start = first;
    block->allocated = 0;
}

static void* allocate_from_fixed_pools(
    MemPoolObject* pool, unsigned long size)
{
    unsigned long poolIndex;
    FixStart* fixed;

    poolIndex = 0;
    while (size > fixedPoolSizes[poolIndex]) {
        poolIndex++;
    }

    fixed = &pool->fixed[poolIndex];
    if (fixed->head == 0 || fixed->head->start == 0) {
        const unsigned long* poolSizes;
        unsigned long count;
        unsigned long maximumCount;
        void* memory;
        unsigned long maximumFreeSize;
        unsigned long memorySize;

        poolSizes = fixedPoolSizes;
        count = 0xFEC / (poolSizes[poolIndex] + 4);
        if (count > 0x100) {
            count = 0x100;
        }
        maximumCount = count;

        while (count >= 10) {
            memory = soft_allocate_from_var_pools(
                pool, count * (poolSizes[poolIndex] + 4) + 0x14,
                &maximumFreeSize);
            if (memory != 0) {
                break;
            }
            if (maximumFreeSize > 0x14) {
                count = (maximumFreeSize - 0x14)
                    / (poolSizes[poolIndex] + 4);
            } else {
                count = 0;
            }
        }

        if (memory == 0 && count < maximumCount) {
            memory = allocate_from_var_pools(
                pool, maximumCount * (poolSizes[poolIndex] + 4) + 0x14);
            if (memory == 0) {
                return 0;
            }
        }

        memorySize = allocation_size(memory);
        if (fixed->head == 0) {
            fixed->head = memory;
            fixed->tail = memory;
        }

        FixBlock_construct(
            memory, fixed->tail, fixed->head, poolIndex,
            (FixSubBlock*)((char*)memory + 0x14), memorySize - 0x14);
        fixed->head = memory;
    }

    {
        FixSubBlock* entry;

        entry = fixed->head->start;
        fixed->head->start = entry->next;
        fixed->head->allocated++;
        if (fixed->head->start == 0) {
            fixed->head = fixed->head->next;
            fixed->tail = fixed->tail->next;
        }
        return (char*)entry + 4;
    }
}

static void deallocate_from_fixed_pools(
    MemPoolObject* pool, void* ptr, unsigned long size)
{
    unsigned long poolIndex;
    FixSubBlock* entry;
    FixBlock* block;
    FixStart* fixed;

    poolIndex = 0;
    while (size > fixedPoolSizes[poolIndex]) {
        poolIndex++;
    }

    fixed = &pool->fixed[poolIndex];
    entry = FixSubBlock_from_pointer(ptr);
    block = entry->block;

    if (block->start == 0 && fixed->head != block) {
        if (fixed->tail == block) {
            fixed->head = fixed->head->prev;
            fixed->tail = fixed->tail->prev;
        } else {
            block->prev->next = block->next;
            block->next->prev = block->prev;
            block->next = fixed->head;
            block->prev = block->next->prev;
            block->prev->next = block;
            block->next->prev = block;
            fixed->head = block;
        }
    }

    entry->next = block->start;
    block->start = entry;
    if (--block->allocated == 0) {
        if (fixed->head == block) {
            fixed->head = block->next;
        }
        if (fixed->tail == block) {
            fixed->tail = block->prev;
        }

        block->prev->next = block->next;
        block->next->prev = block->prev;

        if (fixed->head == block) {
            fixed->head = 0;
        }
        if (fixed->tail == block) {
            fixed->tail = 0;
        }
        deallocate_from_var_pools(pool, block);
    }
}

static void initialize_pool(MemPool* pool)
{
    memset(pool, 0, sizeof(MemPoolObject));
}

static MemPool* get_malloc_pool(void)
{
    static MemPool pool;

    if (!initializedFlag) {
        initialize_pool(&pool);
        initializedFlag = 1;
    }
    return &pool;
}

static void* pool_allocate(MemPool* pool, unsigned long size)
{
    MemPoolObject* object;

    if (size == 0 || size > 0xFFFFFFCF) {
        return 0;
    }

    object = (MemPoolObject*)pool;
    if (size <= 68) {
        return allocate_from_fixed_pools(object, size);
    }
    return allocate_from_var_pools(object, size);
}

static void pool_free(MemPool* pool, void* ptr)
{
    MemPoolObject* object;
    unsigned long size;

    if (ptr == 0) {
        return;
    }

    object = (MemPoolObject*)pool;
    size = allocation_size(ptr);
    if (size <= 68) {
        deallocate_from_fixed_pools(object, ptr, size);
    } else {
        deallocate_from_var_pools(object, ptr);
    }
}

static void* pool_allocate_clear(MemPool* pool, unsigned long size)
{
    void* ptr;

    ptr = pool_allocate(pool, size);
    if (ptr != 0) {
        memset(ptr, 0, size);
    }
    return ptr;
}

void* calloc(size_t count, size_t size)
{
    return pool_allocate_clear(get_malloc_pool(), count * size);
}

void free(void* ptr)
{
    pool_free(get_malloc_pool(), ptr);
}

void* malloc(size_t size)
{
    return pool_allocate(get_malloc_pool(), size);
}
