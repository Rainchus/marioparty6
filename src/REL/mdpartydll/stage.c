/* Use the SDK math inline bodies without math.h's reconstructed weak-local
 * sqrtf constants; this object was compiled with literal pooling disabled. */
#define _MATH_H
#include "dolphin/math.h"
#pragma pool_data off

#include <string.h>

#include "dolphin/os.h"

#include "game/data.h"
#include "game/hu3d.h"
#include "game/memory.h"
#include "game/object.h"
#include "game/sprite.h"

#define sind(x) sin((M_PI * (x)) / 180.0)
#define cosd(x) cos((M_PI * (x)) / 180.0)

typedef struct MdpartyStageTextureWork {
    HuVecF unk_00;
    HuVecF unk_0C;
    GXColor color;
    float unk_1C;
    float unk_20;
    float unk_24;
} MDPARTY_STAGE_TEXTURE_WORK;

extern s32 frandmod(s32 modulus);
extern s32 rand8(void);

extern u32 lbl_1_data_F10[9];

extern OMOBJMAN *lbl_1_bss_A80;
extern OMOBJ *lbl_1_bss_A84;
extern ANIMDATA *lbl_1_bss_A88;
extern ANIMDATA *lbl_1_bss_A8C;
extern void *lbl_1_bss_A90;
extern s16 lbl_1_bss_A94;
extern HU3D_MODELID lbl_1_bss_A96[6][5];
extern HU3D_MODELID lbl_1_bss_AD2[2];
extern HU3D_MODELID lbl_1_bss_AD6[2];
extern HU3D_MODELID lbl_1_bss_ADA[4];
extern HU3D_MODELID lbl_1_bss_AE2;
extern HU3D_MODELID lbl_1_bss_AE4[4];
extern HU3D_MODELID lbl_1_bss_AEC;
extern MDPARTY_STAGE_TEXTURE_WORK lbl_1_bss_AF0[2];
extern ANIMDATA *lbl_1_bss_B40[9];

void fn_1_3F5EC(HU3D_DRAW_OBJ *drawObj, HSF_MATERIAL *material);
void fn_1_3F580(s16 layerNo);
void fn_1_3FF44(void);
void fn_1_3FC60(OMOBJ *obj);
void fn_1_400E0(s16 layerNo);
void fn_1_40BEC(HU3D_MODEL *model, HU3D_PARTICLE *particle, Mtx matrix);
void fn_1_410D4(HU3D_MODEL *model, HU3D_PARTICLE *particle, Mtx matrix);
void fn_1_4161C(void);
void fn_1_418D4(HU3D_MODEL *model, HU3D_PARTICLE *particle, Mtx matrix);
void fn_1_42258(HU3D_MODEL *model, HU3D_PARTICLE *particle, Mtx matrix);
void fn_1_42F34(HU3D_MODEL *model, HU3D_PARTICLE *particle, Mtx matrix);
void fn_1_43778(HU3D_MODEL *model, HU3D_PARTICLE *particle, Mtx matrix);
void fn_1_4459C(HU3D_MODEL *model, HU3D_PARTICLE *particle, Mtx matrix);

float fn_1_3F424(float arg0, float arg1, float time, float duration)
{
    if (time <= 0.0f) {
        return arg0;
    }
    if (time >= duration) {
        return arg1;
    }
    return arg0 + ((arg1 - arg0) * sind((90.0f / duration) * time));
}

float fn_1_3F50C(float arg0, float arg1, float arg2, float arg3)
{
    if (arg2 <= 0.0f) {
        return arg0;
    }
    if (arg2 >= arg3) {
        return arg1;
    }
    return arg0 + ((arg2 / arg3) * (arg1 - arg0));
}

float fn_1_3F550(float arg0, float arg1, float arg2)
{
    if (arg0 == arg1) {
        return arg1;
    }
    return (arg1 + (arg0 * (arg2 - 1.0f))) / arg2;
}

void fn_1_3F580(s16 layerNo)
{
    if (lbl_1_bss_A90) {
        GXSetTexCopySrc(0, 0, 640, 480);
        GXSetTexCopyDst(320, 240, GX_TF_RGB565, GX_TRUE);
        GXCopyTex(lbl_1_bss_A90, GX_FALSE);
    }
}

void fn_1_3F5EC(HU3D_DRAW_OBJ *drawObj, HSF_MATERIAL *material)
{
    HU3D_CAMERA *camera;
    MDPARTY_STAGE_TEXTURE_WORK *work;
    Mtx tmpMtx;
    Mtx scaleMtx;
    Mtx texMtx;
    Mtx lightMtx;
    Mtx invMtx;
    GXTexObj texObj;

    work = drawObj->model->hookData;
    camera = Hu3DCamera;

    GXInitTexObj(
        &texObj, lbl_1_bss_A90, 320, 240, GX_TF_RGB565, GX_CLAMP,
        GX_CLAMP, GX_FALSE);
    GXInitTexObjLOD(
        &texObj, GX_LINEAR, GX_LINEAR, 0.0f, 0.0f, 0.0f, GX_FALSE,
        GX_FALSE, GX_ANISO_1);
    GXLoadTexObj(&texObj, GX_TEXMAP1);
    HuSprTexLoad(lbl_1_bss_A88, 0, GX_TEXMAP2, GX_REPEAT, GX_REPEAT, GX_LINEAR);
    HuSprTexLoad(lbl_1_bss_A8C, 0, GX_TEXMAP3, GX_REPEAT, GX_REPEAT, GX_LINEAR);
    GXSetNumTexGens(3);
    GXSetNumTevStages(3);

    C_MTXLightPerspective(
        lightMtx, camera->fov, 1.2f, 0.5f, -0.5f, 0.5f, 0.5f);
    PSMTXInverse(Hu3DCameraMtx, invMtx);
    PSMTXConcat(invMtx, drawObj->matrix, tmpMtx);
    PSMTXConcat(lightMtx, Hu3DCameraMtx, texMtx);
    PSMTXConcat(texMtx, tmpMtx, texMtx);
    GXLoadTexMtxImm(texMtx, 0x21, GX_MTX3x4);
    GXSetTexCoordGen2(
        GX_TEXCOORD0, GX_TG_MTX3x4, GX_TG_POS, 0x21, GX_FALSE,
        GX_PTIDENTITY);

    PSMTXTrans(
        tmpMtx, 0.0f, work->unk_1C += work->unk_20, 0.0f);
    PSMTXScale(scaleMtx, 0.8f, 0.8f, 1.0f);
    PSMTXConcat(scaleMtx, tmpMtx, texMtx);
    GXLoadTexMtxImm(texMtx, GX_TEXMTX0, GX_MTX2x4);
    GXSetTexCoordGen2(
        GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE,
        GX_PTIDENTITY);
    GXSetTexCoordGen2(
        GX_TEXCOORD2, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE,
        GX_PTIDENTITY);

    GXSetTevColor(GX_TEVREG0, work->color);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP1, GX_COLOR0);
    GXSetTevColorIn(
        GX_TEVSTAGE0, GX_CC_TEXC, GX_CC_ZERO, GX_CC_ZERO, GX_CC_C0);
    GXSetTevColorOp(
        GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE,
        GX_TEVPREV);
    GXSetTevAlphaIn(
        GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_KONST);
    GXSetTevAlphaOp(
        GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE,
        GX_TEVPREV);
    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD2, GX_TEXMAP0, GX_COLOR0);
    GXSetTevColorIn(
        GX_TEVSTAGE1, GX_CC_CPREV, GX_CC_TEXC, GX_CC_C0, GX_CC_ZERO);
    GXSetTevColorOp(
        GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE,
        GX_TEVPREV);
    GXSetTevAlphaIn(
        GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_KONST);
    GXSetTevAlphaOp(
        GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE,
        GX_TEVPREV);
    GXSetTevOrder(GX_TEVSTAGE2, GX_TEXCOORD1, GX_TEXMAP3, GX_COLOR0);
    GXSetTevColorIn(
        GX_TEVSTAGE2, GX_CC_ZERO, GX_CC_TEXC, GX_CC_A0, GX_CC_CPREV);
    GXSetTevColorOp(
        GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE,
        GX_TEVPREV);
    GXSetTevAlphaIn(
        GX_TEVSTAGE2, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_KONST);
    GXSetTevAlphaOp(
        GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE,
        GX_TEVPREV);

    GXSetNumIndStages(1);
    GXSetIndTexOrder(GX_INDTEXSTAGE0, GX_TEXCOORD1, GX_TEXMAP2);
    GXSetIndTexCoordScale(GX_INDTEXSTAGE0, GX_ITS_1, GX_ITS_1);
    GXSetTevIndWarp(
        GX_TEVSTAGE0, GX_INDTEXSTAGE0, GX_TRUE, GX_FALSE, GX_ITM_0);
    GXSetIndTexMtx(GX_ITM_0, (float (*)[3])work, -1);
}

void fn_1_3FA6C(OMOBJ *obj)
{
    MDPARTY_STAGE_TEXTURE_WORK *work = lbl_1_bss_AF0;

    work->unk_00.x = work->unk_0C.x = 0.0f;
    work->unk_00.y = work->unk_0C.y = 0.0f;
    work->unk_00.z = work->unk_0C.z =
        (0.09f * work->unk_24) / 90.0f;
    work->unk_20 = (-0.02f * work->unk_24) / 90.0f;
    if ((work->unk_24 -= 1.0f) < 0.0f) {
        Hu3DModelAttrSet(obj->mdlId[0], HU3D_ATTR_DISPOFF);
        Hu3DLayerHookReset(1);
        obj->objFunc = NULL;
    }
}

void fn_1_3FB6C(s16 arg0, HuVecF *arg1)
{
    OMOBJ *obj = lbl_1_bss_A84;

    if (obj) {
        lbl_1_bss_AF0[arg0].unk_1C = 0.0f;
        lbl_1_bss_AF0[arg0].unk_24 = 90.0f;
        Hu3DModelPosSet(
            obj->mdlId[arg0], arg1->x, arg1->y, -840.0f);
        Hu3DModelAttrReset(obj->mdlId[arg0], HU3D_ATTR_DISPOFF);
        Hu3DLayerHookSet(1, fn_1_3F580);
        obj->objFunc = fn_1_3FA6C;
    }
}

inline void fn_1_3FB6C(s16 arg0, HuVecF *arg1);

void fn_1_3FC60(OMOBJ *obj)
{
    HU3D_MODEL *model = NULL;

    omSetStatBit(obj, 0x100);
    lbl_1_bss_A88 = HuSprAnimRead(
        HuDataSelHeapReadNum(0x00970080, HU_MEMNUM_OVL, HEAP_MODEL));
    lbl_1_bss_A8C = HuSprAnimRead(
        HuDataSelHeapReadNum(0x00970080, HU_MEMNUM_OVL, HEAP_MODEL));
    lbl_1_bss_A90 = HuMemDirectMallocNum(
        HEAP_MODEL,
        GXGetTexBufferSize(320, 240, GX_TF_RGB565, GX_FALSE, 0),
        HU_MEMNUM_OVL);
    obj->mdlId[0] = Hu3DModelCreate(
        HuDataSelHeapReadNum(0x0097007F, HU_MEMNUM_OVL, HEAP_MODEL));
    Hu3DModelPosSet(obj->mdlId[0], 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(obj->mdlId[0], 90.0f, 0.0f, 0.0f);
    Hu3DModelScaleSet(obj->mdlId[0], 6.0f, 6.0f, 6.0f);
    Hu3DModelLayerSet(obj->mdlId[0], 1);
    Hu3DModelAttrSet(obj->mdlId[0], HU3D_ATTR_DISPOFF);
    Hu3DModelMatHookSet(obj->mdlId[0], fn_1_3F5EC);
    memset(lbl_1_bss_AF0, 0, sizeof(lbl_1_bss_AF0[0]));
    model = &Hu3DData[obj->mdlId[0]];
    model->hookData = lbl_1_bss_AF0;
    obj->objFunc = NULL;
}

void fn_1_3FE44(OMOBJ *obj)
{
    HU3D_MODEL *model = NULL;

    Hu3DLayerHookReset(1);
    if (obj) {
        model = &Hu3DData[obj->mdlId[0]];
        model->hookData = lbl_1_bss_AF0;
        Hu3DModelKill(obj->mdlId[0]);
        if (lbl_1_bss_A90) {
            HuMemDirectFree(lbl_1_bss_A90);
        }
        lbl_1_bss_A90 = NULL;
    }
    obj = NULL;
}

inline void fn_1_3FE44(OMOBJ *obj);

void fn_1_3FEF4(void)
{
    lbl_1_bss_A84 =
        omAddObjEx(lbl_1_bss_A80, 0x1000, 1, 0, -1, fn_1_3FC60);
}

void fn_1_3FF44(void)
{
    if (lbl_1_bss_A84) {
        fn_1_3FE44(lbl_1_bss_A84);
    }
    lbl_1_bss_A84 = NULL;
}

inline void fn_1_3FF44(void);

void fn_1_40020(HuVecF *arg0)
{
    fn_1_3FB6C(0, arg0);
}

void fn_1_400E0(s16 layerNo)
{
    GXTexObj texObj;
    Mtx44 projection;
    Mtx model;
    float fade;
    s16 i;

    MTXOrtho(projection, 0.0f, 480.0f, 0.0f, 640.0f, 0.0f, 10.0f);
    GXSetProjection(projection, GX_ORTHOGRAPHIC);
    GXInitTexObj(
        &texObj, lbl_1_bss_A90, 320, 240, GX_TF_RGB565, GX_CLAMP,
        GX_CLAMP, GX_FALSE);
    GXInitTexObjLOD(
        &texObj, GX_LINEAR, GX_LINEAR, 0.0f, 0.0f, 0.0f, GX_FALSE,
        GX_FALSE, GX_ANISO_1);
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(
        GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY,
        GX_FALSE, GX_PTIDENTITY);
    GXSetNumChans(1);
    GXSetChanCtrl(
        GX_COLOR0A0, GX_FALSE, GX_SRC_VTX, GX_SRC_VTX, 0,
        GX_DF_CLAMP, GX_AF_NONE);
    GXSetNumTevStages(1);
    GXSetBlendMode(
        GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
    GXSetTevOrder(
        GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevColorIn(
        GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_TEXC, GX_CC_RASC,
        GX_CC_ZERO);
    GXSetTevColorOp(
        GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
        GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(
        GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_TEXA, GX_CA_RASA,
        GX_CA_ZERO);
    GXSetTevAlphaOp(
        GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
        GX_TRUE, GX_TEVPREV);

    PSMTXIdentity(model);
    GXLoadPosMtxImm(model, GX_PNMTX0);
    fade = fn_1_3F50C(0.0f, 1.0f, lbl_1_bss_A94, 90.0f);
    lbl_1_bss_A94++;
    if (lbl_1_bss_A94 > 90) {
        lbl_1_bss_A94 = 90;
    }

    GXBegin(GX_QUADS, GX_VTXFMT0, 16);
    for (i = 0; i < 4; i++) {
        GXPosition3f32(i * -1, i * -1, 0.0f);
        GXColor4u8(
            255.0f - (8.0f * fade), 255.0f - (8.0f * fade),
            255.0f - (8.0f * fade), 128.0f * fade);
        GXTexCoord2f32(0.0f, 0.0f);

        GXPosition3f32(640 + i, i * -1, 0.0f);
        GXColor4u8(
            255.0f - (8.0f * fade), 255.0f - (8.0f * fade),
            255.0f - (8.0f * fade), 128.0f * fade);
        GXTexCoord2f32(1.0f, 0.0f);

        GXPosition3f32(640 + i, 480 + i, 0.0f);
        GXColor4u8(
            255.0f - (8.0f * fade), 255.0f - (8.0f * fade),
            255.0f - (8.0f * fade), 128.0f * fade);
        GXTexCoord2f32(1.0f, 1.0f);

        GXPosition3f32(i * -1, 480 + i, 0.0f);
        GXColor4u8(
            255.0f - (8.0f * fade), 255.0f - (8.0f * fade),
            255.0f - (8.0f * fade), 128.0f * fade);
        GXTexCoord2f32(0.0f, 1.0f);
    }

    GXSetTexCopySrc(0, 0, 640, 480);
    GXSetTexCopyDst(320, 240, GX_TF_RGB565, GX_TRUE);
    GXCopyTex(lbl_1_bss_A90, GX_FALSE);
    Hu3DZClear();
}

void fn_1_40A9C(void)
{
    Hu3DLayerHookSet(14, fn_1_400E0);
}

void fn_1_40AC8(void)
{
    Hu3DLayerHookReset(14);
}

void fn_1_40AEC(
    s16 arg0, float arg1, float arg2, HuVecF *arg3, GXColor arg4)
{
    HU3D_MODEL *model;
    HU3D_PARTICLE *particle;
    HU3D_PARTICLE_DATA *data;

    model = &Hu3DData[lbl_1_bss_AEC];
    particle = model->hookData;
    data = &particle->data[arg0];
    data->time = 1;
    data->pos.x = arg3->x;
    data->pos.y = arg3->y;
    data->pos.z = arg3->z;
    data->scale = 0.0f;
    data->color.r = arg4.r;
    data->color.g = arg4.g;
    data->color.b = arg4.b;
    data->color.a = arg4.a;
    data->vel.x = 0.0f;
    data->vel.y = arg1;
    data->accel.x = arg2;
    data->accel.y = arg4.a;
    particle->dataCnt++;
    model->attr &= ~HU3D_ATTR_DISPOFF;
}

void fn_1_40BEC(HU3D_MODEL *model, HU3D_PARTICLE *particle, Mtx matrix)
{
    HU3D_PARTICLE_DATA *data;
    s16 i;

    if (particle->dataCnt <= 0) {
        model->attr |= HU3D_ATTR_DISPOFF;
        return;
    }
    for (i = 0, data = particle->data; i < particle->maxCnt; i++, data++) {
        if (data->time == 1) {
            data->scale = fn_1_3F424(
                0.0f, data->accel.x, data->vel.x, data->vel.y);
            data->color.a = fn_1_3F424(
                data->accel.y, 0.0f, data->vel.x, data->vel.y);
            if ((data->vel.x += 1.0f) > data->vel.y) {
                data->time = 0;
                data->scale = 0.0f;
                particle->dataCnt--;
            }
        }
    }
    DCFlushRangeNoSync(
        particle->data, particle->maxCnt * sizeof(HU3D_PARTICLE_DATA));
}

void fn_1_40EAC(void)
{
    lbl_1_bss_AEC = Hu3DParticleCreate(lbl_1_bss_B40[0], 8);
    Hu3DModelPosSet(lbl_1_bss_AEC, 0.0f, 0.0f, 0.0f);
    Hu3DModelScaleSet(lbl_1_bss_AEC, 1.0f, 1.0f, 1.0f);
    Hu3DModelLayerSet(lbl_1_bss_AEC, 7);
    Hu3DModelAttrSet(lbl_1_bss_AEC, HU3D_ATTR_DISPOFF);
    Hu3DParticleScaleSet(lbl_1_bss_AEC, 1.0f);
    Hu3DParticleHookSet(lbl_1_bss_AEC, fn_1_40BEC);
    Hu3DParticleBlendModeSet(
        lbl_1_bss_AEC, HU3D_PARTICLE_BLEND_ADDCOL);
}

void fn_1_40FC0(void)
{
    Hu3DModelKill(lbl_1_bss_AEC);
}

void fn_1_40FEC(s16 arg0, float arg1, HuVecF *arg2, GXColor arg3)
{
    HU3D_MODEL *model;
    HU3D_PARTICLE *particle;
    HU3D_PARTICLE_DATA *data;

    model = &Hu3DData[lbl_1_bss_AE4[arg0]];
    particle = model->hookData;
    Hu3DModelPosSetV(lbl_1_bss_AE4[arg0], arg2);
    data = particle->data;
    data->time = 1;
    data->color.r = arg3.r;
    data->color.g = arg3.g;
    data->color.b = arg3.b;
    data->color.a = arg3.a;
    data->time = 0;
    data->parManId = arg1;
    particle->dataCnt = 1;
    model->attr &= ~HU3D_ATTR_DISPOFF;
}

void fn_1_410D4(HU3D_MODEL *model, HU3D_PARTICLE *particle, Mtx matrix)
{
    HU3D_PARTICLE_DATA *first;
    HU3D_PARTICLE_DATA *data;
    s16 i;

    if (particle->dataCnt <= 0) {
        model->attr |= HU3D_ATTR_DISPOFF;
        return;
    }
    if (particle->dataCnt == 1) {
        first = particle->data;
        for (i = 0, data = particle->data; i < particle->maxCnt; i++, data++) {
            data->attr |= HU3D_PARTICLE_ATTR_SCALEY;
            data->zRot = MTXDegToRad(frandmod(360));
            data->vel.z = frandmod(25) + 50;
            data->vel.x =
                -sind(MTXRadToDeg(data->zRot)) * data->vel.z;
            data->vel.y =
                cosd(MTXRadToDeg(data->zRot)) * data->vel.z;
            data->scale = 0.1f * data->vel.z;
            data->scaleY = 3.0f * data->vel.z;
            data->color.r = first->color.r;
            data->color.g = first->color.g;
            data->color.b = first->color.b;
            data->color.a = first->color.a;
            data->pos.x = data->vel.x;
            data->pos.y = data->vel.y;
            data->pos.z = 0.0f;
        }
        particle->dataCnt = 2;
    } else if (particle->dataCnt == 2) {
        first = particle->data;
        for (i = 0, data = particle->data; i < particle->maxCnt; i++, data++) {
            if ((i % 2) == 0) {
                data->zRot += i / 500.0f;
            } else {
                data->zRot -= i / 500.0f;
            }
            data->vel.x =
                -sind(MTXRadToDeg(data->zRot)) * data->vel.z;
            data->vel.y =
                cosd(MTXRadToDeg(data->zRot)) * data->vel.z;
            data->pos.x = data->vel.x;
            data->pos.y = data->vel.y;
            data->pos.z = 0.0f;
            data->color.a = fn_1_3F424(
                255.0f, 0.0f, first->time, first->parManId);
        }
        if ((first->time += 1.0f) > first->parManId) {
            particle->dataCnt = 0;
        }
    }
    DCFlushRangeNoSync(
        particle->data, particle->maxCnt * sizeof(HU3D_PARTICLE_DATA));
}

void fn_1_4161C(void)
{
    s16 i;

    for (i = 0; i < 4; i++) {
        lbl_1_bss_AE4[i] = Hu3DParticleCreate(lbl_1_bss_B40[1], 16);
        Hu3DModelPosSet(
            lbl_1_bss_AE4[i], 200.0f * i, 200.0f, 0.0f);
        Hu3DModelScaleSet(lbl_1_bss_AE4[i], 1.0f, 1.0f, 1.0f);
        Hu3DModelLayerSet(lbl_1_bss_AE4[i], 7);
        Hu3DModelAttrSet(lbl_1_bss_AE4[i], HU3D_ATTR_DISPOFF);
        Hu3DParticleScaleSet(lbl_1_bss_AE4[i], 1.0f);
        Hu3DParticleHookSet(lbl_1_bss_AE4[i], fn_1_410D4);
        Hu3DParticleBlendModeSet(
            lbl_1_bss_AE4[i], HU3D_PARTICLE_BLEND_ADDCOL);
    }
}

inline void fn_1_4161C(void);

void fn_1_417DC(void)
{
    s16 i;

    for (i = 0; i < 4; i++) {
        Hu3DModelKill(lbl_1_bss_AE4[i]);
    }
}

void fn_1_41834(void)
{
    s16 i;
    HU3D_MODEL *model;
    HU3D_PARTICLE *particle;
    HU3D_PARTICLE_DATA *data;

    model = &Hu3DData[lbl_1_bss_AE2];
    particle = model->hookData;
    for (i = 0, data = particle->data; i < particle->maxCnt; i++, data++) {
        data->time = 0;
        data->scale = 0.0f;
    }
    particle->dataCnt = 1;
    model->attr &= ~HU3D_ATTR_DISPOFF;
}

void fn_1_418D4(HU3D_MODEL *model, HU3D_PARTICLE *particle, Mtx matrix)
{
    HU3D_PARTICLE_DATA *data;
    s16 color;
    s16 i;
    s16 spawnCount = 0;
    float radius = 125.0f;

    if (particle->dataCnt == 1) {
        particle->emitCnt -= 2;
        if (particle->emitCnt < 0) {
            particle->emitCnt += 360.0f;
        }
        if (particle->count > 100) {
            radius -= particle->count - 100;
            if (radius < 1.0f) {
                radius = 1.0f;
            }
        }
        for (i = 0, data = particle->data; i < particle->maxCnt; i++, data++) {
            if (data->time == 0) {
                if (spawnCount < 7) {
                    color = rand8() + 128;
                    color &= 0xFF;
                    data->color.r = 255;
                    color = rand8() + 128;
                    color &= 0xFF;
                    data->color.g = 255;
                    color = rand8() % 204;
                    color &= 0xFF;
                    data->color.b = color;
                    if ((i % 4) == 0) {
                        data->color.b = 255;
                    }
                    data->color.a = 255;
                    data->scale = 80.0f;
                    data->vel.x = (45.0f * (i % 8)) + particle->emitCnt;
                    data->vel.z = 750.0f - (particle->count * 2);
                    if (data->vel.z < 1.0f) {
                        data->vel.z = 1.0f;
                    }
                    data->accel.x = 0.0f;
                    data->accel.y = frandmod(30) + 90;
                    data->pos.x = -sind(data->vel.x) * data->vel.z;
                    data->pos.y = cosd(data->vel.x) * data->vel.z;
                    data->pos.z = 800.0f;
                    data->time = 1;
                    spawnCount++;
                }
            } else if (data->time == 1) {
                data->pos.x = -sind(data->vel.x) * data->vel.z;
                data->pos.y = cosd(data->vel.x) * data->vel.z;
                data->pos.z = fn_1_3F50C(
                    800.0f, -2000.0f, data->accel.x, data->accel.y);
                data->zRot += 0.1f;
                data->scale = fn_1_3F50C(
                    150.0f, 50.0f, data->accel.x, data->accel.y);
                data->vel.x += 2.0f;
                data->vel.z = fn_1_3F50C(
                    500.0f, radius, data->accel.x, data->accel.y);
                if (data->accel.x > data->accel.y - 10.0f) {
                    data->color.a = fn_1_3F50C(
                        255.0f, 0.0f,
                        data->accel.x - (data->accel.y - 10.0f), 10.0f);
                }
                if ((data->accel.x += 1.0f) > data->accel.y) {
                    data->time = 0;
                    data->scale = 0.0f;
                }
            }
        }
    }
    DCFlushRangeNoSync(
        particle->data, particle->maxCnt * sizeof(HU3D_PARTICLE_DATA));
}

void fn_1_42060(void)
{
    lbl_1_bss_AE2 = Hu3DParticleCreate(lbl_1_bss_B40[2], 1000);
    Hu3DModelPosSet(lbl_1_bss_AE2, 0.0f, 275.0f, 0.0f);
    Hu3DModelScaleSet(lbl_1_bss_AE2, 1.0f, 1.0f, 1.0f);
    Hu3DModelLayerSet(lbl_1_bss_AE2, 7);
    Hu3DModelAttrSet(lbl_1_bss_AE2, HU3D_ATTR_DISPOFF);
    Hu3DParticleScaleSet(lbl_1_bss_AE2, 1.0f);
    Hu3DParticleHookSet(lbl_1_bss_AE2, fn_1_418D4);
}

void fn_1_42160(void)
{
    Hu3DModelKill(lbl_1_bss_AE2);
}

void fn_1_4218C(s16 arg0, float arg1, HuVecF *arg2)
{
    HU3D_MODEL *model;
    HU3D_PARTICLE *particle;
    HU3D_PARTICLE_DATA *data;

    model = &Hu3DData[lbl_1_bss_ADA[arg0]];
    particle = model->hookData;
    Hu3DModelPosSetV(lbl_1_bss_ADA[arg0], arg2);
    data = particle->data;
    data->time = 0;
    data->parManId = arg1;
    particle->dataCnt = 1;
    model->attr &= ~HU3D_ATTR_DISPOFF;
}

void fn_1_42258(HU3D_MODEL *model, HU3D_PARTICLE *particle, Mtx matrix)
{
    HU3D_PARTICLE_DATA *first;
    HU3D_PARTICLE_DATA *data;
    HuVecF random;
    HuVecF direction;
    float speed;
    s16 i;
    s16 color;

    if (particle->dataCnt == 1) {
        for (i = 0, data = particle->data; i < particle->maxCnt; i++, data++) {
            random.x = frandmod(100) - 50;
            random.y = frandmod(100) - 50;
            random.z = frandmod(100) - 50;
            PSVECNormalize(&random, &direction);
            speed = 200.0f;
            data->vel.x = direction.x * speed;
            data->vel.y = direction.y * speed;
            data->vel.z = direction.z * speed;
            color = rand8() + 128;
            color &= 0xFF;
            data->color.r = color;
            color = rand8() + 128;
            color &= 0xFF;
            data->color.g = color;
            color = rand8() % 128;
            color &= 0xFF;
            data->color.b = color;
            data->color.a = 204;
            data->pos.x = data->accel.x = frandmod(4) - 2;
            data->pos.y = data->accel.y = frandmod(4) - 2;
            data->pos.z = data->accel.z = frandmod(4) - 2;
            data->scale = 0.0f;
        }
        particle->dataCnt = 2;
    } else if (particle->dataCnt == 2) {
        first = particle->data;
        for (i = 0, data = particle->data; i < particle->maxCnt; i++, data++) {
            data->scale = fn_1_3F424(
                0.0f, 125.0f, first->time, 5.0f);
        }
        if ((first->time += 1.0f) > 15) {
            first->time = 0;
            particle->dataCnt = 3;
        }
    } else if (particle->dataCnt == 3) {
        first = particle->data;
        for (i = 0, data = particle->data; i < particle->maxCnt; i++, data++) {
            data->pos.x = fn_1_3F424(
                data->accel.x, data->vel.x, first->time,
                first->parManId);
            data->pos.y = fn_1_3F424(
                data->accel.y, data->vel.y, first->time,
                first->parManId);
            data->pos.z = fn_1_3F424(
                data->accel.z, data->vel.z, first->time,
                first->parManId);
            data->scale = fn_1_3F50C(
                50.0f, 0.0f, first->time, first->parManId);
        }
        if ((first->time += 1.0f) > first->parManId) {
            particle->dataCnt = 0;
            model->attr |= HU3D_ATTR_DISPOFF;
        }
    }
}

void fn_1_42B40(void)
{
    s16 i;

    for (i = 0; i < 4; i++) {
        lbl_1_bss_ADA[i] = Hu3DParticleCreate(lbl_1_bss_B40[2], 64);
        Hu3DModelPosSet(lbl_1_bss_ADA[i], 0.0f, 0.0f, 0.0f);
        Hu3DModelScaleSet(lbl_1_bss_ADA[i], 1.0f, 1.0f, 1.0f);
        Hu3DModelLayerSet(lbl_1_bss_ADA[i], 7);
        Hu3DModelAttrSet(lbl_1_bss_ADA[i], HU3D_ATTR_DISPOFF);
        Hu3DParticleScaleSet(lbl_1_bss_ADA[i], 1.0f);
        Hu3DParticleHookSet(lbl_1_bss_ADA[i], fn_1_42258);
        Hu3DParticleBlendModeSet(
            lbl_1_bss_ADA[i], HU3D_PARTICLE_BLEND_ADDCOL);
    }
}

void fn_1_42CD4(void)
{
    s16 i;

    for (i = 0; i < 4; i++) {
        Hu3DModelKill(lbl_1_bss_ADA[i]);
    }
}

void fn_1_42D2C(s16 arg0, s16 arg1)
{
    if (arg1) {
        Hu3DModelAttrReset(lbl_1_bss_AD6[arg0], HU3D_ATTR_DISPOFF);
    } else {
        Hu3DModelAttrSet(lbl_1_bss_AD6[arg0], HU3D_ATTR_DISPOFF);
    }
}

void fn_1_42DA8(s16 arg0, HuVecF *arg1, GXColor *arg2)
{
    s16 i;
    HU3D_MODEL *model;
    HU3D_PARTICLE *particle;
    HU3D_PARTICLE_DATA *data;

    model = &Hu3DData[lbl_1_bss_AD6[arg0]];
    particle = model->hookData;
    for (i = 0, data = particle->data; i < particle->maxCnt; i++, data++) {
        data->time = 1;
        if (arg2 != NULL) {
            data->color.r = arg2->r;
            data->color.g = arg2->g;
            data->color.b = arg2->b;
        }
    }
    if (arg1 != NULL) {
        Hu3DModelPosSetV(lbl_1_bss_AD6[arg0], arg1);
    }
    Hu3DModelAttrReset(lbl_1_bss_AD6[arg0], HU3D_ATTR_DISPOFF);
}

void fn_1_42EAC(s16 arg0)
{
    s16 i;
    HU3D_MODEL *model;
    HU3D_PARTICLE *particle;
    HU3D_PARTICLE_DATA *data;

    model = &Hu3DData[lbl_1_bss_AD6[arg0]];
    particle = model->hookData;
    for (i = 0, data = particle->data; i < particle->maxCnt; i++, data++) {
        data->time = 2;
    }
}

void fn_1_42F34(HU3D_MODEL *model, HU3D_PARTICLE *particle, Mtx matrix)
{
    u8 dataTbl[5][2] = {
        { 0x10, 0xFF },
        { 0x10, 0xFF },
        { 0x30, 0x80 },
        { 0x80, 0x40 },
        { 0xFF, 0x20 },
    };
    HU3D_PARTICLE_DATA *data;
    float velX1;
    float velY1;
    float velX2;
    float velY2;
    s16 i;

    if (particle->count == 0) {
        for (i = 0, data = particle->data; i < particle->maxCnt; i++, data++) {
            data->time = 0;
        }
    }
    for (i = 0, data = particle->data; i < particle->maxCnt; i++, data++) {
        if (data->time == 1) {
            data->pos.x = 0.0f;
            data->pos.y = 0.0f;
            data->pos.z = 0.0f;
            data->vel.x = dataTbl[i % 5][1];
            data->vel.y = dataTbl[i % 5][0];
            velX1 = data->vel.x;
            data->color.a =
                fn_1_3F550(data->color.a, velX1, 10.0f);
            velY1 = data->vel.y;
            data->scale = fn_1_3F550(data->scale, velY1, 10.0f);
        } else if (data->time == 2) {
            data->pos.x = 0.0f;
            data->pos.y = 0.0f;
            data->pos.z = 0.0f;
            data->vel.x = 0.0f;
            data->vel.y = 0.0f;
            velX2 = data->vel.x;
            data->color.a = fn_1_3F550(data->color.a, velX2, 10.0f);
            velY2 = data->vel.y;
            data->scale = fn_1_3F550(data->scale, velY2, 10.0f);
        } else {
            data->color.a = 0;
            data->scale = 0.0f;
        }
    }
    DCFlushRangeNoSync(
        particle->data, particle->maxCnt * sizeof(HU3D_PARTICLE_DATA));
}

void fn_1_433AC(void)
{
    s16 i;

    for (i = 0; i < 2; i++) {
        lbl_1_bss_AD6[i] = Hu3DParticleCreate(lbl_1_bss_B40[0], 10);
        Hu3DModelPosSet(lbl_1_bss_AD6[i], 0.0f, 0.0f, 0.0f);
        Hu3DModelScaleSet(lbl_1_bss_AD6[i], 1.0f, 1.0f, 1.0f);
        Hu3DModelAttrSet(lbl_1_bss_AD6[i], HU3D_ATTR_DISPOFF);
        Hu3DModelLayerSet(lbl_1_bss_AD6[i], 2);
        Hu3DParticleHookSet(lbl_1_bss_AD6[i], fn_1_42F34);
        Hu3DParticleBlendModeSet(
            lbl_1_bss_AD6[i], HU3D_PARTICLE_BLEND_ADDCOL);
    }
}

void fn_1_43518(void)
{
    s16 i;

    for (i = 0; i < 2; i++) {
        Hu3DModelKill(lbl_1_bss_AD6[i]);
    }
}

void fn_1_43570(s16 arg0, s16 arg1)
{
    if (arg1) {
        Hu3DModelAttrReset(lbl_1_bss_AD2[arg0], HU3D_ATTR_DISPOFF);
    } else {
        Hu3DModelAttrSet(lbl_1_bss_AD2[arg0], HU3D_ATTR_DISPOFF);
    }
}

void fn_1_435EC(s16 arg0, HuVecF *arg1, GXColor *arg2)
{
    HU3D_MODEL *model;
    HU3D_PARTICLE *particle;

    model = &Hu3DData[lbl_1_bss_AD2[arg0]];
    particle = model->hookData;
    particle->dataCnt = 1;
    if (arg2 != NULL) {
        particle->pos.x = arg2->r;
        particle->pos.y = arg2->g;
        particle->pos.z = arg2->b;
    }
    if (arg1 != NULL) {
        particle->unk_10.x = arg1->x;
        particle->unk_10.y = arg1->y;
        particle->unk_10.z = arg1->z;
    }
    Hu3DModelAttrReset(lbl_1_bss_AD2[arg0], HU3D_ATTR_DISPOFF);
}

void fn_1_43724(s16 arg0)
{
    HU3D_MODEL *model;
    HU3D_PARTICLE *particle;

    model = &Hu3DData[lbl_1_bss_AD2[arg0]];
    particle = model->hookData;
    particle->dataCnt = 0;
}

void fn_1_43778(HU3D_MODEL *model, HU3D_PARTICLE *particle, Mtx matrix)
{
    HU3D_PARTICLE_DATA *data;
    s16 i;
    s16 spawnCount = 0;
    float color;
    float random;

    if (particle->count == 0) {
        for (i = 0, data = particle->data; i < particle->maxCnt; i++, data++) {
            data->time = 0;
        }
        particle->dataCnt = 1;
        particle->pos.x = 255.0f;
        particle->pos.y = 255.0f;
        particle->pos.z = 255.0f;
    }

    for (i = 0, data = particle->data; i < particle->maxCnt; i++, data++) {
        if (data->time == 0 && particle->dataCnt == 1 && spawnCount <= 3) {
            spawnCount++;
            data->time = 1;
            data->vel.x = 0.0f;
            data->vel.y = frandmod(30) + 60;
            data->accel.x = frandmod(100) - 50;
            data->accel.y = -frandmod(100) - 50;
            data->accel.z = frandmod(100) - 50;
            PSVECNormalize(&data->accel, &data->accel);
            data->accel.x *= 2.0f;
            data->accel.y *= 5.0f;
            data->accel.z *= 2.0f;
            data->pos.x = particle->unk_10.x + frandmod(100) - 50;
            data->pos.y = particle->unk_10.y + frandmod(100) - 50;
            data->pos.z = particle->unk_10.z + frandmod(100) - 50;
            random = frandmod(128);
            color = particle->pos.x + random;
            if (color > 255.0f) {
                color = 255.0f;
            }
            data->color.r = color;
            color = particle->pos.y + random;
            if (color > 255.0f) {
                color = 255.0f;
            }
            data->color.g = color;
            color = particle->pos.z + random;
            if (color > 255.0f) {
                color = 255.0f;
            }
            data->color.b = color;
            data->color.a = 0;
        } else if (data->time == 1) {
            data->pos.x += data->accel.x;
            data->pos.y += data->accel.y;
            data->pos.z += data->accel.z;
            if (rand8() % 5 == 0) {
                data->zRot = MTXDegToRad(frandmod(360));
                data->color.a = frandmod(127) + 128;
            }
            random = fn_1_3F50C(1.0f, 0.0f, data->vel.x, data->vel.y);
            data->scale = frandmod(90) * random;
            if ((data->vel.x += 1.0f) > data->vel.y) {
                data->time = 0;
                data->scale = 0.0f;
                data->color.a = 0;
            }
        }
    }
    DCFlushRangeNoSync(
        particle->data, particle->maxCnt * sizeof(HU3D_PARTICLE_DATA));
}

void fn_1_43D78(void)
{
    s16 i;

    for (i = 0; i < 2; i++) {
        lbl_1_bss_AD2[i] = Hu3DParticleCreate(lbl_1_bss_B40[3], 256);
        Hu3DModelPosSet(lbl_1_bss_AD2[i], 0.0f, 0.0f, 0.0f);
        Hu3DModelScaleSet(lbl_1_bss_AD2[i], 1.0f, 1.0f, 1.0f);
        Hu3DModelAttrSet(lbl_1_bss_AD2[i], HU3D_ATTR_DISPOFF);
        Hu3DModelLayerSet(lbl_1_bss_AD2[i], 2);
        Hu3DParticleHookSet(lbl_1_bss_AD2[i], fn_1_43778);
        Hu3DParticleBlendModeSet(
            lbl_1_bss_AD2[i], HU3D_PARTICLE_BLEND_ADDCOL);
    }
}

void fn_1_43EE4(void)
{
    s16 i;

    for (i = 0; i < 2; i++) {
        Hu3DModelKill(lbl_1_bss_AD2[i]);
    }
}

void fn_1_43F3C(s16 groupNo, HuVecF *pos, s16 mode)
{
    GXColor colors[2] = {
        { 255, 114, 46, 0 },
        { 109, 207, 246, 0 },
    };

    if (mode == 1) {
        fn_1_42DA8(groupNo, pos, &colors[groupNo]);
        fn_1_435EC(groupNo, pos, &colors[groupNo]);
    } else if (mode == 0) {
        fn_1_42EAC(groupNo);
        fn_1_43724(groupNo);
    } else if (mode == 2) {
        fn_1_42D2C(groupNo, FALSE);
        fn_1_43570(groupNo, FALSE);
    }
}

void fn_1_44300(s16 groupNo, s16 show)
{
    s16 i;

    for (i = 0; i < 5; i++) {
        if (show) {
            Hu3DModelAttrReset(lbl_1_bss_A96[groupNo][i], HU3D_ATTR_DISPOFF);
        } else {
            Hu3DModelAttrSet(lbl_1_bss_A96[groupNo][i], HU3D_ATTR_DISPOFF);
        }
    }
}

void fn_1_443B4(s16 groupNo, HuVecF *pos, GXColor *color)
{
    s16 i;
    HU3D_MODEL *model;
    HU3D_PARTICLE *particle;

    for (i = 0; i < 5; i++) {
        model = &Hu3DData[lbl_1_bss_A96[groupNo][i]];
        particle = model->hookData;
        particle->dataCnt = 1;
        if (color != NULL) {
            particle->pos.x = color->r;
            particle->pos.y = color->g;
            particle->pos.z = color->b;
        }
        if (pos != NULL) {
            particle->unk_10.x = pos->x;
            particle->unk_10.y = pos->y;
            particle->unk_10.z = pos->z;
        }
        Hu3DModelAttrReset(
            lbl_1_bss_A96[groupNo][i], HU3D_ATTR_DISPOFF);
    }
}

inline void fn_1_443B4(s16 groupNo, HuVecF *pos, GXColor *color);

void fn_1_4451C(s16 groupNo)
{
    s16 i;
    HU3D_MODEL *model;
    s16 *work;

    for (i = 0; i < 5; i++) {
        model = &Hu3DData[lbl_1_bss_A96[groupNo][i]];
        work = model->hookData;
        *work = 0;
    }
}

void fn_1_4459C(HU3D_MODEL *model, HU3D_PARTICLE *particle, Mtx matrix)
{
    HU3D_PARTICLE_DATA *data;
    s16 i;
    s16 spawnCount = 0;
    float color;
    float random;

    if (particle->count == 0) {
        for (i = 0, data = particle->data; i < particle->maxCnt; i++, data++) {
            data->time = 0;
        }
        particle->dataCnt = 1;
        particle->pos.x = 255.0f;
        particle->pos.y = 255.0f;
        particle->pos.z = 255.0f;
    }

    for (i = 0, data = particle->data; i < particle->maxCnt; i++, data++) {
        if (data->time == 0 && particle->dataCnt == 1 && spawnCount < 1) {
            spawnCount++;
            data->time = 1;
            data->vel.x = 0.0f;
            data->vel.y = frandmod(30) + 30;
            data->accel.x = frandmod(100) - 50;
            data->accel.y = -frandmod(100) - 50;
            data->accel.z = frandmod(100) - 50;
            PSVECNormalize(&data->accel, &data->accel);
            data->accel.x *= 2.0f;
            data->accel.y *= 2.0f;
            data->accel.z *= 2.0f;
            data->pos.x = particle->unk_10.x + frandmod(100) - 50;
            data->pos.y = particle->unk_10.y;
            data->pos.z = particle->unk_10.z + frandmod(100) - 50;
            random = frandmod(32);
            color = particle->pos.x + random;
            if (color > 255.0f) {
                color = 255.0f;
            }
            data->color.r = color;
            color = particle->pos.y + random;
            if (color > 255.0f) {
                color = 255.0f;
            }
            data->color.g = color;
            color = particle->pos.z + random;
            if (color > 255.0f) {
                color = 255.0f;
            }
            data->color.b = color;
            data->color.a = 0;
        } else if (data->time == 1) {
            data->pos.y += data->accel.y;
            if (rand8() % 5 == 0) {
                data->zRot = MTXDegToRad(frandmod(360));
                data->color.a = frandmod(127) + 128;
            }
            random = fn_1_3F50C(1.0f, 0.0f, data->vel.x, data->vel.y);
            data->scale = 40.0f * random;
            if (++data->vel.x > data->vel.y) {
                data->time = 0;
                data->scale = 0.0f;
                data->color.a = 0;
            }
        }
    }
    DCFlushRangeNoSync(
        particle->data, particle->maxCnt * sizeof(HU3D_PARTICLE_DATA));
}

void fn_1_44B1C(void)
{
    s16 particleCount[5] = { 10, 10, 10, 10, 256 };
    s16 groupNo;
    s16 particleNo;

    for (groupNo = 0; groupNo < 6; groupNo++) {
        for (particleNo = 0; particleNo < 5; particleNo++) {
            lbl_1_bss_A96[groupNo][particleNo] = Hu3DParticleCreate(
                lbl_1_bss_B40[particleNo + 4], particleCount[particleNo]);
            Hu3DModelPosSet(
                lbl_1_bss_A96[groupNo][particleNo], 0.0f, 0.0f, 0.0f);
            Hu3DModelScaleSet(
                lbl_1_bss_A96[groupNo][particleNo], 1.0f, 1.0f, 1.0f);
            Hu3DModelAttrSet(
                lbl_1_bss_A96[groupNo][particleNo], HU3D_ATTR_DISPOFF);
            Hu3DModelLayerSet(lbl_1_bss_A96[groupNo][particleNo], 2);
            Hu3DParticleHookSet(
                lbl_1_bss_A96[groupNo][particleNo], fn_1_4459C);
            Hu3DParticleBlendModeSet(
                lbl_1_bss_A96[groupNo][particleNo],
                HU3D_PARTICLE_BLEND_ADDCOL);
        }
    }
}

inline void fn_1_44B1C(void);

void fn_1_44D48(void)
{
    s16 i;
    s16 j;

    for (i = 0; i < 6; i++) {
        for (j = 0; j < 5; j++) {
            Hu3DModelKill(lbl_1_bss_A96[i][j]);
        }
    }
}

void fn_1_44DCC(OMOBJMAN *objman)
{
    s16 i;

    lbl_1_bss_A80 = objman;
    fn_1_3FEF4();
    for (i = 0; i < 9; i++) {
        lbl_1_bss_B40[i] = HuSprAnimRead(
            HuDataSelHeapReadNum(
                lbl_1_data_F10[i], HU_MEMNUM_OVL, HEAP_MODEL));
    }
    fn_1_40EAC();
    fn_1_4161C();
    fn_1_42060();
    fn_1_42B40();
    fn_1_433AC();
    fn_1_43D78();
    fn_1_44B1C();
}

void fn_1_4581C(void)
{
    fn_1_40FC0();
    fn_1_417DC();
    fn_1_42160();
    fn_1_42CD4();
    fn_1_43518();
    fn_1_43EE4();
    fn_1_44D48();
    fn_1_40AC8();
    fn_1_3FF44();
}

void fn_1_45A48(HuVecF *arg0)
{
    fn_1_3FB6C(0, arg0);
    fn_1_40AEC(
        5, 120.0f, 3000.0f, arg0, (GXColor) { 128, 128, 255, 128 });
}

void fn_1_45C40(s16 arg0, HuVecF *arg1, s16 arg2)
{
    GXColor colors[5] = {
        { 254, 77, 75, 255 },
        { 50, 127, 200, 255 },
        { 199, 175, 0, 255 },
        { 52, 192, 63, 255 },
        { 159, 93, 200, 255 },
    };
    HuVecF pos;

    pos.x = arg1->x;
    pos.y = arg1->y;
    pos.z = arg1->z + 10.0f;
    fn_1_40AEC(arg0, 10.0f, 500.0f, &pos, colors[arg2]);
    fn_1_40FEC(arg0, 10.0f, &pos, colors[arg2]);
}

void fn_1_45F3C(s16 arg0, HuVecF *arg1, s16 arg2, s16 arg3)
{
    GXColor colors[5] = {
        { 254, 77, 75, 255 },
        { 50, 127, 200, 255 },
        { 199, 175, 0, 255 },
        { 52, 192, 63, 255 },
        { 159, 93, 200, 255 },
    };
    HuVecF pos;

    pos.x = arg1->x;
    pos.y = arg1->y;
    pos.z = arg1->z;
    Hu3DZClearLayerSet(7);
    if (arg3 == 0) {
        fn_1_40AEC(arg0, 20.0f, 300.0f, &pos, colors[arg2]);
        fn_1_4218C(arg0, 30.0f, &pos);
    } else {
        fn_1_40AEC(arg0, 60.0f, 300.0f, &pos, colors[arg2]);
        fn_1_40FEC(arg0, 30.0f, &pos, colors[arg2]);
    }
}

void fn_1_463E0(void)
{
    fn_1_40A9C();
    fn_1_41834();
}

void fn_1_464A0(s16 groupNo, HuVecF *pos, s16 mode, s16 colorNo)
{
    GXColor colors[7] = {
        { 254, 77, 75, 0 },
        { 50, 127, 200, 0 },
        { 199, 175, 0, 0 },
        { 52, 192, 63, 0 },
        { 159, 93, 200, 0 },
        { 255, 114, 46, 0 },
        { 109, 207, 246, 0 },
    };

    if (mode == 1) {
        fn_1_443B4(groupNo, pos, &colors[colorNo]);
    } else if (mode == 0) {
        fn_1_4451C(groupNo);
    } else if (mode == 2) {
        fn_1_44300(groupNo, FALSE);
    }
}

u32 lbl_1_data_F10[9] = {
    DATANUM(DATA_mdparty, 0x82),
    DATANUM(DATA_mdparty, 0x83),
    DATANUM(DATA_mdparty, 0x81),
    DATANUM(DATA_mdparty, 0x84),
    DATANUM(DATA_mdparty, 0x89),
    DATANUM(DATA_mdparty, 0x89),
    DATANUM(DATA_mdparty, 0x89),
    DATANUM(DATA_mdparty, 0x89),
    DATANUM(DATA_mdparty, 0x89),
};

ANIMDATA *lbl_1_bss_B40[9];
MDPARTY_STAGE_TEXTURE_WORK lbl_1_bss_AF0[2];
HU3D_MODELID lbl_1_bss_AEC;
HU3D_MODELID lbl_1_bss_AE4[4];
HU3D_MODELID lbl_1_bss_AE2;
HU3D_MODELID lbl_1_bss_ADA[4];
HU3D_MODELID lbl_1_bss_AD6[2];
HU3D_MODELID lbl_1_bss_AD2[2];
HU3D_MODELID lbl_1_bss_A96[6][5];
s16 lbl_1_bss_A94;
void *lbl_1_bss_A90;
ANIMDATA *lbl_1_bss_A8C;
ANIMDATA *lbl_1_bss_A88;
OMOBJ *lbl_1_bss_A84;
OMOBJMAN *lbl_1_bss_A80;
