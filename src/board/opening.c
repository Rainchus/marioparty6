#include "game/board/opening.h"
#include "game/board/camera.h"
#include "game/board/main.h"
#include "game/board/masu.h"
#include "game/board/object.h"
#include "game/flag.h"
#include "game/gamework.h"
#include "game/hu3d.h"

#include "dolphin/mtx.h"
#include "dolphin/pad.h"

extern float mbHermiteCalcSlope(float a, float b, float c, float d, float t);
extern void ev_OpeningParty(void);

static HuVecF openingRot;
static HuVecF openingPos;
static HuVecF openingCameraRestoreRot = { -20.0f, 0.0f, 0.0f };
static HuVecF openingCameraRestorePos = { 0.0f, 250.0f, 0.0f };
static HUPROCESS *openingProc;
static int openingPadDelay[GW_PLAYER_MAX];
static MBMODELID openingGuideObjId = -1;
static void (*openingStarInstHook)(void);
static void (*openingInstHook)(void);
static float openingZoom;

static void ev_OpeningPartyKill(void);

static float OpeningCurveEval(HuVecF *a, HuVecF *b, HuVecF *c, HuVecF *d, float t)
{
    HuVecF vec;

    vec.x = mbHermiteCalcSlope(a->x, b->x, c->x, d->x, t);
    vec.y = mbHermiteCalcSlope(a->y, b->y, c->y, d->y, t);
    vec.z = mbHermiteCalcSlope(a->z, b->z, c->z, d->z, t);
    return VECMag(&vec);
}

void mbev_Opening(void)
{
    BOOL partyF = GWPartyGet();

    if (!partyF) {
        mbev_OpeningSingle();
    } else {
        mbev_OpeningParty();
    }
}

void mbev_OpeningParty(void)
{
    if (!_CheckFlag(FLAG_BOARD_TUTORIAL)) {
        openingProc = HuPrcChildCreate(ev_OpeningParty, 0x200C, 0x4000, 0, mbMainProc);
    }
    HuPrcDestructorSet2(openingProc, ev_OpeningPartyKill);
    while (openingProc) {
        HuPrcVSleep();
    }
}

static u16 OpeningPadBtn(int playerNo)
{
    if (openingPadDelay[playerNo] != 0 && --openingPadDelay[playerNo] == 0) {
        return PAD_BUTTON_A;
    }
    return 0;
}

static void ev_OpeningPartyKill(void)
{
    openingProc = NULL;
}

void mbOpeningViewSet(HuVecF *rot, HuVecF *pos, float zoom)
{
    if (rot) {
        openingRot = *rot;
    }
    if (pos) {
        openingPos = *pos;
    }
    if (zoom >= 0.0f) {
        openingZoom = zoom;
    }
}

float mbOpeningZoomGet(void)
{
    return openingZoom;
}

void mbOpeningRotGet(HuVecF *rot)
{
    *rot = openingRot;
}

void mbOpeningPosGet(HuVecF *pos)
{
    *pos = openingPos;
}

void mbOpeningInstHookSet(void (*hook)(void))
{
    openingInstHook = hook;
}

void mbOpeningStarInstHookSet(void (*hook)(void))
{
    openingStarInstHook = hook;
}

MBMODELID mbOpeningGuideObjIdGet(void)
{
    return openingGuideObjId;
}

void mbOpeningGuidePosRestore(void)
{
    s16 masuId;
    HuVecF masuPos;
    HuVecF pos;

    masuId = mbMasuFind_AttrIdGet(-1, 0x8000);
    mbMasuPosGet(masuId, &masuPos);
    pos = masuPos;
    pos.z -= 200.0f;
    mbObjPosSetV(openingGuideObjId, &pos);
    mbObjMotionShiftSet(openingGuideObjId, 1, 0.0f, 8.0f, HU3D_MOTATTR_LOOP);
}

void mbOpeningCameraPosRestore(void)
{
    s16 masuId;
    HuVecF masuPos;
    HuVecF pos;

    masuId = mbMasuFind_AttrIdGet(-1, 0x8000);
    mbMasuPosGet(masuId, &masuPos);
    pos = masuPos;
    pos.y -= 50.0f;
    mbCameraMovePos(&pos, &openingCameraRestoreRot, &openingCameraRestorePos, 1800.0f, -1.0f, 0);
}
