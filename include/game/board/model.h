#ifndef _BOARD_MODEL_H
#define _BOARD_MODEL_H

#include "humath.h"

typedef s16 MBMODELID;

#define MB_MODEL_NONE -1

MBMODELID mbObjCreate(int dataNum, int *motFileTbl, BOOL animOffF);
void mbObjKill(MBMODELID modelId);
void mbObjMotionSet(MBMODELID modelId, int motNo, u32 attr);
void mbObjMotionShiftSet(MBMODELID modelId, int motNo, float start, float end, u32 attr);
float mbObjMotionTimeGet(MBMODELID modelId);
void mbObjPosSetV(MBMODELID modelId, HuVecF *pos);
void mbObjPosGet(MBMODELID modelId, HuVecF *pos);
void mbObjRotSet(MBMODELID modelId, float rotX, float rotY, float rotZ);

#endif
