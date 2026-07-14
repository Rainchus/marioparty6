#include "types.h"

#include "gssdk/mqueue.h"

extern void *heap_Calloc(void *heap, u32 count, u32 size);

void *qDeQueueOne(TosQueue *queue, u32 reader)
{
    TosQueueElement **readPointer = &queue->readPointers[reader];
    TosQueueElement *element = *readPointer;

    if (element == (TosQueueElement *)&queue->head || element == NULL ||
        element == (TosQueueElement *)queue) {
        return NULL;
    }

    if (element->next != NULL) {
        *readPointer = element->next;
    } else {
        *readPointer = (TosQueueElement *)queue;
    }
    return element->data;
}

u32 qQueueNbrElements(TosQueue *queue)
{
    TosQueueElement *element = queue->head;
    u32 count = 0;

    while (element != NULL) {
        element = element->next;
        count++;
    }
    return count;
}

TosQueue *qQueueConstruct(
    TosContext *context, u32 queueIndex, u32 readerCount)
{
    struct TosQueueProfile {
        TosContext *context;
        u8 queueIndex;
    } profile;
    TosQueue *queue;

    queue = heap_Calloc(
        context->heap, 1,
        sizeof(TosQueue) + (u8)readerCount * sizeof(TosQueueElement *));
    if (queue == NULL) {
        profile.context = context;
        profile.queueIndex = queueIndex;
        _tosErrorLog(&profile, 2);
        return NULL;
    }

    queue->readerCount = readerCount;
    queue->queueIndex = queueIndex;
    queue->context = context;
    return queue;
}
