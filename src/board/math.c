#include "game/board/camera.h"
#include "game/disp.h"
#include "game/hu3d.h"
#include "game/memory.h"

#include "humath.h"

#define MB_TRIG_TABLE_COUNT 2048
#define MB_TRIG_TABLE_BYTES (MB_TRIG_TABLE_COUNT * sizeof(float))
#define MB_TRIG_BYTE_MASK (MB_TRIG_TABLE_BYTES - sizeof(float))
#define MB_TRIG_DEG_SCALE (MB_TRIG_TABLE_BYTES / 360.0f)
#define MB_TRIG_RAD_SCALE 1303.7972412109375f
#define MB_TRIG_COS_INDEX(angle, scale) \
    ((((s32)((angle) * (scale)) + 2) & MB_TRIG_BYTE_MASK) >> 2)
#define MB_TRIG_SIN_INDEX(angle, scale) \
    ((((s32)((angle) * (scale)) - 2046) & MB_TRIG_BYTE_MASK) >> 2)

static float *cosTab;

void mbMathInit(void)
{
    s32 i;

    cosTab = HuMemDirectMallocNum(HEAP_HEAP, MB_TRIG_TABLE_BYTES, HU_MEMNUM_OVL);
    for (i = 0; i < MB_TRIG_TABLE_COUNT; i++) {
        cosTab[i] = HuCos((360.0f / MB_TRIG_TABLE_COUNT) * i);
    }
}

void mbMathClose(void)
{
    if (cosTab != NULL) {
        HuMemDirectFree(cosTab);
        cosTab = NULL;
    }
}

float mbCosDeg(float deg)
{
    return cosTab[MB_TRIG_COS_INDEX(deg, MB_TRIG_DEG_SCALE)];
}

float mbCosRad(float rad)
{
    return cosTab[MB_TRIG_COS_INDEX(rad, MB_TRIG_RAD_SCALE)];
}

float mbSinDeg(float deg)
{
    return cosTab[MB_TRIG_SIN_INDEX(deg, MB_TRIG_DEG_SCALE)];
}

float mbSinRad(float rad)
{
    return cosTab[MB_TRIG_SIN_INDEX(rad, MB_TRIG_RAD_SCALE)];
}

void mbMtxRotTrigX(Mtx mtx, float sin, float cos)
{
    float y;
    float z;
    s32 i;

    for (i = 0; i < 4; i++) {
        y = mtx[1][i];
        z = mtx[2][i];
        mtx[1][i] = (cos * y) - (sin * z);
        mtx[2][i] = (sin * y) + (cos * z);
    }
}

void mbMtxRotTrigY(Mtx mtx, float sin, float cos)
{
    float x;
    float z;
    s32 i;

    for (i = 0; i < 4; i++) {
        x = mtx[0][i];
        z = mtx[2][i];
        mtx[0][i] = (cos * x) + (sin * z);
        mtx[2][i] = (-sin * x) + (cos * z);
    }
}

void mbMtxRotTrigZ(Mtx mtx, float sin, float cos)
{
    float x;
    float y;
    s32 i;

    for (i = 0; i < 4; i++) {
        x = mtx[0][i];
        y = mtx[1][i];
        mtx[0][i] = (cos * x) - (sin * y);
        mtx[1][i] = (sin * x) + (cos * y);
    }
}

void mbMtxRotTrigScaleX(Mtx mtx, float sin, float cos, HuVecF *scale)
{
    mtx[0][0] = scale->x;
    mtx[0][1] = 0.0f;
    mtx[0][2] = 0.0f;
    mtx[0][3] = 0.0f;
    mtx[1][0] = 0.0f;
    mtx[1][1] = cos * scale->y;
    mtx[1][2] = -sin * scale->z;
    mtx[1][3] = 0.0f;
    mtx[2][0] = 0.0f;
    mtx[2][1] = sin * scale->y;
    mtx[2][2] = cos * scale->z;
    mtx[2][3] = 0.0f;
}

void mbMtxRotTrigScaleY(Mtx mtx, float sin, float cos, HuVecF *scale)
{
    mtx[0][0] = cos * scale->x;
    mtx[0][1] = 0.0f;
    mtx[0][2] = sin * scale->z;
    mtx[0][3] = 0.0f;
    mtx[1][0] = 0.0f;
    mtx[1][1] = scale->y;
    mtx[1][2] = 0.0f;
    mtx[1][3] = 0.0f;
    mtx[2][0] = -sin * scale->x;
    mtx[2][1] = 0.0f;
    mtx[2][2] = cos * scale->z;
    mtx[2][3] = 0.0f;
}

void mbMtxRotTrigScaleZ(Mtx mtx, float sin, float cos, HuVecF *scale)
{
    mtx[0][0] = cos * scale->x;
    mtx[0][1] = -sin * scale->y;
    mtx[0][2] = 0.0f;
    mtx[0][3] = 0.0f;
    mtx[1][0] = sin * scale->x;
    mtx[1][1] = cos * scale->y;
    mtx[1][2] = 0.0f;
    mtx[1][3] = 0.0f;
    mtx[2][0] = 0.0f;
    mtx[2][1] = 0.0f;
    mtx[2][2] = scale->z;
    mtx[2][3] = 0.0f;
}

void mbMtxRotAxisDeg(Mtx mtx, char axis, float angle)
{
    MTXRotTrig(mtx, axis, mbSinDeg(angle), mbCosDeg(angle));
}

void mbMtxRotAxisRad(Mtx mtx, char axis, float angle)
{
    MTXRotTrig(mtx, axis, mbSinRad(angle), mbCosRad(angle));
}

void mbMtxRotXDeg(Mtx mtx, float angle)
{
    mbMtxRotTrigX(mtx, mbSinDeg(angle), mbCosDeg(angle));
}

void mbMtxRotXRad(Mtx mtx, float angle)
{
    mbMtxRotTrigX(mtx, mbSinRad(angle), mbCosRad(angle));
}

void mbMtxRotYDeg(Mtx mtx, float angle)
{
    mbMtxRotTrigY(mtx, mbSinDeg(angle), mbCosDeg(angle));
}

void mbMtxRotYRad(Mtx mtx, float angle)
{
    mbMtxRotTrigY(mtx, mbSinRad(angle), mbCosRad(angle));
}

void mbMtxRotZDeg(Mtx mtx, float angle)
{
    mbMtxRotTrigZ(mtx, mbSinDeg(angle), mbCosDeg(angle));
}

void mbMtxRotZRad(Mtx mtx, float angle)
{
    mbMtxRotTrigZ(mtx, mbSinRad(angle), mbCosRad(angle));
}

void mbMtxScaleRotXDeg(Mtx mtx, float angle, HuVecF *scale)
{
    mbMtxRotTrigScaleX(mtx, mbSinDeg(angle), mbCosDeg(angle), scale);
}

void mbMtxScaleRotYDeg(Mtx mtx, float angle, HuVecF *scale)
{
    mbMtxRotTrigScaleY(mtx, mbSinDeg(angle), mbCosDeg(angle), scale);
}

void mbMtxScaleRotZDeg(Mtx mtx, float angle, HuVecF *scale)
{
    mbMtxRotTrigScaleZ(mtx, mbSinDeg(angle), mbCosDeg(angle), scale);
}

void mbMtxRot(Mtx mtx, float x, float y, float z)
{
    if (x != 0.0f) {
        mbMtxRotAxisDeg(mtx, 'x', x);
    } else {
        MTXIdentity(mtx);
    }
    if (y != 0.0f) {
        mbMtxRotYDeg(mtx, y);
    }
    if (z != 0.0f) {
        mbMtxRotZDeg(mtx, z);
    }
}

void mbMtxTransCat(Mtx mtx, float x, float y, float z)
{
    mtx[0][3] += x;
    mtx[1][3] += y;
    mtx[2][3] += z;
}

float mbVecMagXZ(HuVecF *a, HuVecF *b)
{
    float dx = a->x - b->x;
    float dz = a->z - b->z;

    return HuMagPoint2D(dx, dz);
}

BOOL mbVecMagXZCheck(HuVecF *a, HuVecF *b, float maxDist)
{
    float dist = mbVecMagXZ(a, b);

    if (dist <= maxDist) {
        return TRUE;
    } else {
        return FALSE;
    }
}

float mbAngleWrap(float angle)
{
    angle = fmod(angle, 360);
    if (angle < -180.0f) {
        angle += 360.0f;
    } else if (angle > 180.0f) {
        angle -= 360.0f;
    }
    return angle;
}

void mbAngleWrapV(HuVecF *angle)
{
    float *dest = (float *)angle;
    int i;

    for (i = 0; i < 3; dest++, i++) {
        *dest = mbAngleWrap(*dest);
    }
}

BOOL mbAngleAdd(float *dest, float angle, float speed)
{
    float wrapAngle = fmod(angle - *dest, 360);
    float diff;

    if (fabs(wrapAngle) < speed) {
        *dest = angle;
        return TRUE;
    }
    if (wrapAngle < 0.0f) {
        wrapAngle += 360.0f;
    }
    if (wrapAngle > 180.0f) {
        diff = -speed;
    } else {
        diff = speed;
    }
    *dest += diff;
    *dest = mbAngleWrap(*dest);
    return FALSE;
}

BOOL mbAngleMoveTo(float *dest, float angle, float speed)
{
    float wrapAngle = fmod(angle - *dest, 360);

    if (fabs(wrapAngle) < 1.0) {
        *dest = angle;
        return TRUE;
    }
    if (wrapAngle < 0.0f) {
        wrapAngle += 360.0f;
    }
    if (wrapAngle > 180.0f) {
        wrapAngle -= 360.0f;
    }
    *dest = fmod(*dest + (speed * wrapAngle), 360.0);
    if (*dest < 0.0f) {
        *dest += 360.0f;
    }
    return FALSE;
}

float mbAngleWrap2(float a, float b)
{
    float angle = fmod(a - b, 360);

    if (angle < 0.0f) {
        angle += 360.0f;
    }
    if (angle >= 180.0f) {
        angle -= 360.0f;
    }
    return angle;
}

BOOL mbVecMagCheck(HuVecF *a, HuVecF *b, float dist)
{
    HuVecF diff;

    VECSubtract(a, b, &diff);
    if (VECSquareMag(&diff) >= dist * dist) {
        return FALSE;
    } else {
        return TRUE;
    }
}

void mbMtxLookAtCalc(Mtx dest, HuVecF *eye, HuVecF *up, HuVecF *target)
{
    HuVecF f;
    HuVecF u;
    HuVecF s;

    f.x = eye->x - target->x;
    f.y = eye->y - target->y;
    f.z = eye->z - target->z;
    VECNormalize(&f, &f);
    VECCrossProduct(up, &f, &u);
    VECNormalize(&u, &u);
    VECCrossProduct(&f, &u, &s);
    dest[0][0] = u.x;
    dest[0][1] = u.y;
    dest[0][2] = u.z;
    dest[0][3] = 0.0f;
    dest[1][0] = s.x;
    dest[1][1] = s.y;
    dest[1][2] = s.z;
    dest[1][3] = 0.0f;
    dest[2][0] = f.x;
    dest[2][1] = f.y;
    dest[2][2] = f.z;
    dest[2][3] = 0.0f;
}

void mbPos3Dto2D(HuVecF *src, HuVecF *dst)
{
    MBCAMERA *cameraP = mbCameraGet();
    float tanFov;
    float width;
    float height;
    Mtx lookAt;
    HuVecF pos;

    MTXLookAt(lookAt, &cameraP->eye, &cameraP->up, &cameraP->center);
    MTXMultVec(lookAt, src, &pos);
    tanFov = mbSinDeg(cameraP->fov * 0.5f) / mbCosDeg(cameraP->fov * 0.5f);
    width = HU_DISP_ASPECT * (tanFov * pos.z);
    height = tanFov * pos.z;
    dst->x = HU_DISP_CENTERX + (pos.x * (HU_DISP_CENTERX / -width));
    dst->y = HU_DISP_CENTERY + (pos.y * (HU_DISP_CENTERY / height));
    dst->z = -pos.z;
}

void mbPos3DtoNorm(HuVecF *src, s16 cameraMask, HuVecF *dst)
{
    HU3D_CAMERA *cameraP;
    float tanFov;
    float height;
    float width;
    Mtx lookAt;
    HuVecF pos;
    s32 cameraNo;

    for (cameraNo = 0; cameraNo < HU3D_CAM_MAX; cameraNo++) {
        if (cameraMask & (1 << cameraNo)) {
            break;
        }
    }
    cameraP = &Hu3DCamera[cameraNo];
    MTXLookAt(lookAt, &cameraP->pos, &cameraP->up, &cameraP->target);
    MTXMultVec(lookAt, src, &pos);
    tanFov = mbSinDeg(cameraP->fov * 0.5f) / mbCosDeg(cameraP->fov * 0.5f);
    height = tanFov * -pos.z;
    width = HU_DISP_ASPECT * height;
    dst->x = pos.x / width;
    dst->y = pos.y / height;
    dst->z = pos.z;
}

void mbPos2Dto3D(HuVecF *src, HuVecF *dst)
{
    MBCAMERA *cameraP = mbCameraGet();
    float tanFov = mbSinDeg(cameraP->fov * 0.5f) / mbCosDeg(cameraP->fov * 0.5f);
    float height = 2.0f * (tanFov * src->z);
    float width = HU_DISP_ASPECT * height;
    float normX = src->x / HU_DISP_WIDTH;
    float normY = src->y / HU_DISP_HEIGHT;
    Mtx lookAt;

    dst->x = (normX - 0.5) * width;
    dst->y = -(normY - 0.5) * height;
    dst->z = -src->z;
    mbCameraLookAtInvGet(lookAt);
    MTXMultVec(lookAt, dst, dst);
}

void mbNormPosto3D(HuVecF *src, s16 cameraMask, HuVecF *dst)
{
    HU3D_CAMERA *cameraP;
    float tanFov;
    float depth;
    Mtx lookAt;
    Mtx lookAtInv;
    s32 cameraNo;

    for (cameraNo = 0; cameraNo < HU3D_CAM_MAX; cameraNo++) {
        if (cameraMask & (1 << cameraNo)) {
            break;
        }
    }
    cameraP = &Hu3DCamera[cameraNo];
    tanFov = mbSinDeg(cameraP->fov * 0.5f) / mbCosDeg(cameraP->fov * 0.5f);
    depth = tanFov * fabs(src->z);
    dst->x = src->x * (HU_DISP_ASPECT * depth);
    dst->y = src->y * depth;
    dst->z = src->z;
    MTXLookAt(lookAt, &cameraP->pos, &cameraP->up, &cameraP->target);
    MTXInverse(lookAt, lookAtInv);
    MTXMultVec(lookAtInv, dst, dst);
}

void mbNormPosto2D(HuVecF *src, HuVecF *dst)
{
    dst->x = HU_DISP_CENTERX * (1.0f + src->x);
    dst->y = HU_DISP_HEIGHT * (src->y - 1.0f);
    dst->z = src->z;
}

float mbBezierCalc(float a, float b, float c, float t)
{
    float invTime = 1.0f - t;

    return (t * t * c) + ((invTime * invTime * a) + (b * ((2.0f * invTime) * t)));
}

void mbBezierCalcV(HuVecF *a, HuVecF *b, HuVecF *c, HuVecF *dst, float t)
{
    dst->x = mbBezierCalc(a->x, b->x, c->x, t);
    dst->y = mbBezierCalc(a->y, b->y, c->y, t);
    dst->z = mbBezierCalc(a->z, b->z, c->z, t);
}

void mbBezierCalcVList(HuVecF *src, HuVecF *dst, float t)
{
    dst->x = mbBezierCalc(src[0].x, src[1].x, src[2].x, t);
    dst->y = mbBezierCalc(src[0].y, src[1].y, src[2].y, t);
    dst->z = mbBezierCalc(src[0].z, src[1].z, src[2].z, t);
}

float mbBezierCalcSlope(float a, float b, float c, float t)
{
    return 2.0f * ((-a + b) + (t * (c + (a - (2.0f * b)))));
}

void mbBezierCalcSlopeV(HuVecF *a, HuVecF *b, HuVecF *c, HuVecF *dst, float t)
{
    dst->x = mbBezierCalcSlope(a->x, b->x, c->x, t);
    dst->y = mbBezierCalcSlope(a->y, b->y, c->y, t);
    dst->z = mbBezierCalcSlope(a->z, b->z, c->z, t);
}

float mbHermiteCalc(float a, float b, float c, float d, float t)
{
    float tt = t * t;
    float ttt = t * t * t;
    float aCoef = 1.0f + ((2.0f * ttt) - (3.0f * tt));
    float bCoef = (-2.0f * ttt) + (3.0f * tt);
    float cCoef = t + (ttt - (2.0f * tt));
    float dCoef = ttt - tt;

    return (aCoef * a) + (bCoef * b) + (cCoef * c) + (dCoef * d);
}

void mbHermiteCalcV(HuVecF *a, HuVecF *b, HuVecF *c, HuVecF *d, HuVecF *dst, float t)
{
    dst->x = mbHermiteCalc(a->x, b->x, c->x, d->x, t);
    dst->y = mbHermiteCalc(a->y, b->y, c->y, d->y, t);
    dst->z = mbHermiteCalc(a->z, b->z, c->z, d->z, t);
}

float mbHermiteCalcSlope(float a, float b, float c, float d, float t)
{
    float tt = t * t;
    float aCoef = (6.0f * tt) - (6.0f * t);
    float bCoef = (-6.0f * tt) + (6.0f * t);
    float cCoef = 1.0f + ((3.0f * tt) - (4.0f * t));
    float dCoef = (3.0f * tt) - (2.0f * t);

    return (aCoef * a) + (bCoef * b) + (cCoef * c) + (dCoef * d);
}

float mbAngleLerp(float a, float b, float t)
{
    float diff = fmod(b - a, 360);
    float ret;

    if (diff < 0.0f) {
        diff += 360.0f;
    }
    if (diff > 180.0f) {
        diff -= 360.0f;
    }
    ret = fmod(a + (t * diff), 360);
    if (ret < 0.0f) {
        ret += 360.0f;
    }
    return ret;
}

float mbAngleEaseOut(float a, float b, float t)
{
    return mbAngleLerp(a, b, HuSin(t * 90.0f));
}

float mbAngleEaseIn(float a, float b, float t)
{
    return mbAngleLerp(a, b, 1.0f - HuCos(t * 90.0f));
}

float mbMathDistScale(HuVecF *src, float scale, HuVecF *dst)
{
    MBCAMERA *cameraP = mbCameraGet();
    HuVecF pos;
    float tanFov;
    float depth;
    float z;

    mbPos3Dto2D(src, &pos);
    tanFov = HuSin(cameraP->fov * 0.5f) / HuCos(cameraP->fov * 0.5f);
    depth = pos.z * tanFov;
    z = (depth / scale) / tanFov;
    pos.z = z;
    mbPos2Dto3D(&pos, dst);
    return 0.0f;
}
