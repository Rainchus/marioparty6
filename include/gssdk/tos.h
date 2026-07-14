#ifndef GSSDK_TOS_H
#define GSSDK_TOS_H

#include "types.h"

typedef struct TosContext {
u8 reserved[0x50];
void *heap;
} TosContext;

typedef struct TosQueuePort {
void *queue;
u16 outputSize;
u16 inputSize;
} TosQueuePort;

typedef struct TosBaseBlock {
TosContext *context;
u8 reserved04[8];
TosQueuePort *input;
TosQueuePort *output;
u8 reserved14[0x14];
} TosBaseBlock;

typedef void (*TosProcessFunction)(void *, void **);
typedef u32 (*TosInitFunction)(void *);
typedef u32 (*TosControlFunction)(void *, u32);

extern void *qEnQueueOne(void *queue);
extern u32 _tosGetProfileU32(void *block, u32 key, u32 defaultValue);
extern u32 _tosErrorLog(void *block, u32 error);
extern void tosBaseBlockDestruct(void *block);
extern void *tosBaseBlockConstruct(
    void *block, void *profile, u32 inputCount, u32 outputCount,
    TosProcessFunction process, TosInitFunction init,
    TosControlFunction control, u32 size);

#endif
