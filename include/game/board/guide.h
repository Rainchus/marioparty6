#ifndef _BOARD_GUIDE_H
#define _BOARD_GUIDE_H

#include "game/board/effect.h"
#include "game/board/main.h"
#include "game/board/object_data.h"
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

// Board object model system (linked from DOL)
MBMODELID mbObjCreate(int dataNum, int *motData, BOOL link);
int mbObjMotionNoCreate(int modelId, int dataNum, int motNo);
int mbObjMotionIDGet(int modelId, int motNo);
int mbObjMotionShiftIDGet(int modelId);
BOOL mbObjMotionEndCheck(int modelId);
void mbObjMotionSet(int modelId, int motNo, u32 attr);
void mbObjMotionShiftSet(int modelId, int motNo, float start, float end, BOOL loopF);
void mbObjPosSetV(int modelId, HuVecF *pos);
void mbObjPosGet(int modelId, HuVecF *pos);
void mbObjRotSet(MBMODELID modelId, float rotX, float rotY, float rotZ);
void mbObjRotSetV(int modelId, HuVecF *rot);
void mbObjScaleSet(int modelId, float x, float y, float z);
void mbObjScaleGet(int modelId, HuVecF *scale);
void mbObjMtxSet(int modelId, Mtx *mtx);
void mbObjMtxGet(int modelId, Mtx *mtx);
void mbObjCameraSet(int modelId, u16 cameraBit);
void mbObjLayerSet(int modelId, int layer);
void mbObjAttrSet(int modelId, u32 attr);
void mbObjDispSet(int modelId, BOOL dispF);
void mbObjKill(int modelId);
void mbObjHookSet(int modelId, char *objName, int hookMdlId);
MBOBJMODEL *mbObjGet(int modelId);

void mbObjFadeCreate(int modelId, HuVecF *pos);
void mbObjFadeKill(int modelId);
void mbObjFadeTexColorSet(int modelId, int r, int g, int b, float a);
void mbObjFadeTexRotSet(int modelId, HuVecF *pos, HuVecF *rot);

float mbSinDeg(float deg);
float mbCosDeg(float deg);
void mbMtxRotAxisDeg(Mtx m, char axis, float deg);
BOOL mbPauseProcCheck(void);

#endif
