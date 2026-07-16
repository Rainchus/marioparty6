#define _MATH_H
#define M_PI 3.141592653589793

double cos(double);

#include "game/board/object.h"
#include "game/board/audio.h"
#include "game/board/camera.h"
#include "game/board/main.h"
#include "game/data.h"
#include "game/memory.h"

#include "string.h"

extern void *mbMalloc(s32 size);
extern void mbPos3Dto2D(HuVecF *pos3D, HuVecF *pos2D);
extern void mbObjCullInit(MBMODELID modelId);

static OMOBJ *objManOMObj;
static MBOBJMODEL *objManData;
static s16 objManNum;
static int objManDispNum;
static int dataDirDay;
static int dataDirNight;

static void ObjManOMExec(OMOBJ *obj);
static MBMODELID ObjManObjCreate(int charNo, int dataNum, const int *motDataNum, BOOL linkF);
static BOOL ObjManModelCreate(MBOBJMODEL *modelP, int dataNum, BOOL linkF);
static int ObjManMotionCreate(MBOBJMODEL *modelP, int motNum, const int *motDataNum);
static MBOBJMODEL *ObjManLinkSearch(int dataNum);

int mbObjDataNumGet(int dataNum)
{
    int result;

    if (!GwSystem.curTime) {
        if (DIRNUM(dataNum) == dataDirNight) {
            result = dataDirDay | FILENUM(dataNum);
            goto done;
        }
    } else if (DIRNUM(dataNum) == dataDirDay) {
        result = dataDirNight | FILENUM(dataNum);
        goto done;
    }
    result = dataNum;
done:
    return result;
}

void mbObjInit(void)
{
    objManNum = 0;
    dataDirDay = dataDirNight = DIRNUM(HU_DATANUM_NONE);
    objManData = mbMalloc(sizeof(MBOBJMODEL) * MB_OBJ_MAX);
    objManOMObj = omAddObjEx(mbObjMan, 32259, 0, 0, OM_GRP_NONE, ObjManOMExec);
    omSetStatBit(objManOMObj, OM_STAT_NOPAUSE | OM_STAT_SPRPAUSE);
}

void mbObjClose(void)
{
    int i;

    for (i = 1; i < MB_OBJ_MAX; i++) {
        if (objManData[i].id != 0) {
            mbObjKill(i);
        }
    }
    if (objManData) {
        MBOBJMODEL *modelP = objManData;

        HuMemDirectFree(modelP);
        objManData = NULL;
    }
}

static void ObjManOMExec(OMOBJ *obj)
{
    MBOBJMODEL *modelP;
    HuVecF pos;
    HuVecF pos2D;
    u32 attr;
    float fadePos;
    float fade;
    int i;

    if (mbExitCheck()) {
        objManOMObj = NULL;
        omDelObjEx(HuPrcCurrentGet(), obj);
        return;
    }

    modelP = &objManData[1];
    objManDispNum = 0;
    for (i = 1; i < MB_OBJ_MAX; i++, modelP++) {
        if (modelP->id == 0) {
            continue;
        }
        if (modelP->cullRadius >= 0.0f) {
            pos = modelP->pos;
            PSVECAdd(&pos, &modelP->offset, &pos);
            if (!mbCameraCullCheck(&pos, modelP->cullRadius)) {
                continue;
            }
        }
        if (modelP->fadeF) {
            mbPos3Dto2D(&pos, &pos2D);
            if (pos2D.z < 1000.0f) {
                fadePos = pos2D.z / 1000.0f;
                if (fadePos < 0.0f) {
                    fadePos = 0.0f;
                }
                fade = 1.0 - cos((M_PI * (90.0f * fadePos)) / 180.0);
            } else {
                fade = 1.0f;
            }
            Hu3DModelTPLvlSet(modelP->modelId, fade * ((float) modelP->alpha / 255.0f));
        }

        attr = Hu3DModelAttrGet(modelP->modelId);
        if (!modelP->dispF) {
            Hu3DModelAttrSet(modelP->modelId, HU3D_ATTR_DISPOFF);
        } else {
            Hu3DModelAttrReset(modelP->modelId, HU3D_ATTR_DISPOFF);
            objManDispNum++;
        }
        Hu3DModelPosSet(modelP->modelId,
            modelP->pos.x + modelP->offset.x,
            modelP->pos.y + modelP->offset.y,
            modelP->pos.z + modelP->offset.z);
        Hu3DModelRotSetV(modelP->modelId, &modelP->rot);
        Hu3DModelScaleSetV(modelP->modelId, &modelP->scale);
    }
}

static MBMODELID ObjManObjCreate(int charNo, int dataNum, const int *motDataNum, BOOL linkF)
{
    MBOBJMODEL *modelP;
    HU3D_MODEL *model3DP;
    BOOL result;
    int motNum;
    int i;

    for (modelP = &objManData[1], i = 1; i < MB_OBJ_MAX; i++, modelP++) {
        if (modelP->id == 0) {
            break;
        }
    }
    if (i >= MB_OBJ_MAX) {
        return MB_MODEL_NONE;
    }

    memset(modelP, 0, sizeof(MBOBJMODEL));
    modelP->id = i;
    modelP->charNo = charNo;
    result = ObjManModelCreate(modelP, dataNum, linkF);

    motNum = 0;
    if (motDataNum != NULL) {
        while (motDataNum[motNum] >= 0) {
            motNum++;
        }
    }
    for (i = 0; i < MB_OBJ_MOT_MAX; i++) {
        modelP->motId[i] = HU3D_MOTIONID_NONE;
    }
    result = ObjManMotionCreate(modelP, motNum, motDataNum);

    modelP->dispF = TRUE;
    modelP->dispOldF = modelP->dispF;
    modelP->cameraBit = HU3D_CAM0;
    modelP->layer = 0;
    modelP->cullRadius = -1.0f;
    modelP->alpha = 255;
    Hu3DModelCameraSet(modelP->modelId, modelP->cameraBit);
    Hu3DModelLayerSet(modelP->modelId, modelP->layer);
    mbObjPosSet(modelP->id, 0.0f, 0.0f, 0.0f);
    mbObjRotSet(modelP->id, 0.0f, 0.0f, 0.0f);
    mbObjScaleSet(modelP->id, 1.0f, 1.0f, 1.0f);
    mbObjOffsetSet(modelP->id, 0.0f, 0.0f, 0.0f);
    mbObjAmbSet(modelP->id, 1.0f, 1.0f, 1.0f);
    objManNum++;

    model3DP = &Hu3DData[modelP->modelId];
    if (model3DP->linkMdlId < 0) {
        mbObjCullInit(modelP->id);
    }
    return modelP->id;
}

MBMODELID mbObjCreate(int dataNum, const int *motDataNum, BOOL linkF)
{
    MBMODELID modelId;

    if (linkF && !ObjManLinkSearch(dataNum)) {
        modelId = ObjManObjCreate(CHARNO_NONE, dataNum, motDataNum, linkF);
        mbObjDispSet(modelId, FALSE);
    }
    return ObjManObjCreate(CHARNO_NONE, dataNum, motDataNum, linkF);
}

MBMODELID mbObjCharCreate(int charNo, int dataNum, const int *motDataNum, BOOL linkF)
{
    MBMODELID modelId = ObjManObjCreate(charNo, dataNum, motDataNum, linkF);

    mbObjLayerSet(modelId, 3);
    return modelId;
}

static BOOL ObjManModelCreate(MBOBJMODEL *modelP, int dataNum, BOOL linkF)
{
    MBOBJMODEL *linkP = NULL;
    void *data;
    int readDataNum;

    modelP->linkNum = 0;
    if (linkF && modelP->charNo == CHARNO_NONE) {
        linkP = ObjManLinkSearch(dataNum);
        if (linkP) {
            modelP->modelId = Hu3DModelLink(linkP->modelId);
            modelP->dataNum = 0;
            modelP->linkP = linkP;
            linkP->linkNum++;
            return TRUE;
        }
    }

    modelP->linkP = NULL;
    if (modelP->charNo == CHARNO_NONE) {
        if (!GwSystem.curTime) {
            if (DIRNUM(dataNum) == dataDirNight) {
                readDataNum = dataDirDay | FILENUM(dataNum);
                goto read_data;
            }
        } else if (DIRNUM(dataNum) == dataDirDay) {
            readDataNum = dataDirNight | FILENUM(dataNum);
            goto read_data;
        }
        readDataNum = dataNum;
read_data:
        data = HuDataSelHeapReadNum(readDataNum, HU_MEMNUM_OVL, HEAP_MODEL);
        if (!data) {
            s32 modelRestMem = HuRestMemGet(HEAP_MODEL);
            s32 dvdRestMem = HuRestMemGet(HEAP_DVD);
        }
    } else {
        data = NULL;
    }

    if (modelP->charNo == CHARNO_NONE) {
        modelP->modelId = Hu3DModelCreate(data);
    } else {
        modelP->modelId = CharModelCreate(modelP->charNo, CHAR_MODEL2);
    }
    if (!linkF) {
        modelP->dataNum = 0;
    } else {
        modelP->dataNum = dataNum;
    }
    return TRUE;
}

static int ObjManMotionCreate(MBOBJMODEL *modelP, int motNum, const int *motDataNum)
{
    void *data;
    HU3D_MOTIONID motId;
    int i;

    modelP->motId[0] = Hu3DData[modelP->modelId].motId;
    if (motDataNum == NULL) {
        modelP->motNum = 1;
        return TRUE;
    }

    data = NULL;
    for (i = 0; i < motNum; i++) {
        if (modelP->charNo == CHARNO_NONE) {
            data = HuDataSelHeapReadNum(mbObjDataNumGet(motDataNum[i]), HU_MEMNUM_OVL, HEAP_MODEL);
            if (data == NULL) {
                return -1;
            }
            motId = Hu3DJointMotion(modelP->modelId, data);
        } else {
            motId = CharMotionCreate(modelP->charNo, motDataNum[i]);
        }
        if (motId < 0) {
            return -1;
        }
        modelP->motId[i + 1] = motId;
        modelP->motData[i + 1] = data;
        modelP->motDataNum[i + 1] = motDataNum[i];
    }
    modelP->motNum = motNum + 1;
    return TRUE;
}

static MBOBJMODEL *ObjManLinkSearch(int dataNum)
{
    MBOBJMODEL *modelP;
    int i;

    for (modelP = &objManData[1], i = 1; i < MB_OBJ_MAX; i++, modelP++) {
        if (modelP->id != 0 && modelP->dataNum == dataNum) {
            return modelP;
        }
    }
    return NULL;
}

void mbObjKill(MBMODELID modelId)
{
    MBOBJMODEL *modelP = &objManData[modelId];
    MBOBJMODEL *searchP;
    int i;

    if (modelP->charNo == CHARNO_NONE) {
        if (modelP->linkNum != 0) {
            for (searchP = &objManData[1], i = 1; i < MB_OBJ_MAX; i++, searchP++) {
                if (searchP->id != 0 && searchP->linkP == modelP) {
                    mbObjKill(searchP->id);
                }
            }
            return;
        }
        modelP->motId[0] = HU3D_MOTIONID_NONE;
        if (modelP->modelId >= 0) {
            Hu3DModelKill(modelP->modelId);
            modelP->modelId = HU3D_MODELID_NONE;
        }
        for (i = 1; i < MB_OBJ_MOT_MAX; i++) {
            if (modelP->motId[i] >= 0) {
                Hu3DMotionKill(modelP->motId[i]);
                modelP->motId[i] = HU3D_MOTIONID_NONE;
            }
            modelP->motData[i] = NULL;
        }
        modelP->dataNum = 0;
        if (modelP->linkP) {
            if (--modelP->linkP->linkNum == 0) {
                mbObjKill(modelP->linkP->id);
            }
        }
    } else {
        for (i = 0; i < MB_OBJ_MOT_MAX; i++) {
            modelP->motId[i] = HU3D_MOTIONID_NONE;
            modelP->motData[i] = NULL;
        }
        if (modelP->modelId >= 0) {
            modelP->modelId = HU3D_MODELID_NONE;
        }
        CharModelKill(modelP->charNo);
        modelP->charNo = CHARNO_NONE;
    }
    objManNum--;
    modelP->id = 0;
    modelP->dispF = FALSE;
}

void mbObjDirSet(int day, int night)
{
    dataDirDay = day;
    dataDirNight = night;
}

MBOBJMODEL *mbObjGet(MBMODELID modelId)
{
    return &objManData[modelId];
}

BOOL mbObjDispGet(MBMODELID modelId)
{
    return objManData[modelId].dispF;
}

void mbObjDispSet(MBMODELID modelId, BOOL dispF)
{
    objManData[modelId].dispF = dispF;
}

void mbObjCameraSet(MBMODELID modelId, u16 cameraBit)
{
    MBOBJMODEL *modelP = &objManData[modelId];

    modelP->cameraBit = cameraBit;
    Hu3DModelCameraSet(modelP->modelId, cameraBit);
}

int mbObjLayerGet(MBMODELID modelId)
{
    return objManData[modelId].layer;
}

void mbObjLayerSet(MBMODELID modelId, u8 layer)
{
    MBOBJMODEL *modelP = &objManData[modelId];

    modelP->layer = layer;
    Hu3DModelLayerSet(modelP->modelId, layer);
}

void mbObjLayerSetAll(u8 layer)
{
    MBOBJMODEL *modelP;
    int i;

    for (modelP = &objManData[1], i = 1; i < MB_OBJ_MAX; i++, modelP++) {
        if (modelP->id != 0) {
            Hu3DModelLayerSet(modelP->modelId, layer);
        }
    }
}

HU3D_MODELID mbObjModelIDGet(MBMODELID modelId)
{
    return objManData[modelId].modelId;
}

void mbObjPosGet(MBMODELID modelId, HuVecF *pos)
{
    *pos = objManData[modelId].pos;
}

void mbObjPosSetV(MBMODELID modelId, const HuVecF *pos)
{
    mbObjPosSet(modelId, pos->x, pos->y, pos->z);
}

void mbObjPosSet(MBMODELID modelId, float x, float y, float z)
{
    MBOBJMODEL *modelP = &objManData[modelId];

    modelP->pos.x = x;
    modelP->pos.y = y;
    modelP->pos.z = z;
    Hu3DModelPosSet(modelP->modelId,
        modelP->pos.x + modelP->offset.x,
        modelP->pos.y + modelP->offset.y,
        modelP->pos.z + modelP->offset.z);
}

void mbObjOffsetGet(MBMODELID modelId, HuVecF *offset)
{
    *offset = objManData[modelId].offset;
}

void mbObjOffsetSetV(MBMODELID modelId, const HuVecF *offset)
{
    mbObjOffsetSet(modelId, offset->x, offset->y, offset->z);
}

void mbObjOffsetSet(MBMODELID modelId, float x, float y, float z)
{
    MBOBJMODEL *modelP = &objManData[modelId];
    HuVecF pos;

    modelP->offset.x = x;
    modelP->offset.y = y;
    modelP->offset.z = z;
    mbObjPosGet(modelId, &pos);
    mbObjPosSetV(modelId, &pos);
}

void mbObjRotGet(MBMODELID modelId, HuVecF *rot)
{
    *rot = objManData[modelId].rot;
}

void mbObjRotSetV(MBMODELID modelId, const HuVecF *rot)
{
    mbObjRotSet(modelId, rot->x, rot->y, rot->z);
}

void mbObjRotSet(MBMODELID modelId, float x, float y, float z)
{
    MBOBJMODEL *modelP = &objManData[modelId];

    modelP->rot.x = x;
    modelP->rot.y = y;
    modelP->rot.z = z;
    Hu3DModelRotSetV(modelP->modelId, &modelP->rot);
}

float mbObjRotYGet(MBMODELID modelId)
{
    return objManData[modelId].rot.y;
}

void mbObjRotYSet(MBMODELID modelId, float rotY)
{
    HuVecF rot;

    mbObjRotGet(modelId, &rot);
    mbObjRotSet(modelId, rot.x, rotY, rot.z);
}

void mbObjScaleGet(MBMODELID modelId, HuVecF *scale)
{
    *scale = objManData[modelId].scale;
}

void mbObjScaleSetV(MBMODELID modelId, const HuVecF *scale)
{
    mbObjScaleSet(modelId, scale->x, scale->y, scale->z);
}

void mbObjScaleSet(MBMODELID modelId, float x, float y, float z)
{
    MBOBJMODEL *modelP = &objManData[modelId];

    modelP->scale.x = x;
    modelP->scale.y = y;
    modelP->scale.z = z;
    Hu3DModelScaleSetV(modelP->modelId, &modelP->scale);
}

void mbObjMtxGet(MBMODELID modelId, Mtx *mtx)
{
    MBOBJMODEL *modelP = &objManData[modelId];
    HU3D_MODEL *model3DP = &Hu3DData[modelP->modelId];

    PSMTXCopy(model3DP->mtx, *mtx);
}

void mbObjMtxSet(MBMODELID modelId, Mtx *mtx)
{
    MBOBJMODEL *modelP = &objManData[modelId];
    HU3D_MODEL *model3DP = &Hu3DData[modelP->modelId];

    PSMTXCopy(*mtx, model3DP->mtx);
}

void mbObjAttrSet(MBMODELID modelId, u32 attr)
{
    MBOBJMODEL *modelP = &objManData[modelId];

    Hu3DModelAttrSet(modelP->modelId, attr);
}

void mbObjAttrReset(MBMODELID modelId, u32 attr)
{
    MBOBJMODEL *modelP = &objManData[modelId];

    Hu3DModelAttrReset(modelP->modelId, attr);
}

void mbObjAmbSet(MBMODELID modelId, float r, float g, float b)
{
    Hu3DModelAmbSet(objManData[modelId].modelId, r, g, b);
}

void mbObjAmbSetAll(float r, float g, float b)
{
    MBOBJMODEL *modelP;
    int i;

    for (modelP = &objManData[1], i = 1; i < MB_OBJ_MAX; i++, modelP++) {
        if (modelP->id != 0) {
            mbObjAmbSet(modelP->id, r, g, b);
        }
    }
}

u8 mbObjAlphaGet(MBMODELID modelId)
{
    return objManData[modelId].alpha;
}

void mbObjAlphaSet(MBMODELID modelId, u8 alpha)
{
    MBOBJMODEL *modelP = &objManData[modelId];

    modelP->alpha = alpha;
    Hu3DModelTPLvlSet(modelP->modelId, (float) alpha / 255.0f);
}

void mbObjFadeSet(MBMODELID modelId, BOOL fadeF)
{
    objManData[modelId].fadeF = fadeF;
}

void mbObjZWriteOffSet(MBMODELID modelId, BOOL zWriteOffF)
{
    MBOBJMODEL *modelP = &objManData[modelId];

    if (zWriteOffF) {
        Hu3DModelAttrSet(modelP->modelId, HU3D_ATTR_ZWRITE_OFF);
    } else {
        Hu3DModelAttrReset(modelP->modelId, HU3D_ATTR_ZWRITE_OFF);
    }
}

void mbObjCullRadiusSet(MBMODELID modelId, float radius)
{
    if (radius < 0.0f) {
        radius = -1.0f;
    }
    objManData[modelId].cullRadius = radius;
}

void mbObjHookSet(MBMODELID modelId, char *objName, MBMODELID hookModelId)
{
    MBOBJMODEL *modelP = &objManData[modelId];
    MBOBJMODEL *hookP = &objManData[hookModelId];

    Hu3DModelHookSet(modelP->modelId, objName, hookP->modelId);
    mbObjPosSet(hookModelId, 0.0f, 0.0f, 0.0f);
}

void mbObjHookReset(MBMODELID modelId)
{
    MBOBJMODEL *modelP = &objManData[modelId];

    Hu3DModelHookReset(modelP->modelId);
}

void mbObjHookObjReset(MBMODELID modelId, char *objName)
{
    MBOBJMODEL *modelP = &objManData[modelId];

    Hu3DModelHookObjReset(modelP->modelId, objName);
}

int mbObjSePlay(MBMODELID modelId, s16 seId)
{
    MBOBJMODEL *modelP = &objManData[modelId];

    return mbAudFXPosPlay(seId, &modelP->pos);
}

void mbObjMotionVoiceOnSet(MBMODELID modelId, int motNo, BOOL voiceOnF)
{
    MBOBJMODEL *modelP = &objManData[modelId];

    if (modelP->charNo != CHARNO_NONE) {
        CharMotionVoiceOnSet(modelP->charNo, modelP->motDataNum[motNo], voiceOnF);
    }
}

int mbObjMotionGet(MBMODELID modelId)
{
    MBOBJMODEL *modelP = &objManData[modelId];

    return modelP->motNo;
}

void mbObjMotionSet(MBMODELID modelId, int motNo, u32 attr)
{
    MBOBJMODEL *modelP = &objManData[modelId];

    if (modelP->charNo == CHARNO_NONE) {
        Hu3DMotionSet(modelP->modelId, modelP->motId[motNo]);
    } else {
        CharMotionSet(modelP->charNo, modelP->motId[motNo]);
    }
    Hu3DMotionSpeedSet(modelP->modelId, 1.0f);
    modelP->motStart = 0.0f;
    if (modelP->charNo == CHARNO_NONE) {
        modelP->motEnd = Hu3DMotionMaxTimeGet(modelP->modelId);
    } else {
        modelP->motEnd = CharMotionMaxTimeGet(modelP->charNo);
    }
    if (attr & (HU3D_MOTATTR_REV | HU3D_MOTATTR_SHIFT_REV)) {
        if (modelP->charNo == CHARNO_NONE) {
            Hu3DMotionTimeSet(modelP->modelId, modelP->motEnd);
        } else {
            CharMotionTimeSet(modelP->charNo, modelP->motEnd);
        }
    }
    Hu3DModelAttrReset(modelP->modelId, HU3D_MOTATTR | HU3D_MOTATTR_LOOP | HU3D_MOTATTR_PAUSE);
    Hu3DModelAttrSet(modelP->modelId, attr);
    modelP->motNo = motNo;
}

int mbObjMotionIDCurGet(MBMODELID modelId)
{
    return mbObjMotionIDGet(modelId, mbObjMotionGet(modelId));
}

int mbObjMotionIDGet(MBMODELID modelId, int motNo)
{
    MBOBJMODEL *modelP = &objManData[modelId];

    return modelP->motId[motNo];
}

void mbObjMotionShiftSet(MBMODELID modelId, int motNo, float start, float end, u32 attr)
{
    MBOBJMODEL *modelP = &objManData[modelId];

    modelP->motStart = 0.0f;
    if (modelP->charNo == CHARNO_NONE) {
        modelP->motEnd = Hu3DMotionMaxTimeGet(modelP->modelId);
    } else {
        modelP->motEnd = CharMotionMaxTimeGet(modelP->charNo);
    }
    if (modelP->charNo == CHARNO_NONE) {
        Hu3DMotionShiftSet(modelP->modelId, modelP->motId[motNo], start, end, attr);
    } else {
        CharMotionShiftSet(modelP->charNo, modelP->motId[motNo], start, end, attr);
    }
    modelP->motNo = motNo;
}

int mbObjMotionShiftIDGet(MBMODELID modelId)
{
    MBOBJMODEL *modelP = &objManData[modelId];

    return Hu3DMotionShiftIDGet(modelP->modelId);
}

void mbObjMotionNoCreate(MBMODELID modelId, int dataNum, int motNo)
{
    MBOBJMODEL *modelP = &objManData[modelId];
    int readDataNum;

    if (modelP->charNo == CHARNO_NONE) {
        if (!GwSystem.curTime) {
            if (DIRNUM(dataNum) == dataDirNight) {
                readDataNum = dataDirDay | FILENUM(dataNum);
                goto read_data;
            }
        } else if (DIRNUM(dataNum) == dataDirDay) {
            readDataNum = dataDirNight | FILENUM(dataNum);
            goto read_data;
        }
        readDataNum = dataNum;
read_data:
        modelP->motData[motNo] = HuDataSelHeapReadNum(readDataNum, HU_MEMNUM_OVL, HEAP_MODEL);
        modelId = Hu3DJointMotion(modelP->modelId, modelP->motData[motNo]);
    } else {
        modelId = CharMotionCreate(modelP->charNo, dataNum);
        CharMotionDataClose(modelP->charNo);
    }
    modelP->motId[motNo] = modelId;
    modelP->motNum++;
}

int mbObjMotionCreate(MBMODELID modelId, int dataNum)
{
    MBOBJMODEL *modelP = &objManData[modelId];
    int motNo;
    int readDataNum;

    for (motNo = 1; motNo < MB_OBJ_MOT_MAX; motNo++) {
        if (modelP->motId[motNo] == HU3D_MOTIONID_NONE) {
            break;
        }
    }
    if (modelP->charNo == CHARNO_NONE) {
        if (!GwSystem.curTime) {
            if (DIRNUM(dataNum) == dataDirNight) {
                readDataNum = dataDirDay | FILENUM(dataNum);
                goto read_data;
            }
        } else if (DIRNUM(dataNum) == dataDirDay) {
            readDataNum = dataDirNight | FILENUM(dataNum);
            goto read_data;
        }
        readDataNum = dataNum;
read_data:
        modelP->motData[motNo] = HuDataSelHeapReadNum(readDataNum, HU_MEMNUM_OVL, HEAP_MODEL);
        modelId = Hu3DJointMotion(modelP->modelId, modelP->motData[motNo]);
    } else {
        modelId = CharMotionCreate(modelP->charNo, dataNum);
        CharMotionDataClose(modelP->charNo);
    }
    modelP->motId[motNo] = modelId;
    modelP->motNum++;
    return motNo;
}

void mbObjMotionKill(MBMODELID modelId, int motNo)
{
    MBOBJMODEL *modelP = &objManData[modelId];

    if (motNo == modelP->motNo && modelP->motNo != 0) {
        mbObjMotionSet(modelId, 0, HU3D_MOTATTR_NONE);
    }
    if (modelP->motId[motNo] >= 0) {
        if (modelP->charNo == CHARNO_NONE) {
            Hu3DMotionKill(modelP->motId[motNo]);
        } else {
            CharMotionKill(modelP->charNo, modelP->motId[motNo]);
        }
        modelP->motId[motNo] = HU3D_MOTIONID_NONE;
    }
    modelP->motData[motNo] = NULL;
    modelP->motNum--;
}

float mbObjMotionTimeGet(MBMODELID modelId)
{
    MBOBJMODEL *modelP = &objManData[modelId];
    float time;

    if (modelP->charNo == CHARNO_NONE) {
        time = Hu3DData[modelP->modelId].motWork.time;
    } else {
        time = CharMotionTimeGet(modelP->charNo);
    }
    return time;
}

void mbObjMotionTimeSet(MBMODELID modelId, float time)
{
    MBOBJMODEL *modelP = &objManData[modelId];

    if (modelP->charNo == CHARNO_NONE) {
        Hu3DMotionTimeSet(modelP->modelId, time);
    } else {
        CharMotionTimeSet(modelP->charNo, time);
    }
}

float mbObjMotionMaxTimeGet(MBMODELID modelId)
{
    MBOBJMODEL *modelP = &objManData[modelId];
    float time;

    if (modelP->charNo == CHARNO_NONE) {
        time = Hu3DMotionMaxTimeGet(modelP->modelId);
    } else {
        time = CharMotionMaxTimeGet(modelP->charNo);
    }
    return time;
}

float mbObjMotionSpeedGet(MBMODELID modelId)
{
    HU3D_MODEL *model3DP;
    MBOBJMODEL *modelP = &objManData[modelId];

    model3DP = &Hu3DData[modelP->modelId];

    return model3DP->motWork.speed;
}

void mbObjMotionSpeedSet(MBMODELID modelId, float speed)
{
    MBOBJMODEL *modelP = &objManData[modelId];
    HU3D_MODEL *model3DP;

    if (modelP->charNo == CHARNO_NONE) {
        Hu3DMotionSpeedSet(modelP->modelId, speed);
    } else {
        CharMotionSpeedSet(modelP->charNo, speed);
    }
    model3DP = &Hu3DData[modelP->modelId];
    model3DP->motShiftWork.speed = speed;
}

BOOL mbObjMotionEndCheck(MBMODELID modelId)
{
    MBOBJMODEL *modelP = &objManData[modelId];
    BOOL result;

    if (modelP->charNo == CHARNO_NONE) {
        result = Hu3DMotionEndCheck(modelP->modelId);
    } else {
        result = CharMotionEndCheck(modelP->charNo);
    }
    return result;
}

void mbObjMotionStartEndSet(MBMODELID modelId, s16 start, s16 end)
{
    MBOBJMODEL *modelP = &objManData[modelId];

    if (start >= 0) {
        modelP->motStart = start;
    }
    if (end >= 0) {
        modelP->motEnd = end;
    }
    Hu3DMotionStartEndSet(modelP->modelId, modelP->motStart, modelP->motEnd);
}

void mbObjMotionLoopSet(MBMODELID modelId, BOOL loopF)
{
    MBOBJMODEL *modelP = &objManData[modelId];

    if (loopF) {
        Hu3DModelAttrSet(modelP->modelId, HU3D_MOTATTR_LOOP);
    } else {
        Hu3DModelAttrReset(modelP->modelId, HU3D_MOTATTR_LOOP);
    }
}

float mbObjMotionShapeTimeGet(MBMODELID modelId)
{
    MBOBJMODEL *modelP = &objManData[modelId];
    HU3D_MODEL *model3DP = &Hu3DData[modelP->modelId];

    return model3DP->motShapeWork.time;
}

void mbObjMotionShapeSet(MBMODELID modelId, int motNo, u32 attr)
{
    MBOBJMODEL *modelP = &objManData[modelId];
    HU3D_MODEL *model3DP = &Hu3DData[modelP->modelId];

    model3DP->motShapeWork.speed = 1.0f;
    Hu3DMotionShapeSet(modelP->modelId, modelP->motId[motNo]);
    if (attr & HU3D_MOTATTR_SHAPE_REV) {
        model3DP->motShapeWork.time = Hu3DMotionMaxTimeGet(modelP->modelId);
    }
    Hu3DModelAttrSet(modelP->modelId, attr);
}

void mbObjMotionShapeTimeSet(MBMODELID modelId, float time)
{
    MBOBJMODEL *modelP = &objManData[modelId];
    HU3D_MODEL *model3DP = &Hu3DData[modelP->modelId];

    model3DP->motShapeWork.time = time;
}

float mbObjMotionShapeMaxTimeGet(MBMODELID modelId)
{
    MBOBJMODEL *modelP = &objManData[modelId];
    HU3D_MODEL *model3DP = &Hu3DData[modelP->modelId];

    model3DP = &Hu3DData[modelP->modelId];
    return model3DP->motShapeWork.end;
}

void mbObjMotionShapeSpeedSet(MBMODELID modelId, float speed)
{
    MBOBJMODEL *modelP = &objManData[modelId];
    HU3D_MODEL *model3DP = &Hu3DData[modelP->modelId];

    model3DP->motShapeWork.speed = speed;
}

float mbObjMotionShapeSpeedGet(MBMODELID modelId)
{
    HU3D_MODEL *model3DP;
    MBOBJMODEL *modelP = &objManData[modelId];

    model3DP = &Hu3DData[modelP->modelId];

    return model3DP->motShapeWork.speed;
}

BOOL mbObjMotionShapeEndCheck(MBMODELID modelId)
{
    MBOBJMODEL *modelP = &objManData[modelId];
    HU3D_MODEL *model3DP = &Hu3DData[modelP->modelId];

    if (model3DP->attr & HU3D_MOTATTR_SHAPE_REV) {
        return model3DP->motShapeWork.time <= 0.0f;
    }
    return mbObjMotionShapeMaxTimeGet(modelId) <= model3DP->motShapeWork.time;
}

void mbObjMotionShapeStartEndSet(MBMODELID modelId, s16 start, s16 end)
{
    MBOBJMODEL *modelP = &objManData[modelId];

    if (start >= 0) {
        modelP->motStart = start;
    }
    if (end >= 0) {
        modelP->motEnd = end;
    }
    Hu3DMotionShapeStartEndSet(modelP->modelId, modelP->motStart, modelP->motEnd);
}

void mbObjShadowSet(MBMODELID modelId)
{
    MBOBJMODEL *modelP = &objManData[modelId];

    Hu3DModelShadowSet(modelP->modelId);
}

void mbObjShadowReset(MBMODELID modelId)
{
    MBOBJMODEL *modelP = &objManData[modelId];

    Hu3DModelShadowReset(modelP->modelId);
}

void mbObjShadowMapSet(MBMODELID modelId)
{
    MBOBJMODEL *modelP = &objManData[modelId];

    Hu3DModelShadowMapSet(modelP->modelId);
}

void mbObjShadowMapReset(MBMODELID modelId)
{
    MBOBJMODEL *modelP = &objManData[modelId];

    Hu3DModelShadowMapReset(modelP->modelId);
}

int mbObjStub0(void)
{
    return 0;
}
