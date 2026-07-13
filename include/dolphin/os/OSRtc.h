#ifndef _DOLPHIN_OSRTC_H_
#define _DOLPHIN_OSRTC_H_

#include <dolphin/types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OSSram {
    u16 checkSum;
    u16 checkSumInv;
    u32 ead0;
    u32 ead1;
    u32 counterBias;
    s8 displayOffsetH;
    u8 ntd;
    u8 language;
    u8 flags;
} OSSram;

typedef struct OSSramEx {
    u8 flashID[2][12];
    u32 wirelessKeyboardID;
    u16 wirelessPadID[4];
    u8 dvdErrorCode;
    u8 _padding0;
    u8 flashIDCheckSum[2];
    u16 gbs;
    u8 _padding1[2];
} OSSramEx;

#define SRAM_SIZE (sizeof(OSSram) + sizeof(OSSramEx))

typedef struct SramControl {
    u8 sram[SRAM_SIZE];
    u32 offset;
    BOOL enabled;
    BOOL locked;
    int sync;
    void (*callback)(void);
} SramControl;

u32 OSGetSoundMode(void);
void OSSetSoundMode(u32 mode);
u8 OSGetLanguage(void);
u16 OSGetGbsMode(void);
void OSSetGbsMode(u16 mode);
u32 OSGetProgressiveMode(void);
void OSSetProgressiveMode(u32 on);
u16 OSGetWirelessID(s32 chan);
void OSSetWirelessID(s32 chan, u16 id);

#ifdef __cplusplus
}
#endif

#endif
