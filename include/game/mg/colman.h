#ifndef _MG_COLMAN_H
#define _MG_COLMAN_H

#include <dolphin.h>
#include "game/hu3d.h"

#define COLBODY_MAX 256
#define COLBODY_ATTR_ACTIVE (1 << 0)
#define COLBODY_ATTR_MESHCOL_OFF (1 << 1)
#define COLBODY_ATTR_BODYCOL_OFF (1 << 2)
#define COLBODY_ATTR_COL_OFF (1 << 4)

#define COLBODY_ATTR_RESET (1 << 24)

typedef struct ColNarrowParam_s {
    int paramA;
    int paramB;
    int type;
    HuVecF point;
    HuVecF normPos;
    int colResult;
} COL_NARROW_PARAM;

typedef struct ColBody_s COLBODY;

typedef int (*COL_NARROW_HOOK)(COL_NARROW_PARAM *a, COL_NARROW_PARAM *b);
typedef void (*COL_CORRECT_HOOK)(COLBODY *body, void *user);


typedef struct ColAttrParam_s {
    int type;
    float speed;
    float yDeviate;
    float maxDot;
    u32 attr;
} COL_ATTRPARAM;

#endif
