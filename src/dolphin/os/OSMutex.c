#include "dolphin/os.h"

#define PushTail(queue, mutex, link)                                                                                                                 \
    do {                                                                                                                                             \
        OSMutex *__prev;                                                                                                                             \
                                                                                                                                                     \
        __prev = (queue)->tail;                                                                                                                      \
        if (__prev == NULL)                                                                                                                          \
            (queue)->head = (mutex);                                                                                                                 \
        else                                                                                                                                         \
            __prev->link.next = (mutex);                                                                                                             \
        (mutex)->link.prev = __prev;                                                                                                                 \
        (mutex)->link.next = NULL;                                                                                                                   \
        (queue)->tail = (mutex);                                                                                                                     \
    } while (0)

#define PopHead(queue, mutex, link)                                                                                                                  \
    do {                                                                                                                                             \
        OSMutex *__next;                                                                                                                             \
                                                                                                                                                     \
        (mutex) = (queue)->head;                                                                                                                     \
        __next = (mutex)->link.next;                                                                                                                 \
        if (__next == NULL)                                                                                                                          \
            (queue)->tail = NULL;                                                                                                                    \
        else                                                                                                                                         \
            __next->link.prev = NULL;                                                                                                                \
        (queue)->head = __next;                                                                                                                      \
    } while (0)

#define PopItem(queue, mutex, link)                                                                                                                  \
    do {                                                                                                                                             \
        OSMutex *__next;                                                                                                                             \
        OSMutex *__prev;                                                                                                                             \
                                                                                                                                                     \
        __next = (mutex)->link.next;                                                                                                                 \
        __prev = (mutex)->link.prev;                                                                                                                 \
                                                                                                                                                     \
        if (__next == NULL)                                                                                                                          \
            (queue)->tail = __prev;                                                                                                                  \
        else                                                                                                                                         \
            __next->link.prev = __prev;                                                                                                              \
                                                                                                                                                     \
        if (__prev == NULL)                                                                                                                          \
            (queue)->head = __next;                                                                                                                  \
        else                                                                                                                                         \
            __prev->link.next = __next;                                                                                                              \
    } while (0)

void __OSUnlockAllMutex(OSThread *thread)
{
    OSMutex *mutex;

    while (thread->queueMutex.head) {
        PopHead(&thread->queueMutex, mutex, link);
        mutex->count = 0;
        mutex->thread = NULL;
        OSWakeupThread(&mutex->queue);
    }
}

BOOL __OSCheckMutex(OSMutex *mutex)
{
    OSThread *thread;
    OSThreadQueue *queue;
    OSPriority priority = 0;

    queue = &mutex->queue;
    if (!(queue->head == NULL || queue->head->link.prev == NULL))
        return FALSE;
    if (!(queue->tail == NULL || queue->tail->link.next == NULL))
        return FALSE;
    for (thread = queue->head; thread; thread = thread->link.next) {
        if (!(thread->link.next == NULL || thread == thread->link.next->link.prev))
            return FALSE;
        if (!(thread->link.prev == NULL || thread == thread->link.prev->link.next))
            return FALSE;

        if (thread->state != OS_THREAD_STATE_WAITING)
            return FALSE;

        if (thread->priority < priority)
            return FALSE;
        priority = thread->priority;
    }

    if (mutex->thread) {
        if (mutex->count <= 0)
            return FALSE;
    }
    else {
        if (0 != mutex->count)
            return FALSE;
    }

    return TRUE;
}

BOOL __OSCheckDeadLock(OSThread *thread)
{
    OSMutex *mutex;

    mutex = thread->mutex;
    while (mutex && mutex->thread) {
        if (mutex->thread == thread)
            return TRUE;
        mutex = mutex->thread->mutex;
    }
    return FALSE;
}

BOOL __OSCheckMutexes(OSThread *thread)
{
    OSMutex *mutex;

    for (mutex = thread->queueMutex.head; mutex; mutex = mutex->link.next) {
        if (mutex->thread != thread)
            return FALSE;
        if (!__OSCheckMutex(mutex))
            return FALSE;
    }
    return TRUE;
}
