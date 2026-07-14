#ifndef GSSDK_TOS_H
#define GSSDK_TOS_H

#include "types.h"

typedef struct TosContext {
u8 reserved[0x50];
void *heap;
} TosContext;

typedef struct TosQueue TosQueue;

typedef struct TosQueuePort {
TosQueue *queue;
u16 outputSize;
u16 inputSize;
} TosQueuePort;

typedef struct TosBaseBlock TosBaseBlock;

typedef void (*TosProcessFunction)(
    TosBaseBlock *, void **inputs, s32 inputCount);
typedef u32 (*TosInitFunction)(TosBaseBlock *);
typedef u32 (*TosControlFunction)(
    TosBaseBlock *, u32 command, void *argument, u32 argumentSize);

struct TosBaseBlock {
TosContext *context;
u8 blockIndex;
u8 reserved05[3];
u8 inputCount;
u8 outputCount;
u8 reserved0A[2];
TosQueuePort *input;
TosQueuePort *output;
TosInitFunction init;
TosProcessFunction process;
TosControlFunction control;
u8 enabled;
u8 reserved21[7];
};

extern void *qEnQueueOne(TosQueue *queue);
extern u32 _tosGetProfileU32(void *block, u32 key, u32 defaultValue);
extern f32 _tosGetProfileFloat(void *block, u32 key, f32 defaultValue);
extern u32 _tosErrorLog(void *block, u32 error);
extern void tosBaseBlockDestruct(void *block);
extern TosBaseBlock *tosBaseBlockConstruct(
    TosContext *context, u32 blockIndex, u8 inputCount, u8 outputCount,
    TosProcessFunction process, TosInitFunction init,
    TosControlFunction control, u32 size);

#endif
