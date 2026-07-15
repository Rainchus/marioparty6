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
    u8 profileOwnerAlignment[3];
    u16 elementSize;
    u16 maxElements;
    u8 state;
    u8 readerCount;
    TosQueueElement *head;
    TosQueueElement *readPointers[];
};

extern u8 qQueueControl(
    TosQueue *queue, u32 command, u16 value, void *argument);
extern u8 qQueueJumpBack(TosQueue *queue, u16 count);
extern u8 qQueueJumpBackOne(TosQueue *queue, u16 reader, u16 count);
extern TosQueue *qQueueInit(TosQueue *queue);
extern TosQueue *qQueueInitEx(
    TosQueue *queue, u16 elementSize, u8 initializeReaders,
    u16 maxElements);
extern TosQueue *qQueueReDimension(TosQueue *queue, u16 elementSize);
extern u32 qEnQueue(TosQueue *queue, void *elements, u32 count);
extern void *qEnQueueOne(TosQueue *queue);
extern void *qDeQueueOne(TosQueue *queue, u32 reader);
extern void qFreeUnusedElemements(TosQueue *queue);
extern u8 qCheckDeQueueOne(
    TosQueuePort *inputs, u32 inputCount, void **elements);
extern u8 qCheckInputQueues(TosQueuePort *inputs, u32 inputCount);
extern void qQueueReset(TosQueue *queue);
extern u32 qQueueNbrElements(TosQueue *queue);
extern TosQueue *qQueueConstruct(
    TosContext *context, u32 queueIndex, u32 readerCount);

#endif
