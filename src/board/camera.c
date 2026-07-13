#define _MATH_H
#define M_PI 3.141592653589793
double sin(double);
double cos(double);
#pragma cplusplus on
extern inline double fabs(double x)
{
    return __fabs(x);
}
#pragma cplusplus reset

#include "game/board/main.h"
#include "game/board/camera.h"

#include "game/hu3d.h"
#include "game/audio.h"
#include "game/flag.h"
#include "game/frand.h"
#include "game/object.h"
#include "game/process.h"

#include "humath.h"

#define FLAG_BOARD_CAMERAMOT FLAGNUM(FLAG_GROUP_COMMON, 26)

#define MB_CAMERA_STACK_MAX 16

#define MBNO_MAX 11

typedef struct listenerParam_s {
    float sndDist;
    float sndSpeed;
    float startDis;
    float frontSurDis;
    float backSurDis;
} LISTENERPARAM;

typedef struct listenerParamEntry_s {
    int no;
    LISTENERPARAM param;
} LISTENERPARAMENTRY;

BOOL mbPauseEnableCheck(void);
void mbObjPosGet(MBMODELID modelId, HuVecF *pos);
void mbMasuPosGet(s16 masuId, HuVecF *pos);
void mbPlayerPosGet(s16 playerNo, HuVecF *pos);
MBMODELID mbPlayerObjIDGet(int playerNo);
float mbAngleLerp(float a, float b, float t);

static inline int BoardNoGet(void)
{
    return GwSystem.boardNo;
}

static void CameraOMExec(OMOBJ *obj);
static void Camera0LayerHook(s16 layerNo);
static void Camera1LayerHook(s16 layerNo);
static void Camera2LayerHook(s16 layerNo);
static void CameraMotionMain(MBCAMERA *cameraP);
static BOOL CameraFocusCenterCalc(HuVecF *center, HuVecF *focusPos);
static float CameraCanterCalc(HuVecF *rot, float fov, HuVecF *center);
static void CameraMotionSet(MBCAMERA *cameraP);
static void CameraLookAt(MBCAMERA *cameraP);
static void CameraMoveApply(HuVecF *pos, HuVecF *rot, HuVecF *offset, float zoom, float fov, s16 maxTime);

static MBVIEW viewData[] = {
    { -35, 1600, 30 },
    { -35, 2000, 30 },
    { -35, 2400, 30 },
};

static MBCAMERA cameraCur;
static MBCAMERA cameraStack[MB_CAMERA_STACK_MAX];

static OMOBJ *cameraOMObj;
static s16 lightTypeBackup[2];
static s16 cameraStackLevel;

void mbCameraInit(void)
{
    MBCAMERA *cameraP = mbCameraGet();
    const LISTENERPARAMENTRY listenerParamTbl[MBNO_MAX] = {
        { 0, { 8000, 1000, 0, 4000, 4000 } },
        { 1, { 8000, 1000, 0, 4000, 4000 } },
        { 2, { 8000, 1000, 0, 4000, 4000 } },
        { 3, { 8000, 1000, 0, 4000, 4000 } },
        { 4, { 8000, 1000, 0, 4000, 4000 } },
        { 5, { 8000, 1000, 0, 4000, 4000 } },
        { 6, { 8000, 1000, 0, 4000, 4000 } },
        { 7, { 8000, 1000, 0, 4000, 4000 } },
        { 8, { 8000, 1000, 0, 4000, 4000 } },
        { 9, { 8000, 1000, 0, 4000, 4000 } },
        { 10, { 8000, 1000, 0, 4000, 4000 } },
    };
    const LISTENERPARAM *listenerParam;
    HU3D_CAMERA *cam;
    HuVecF pos;
    HuVecF dir;
    int i;
    memset(&cameraStack[0], 0, sizeof(cameraStack));
    memset(&cameraCur, 0, sizeof(cameraCur));
    cameraP->attr = 0;
    cameraP->fov = 30;
    cameraP->near = 100;
    cameraP->far = 20000;
    cameraP->aspect = 1.2f;
    cameraP->viewportX = 0;
    cameraP->viewportY = 0;
    cameraP->viewportH = 480;
    cameraP->viewportW = 640;
    cameraP->viewportNear = 0;
    cameraP->viewportFar = 1;
    cameraP->up.x = cameraP->up.z = 0;
    cameraP->up.y = 1;
    cameraP->quakeTime = 0;
    cameraP->eye.x = cameraP->eye.y = cameraP->eye.z = 0;
    cameraP->center.x = cameraP->center.y = cameraP->center.z = 0;
    cameraP->offset.x = cameraP->offset.y = cameraP->offset.z = 0;
    cameraP->lookAtFunc = CameraLookAt;
    cameraP->zoom = viewData[2].zoom;
    cameraP->rot.x = viewData[2].rotX;
    cameraP->rot.y = cameraP->rot.z = 0;
    cameraP->bit = HU3D_CAM0 | HU3D_CAM1;
    cameraP->speed = 0.15f;
    cameraP->focusNum = 0;
    memset(&cameraP->focus[0], 0, MB_CAMERA_FOCUS_MAX * sizeof(MBCAMERAFOCUS));
    cameraP->curveType = MB_CAMERA_CURVE_LINEAR;
    cameraP->time = 0;
    cameraP->maxTime = -1;
    Hu3DNoSyncSet(0);
    Hu3DCameraCreate(cameraP->bit | HU3D_CAM2);
    cam = &Hu3DCamera[2];
    cam->pos.x = cam->pos.y = cam->pos.z = 0;
    cam->target.z = -100;
    Hu3DCameraScissorSet(cameraP->bit, cameraP->viewportX, cameraP->viewportY, cameraP->viewportW, cameraP->viewportH);
    cameraP->center.x = cameraP->center.y = cameraP->center.z = 0;
    for(i = 0; i < MBNO_MAX; i++) {
        if(listenerParamTbl[i].no == BoardNoGet()) {
            break;
        }
    }
    if(i >= MBNO_MAX) {
        i = 0;
    }
    listenerParam = &listenerParamTbl[i].param;
    pos.x = pos.y = 0;
    pos.z = 100000;
    dir.x = 0;
    dir.y = 0;
    dir.z = -1;
    VECNormalize(&dir, &dir);
    HuAudFXListnerSetEX(&pos, &dir, listenerParam->sndDist, listenerParam->sndSpeed, listenerParam->startDis,
                       listenerParam->frontSurDis, listenerParam->backSurDis);
    cameraOMObj = omAddObjEx(mbObjMan, 32256, 0, 0, OM_GRP_NONE, CameraOMExec);
    Hu3DCameraLayerHookSet(HU3D_CAM0, 0, Camera0LayerHook);
    Hu3DCameraLayerHookSet(HU3D_CAM1, 0, Camera1LayerHook);
    Hu3DCameraLayerHookSet(HU3D_CAM2, 0, Camera2LayerHook);
    omSetStatBit(cameraOMObj, OM_STAT_NOPAUSE);
    lightTypeBackup[0] = lightTypeBackup[1] = -1;
}

static void Camera0LayerHook(s16 layerNo)
{
    HU3D_LIGHT *light = &Hu3DGlobalLight[0];
    if(lightTypeBackup[0] != -1) {
        light->type = lightTypeBackup[0];
    }
    light = &Hu3DGlobalLight[1];
    if(light->type != -1) {
        lightTypeBackup[1] = light->type;
        light->type = -1;
    }
}

static void Camera1LayerHook(s16 layerNo)
{
    HU3D_LIGHT *light = &Hu3DGlobalLight[0];
    if(light->type != -1) {
        lightTypeBackup[0] = light->type;
        light->type = -1;
    }
    light = &Hu3DGlobalLight[1];
    if(lightTypeBackup[1] != -1) {
        light->type = lightTypeBackup[1];
    }
    Hu3DZClear();
}

static void Camera2LayerHook(s16 layerNo)
{
    Hu3DZClear();
}

static void CameraOMExec(OMOBJ *obj)
{
    MBCAMERA *cameraP = mbCameraGet();
    HuVecF dir;
    if(mbExitCheck()) {
        omDelObjEx(HuPrcCurrentGet(), obj);
        return;
    }
    if(mbPauseEnableCheck()) {
        return;
    }
    Hu3DCameraPerspectiveSet(cameraP->bit, cameraP->fov, cameraP->near, cameraP->far, cameraP->aspect);
    Hu3DCameraViewportSet(cameraP->bit, cameraP->viewportX, cameraP->viewportY, cameraP->viewportW, cameraP->viewportH, cameraP->viewportNear, cameraP->viewportFar);
    CameraMotionMain(cameraP);
    cameraP->lookAtFunc(cameraP);
    cameraP->dispOn = TRUE;
    Hu3DCameraPosSetV(cameraP->bit, &cameraP->eye, &cameraP->up, &cameraP->center);
    VECSubtract(&cameraP->center, &cameraP->eye, &dir);
    VECNormalize(&dir, &dir);
    HuAudFXListnerUpdate(&cameraP->eye, &dir);
}

static void CameraMotionMain(MBCAMERA *cameraP)
{
    HuVecF pos;
    float zoom;
    float time;
    if(_CheckFlag(FLAG_BOARD_CAMERAMOT)) {
        CameraMotionSet(cameraP);
        return;
    }
    if(cameraP->focusNum == 0) {
        return;
    }
    zoom = CameraCanterCalc(&cameraP->rot, cameraP->fov, &pos);
    VECAdd(&pos, &cameraP->offset, &pos);
    time = (cameraP->moveOn) ? cameraP->speed : 1.0f;
    cameraP->center.x += time * (pos.x - cameraP->center.x);
    cameraP->center.y += time * (pos.y - cameraP->center.y);
    cameraP->center.z += time * (pos.z - cameraP->center.z);
    if(zoom < 0.0f) {
        return;
    }
    if(cameraP->moveOn) {
        cameraP->zoom += 0.2f * (zoom - cameraP->zoom);
    } else {
        cameraP->zoom = zoom;
    }
}

static BOOL CameraFocusCenterCalc(HuVecF *center, HuVecF *focusPos)
{
    MBCAMERA *cameraP = mbCameraGet();
    int i;
    HuVecF targetEnd;
    HuVecF targetBegin;
    HuVecF apos;
    if(cameraP->focusNum == 0) {
        return FALSE;
    }
    for(i = 0; i < cameraP->focusNum; i++) {
        switch(cameraP->focus[i].type) {
            case MB_CAMERA_FOCUS_POS:
                apos = cameraP->focus[i].pos;
                break;
            case MB_CAMERA_FOCUS_OBJ:
                mbObjPosGet(cameraP->focus[i].targetId, &apos);
                break;
            case MB_CAMERA_FOCUS_MASU:
                mbMasuPosGet(cameraP->focus[i].targetId, &apos);
                break;
        }
        if(i == 0) {
            targetEnd = targetBegin = apos;
        } else {
            if(apos.x < targetEnd.x) {
                targetEnd.x = apos.x;
            } else if(apos.x > targetBegin.x) {
                targetBegin.x = apos.x;
            }
            if(apos.y < targetEnd.y) {
                targetEnd.y = apos.y;
            } else if(apos.y > targetBegin.y) {
                targetBegin.y = apos.y;
            }
            if(apos.z < targetEnd.z) {
                targetEnd.z = apos.z;
            } else if(apos.z > targetBegin.z) {
                targetBegin.z = apos.z;
            }
        }
        if(focusPos) {
            focusPos[i] = apos;
        }
    }
    center->x = targetEnd.x+(0.5f*(targetBegin.x-targetEnd.x));
    center->y = targetEnd.y+(0.5f*(targetBegin.y-targetEnd.y));
    center->z = targetEnd.z+(0.5f*(targetBegin.z-targetEnd.z));
    return TRUE;
}

static float CameraCanterCalc(HuVecF *rot, float fov, HuVecF *center)
{
    MBCAMERA *cameraP = mbCameraGet();
    int i, j;
    HuVecF localCenter;
    HuVecF focusPos[MB_CAMERA_FOCUS_MAX];
    HuVecF dir[2];
    HuVecF ofs2;
    HuVecF ofs1;
    HuVecF rot2;
    HuVecF rot1;
    HuVecF aim;
    float targetDist[2];
    float len;
    float zoom;
    float weight;
    float zoomMax;
    if(rot == NULL) {
        rot = &cameraP->rot;
    }
    if(fov < 0.0f) {
        fov = cameraP->fov;
    }
    if(CameraFocusCenterCalc(&localCenter, focusPos)) {
        *center = localCenter;
    }
    if(cameraP->focusNum < 2) {
        return -1;
    }
    rot2.x = HuSin(rot->y)*HuSin(rot->x);
    rot2.y = HuCos(rot->x);
    rot2.z = HuCos(rot->y)*HuSin(rot->x);
    rot1.x = -HuSin(rot->y)*HuCos(rot->x);
    rot1.y = HuSin(rot->x);
    rot1.z = -HuCos(rot->y)*HuCos(rot->x);
    VECCrossProduct(&rot2, &rot1, &dir[0]);
    VECNormalize(&dir[0], &dir[0]);
    VECCrossProduct(&dir[0], &rot1, &dir[1]);
    VECNormalize(&dir[1], &dir[1]);
    for(i = 0; i < 2; i++) {
        targetDist[i] = -((center->x*dir[i].x)+(center->y*dir[i].y)+(center->z*dir[i].z));
    }
    zoomMax = 0;
    for(i = 0; i < cameraP->focusNum; i++) {
        for(j = 0; j < 2; j++) {
            len = ((dir[j].x*focusPos[i].x)+(dir[j].y*focusPos[i].y)+(dir[j].z*focusPos[i].z))+targetDist[j];
            ofs2.x = focusPos[i].x-(dir[j].x*len);
            ofs2.y = focusPos[i].y-(dir[j].y*len);
            ofs2.z = focusPos[i].z-(dir[j].z*len);
            weight = ((ofs2.z * rot1.z) + (((ofs2.y * rot1.y) + ((ofs2.x * rot1.x) - (center->x * rot1.x))) - (center->y * rot1.y))) - (center->z * rot1.z);
            ofs1.x = center->x+(weight*rot1.x);
            ofs1.y = center->y+(weight*rot1.y);
            ofs1.z = center->z+(weight*rot1.z);
            VECSubtract(&ofs2, &ofs1, &aim);
            len = VECMag(&aim);
            if(j != 0) {
                len /= cameraP->aspect;
            }
            zoom = len/(HuSin(fov*0.5f)/HuCos(fov*0.5f));
            zoom += -weight;
            if(zoom > zoomMax) {
                zoomMax = zoom;
            }
        }
    }
    zoomMax *= 1.3f;
    if(zoomMax < viewData[2].zoom) {
        zoomMax = viewData[2].zoom;
    }
    return zoomMax;
}

static void CameraMotionSet(MBCAMERA *cameraP)
{
    MBCAMERAVIEWKEY *key1 = &cameraP->viewKey[0];
    MBCAMERAVIEWKEY *key2 = &cameraP->viewKey[1];
    float weight;
    float t;
    HuVecF pos;
    if(++cameraP->time >= cameraP->maxTime) {
        _ClearFlag(FLAG_BOARD_CAMERAMOT);
    }
    t = (float)cameraP->time / (float)cameraP->maxTime;
    switch(cameraP->curveType) {
        case MB_CAMERA_CURVE_LINEAR:
            weight = t;
            break;
        case MB_CAMERA_CURVE_SIN:
            weight = HuSin(90.0f * t);
            break;
        case MB_CAMERA_CURVE_SIN2:
            weight = HuSin(90.0f * t);
            weight = weight * weight;
            break;
        case MB_CAMERA_CURVE_COS:
            weight = 1.0 - HuCos(90.0f * t);
            break;
        case MB_CAMERA_CURVE_COS2:
            weight = 1.0 - HuCos(90.0f * t);
            weight = weight * weight;
            break;
    }
    if(!CameraFocusCenterCalc(&pos, NULL)) {
        pos = key2->pos;
    }
    cameraP->offset.x = key1->ofs.x+(weight*(key2->ofs.x-key1->ofs.x));
    cameraP->offset.y = key1->ofs.y+(weight*(key2->ofs.y-key1->ofs.y));
    cameraP->offset.z = key1->ofs.z+(weight*(key2->ofs.z-key1->ofs.z));
    VECAdd(&pos, &cameraP->offset, &pos);
    cameraP->fov = key1->fov+(weight*(key2->fov-key1->fov));
    cameraP->zoom = key1->zoom+(weight*(key2->zoom-key1->zoom));
    cameraP->rot.x = mbAngleLerp(key1->rot.x, key2->rot.x, weight);
    cameraP->rot.y = mbAngleLerp(key1->rot.y, key2->rot.y, weight);
    cameraP->rot.z = mbAngleLerp(key1->rot.z, key2->rot.z, weight);
    cameraP->center.x = key1->pos.x+(weight*(pos.x-key1->pos.x));
    cameraP->center.y = key1->pos.y+(weight*(pos.y-key1->pos.y));
    cameraP->center.z = key1->pos.z+(weight*(pos.z-key1->pos.z));
}

static void CameraLookAt(MBCAMERA *cameraP)
{
    cameraP->eye.x = (HuSin(cameraP->rot.y)*HuCos(cameraP->rot.x)*cameraP->zoom)+cameraP->center.x;
    cameraP->eye.y = (-HuSin(cameraP->rot.x)*cameraP->zoom)+cameraP->center.y;
    cameraP->eye.z = (HuCos(cameraP->rot.y)*HuCos(cameraP->rot.x)*cameraP->zoom)+cameraP->center.z;
    cameraP->up.x = HuSin(cameraP->rot.y)*HuSin(cameraP->rot.x);
    cameraP->up.y = HuCos(cameraP->rot.x);
    cameraP->up.z = HuCos(cameraP->rot.y)*HuSin(cameraP->rot.x);
    if(cameraP->quake) {
        cameraP->eye.x += (frandf()-0.5f)*cameraP->quakePower;
        cameraP->eye.y += (frandf()-0.5f)*cameraP->quakePower;
        cameraP->eye.z += (frandf()-0.5f)*cameraP->quakePower;
        if(--cameraP->quakeTime == 0) {
            cameraP->quake = FALSE;
            cameraP->quakeTime = 0;
            cameraP->quakePower = 0;
        }
    }
}

void mbCameraZoomSet(float zoom)
{
    MBCAMERA *cameraP = mbCameraGet();
    cameraP->zoom = zoom;
}

float mbCameraZoomGet(void)
{
    MBCAMERA *cameraP = mbCameraGet();
    return cameraP->zoom;
}

void mbCameraRotSetV(HuVecF *rot)
{
    MBCAMERA *cameraP = mbCameraGet();
    cameraP->rot.x = rot->x;
    cameraP->rot.y = rot->y;
}

void mbCameraRotSet(float rotX, float rotY, float rotZ)
{
    MBCAMERA *cameraP = mbCameraGet();
    cameraP->rot.x = rotX;
    cameraP->rot.y = rotY;
}

void mbCameraRotGet(HuVecF *rot)
{
    MBCAMERA *cameraP = mbCameraGet();
    *rot = cameraP->rot;
}

void mbCameraCenterSetV(HuVecF *center)
{
    MBCAMERA *cameraP = mbCameraGet();
    cameraP->focusNum = 0;
    cameraP->center.x = center->x;
    cameraP->center.y = center->y;
    cameraP->center.z = center->z;
}

void mbCameraCenterSet(float centerX, float centerY, float centerZ)
{
    MBCAMERA *cameraP = mbCameraGet();
    cameraP->focusNum = 0;
    cameraP->center.x = centerX;
    cameraP->center.y = centerY;
    cameraP->center.z = centerZ;
}

void mbCameraCenterGet(HuVecF *center)
{
    MBCAMERA *cameraP = mbCameraGet();
    *center = cameraP->center;
}

void mbCameraEyeSetV(HuVecF *eye)
{
    MBCAMERA *cameraP = mbCameraGet();
    cameraP->eye.x = eye->x;
    cameraP->eye.y = eye->y;
    cameraP->eye.z = eye->z;
}

void mbCameraEyeSet(float eyeX, float eyeY, float eyeZ)
{
    MBCAMERA *cameraP = mbCameraGet();
    cameraP->eye.x = eyeX;
    cameraP->eye.y = eyeY;
    cameraP->eye.z = eyeZ;
}

void mbCameraEyeGet(HuVecF *eye)
{
    MBCAMERA *cameraP = mbCameraGet();
    *eye = cameraP->eye;
}

void mbCameraOffsetSetV(HuVecF *offset)
{
    MBCAMERA *cameraP = mbCameraGet();
    cameraP->offset = *offset;
}

void mbCameraOffsetSet(float offsetX, float offsetY, float offsetZ)
{
    MBCAMERA *cameraP = mbCameraGet();
    cameraP->offset.x = offsetX;
    cameraP->offset.y = offsetY;
    cameraP->offset.z = offsetZ;
}

void mbCameraOffsetGet(HuVecF *offset)
{
    MBCAMERA *cameraP = mbCameraGet();
    *offset = cameraP->offset;
}

void mbCameraCurveTypeSet(int curveType)
{
    MBCAMERA *cameraP = mbCameraGet();
    if(curveType < 0) {
        cameraP->curveType = 0;
    } else {
        cameraP->curveType = curveType;
    }
}

s16 mbCameraCurveTypeGet(void)
{
    MBCAMERA *cameraP = mbCameraGet();
    return cameraP->curveType;
}

void mbCameraDirGet(HuVecF *dir)
{
    MBCAMERA *cameraP = mbCameraGet();
    VECSubtract(&cameraP->center, &cameraP->eye, dir);
    if(dir->x != 0.0f || dir->y != 0.0f || dir->z != 0.0f) {
        VECNormalize(dir, dir);
    }
}

void mbCameraPosDirGet(HuVecF *pos, HuVecF *dir)
{
    MBCAMERA *cameraP = mbCameraGet();
    VECSubtract(pos, &cameraP->eye, dir);
    if(dir->x != 0.0f || dir->y != 0.0f || dir->z != 0.0f) {
        VECNormalize(dir, dir);
    }
}

void mbCameraLookAtFuncSet(MBCAMERALOOKATFUNC lookAtFunc)
{
    MBCAMERA *cameraP = mbCameraGet();
    if(lookAtFunc != NULL) {
        cameraP->lookAtFunc = lookAtFunc;
    } else {
        cameraP->lookAtFunc = CameraLookAt;
    }
}

void mbCameraBitSet(u16 bit)
{
    MBCAMERA *cameraP = mbCameraGet();
    cameraP->bit = bit;
}

void mbCameraFovSet(float fov)
{
    MBCAMERA *cameraP = mbCameraGet();
    cameraP->fov = fov;
}

void mbCameraScissorSet(int x, int y, int w, int h)
{
    MBCAMERA *cameraP = mbCameraGet();
    Hu3DCameraScissorSet(cameraP->bit, x, y, w, h);
}

void mbCameraNearFarSet(float near, float far)
{
    MBCAMERA *cameraP = mbCameraGet();
    cameraP->near = near;
    cameraP->far = far;
}

void mbCameraNearFarGet(float *near, float *far)
{
    MBCAMERA *cameraP = mbCameraGet();
    if(near) {
        *near = cameraP->near;
    }
    if(far) {
        *far = cameraP->far;
    }
}

void mbCameraLookAtGet(Mtx lookAt)
{
    MBCAMERA *cameraP = mbCameraGet();
    MTXLookAt(lookAt, &cameraP->eye, &cameraP->up, &cameraP->center);
}

void mbCameraLookAtInvGet(Mtx lookAtInv)
{
    MBCAMERA *cameraP = mbCameraGet();
    Mtx temp;
    MTXLookAt(temp, &cameraP->eye, &cameraP->up, &cameraP->center);
    MTXInverse(temp, lookAtInv);
}

MBCAMERA *mbCameraGet(void)
{
    return &cameraCur;
}

int mbCameraStackPush(void)
{
    int i;
    for(i = 0; i < MB_CAMERA_STACK_MAX; i++) {
        if(!cameraStack[i].dispOn) {
            break;
        }
    }
    memcpy(&cameraStack[i], &cameraCur, sizeof(MBCAMERA));
    cameraStackLevel = i + 1;
    return i + 1;
}

void mbCameraStackIdxSet(s16 idx, int maxTime)
{
    MBCAMERA *cameraP = &cameraStack[idx - 1];
    int i;
    cameraP->dispOn = FALSE;
    if(cameraP->focusNum == 0) {
        CameraMoveApply(&cameraP->center, &cameraP->rot, &cameraP->offset, cameraP->zoom, cameraP->fov, (s16)maxTime);
    } else {
        CameraMoveApply(NULL, &cameraP->rot, &cameraP->offset, cameraP->zoom, cameraP->fov, (s16)maxTime);
    }
    cameraCur.focusNum = cameraP->focusNum;
    for(i = 0; i < cameraCur.focusNum; i++) {
        cameraCur.focus[i] = cameraP->focus[i];
    }
}

void mbCameraStackPop(int maxTime)
{
    s16 idx = cameraStackLevel;
    MBCAMERA *cameraP = &cameraStack[idx - 1];
    int i;
    cameraP->dispOn = FALSE;
    if(cameraP->focusNum == 0) {
        CameraMoveApply(&cameraP->center, &cameraP->rot, &cameraP->offset, cameraP->zoom, cameraP->fov, (s16)maxTime);
    } else {
        CameraMoveApply(NULL, &cameraP->rot, &cameraP->offset, cameraP->zoom, cameraP->fov, (s16)maxTime);
    }
    cameraCur.focusNum = cameraP->focusNum;
    for(i = 0; i < cameraCur.focusNum; i++) {
        cameraCur.focus[i] = cameraP->focus[i];
    }
}

BOOL mbCameraCullCheck(HuVecF *pos, float radius)
{
    MBCAMERA *cameraP = &cameraCur;
    Mtx lookAt;
    HuVecF viewPos;
    float tanHalfFov;
    float boundX;
    float boundY;
    float r;
    if(!cameraP->dispOn) {
        return FALSE;
    }
    MTXLookAt(lookAt, &cameraP->eye, &cameraP->up, &cameraP->center);
    PSMTXMultVec(lookAt, pos, &viewPos);
    tanHalfFov = HuSin(cameraP->fov*0.5f)/HuCos(cameraP->fov*0.5f);
    boundX = 1.2f*(tanHalfFov*-viewPos.z);
    boundY = tanHalfFov*-viewPos.z;
    r = radius/HuCos(cameraP->fov*0.5f);
    if(fabs(viewPos.x)-r < boundX) {
        if(fabs(viewPos.y)-r < boundY) {
            if((viewPos.z-radius) <= 0.0f) {
                return TRUE;
            }
        }
    }
    return FALSE;
}

void mbCameraMoveOnSet(BOOL moveOn)
{
    MBCAMERA *cameraP = mbCameraGet();
    cameraP->moveOn = moveOn;
}

void mbCameraSpeedSet(float speed)
{
    MBCAMERA *cameraP = mbCameraGet();
    if(speed < 0.0f) {
        cameraP->speed = 0.15f;
    } else {
        cameraP->speed = speed;
    }
}

void mbCameraFocusReset(void)
{
    MBCAMERA *cameraP = mbCameraGet();
    cameraP->focusNum = 0;
}

void mbCameraFocusPosSet(HuVecF *pos)
{
    MBCAMERA *cameraP = mbCameraGet();
    mbCameraFocusReset();
    mbCameraFocusPosAdd(pos);
}

void mbCameraFocusPosAdd(HuVecF *pos)
{
    MBCAMERA *cameraP = mbCameraGet();
    cameraP->focus[cameraP->focusNum].type = MB_CAMERA_FOCUS_POS;
    cameraP->focus[cameraP->focusNum].pos = *pos;
    cameraP->focusNum++;
    if(!_CheckFlag(FLAG_BOARD_CAMERAMOT)) {
        mbCameraOffsetSet(0, 0, 0);
    }
    mbCameraMoveOnSet(TRUE);
    mbCameraSpeedSet(0.15f);
}

void mbCameraFocusPlayerSet(int playerNo)
{
    MBCAMERA *cameraP = mbCameraGet();
    mbCameraFocusReset();
    if(playerNo >= 0) {
        mbCameraFocusPlayerAdd(playerNo);
    }
}

void mbCameraFocusPlayerAdd(int playerNo)
{
    mbCameraFocusObjAdd(mbPlayerObjIDGet(playerNo));
    if(!_CheckFlag(FLAG_BOARD_CAMERAMOT)) {
        mbCameraOffsetSet(0, 100, 0);
    }
    mbCameraMoveOnSet(TRUE);
    mbCameraSpeedSet(0.15f);
}

void mbCameraFocusObjSet(MBMODELID modelId)
{
    MBCAMERA *cameraP = mbCameraGet();
    mbCameraFocusReset();
    if(modelId >= 0) {
        mbCameraFocusObjAdd(modelId);
    }
}

void mbCameraFocusObjAdd(MBMODELID modelId)
{
    MBCAMERA *cameraP = mbCameraGet();
    cameraP->focus[cameraP->focusNum].type = MB_CAMERA_FOCUS_OBJ;
    cameraP->focus[cameraP->focusNum].targetId = modelId;
    cameraP->focusNum++;
    if(!_CheckFlag(FLAG_BOARD_CAMERAMOT)) {
        mbCameraOffsetSet(0, 0, 0);
    }
    mbCameraMoveOnSet(TRUE);
    mbCameraSpeedSet(0.15f);
}

void mbCameraFocusMasuSet(int masuId)
{
    MBCAMERA *cameraP = mbCameraGet();
    mbCameraFocusReset();
    if(masuId >= 0) {
        mbCameraFocusMasuAdd(masuId);
    }
}

void mbCameraFocusMasuAdd(int masuId)
{
    MBCAMERA *cameraP = mbCameraGet();
    cameraP->focus[cameraP->focusNum].type = MB_CAMERA_FOCUS_MASU;
    cameraP->focus[cameraP->focusNum].targetId = masuId;
    cameraP->focusNum++;
    if(!_CheckFlag(FLAG_BOARD_CAMERAMOT)) {
        mbCameraOffsetSet(0, 0, 0);
    }
    mbCameraMoveOnSet(TRUE);
    mbCameraSpeedSet(0.15f);
}

static void CameraMoveApply(HuVecF *pos, HuVecF *rot, HuVecF *offset, float zoom, float fov, s16 maxTime)
{
    MBCAMERA *cameraP = mbCameraGet();
    MBCAMERAVIEWKEY *key1 = &cameraP->viewKey[0];
    MBCAMERAVIEWKEY *key2 = &cameraP->viewKey[1];
    HuVecF rotTmp;
    HuVecF posTmp;
    HuVecF ofsTmp;
    mbCameraCurveTypeSet(MB_CAMERA_CURVE_LINEAR);
    key1->fov = cameraP->fov;
    key1->zoom = cameraP->zoom;
    key1->rot = cameraP->rot;
    key1->pos = cameraP->center;
    key1->ofs = cameraP->offset;
    key2->fov = (fov < 0.0f) ? key1->fov : fov;
    key2->zoom = (zoom < 0.0f) ? key1->zoom : zoom;
    if(!rot) {
        rotTmp = key1->rot;
    } else {
        rotTmp = *rot;
    }
    key2->rot = rotTmp;
    if(!pos) {
        posTmp = key1->pos;
    } else {
        posTmp = *pos;
    }
    key2->pos = posTmp;
    if(!offset) {
        ofsTmp = key1->ofs;
    } else {
        ofsTmp = *offset;
    }
    key2->ofs = ofsTmp;
    _SetFlag(FLAG_BOARD_CAMERAMOT);
    cameraP->time = 0;
    if(maxTime < 1) {
        maxTime = 1;
    }
    cameraP->maxTime = maxTime;
}

static inline void CameraMoveApplyI(HuVecF *pos, HuVecF *rot, HuVecF *offset, float zoom, float fov, s16 maxTime)
{
    MBCAMERA *cameraP = mbCameraGet();
    MBCAMERAVIEWKEY *key1 = &cameraP->viewKey[0];
    MBCAMERAVIEWKEY *key2 = &cameraP->viewKey[1];
    HuVecF ofsTmp;
    HuVecF posTmp;
    HuVecF rotTmp;
    mbCameraCurveTypeSet(MB_CAMERA_CURVE_LINEAR);
    key1->fov = cameraP->fov;
    key1->zoom = cameraP->zoom;
    key1->rot = cameraP->rot;
    key1->pos = cameraP->center;
    key1->ofs = cameraP->offset;
    key2->fov = (fov < 0.0f) ? key1->fov : fov;
    key2->zoom = (zoom < 0.0f) ? key1->zoom : zoom;
    if(!rot) {
        rotTmp = key1->rot;
    } else {
        rotTmp = *rot;
    }
    key2->rot = rotTmp;
    if(!pos) {
        posTmp = key1->pos;
    } else {
        posTmp = *pos;
    }
    key2->pos = posTmp;
    if(!offset) {
        ofsTmp = key1->ofs;
    } else {
        ofsTmp = *offset;
    }
    key2->ofs = ofsTmp;
    _SetFlag(FLAG_BOARD_CAMERAMOT);
    cameraP->time = 0;
    if(maxTime < 1) {
        maxTime = 1;
    }
    cameraP->maxTime = maxTime;
}

void mbCameraMovePos(HuVecF *pos, HuVecF *rot, HuVecF *offset, float zoom, float fov, s16 maxTime)
{
    HuVecF ofs;
    if(!offset) {
        ofs.x = ofs.y = ofs.z = 0;
    } else {
        ofs = *offset;
    }
    CameraMoveApplyI(pos, rot, (HuVecF *)&ofs, zoom, fov, maxTime);
    if(pos) {
        mbCameraFocusPosSet(pos);
    }
}

void mbCameraMovePlayer(s16 playerNo, HuVecF *rot, HuVecF *offset, float zoom, float fov, s16 maxTime)
{
    HuVecF pos;
    HuVecF ofs;
    if(playerNo == -1) {
        CameraMoveApplyI(NULL, rot, offset, zoom, fov, maxTime);
    } else {
        mbPlayerPosGet(playerNo, &pos);
        if(!offset) {
            ofs.x = ofs.z = 0;
            ofs.y = 100;
        } else {
            ofs = *offset;
        }
        CameraMoveApplyI((HuVecF *)&pos, rot, (HuVecF *)&ofs, zoom, fov, maxTime);
        mbCameraFocusPlayerSet(playerNo);
    }
}

void mbCameraMoveObj(MBMODELID modelId, HuVecF *rot, HuVecF *offset, float zoom, float fov, s16 maxTime)
{
    HuVecF pos;
    HuVecF ofs;
    if(modelId == -1) {
        CameraMoveApplyI(NULL, rot, offset, zoom, fov, maxTime);
    } else {
        mbObjPosGet(modelId, &pos);
        if(!offset) {
            ofs.x = ofs.y = ofs.z = 0;
        } else {
            ofs = *offset;
        }
        CameraMoveApplyI((HuVecF *)&pos, rot, (HuVecF *)&ofs, zoom, fov, maxTime);
        mbCameraFocusObjSet(modelId);
    }
}

void mbCameraMoveMasu(s16 masuId, HuVecF *rot, HuVecF *offset, float zoom, float fov, s16 maxTime)
{
    HuVecF pos;
    HuVecF ofs;
    if(masuId == -1) {
        CameraMoveApplyI(NULL, rot, offset, zoom, fov, maxTime);
    } else {
        mbMasuPosGet(masuId, &pos);
        if(!offset) {
            ofs.x = ofs.y = ofs.z = 0;
        } else {
            ofs = *offset;
        }
        CameraMoveApplyI((HuVecF *)&pos, rot, (HuVecF *)&ofs, zoom, fov, maxTime);
        mbCameraFocusMasuSet(masuId);
    }
}

BOOL mbCameraMoveCheck(void)
{
    return _CheckFlag(FLAG_BOARD_CAMERAMOT) ? FALSE : TRUE;
}

void mbCameraMoveWait(void)
{
    while(!mbCameraMoveCheck()) {
        HuPrcVSleep();
    }
    HuPrcVSleep();
}

void mbCameraMoveStop(void)
{
    MBCAMERA *cameraP = mbCameraGet();
    if(_CheckFlag(FLAG_BOARD_CAMERAMOT)) {
        _ClearFlag(FLAG_BOARD_CAMERAMOT);
        cameraP->fov = cameraP->viewKey[1].fov;
        cameraP->zoom = cameraP->viewKey[1].zoom;
        cameraP->rot = cameraP->viewKey[1].rot;
        cameraP->center = cameraP->viewKey[1].pos;
        cameraP->offset = cameraP->viewKey[1].ofs;
        cameraP->time = cameraP->maxTime;
    }
}

void mbCameraShakeSet(int maxTime, float power)
{
    MBCAMERA *cameraP = mbCameraGet();
    cameraP->quake = TRUE;
    cameraP->quakePower = power;
    cameraP->quakeTime = maxTime;
}

void mbCameraShakeReset(void)
{
    MBCAMERA *cameraP = mbCameraGet();
    cameraP->quake = FALSE;
    cameraP->quakePower = 0;
    cameraP->quakeTime = 0;
}

void mbCameraPlayerViewSetTime(int playerNo, int viewNo, int maxTime)
{
    MBCAMERA *cameraP = mbCameraGet();
    HuVecF rot;
    rot.x = viewData[viewNo].rotX;
    rot.y = rot.z = 0;
    mbCameraMovePlayer(playerNo, &rot, NULL, viewData[viewNo].zoom, viewData[viewNo].fov, maxTime);
}

void mbCameraPlayerViewSet(int playerNo, int viewNo)
{
    mbCameraPlayerViewSetTime(playerNo, viewNo, 21);
}

void mbCameraPlayerViewSetFast(int playerNo, int viewNo)
{
    mbCameraPlayerViewSetTime(playerNo, viewNo, -1);
}

void mbCameraPlayerViewGet(int viewNo, MBVIEW *viewP)
{
    memcpy(viewP, &viewData[viewNo], sizeof(MBVIEW));
}

int mbCameraPlayerViewNoGet(void)
{
    MBCAMERA *cameraP = mbCameraGet();
    int i;
    for(i = 0; i < 3; i++) {
        if(cameraP->zoom <= viewData[i].zoom) {
            break;
        }
    }
    if(i >= 3) {
        return 0;
    } else {
        return i;
    }
}

float mbCameraPlayerViewZoomGet(int viewNo)
{
    return viewData[viewNo].zoom;
}

float mbCameraPlayerViewRotXGet(int viewNo)
{
    return viewData[viewNo].rotX;
}

void mbCameraMultiFocusSet(void)
{
}
