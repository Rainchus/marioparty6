#ifndef _BOARD_GUIDE_H
#define _BOARD_GUIDE_H

#include "game/board/effect.h"
#include "game/board/main.h"
#include "game/board/object.h"
#include "game/object.h"
#include "game/hu3d.h"
#include "game/sprite.h"

#define MB_GUIDE_ATTR_NONE 0
#define MB_GUIDE_ATTR_SCREEN (1 << 0)
#define MB_GUIDE_ATTR_ALTMTX (1 << 1)
#define MB_GUIDE_ATTR_LAYER (1 << 2)

typedef struct GuideWork_s {
    /* 0x00 */ u32 name;
    /* 0x04 */ u16 dispF : 1;
    /* 0x04 */ u16 killF : 1;
    /* 0x04 */ u16 screenF : 1;
    /* 0x04 */ u16 altMtxF : 1;
    /* 0x04 */ u16 motionF : 1;
    /* 0x04 */ u16 mode : 4;
    /* 0x04 */ u16 pad : 7;
    /* 0x06 */ s16 time;
    /* 0x08 */ s16 timeMax;
    /* 0x0A */ s16 phase;
    /* 0x0C */ s16 nextMotion;
} GUIDE_WORK;

// Guide public API
void mbGuideInit(void);
OMOBJ *mbGuideCreate(int guideNo, HuVecF *pos, HuVecF *rot, s8 *motTbl, float scale, u32 attr);
OMOBJ *mbGuideCreateFlag(HuVecF *pos, s8 *motTbl, BOOL screenF, BOOL altMtxF, BOOL layerF);
OMOBJ *mbGuideCreateIn(void);
void mbGuideKill(OMOBJ *obj);
void mbGuideEnd(OMOBJ *obj);
void mbGuideFadeIn(OMOBJ *obj);
void mbGuideFadeOut(OMOBJ *obj);
int mbGuideModelGet(OMOBJ *obj);
void mbGuideScreenSet(OMOBJ *obj, BOOL screenF);
BOOL mbGuideIdleCheck(OMOBJ *obj);
void mbGuideMotionNextSet(OMOBJ *obj, s16 motNo);
void mbGuideMotionSet(OMOBJ *obj, s16 motNo, BOOL shiftF);
void mbGuideMotionShiftSet(OMOBJ *obj, s16 motNo, BOOL shiftF);
void mbGuideMotionStop(OMOBJ *obj);
BOOL mbGuideMotionCheck(OMOBJ *obj);
int mbGuideNoGet(void);
int mbGuideSpeakerNoGet(void);

void mbObjFadeCreate(int modelId, HuVecF *pos);
void mbObjFadeKill(int modelId);
void mbObjFadeTexColorSet(int modelId, int r, int g, int b, float a);
void mbObjFadeTexRotSet(int modelId, HuVecF *pos, HuVecF *rot);

float mbSinDeg(float deg);
float mbCosDeg(float deg);
void mbMtxRotAxisDeg(Mtx m, char axis, float deg);
BOOL mbPauseProcCheck(void);

#endif
