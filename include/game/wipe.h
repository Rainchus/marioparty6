#ifndef _GAME_WIPE_H
#define _GAME_WIPE_H

#include "dolphin.h"
#include "game/animdata.h"
#include "game/process.h"

#define WIPE_TYPE_MASK 0x7F
#define WIPE_TYPE_FBKEEP 0x80

typedef enum {
    WIPE_TYPE_NORMAL, //0
    WIPE_TYPE_CROSS, //1
    WIPE_TYPE_DISSOLVE_IN, //2
    WIPE_TYPE_DISSOLVE_OUT, //3
    WIPE_TYPE_VIEW_SHIFT, //4
    WIPE_TYPE_PREV, //5
    WIPE_TYPE_WHITE, //6
    WIPE_TYPE_STAR, //7
    WIPE_TYPE_WAVE, //8
    WIPE_TYPE_KOOPA, //9
    WIPE_TYPE_MOON, //10
    WIPE_TYPE_SUN, //11
    WIPE_TYPE_SUNMOON, //12
    WIPE_TYPE_MAX, //13
    
    WIPE_TYPE_CROSS_COPY = (WIPE_TYPE_CROSS|WIPE_TYPE_FBKEEP),
    WIPE_TYPE_DISSOLVE_IN_BLUR = (WIPE_TYPE_DISSOLVE_IN|WIPE_TYPE_FBKEEP),
    WIPE_TYPE_DISSOLVE_OUT_BLUR = (WIPE_TYPE_DISSOLVE_OUT|WIPE_TYPE_FBKEEP),
} WIPETYPE;

typedef enum {
    WIPE_MODE_DUMMY, //0
    WIPE_MODE_IN, //1
    WIPE_MODE_OUT, //2
    WIPE_MODE_END //3
} WIPEMODE;

typedef struct WipeWork_s {
	void *image[2];
	void *unk08[8];
    float time;
    float maxTime;
    u32 unk30;
	GXColor color;
	u8 type;
    u8 mode;
	u8 stat;
} WIPEWORK;

void WipeInit(GXRenderModeObj *rmode);
void WipeExecAlways(void);
void WipeCreate(s16 mode, s16 type, s16 maxTime);
void WipeColorSet(u8 r, u8 g, u8 b);
u8 WipeTypeGet(void);
u8 WipeCheckIn(void);
u8 WipeCheck(void);

void WipeLoadCreate(void);
void WipeLoadKill(void);

static inline void WipeWait(void)
{
    while(WipeCheck()) {
        HuPrcVSleep();
    }
}

extern WIPEWORK wipeData;
extern BOOL wipeFadeInF;

#endif