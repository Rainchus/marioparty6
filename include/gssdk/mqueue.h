#ifndef GSSDK_MQUEUE_H
#define GSSDK_MQUEUE_H

#include "types.h"

#include "gssdk/tos.h"

typedef struct TosQueueElement {
    struct TosQueueElement *next;
    u8 data[];
} TosQueueElement;

struct TosQueue {
    TosContext *context;
    u8 queueIndex;
    u8 reserved05[3];
    u16 elementSize;
    u16 maxElements;
    u8 state;
    u8 readerCount;
    u8 reserved0E[2];
    TosQueueElement *head;
    TosQueueElement *readPointers[];
};

extern u32 qEnQueue(TosQueue *queue, void *elements, u32 count);
extern void *qDeQueueOne(TosQueue *queue, u32 reader);
extern u32 qQueueNbrElements(TosQueue *queue);
extern TosQueue *qQueueConstruct(
    TosContext *context, u32 queueIndex, u32 readerCount);

#endif
