#ifndef _BOARD_EFFECT_H
#define _BOARD_EFFECT_H

#include "game/frand.h"
#include "game/hu3d.h"
#include "game/sprite.h"

#define MB_PARTICLE_BLEND_NORMAL 0
#define MB_PARTICLE_BLEND_ADDCOL 1
#define MB_PARTICLE_BLEND_INVCOL 2

#define MB_PARTICLE_ATTR_LOOP (1 << 0)
#define MB_PARTICLE_ATTR_STOPCNT (1 << 1)
#define MB_PARTICLE_ATTR_UPAUSE (1 << 3)
#define MB_PARTICLE_ATTR_3D (1 << 4)

#define mbParticleRandF() (0.000015258789f * (frand() & 0xFFFF))
#define mbParticleSRandF() ((0.000030517578f * (frand() & 0xFFFF)) - 1.0f)

typedef struct MbParticle_s MBPARTICLE;

typedef struct MbParticleData_s {
    s16 time;
    union {
        s16 activeF;
        s16 no;
    };
    s16 unk04;
    s16 cameraBit;
    HuVecF vel;
    union {
        HuVecF accel;
        struct {
            float guideScaleBase;
            float alphaF;
            float guideUnused1C;
        };
    };
    union {
        struct {
            float speedDecay;
            float colorIdx;
            float scaleBase;
        };
        HuVecF guideAccel;
        s32 vertexNo;
    };
    float scale;
    union {
        HuVecF rot;
        struct {
            float guideUnused30;
            float guideUnused34;
            float weight;
        };
    };
    HuVecF pos;
    GXColor color;
    union {
        s16 animBank;
        s16 rndNo;
    };
    s16 animNo;
    float animSpeed;
    float animTime;
    u8 dispF;
    u8 pauseF;
} MBPARTICLEDATA;

typedef void (*MBPARTICLEHOOK)(HU3D_MODEL *modelP, MBPARTICLE *particleP, Mtx mtx);

struct MbParticle_s {
    union {
        s16 mode;
        s16 stopF;
    };
    s16 time;
    HuVecF vel;
    union {
        s16 work[8];
        struct {
            s16 _work10[6];
            void *hookData;
        };
        struct {
            u8 guideUnused10[4];
            float unk14;
            u8 guideUnused18[8];
        };
    };
    u8 blendMode;
    u8 attr;
    s16 colorF;
    HU3D_MODELID modelId;
    s16 num;
    union {
        u32 count;
        u32 initF;
    };
    u32 prevCounter;
    u32 prevCount;
    GXTevColorArg colorIn[4];
    GXTevAlphaArg alphaIn[4];
    GXColor tevColor[2];
    u32 dlSize;
    ANIMDATA *anim;
    HuVec2f *animSt;
    MBPARTICLEDATA *data;
    HuVecF *vertex;
    HuVec2f *st;
    GXColor *color;
    void *dl;
    MBPARTICLEHOOK hook;
};

typedef struct MbParMan_s {
    MBPARTICLE particle;
    HU3D_PARMAN parMan;
} MBPARMAN;

void mbEffInit(void);
void mbEffClose(void);
void mbEffFadeOutSet(s16 maxTime);
void mbEffFadeCreate(s16 maxTime, u8 alpha);
BOOL mbEffFadeDoneCheck(void);
void mbEffFadeCameraSet(u16 cameraBit);
BOOL mbEffFadeCheck(void);
void mbEffConfettiCreate(HuVecF *pos, s16 maxCnt, float width);
void mbEffConfettiKill(void);
void mbEffConfettiReset(void);

HU3D_MODELID mbParticleCreate(ANIMDATA *anim, s16 maxCnt);
void mbParticleKill(HU3D_MODELID modelId);
void mbParticleColorCreate(HU3D_MODELID modelId);
void mbParticleHookSet(HU3D_MODELID modelId, MBPARTICLEHOOK hook);
void mbParticleAttrSet(HU3D_MODELID modelId, u8 attr);
void mbParticleAttrReset(HU3D_MODELID modelId, u8 attr);
MBPARTICLEDATA *mbParticleDataCreate(MBPARTICLE *particleP);
int mbParticleUnkTotalGet(ANIMDATA *anim, int bankNo);

HU3D_MODELID mbParManCreate(ANIMDATA *anim, s16 maxCnt, HU3D_PARMAN_PARAM *param);
void mbParManKill(HU3D_MODELID modelId);
void mbParticleBlendModeSet(HU3D_MODELID modelId, u8 blendMode);
void mbParManPosSet(HU3D_MODELID modelId, float x, float y, float z);
void mbParManVecSet(HU3D_MODELID modelId, float x, float y, float z);
void mbParManRotSet(HU3D_MODELID modelId, float rotX, float rotY, float rotZ);
void mbParManAttrSet(HU3D_MODELID modelId, s32 attr);
void mbParManAttrReset(HU3D_MODELID modelId, s32 attr);

#endif
