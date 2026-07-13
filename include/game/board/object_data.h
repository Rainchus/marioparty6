#ifndef _BOARD_OBJECT_DATA_H
#define _BOARD_OBJECT_DATA_H

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

#endif
