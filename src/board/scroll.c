#include "game/board/main.h"

#include "game/board/masu.h"
#include "game/hu3d.h"
#include "game/memory.h"
#include "game/sprite.h"

typedef void (*MBSCROLLHOOK)(BOOL enterF);
typedef s16 (*MBSCROLLSTARFINDFUNC)(int playerNo);

static HuVecF mapViewPos;
static HuVecF mapViewRot;

static MBSCROLLHOOK mapHook;
static ANIMDATA *pathAnim;
static ANIMDATA *masuMapAnim;
static MBSCROLLHOOK scrollHook;
static HSF_FACE *scrollColTriData;
static int scrollColTriNum;
static MBSCROLLSTARFINDFUNC scrollStarFindFunc;
static HU3D_MODELID scrollColModel;
static float mapViewZoom;

static void ScrollKill(void);
static s16 StarMasuGet(int playerNo);
static void MapViewCreate(void);
static void MapViewKill(void);

void mbScrollClose(void)
{
    ScrollKill();
    MapViewKill();
}

static void ScrollKill(void)
{
    if (scrollColModel >= 0) {
        Hu3DModelKill(scrollColModel);
        scrollColModel = HU3D_MODELID_NONE;
    }
    if (scrollColTriData != NULL) {
        HuMemDirectFree(scrollColTriData);
        scrollColTriData = NULL;
    }
}

static s16 StarMasuGet(int playerNo)
{
    return mbMasuFind_TypeIdGet(GwPlayer[playerNo].masuId, 7, TRUE, TRUE);
}

static void MapViewCreate(void)
{
    mapViewZoom = 20000.0f;
    mapViewPos.x = mapViewPos.y = mapViewPos.z = 0.0f;
    mapViewRot.x = -78.0f;
    mapViewRot.y = 0.0f;
    mapViewRot.z = 0.0f;
    mapHook = NULL;

    masuMapAnim = HuSprAnimDataRead(DATANUM(DATA_bmasu, 2));
    HuSprAnimLock(masuMapAnim);
    pathAnim = HuSprAnimDataRead(DATANUM(DATA_bmasu, 6));
    HuSprAnimLock(pathAnim);
    HuDataDirClose(DATA_bmasu);
}

static void MapViewKill(void)
{
    if (masuMapAnim != NULL) {
        HuSprAnimKill(masuMapAnim);
        masuMapAnim = NULL;
    }
    if (pathAnim != NULL) {
        HuSprAnimKill(pathAnim);
        pathAnim = NULL;
    }
}

void mbScrollStarFindFuncSet(MBSCROLLSTARFINDFUNC findFunc)
{
    scrollStarFindFunc = findFunc;
}

void mbMapCameraSet(const HuVecF *rot, const HuVecF *pos, float zoom)
{
    if (rot != NULL) {
        mapViewRot = *rot;
    }
    if (pos != NULL) {
        mapViewPos = *pos;
    }
    if (zoom >= 0.0f) {
        mapViewZoom = zoom;
    }
}

void mbMapHookSet(MBSCROLLHOOK hook)
{
    mapHook = hook;
}

void mbScrollHookSet(MBSCROLLHOOK hook)
{
    scrollHook = hook;
}
