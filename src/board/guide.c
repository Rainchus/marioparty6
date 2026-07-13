#define _MATH_H
#define M_PI 3.141592653589793
double sin(double);
double cos(double);

#include "game/board/guide.h"
#include "game/board/audio.h"

#include "game/hu3d.h"
#include "game/sprite.h"
#include "game/data.h"
#include "game/frand.h"
#include "game/gamework.h"

#include "dolphin/mtx.h"

#define DATA_board 0x50000

static int guideSunMotTbl[24] = {
    0x00110001, 0x00110001, 0x00110002, 0x00110003, 0x00110004, 0x00110005,
    0x00110006, 0x00110007, 0x00110008, 0x00110009, 0x0011000A, 0x0011000B,
    0x0011000C, 0x0011000D, 0x0011000E, 0x0011000F, 0x00110010, 0x00110011,
    0x00110012, 0x00110013, 0x00110014, 0x00110015, 0x00110016, 0x00110017,
};

static int guideMoonMotTbl[24] = {
    0x0011001C, 0x0011001C, 0x0011001D, 0x0011001E, 0x0011001F, 0x00110020,
    0x00110021, 0x00110022, 0x00110023, 0x00110023, 0x00110024, 0x00110025,
    0x00110026, 0x00110027, 0x00110028, 0x00110029, 0x0011002A, 0x0011002B,
    0x0011002C, 0x0011002D, 0x0011002E, 0x0011002F, 0x00110030, 0x00110031,
};

static int guideMdlFileTbl[2] = { 0x00110000, 0x0011001B };
static int *guideMotTbl[2] = { guideSunMotTbl, guideMoonMotTbl };
static s8 guideDefaultMotTbl[5] = { 1, 7, 4, 17, -1 };

static void GuideOMExec(OMOBJ *obj);
static BOOL GuideFadeInUpdate(OMOBJ *obj);
static BOOL GuideFadeOutUpdate(OMOBJ *obj);
static MBMODELID GuideFadeInEffectCreate(OMOBJ *obj);
static MBMODELID GuideFadeOutEffectCreate(OMOBJ *obj);
static void GuideFadeInEffectHook(HU3D_MODEL *modelP, MBPARTICLE *effP, Mtx matrix);
static void GuideFadeOutEffectHook(HU3D_MODEL *modelP, MBPARTICLE *effP, Mtx matrix);

void mbGuideInit(void)
{
}

OMOBJ *mbGuideCreate(int guideNo, HuVecF *pos, HuVecF *rot, s8 *motTbl, float scale, u32 attr)
{
    OMOBJ *obj;
    GUIDE_WORK *work;
    MBMODELID modelId;
    int motId;
    int i;
    int motNo;
    s8 *motP;
    u8 motEnable[25];

    for (i = 0; i < sizeof(motEnable); i++) {
        motEnable[i] = 0;
    }
    for (motP = guideDefaultMotTbl; *motP >= 0; ) {
        motEnable[*motP++] = 1;
    }
    if (motTbl != NULL) {
        for (motP = motTbl; *motP >= 0; ) {
            motEnable[*motP++] = 1;
        }
    }
    obj = omAddObjEx(mbObjMan, 0x100, 4, 0, OM_GRP_NONE, GuideOMExec);
    for (i = 0; i < 4; i++) {
        obj->mdlId[i] = -1;
    }
    omSetStatBit(obj, 0x100);
    work = (GUIDE_WORK *)&obj->work[0];
    work->name = 0x4D424744;
    work->dispF = 1;
    work->killF = 0;
    work->altMtxF = 0;
    work->motionF = 0;
    work->screenF = 0;
    work->mode = 0;
    work->nextMotion = 0;
    modelId = (int)mbObjCreate(guideMdlFileTbl[guideNo], 0, 1);
    obj->mdlId[0] = modelId;
    if (guideNo == 1) {
        obj->mdlId[1] = mbObjCreate(0x00110035, 0, 1);
        mbObjHookSet(modelId, "itemhook_R", obj->mdlId[1]);
    }
    for (i = 1; i < sizeof(motEnable); i++) {
        if (motEnable[i]) {
            mbObjMotionNoCreate(modelId, guideMotTbl[guideNo][i], i);
            if (i == 11 || i == 16) {
                motId = mbObjMotionIDGet(modelId, i);
                Hu3DMotionAttrSet(motId, 1);
            }
        }
    }
    if (mbPauseProcCheck()) {
        omSetStatBit(obj, 0xa0);
        mbObjAttrSet(modelId, 0x00200000);
    }
    if (attr & MB_GUIDE_ATTR_SCREEN) {
        work->screenF = 1;
        mbObjCameraSet(modelId, 4);
        mbObjLayerSet(modelId, 3);
    } else {
        mbObjLayerSet(modelId, 3);
    }
    if (pos) {
        mbObjPosSetV(modelId, pos);
    }
    if (rot) {
        mbObjRotSetV(modelId, rot);
    }
    mbObjScaleSet(modelId, scale, scale, scale);
    if (attr & MB_GUIDE_ATTR_LAYER) {
        motNo = 1;
    } else {
        motNo = 4;
    }
    mbObjMotionSet(modelId, motNo, 0x40000001);
    HuDataDirClose(0x00110000);
    if (attr & MB_GUIDE_ATTR_ALTMTX) {
        work->mode = 1;
        work->phase = 0;
        while (!mbGuideIdleCheck(obj)) {
            HuPrcVSleep();
        }
    }
    return obj;
}

OMOBJ *mbGuideCreateFlag(HuVecF *pos, s8 *motTbl, BOOL screenF, BOOL altMtxF, BOOL layerF)
{
    u32 attr = MB_GUIDE_ATTR_NONE;
    if (screenF) {
        attr |= MB_GUIDE_ATTR_SCREEN;
    }
    if (altMtxF) {
        attr |= MB_GUIDE_ATTR_ALTMTX;
    }
    if (layerF) {
        attr |= MB_GUIDE_ATTR_LAYER;
    }
    return mbGuideCreate(mbGuideNoGet(), pos, NULL, motTbl, 1.0f, attr);
}

OMOBJ *mbGuideCreateIn(void)
{
    return mbGuideCreate(mbGuideNoGet(), NULL, NULL, NULL, 1.0f, MB_GUIDE_ATTR_NONE);
}

void mbGuideKill(OMOBJ *obj)
{
    GUIDE_WORK *work = (GUIDE_WORK *)&obj->work[0];
    work->killF = 1;
    if (obj->mdlId[0] != 0) {
        mbObjDispSet(obj->mdlId[0], FALSE);
    }
}

void mbGuideEnd(OMOBJ *obj)
{
    GUIDE_WORK *work = (GUIDE_WORK *)&obj->work[0];
    work->mode = 3;
    work->phase = 0;
    while (!mbGuideIdleCheck(obj)) {
        HuPrcVSleep();
    }
    work->killF = 1;
    if (obj->mdlId[0] != 0) {
        mbObjDispSet(obj->mdlId[0], FALSE);
    }
}

void mbGuideFadeIn(OMOBJ *obj)
{
    GUIDE_WORK *work = (GUIDE_WORK *)&obj->work[0];
    work->mode = 1;
    work->phase = 0;
    while (!mbGuideIdleCheck(obj)) {
        HuPrcVSleep();
    }
}

void mbGuideFadeOut(OMOBJ *obj)
{
    GUIDE_WORK *work = (GUIDE_WORK *)&obj->work[0];
    work->mode = 3;
    work->phase = 0;
    while (!mbGuideIdleCheck(obj)) {
        HuPrcVSleep();
    }
    if (obj->mdlId[0] != 0) {
        mbObjDispSet(obj->mdlId[0], FALSE);
    }
}

int mbGuideModelGet(OMOBJ *obj)
{
    GUIDE_WORK *work = (GUIDE_WORK *)&obj->work[0];
    MBMODELID modelId = 0;
    if (!work->killF) {
        modelId = obj->mdlId[0];
    }
    return modelId;
}

void mbGuideScreenSet(OMOBJ *obj, BOOL screenF)
{
    GUIDE_WORK *work = (GUIDE_WORK *)&obj->work[0];
    MBMODELID modelId = 0;
    Mtx mtx;
    if (!work->killF) {
        modelId = obj->mdlId[0];
        if (screenF) {
            work->screenF = 1;
            mbObjCameraSet(modelId, 4);
            mbObjLayerSet(modelId, 3);
        } else {
            work->screenF = 0;
            mbObjCameraSet(modelId, 1);
            mbObjLayerSet(modelId, 3);
            PSMTXIdentity(mtx);
            mbObjMtxSet(modelId, &mtx);
        }
    }
}

BOOL mbGuideIdleCheck(OMOBJ *obj)
{
    GUIDE_WORK *work = (GUIDE_WORK *)&obj->work[0];
    return work->mode == 0;
}

void mbGuideMotionNextSet(OMOBJ *obj, s16 motNo)
{
    GUIDE_WORK *work = (GUIDE_WORK *)&obj->work[0];
    work->nextMotion = motNo;
}

void mbGuideMotionSet(OMOBJ *obj, s16 motNo, BOOL shiftF)
{
    GUIDE_WORK *work = (GUIDE_WORK *)&obj->work[0];
    MBMODELID modelId = obj->mdlId[0];
    if (shiftF) {
        mbObjMotionShiftSet(modelId, motNo, 0.0f, 12.0f, TRUE);
    } else {
        mbObjMotionSet(modelId, motNo, 1);
    }
    work->motionF = 0;
    work->nextMotion = motNo;
}

void mbGuideMotionShiftSet(OMOBJ *obj, s16 motNo, BOOL shiftF)
{
    GUIDE_WORK *work = (GUIDE_WORK *)&obj->work[0];
    MBMODELID modelId = obj->mdlId[0];
    if (shiftF) {
        mbObjMotionShiftSet(modelId, motNo, 0.0f, 12.0f, FALSE);
    } else {
        mbObjMotionSet(modelId, motNo, 0);
    }
    work->motionF = 1;
}

void mbGuideMotionStop(OMOBJ *obj)
{
    GUIDE_WORK *work = (GUIDE_WORK *)&obj->work[0];
    work->motionF = 0;
}

BOOL mbGuideMotionCheck(OMOBJ *obj)
{
    GUIDE_WORK *work = (GUIDE_WORK *)&obj->work[0];
    return work->motionF != 1;
}

static void GuideOMExec(OMOBJ *obj)
{
    GUIDE_WORK *work = (GUIDE_WORK *)&obj->work[0];
    int i;
    HU3D_CAMERA *camP;
    MBMODELID modelId;
    HuVecF pos;
    HuVecF scale;
    Mtx mtx;

    if (mbExitCheck() || work->killF) {
        if (obj->mdlId[0] > 0) {
            mbObjKill(obj->mdlId[0]);
        }
        if (obj->mdlId[1] > 0) {
            mbObjKill(obj->mdlId[1]);
        }
        obj->mdlId[0] = 0;
        obj->mdlId[1] = 0;
        if (obj->mdlId[2] >= 0) {
            mbParticleKill(obj->mdlId[2]);
        }
        if (obj->mdlId[3] >= 0) {
            mbParticleKill(obj->mdlId[3]);
        }
        obj->mdlId[2] = -1;
        obj->mdlId[3] = -1;
        work->name = 0;
        omDelObjEx(HuPrcCurrentGet(), obj);
        return;
    }
    switch (work->mode) {
        case 0:
            if (work->motionF) {
                if (mbObjMotionShiftIDGet(obj->mdlId[0]) < 0) {
                    if (mbObjMotionEndCheck(obj->mdlId[0])) {
                        mbObjMotionShiftSet(obj->mdlId[0], work->nextMotion, 0.0f, 12.0f, TRUE);
                        work->motionF = 0;
                    }
                }
            }
            break;

        case 1:
            if (GuideFadeInUpdate(obj)) {
                work->mode = 0;
            }
            break;

        case 2:
            break;

        case 3:
            if (GuideFadeOutUpdate(obj)) {
                work->mode = 0;
            }
            break;

        case 4:
            break;
    }
    if (work->screenF) {
        for (i = 0; i < 16; i++) {
            if ((1 << i) & 4) {
                break;
            }
        }
        camP = &Hu3DCamera[i];
        modelId = obj->mdlId[0];
        mbObjPosGet(modelId, &pos);
        mbObjScaleGet(modelId, &scale);
        pos.y = pos.y + 150.0f * scale.y;
        C_MTXLookAt(mtx, &camP->pos, &camP->up, &pos);
        PSMTXInverse(mtx, mtx);
        mbObjMtxSet(modelId, &mtx);
    }
}

static BOOL GuideFadeInUpdate(OMOBJ *obj)
{
    GUIDE_WORK *work = (GUIDE_WORK *)&obj->work[0];
    HuVecF pos;
    HuVecF rot = { 0.0f, 0.0f, 0.0f };
    BOOL done = FALSE;
    MBMODELID modelId = obj->mdlId[0];
    MBPARTICLE *effP;
    MBMODELID particleId;
    MBPARTICLE *effP2;
    float weight;

    switch (work->phase) {
        case 0:
            mbObjDispSet(modelId, TRUE);
            mbObjPosGet(modelId, &pos);
            obj->trans.x = pos.x;
            obj->trans.y = pos.y;
            obj->trans.z = pos.z;
            pos.y += 200.0f;
            mbObjFadeCreate(modelId, &pos);
            mbObjFadeTexColorSet(modelId, 0x80, 0x80, 0x80, 0.5f);
            if (obj->mdlId[1] > 0) {
                mbObjFadeCreate((s16)obj->mdlId[1], &pos);
                mbObjFadeTexColorSet((s16)obj->mdlId[1], 0x80, 0x80, 0x80, 0.5f);
            }
            obj->mdlId[2] = GuideFadeInEffectCreate(obj);
            Hu3DModelPosSetV(obj->mdlId[2], &pos);
            work->time = 0;
            work->timeMax = 60;
            work->phase++;
            mbAudFXPlay(0x460);
            break;

        case 1:
            work->time = 0;
            work->timeMax = 60;
            work->phase++;
            break;

        case 2:
            work->time++;
            weight = (float)work->time / work->timeMax;
            pos.x = obj->trans.x;
            pos.y = (obj->trans.y - 50.0f) + 250.0f * (1.0f - weight);
            pos.z = obj->trans.z;
            mbObjFadeTexRotSet(modelId, &pos, &rot);
            if (obj->mdlId[1] > 0) {
                mbObjFadeTexRotSet((s16)obj->mdlId[1], &pos, &rot);
            }
            if (pos.y <= obj->trans.y) {
                particleId = obj->mdlId[2];
                effP2 = Hu3DData[particleId].hookData;
                effP = effP2;
                effP->stopF = 1;
            } else {
                Hu3DModelPosSetV(obj->mdlId[2], &pos);
            }
            if (work->time >= work->timeMax) {
                work->timeMax = 30;
                work->time = 0;
                work->phase++;
            }
            break;

        case 3:
            work->time++;
            if (work->time >= work->timeMax) {
                work->phase++;
            } else {
                break;
            }
            /* fallthrough */

        case 4:
            mbObjFadeKill(modelId);
            if (obj->mdlId[1] > 0) {
                mbObjFadeKill((s16)obj->mdlId[1]);
            }
            mbParticleKill(obj->mdlId[2]);
            obj->mdlId[2] = -1;
            done = TRUE;
            break;
    }
    return done;
}

static BOOL GuideFadeOutUpdate(OMOBJ *obj)
{
    GUIDE_WORK *work = (GUIDE_WORK *)&obj->work[0];
    HuVecF pos;
    HuVecF rot = { 0.0f, 0.0f, 0.0f };
    BOOL done = FALSE;
    MBMODELID modelId = obj->mdlId[0];
    MBPARTICLE *effP;
    MBMODELID particleId;
    MBPARTICLE *effP2;
    float weight;

    switch (work->phase) {
        case 0:
            mbObjPosGet(modelId, &pos);
            obj->trans.x = pos.x;
            obj->trans.y = pos.y;
            obj->trans.z = pos.z;
            pos.y -= 50.0f;
            mbObjFadeCreate(modelId, &pos);
            mbObjFadeTexColorSet(modelId, 0x80, 0x80, 0x80, 0.5f);
            if (obj->mdlId[1] > 0) {
                mbObjFadeCreate((s16)obj->mdlId[1], &pos);
                mbObjFadeTexColorSet((s16)obj->mdlId[1], 0x80, 0x80, 0x80, 0.5f);
            }
            obj->mdlId[2] = GuideFadeOutEffectCreate(obj);
            Hu3DModelPosSetV(obj->mdlId[2], &obj->trans);
            work->time = 0;
            work->timeMax = 60;
            work->phase++;
            mbAudFXPlay(0x461);
            break;

        case 1:
            work->time = 0;
            work->timeMax = 60;
            work->phase++;
            break;

        case 2:
            work->time++;
            weight = (float)work->time / work->timeMax;
            pos.x = obj->trans.x;
            pos.y = (obj->trans.y - 50.0f) + 250.0f * weight;
            pos.z = obj->trans.z;
            mbObjFadeTexRotSet(modelId, &pos, &rot);
            if (obj->mdlId[1] > 0) {
                mbObjFadeTexRotSet((s16)obj->mdlId[1], &pos, &rot);
            }
            if (pos.y >= obj->trans.y) {
                Hu3DModelPosSetV(obj->mdlId[2], &pos);
            }
            if (pos.y >= obj->trans.y + 160.0f) {
                particleId = obj->mdlId[2];
                effP2 = Hu3DData[particleId].hookData;
                effP = effP2;
                effP->stopF = 1;
            }
            if (work->time >= work->timeMax) {
                work->timeMax = 30;
                work->time = 0;
                work->phase++;
            }
            break;

        case 3:
            work->time++;
            if (work->time >= work->timeMax) {
                work->phase++;
            } else {
                break;
            }
            /* fallthrough */

        case 4:
            mbObjDispSet(obj->mdlId[0], FALSE);
            mbObjFadeKill(modelId);
            if (obj->mdlId[1] > 0) {
                mbObjDispSet(obj->mdlId[1], FALSE);
                mbObjFadeKill((s16)obj->mdlId[1]);
            }
            mbParticleKill(obj->mdlId[2]);
            obj->mdlId[2] = -1;
            done = TRUE;
            break;
    }
    return done;
}

int mbGuideNoGet(void)
{
    int no = 0;
    int party = GwSystem.partyF;
    if (party == 0) {
        no = 0;
    } else if (GwSystem.curTime == 0) {
        no = 0;
    } else {
        no = 1;
    }
    return no;
}

int mbGuideSpeakerNoGet(void)
{
    static int speakerTbl[2] = { 6, 7 };
    return speakerTbl[mbGuideNoGet()];
}

static MBMODELID GuideFadeInEffectCreate(OMOBJ *obj)
{
    int particleId;
    Mtx mtx;
    particleId = mbParticleCreate(HuSprAnimDataRead(mbBoardDataNumGet(DATANUM(DATA_board, 100))), 0xc8);
    mbParticleHookSet(particleId, GuideFadeInEffectHook);
    Hu3DModelCameraSet(particleId, mbObjGet(obj->mdlId[0])->cameraBit);
    Hu3DModelLayerSet(particleId, mbObjGet(obj->mdlId[0])->layer + 1);
    mbObjMtxGet(obj->mdlId[0], &mtx);
    Hu3DModelMtxSet(particleId, &mtx);
    if (mbPauseProcCheck()) {
        Hu3DModelAttrSet(particleId, 0x00200000);
    }
    return particleId;
}

static MBMODELID GuideFadeOutEffectCreate(OMOBJ *obj)
{
    int particleId;
    Mtx mtx;
    particleId = mbParticleCreate(HuSprAnimDataRead(mbBoardDataNumGet(DATANUM(DATA_board, 100))), 0xc8);
    mbParticleHookSet(particleId, GuideFadeOutEffectHook);
    Hu3DModelCameraSet(particleId, mbObjGet(obj->mdlId[0])->cameraBit);
    Hu3DModelLayerSet(particleId, mbObjGet(obj->mdlId[0])->layer + 1);
    mbObjMtxGet(obj->mdlId[0], &mtx);
    Hu3DModelMtxSet(particleId, &mtx);
    if (mbPauseProcCheck()) {
        Hu3DModelAttrSet(particleId, 0x00200000);
    }
    return particleId;
}

static void GuideFadeInEffectHook(HU3D_MODEL *modelP, MBPARTICLE *effP, Mtx matrix)
{
    static u8 effNo[16] = { 0, 1, 2, 2, 3, 3, 3, 3, 0, 1, 2, 2, 3, 3, 3, 3 };
    static float effSize[4] = { 0.9f, 0.9f, 0.9f, 0.8f };
    static GXColor effColor[8] = {
        { 0xDC, 0x40, 0x40, 0x00 }, { 0x40, 0xDC, 0x40, 0x00 },
        { 0xDC, 0xDC, 0x40, 0x00 }, { 0x40, 0x40, 0xDC, 0x00 },
        { 0xDC, 0x40, 0xDC, 0x00 }, { 0x40, 0xDC, 0xDC, 0x00 },
        { 0xDC, 0x78, 0x40, 0x00 }, { 0x40, 0x78, 0xDC, 0x00 },
    };
    MBPARTICLEDATA *data;
    int i;
    int spawn;
    int type;
    int colorIdx;
    float f;
    float weight;

    if (effP->initF == 0) {
        effP->initF = 1;
        effP->stopF = 0;
        effP->unk14 = 0.0f;
    }
    spawn = 0;
    if (effP->stopF == 0) {
        spawn = 6;
    }
    data = effP->data;
    for (i = 0; i < effP->num; i++, data++) {
        if (spawn <= 0) {
            break;
        }
        if (data->time == 0) {
            type = effNo[mbRandMod(16)];
            data->rndNo = type;
            data->vel.x = 360.0f * frandf();
            data->vel.y = 80.0f * (0.3f + 0.7f * frandf());
            data->vel.z = 130.0f * (0.7f + 0.3f * frandf());
            data->scale = effSize[type] * (35.0f * (0.5f + 0.5f * frandf()));
            data->guideScaleBase = data->scale;
            colorIdx = mbRandMod(8);
            f = 0.3f * frandf();
            data->color.r = f * (255.0f - effColor[colorIdx].r) + effColor[colorIdx].r;
            data->color.g = f * (255.0f - effColor[colorIdx].g) + effColor[colorIdx].g;
            data->color.b = f * (255.0f - effColor[colorIdx].b) + effColor[colorIdx].b;
            data->color.a = mbRandMod(0x46) + 0x96;
            data->alphaF = data->color.a;
            data->color.a = 0;
            data->weight = 0.0f;
            if (data->rndNo < 3) {
                data->weight = 360.0f * frandf();
            }
            data->time = 30;
            spawn--;
        }
    }
    data = effP->data;
    for (i = 0; i < effP->num; i++, data++) {
        if (data->time != 0) {
            f = 0.033333335f * (float)data->time;
            data->vel.x += 5.0f;
            data->pos.x = f * (data->vel.z * mbSinDeg(data->vel.x));
            data->pos.z = f * (data->vel.z * mbCosDeg(data->vel.x));
            weight = 1.6666666f * (f - 0.4f);
            if (weight < 0.0f) {
                weight = 0.0f;
            }
            data->pos.y = data->vel.y * (f * f);
            data->scale -= 0.4f;
        }
    }
    data = effP->data;
    for (i = 0; i < effP->num; i++, data++) {
        if (data->time != 0) {
            data->time--;
            if (data->time < 10) {
                data->scale *= 0.95f;
                data->color.a *= 0.7f;
                if (data->time == 0) {
                    data->color.a = 0;
                    data->scale = 0.0f;
                }
            } else if (effP->stopF == 0) {
                f = data->color.a;
                f = 1.0f + (f + 0.3f * (data->alphaF - f));
                data->color.a = f;
            }
            if (data->color.r < 0xfa) {
                data->color.r += 5;
            }
            if (data->color.g < 0xfa) {
                data->color.g += 5;
            }
            if (data->color.b < 0xfa) {
                data->color.b += 5;
            }
            if (effP->stopF != 0) {
                data->color.a *= 0.8f;
            }
        }
    }
}

static void GuideFadeOutEffectHook(HU3D_MODEL *modelP, MBPARTICLE *effP, Mtx matrix)
{
    static u8 effNo[16] = { 0, 1, 2, 2, 3, 3, 3, 3, 0, 1, 2, 2, 3, 3, 3, 3 };
    static float effSize[4] = { 0.9f, 0.9f, 0.9f, 0.8f };
    static GXColor effColor[8] = {
        { 0xDC, 0x40, 0x40, 0x00 }, { 0x40, 0xDC, 0x40, 0x00 },
        { 0xDC, 0xDC, 0x40, 0x00 }, { 0x40, 0x40, 0xDC, 0x00 },
        { 0xDC, 0x40, 0xDC, 0x00 }, { 0x40, 0xDC, 0xDC, 0x00 },
        { 0xDC, 0x78, 0x40, 0x00 }, { 0x40, 0x78, 0xDC, 0x00 },
    };
    MBPARTICLEDATA *data;
    int i;
    int spawn;
    int type;
    int colorIdx;
    float f;
    float g;
    HuVecF vec;
    Mtx mtx;

    if (effP->initF == 0) {
        effP->initF = 1;
        effP->stopF = 0;
        effP->unk14 = 0.0f;
    }
    spawn = 0;
    if (effP->stopF == 0) {
        spawn = 6;
    }
    data = effP->data;
    for (i = 0; i < effP->num; i++, data++) {
        if (spawn <= 0) {
            break;
        }
        if (data->time == 0) {
            type = effNo[mbRandMod(16)];
            data->rndNo = type;
            f = 360.0f * frandf();
            mbMtxRotAxisDeg(mtx, 'Y', f);
            vec.x = vec.y = 0.0f;
            vec.z = 0.016666668f * (600.0f * (0.7f + 0.3f * frandf()));
            PSMTXMultVec(mtx, &vec, &data->vel);
            f = 360.0f * frandf();
            g = 0.7f + 0.3f * frandf();
            vec.x = g * mbCosDeg(f);
            vec.y = 0.2f + g * mbSinDeg(f);
            vec.z = -0.8f;
            PSMTXMultVec(mtx, &vec, &data->guideAccel);
            PSVECScale(&data->guideAccel, &data->guideAccel, 0.55555564f);
            PSVECScale(&data->vel, &data->pos, 1.0f);
            data->pos.y += 5.0f;
            data->scale = effSize[type] * (30.0f * (0.5f + 0.5f * frandf()));
            data->guideScaleBase = data->scale;
            colorIdx = mbRandMod(8);
            data->no = colorIdx;
            f = 0.8f + 0.2f * frandf();
            data->color.r = f * (255.0f - effColor[colorIdx].r) + effColor[colorIdx].r;
            data->color.g = f * (255.0f - effColor[colorIdx].g) + effColor[colorIdx].g;
            data->color.b = f * (255.0f - effColor[colorIdx].b) + effColor[colorIdx].b;
            data->color.a = mbRandMod(0x46) + 0x96;
            data->alphaF = data->color.a;
            data->color.a = 0;
            data->weight = 0.0f;
            if (data->rndNo < 3) {
                data->weight = 360.0f * frandf();
            }
            data->time = 20;
            spawn--;
        }
    }
    data = effP->data;
    for (i = 0; i < effP->num; i++, data++) {
        if (data->time != 0) {
            PSVECAdd(&data->pos, &data->vel, &data->pos);
            PSVECAdd(&data->vel, &data->guideAccel, &data->vel);
        }
    }
    data = effP->data;
    for (i = 0; i < effP->num; i++, data++) {
        if (data->time != 0) {
            data->time--;
            if (data->time < 10) {
                data->scale *= 0.95f;
                data->color.a *= 0.7f;
                if (data->time == 0) {
                    data->color.a = 0;
                    data->scale = 0.0f;
                }
            } else if (effP->stopF == 0) {
                f = data->color.a;
                f = 1.0f + (f + 0.3f * (data->alphaF - f));
                data->color.a = f;
            }
            colorIdx = data->no;
            data->color.r = data->color.r + 0.1f * ((float)effColor[colorIdx].r - data->color.r);
            data->color.g = data->color.g + 0.1f * ((float)effColor[colorIdx].g - data->color.g);
            data->color.b = data->color.b + 0.1f * ((float)effColor[colorIdx].b - data->color.b);
            if (effP->stopF != 0) {
                data->color.a *= 0.8f;
            }
        }
    }
}
