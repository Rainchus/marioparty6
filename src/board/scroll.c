#include "game/board/main.h"

#include "game/board/audio.h"
#include "game/board/camera.h"
#include "game/board/masu.h"
#include "game/board/object.h"
#include "game/board/pause.h"
#include "game/board/status.h"
#include "game/board/window.h"
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
static int lbl_802C0DD8;
static float mapViewZoom;

static void ScrollCreate(u32 dataNum);
static void ScrollKill(void);
static BOOL ScrollMain(int playerNo);
static BOOL ScrollExec(int playerNo, s16 starMasuId);
static void RotateScrollView(HuVecF *rot, HuVecF *pos, HuVecF *posOut);
static s16 StarMasuGet(int playerNo);
static void InitScrollCol(void);
static void MapViewCreate(void);
static void MapViewKill(void);
static BOOL MapViewExec(int playerNo);
static void MapSprCreate(int type, int id, int layer);
static BOOL MapSprPlayerCol(void);
static void MapSprPlayerColAll(void);

extern void mbWipeDissolveFadeOut(void);
extern void mbWipeDissolveFadeIn(void);
extern BOOL mbWipeSpecialStatGet(void);
extern void mbWipeSpecialFadeInCreate(int type, BOOL pauseF);

void mbScrollInit(int dataNum)
{
    ScrollCreate(mbObjDataNumGet(dataNum));
    MapViewCreate();
}

void mbScrollClose(void)
{
    ScrollKill();
    MapViewKill();
}

void mbev_Scroll(int playerNo, BOOL mapF)
{
    int cameraStackNo;
    BOOL result;
    BOOL pauseDisableF;

    pauseDisableF = mbPauseDisableGet();
    mbPauseDisableSet(TRUE);
    mbStatusDispBackup();
    mbStatusMasuDispSet(FALSE);
    mbMusParamSet(MB_MUS_CHAN_BG, 96, 500);
    cameraStackNo = mbCameraStackPush();
    lbl_802C0DD8 = 0;
    while (TRUE) {
        if (mapF == FALSE) {
            result = ScrollMain(playerNo);
            if (result == FALSE) {
                break;
            }
            mapF = TRUE;
        } else {
            result = MapViewExec(playerNo);
            if (result == FALSE) {
                break;
            }
            mapF = FALSE;
        }
        mbAudFXPlay(1);
    }
    mbAudFXPlay(3);
    mbMusParamSet(MB_MUS_CHAN_BG, 127, 100);
    mbCameraStackIdxSet(cameraStackNo, -1);
    mbStatusDispRestoreForce();
    mbStatusMasuDispSet(TRUE);
    mbWipeDissolveFadeIn();
    mbPauseDisableSet(pauseDisableF);
}

static void ScrollCreate(u32 dataNum)
{
    if (dataNum == 0) {
        scrollColModel = HU3D_MODELID_NONE;
        scrollColTriNum = 0;
        scrollColTriData = NULL;
    } else {
        scrollColModel = Hu3DModelCreate(HuDataSelHeapReadNum(dataNum, HU_MEMNUM_OVL, HEAP_MODEL));
        InitScrollCol();
        Hu3DModelDispOff(scrollColModel);
    }
    scrollStarFindFunc = StarMasuGet;
    scrollHook = NULL;
}

static void ScrollKill(void)
{
    HSF_FACE *triData;

    if (scrollColModel >= 0) {
        Hu3DModelKill(scrollColModel);
        scrollColModel = HU3D_MODELID_NONE;
    }
    if (scrollColTriData) {
        triData = scrollColTriData;
        HuMemDirectFree(triData);
        scrollColTriData = NULL;
    }
}

static BOOL ScrollMain(int playerNo)
{
    BOOL result;
    s16 winNo;
    s16 starMasuId;

    if (mbWipeSpecialStatGet() == FALSE) {
        mbWipeDissolveFadeOut();
        mbStatusDispForceSetAll(FALSE);
    }
    if (scrollHook) {
        scrollHook(TRUE);
    }
    if (scrollStarFindFunc) {
        starMasuId = scrollStarFindFunc(playerNo);
    } else {
        starMasuId = 0;
    }
    if (GWPartyGet() != FALSE) {
        if (starMasuId > 0) {
            winNo = mbWinCreateHelp(0x0026000B);
        } else {
            winNo = mbWinCreateHelp(0x00260005);
        }
    } else {
        winNo = mbWinCreateHelp(0x0026000F);
    }
    result = ScrollExec(playerNo, starMasuId);
    if (result == FALSE) {
        mbWipeDissolveFadeOut();
    } else {
        mbWipeSpecialFadeInCreate(4, TRUE);
    }
    mbWinKill(winNo);
    if (scrollHook) {
        scrollHook(FALSE);
    }
    return result;
}

static void RotateScrollView(HuVecF *rot, HuVecF *pos, HuVecF *posOut)
{
    posOut->x = pos->x + (HuSin(rot->y) * (pos->y / (HuSin(rot->x) / HuCos(rot->x))));
    posOut->z = pos->z + (HuCos(rot->y) * (pos->y / (HuSin(rot->x) / HuCos(rot->x))));
    posOut->y = 0.0f;
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

static void MapSprPlayerColAll(void)
{
    int i;

    for (i = 0; i < 50; i++) {
        if (MapSprPlayerCol() == FALSE) {
            break;
        }
    }
}

void mbScrollStarFindFuncSet(MBSCROLLSTARFINDFUNC findFunc)
{
    scrollStarFindFunc = findFunc;
}

void mbMapCameraSet(const HuVecF *rot, const HuVecF *pos, float zoom)
{
    if (rot) {
        mapViewRot = *rot;
    }
    if (pos) {
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

void mbMapSprAdd(int type, int id)
{
    if (type >= 16) {
        MapSprCreate(type, id, 0);
    } else {
        MapSprCreate(type, id, 5);
    }
}

void mbev_ScrollCapsule(int playerNo)
{
    mbev_Scroll(playerNo, FALSE);
}
