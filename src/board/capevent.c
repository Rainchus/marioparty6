#include "game/gamework.h"

#include "humath.h"

#define CAPSULE_KOOPA 43
#define CAPSULE_INVALID -99

typedef void (*CAPSULE_HOOK)(int, int, int, BOOL, BOOL, BOOL);

static int kettouCoinLose = 10;
static int kettouOppCoinLose = 5;
static int capsuleEventMasu = -1;
static int capsuleEventPlayer = -1;
static int capsuleEventPrevMasu = -1;
static int capsuleEventPrevPlayer = -1;
static CAPSULE_HOOK capsuleHook;

void MBCapsuleStub5(void)
{
}

void MBCapsuleStub6(void)
{
}

void MBCapsuleStub7(void)
{
}

void mbev_CapMoveMasuSet(int playerNo, int masuId)
{
    capsuleEventPlayer = playerNo;
    capsuleEventMasu = masuId;
}

void mbev_CapStopMasuSet(int playerNo, int masuId)
{
    capsuleEventPrevPlayer = playerNo;
    capsuleEventPrevMasu = masuId;
}

void mbev_CapVsEndCall(void)
{
}

void mbev_CapKillerCall(void)
{
}

void mbev_CapKillerMultiCall(void)
{
}

void MBCapsuleStub11(void)
{
}

void MBCapsuleStub12(void)
{
}

BOOL mbev_CapKillerMoveCheck(int playerNo)
{
    return TRUE;
}

BOOL mbev_CapKillerMoveCheckAll(void)
{
    return TRUE;
}

int mbev_CapCapGet(void)
{
    return -1;
}

int mbev_CapBankCoinGet(void)
{
    return GwSystem.bankCoin;
}

int mbev_CapKettouCoinLoseGet(void)
{
    return kettouCoinLose;
}

int mbev_CapKettouOppCoinLoseGet(void)
{
    return kettouOppCoinLose;
}

int mbev_CapKettouCoinLoseGet2(void)
{
    return kettouCoinLose;
}

int mbev_CapKettouOppCoinLoseGet2(void)
{
    return kettouOppCoinLose;
}

void mbev_CapOpeningAdd(int capsuleNum)
{
}

void mbev_CapKoopaAdd(void)
{
}

void mbev_CapBubbleHookSet(CAPSULE_HOOK hook)
{
    capsuleHook = hook;
}

void mbev_CapBubbleHookCall(int type, int modelId, BOOL flag1, BOOL flag2, BOOL flag3)
{
    if (capsuleHook != NULL) {
        capsuleHook(-1, type, modelId, flag1, flag2, flag3);
    }
}

void mbev_CapBubbleHookCallStory(int eventType, int type, int modelId)
{
    if (capsuleHook != NULL) {
        switch (eventType) {
            case 0:
                capsuleHook(CAPSULE_KOOPA, type, modelId, FALSE, FALSE, FALSE);
                break;
            case 1:
                capsuleHook(CAPSULE_INVALID, type, modelId, FALSE, FALSE, FALSE);
                break;
        }
    }
}

void mbev_CapBankCoinInit(void)
{
    GwSystem.bankCoin = 0;
}

void mbev_CapNullKill(void)
{
}

void mbev_CapDebugCamKlll(void)
{
}

void mbev_CapDebugWarpKill(void)
{
}

void mbev_CapDebugPosSelectKill(void)
{
}

float mbev_CapAngleWrap(float a, float b)
{
    float result;

    if (a >= 360) {
        a -= 360;
    } else if (a < 0) {
        a += 360;
    }
    if (b >= 360) {
        b -= 360;
    } else if (b < 0) {
        b += 360;
    }
    result = a - b;
    if (result <= -180.0f) {
        result += 360;
    } else if (result >= 180.0f) {
        result -= 360;
    }
    return result;
}

float mbev_CapAngleLerp(float a, float b, float t)
{
    float result;
    float delta;

    if (a >= 360.0) {
        a -= 360.0;
    } else if (a < 0.0) {
        a += 360.0;
    }
    if (b >= 360.0) {
        b -= 360.0;
    } else if (b < 0.0) {
        b += 360.0;
    }
    delta = (a - b) + 360.0;
    if (fabs(delta) >= 360) {
        delta = fmod(delta, 360);
    }
    if (delta < 180.0) {
        if (delta <= t) {
            result = delta;
        } else {
            result = t;
        }
    } else {
        if ((360.0 - delta) <= t) {
            result = -(360.0 - delta);
        } else {
            result = -t;
        }
    }
    result += b;
    if (result >= 360.0) {
        result -= 360.0;
    } else if (result < 0.0) {
        result += 360.0;
    }
    return result;
}

float mbev_CapAngleSumLerp(float t, float a, float b)
{
    float wrapAngle = mbev_CapAngleWrap(b, a);

    return mbev_CapAngleLerp(b, a, fabs(wrapAngle * t));
}

void mbev_CapHermiteConstGet(float t, float *a, float *b, float *c, float *d)
{
    float square = t * t;
    float cube = t * square;
    float blend = ((3.0 * square) - cube) - cube;

    *a = 1.0 - blend;
    *b = blend;
    *c = t + ((cube - square) - square);
    *d = cube - square;
}

float mbev_CapHermiteConstGet2(float t, float a, float b, float c, float d)
{
    float delta;
    float h00;
    float h01;
    float h10;
    float h11;
    float result;
    float half0;
    float half1;
    float tangent0;
    float tangent1;
    int deltaInt;

    delta = c - b;
    deltaInt = c - b;
    if (b == c) {
        tangent0 = delta;
    } else {
        half0 = 0.5f;
        tangent0 = half0 * (delta + (b - a));
    }
    if (b == d) {
        tangent1 = delta;
    } else {
        half1 = 0.5f;
        tangent1 = half1 * (delta + (d - c));
    }
    mbev_CapHermiteConstGet(t, &h00, &h01, &h10, &h11);
    return result = (h00 * b) + (h01 * c) + (h10 * tangent0)
        + (h11 * tangent1);
}

void mbev_CapHermiteGetV(
    float t,
    HuVecF *a,
    HuVecF *b,
    HuVecF *c,
    HuVecF *d,
    HuVecF *out)
{
    out->x = mbev_CapHermiteConstGet2(t, a->x, b->x, c->x, d->x);
    out->y = mbev_CapHermiteConstGet2(t, a->y, b->y, c->y, d->y);
    out->z = mbev_CapHermiteConstGet2(t, a->z, b->z, c->z, d->z);
}

float mbev_CapBezierGet(float t, float a, float b, float c)
{
    float temp = 1.0 - t;
    float result = (a * (temp * temp)) + (temp * t * b * 2.0) + (t * t * c);

    return result;
}

void mbev_CapBezierGetV(float t, float *a, float *b, float *c, float *out)
{
    int i;

    for (i = 0; i < 3; i++) {
        *out++ = mbev_CapBezierGet(t, *a++, *b++, *c++);
    }
}

float mbev_CapBezierSlopeGet(float t, float a, float b, float c)
{
    float result = 2.0 * (((t - 1.0) * a) + ((1.0 - (2.0 * t)) * b)
        + (t * c));

    return result;
}

void mbev_CapBezierNormGetV(float t, float *a, float *b, float *c, float *out)
{
    int i;
    float temp[3];
    float mag;

    for (i = 0; i < 3; i++) {
        temp[i] = mbev_CapBezierSlopeGet(t, *a++, *b++, *c++);
    }
    mag = HuMagPoint3D(temp[0], temp[1], temp[2]);
    if (mag) {
        mag = 1.0 / mag;
        for (i = 0; i < 3; i++) {
            *out++ = temp[i] * mag;
        }
    } else {
        *out++ = 0;
        *out++ = 0;
        *out++ = 1;
    }
}
