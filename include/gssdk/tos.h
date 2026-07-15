#ifndef GSSDK_TOS_H
#define GSSDK_TOS_H

#include "types.h"
#include "gssdk/fastallo.h"

typedef struct TosContext TosContext;
typedef struct TosQueue TosQueue;
typedef struct TosBaseBlock TosBaseBlock;

typedef struct TosQueuePort {
    TosQueue *queue;
    u16 outputSize;
    u16 inputSize;
} TosQueuePort;

typedef void (*TosProcessFunction)(
    TosBaseBlock *, void **inputs, s32 inputCount);
typedef u32 (*TosInitFunction)(TosBaseBlock *);
typedef u32 (*TosControlFunction)(
    TosBaseBlock *, u32 command, void *argument, u32 argumentSize);

typedef u8 (*TosStateChangeFunction)(TosContext *, u16 state);
typedef u8 (*TosArchitectureQueueInitFunction)(
    TosContext *, u8 inputCount, u8 outputCount);
typedef u8 (*TosRegisterInputQueueFunction)(
    TosContext *, TosQueue *, u16 readerIndex);
typedef u8 (*TosRegisterOutputQueueFunction)(TosContext *, TosQueue *);
typedef void (*TosLogFunction)(TosContext *, u32 event);

typedef struct TosArchitectureCallbacks {
    TosStateChangeFunction stateChanged;
    TosArchitectureQueueInitFunction initializeQueues;
    TosRegisterInputQueueFunction registerInputQueue;
    TosRegisterOutputQueueFunction registerOutputQueue;
    TosLogFunction errorLogged;
    TosLogFunction warningLogged;
} TosArchitectureCallbacks;

typedef struct TosFloatProfileEntry {
    u16 key;
    f32 value;
} TosFloatProfileEntry;

typedef struct TosU32ProfileEntry {
    u16 key;
    u32 value;
} TosU32ProfileEntry;

typedef struct TosProfileTable {
    const TosFloatProfileEntry *floatValues;
    const TosU32ProfileEntry *u32Values;
} TosProfileTable;

typedef void *(*TosBlockConstructor)(TosContext *, u32 blockIndex);

typedef struct TosBlockDefinition {
    u8 blockIndex;
    u8 construct;
    TosBlockConstructor constructor;
} TosBlockDefinition;

typedef struct TosQueueWriterDefinition {
    u8 queueIndex;
    u8 construct;
    u8 blockIndex;
} TosQueueWriterDefinition;

typedef struct TosQueueReaderDefinition {
    u8 queueIndex;
    u8 blockIndex;
} TosQueueReaderDefinition;

typedef u32 (*TosControlCallback)(
    TosContext *, u8 blockIndex, u8 command, void *argument,
    u32 argumentSize);

typedef struct TosControlTarget {
    u8 ownerIndex;
    u8 command;
} TosControlTarget;

typedef struct TosControlCommand {
    u8 command;
    u8 runBetweenTargets;
    u16 nextState;
    TosControlCallback callback;
} TosControlCommand;

typedef struct TosControlBlock {
    u8 blockIndex;
    u8 controlBlockAlignment[3];
    const TosControlCommand *const *commands;
} TosControlBlock;

struct TosContext {
    u16 blockCount;
    u16 firstBlockIndex;
    TosBaseBlock **blocks;
    u16 queueCount;
    u16 firstQueueIndex;
    TosQueue **queues;
    TosStateChangeFunction stateChanged;
    TosArchitectureQueueInitFunction initializeQueues;
    TosRegisterInputQueueFunction registerInputQueue;
    TosRegisterOutputQueueFunction registerOutputQueue;
    TosLogFunction errorLogged;
    TosLogFunction warningLogged;
    const TosProfileTable *profiles;
    const TosControlBlock *const *const *controlTable;
    u16 controlBank;
    u16 state;
    u32 lastError;
    u32 lastWarning;
    u32 enqueuesThisPass;
    const TosBlockDefinition *blockDefinitions;
    const TosQueueWriterDefinition *queueWriters;
    const TosQueueReaderDefinition *queueReaders;
    void **processInputs;
    void *heap;
    FastAllocator queueAllocator;
};

struct TosBaseBlock {
    TosContext *context;
    u8 blockIndex;
    u8 profileOwnerAlignment[3];
    u8 inputCount;
    u8 outputCount;
    TosQueuePort *input;
    TosQueuePort *output;
    TosInitFunction init;
    TosProcessFunction process;
    TosControlFunction control;
    u8 enabled;
    TosProcessFunction savedProcess;
};

extern void *qEnQueueOne(TosQueue *queue);
extern u32 _tosGetProfileU32(void *block, u32 key, u32 defaultValue);
extern f32 _tosGetProfileFloat(void *block, u32 key, f32 defaultValue);
extern u32 _tosErrorLog(void *block, u32 error);
extern u32 _tosWarningLog(void *block, u32 warning);
extern TosQueue *tosGetQueuePtr(TosContext *context, u32 queueIndex);
extern u8 tosCallControlFunc(
    TosContext *context, const TosControlTarget *target,
    u32 argument0, u32 argument1);
extern u8 tosInit(
    TosContext *context, void *heap,
    const TosArchitectureCallbacks *callbacks,
    const TosProfileTable *profiles,
    const TosBlockDefinition *blockDefinitions,
    const TosQueueWriterDefinition *queueWriters,
    const TosQueueReaderDefinition *queueReaders,
    const TosControlBlock *const *const *controlTable,
    u16 controlBank, u16 initialState);
extern u8 tosDestruct(TosContext *context);
extern u8 tosRun(TosContext *context, const u8 *schedule);
extern u8 _tosControl(
    void *owner, u32 command, u32 argument0, u32 argument1);
extern void tosBaseBlockDestruct(void *block);
extern TosBaseBlock *tosBaseBlockConstruct(
    TosContext *context, u32 blockIndex, u8 inputCount, u8 outputCount,
    TosProcessFunction process, TosInitFunction init,
    TosControlFunction control, u32 size);
extern u8 tosBaseBlockDisableInputQueues(TosBaseBlock *block);
extern u8 tosBaseBlockStartInputQueues(
    TosBaseBlock *block, u32 jumpBackCount);
extern u8 tosBaseBlockControl(
    TosBaseBlock *block, u32 command, void *argument,
    u32 argumentSize);

#endif
