#ifndef _BOARD_CAMERA_H
#define _BOARD_CAMERA_H

#include "game/hu3d.h"

#define MB_CAMERA_FOCUS_MAX 8

#define MB_CAMERA_VIEW_ZOOMIN 0
#define MB_CAMERA_VIEW_WALK 2

#define MB_CAMERA_CURVE_LINEAR 0
#define MB_CAMERA_CURVE_SIN 1
#define MB_CAMERA_CURVE_SIN2 2
#define MB_CAMERA_CURVE_COS 3
#define MB_CAMERA_CURVE_COS2 4

#define MB_CAMERA_FOCUS_POS 0
#define MB_CAMERA_FOCUS_OBJ 1
#define MB_CAMERA_FOCUS_MASU 2

typedef s16 MBMODELID;

typedef struct mbCamera_s MBCAMERA;

typedef void (*MBCAMERALOOKATFUNC)(MBCAMERA *cameraP);

typedef struct mbCameraViewKey_s {
    HuVecF rot;
    HuVecF pos;
    HuVecF ofs;
    float zoom;
    float fov;
} MBCAMERAVIEWKEY;

typedef struct mbCameraFocus_s {
    int type;
    int targetId;
    HuVecF pos;
} MBCAMERAFOCUS;

struct mbCamera_s {
    union {
        struct {
            u8 dispOn : 1;
            u8 moveOn : 1;
            u8 quake : 1;
        };
        u8 attr;
    };
    u16 bit;
    float fov;
    float near;
    float far;
    float aspect;
    float viewportX;
    float viewportY;
    float viewportW;
    float viewportH;
    float viewportNear;
    float viewportFar;
    HuVecF eye;
    HuVecF up;
    HuVecF center;
    HuVecF offset;
    HuVecF rot;
    float zoom;
    float speed;
    int quakeTime;
    float quakePower;
    s16 curveType;
    s16 time;
    s16 maxTime;
    MBCAMERALOOKATFUNC lookAtFunc;
    MBCAMERAVIEWKEY viewKey[2];
    int focusNum;
    MBCAMERAFOCUS focus[MB_CAMERA_FOCUS_MAX];
};

typedef struct mbView_s {
    s16 rotX;
    s16 zoom;
    s16 fov;
} MBVIEW;

void mbCameraInit(void);
void mbCameraZoomSet(float zoom);
float mbCameraZoomGet(void);
void mbCameraRotSetV(HuVecF *rot);
void mbCameraRotSet(float rotX, float rotY, float rotZ);
void mbCameraRotGet(HuVecF *rot);
void mbCameraCenterSetV(HuVecF *center);
void mbCameraCenterSet(float centerX, float centerY, float centerZ);
void mbCameraCenterGet(HuVecF *center);
void mbCameraEyeSetV(HuVecF *eye);
void mbCameraEyeSet(float eyeX, float eyeY, float eyeZ);
void mbCameraEyeGet(HuVecF *eye);
void mbCameraOffsetSetV(HuVecF *offset);
void mbCameraOffsetSet(float offsetX, float offsetY, float offsetZ);
void mbCameraOffsetGet(HuVecF *offset);
void mbCameraCurveTypeSet(int curveType);
s16 mbCameraCurveTypeGet(void);
void mbCameraDirGet(HuVecF *dir);
void mbCameraPosDirGet(HuVecF *pos, HuVecF *dir);
void mbCameraLookAtFuncSet(MBCAMERALOOKATFUNC lookAtFunc);
void mbCameraBitSet(u16 bit);
void mbCameraFovSet(float fov);
void mbCameraScissorSet(int x, int y, int w, int h);
void mbCameraNearFarSet(float near, float far);
void mbCameraNearFarGet(float *near, float *far);
void mbCameraLookAtGet(Mtx lookAt);
void mbCameraLookAtInvGet(Mtx lookAtInv);
MBCAMERA *mbCameraGet(void);
int mbCameraStackPush(void);
void mbCameraStackIdxSet(s16 idx, int maxTime);
void mbCameraStackPop(int maxTime);
BOOL mbCameraCullCheck(HuVecF *pos, float radius);
void mbCameraMoveOnSet(BOOL moveOn);
void mbCameraSpeedSet(float speed);
void mbCameraFocusReset(void);
void mbCameraFocusPosSet(HuVecF *pos);
void mbCameraFocusPosAdd(HuVecF *pos);
void mbCameraFocusPlayerSet(int playerNo);
void mbCameraFocusPlayerAdd(int playerNo);
void mbCameraFocusObjSet(MBMODELID modelId);
void mbCameraFocusObjAdd(MBMODELID modelId);
void mbCameraFocusMasuSet(int masuId);
void mbCameraFocusMasuAdd(int masuId);
void mbCameraMovePos(HuVecF *pos, HuVecF *rot, HuVecF *offset, float zoom, float fov, s16 maxTime);
void mbCameraMovePlayer(s16 playerNo, HuVecF *rot, HuVecF *offset, float zoom, float fov, s16 maxTime);
void mbCameraMoveObj(MBMODELID modelId, HuVecF *rot, HuVecF *offset, float zoom, float fov, s16 maxTime);
void mbCameraMoveMasu(s16 masuId, HuVecF *rot, HuVecF *offset, float zoom, float fov, s16 maxTime);
BOOL mbCameraMoveCheck(void);
void mbCameraMoveWait(void);
void mbCameraMoveStop(void);
void mbCameraShakeSet(int maxTime, float power);
void mbCameraShakeReset(void);
void mbCameraPlayerViewSetTime(int playerNo, int viewNo, int maxTime);
void mbCameraPlayerViewSet(int playerNo, int viewNo);
void mbCameraPlayerViewSetFast(int playerNo, int viewNo);
void mbCameraPlayerViewGet(int viewNo, MBVIEW *viewP);
int mbCameraPlayerViewNoGet(void);
float mbCameraPlayerViewZoomGet(int viewNo);
float mbCameraPlayerViewRotXGet(int viewNo);
void mbCameraMultiFocusSet(void);

#endif
