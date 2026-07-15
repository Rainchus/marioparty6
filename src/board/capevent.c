#include "game/gamework.h"
#include "game/memory.h"
#include "game/object.h"
#include "game/board/player.h"
#include "game/process.h"
#include "game/board/window.h"

#include "humath.h"

#define CAPSULE_KOOPA 43
#define CAPSULE_INVALID -99

typedef void (*CAPSULE_HOOK)(int, int, int, BOOL, BOOL, BOOL);

static HUPROCESS *ev_CapBonusCoinProc[GW_PLAYER_MAX];
static OMOBJ *ev_CapEffExplodeOMObj[8];
static OMOBJ *ev_CapEffBoostOMObj[8];
static OMOBJ *ev_CapEffSnowOMObj[8];
static OMOBJ *ev_CapEffGlowOMObj[8];
static OMOBJ *ev_CapEffRingOMObj[8];
static OMOBJ *ev_CapEffCoinOMObj[8];
static OMOBJ *ev_CapEffCoinManOMObj[8];
static OMOBJ *ev_CapEffStarManOMObj[8];
static OMOBJ *ev_CapEffCapLoseOMObj[8];
static OMOBJ *ev_CapEffRayOMObj[8];
static OMOBJ *ev_CapEffMasuHitOMObj[8];
static OMOBJ *ev_CapEffMoveOMObj[GW_PLAYER_MAX];

typedef struct CapBonusCoinWork {
    int playerNo;
} CAPBONUSCOINWORK;

typedef struct CapEffBoostWork {
    u8 _unk00[4];
    int time;
} CAPEFFBOOSTWORK;

typedef struct CapEffDispWork {
    u8 _unk00[4];
    int dispF;
} CAPEFFDISPWORK;

typedef struct CapEffRingWork {
    u8 _unk00[0xC];
    int dispF;
} CAPEFFRINGWORK;

typedef struct CapEffCoinWork {
    u8 _unk00[8];
    int activeF;
    u8 _unk0C[0x14];
    float maxY;
    u8 _unk24[0x38];
    int glowF;
} CAPEFFCOINWORK;

typedef struct CapEffMoveWork {
    u8 _unk00[4];
    int state;
    u8 _unk08[0x10];
    int minYF;
    float minY;
    float vel;
    u8 _unk24[0xC];
    HuVecF moveDir;
} CAPEFFMOVEWORK;

typedef struct CapEffRayWork {
    u8 _unk00[4];
    int objIdx;
} CAPEFFRAYWORK;

typedef struct CapEffMasuHitWork {
    u8 _unk00[8];
    int objIdx;
} CAPEFFMASUHITWORK;

typedef struct CapCoinManWork {
    u8 _unk00[4];
    int activeF;
    u8 _unk08[0x34];
} CAPCOINMANWORK;

typedef struct CapStarManWork {
    u8 _unk00[4];
    int activeF;
    u8 _unk08[0x34];
} CAPSTARMANWORK;

typedef struct CapEffCapLoseWork {
    u8 _unk00[4];
    int activeF;
    u8 _unk08[0x2C];
} CAPEFFCAPLOSEWORK;

static int kettouCoinLose = 10;
static int kettouOppCoinLose = 5;
static int capsuleEventMasu = -1;
static int capsuleEventPlayer = -1;
static int capsuleEventPrevMasu = -1;
static int capsuleEventPrevPlayer = -1;
static GXColor ev_CapsuleRandomColorTbl[7] = {
    { 255, 127, 127, 255 },
    { 255, 127, 64, 255 },
    { 255, 255, 127, 255 },
    { 127, 255, 127, 255 },
    { 127, 127, 255, 255 },
    { 64, 64, 255, 255 },
    { 255, 127, 255, 255 }
};
static CAPSULE_HOOK capsuleHook;
static int capsuleChoice;

static void ev_CapCoinAdd(OMOBJ *obj, int playerNo, int coinNum, BOOL highF,
    void (*hook)(void));
static void ev_CapComChoiceHook(void);
void mbev_CapBonusCoinCall(int playerNo, int capsuleNo, int coinNum,
    BOOL waitF);
void mbev_CapPlayerSquishVoiceSet(int *playerNo, int masuId, BOOL voiceF);
BOOL mbev_CapCullCheck(int playerNo, int masuId);
int mbev_CapPlayerComSelSameGet(int playerNo, int selection, BOOL sameF);
void mbev_CapBiriQMetalShock(void *workP);

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

void mbev_CapCircuitCallKettou(void)
{
}

void mbev_CapRandomBonusCoin(int playerNo, int capsuleNo, BOOL waitF)
{
    mbev_CapBonusCoinCall(playerNo, capsuleNo, -1, waitF);
}

BOOL mbev_CapBonusCoinCheck(int playerNo)
{
    if (ev_CapBonusCoinProc[playerNo] != NULL) {
        return FALSE;
    } else {
        return TRUE;
    }
}

void mbev_CapNull(void)
{
    void *workP = HuPrcCurrentGet()->property;

    HuPrcEnd();
}

void mbev_CapPlayerMoveObjInit(void)
{
    int i;

    for (i = 0; i < GW_PLAYER_MAX; i++) {
        ev_CapEffMoveOMObj[i] = NULL;
    }
}

void mbev_CapPlayerMoveObjClose(int playerNo)
{
    ev_CapEffMoveOMObj[playerNo] = NULL;
}

void mbev_CapPlayerMoveObjKill(void)
{
    int i;

    for (i = 0; i < GW_PLAYER_MAX; i++) {
        ev_CapEffMoveOMObj[i] = NULL;
    }
}

void mbev_CapCoinAdd(
    OMOBJ *obj, int playerNo, int coinNum, BOOL highF)
{
    ev_CapCoinAdd(obj, playerNo, coinNum, highF, NULL);
}

void mbev_CapPlayerSquishSet(int *playerNo, int masuId)
{
    mbev_CapPlayerSquishVoiceSet(playerNo, masuId, FALSE);
}

BOOL mbev_CapCullPlayerCheck(int playerNo)
{
    return mbev_CapCullCheck(playerNo, 0);
}

int mbev_CapPlayerMasuNumGet(int masuId)
{
    int i;
    int count;

    i = 0;
    count = 0;
    for (; i < GW_PLAYER_MAX; i++) {
        if (masuId == GwPlayer[i].masuId) {
            count++;
        }
    }
    return count;
}

int mbev_CapPlayerNoSearch(int playerNo)
{
    int i;

    for (i = 0; i < GW_PLAYER_MAX; i++) {
        if (i != playerNo) {
            return i;
        }
    }
    return -1;
}

int mbev_CapPlayerComSelGet(int playerNo, int selection)
{
    return mbev_CapPlayerComSelSameGet(playerNo, selection, FALSE);
}

void mbev_CapChoiceSet(int choice)
{
    capsuleChoice = choice;
    mbWinTopComKeyHookSet(ev_CapComChoiceHook);
}

void mbev_CapVecChase(
    float weight, HuVecF *src, HuVecF *target, HuVecF *out)
{
    HuVecF delta;

    PSVECSubtract(target, src, &delta);
    PSVECScale(&delta, &delta, weight);
    PSVECAdd(src, &delta, out);
}

void mbev_CapEffBoostKill(OMOBJ *obj)
{
    int i;

    for (i = 0; i < 8; i++) {
        if (ev_CapEffBoostOMObj[i] == obj) {
            break;
        }
    }
    ev_CapEffBoostOMObj[i] = (OMOBJ *)-1;
}

int mbev_CapEffBoostTimeGet(OMOBJ *obj)
{
    int i;
    CAPEFFBOOSTWORK *workP;

    for (i = 0; i < 8; i++) {
        if (ev_CapEffBoostOMObj[i] == obj) {
            break;
        }
    }
    workP = omObjGetDataAs(obj, CAPEFFBOOSTWORK);
    return workP->time;
}

void mbev_CapEffSnowKill(OMOBJ *obj)
{
    int i;

    for (i = 0; i < 8; i++) {
        if (ev_CapEffSnowOMObj[i] == obj) {
            break;
        }
    }
    ev_CapEffSnowOMObj[i] = (OMOBJ *)-1;
}

int mbev_CapEffSnowDispGet(OMOBJ *obj)
{
    int i;
    CAPEFFDISPWORK *workP;

    for (i = 0; i < 8; i++) {
        if (ev_CapEffSnowOMObj[i] == obj) {
            break;
        }
    }
    workP = omObjGetDataAs(obj, CAPEFFDISPWORK);
    return workP->dispF;
}

void mbev_CapEffGlowKill(OMOBJ *obj)
{
    int i;

    for (i = 0; i < 8; i++) {
        if (ev_CapEffGlowOMObj[i] == obj) {
            break;
        }
    }
    ev_CapEffGlowOMObj[i] = (OMOBJ *)-1;
}

int mbev_CapEffGlowDispGet(OMOBJ *obj)
{
    int i;
    CAPEFFDISPWORK *workP;

    for (i = 0; i < 8; i++) {
        if (ev_CapEffGlowOMObj[i] == obj) {
            break;
        }
    }
    workP = omObjGetDataAs(obj, CAPEFFDISPWORK);
    return workP->dispF;
}

void mbev_CapEffRingKill(OMOBJ *obj)
{
    int i;

    for (i = 0; i < 8; i++) {
        if (ev_CapEffRingOMObj[i] == obj) {
            break;
        }
    }
    ev_CapEffRingOMObj[i] = (OMOBJ *)-1;
}

int mbev_CapEffRingDispGet(OMOBJ *obj)
{
    int i;
    CAPEFFRINGWORK *workP;

    for (i = 0; i < 8; i++) {
        if (ev_CapEffRingOMObj[i] == obj) {
            break;
        }
    }
    workP = omObjGetDataAs(obj, CAPEFFRINGWORK);
    return workP->dispF;
}

void mbev_CapEffExplodeKill(OMOBJ *obj)
{
    int i;

    for (i = 0; i < 8; i++) {
        if (ev_CapEffExplodeOMObj[i] == obj) {
            break;
        }
    }
    ev_CapEffExplodeOMObj[i] = (OMOBJ *)-1;
}

void mbev_CapEffCoinKill(OMOBJ *obj)
{
    int i;

    for (i = 0; i < 8; i++) {
        if (ev_CapEffCoinOMObj[i] == obj) {
            break;
        }
    }
    ev_CapEffCoinOMObj[i] = (OMOBJ *)-1;
}

void mbev_CapCoinManKill(OMOBJ *obj)
{
    int i;

    for (i = 0; i < 8; i++) {
        if (ev_CapEffCoinManOMObj[i] == obj) {
            break;
        }
    }
    ev_CapEffCoinManOMObj[i] = (OMOBJ *)-1;
}

void mbev_CapStarManKill(OMOBJ *obj)
{
    int i;

    for (i = 0; i < 8; i++) {
        if (ev_CapEffStarManOMObj[i] == obj) {
            break;
        }
    }
    ev_CapEffStarManOMObj[i] = (OMOBJ *)-1;
}

void mbev_CapEffCapLoseKill(OMOBJ *obj)
{
    int i;

    for (i = 0; i < 8; i++) {
        if (ev_CapEffCapLoseOMObj[i] == obj) {
            break;
        }
    }
    ev_CapEffCapLoseOMObj[i] = (OMOBJ *)-1;
}

int mbev_CapEffCoinNumGet(OMOBJ *obj)
{
    int i;
    CAPEFFCOINWORK *workP;
    int objIdx;
    int count;

    count = 0;
    for (objIdx = 0; objIdx < 8; objIdx++) {
        if (ev_CapEffCoinOMObj[objIdx] == obj) {
            break;
        }
    }
    workP = omObjGetDataAs(obj, CAPEFFCOINWORK);
    for (i = 0; i < 128; i++, workP++) {
        if (workP->activeF) {
            count++;
        }
    }
    return count;
}

int mbev_CapCoinManNumGet(OMOBJ *obj)
{
    CAPCOINMANWORK *workP;
    int objIdx;
    int i;
    int count;

    for (objIdx = 0; objIdx < 8; objIdx++) {
        if (ev_CapEffCoinManOMObj[objIdx] == obj) {
            break;
        }
    }
    workP = omObjGetDataAs(obj, CAPCOINMANWORK);
    i = 0;
    count = 0;
    for (; i < 64; i++, workP++) {
        if (workP->activeF) {
            count++;
        }
    }
    return count;
}

int mbev_CapStarManNumGet(OMOBJ *obj)
{
    CAPSTARMANWORK *workP;
    int objIdx;
    int i;
    int count;

    for (objIdx = 0; objIdx < 8; objIdx++) {
        if (ev_CapEffStarManOMObj[objIdx] == obj) {
            break;
        }
    }
    workP = omObjGetDataAs(obj, CAPSTARMANWORK);
    i = 0;
    count = 0;
    for (; i < 8; i++, workP++) {
        if (workP->activeF) {
            count++;
        }
    }
    return count;
}

int mbev_CapEffCapLoseNumGet(OMOBJ *obj)
{
    CAPEFFCAPLOSEWORK *workP;
    int objIdx;
    int i;
    int count;

    for (objIdx = 0; objIdx < 8; objIdx++) {
        if (ev_CapEffCapLoseOMObj[objIdx] == obj) {
            break;
        }
    }
    workP = omObjGetDataAs(obj, CAPEFFCAPLOSEWORK);
    i = 0;
    count = 0;
    for (; i < 6; i++, workP++) {
        if (workP->activeF) {
            count++;
        }
    }
    return count;
}

static void ev_CapBiriQMetalShockDestroy(void)
{
    void *workP = HuPrcCurrentGet()->property;

    HuMemDirectFree(workP);
}

static void ev_CapBiriQMetalShock(void)
{
    void *workP = HuPrcCurrentGet()->property;

    mbev_CapBiriQMetalShock(workP);
    HuPrcEnd();
}

static void ev_CapEffOpenKill(void)
{
    HUPROCESS *process = HuPrcCurrentGet();
    void *workP = process->property;

    HuMemDirectFree(workP);
}

static void ev_CapBonusCoinKill(void)
{
    HUPROCESS *process = HuPrcCurrentGet();
    CAPBONUSCOINWORK *workP = process->property;

    ev_CapBonusCoinProc[workP->playerNo] = NULL;
    HuMemDirectFree(workP);
}

void mbev_CapPlayerMoveMinYSet(int playerNo, float minY)
{
    CAPEFFMOVEWORK *workP;
    OMOBJ *obj = ev_CapEffMoveOMObj[playerNo];

    if (obj != NULL) {
        workP = omObjGetDataAs(obj, CAPEFFMOVEWORK);
        workP->minY = minY;
        workP->minYF = FALSE;
    }
}

void mbev_CapPlayerMoveVelSet(int playerNo, float vel, HuVecF *moveDir)
{
    CAPEFFMOVEWORK *workP;
    OMOBJ *obj = ev_CapEffMoveOMObj[playerNo];

    if (obj != NULL) {
        workP = omObjGetDataAs(obj, CAPEFFMOVEWORK);
        workP->vel = vel;
        workP->moveDir = *moveDir;
    }
}

BOOL mbev_CapEffCoinMaxYSet(OMOBJ *obj, int coinNo, float maxY)
{
    CAPEFFCOINWORK *workP;
    int i;

    for (i = 0; i < 8; i++) {
        if (ev_CapEffCoinOMObj[i] == obj) {
            break;
        }
    }
    workP = omObjGetDataAs(obj, CAPEFFCOINWORK);
    workP = &workP[coinNo];
    workP->maxY = maxY;
    return TRUE;
}

BOOL mbev_CapPlayerMoveObjCheck(int playerNo)
{
    OMOBJ *obj = ev_CapEffMoveOMObj[playerNo];
    CAPEFFMOVEWORK *workP;

    if (obj == NULL) {
        return TRUE;
    }
    workP = omObjGetDataAs(obj, CAPEFFMOVEWORK);
    if (workP->state >= 1) {
        return TRUE;
    }
    return FALSE;
}

void mbev_CapEffRayKill(OMOBJ *obj)
{
    int i;
    CAPEFFRAYWORK *workP;

    for (i = 0; i < 8; i++) {
        if (ev_CapEffRayOMObj[i] == obj) {
            break;
        }
    }
    workP = omObjGetDataAs(obj, CAPEFFRAYWORK);
    ev_CapEffRayOMObj[workP->objIdx] = (OMOBJ *)-1;
}

void mbev_CapEffMasuHitKill(OMOBJ *obj)
{
    int i;
    CAPEFFMASUHITWORK *workP;

    for (i = 0; i < 8; i++) {
        if (ev_CapEffMasuHitOMObj[i] == obj) {
            break;
        }
    }
    workP = omObjGetDataAs(obj, CAPEFFMASUHITWORK);
    ev_CapEffMasuHitOMObj[workP->objIdx] = (OMOBJ *)-1;
}

void mbev_CapEffCoinGlowSet(OMOBJ *obj, BOOL glowF)
{
    CAPEFFCOINWORK *workP;
    int objIdx;
    int i;

    for (objIdx = 0; objIdx < 8; objIdx++) {
        if (ev_CapEffCoinOMObj[objIdx] == obj) {
            break;
        }
    }
    workP = omObjGetDataAs(obj, CAPEFFCOINWORK);
    for (i = 0; i < 128; i++, workP++) {
        workP->glowF = glowF;
    }
}

void mbev_CapPlayerRotate(int playerNo, float angle)
{
    mbPlayerRotateStart(playerNo, angle, 15);
    while (mbPlayerRotateCheck(playerNo) == FALSE) {
        HuPrcVSleep();
    }
}

void mbev_CapEffColorSet(GXColor *color, int colorNo)
{
    if (colorNo < 0) {
        colorNo *= -1;
    }
    *color = ev_CapsuleRandomColorTbl[colorNo % 7];
}
