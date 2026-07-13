#include "dolphin/mtx.h"

#pragma fp_contract off

extern f32 sinf(f32);
extern f32 cosf(f32);
extern f32 tanf(f32);

#ifdef GEKKO
void PSMTXIdentity(Mtx m)
{
    m[0][0] = 1;
    m[0][1] = 0;
    m[0][2] = 0;
    m[0][3] = 0;
    m[1][0] = 0;
    m[1][1] = 1;
    m[1][2] = 0;
    m[1][3] = 0;
    m[2][0] = 0;
    m[2][1] = 0;
    m[2][2] = 1;
    m[2][3] = 0;
}

void PSMTXCopy(const Mtx src, Mtx dst)
{
    if (src != dst) {
        dst[0][0] = src[0][0];
        dst[0][1] = src[0][1];
        dst[0][2] = src[0][2];
        dst[0][3] = src[0][3];
        dst[1][0] = src[1][0];
        dst[1][1] = src[1][1];
        dst[1][2] = src[1][2];
        dst[1][3] = src[1][3];
        dst[2][0] = src[2][0];
        dst[2][1] = src[2][1];
        dst[2][2] = src[2][2];
        dst[2][3] = src[2][3];
    }
}

void PSMTXConcat(const Mtx a, const Mtx b, Mtx ab)
{
    Mtx mTmp;
    MtxPtr m;

    if (ab == a || ab == b) {
        m = mTmp;
    }
    else {
        m = ab;
    }

    m[0][0] = a[0][2] * b[2][0] + ((a[0][0] * b[0][0]) + (a[0][1] * b[1][0]));
    m[0][1] = a[0][2] * b[2][1] + ((a[0][0] * b[0][1]) + (a[0][1] * b[1][1]));
    m[0][2] = a[0][2] * b[2][2] + ((a[0][0] * b[0][2]) + (a[0][1] * b[1][2]));
    m[0][3] = a[0][3] + (a[0][2] * b[2][3] + (a[0][0] * b[0][3] + (a[0][1] * b[1][3])));

    m[1][0] = a[1][2] * b[2][0] + ((a[1][0] * b[0][0]) + (a[1][1] * b[1][0]));
    m[1][1] = a[1][2] * b[2][1] + ((a[1][0] * b[0][1]) + (a[1][1] * b[1][1]));
    m[1][2] = a[1][2] * b[2][2] + ((a[1][0] * b[0][2]) + (a[1][1] * b[1][2]));
    m[1][3] = a[1][3] + (a[1][2] * b[2][3] + (a[1][0] * b[0][3] + (a[1][1] * b[1][3])));

    m[2][0] = a[2][2] * b[2][0] + ((a[2][0] * b[0][0]) + (a[2][1] * b[1][0]));
    m[2][1] = a[2][2] * b[2][1] + ((a[2][0] * b[0][1]) + (a[2][1] * b[1][1]));
    m[2][2] = a[2][2] * b[2][2] + ((a[2][0] * b[0][2]) + (a[2][1] * b[1][2]));
    m[2][3] = a[2][3] + (a[2][2] * b[2][3] + (a[2][0] * b[0][3] + (a[2][1] * b[1][3])));

    if (m == mTmp) {
        PSMTXCopy(mTmp, ab);
    }
}

void PSMTXTranspose(const Mtx src, Mtx xPose)
{
    Mtx mTmp;
    MtxPtr m;

    if (src == xPose) {
        m = mTmp;
    }
    else {
        m = xPose;
    }

    m[0][0] = src[0][0];
    m[0][1] = src[1][0];
    m[0][2] = src[2][0];
    m[0][3] = 0;
    m[1][0] = src[0][1];
    m[1][1] = src[1][1];
    m[1][2] = src[2][1];
    m[1][3] = 0;
    m[2][0] = src[0][2];
    m[2][1] = src[1][2];
    m[2][2] = src[2][2];
    m[2][3] = 0;

    if (m == mTmp) {
        PSMTXCopy(mTmp, xPose);
    }
}

u32 PSMTXInverse(const Mtx src, Mtx inv)
{
    Mtx mTmp;
    MtxPtr m;
    f32 det;

    if (src == inv) {
        m = mTmp;
    }
    else {
        m = inv;
    }

    det = ((((src[2][1] * (src[0][2] * src[1][0]))
          + ((src[2][2] * (src[0][0] * src[1][1]))
          + (src[2][0] * (src[0][1] * src[1][2]))))
          - (src[0][2] * (src[2][0] * src[1][1])))
          - (src[2][2] * (src[1][0] * src[0][1])))
          - (src[1][2] * (src[0][0] * src[2][1]));
    if (det == 0) {
        return 0;
    }

    det = 1 / det;
    m[0][0] = det * ((src[1][1] * src[2][2]) - (src[2][1] * src[1][2]));
    m[0][1] = det * -((src[0][1] * src[2][2]) - (src[2][1] * src[0][2]));
    m[0][2] = det * ((src[0][1] * src[1][2]) - (src[1][1] * src[0][2]));

    m[1][0] = det * -((src[1][0] * src[2][2]) - (src[2][0] * src[1][2]));
    m[1][1] = det * ((src[0][0] * src[2][2]) - (src[2][0] * src[0][2]));
    m[1][2] = det * -((src[0][0] * src[1][2]) - (src[1][0] * src[0][2]));

    m[2][0] = det * ((src[1][0] * src[2][1]) - (src[2][0] * src[1][1]));
    m[2][1] = det * -((src[0][0] * src[2][1]) - (src[2][0] * src[0][1]));
    m[2][2] = det * ((src[0][0] * src[1][1]) - (src[1][0] * src[0][1]));

    m[0][3] = ((-m[0][0] * src[0][3]) - (m[0][1] * src[1][3])) - (m[0][2] * src[2][3]);
    m[1][3] = ((-m[1][0] * src[0][3]) - (m[1][1] * src[1][3])) - (m[1][2] * src[2][3]);
    m[2][3] = ((-m[2][0] * src[0][3]) - (m[2][1] * src[1][3])) - (m[2][2] * src[2][3]);

    if (m == mTmp) {
        PSMTXCopy(mTmp, inv);
    }
    return 1;
}

u32 PSMTXInvXpose(const Mtx src, Mtx invX)
{
    Mtx mTmp;
    MtxPtr m;
    f32 det;

    if (src == invX) {
        m = mTmp;
    }
    else {
        m = invX;
    }

    det = ((((src[2][1] * (src[0][2] * src[1][0]))
          + ((src[2][2] * (src[0][0] * src[1][1]))
          + (src[2][0] * (src[0][1] * src[1][2]))))
          - (src[0][2] * (src[2][0] * src[1][1])))
          - (src[2][2] * (src[1][0] * src[0][1])))
          - (src[1][2] * (src[0][0] * src[2][1]));
    if (det == 0) {
        return 0;
    }

    det = 1 / det;
    m[0][0] = det * ((src[1][1] * src[2][2]) - (src[2][1] * src[1][2]));
    m[0][1] = det * -((src[1][0] * src[2][2]) - (src[2][0] * src[1][2]));
    m[0][2] = det * ((src[1][0] * src[2][1]) - (src[2][0] * src[1][1]));

    m[1][0] = det * -((src[0][1] * src[2][2]) - (src[2][1] * src[0][2]));
    m[1][1] = det * ((src[0][0] * src[2][2]) - (src[2][0] * src[0][2]));
    m[1][2] = det * -((src[0][0] * src[2][1]) - (src[2][0] * src[0][1]));

    m[2][0] = det * ((src[0][1] * src[1][2]) - (src[1][1] * src[0][2]));
    m[2][1] = det * -((src[0][0] * src[1][2]) - (src[1][0] * src[0][2]));
    m[2][2] = det * ((src[0][0] * src[1][1]) - (src[1][0] * src[0][1]));

    m[0][3] = 0;
    m[1][3] = 0;
    m[2][3] = 0;

    if (m == mTmp) {
        PSMTXCopy(mTmp, invX);
    }
    return 1;
}

void PSMTXRotRad(Mtx m, char axis, f32 rad)
{
    f32 sinA;
    f32 cosA;

    sinA = sinf(rad);
    cosA = cosf(rad);
    PSMTXRotTrig(m, axis, sinA, cosA);
}

void PSMTXRotTrig(Mtx m, char axis, f32 sinA, f32 cosA)
{
    switch (axis) {
        case 'x':
        case 'X':
            m[0][0] = 1;
            m[0][1] = 0;
            m[0][2] = 0;
            m[0][3] = 0;
            m[1][0] = 0;
            m[1][1] = cosA;
            m[1][2] = -sinA;
            m[1][3] = 0;
            m[2][0] = 0;
            m[2][1] = sinA;
            m[2][2] = cosA;
            m[2][3] = 0;
            break;
        case 'y':
        case 'Y':
            m[0][0] = cosA;
            m[0][1] = 0;
            m[0][2] = sinA;
            m[0][3] = 0;
            m[1][0] = 0;
            m[1][1] = 1;
            m[1][2] = 0;
            m[1][3] = 0;
            m[2][0] = -sinA;
            m[2][1] = 0;
            m[2][2] = cosA;
            m[2][3] = 0;
            break;
        case 'z':
        case 'Z':
            m[0][0] = cosA;
            m[0][1] = -sinA;
            m[0][2] = 0;
            m[0][3] = 0;
            m[1][0] = sinA;
            m[1][1] = cosA;
            m[1][2] = 0;
            m[1][3] = 0;
            m[2][0] = 0;
            m[2][1] = 0;
            m[2][2] = 1;
            m[2][3] = 0;
            break;
    }
}

void __PSMTXRotAxisRadInternal(Mtx m, const Vec *axis, f32 sT, f32 cT)
{
    Vec vN;
    f32 t;
    f32 x;
    f32 y;
    f32 z;
    f32 xSq;
    f32 ySq;
    f32 zSq;

    t = 1 - cT;
    PSVECNormalize(axis, &vN);
    x = vN.x;
    y = vN.y;
    z = vN.z;
    xSq = x * x;
    ySq = y * y;
    zSq = z * z;

    m[0][0] = cT + (t * xSq);
    m[0][1] = (y * (t * x)) - (sT * z);
    m[0][2] = (z * (t * x)) + (sT * y);
    m[0][3] = 0;
    m[1][0] = (y * (t * x)) + (sT * z);
    m[1][1] = cT + (t * ySq);
    m[1][2] = (z * (t * y)) - (sT * x);
    m[1][3] = 0;
    m[2][0] = (z * (t * x)) - (sT * y);
    m[2][1] = (z * (t * y)) + (sT * x);
    m[2][2] = cT + (t * zSq);
    m[2][3] = 0;
}

void PSMTXRotAxisRad(Mtx m, const Vec *axis, f32 rad)
{
    f32 sT;
    f32 cT;

    sT = sinf(rad);
    cT = cosf(rad);
    __PSMTXRotAxisRadInternal(m, axis, sT, cT);
}

void PSMTXTrans(Mtx m, f32 xT, f32 yT, f32 zT)
{
    m[0][0] = 1;
    m[0][1] = 0;
    m[0][2] = 0;
    m[0][3] = xT;
    m[1][0] = 0;
    m[1][1] = 1;
    m[1][2] = 0;
    m[1][3] = yT;
    m[2][0] = 0;
    m[2][1] = 0;
    m[2][2] = 1;
    m[2][3] = zT;
}

void PSMTXTransApply(const Mtx src, Mtx dst, f32 xT, f32 yT, f32 zT)
{
    if (src != dst) {
        dst[0][0] = src[0][0];
        dst[0][1] = src[0][1];
        dst[0][2] = src[0][2];
        dst[1][0] = src[1][0];
        dst[1][1] = src[1][1];
        dst[1][2] = src[1][2];
        dst[2][0] = src[2][0];
        dst[2][1] = src[2][1];
        dst[2][2] = src[2][2];
    }

    dst[0][3] = src[0][3] + xT;
    dst[1][3] = src[1][3] + yT;
    dst[2][3] = src[2][3] + zT;
}

void PSMTXScale(Mtx m, f32 xS, f32 yS, f32 zS)
{
    m[0][0] = xS;
    m[0][1] = 0;
    m[0][2] = 0;
    m[0][3] = 0;
    m[1][0] = 0;
    m[1][1] = yS;
    m[1][2] = 0;
    m[1][3] = 0;
    m[2][0] = 0;
    m[2][1] = 0;
    m[2][2] = zS;
    m[2][3] = 0;
}

void PSMTXScaleApply(const Mtx src, Mtx dst, f32 xS, f32 yS, f32 zS)
{
    dst[0][0] = src[0][0] * xS;
    dst[0][1] = src[0][1] * xS;
    dst[0][2] = src[0][2] * xS;
    dst[0][3] = src[0][3] * xS;
    dst[1][0] = src[1][0] * yS;
    dst[1][1] = src[1][1] * yS;
    dst[1][2] = src[1][2] * yS;
    dst[1][3] = src[1][3] * yS;
    dst[2][0] = src[2][0] * zS;
    dst[2][1] = src[2][1] * zS;
    dst[2][2] = src[2][2] * zS;
    dst[2][3] = src[2][3] * zS;
}

void PSMTXQuat(Mtx m, const Quaternion *q)
{
    f32 s;
    f32 xs;
    f32 ys;
    f32 zs;
    f32 wx;
    f32 wy;
    f32 wz;
    f32 xx;
    f32 xy;
    f32 xz;
    f32 yy;
    f32 yz;
    f32 zz;

    s = 2 / ((q->w * q->w) + ((q->z * q->z) + ((q->x * q->x) + (q->y * q->y))));
    xs = q->x * s;
    ys = q->y * s;
    zs = q->z * s;
    wx = q->w * xs;
    wy = q->w * ys;
    wz = q->w * zs;
    xx = q->x * xs;
    xy = q->x * ys;
    xz = q->x * zs;
    yy = q->y * ys;
    yz = q->y * zs;
    zz = q->z * zs;

    m[0][0] = 1 - (yy + zz);
    m[0][1] = xy - wz;
    m[0][2] = xz + wy;
    m[0][3] = 0;
    m[1][0] = xy + wz;
    m[1][1] = 1 - (xx + zz);
    m[1][2] = yz - wx;
    m[1][3] = 0;
    m[2][0] = xz - wy;
    m[2][1] = yz + wx;
    m[2][2] = 1 - (xx + yy);
    m[2][3] = 0;
}
#endif

void C_MTXLookAt(Mtx m, const Vec *camPos, const Vec *camUp, const Vec *target)
{
    Vec vLook, vRight, vUp;

    vLook.x = camPos->x - target->x;
    vLook.y = camPos->y - target->y;
    vLook.z = camPos->z - target->z;
    VECNormalize(&vLook, &vLook);
    VECCrossProduct(camUp, &vLook, &vRight);
    VECNormalize(&vRight, &vRight);
    VECCrossProduct(&vLook, &vRight, &vUp);

    m[0][0] = vRight.x;
    m[0][1] = vRight.y;
    m[0][2] = vRight.z;
    m[0][3] = -(camPos->x * vRight.x + camPos->y * vRight.y + camPos->z * vRight.z);

    m[1][0] = vUp.x;
    m[1][1] = vUp.y;
    m[1][2] = vUp.z;
    m[1][3] = -(camPos->x * vUp.x + camPos->y * vUp.y + camPos->z * vUp.z);

    m[2][0] = vLook.x;
    m[2][1] = vLook.y;
    m[2][2] = vLook.z;
    m[2][3] = -(camPos->x * vLook.x + camPos->y * vLook.y + camPos->z * vLook.z);
}

void C_MTXLightPerspective(Mtx m, f32 fovY, f32 aspect, float scaleS, float scaleT, float transS, float transT)
{
    f32 angle;
    f32 cot;

    angle = fovY * 0.5f;
    angle = MTXDegToRad(angle);
    cot = 1.0f / tanf(angle);

    m[0][0] = (cot / aspect) * scaleS;
    m[0][1] = 0.0f;
    m[0][2] = -transS;
    m[0][3] = 0.0f;

    m[1][0] = 0.0f;
    m[1][1] = cot * scaleT;
    m[1][2] = -transT;
    m[1][3] = 0.0f;

    m[2][0] = 0.0f;
    m[2][1] = 0.0f;
    m[2][2] = -1.0f;
    m[2][3] = 0.0f;
}
