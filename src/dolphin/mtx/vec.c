#include "dolphin/mtx.h"
#include "math.h"

#pragma fp_contract off

void PSVECAdd(const Vec *a, const Vec *b, Vec *ab)
{
    ab->x = a->x + b->x;
    ab->y = a->y + b->y;
    ab->z = a->z + b->z;
}

void PSVECSubtract(const Vec *a, const Vec *b, Vec *a_b)
{
    a_b->x = a->x - b->x;
    a_b->y = a->y - b->y;
    a_b->z = a->z - b->z;
}

void PSVECScale(const Vec *src, Vec *dst, f32 scale)
{
    dst->x = src->x * scale;
    dst->y = src->y * scale;
    dst->z = src->z * scale;
}

void PSVECNormalize(const Vec *src, Vec *unit)
{
    f32 mag;

    mag = src->z * src->z + ((src->x * src->x) + (src->y * src->y));
    mag = 1.0f / sqrtf(mag);
    unit->x = src->x * mag;
    unit->y = src->y * mag;
    unit->z = src->z * mag;
}

f32 PSVECSquareMag(const Vec *v)
{
    return v->z * v->z + ((v->x * v->x) + (v->y * v->y));
}

f32 PSVECMag(const Vec *v)
{
    return sqrtf(PSVECSquareMag(v));
}

f32 PSVECDotProduct(const Vec *a, const Vec *b)
{
    return (a->z * b->z) + ((a->x * b->x) + (a->y * b->y));
}

void PSVECCrossProduct(const Vec *a, const Vec *b, Vec *axb)
{
    Vec vTmp;

    vTmp.x = (a->y * b->z) - (a->z * b->y);
    vTmp.y = (a->z * b->x) - (a->x * b->z);
    vTmp.z = (a->x * b->y) - (a->y * b->x);
    axb->x = vTmp.x;
    axb->y = vTmp.y;
    axb->z = vTmp.z;
}

void C_VECHalfAngle(const Vec *a, const Vec *b, Vec *half)
{
    Vec a0;
    Vec b0;
    Vec ab;

    a0.x = -a->x;
    a0.y = -a->y;
    a0.z = -a->z;

    b0.x = -b->x;
    b0.y = -b->y;
    b0.z = -b->z;

    VECNormalize(&a0, &a0);
    VECNormalize(&b0, &b0);
    VECAdd(&a0, &b0, &ab);

    if (VECDotProduct(&ab, &ab) > 0.0f) {
        VECNormalize(&ab, half);
    }
    else {
        *half = ab;
    }
}

void C_VECReflect(const Vec *src, const Vec *normal, Vec *dst)
{
    Vec a0;
    Vec b0;
    f32 dot;

    a0.x = -src->x;
    a0.y = -src->y;
    a0.z = -src->z;

    VECNormalize(&a0, &a0);
    VECNormalize(normal, &b0);

    dot = VECDotProduct(&a0, &b0);
    dst->x = b0.x * 2.0f * dot - a0.x;
    dst->y = b0.y * 2.0f * dot - a0.y;
    dst->z = b0.z * 2.0f * dot - a0.z;

    VECNormalize(dst, dst);
}

f32 PSVECSquareDistance(const Vec *a, const Vec *b)
{
    Vec diff;

    diff.x = a->x - b->x;
    diff.y = a->y - b->y;
    diff.z = a->z - b->z;
    return (diff.z * diff.z) + ((diff.x * diff.x) + (diff.y * diff.y));
}

f32 PSVECDistance(const Vec *a, const Vec *b)
{
    return sqrtf(PSVECSquareDistance(a, b));
}
