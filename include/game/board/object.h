#ifndef _BOARD_OBJECT_H
#define _BOARD_OBJECT_H

#include "game/charman.h"
#include "game/hu3d.h"

#define MB_OBJ_MAX 512
#define MB_OBJ_MOT_MAX CHAR_MOT_MAX
#define MB_MODEL_NONE -1

typedef s16 MBMODELID;

typedef struct MbObjModel_s MBOBJMODEL;

struct MbObjModel_s {
    /* 0x000 */ u8 dispF : 1;
    /* 0x000 */ u8 dispOldF : 1;
    /* 0x000 */ u8 fadeF : 1;
    /* 0x000 */ u8 : 5;
    /* 0x002 */ u16 cameraBit;
    /* 0x004 */ u8 layer;
    /* 0x005 */ s8 charNo;
    /* 0x006 */ MBMODELID id;
    /* 0x008 */ HU3D_MODELID modelId;
    /* 0x00A */ s16 motNum;
    /* 0x00C */ HU3D_MOTIONID motId[MB_OBJ_MOT_MAX];
    /* 0x04C */ s16 motNo;
    /* 0x050 */ int dataNum;
    /* 0x054 */ s16 linkNum;
    /* 0x058 */ MBOBJMODEL *linkP;
    /* 0x05C */ void *motData[MB_OBJ_MOT_MAX];
    /* 0x0DC */ int motDataNum[MB_OBJ_MOT_MAX];
    /* 0x15C */ float motStart;
    /* 0x160 */ float motEnd;
    /* 0x164 */ HuVecF pos;
    /* 0x170 */ HuVecF rot;
    /* 0x17C */ HuVecF scale;
    /* 0x188 */ HuVecF offset;
    /* 0x194 */ float cullRadius;
    /* 0x198 */ u8 alpha;
};

int mbObjDataNumGet(int dataNum);
void mbObjInit(void);
void mbObjClose(void);

MBMODELID mbObjCreate(int dataNum, const int *motDataNum, BOOL linkF);
MBMODELID mbObjCharCreate(int charNo, int dataNum, const int *motDataNum, BOOL linkF);
void mbObjKill(MBMODELID modelId);

void mbObjDirSet(int dataDirDay, int dataDirNight);
MBOBJMODEL *mbObjGet(MBMODELID modelId);

BOOL mbObjDispGet(MBMODELID modelId);
void mbObjDispSet(MBMODELID modelId, BOOL dispF);
void mbObjCameraSet(MBMODELID modelId, u16 cameraBit);
int mbObjLayerGet(MBMODELID modelId);
void mbObjLayerSet(MBMODELID modelId, u8 layer);
void mbObjLayerSetAll(u8 layer);
HU3D_MODELID mbObjModelIDGet(MBMODELID modelId);

void mbObjPosGet(MBMODELID modelId, HuVecF *pos);
void mbObjPosSetV(MBMODELID modelId, const HuVecF *pos);
void mbObjPosSet(MBMODELID modelId, float x, float y, float z);
void mbObjOffsetGet(MBMODELID modelId, HuVecF *offset);
void mbObjOffsetSetV(MBMODELID modelId, const HuVecF *offset);
void mbObjOffsetSet(MBMODELID modelId, float x, float y, float z);
void mbObjRotGet(MBMODELID modelId, HuVecF *rot);
void mbObjRotSetV(MBMODELID modelId, const HuVecF *rot);
void mbObjRotSet(MBMODELID modelId, float x, float y, float z);
float mbObjRotYGet(MBMODELID modelId);
void mbObjRotYSet(MBMODELID modelId, float rotY);
void mbObjScaleGet(MBMODELID modelId, HuVecF *scale);
void mbObjScaleSetV(MBMODELID modelId, const HuVecF *scale);
void mbObjScaleSet(MBMODELID modelId, float x, float y, float z);
void mbObjMtxGet(MBMODELID modelId, Mtx *mtx);
void mbObjMtxSet(MBMODELID modelId, Mtx *mtx);

void mbObjAttrSet(MBMODELID modelId, u32 attr);
void mbObjAttrReset(MBMODELID modelId, u32 attr);
void mbObjAmbSet(MBMODELID modelId, float r, float g, float b);
void mbObjAmbSetAll(float r, float g, float b);
u8 mbObjAlphaGet(MBMODELID modelId);
void mbObjAlphaSet(MBMODELID modelId, u8 alpha);
void mbObjFadeSet(MBMODELID modelId, BOOL fadeF);
void mbObjZWriteOffSet(MBMODELID modelId, BOOL zWriteOffF);
void mbObjCullRadiusSet(MBMODELID modelId, float radius);

void mbObjHookSet(MBMODELID modelId, char *objName, MBMODELID hookModelId);
void mbObjHookReset(MBMODELID modelId);
void mbObjHookObjReset(MBMODELID modelId, char *objName);
int mbObjSePlay(MBMODELID modelId, s16 seId);

void mbObjMotionVoiceOnSet(MBMODELID modelId, int motNo, BOOL voiceOnF);
int mbObjMotionGet(MBMODELID modelId);
void mbObjMotionSet(MBMODELID modelId, int motNo, u32 attr);
HU3D_MOTIONID mbObjMotionIDCurGet(MBMODELID modelId);
HU3D_MOTIONID mbObjMotionIDGet(MBMODELID modelId, int motNo);
void mbObjMotionShiftSet(MBMODELID modelId, int motNo, float start, float end, u32 attr);
HU3D_MOTIONID mbObjMotionShiftIDGet(MBMODELID modelId);
void mbObjMotionNoCreate(MBMODELID modelId, int dataNum, int motNo);
int mbObjMotionCreate(MBMODELID modelId, int dataNum);
void mbObjMotionKill(MBMODELID modelId, int motNo);
float mbObjMotionTimeGet(MBMODELID modelId);
void mbObjMotionTimeSet(MBMODELID modelId, float time);
float mbObjMotionMaxTimeGet(MBMODELID modelId);
float mbObjMotionSpeedGet(MBMODELID modelId);
void mbObjMotionSpeedSet(MBMODELID modelId, float speed);
BOOL mbObjMotionEndCheck(MBMODELID modelId);
void mbObjMotionStartEndSet(MBMODELID modelId, s16 start, s16 end);
void mbObjMotionLoopSet(MBMODELID modelId, BOOL loopF);

float mbObjMotionShapeTimeGet(MBMODELID modelId);
void mbObjMotionShapeSet(MBMODELID modelId, int motNo, u32 attr);
void mbObjMotionShapeTimeSet(MBMODELID modelId, float time);
float mbObjMotionShapeMaxTimeGet(MBMODELID modelId);
void mbObjMotionShapeSpeedSet(MBMODELID modelId, float speed);
float mbObjMotionShapeSpeedGet(MBMODELID modelId);
BOOL mbObjMotionShapeEndCheck(MBMODELID modelId);
void mbObjMotionShapeStartEndSet(MBMODELID modelId, s16 start, s16 end);

void mbObjShadowSet(MBMODELID modelId);
void mbObjShadowReset(MBMODELID modelId);
void mbObjShadowMapSet(MBMODELID modelId);
void mbObjShadowMapReset(MBMODELID modelId);
int mbObjStub0(void);

#endif
