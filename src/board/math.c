#include "game/board/camera.h"
#include "game/board/object.h"
#include "game/disp.h"
#include "game/frand.h"
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
static HuVecF objectBBox[8];
static HuVecF objectBBoxView[8];

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

u32 mbRandMod(u32 mod)
{
    u32 value = frand();

    value &= 0x7FFFFFFF;
    if (value % 2 != 0) {
        value |= 0x80000000;
    }
    return ((u64)value * mod) >> 32;
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

static void ObjectCullUpdate(HSF_OBJECT *object, Mtx mtx)
{
    HU3D_CAMERA *cameraP;
    HuVecF center;
    HuVecF centerView;
    ROMtx cullMtx;
    float fov;
    float near;
    float aspect;
    float fovTan;
    float cameraH;
    float aspectInv;
    s32 i;
    BOOL cullF;

    object->flags &= ~HSF_MATERIAL_DISPOFF;
    if (shadowModelDrawF == FALSE) {
        cameraP = &Hu3DCamera[Hu3DCameraNo];
        fov = cameraP->fov;
        near = cameraP->near;
        aspect = cameraP->aspect;
    } else {
        fov = Hu3DShadow->fov;
        near = Hu3DShadow->near;
        aspect = 1.0f;
    }
    fovTan = -1.0f * (mbSinDeg(fov * 0.5f) / mbCosDeg(fov * 0.5f));
    PSVECAdd(&object->mesh.mesh.min, &object->mesh.mesh.max, &center);
    PSVECScale(&center, &center, 0.5f);
    PSMTXMultVec(mtx, &center, &centerView);
    cameraH = fabs(centerView.z * fovTan);
    if (fabs(centerView.x) <= cameraH * aspect
        && fabs(centerView.y) <= cameraH
        && centerView.y < -near) {
        return;
    }

    PSMTXReorder(mtx, cullMtx);
    aspectInv = 1.0f / aspect;
    cullMtx[0][0] *= aspectInv;
    cullMtx[1][0] *= aspectInv;
    cullMtx[2][0] *= aspectInv;
    cullMtx[3][0] *= aspectInv;
    cullMtx[0][2] *= fovTan;
    cullMtx[1][2] *= fovTan;
    cullMtx[2][2] *= fovTan;
    cullMtx[3][2] *= fovTan;

    objectBBox[0].x = object->mesh.mesh.max.x;
    objectBBox[0].y = object->mesh.mesh.max.y;
    objectBBox[0].z = object->mesh.mesh.max.z;
    objectBBox[1].x = object->mesh.mesh.max.x;
    objectBBox[1].y = object->mesh.mesh.max.y;
    objectBBox[1].z = object->mesh.mesh.min.z;
    objectBBox[2].x = object->mesh.mesh.max.x;
    objectBBox[2].y = object->mesh.mesh.min.y;
    objectBBox[2].z = object->mesh.mesh.max.z;
    objectBBox[3].x = object->mesh.mesh.max.x;
    objectBBox[3].y = object->mesh.mesh.min.y;
    objectBBox[3].z = object->mesh.mesh.min.z;
    objectBBox[4].x = object->mesh.mesh.min.x;
    objectBBox[4].y = object->mesh.mesh.max.y;
    objectBBox[4].z = object->mesh.mesh.max.z;
    objectBBox[5].x = object->mesh.mesh.min.x;
    objectBBox[5].y = object->mesh.mesh.max.y;
    objectBBox[5].z = object->mesh.mesh.min.z;
    objectBBox[6].x = object->mesh.mesh.min.x;
    objectBBox[6].y = object->mesh.mesh.min.y;
    objectBBox[6].z = object->mesh.mesh.max.z;
    objectBBox[7].x = object->mesh.mesh.min.x;
    objectBBox[7].y = object->mesh.mesh.min.y;
    objectBBox[7].z = object->mesh.mesh.min.z;
    PSMTXROMultVecArray(cullMtx, objectBBox, objectBBoxView, 8);

    for (i = 0; i < 8; i++) {
        if (objectBBoxView[i].z > near) {
            break;
        }
    }
    if (i >= 8) {
        object->flags |= HSF_MATERIAL_DISPOFF;
        return;
    }

    cullF = FALSE;
    if (centerView.x >= 0.0f) {
        for (i = 0; i < 8; i++) {
            if (objectBBoxView[i].x < objectBBoxView[i].z) {
                break;
            }
        }
        if (i >= 8) {
            cullF = TRUE;
        }
    } else {
        for (i = 0; i < 8; i++) {
            if (objectBBoxView[i].x > -objectBBoxView[i].z) {
                break;
            }
        }
        if (i >= 8) {
            cullF = TRUE;
        }
    }
    if (cullF) {
        object->flags |= HSF_MATERIAL_DISPOFF;
        return;
    }

    cullF = FALSE;
    if (centerView.y >= 0.0f) {
        for (i = 0; i < 8; i++) {
            if (objectBBoxView[i].y < objectBBoxView[i].z) {
                break;
            }
        }
        if (i >= 8) {
            cullF = TRUE;
        }
    } else {
        for (i = 0; i < 8; i++) {
            if (objectBBoxView[i].y > -objectBBoxView[i].z) {
                break;
            }
        }
        if (i >= 8) {
            cullF = TRUE;
        }
    }
    if (cullF) {
        object->flags |= HSF_MATERIAL_DISPOFF;
    }
}

#pragma dont_inline on
static void ObjectBBoxUpdate(HSF_OBJECT *object)
{
    HuVecF *vertex = object->mesh.vertex->data;
    s32 i;

    object->mesh.mesh.max.x = -1000000.0f;
    object->mesh.mesh.max.y = -1000000.0f;
    object->mesh.mesh.max.z = -1000000.0f;
    object->mesh.mesh.min.x = 1000000.0f;
    object->mesh.mesh.min.y = 1000000.0f;
    object->mesh.mesh.min.z = 1000000.0f;
    for (i = 0; i < object->mesh.vertex->count; i++, vertex++) {
        if (object->mesh.mesh.max.x < vertex->x) {
            object->mesh.mesh.max.x = vertex->x;
        }
        if (object->mesh.mesh.max.y < vertex->y) {
            object->mesh.mesh.max.y = vertex->y;
        }
        if (object->mesh.mesh.max.z < vertex->z) {
            object->mesh.mesh.max.z = vertex->z;
        }
        if (object->mesh.mesh.min.x > vertex->x) {
            object->mesh.mesh.min.x = vertex->x;
        }
        if (object->mesh.mesh.min.y > vertex->y) {
            object->mesh.mesh.min.y = vertex->y;
        }
        if (object->mesh.mesh.min.z > vertex->z) {
            object->mesh.mesh.min.z = vertex->z;
        }
    }
}
#pragma dont_inline reset

static void ObjectCullHook(HSF_OBJECT *object, HSF_TRANSFORM *transform,
    Mtx *prevMtx, Mtx *currMtx)
{
    Mtx objectMtx;
    BOOL rotF = FALSE;

    if (transform->rot.x != 0.0f) {
        rotF = TRUE;
        mbMtxRotTrigScaleX(objectMtx, mbSinDeg(transform->rot.x),
            mbCosDeg(transform->rot.x), &transform->scale);
    }
    if (transform->rot.y != 0.0f) {
        if (rotF == FALSE) {
            rotF = TRUE;
            mbMtxRotTrigScaleY(objectMtx, mbSinDeg(transform->rot.y),
                mbCosDeg(transform->rot.y), &transform->scale);
        } else {
            mbMtxRotTrigY(objectMtx, mbSinDeg(transform->rot.y),
                mbCosDeg(transform->rot.y));
        }
    }
    if (transform->rot.z != 0.0f) {
        if (rotF == FALSE) {
            rotF = TRUE;
            mbMtxRotTrigScaleZ(objectMtx, mbSinDeg(transform->rot.z),
                mbCosDeg(transform->rot.z), &transform->scale);
        } else {
            mbMtxRotTrigZ(objectMtx, mbSinDeg(transform->rot.z),
                mbCosDeg(transform->rot.z));
        }
    }
    if (rotF == FALSE) {
        PSMTXScale(objectMtx, transform->scale.x, transform->scale.y,
            transform->scale.z);
    }
    objectMtx[0][3] = transform->pos.x;
    objectMtx[1][3] = transform->pos.y;
    objectMtx[2][3] = transform->pos.z;
    PSMTXConcat(*prevMtx, objectMtx, *currMtx);
    ObjectCullUpdate(object, *currMtx);
}

void mbObjCullInit(MBMODELID modelId)
{
    HSF_DATA *hsf;
    HSF_OBJECT *object;
    s16 i;
    BOOL cullF = FALSE;

    hsf = Hu3DData[mbObjModelIDGet(modelId)].hsf;
    mbObjModelIDGet(modelId);
    object = hsf->object;
    for (i = 0; i < hsf->objectNum; i++, object++) {
        if (object->mesh.cenvNum == 0 && object->constData != NULL) {
            cullF = TRUE;
            ObjectBBoxUpdate(object);
            ((HSF_CONSTDATA *)object->constData)->hook = ObjectCullHook;
        }
    }
    if (cullF == FALSE) {
        Hu3DModelAttrSet(mbObjModelIDGet(modelId), HU3D_ATTR_NOCULL);
    }
}
