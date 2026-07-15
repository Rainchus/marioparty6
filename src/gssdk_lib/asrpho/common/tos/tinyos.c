#include "types.h"

#include <string.h>

#include "gssdk/mqueue.h"
#include "gssdk/tos.h"

typedef struct TosProfileOwner {
    TosContext *context;
    u8 ownerIndex;
    u8 profileOwnerAlignment[3];
} TosProfileOwner;

extern void *heap_Calloc(void *heap, u32 count, u32 size);
extern void *heap_Alloc(void *heap, u32 size);
extern void heap_Free(void *heap, void *ptr);

enum {
    TOS_CONTROL_CONSTRUCT = 0xF8,
    TOS_CONTROL_START = 0xF9,
    TOS_CONTROL_STOP = 0xFA,
    TOS_CONTROL_ENABLE = 0xFB,
    TOS_CONTROL_DISABLE = 0xFC,
    TOS_CONTROL_DESTRUCT = 0xFF
};

static u8 SetAndControlQSizes(TosContext *context)
{
    TosQueue *queue;
    TosBaseBlock *block;
    TosQueuePort *port;
    u16 elementSize;
    u32 queueNumber;
    u32 blockNumber;
    u32 portNumber;
    u8 found;

    for (queueNumber = 0; queueNumber < context->queueCount;
         queueNumber++) {
        queue = context->queues[queueNumber];
        if (queue == NULL) {
            continue;
        }

        elementSize = 0xFFFF;
        found = 0;
        for (blockNumber = 0; blockNumber < context->blockCount;
             blockNumber++) {
            block = context->blocks[blockNumber];
            if (block == NULL) {
                continue;
            }

            for (portNumber = 0; portNumber < block->inputCount;
                 portNumber++) {
                port = &block->input[portNumber];
                if (port->queue != queue) {
                    continue;
                }
                if (found != 0 && elementSize != port->inputSize) {
                    return 1;
                }
                found = 1;
                elementSize = port->inputSize;
            }

            for (portNumber = 0; portNumber < block->outputCount;
                 portNumber++) {
                port = &block->output[portNumber];
                if (port->queue != queue) {
                    continue;
                }
                if (found != 0 && elementSize != port->outputSize) {
                    return 1;
                }
                found = 1;
                elementSize = port->outputSize;
            }
        }

        if (found == 0) {
            return 1;
        }
        queue->elementSize = elementSize;
    }
    return 0;
}

static u8 ConnectInQtoBlock(
    TosContext *context, TosBaseBlock *block, TosQueue *queue,
    u16 readerIndex)
{
    u32 inputNumber;

    if (block != NULL) {
        if (block == (TosBaseBlock *)context) {
            if (context->registerInputQueue(
                    context, queue, readerIndex) != 0) {
                return 1;
            }
        } else {
            for (inputNumber = 0; inputNumber < block->inputCount;
                 inputNumber++) {
                if (block->input[inputNumber].queue == NULL) {
                    block->input[inputNumber].queue = queue;
                    block->input[inputNumber].outputSize = readerIndex;
                    break;
                }
            }
        }

        if (queue != NULL &&
            qQueueControl(queue, 10, readerIndex, NULL) == 0) {
            return 1;
        }
    } else if (queue != NULL &&
               qQueueControl(queue, 8, readerIndex, NULL) == 0) {
        return 1;
    }
    return 0;
}

static u8 ConstructBlock(
    TosContext *context, u32 blockIndex, u32 blockNumber)
{
    const TosBlockDefinition *definition;
    const TosQueueWriterDefinition *writer;
    const TosQueueReaderDefinition *reader;
    TosBaseBlock *block;
    TosQueue *queue;
    u8 *readerCounts;
    u32 inputNumber;
    u32 outputNumber;
    u32 queueNumber;

    definition = context->blockDefinitions;
    while (definition->blockIndex != (u8)blockIndex) {
        definition++;
    }

    block = definition->constructor(context, (u8)blockIndex);
    if (block == NULL) {
        return 1;
    }
    context->blocks[blockNumber] = block;

    outputNumber = 0;
    writer = context->queueWriters;
    while (writer->queueIndex != 0xFF) {
        if (writer->blockIndex == (u8)blockIndex) {
            queue = tosGetQueuePtr(context, writer->queueIndex);
            if (block == (TosBaseBlock *)context) {
                if (context->registerOutputQueue(context, queue) != 0) {
                    return 1;
                }
            } else if (block != NULL && queue != NULL) {
                block->output[outputNumber].queue = queue;
                if (queue->elementSize == 0) {
                    queue->elementSize =
                        block->output[outputNumber].outputSize;
                }
            }
            outputNumber++;
        }
        writer++;
    }

    readerCounts = heap_Calloc(
        context->heap, context->queueCount, sizeof(u8));
    if (readerCounts == NULL) {
        return 1;
    }

    reader = context->queueReaders;
    while (reader->queueIndex != 0xFF) {
        queueNumber = reader->queueIndex - context->firstQueueIndex;
        readerCounts[queueNumber]++;
        reader++;
    }

    inputNumber = 0;
    reader = context->queueReaders;
    while (reader->queueIndex != 0xFF) {
        if (reader->blockIndex == (u8)blockIndex) {
            queueNumber = reader->queueIndex - context->firstQueueIndex;
            queue = context->queues[queueNumber];
            readerCounts[queueNumber]--;
            if (ConnectInQtoBlock(
                    context, block, queue,
                    readerCounts[queueNumber]) != 0) {
                heap_Free(context->heap, readerCounts);
                return 1;
            }
            inputNumber++;
        }
        reader++;
    }

    heap_Free(context->heap, readerCounts);
    if (block->init(block) != 0) {
        return 1;
    }
    return 0;
}

static u8 ConstructQueue(
    TosContext *context, u32 queueIndex, u32 queueNumber)
{
    const TosQueueWriterDefinition *writer;
    const TosQueueReaderDefinition *reader;
    TosBaseBlock *block;
    TosQueue *queue;
    u8 *writerCounts;
    u32 outputNumber;
    u32 readerCount;
    u32 readerIndex;
    u32 blockNumber;
    u32 index;

    writerCounts = heap_Calloc(
        context->heap, context->blockCount, sizeof(u8));
    if (writerCounts == NULL) {
        return 1;
    }

    writer = context->queueWriters;
    while (writer->queueIndex != (u8)queueIndex) {
        writerCounts[writer->blockIndex]++;
        writer++;
    }

    readerCount = 0;
    reader = context->queueReaders;
    while (reader->queueIndex != 0xFF) {
        if (reader->queueIndex == (u8)queueIndex) {
            readerCount++;
        }
        reader++;
    }

    queue = qQueueConstruct(context, queueIndex, (u8)readerCount);
    if (queue == NULL) {
        heap_Free(context->heap, writerCounts);
        return 1;
    }
    context->queues[queueNumber] = queue;

    while (writer->queueIndex == (u8)queueIndex) {
        if (writer->blockIndex != 0) {
            blockNumber = writer->blockIndex - context->firstBlockIndex;
            block = context->blocks[blockNumber];
        } else {
            block = (TosBaseBlock *)context;
        }

        outputNumber = writerCounts[writer->blockIndex]++;
        if (block == (TosBaseBlock *)context) {
            if (context->registerOutputQueue(context, queue) != 0) {
                heap_Free(context->heap, writerCounts);
                return 1;
            }
        } else if (block != NULL && queue != NULL) {
            block->output[outputNumber].queue = queue;
            if (queue->elementSize == 0) {
                queue->elementSize =
                    block->output[outputNumber].outputSize;
            }
        }
        writer++;
    }

    heap_Free(context->heap, writerCounts);

    readerIndex = readerCount;
    reader = context->queueReaders;
    while (reader->queueIndex != 0xFF) {
        if (reader->queueIndex == (u8)queueIndex) {
            if (reader->blockIndex != 0) {
                blockNumber =
                    reader->blockIndex - context->firstBlockIndex;
                block = context->blocks[blockNumber];
            } else {
                block = (TosBaseBlock *)context;
            }
            index = queueIndex - context->firstQueueIndex;
            readerIndex--;
            if (ConnectInQtoBlock(
                    context, block, context->queues[index],
                    (u16)readerIndex) != 0) {
                return 1;
            }
        }
        reader++;
    }

    if (qQueueInit(queue) == NULL) {
        return 1;
    }
    return 0;
}

u8 DestructQueue(TosQueue *queue)
{
    TosContext *context = queue->context;
    const TosQueueWriterDefinition *writer;
    const TosQueueReaderDefinition *reader;
    TosBaseBlock *block;
    TosQueuePort *port;
    u32 portNumber;
    u32 blockNumber;
    u32 queueNumber;
    u8 queueIndex = queue->queueIndex;

    qQueueReset(queue);

    if (queueIndex != 0xFF) {
        writer = context->queueWriters;
        while (writer->queueIndex != queueIndex) {
            writer++;
        }
        while (writer->queueIndex == queueIndex) {
            if (writer->blockIndex != 0) {
                blockNumber =
                    writer->blockIndex - context->firstBlockIndex;
                block = context->blocks[blockNumber];
            } else {
                block = (TosBaseBlock *)context;
            }
            if (block != NULL && block != (TosBaseBlock *)context) {
                for (portNumber = 0;
                     portNumber < block->outputCount; portNumber++) {
                    port = &block->output[portNumber];
                    if (port->queue == queue) {
                        port->queue = NULL;
                        break;
                    }
                }
            }
            writer++;
        }
    }

    reader = context->queueReaders;
    while (reader->queueIndex != 0xFF) {
        if (reader->queueIndex == queueIndex) {
            if (reader->blockIndex != 0) {
                blockNumber =
                    reader->blockIndex - context->firstBlockIndex;
                block = context->blocks[blockNumber];
            } else {
                block = (TosBaseBlock *)context;
            }
            queueNumber = queueIndex - context->firstQueueIndex;
            if (block != NULL && block != (TosBaseBlock *)context) {
                for (portNumber = 0;
                     portNumber < block->inputCount; portNumber++) {
                    port = &block->input[portNumber];
                    if (port->queue == context->queues[queueNumber]) {
                        port->queue = NULL;
                        break;
                    }
                }
            }
        }
        reader++;
    }

    if (queueIndex != 0xFF) {
        queueNumber = queueIndex - context->firstQueueIndex;
        context->queues[queueNumber] = NULL;
    }
    heap_Free(context->heap, queue);
    return 0;
}

static u8 ConstructArchitecture(
    TosContext *context,
    const TosBlockDefinition *blockDefinitions,
    const TosQueueWriterDefinition *queueWriters,
    const TosQueueReaderDefinition *queueReaders)
{
    const TosBlockDefinition *definition;
    const TosQueueWriterDefinition *writer;
    const TosQueueReaderDefinition *reader;
    TosBaseBlock *block;
    TosQueue *queue = NULL;
    u8 *counts;
    u8 *outputCounts;
    u8 *inputCounts;
    u8 *queueReaderCounts;
    u32 definitionCount;
    u32 queueDefinitionCount;
    u32 blockNumber;
    u32 queueNumber;
    u32 outputNumber;
    u32 readerIndex;
    u32 maximumInputs;
    u8 lastQueueIndex;

    context->blockDefinitions = blockDefinitions;
    context->queueWriters = queueWriters;
    context->queueReaders = queueReaders;

    definitionCount = 0;
    definition = blockDefinitions;
    while (definition->blockIndex != 0xFF) {
        definitionCount++;
        definition++;
    }

    queueDefinitionCount = 0;
    lastQueueIndex = 0xFF;
    writer = queueWriters;
    while (writer->queueIndex != 0xFF) {
        if (writer->queueIndex != lastQueueIndex) {
            lastQueueIndex = writer->queueIndex;
            queueDefinitionCount++;
        }
        writer++;
    }

    context->firstBlockIndex = blockDefinitions[1].blockIndex;
    context->firstQueueIndex = queueWriters[0].queueIndex;

    context->blocks = heap_Alloc(
        context->heap, definitionCount * sizeof(TosBaseBlock *));
    if (context->blocks == NULL) {
        return 1;
    }

    context->queues = heap_Alloc(
        context->heap, queueDefinitionCount * sizeof(TosQueue *));
    if (context->queues == NULL) {
        return 1;
    }

    counts = heap_Calloc(
        context->heap,
        queueDefinitionCount + definitionCount * 2,
        sizeof(u8));
    if (counts == NULL) {
        return 1;
    }
    outputCounts = counts;
    inputCounts = outputCounts + definitionCount;
    queueReaderCounts = inputCounts + definitionCount;

    writer = queueWriters;
    while (writer->queueIndex != 0xFF) {
        outputCounts[writer->blockIndex]++;
        writer++;
    }

    reader = queueReaders;
    while (reader->queueIndex != 0xFF) {
        queueNumber = reader->queueIndex - context->firstQueueIndex;
        queueReaderCounts[queueNumber]++;
        inputCounts[reader->blockIndex]++;
        reader++;
    }

    maximumInputs = 0;
    context->blockCount = 0;
    definition = blockDefinitions;
    while (definition->blockIndex != 0xFF) {
        if (inputCounts[definition->blockIndex] > maximumInputs) {
            maximumInputs = inputCounts[definition->blockIndex];
        }

        if (definition->constructor != NULL) {
            block = NULL;
            if (definition->construct != 0) {
                block = definition->constructor(
                    context, definition->blockIndex);
            }
            blockNumber = context->blockCount++;
            context->blocks[blockNumber] = block;
            if (context->blockCount != definition->blockIndex) {
                heap_Free(context->heap, counts);
                return 1;
            }
        } else if (definition->blockIndex == 0 &&
                   context->initializeQueues(
                       context, inputCounts[0],
                       outputCounts[0]) != 0) {
            heap_Free(context->heap, counts);
            return 1;
        }
        definition++;
    }

    memset(outputCounts, 0, definitionCount);
    context->queueCount = 0;
    lastQueueIndex = 0xFF;
    writer = queueWriters;
    while (writer->queueIndex != 0xFF) {
        if (writer->blockIndex != 0) {
            blockNumber =
                writer->blockIndex - context->firstBlockIndex;
            block = context->blocks[blockNumber];
        } else {
            block = (TosBaseBlock *)context;
        }

        if (writer->queueIndex != lastQueueIndex) {
            queue = NULL;
            if (writer->construct != 0) {
                queueNumber =
                    writer->queueIndex - context->firstQueueIndex;
                queue = qQueueConstruct(
                    context, writer->queueIndex,
                    queueReaderCounts[queueNumber]);
                if (queue == NULL) {
                    heap_Free(context->heap, counts);
                    return 1;
                }
            }
            queueNumber = context->queueCount++;
            context->queues[queueNumber] = queue;
            lastQueueIndex = writer->queueIndex;
        }

        outputNumber = outputCounts[writer->blockIndex]++;
        if (block == (TosBaseBlock *)context) {
            if (context->registerOutputQueue(context, queue) != 0) {
                heap_Free(context->heap, counts);
                return 1;
            }
        } else if (block != NULL && queue != NULL) {
            block->output[outputNumber].queue = queue;
            if (queue->elementSize == 0) {
                queue->elementSize =
                    block->output[outputNumber].outputSize;
            }
        }
        writer++;
    }

    reader = queueReaders;
    while (reader->queueIndex != 0xFF) {
        if (reader->blockIndex != 0) {
            blockNumber =
                reader->blockIndex - context->firstBlockIndex;
            block = context->blocks[blockNumber];
        } else {
            block = (TosBaseBlock *)context;
        }

        queueNumber = reader->queueIndex - context->firstQueueIndex;
        queueReaderCounts[queueNumber]--;
        if (ConnectInQtoBlock(
                context, block, context->queues[queueNumber],
                queueReaderCounts[queueNumber]) != 0) {
            heap_Free(context->heap, counts);
            return 1;
        }
        reader++;
    }

    heap_Free(context->heap, counts);
    context->processInputs = heap_Alloc(
        context->heap, maximumInputs * sizeof(void *));
    if (context->processInputs == NULL) {
        return 1;
    }
    return 0;
}

static u8 DestructArchitecture(TosContext *context)
{
    TosBaseBlock *block;
    TosQueue *queue;
    u32 blockNumber;
    u32 queueNumber;
    u8 failed = 0;

    for (blockNumber = 0; blockNumber < context->blockCount;
         blockNumber++) {
        block = context->blocks[blockNumber];
        if (block != NULL && block->control != NULL &&
            block->control(
                block, TOS_CONTROL_DESTRUCT, NULL, 0) == 0) {
            failed = 1;
        }
    }

    for (queueNumber = 0; queueNumber < context->queueCount;
         queueNumber++) {
        queue = context->queues[queueNumber];
        if (queue != NULL &&
            qQueueControl(
                queue, TOS_CONTROL_DESTRUCT, 0, NULL) == 0) {
            failed = 1;
        }
    }

    heap_Free(context->heap, context->blocks);
    heap_Free(context->heap, context->queues);
    heap_Free(context->heap, context->processInputs);
    return failed;
}

static u8 InitializeArchitecture(TosContext *context)
{
    TosBaseBlock *block;
    TosQueue *queue;
    u32 blockNumber;
    u32 queueNumber;

    for (blockNumber = 0; blockNumber < context->blockCount;
         blockNumber++) {
        block = context->blocks[blockNumber];
        if (block != NULL && block->init != NULL &&
            block->init(block) != 0) {
            return 1;
        }
    }

    if (SetAndControlQSizes(context) != 0) {
        return 1;
    }

    for (queueNumber = 0; queueNumber < context->queueCount;
         queueNumber++) {
        queue = context->queues[queueNumber];
        if (queue != NULL && qQueueInit(queue) == NULL) {
            return 1;
        }
    }
    return 0;
}

TosQueue *tosGetQueuePtr(TosContext *context, u32 queueIndex)
{
    s32 queueNumber =
        (u8)queueIndex - (s32)context->firstQueueIndex;

    if (queueNumber < 0 || queueNumber >= context->queueCount) {
        return NULL;
    }
    return context->queues[queueNumber];
}

u8 tosCallControlFunc(
    TosContext *context, const TosControlTarget *target,
    u32 argument0, u32 argument1)
{
    TosBaseBlock *block;
    TosQueue *queue;
    s32 blockNumber;
    s32 queueNumber;
    u8 wasEnabled;
    u8 result = 0;

    blockNumber =
        target->ownerIndex - (s32)context->firstBlockIndex;
    if (blockNumber >= 0 && blockNumber < context->blockCount) {
        block = context->blocks[blockNumber];
    } else {
        block = NULL;
    }

    if (block != NULL) {
        wasEnabled = block->enabled;
        if (wasEnabled != 0 ||
            target->command == TOS_CONTROL_DESTRUCT) {
            result = block->control(
                block, target->command, (void *)argument0,
                argument1);
        }
        if (target->command >= TOS_CONTROL_CONSTRUCT) {
            result |= tosBaseBlockControl(
                block, target->command, (void *)argument0,
                argument1);
        }
        if (wasEnabled == 0 && result == 0) {
            result = 1;
        }
        return result;
    }

    queueNumber =
        target->ownerIndex - (s32)context->firstQueueIndex;
    if (queueNumber >= 0 && queueNumber < context->queueCount) {
        queue = context->queues[queueNumber];
    } else {
        queue = NULL;
    }

    if (queue != NULL) {
        return qQueueControl(
            queue, target->command, (u16)argument0,
            (void *)argument1);
    }

    if (target->command == TOS_CONTROL_CONSTRUCT) {
        if (blockNumber >= 0 && blockNumber < context->blockCount &&
            context->blocks[blockNumber] == NULL) {
            ConstructBlock(
                context, target->ownerIndex, blockNumber);
        } else if (!(queueNumber >= 0 &&
                     queueNumber < context->queueCount &&
                     context->queues[queueNumber] != NULL)) {
            ConstructQueue(
                context, target->ownerIndex, queueNumber);
        }
        return 1;
    }
    return 1;
}

u8 tosInit(
    TosContext *context, void *heap,
    const TosArchitectureCallbacks *callbacks,
    const TosProfileTable *profiles,
    const TosBlockDefinition *blockDefinitions,
    const TosQueueWriterDefinition *queueWriters,
    const TosQueueReaderDefinition *queueReaders,
    const TosControlBlock *const *const *controlTable,
    u16 controlBank, u16 initialState)
{
    context->heap = heap;
    fastallo_Init(&context->queueAllocator, heap, 0x1000);
    context->profiles = profiles;
    context->controlTable = controlTable;
    context->lastError = 0;
    context->stateChanged = callbacks->stateChanged;
    context->initializeQueues = callbacks->initializeQueues;
    context->registerInputQueue = callbacks->registerInputQueue;
    context->registerOutputQueue = callbacks->registerOutputQueue;
    context->errorLogged = callbacks->errorLogged;
    context->warningLogged = callbacks->warningLogged;
    context->controlBank = controlBank;
    context->state = initialState;

    if (ConstructArchitecture(
            context, blockDefinitions, queueWriters,
            queueReaders) == 0 &&
        InitializeArchitecture(context) == 0) {
        return 0;
    }

    fastallo_Terminate(&context->queueAllocator);
    return 1;
}

u8 tosDestruct(TosContext *context)
{
    if (DestructArchitecture(context) != 0) {
        return 1;
    }
    fastallo_Terminate(&context->queueAllocator);
    return 0;
}

static void blockFreeUnusedElementsInputQueues(TosBaseBlock *block)
{
    u32 inputNumber;

    for (inputNumber = 0; inputNumber < block->inputCount;
         inputNumber++) {
        if (block->input[inputNumber].queue != NULL) {
            qFreeUnusedElemements(block->input[inputNumber].queue);
        }
    }
}

u8 tosRun(TosContext *context, const u8 *schedule)
{
    TosBaseBlock *block;
    const u8 *entry;
    u32 blockNumber;

    if (schedule == NULL) {
        do {
            context->enqueuesThisPass = 0;
            for (blockNumber = 0;
                 blockNumber < context->blockCount; blockNumber++) {
                block = context->blocks[blockNumber];
                if (block == NULL) {
                    continue;
                }
                qCheckDeQueueOne(
                    block->input, block->inputCount,
                    context->processInputs);
                block->process(
                    block, context->processInputs,
                    block->inputCount);
                blockFreeUnusedElementsInputQueues(block);
            }
        } while (context->enqueuesThisPass != 0);
    } else {
        do {
            context->enqueuesThisPass = 0;
            entry = schedule;
            while (*entry != 0) {
                blockNumber = *entry - context->firstBlockIndex;
                entry++;
                block = context->blocks[blockNumber];
                if (block == NULL) {
                    break;
                }
                qCheckDeQueueOne(
                    block->input, block->inputCount,
                    context->processInputs);
                block->process(
                    block, context->processInputs,
                    block->inputCount);
                blockFreeUnusedElementsInputQueues(block);
            }
        } while (context->enqueuesThisPass != 0);
    }
    return 0;
}

u8 _tosControl(
    void *ownerPointer, u32 command, u32 argument0, u32 argument1)
{
    TosProfileOwner *owner = ownerPointer;
    TosContext *context = owner->context;
    const TosControlBlock *blockControls;
    const TosControlCommand *control;
    const TosControlCommand *const *controls;
    const TosControlTarget *target;
    u8 failed = 0;

    blockControls =
        context->controlTable[context->controlBank][context->state];
    while (blockControls->commands != NULL &&
           blockControls->blockIndex != owner->ownerIndex) {
        blockControls++;
    }
    if (blockControls->commands == NULL) {
        return _tosErrorLog(owner, 5);
    }

    controls = blockControls->commands;
    while (*controls != NULL && (*controls)->command != (u8)command) {
        controls++;
    }
    control = *controls;
    if (control == NULL) {
        return _tosErrorLog(owner, 5);
    }

    if (control->nextState != 0xFFFF &&
        control->nextState != context->state) {
        if (control->nextState == 0xFFFE) {
            context->state++;
        } else {
            context->state = control->nextState;
        }
        failed = context->stateChanged(context, context->state);
        if (failed != 0) {
            return failed;
        }
    }

    target = (const TosControlTarget *)(control + 1);
    if (control->runBetweenTargets == 0) {
        while (target->command != 0) {
            if (tosCallControlFunc(
                    context, target, argument0, argument1) == 0) {
                failed = 1;
            }
            target++;
        }
    } else {
        while (target->command != 0) {
            if (tosCallControlFunc(
                    context, target, argument0, argument1) == 0) {
                failed = 1;
            }
            target++;
            tosRun(context, NULL);
        }
    }

    if (control->callback != NULL) {
        failed |= control->callback(
            context, owner->ownerIndex, (u8)command,
            (void *)argument0, argument1);
    }
    return failed;
}

u32 _tosErrorLog(void *ownerPointer, u32 error)
{
    TosProfileOwner *owner = ownerPointer;
    TosContext *context = owner->context;

    if (context->lastError == 0) {
        context->lastError =
            ((u32)owner->ownerIndex << 16) | (u16)error;
    }
    if (context->errorLogged != NULL) {
        context->errorLogged(context, context->lastError);
    }
    return (u8)error;
}

u32 _tosWarningLog(void *ownerPointer, u32 warning)
{
    TosProfileOwner *owner = ownerPointer;
    TosContext *context = owner->context;

    if (context->lastWarning == 0) {
        context->lastWarning =
            ((u32)owner->ownerIndex << 16) | (u16)warning;
    }
    if (context->warningLogged != NULL) {
        context->warningLogged(context, context->lastWarning);
    }
    return (u8)warning;
}

f32 _tosGetProfileFloat(
    void *ownerPointer, u32 key, f32 defaultValue)
{
    TosProfileOwner *owner = ownerPointer;
    const TosProfileTable *profile =
        &owner->context->profiles[owner->ownerIndex];
    const TosFloatProfileEntry *floatEntry;
    const TosU32ProfileEntry *u32Entry;

    floatEntry = profile->floatValues;
    if (floatEntry != NULL) {
        while (floatEntry->key != (u16)key &&
               floatEntry->key != 0) {
            floatEntry++;
        }
        if (floatEntry->key != 0) {
            return floatEntry->value;
        }
    }

    u32Entry = profile->u32Values;
    if (u32Entry != NULL) {
        while (u32Entry->key != (u16)key && u32Entry->key != 0) {
            u32Entry++;
        }
        if (u32Entry->key != 0) {
            return (f32)u32Entry->value;
        }
    }
    return defaultValue;
}

u32 _tosGetProfileU32(
    void *ownerPointer, u32 key, u32 defaultValue)
{
    TosProfileOwner *owner = ownerPointer;
    const TosProfileTable *profile =
        &owner->context->profiles[owner->ownerIndex];
    const TosFloatProfileEntry *floatEntry;
    const TosU32ProfileEntry *u32Entry;

    u32Entry = profile->u32Values;
    if (u32Entry != NULL) {
        while (u32Entry->key != (u16)key && u32Entry->key != 0) {
            u32Entry++;
        }
        if (u32Entry->key != 0) {
            return u32Entry->value;
        }
    }

    floatEntry = profile->floatValues;
    if (floatEntry != NULL) {
        while (floatEntry->key != (u16)key &&
               floatEntry->key != 0) {
            floatEntry++;
        }
        if (floatEntry->key != 0) {
            return (u32)floatEntry->value;
        }
    }
    return defaultValue;
}

TosBaseBlock *tosBaseBlockConstruct(
    TosContext *context, u32 blockIndex, u8 inputCount,
    u8 outputCount, TosProcessFunction process,
    TosInitFunction init, TosControlFunction control, u32 size)
{
    TosProfileOwner failedOwner;
    TosBaseBlock *block;
    u32 alignedSize = size;

    if ((alignedSize & 3) != 0) {
        alignedSize += 4 - (alignedSize & 3);
    }

    block = heap_Calloc(
        context->heap, 1,
        alignedSize +
            inputCount * sizeof(TosQueuePort) +
            outputCount * sizeof(TosQueuePort));
    if (block == NULL) {
        failedOwner.context = context;
        failedOwner.ownerIndex = blockIndex;
        _tosErrorLog(&failedOwner, 2);
        return NULL;
    }

    block->input = (TosQueuePort *)((u8 *)block + alignedSize);
    block->output = block->input + inputCount;
    block->context = context;
    block->blockIndex = blockIndex;
    block->inputCount = inputCount;
    block->outputCount = outputCount;
    block->init = init;
    block->control = control;
    block->process = process;
    block->enabled = 1;
    return block;
}

void tosBaseBlockDestruct(void *blockPointer)
{
    TosBaseBlock *block = blockPointer;
    TosContext *context = block->context;
    u32 inputNumber;
    u32 blockNumber;

    for (inputNumber = 0; inputNumber < block->inputCount;
         inputNumber++) {
        if (block->input[inputNumber].queue != NULL) {
            qQueueControl(
                block->input[inputNumber].queue, 8,
                block->input[inputNumber].outputSize, NULL);
        }
    }

    blockNumber = block->blockIndex - context->firstBlockIndex;
    context->blocks[blockNumber] = NULL;
    heap_Free(context->heap, block);
}

u8 tosBaseBlockDisableInputQueues(TosBaseBlock *block)
{
    u32 inputNumber;
    u8 failed = 0;

    for (inputNumber = 0; inputNumber < block->inputCount;
         inputNumber++) {
        if (block->input[inputNumber].queue != NULL &&
            qQueueControl(
                block->input[inputNumber].queue, 8,
                block->input[inputNumber].outputSize, NULL) == 0) {
            failed = 1;
        }
    }
    return failed;
}

u8 tosBaseBlockStartInputQueues(
    TosBaseBlock *block, u32 jumpBackCount)
{
    u32 inputNumber;
    u32 queueCommand;
    u8 failed = 0;

    if (jumpBackCount != 0) {
        queueCommand = 7;
    } else {
        queueCommand = 5;
    }

    for (inputNumber = 0; inputNumber < block->inputCount;
         inputNumber++) {
        if (block->input[inputNumber].queue != NULL &&
            qQueueControl(
                block->input[inputNumber].queue, queueCommand,
                block->input[inputNumber].outputSize,
                (void *)jumpBackCount) == 0) {
            failed = 1;
        }
    }
    return failed;
}

static void DisabledProcess(
    TosBaseBlock *block, void **inputs, s32 inputCount)
{
}

u8 tosBaseBlockControl(
    TosBaseBlock *block, u32 command, void *argument,
    u32 argumentSize)
{
    u32 inputNumber;
    u32 queueCommand;
    u8 failed;

    switch ((u8)command) {
    case TOS_CONTROL_CONSTRUCT:
        return 1;
    case TOS_CONTROL_START:
        if (block->enabled == 0) {
            return 0;
        }
        return tosBaseBlockStartInputQueues(block, (u32)argument) == 0;
    case TOS_CONTROL_STOP:
        if (block->enabled == 0) {
            return 0;
        }
        failed = 0;
        for (inputNumber = 0; inputNumber < block->inputCount;
             inputNumber++) {
            if (qQueueControl(
                    block->input[inputNumber].queue, 6,
                    block->input[inputNumber].outputSize, NULL) == 0) {
                failed = 1;
            }
        }
        return failed == 0;
    case TOS_CONTROL_ENABLE:
        block->enabled = 1;
        if (block->process == DisabledProcess) {
            block->process = block->savedProcess;
        }
        if (block->enabled == 0) {
            return 0;
        }
        if (argument != NULL) {
            queueCommand = 7;
        } else {
            queueCommand = 5;
        }
        failed = 0;
        for (inputNumber = 0; inputNumber < block->inputCount;
             inputNumber++) {
            if (block->input[inputNumber].queue != NULL &&
                qQueueControl(
                    block->input[inputNumber].queue, queueCommand,
                    block->input[inputNumber].outputSize,
                    argument) == 0) {
                failed = 1;
            }
        }
        return failed == 0;
    case TOS_CONTROL_DISABLE:
        if (block->enabled == 0) {
            return 0;
        }
        block->enabled = 0;
        block->savedProcess = block->process;
        block->process = DisabledProcess;
        return tosBaseBlockDisableInputQueues(block) == 0;
    default:
        return 0;
    }
}
