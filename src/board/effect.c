#include "game/board/effect.h"
#include "dolphin/os/OSFastCast.h"
#include "game/board/main.h"
#include "game/board/model.h"
#include "game/frand.h"
#include "game/main.h"
#include "game/init.h"
#include "game/disp.h"
#include "game/memory.h"

#include "string.h"

#define DATA_board 0x50000

extern void *mbMallocNum(s32 size, u32 num);
extern void *mbMallocFlushModelNum(s32 size, u32 num);
extern HU3D_MODELID mbObjModelIDGet(MBMODELID modelId);
extern void mbObjLayerSet(MBMODELID modelId, s16 layer);
extern void mbObjDispSet(MBMODELID modelId, BOOL dispF);
extern void mbMtxRot(Mtx mtx, float x, float y, float z);
extern void mbMtxRotAxisDeg(Mtx mtx, char axis, float angle);
extern float mbSinDeg(float angle);
extern float mbCosDeg(float angle);

static int tempColorNum;
static void *tempColorBuf;
static OMOBJ *confettiOMObj;
static OMOBJ *fadeOMObj;

void mbEffInit(void)
{
    fadeOMObj = NULL;
    confettiOMObj = NULL;
    tempColorBuf = HuMemDirectMallocNum(HEAP_HEAP, 0x4000, HU_MEMNUM_OVL);
    tempColorNum = 0;
}

void mbEffClose(void)
{
    if (tempColorBuf) {
        HuMemDirectFree(tempColorBuf);
        tempColorBuf = NULL;
    }
}

typedef struct fadeEffWork_s {
    unsigned killF : 1;
    unsigned pauseF : 1;
    u8 alpha;
    s16 time;
    s16 maxTime;
    HU3D_MODELID modelId;
    GXColor color;
    float speed;
} FADEEFFWORK;

static void FadeOMExec(OMOBJ *obj);
static void FadeDraw(HU3D_MODEL *modelP, Mtx *mtx);

void mbEffFadeOutSet(s16 maxTime)
{
    FADEEFFWORK *work;
    float maxTimeF;
    if(!fadeOMObj) {
        return;
    }
    if(maxTime <= 0) {
        maxTime = 1;
    }
    work = omObjGetWork(fadeOMObj, FADEEFFWORK);
    work->maxTime = maxTime;
    OSs16tof32(&maxTime, &maxTimeF);
    work->speed = -work->color.a/maxTimeF;
    work->pauseF = FALSE;
    work->time = work->maxTime;
}

void mbEffFadeCreate(s16 maxTime, u8 alpha)
{
    FADEEFFWORK *work;
    if(fadeOMObj) {
        work = omObjGetWork(fadeOMObj, FADEEFFWORK);
        work->killF = TRUE;
        while(fadeOMObj) {
            HuPrcVSleep();
        }
    }
    fadeOMObj = omAddObjEx(mbObjMan, 32000, 0, 0, OM_GRP_NONE, FadeOMExec);
    omSetStatBit(fadeOMObj, OM_STAT_NOPAUSE|OM_STAT_SPRPAUSE);
    if(maxTime <= 0) {
        maxTime = 1;
    }
    work = omObjGetWork(fadeOMObj, FADEEFFWORK);
    work->killF = FALSE;
    work->pauseF = FALSE;
    work->color.r = 0;
    work->color.g = 0;
    work->color.b = 0;
    work->color.a = 0;
    work->alpha = alpha;
    work->speed = (float)(alpha-work->color.a)/maxTime;
    work->time = maxTime;
    work->maxTime = maxTime;
    work->modelId = Hu3DHookFuncCreate(FadeDraw);
    Hu3DModelCameraSet(work->modelId, HU3D_CAM2);
    Hu3DModelLayerSet(work->modelId, 1);
}

BOOL mbEffFadeDoneCheck(void)
{
    FADEEFFWORK *work;
    if(!fadeOMObj) {
        return TRUE;
    }
    work = omObjGetWork(fadeOMObj, FADEEFFWORK);
    return (work->pauseF) ? TRUE : FALSE;
}

void mbEffFadeCameraSet(u16 bit)
{
    if(fadeOMObj) {
        FADEEFFWORK *work = omObjGetWork(fadeOMObj, FADEEFFWORK);
        Hu3DModelCameraSet(work->modelId, bit);
    }
}

BOOL mbEffFadeCheck(void)
{
    return (fadeOMObj != NULL) ? FALSE : TRUE;
}

static void FadeOMExec(OMOBJ *obj)
{
    FADEEFFWORK *work = omObjGetWork(obj, FADEEFFWORK);
    float alpha;
    if(work->killF || mbExitCheck()) {
        if(work->modelId != HU3D_MODELID_NONE) {
            Hu3DModelKill(work->modelId);
        }
        fadeOMObj = NULL;
        omDelObjEx(HuPrcCurrentGet(), obj);
        return;
    }
    if(work->pauseF) {
        return;
    }
    OSu8tof32(&work->color.a, &alpha);
    alpha += work->speed;
    OSf32tou8(&alpha, &work->color.a);
    if(work->time > 0) {
        work->time--;
        return;
    }
    if(work->speed > 0) {
        work->pauseF = TRUE;
        work->color.a = work->alpha;
    } else {
        work->killF = TRUE;
    }
}

static void FadeDraw(HU3D_MODEL *modelP, Mtx *mtx)
{
    static GXColor colorN = { 0xFF, 0xFF, 0xFF, 0xFF };
	Mtx44 proj;
	Mtx modelview;

	float x1, x2, y1, y2;
	FADEEFFWORK *work;
	if(!fadeOMObj) {
		return;
	}
	work = omObjGetWork(fadeOMObj, FADEEFFWORK);
	x1 = 0.0f;
	x2 = HU_FB_WIDTH;
	y1 = 0.0f;
	y2 = HU_FB_HEIGHT;
	MTXOrtho(proj, y1, y2, x1, x2, 0, 10);
	GXSetProjection(proj, GX_ORTHOGRAPHIC);
	MTXIdentity(modelview);
	GXLoadPosMtxImm(modelview, GX_PNMTX0);
	GXSetCurrentMtx(GX_PNMTX0);
	GXSetViewport(0, 0, x2, 1.0f+y2, 0, 1);
	GXSetScissor(0, 0, x2, 1.0f+y2);
	GXClearVtxDesc();
	GXSetChanMatColor(GX_COLOR0A0, work->color);
	GXSetNumChans(1);
	GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE, GX_AF_NONE);
	GXSetChanCtrl(GX_COLOR1A1, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE, GX_AF_NONE);
	GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
	GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
	GXSetNumTexGens(0);
	GXSetNumTevStages(1);
	GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XY, GX_U16, 0);
	GXSetZMode(GX_TRUE, GX_ALWAYS, GX_FALSE);
	GXSetAlphaUpdate(GX_FALSE);
	GXSetColorUpdate(GX_TRUE);
	GXSetAlphaCompare(GX_GEQUAL, 1, GX_AOP_AND, GX_GEQUAL, 1);
	GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
	GXBegin(GX_QUADS, GX_VTXFMT0, 4);
	GXPosition2u16(x1, y1);
	GXPosition2u16(x2, y1);
	GXPosition2u16(x2, y2);
	GXPosition2u16(x1, y2);
	GXEnd();
	GXSetChanMatColor(GX_COLOR0A0, colorN);
}

typedef struct confettiEffData_s {
    s16 time;
    u8 alpha;
    u8 ambNo;
    HuVecF pos;
    HuVecF rot;
    HuVecF vel;
    HuVecF rotVel;
} CONFETTIEFFDATA;

typedef struct confettiEffWork_s {
    unsigned killF : 1;
    unsigned pauseF : 1;
    s8 addNum;
    s8 time;
    s8 delay;
    s16 maxCnt;
    MBMODELID modelId;
    HU3D_MODELID hookMdlId;
    CONFETTIEFFDATA *data;
} CONFETTIEFFWORK;

static void EffConfettiOMExec(OMOBJ *obj);
static void EffConfettiAdd(OMOBJ *obj);
static void EffConfettiUpdate(OMOBJ *obj);
static void EffConfettiDraw(HU3D_MODEL *modelP, Mtx *mtx);

void mbEffConfettiCreate(HuVecF *pos, s16 maxCnt, float width)
{
    CONFETTIEFFWORK *work;
    OMOBJ *obj;
    CONFETTIEFFDATA *data;
    int i;
    if(confettiOMObj) {
        mbEffConfettiReset();
        HuPrcSleep(17);
    }
    confettiOMObj = obj = omAddObjEx(mbObjMan, 257, 0, 0, OM_GRP_NONE, EffConfettiOMExec);
    work = omObjGetWork(obj, CONFETTIEFFWORK);
    work->killF = FALSE;
    work->pauseF = FALSE;
    work->maxCnt = maxCnt;
    work->addNum = 1;
    work->time = 0;
    work->delay = 10;
    work->hookMdlId = Hu3DHookFuncCreate(EffConfettiDraw);
    Hu3DModelCameraSet(work->hookMdlId, HU3D_CAM0);
    work->data = HuMemDirectMallocNum(HEAP_HEAP, work->maxCnt*sizeof(CONFETTIEFFDATA), HU_MEMNUM_OVL);
    obj->trans.x = pos->x;
    obj->trans.y = pos->y;
    obj->trans.z = pos->z;
    obj->rot.x = width;
    work->modelId = mbObjCreate(mbBoardDataNumGet(DATANUM(DATA_board, 2)), NULL, FALSE);
    mbObjLayerSet(work->modelId, 5);
    mbObjDispSet(work->modelId, FALSE);
    for(data=work->data, i=0; i<work->maxCnt; i++, data++) {
        data->time = -1;
    }
}

void mbEffConfettiKill(void)
{
    if(confettiOMObj) {
        omObjGetWork(confettiOMObj, CONFETTIEFFWORK)->killF = TRUE;
    }
}

void mbEffConfettiReset(void)
{
    CONFETTIEFFWORK *work;
    CONFETTIEFFDATA *data;
    int i;
    if(!confettiOMObj) {
        return;
    }
    work = omObjGetWork(confettiOMObj, CONFETTIEFFWORK);
    work->pauseF = TRUE;
    for(data=work->data, i=0; i<work->maxCnt; i++, data++) {
        if(data->time != -1 && data->time > 16) {
            data->time = 16;
        }
    }
}

static void EffConfettiOMExec(OMOBJ *obj)
{
    CONFETTIEFFWORK *work = omObjGetWork(obj, CONFETTIEFFWORK);
    if(work->killF || mbExitCheck()) {
        mbObjKill(work->modelId);
        Hu3DModelKill(work->hookMdlId);
        HuMemDirectFree(work->data);
        confettiOMObj = NULL;
        omDelObjEx(HuPrcCurrentGet(), obj);
        return;
    }
    EffConfettiAdd(obj);
    EffConfettiUpdate(obj);
}

static void EffConfettiAdd(OMOBJ *obj)
{
    CONFETTIEFFWORK *work = omObjGetWork(obj, CONFETTIEFFWORK);
    int j, i;
    CONFETTIEFFDATA *data;
    if(work->pauseF) {
        return;
    }
    if(work->addNum < 5) {
        if(work->time++ > work->delay) {
            work->time = 0;
            work->addNum++;
        }
    }
    for(j=0; j<work->addNum; j++) {
        float angle;
        for(data=work->data, i=0; i<work->maxCnt; i++, data++) {
            if(data->time == -1) {
                break;
            }
        }
        if(i == work->maxCnt) {
            break;
        }
        data->time = frandmod(60)+120;
        angle = frandf()*360;
        data->pos.x = (obj->rot.x*HuSin(angle))+obj->trans.x;
        data->pos.y = obj->trans.y;
        data->pos.z = (obj->rot.x*HuCos(angle))+obj->trans.z;
        data->vel.x = (frandf()-0.5f)*2;
        data->vel.y = -6.533334f*frandf();
        data->vel.z = (frandf()-0.5f)*2;
        data->rotVel.x = 8.0f+((frandf()-0.5f)*20.0f);
        data->rotVel.y = 8.0f+((frandf()-0.5f)*20.0f);
        data->rotVel.z = 8.0f+((frandf()-0.5f)*20.0f);
        data->rot.x = 0;
        data->rot.y = 0;
        data->rot.z = 0;
        data->alpha = 255;
        data->ambNo = frandmod(6);
    }
}

static void EffConfettiUpdate(OMOBJ *obj)
{
    CONFETTIEFFDATA *data;
    int i;
    CONFETTIEFFWORK *work = omObjGetWork(obj, CONFETTIEFFWORK);
    BOOL noKillF = FALSE;

    for(data=work->data, i=0; i<work->maxCnt; i++, data++) {
        if(data->time == -1) {
            continue;
        }
        if(data->time <= 0) {
            data->time = -1;
            continue;
        }
        data->time--;
        data->pos.x += data->vel.x;
        data->pos.y += data->vel.y;
        data->pos.z += data->vel.z;
        data->rot.x += data->rotVel.x;
        data->rot.y += data->rotVel.y;
        data->rot.z += data->rotVel.z;
        if(data->time < 16) {
            if(data->alpha >= 15) {
                data->alpha -= 15;
            } else {
                data->alpha = 0;
            }
        }
        if(!noKillF) {
            noKillF = TRUE;
        }
    }
    if(noKillF == FALSE && work->pauseF) {
        work->killF = TRUE;
    }
}

static void EffConfettiDraw(HU3D_MODEL *modelP, Mtx *mtx)
{
    static HuVecF ambTbl[6] = {
        { 0.1, 0.4, 1 },
        { 0.2, 1, 0.1 },
        { 0.3, 1, 1 },
        { 1, 0.2, 0.1 },
        { 1, 0.2, 0.8 },
        { 1, 8, 0.3 }
    };
    if(!confettiOMObj || mbExitCheck()) {
        return;
    } else {
        CONFETTIEFFWORK *work = omObjGetWork(confettiOMObj, CONFETTIEFFWORK);
        HU3D_MODEL *modelDataP = &Hu3DData[mbObjModelIDGet(work->modelId)];
        CONFETTIEFFDATA *data;
        int i;
        if(!modelDataP->hsf) {
            return;
        }
        for(data=work->data, i=0; i<work->maxCnt; i++, data++) {
            Mtx final, temp;
            float ambR, ambG, ambB, tpLvl;
            if(data->time == -1) {
                continue;
            }
			mbMtxRotAxisDeg(temp, 'z', data->rot.z);
			mbMtxRotAxisDeg(final, 'x', data->rot.x);
			MTXConcat(temp, final, final);
			mbMtxRotAxisDeg(temp, 'y', data->rot.y);
			MTXConcat(temp, final, final);
			MTXTrans(temp, data->pos.x, data->pos.y, data->pos.z);
			MTXConcat(temp, final, final);
			MTXConcat(*mtx, final, final);
            ambR = ambTbl[data->ambNo].x;
            ambG = ambTbl[data->ambNo].y;
            ambB = ambTbl[data->ambNo].z;
            OSu8tof32(&data->alpha, &tpLvl);
            tpLvl = (1.0f/255.0f)*tpLvl;
            Hu3DModelTPLvlSet(mbObjModelIDGet(work->modelId), tpLvl);
            Hu3DModelAmbSet(mbObjModelIDGet(work->modelId), ambR, ambG, ambB);
            Hu3DModelObjDraw(mbObjModelIDGet(work->modelId), "grid2", final);
        }
    }
}

static void ParticleDraw(HU3D_MODEL *modelP, Mtx *mtx);
static void ParManFunc(HU3D_MODEL *modelP, MBPARTICLE *particleP, Mtx mtx);

HU3D_MODELID mbParticleCreate(ANIMDATA *anim, s16 maxCnt)
{
    HU3D_MODELID modelId = Hu3DHookFuncCreate(ParticleDraw);
    MBPARTICLEDATA *particleDataP;
    HU3D_MODEL *modelP;
    MBPARTICLE *particleP;
    ANIMLAYER *layerP;
    ANIMBMP *bmpP;
    HuVec2f *stP;
    s16 i;

    Hu3DModelCameraSet(modelId, HU3D_CAM0);
    modelP = &Hu3DData[modelId];
    modelP->hookData = particleP = mbMallocNum(sizeof(MBPARTICLE), modelP->mallocNo);
    particleP->anim = anim;
    anim->useNum++;
    particleP->num = maxCnt;
    particleP->blendMode = MB_PARTICLE_BLEND_NORMAL;
    particleP->prevCounter = -1;
    particleP->modelId = modelId;
    if ((anim->bmp->dataFmt & ANIM_BMP_FMTMASK) == ANIM_BMP_I8 ||
        (anim->bmp->dataFmt & ANIM_BMP_FMTMASK) == ANIM_BMP_I4) {
        particleP->colorIn[0] = GX_CC_ZERO;
        particleP->colorIn[1] = GX_CC_ONE;
        particleP->colorIn[2] = GX_CC_RASC;
        particleP->colorIn[3] = GX_CC_ZERO;
    } else {
        particleP->colorIn[0] = GX_CC_ZERO;
        particleP->colorIn[1] = GX_CC_TEXC;
        particleP->colorIn[2] = GX_CC_RASC;
        particleP->colorIn[3] = GX_CC_ZERO;
    }
    particleP->alphaIn[0] = GX_CA_ZERO;
    particleP->alphaIn[1] = GX_CA_TEXA;
    particleP->alphaIn[2] = GX_CA_RASA;
    particleP->alphaIn[3] = GX_CA_ZERO;
    particleP->data = particleDataP = mbMallocNum(maxCnt * sizeof(MBPARTICLEDATA), modelP->mallocNo);
    for (i = 0; i < maxCnt; i++, particleDataP++) {
        particleDataP->cameraBit = HU3D_CAM_ALL;
        particleDataP->color.r = particleDataP->color.g = particleDataP->color.b = particleDataP->color.a = 255;
        particleDataP->dispF = TRUE;
    }
    particleP->vertex = HuMemDirectMallocNum(HEAP_MODEL, maxCnt * sizeof(HuVecF) * 4, modelP->mallocNo);
    particleP->st = HuMemDirectMallocNum(HEAP_MODEL, maxCnt * sizeof(HuVec2f) * 4, modelP->mallocNo);
    particleP->animSt = stP = HuMemDirectMallocNum(HEAP_MODEL, anim->patNum * sizeof(HuVec2f) * 4, modelP->mallocNo);
    for (i = 0; i < anim->patNum; i++) {
        layerP = anim->pat[i].layer;
        bmpP = &anim->bmp[layerP->bmpNo];
        stP->x = (float)(layerP->startX + layerP->vtx[0]) / bmpP->sizeX;
        stP->y = (float)(layerP->startY + layerP->vtx[1]) / bmpP->sizeY;
        stP++;
        stP->x = (float)(layerP->startX + layerP->vtx[2]) / bmpP->sizeX;
        stP->y = (float)(layerP->startY + layerP->vtx[3]) / bmpP->sizeY;
        stP++;
        stP->x = (float)(layerP->startX + layerP->vtx[4]) / bmpP->sizeX;
        stP->y = (float)(layerP->startY + layerP->vtx[5]) / bmpP->sizeY;
        stP++;
        stP->x = (float)(layerP->startX + layerP->vtx[6]) / bmpP->sizeX;
        stP->y = (float)(layerP->startY + layerP->vtx[7]) / bmpP->sizeY;
        stP++;
    }
    particleP->dl = mbMallocFlushModelNum((maxCnt * 24) + 128, modelP->mallocNo);
    GXBeginDisplayList(particleP->dl, 0x20000);
    GXBegin(GX_QUADS, GX_VTXFMT0, maxCnt * 4);
    for (i = 0; i < maxCnt; i++) {
        GXPosition1x16(i*4);
        GXColor1x16(i);
        GXTexCoord1x16(i*4);
        GXPosition1x16((i*4)+1);
        GXColor1x16(i);
        GXTexCoord1x16((i*4)+1);
        GXPosition1x16((i*4)+2);
        GXColor1x16(i);
        GXTexCoord1x16((i*4)+2);
        GXPosition1x16((i*4)+3);
        GXColor1x16(i);
        GXTexCoord1x16((i*4)+3);
    }
    particleP->dlSize = GXEndDisplayList();
    return modelId;
}

void mbParticleKill(HU3D_MODELID modelId)
{
    HU3D_MODEL *modelP = &Hu3DData[modelId];
    MBPARTICLE *particleP = modelP->hookData;
    HuSprAnimKill(particleP->anim);
    HuMemDirectFreeNum(HEAP_MODEL, modelP->mallocNo);
    modelP->hsf = NULL;
}

void mbParticleColorCreate(HU3D_MODELID modelId)
{
    HU3D_MODEL *modelP = &Hu3DData[modelId];
    MBPARTICLE *particleP = modelP->hookData;

    particleP->colorF = TRUE;
    if (!particleP->color) {
        particleP->color = HuMemDirectMallocNum(HEAP_MODEL, particleP->num * sizeof(GXColor), modelP->mallocNo);
    }
}

typedef struct ParticleColorSort_s {
    s32 key;
    u32 index;
} PARTICLECOLORSORT;

static void ParticleColorCopy(PARTICLECOLORSORT *color, int count)
{
    static s32 effColorNum[32];
    static s32 effColorNo[32];
    int left = 0;
    int right = count - 1;
    int stackSize = 0;
    int i;
    int j;
    int pivot;
    PARTICLECOLORSORT swap;

    for (;;) {
        if (right - left <= 10) {
            if (stackSize == 0) {
                break;
            }
            stackSize--;
            left = effColorNum[stackSize];
            right = effColorNo[stackSize];
        }
        pivot = color[(left + right) >> 1].key;
        i = left;
        j = right;
        do {
            while (color[i].key < pivot) {
                i++;
            }
            while (pivot < color[j].key) {
                j--;
            }
            if (i < j) {
                swap = color[i];
                color[i] = color[j];
                color[j] = swap;
                i++;
                j--;
            }
        } while (i < j);
        if (i - left > right - j) {
            if (i - left > 10) {
                effColorNum[stackSize] = left;
                effColorNo[stackSize] = i - 1;
                stackSize++;
            }
            left = j + 1;
        } else {
            if (right - j > 10) {
                effColorNum[stackSize] = j + 1;
                effColorNo[stackSize] = right;
                stackSize++;
            }
            right = i - 1;
        }
    }
    for (i = 1; i < count; i++) {
        swap = color[i];
        j = i - 1;
        while (j >= 0 && color[j].key > swap.key) {
            color[j + 1] = color[j];
            j--;
        }
        color[j + 1] = swap;
    }
}

static void ParticleDraw(HU3D_MODEL *modelP, Mtx *mtx)
{
    static Vec basePos[] = {
        { -0.5f,  0.5f, 0.0f },
        {  0.5f,  0.5f, 0.0f },
        {  0.5f, -0.5f, 0.0f },
        { -0.5f, -0.5f, 0.0f }
    };
    static Vec basePosRot[] = {
        { -0.5f,  0.5f, 0.0f },
        {  0.5f,  0.5f, 0.0f },
        {  0.5f, -0.5f, 0.0f },
        { -0.5f, -0.5f, 0.0f }
    };
    static HuVec2f nullVec = { 0.0f, 0.0f };
    MBPARTICLE *particleP = modelP->hookData;
    ANIMDATA *anim = particleP->anim;
    MBPARTICLEDATA *particleDataP;
    PARTICLECOLORSORT *colorSortP;
    HuVecF *vertexP;
    HuVec2f *stP;
    HuVec2f *animStP;
    ANIMBANK *animBankP;
    ANIMFRAME *animFrameP;
    ROMtx basePosMtx;
    Mtx mtxInv;
    Mtx rotMtx;
    Vec initVtx[4];
    Vec finalVtx[4];
    Vec *drawVtxP;
    MBPARTICLEHOOK hook;
    union {
        float depth;
        s32 key;
    } sortKey;
    float s;
    float c;
    BOOL particle3DF;
    int i;
    int j;

    if (HmfInverseMtxF3X3(*mtx, mtxInv) == FALSE) {
        PSMTXIdentity(mtxInv);
    }
    PSMTXReorder(mtxInv, basePosMtx);
    if (Hu3DPauseF == FALSE || (modelP->attr & HU3D_ATTR_NOPAUSE)) {
        if (particleP->hook && particleP->prevCounter != GlobalCounter) {
            hook = particleP->hook;
            hook(modelP, particleP, *mtx);
        }
    } else if (particleP->prevCounter == -1) {
        return;
    }

    colorSortP = tempColorBuf;
    if (particleP->colorF && particleP->color) {
        particleDataP = particleP->data;
        for (i = 0; i < particleP->num; i++, particleDataP++, colorSortP++) {
            sortKey.depth = particleDataP->pos.x * mtxInv[0][2]
                + particleDataP->pos.y * mtxInv[1][2]
                + particleDataP->pos.z * mtxInv[2][2];
            if (sortKey.key < 0) {
                sortKey.key = -(sortKey.key & 0x7FFFFFFF);
            }
            colorSortP->key = sortKey.key;
            colorSortP->index = i;
        }
        ParticleColorCopy(tempColorBuf, particleP->num);
    } else {
        particleP->colorF = FALSE;
    }

    particleDataP = particleP->data;
    vertexP = particleP->vertex;
    stP = particleP->st;
    PSMTXROMultVecArray(basePosMtx, basePos, initVtx, 4);
    particle3DF = (particleP->attr & MB_PARTICLE_ATTR_3D) ? TRUE : FALSE;
    colorSortP = tempColorBuf;
    for (i = 0; i < particleP->num; i++, particleDataP++, colorSortP++) {
        if (particleP->colorF) {
            particleDataP = &particleP->data[colorSortP->index];
            particleP->color[i] = particleDataP->color;
        }
        if (particleDataP->scale != 0.0f
            && (particleDataP->cameraBit & Hu3DCameraBit)
            && particleDataP->dispF) {
            if (!particle3DF) {
                if (particleDataP->rot.z == 0.0f) {
                    drawVtxP = initVtx;
                } else {
                    s = 0.5f * mbSinDeg(particleDataP->rot.z);
                    c = 0.5f * mbCosDeg(particleDataP->rot.z);
                    basePosRot[0].x = basePosRot[3].x = -s - c;
                    basePosRot[0].y = basePosRot[1].x = -s + c;
                    basePosRot[1].y = basePosRot[2].x = s + c;
                    basePosRot[2].y = basePosRot[3].y = s - c;
                    PSMTXROMultVecArray(basePosMtx, basePosRot, finalVtx, 4);
                    drawVtxP = finalVtx;
                }
            } else {
                if (particleDataP->rot.x == 0.0f
                    && particleDataP->rot.y == 0.0f
                    && particleDataP->rot.z == 0.0f) {
                    drawVtxP = basePos;
                } else {
                    mbMtxRot(rotMtx, particleDataP->rot.x, particleDataP->rot.y, particleDataP->rot.z);
                    PSMTXMultVecArray(rotMtx, basePos, finalVtx, 4);
                    drawVtxP = finalVtx;
                }
            }
            for (j = 0; j < 4; j++, drawVtxP++, vertexP++) {
                vertexP->x = drawVtxP->x * particleDataP->scale + particleDataP->pos.x;
                vertexP->y = drawVtxP->y * particleDataP->scale + particleDataP->pos.y;
                vertexP->z = drawVtxP->z * particleDataP->scale + particleDataP->pos.z;
            }
        } else {
            for (j = 0; j < 4; j++, vertexP++) {
                vertexP->x = nullVec.x;
                vertexP->y = nullVec.y;
                vertexP->z = nullVec.x;
            }
        }

        animBankP = &anim->bank[particleDataP->animBank];
        animFrameP = &animBankP->frame[particleDataP->animNo];
        if ((particleP->attr & MB_PARTICLE_ATTR_UPAUSE) && !particleDataP->pauseF) {
            if (Hu3DPauseF == FALSE || (modelP->attr & HU3D_ATTR_NOPAUSE)) {
                for (j = 0; j < (s32)particleDataP->animSpeed * minimumVcount; j++) {
                    particleDataP->animTime += 1.0f;
                    if (particleDataP->animTime >= animFrameP->time) {
                        particleDataP->animNo++;
                        particleDataP->animTime -= animFrameP->time;
                        animFrameP = &animBankP->frame[particleDataP->animNo];
                        if (particleDataP->animNo >= animBankP->timeNum || animFrameP->time == -1) {
                            if (!(particleP->attr & MB_PARTICLE_ATTR_LOOP)) {
                                particleDataP->dispF = FALSE;
                                particleDataP->pauseF = TRUE;
                            }
                            particleDataP->animNo = 0;
                        }
                    }
                }
                particleDataP->animTime += particleDataP->animSpeed * minimumVcount - j;
                if (particleDataP->animTime >= animFrameP->time) {
                    particleDataP->animNo++;
                    particleDataP->animTime -= animFrameP->time;
                    animFrameP = &animBankP->frame[particleDataP->animNo];
                    if (particleDataP->animNo >= animBankP->timeNum || animFrameP->time == -1) {
                        if (!(particleP->attr & MB_PARTICLE_ATTR_LOOP)) {
                            particleDataP->dispF = FALSE;
                            particleDataP->pauseF = TRUE;
                        }
                        particleDataP->animNo = 0;
                    }
                }
            }
        }
        animFrameP = &animBankP->frame[particleDataP->animNo];
        animStP = &particleP->animSt[animFrameP->pat * 4];
        *stP++ = *animStP++;
        *stP++ = *animStP++;
        *stP++ = *animStP++;
        *stP++ = *animStP++;
    }

    DCFlushRangeNoSync(particleP->vertex, particleP->num * sizeof(HuVecF) * 4);
    if (particleP->colorF) {
        DCFlushRangeNoSync(particleP->color, particleP->num * sizeof(GXColor));
    } else {
        DCFlushRangeNoSync(particleP->data, particleP->num * sizeof(MBPARTICLEDATA));
    }
    DCFlushRange(particleP->st, particleP->num * sizeof(HuVec2f) * 4);

    GXSetNumTexGens(1);
    GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, particleP->alphaIn[0], particleP->alphaIn[1],
        particleP->alphaIn[2], particleP->alphaIn[3]);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_CLAMP, GX_AF_NONE);
    GXSetAlphaCompare(GX_GEQUAL, 1, GX_AOP_AND, GX_GEQUAL, 1);
    GXSetZCompLoc(FALSE);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_INDEX16);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetArray(GX_VA_POS, particleP->vertex, sizeof(HuVecF));
    GXSetVtxDesc(GX_VA_CLR0, GX_INDEX16);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    if (particleP->colorF && particleP->color) {
        GXSetArray(GX_VA_CLR0, particleP->color, sizeof(GXColor));
    } else {
        GXSetArray(GX_VA_CLR0, &particleP->data->color, sizeof(MBPARTICLEDATA));
    }
    GXSetVtxDesc(GX_VA_TEX0, GX_INDEX16);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    GXSetArray(GX_VA_TEX0, particleP->st, sizeof(HuVec2f));
    GXSetCullMode(GX_CULL_NONE);
    GXLoadPosMtxImm(*mtx, GX_PNMTX0);
    if (shadowModelDrawF) {
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ONE, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO);
        GXSetZMode(GX_FALSE, GX_LEQUAL, GX_FALSE);
    } else {
        GXSetTevColorIn(GX_TEVSTAGE0, particleP->colorIn[0], particleP->colorIn[1],
            particleP->colorIn[2], particleP->colorIn[3]);
        if (modelP->attr & HU3D_ATTR_ZWRITE_OFF) {
            GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
        } else {
            GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
        }
    }
    GXSetTevColor(GX_TEVREG1, particleP->tevColor[0]);
    GXSetTevColor(GX_TEVREG2, particleP->tevColor[1]);
    switch (particleP->blendMode) {
        case MB_PARTICLE_BLEND_NORMAL:
            GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
            break;
        case MB_PARTICLE_BLEND_ADDCOL:
            GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_ONE, GX_LO_NOOP);
            break;
        case MB_PARTICLE_BLEND_INVCOL:
            GXSetBlendMode(GX_BM_BLEND, GX_BL_ZERO, GX_BL_INVDSTCLR, GX_LO_NOOP);
            break;
    }
    HuSprTexLoad(particleP->anim, 0, GX_TEXMAP0, GX_CLAMP, GX_CLAMP, GX_LINEAR);
    GXCallDisplayList(particleP->dl, particleP->dlSize);
    totalPolyCnt += particleP->num;
    if (shadowModelDrawF == FALSE) {
        if (!(particleP->attr & MB_PARTICLE_ATTR_STOPCNT) && Hu3DPauseF == FALSE) {
            particleP->count++;
        }
        if (particleP->prevCount != 0 && particleP->prevCount <= particleP->count) {
            if (particleP->attr & MB_PARTICLE_ATTR_LOOP) {
                particleP->count = 0;
            } else {
                particleP->count = particleP->prevCount;
            }
        }
        particleP->prevCounter = GlobalCounter;
    }
}

static float baseScale[] = {
    1.0f, 0.9f, 0.7f, 0.5f, 0.5f, 0.7f, 0.9f, 1.0f
};

void mbParticleHookSet(HU3D_MODELID modelId, MBPARTICLEHOOK hook)
{
    HU3D_MODEL *modelP = &Hu3DData[modelId];
    MBPARTICLE *effP = modelP->hookData;
    effP->hook = hook;
}

void mbParticleAttrSet(HU3D_MODELID modelId, u8 attr)
{
    HU3D_MODEL *modelP = &Hu3DData[modelId];
    MBPARTICLE *effP = modelP->hookData;
    effP->attr |= attr;
}

void mbParticleAttrReset(HU3D_MODELID modelId, u8 attr)
{
    HU3D_MODEL *modelP = &Hu3DData[modelId];
    MBPARTICLE *effP = modelP->hookData;
    effP->attr &= ~attr;
}

MBPARTICLEDATA *mbParticleDataCreate(MBPARTICLE *effP)
{
    MBPARTICLEDATA *effData;
    int i;
    for(effData=effP->data, i=0; i<effP->num; i++, effData++) {
        if(effData->pauseF) {
            effData->pauseF = FALSE;
            effData->dispF = TRUE;
            effData->animTime = 0;
            return effData;
        }
    }
    return NULL;
}

int mbParticleUnkTotalGet(ANIMDATA *anim, int bankNo)
{
    int i;
    int total = 0;
    ANIMBANK *bank = &anim->bank[bankNo];

    for(i=0; i<bank->timeNum; i++) {
        ANIMFRAME *frame = &bank->frame[i];
        total += frame->time;
    }
    return total;
}

HU3D_MODELID mbParManCreate(ANIMDATA *anim, s16 maxCnt, HU3D_PARMAN_PARAM *param)
{
    HU3D_MODELID modelId;
    HU3D_MODEL *modelP;
    MBPARTICLE *particleP;
    MBPARMAN *parManParticleP;
    HU3D_PARMAN *parManP;
    MBPARTICLEDATA *particleDataP;
    s16 i;

    modelId = mbParticleCreate(anim, maxCnt);
    modelP = &Hu3DData[modelId];
    particleP = modelP->hookData;
    parManParticleP = mbMallocNum(sizeof(MBPARMAN), modelP->mallocNo);
    memcpy(parManParticleP, particleP, sizeof(MBPARTICLE));
    HuMemDirectFree(particleP);
    modelP->hookData = parManParticleP;
    mbParticleHookSet(modelId, ParManFunc);
    particleDataP = parManParticleP->particle.data;
    for (i = 0; i < parManParticleP->particle.num; i++, particleDataP++) {
        particleDataP->scale = 0.0f;
    }
    parManP = &parManParticleP->parMan;
    parManP->modelId = modelId;
    parManP->param = param;
    parManP->attr = HU3D_PARMAN_ATTR_NONE;
    parManP->pos.x = parManP->pos.y = parManP->pos.z = 0.0f;
    parManP->vec.x = 0.0f;
    parManP->vec.y = 1.0f;
    parManP->vec.z = 1.0f;
    parManP->vacuum.x = parManP->vacuum.y = parManP->vacuum.z = 0.0f;
    parManP->vacuumSpeed = 1.0f;
    parManP->accel = 0.0f;
    parManP->timeLimit = 0;
    return modelId;
}

void mbParManKill(HU3D_MODELID modelId)
{
    mbParticleKill(modelId);
}

void mbParticleBlendModeSet(HU3D_MODELID modelId, u8 blendMode)
{
    MBPARTICLE *particleP = Hu3DData[modelId].hookData;
    particleP->blendMode = blendMode;
}

void mbParManPosSet(HU3D_MODELID modelId, float x, float y, float z)
{
    Hu3DModelPosSet(modelId, x, y, z);
}

void mbParManVecSet(HU3D_MODELID modelId, float x, float y, float z)
{
    MBPARMAN *parManParticleP = Hu3DData[modelId].hookData;
    parManParticleP->parMan.vec.x = x;
    parManParticleP->parMan.vec.y = y;
    parManParticleP->parMan.vec.z = z;
}

void mbParManRotSet(HU3D_MODELID modelId, float rotX, float rotY, float rotZ)
{
    MBPARMAN *parManParticleP = Hu3DData[modelId].hookData;
    Mtx rotMtx;

    mbMtxRot(rotMtx, rotX, rotY, rotZ);
    parManParticleP->parMan.vec.x = rotMtx[0][2];
    parManParticleP->parMan.vec.y = rotMtx[1][2];
    parManParticleP->parMan.vec.z = rotMtx[2][2];
}

void mbParManAttrSet(HU3D_MODELID modelId, s32 attr)
{
    MBPARMAN *parManParticleP = Hu3DData[modelId].hookData;
    parManParticleP->parMan.attr |= attr;
}

void mbParManAttrReset(HU3D_MODELID modelId, s32 attr)
{
    MBPARMAN *parManParticleP = Hu3DData[modelId].hookData;
    parManParticleP->parMan.attr &= ~attr;
}

static void ParManFunc(HU3D_MODEL *modelP, MBPARTICLE *particleP, Mtx mtx)
{
    MBPARMAN *parManParticleP = (MBPARMAN *)particleP;
    HU3D_PARMAN *parManP = &parManParticleP->parMan;
    HU3D_PARMAN_PARAM *param;
    MBPARTICLEDATA *particleDataP;
    MBPARTICLEDATA *particleDataEnd;
    GXColor *colorStart;
    GXColor *colorEnd;
    Vec vecDir;
    Vec vel;
    Vec dir;
    Vec up;
    Vec vacuumAccel;
    Vec vacuumDist;
    float c;
    float s;
    float angleStart;
    float upRot;
    float accelVal;
    float rot;
    float weight;
    s16 colorIdx;
    s16 circleIdx;
    s16 i;

    param = parManP->param;
    if (Hu3DPauseF == FALSE || (modelP->attr & HU3D_ATTR_NOPAUSE)) {
        if (parManP->attr & HU3D_PARMAN_ATTR_RANDTIME90) {
            accelVal = param->accelRange * 0.9f
                + frandmod((u32)(param->accelRange * 0.1f * 1000.0f)) / 1000.0f;
        } else if (parManP->attr & HU3D_PARMAN_ATTR_RANDTIME70) {
            accelVal = param->accelRange * 0.7f
                + frandmod((u32)(param->accelRange * 0.3f * 1000.0f)) / 1000.0f;
        } else {
            accelVal = param->accelRange;
        }
        parManP->accel += accelVal;
        particleDataP = particleP->data;
        particleDataEnd = &particleP->data[particleP->num];
        circleIdx = 0;
        if (parManP->attr & HU3D_PARMAN_ATTR_RANDANGLE) {
            angleStart = frandmod((u32)((360.0f / param->accelRange) * 100.0f)) / 100.0f;
        }
        while (parManP->accel >= 1.0f) {
            if (parManP->attr & HU3D_PARMAN_ATTR_TIMEUP) {
                break;
            }
            while (particleDataP < particleDataEnd) {
                if (!particleDataP->activeF) {
                    particleDataP->activeF = TRUE;
                    s = param->scaleBase;
                    if (parManP->attr & HU3D_PARMAN_ATTR_RANDSCALE90) {
                        s = s * 0.9f + frandmod((u32)(s * 0.1f * 1000.0f)) / 1000.0f;
                    } else if (parManP->attr & HU3D_PARMAN_ATTR_RANDSCALE70) {
                        s = s * 0.7f + frandmod((u32)(s * 0.3f * 1000.0f)) / 1000.0f;
                    }
                    particleDataP->scaleBase = s;
                    particleDataP->scale = s;
                    vel.x = frandmod((u32)(param->scaleRange * 2.0f)) - param->scaleRange;
                    vel.y = frandmod((u32)(param->scaleRange * 2.0f)) - param->scaleRange;
                    vel.z = frandmod((u32)(param->scaleRange * 2.0f)) - param->scaleRange;
                    if (vel.x == 0.0f && vel.y == 0.0f && vel.z == 0.0f) {
                        vel.z = 1.0f;
                    }
                    PSVECNormalize(&vel, &vel);
                    PSVECScale(&vel, &particleDataP->pos, param->scaleRange);
                    PSVECNormalize(&parManP->vec, &vecDir);
                    if (parManP->attr & HU3D_PARMAN_ATTR_RANDANGLE) {
                        upRot = angleStart + (360.0f / param->accelRange) * circleIdx;
                        rot = param->angleRange;
                    } else {
                        upRot = frandmod(360);
                        if (param->angleRange != 0.0f) {
                            rot = frandmod((u32)param->angleRange);
                        } else {
                            rot = 0.0f;
                        }
                    }
                    if (vecDir.x * vecDir.x < 0.000001f && vecDir.z * vecDir.z < 0.000001f) {
                        up.x = 1.0f;
                        up.y = up.z = 0.0f;
                    } else {
                        if (vecDir.y * vecDir.y > 0.000001f) {
                            dir.x = vecDir.x;
                            dir.y = 0.0f;
                            dir.z = vecDir.z;
                        } else {
                            dir.x = vecDir.x;
                            dir.y = 1.0f;
                            dir.z = vecDir.z;
                        }
                        PSVECCrossProduct(&dir, &vecDir, &up);
                        if (up.x == 0.0f && up.y == 0.0f && up.z == 0.0f) {
                            up.z = 1.0f;
                        }
                    }
                    PSVECNormalize(&up, &up);
                    s = mbSinDeg(upRot);
                    c = mbCosDeg(upRot);
                    dir.x = up.x * (vecDir.x * vecDir.x + c * (1.0f - vecDir.x * vecDir.x))
                        + up.y * (vecDir.x * vecDir.y * (1.0f - c) - vecDir.z * s)
                        + up.z * (vecDir.x * vecDir.z * (1.0f - c) + vecDir.y * s);
                    dir.y = up.x * (vecDir.x * vecDir.y * (1.0f - c) + vecDir.z * s)
                        + up.y * (vecDir.y * vecDir.y + c * (1.0f - vecDir.y * vecDir.y))
                        + up.z * (vecDir.y * vecDir.z * (1.0f - c) - vecDir.x * s);
                    dir.z = up.x * (vecDir.x * vecDir.z * (1.0f - c) - vecDir.y * s)
                        + up.y * (vecDir.y * vecDir.z * (1.0f - c) + vecDir.x * s)
                        + up.z * (vecDir.z * vecDir.z + c * (1.0f - vecDir.z * vecDir.z));
                    PSVECCrossProduct(&dir, &vecDir, &up);
                    s = mbSinDeg(rot);
                    c = mbCosDeg(rot);
                    dir.x = vecDir.x * (up.x * up.x + c * (1.0f - up.x * up.x))
                        + vecDir.y * (up.x * up.y * (1.0f - c) - up.z * s)
                        + vecDir.z * (up.x * up.z * (1.0f - c) + up.y * s);
                    dir.y = vecDir.x * (up.x * up.y * (1.0f - c) + up.z * s)
                        + vecDir.y * (up.y * up.y + c * (1.0f - up.y * up.y))
                        + vecDir.z * (up.y * up.z * (1.0f - c) - up.x * s);
                    dir.z = vecDir.x * (up.x * up.z * (1.0f - c) - up.y * s)
                        + vecDir.y * (up.y * up.z * (1.0f - c) + up.x * s)
                        + vecDir.z * (up.z * up.z + c * (1.0f - up.z * up.z));
                    if (dir.x == 0.0f && dir.y == 0.0f && dir.z == 0.0f) {
                        dir.z = 1.0f;
                    }
                    PSVECNormalize(&dir, &dir);
                    s = param->speedBase;
                    if (parManP->attr & HU3D_PARMAN_ATTR_RANDSPEED90) {
                        s = s * 0.9f + frandmod((u32)(s * 0.1f * 1000.0f)) / 1000.0f;
                    } else if (parManP->attr & HU3D_PARMAN_ATTR_RANDSPEED70) {
                        s = s * 0.7f + frandmod((u32)(s * 0.3f * 1000.0f)) / 1000.0f;
                    } else if (parManP->attr & HU3D_PARMAN_ATTR_RANDSPEED100) {
                        s = frandmod((u32)(s * 1000.0f)) / 1000.0f;
                    }
                    PSVECScale(&dir, &particleDataP->vel, s);
                    particleDataP->accel = param->gravity;
                    particleDataP->speedDecay = param->speedDecay;
                    if (parManP->attr & HU3D_PARMAN_ATTR_SETCOLOR) {
                        particleDataP->colorIdx = colorIdx = parManP->color;
                    } else {
                        particleDataP->colorIdx = colorIdx = frandmod(param->colorNum);
                    }
                    particleDataP->color = param->colorStart[colorIdx];
                    particleDataP->time = 0;
                    break;
                }
                particleDataP++;
            }
            parManP->accel -= 1.0f;
            circleIdx++;
        }
        parManP->accel = 0.0f;
        if (parManP->timeLimit != 0) {
            parManP->timeLimit--;
            if (parManP->timeLimit == 0) {
                parManP->attr |= HU3D_PARMAN_ATTR_TIMEUP;
            }
        }
    }

    if (Hu3DPauseF == FALSE || (modelP->attr & HU3D_ATTR_NOPAUSE)) {
        particleDataP = particleP->data;
        for (i = 0; i < particleP->num; i++, particleDataP++) {
            if (particleDataP->activeF) {
                param = parManP->param;
                if (parManP->attr & HU3D_PARMAN_ATTR_SCALEJITTER) {
                    particleDataP->scale = particleDataP->scaleBase * baseScale[(parManP->jitterNo + i) & 7];
                } else {
                    particleDataP->scale = particleDataP->scaleBase;
                }
                if (!(parManP->attr & HU3D_PARMAN_ATTR_PAUSE)) {
                    PSVECAdd(&particleDataP->pos, &particleDataP->vel, &particleDataP->pos);
                    PSVECAdd(&particleDataP->pos, &particleDataP->accel, &particleDataP->pos);
                    PSVECScale(&particleDataP->vel, &particleDataP->vel, particleDataP->speedDecay);
                    PSVECAdd(&param->gravity, &particleDataP->accel, &particleDataP->accel);
                    if (parManP->attr & HU3D_PARMAN_ATTR_VACUUM) {
                        PSVECSubtract(&parManP->vacuum, &particleDataP->pos, &vacuumAccel);
                        if (vacuumAccel.x == 0.0f && vacuumAccel.y == 0.0f && vacuumAccel.z == 0.0f) {
                            vacuumAccel.z = 1.0f;
                        }
                        PSVECNormalize(&vacuumAccel, &vacuumAccel);
                        PSVECScale(&vacuumAccel, &vacuumAccel, parManP->vacuumSpeed);
                        PSVECAdd(&vacuumAccel, &particleDataP->accel, &particleDataP->accel);
                        PSVECAdd(&particleDataP->vel, &particleDataP->accel, &vacuumAccel);
                        PSVECSubtract(&parManP->vacuum, &particleDataP->pos, &vacuumDist);
                        if (PSVECSquareMag(&vacuumDist) <= PSVECSquareMag(&vacuumAccel)) {
                            particleDataP->activeF = FALSE;
                            particleDataP->scale = 0.0f;
                            continue;
                        }
                    }
                    particleDataP->scaleBase *= param->scaleDecay;
                    weight = (float)particleDataP->time / param->maxTime;
                    if (weight > 1.0f) {
                        weight = 1.0f;
                    }
                    OSf32tos16(&particleDataP->colorIdx, &colorIdx);
                    colorStart = &param->colorStart[colorIdx];
                    colorEnd = &param->colorEnd[colorIdx];
                    particleDataP->color.r = colorStart->r + weight * (colorEnd->r - colorStart->r);
                    particleDataP->color.g = colorStart->g + weight * (colorEnd->g - colorStart->g);
                    particleDataP->color.b = colorStart->b + weight * (colorEnd->b - colorStart->b);
                    particleDataP->color.a = colorStart->a + weight * (colorEnd->a - colorStart->a);
                    if (particleDataP->scale < 0.01f || particleDataP->time >= param->maxTime) {
                        particleDataP->activeF = FALSE;
                        particleDataP->scale = 0.0f;
                    }
                    particleDataP->time++;
                }
            }
        }
        parManP->jitterNo++;
    }
}
