#include "game/board/main.h"

#include "datanum/effect.h"
#include "game/board/audio.h"
#include "game/board/camera.h"
#include "game/board/comchoice.h"
#include "game/board/effect.h"
#include "game/board/masu.h"
#include "game/board/object.h"
#include "game/board/opening.h"
#include "game/board/pause.h"
#include "game/board/player.h"
#include "game/board/status.h"
#include "game/board/window.h"
#include "game/flag.h"
#include "game/hsfex.h"
#include "game/memory.h"
#include "game/sprite.h"

#include "dolphin/os/OSFastCast.h"
#include "humath.h"

#define STAR_OBJ_MAX 999

enum {
    STAR_MODE_GROW,
    STAR_MODE_IDLE,
    STAR_MODE_SHRINK_IDLE,
    STAR_MODE_SHRINK,
    STAR_MODE_SHRINK_IDLE_DONE
};

enum {
    STAR_DISP_MODE_ON,
    STAR_DISP_MODE_MAIN,
    STAR_DISP_MODE_NONE,
    STAR_DISP_MODE_OFF,
    STAR_DISP_MODE_KILL
};

typedef struct StarWork {
    unsigned killF : 1;
    unsigned rotateF : 1;
    unsigned effectDispF : 1;
    unsigned signF : 1;
    unsigned modelDispF : 1;
    unsigned autoDispF : 1;
    unsigned scaleF : 1;
    unsigned : 1;
    int objNo;
    int no;
    int masuId;
    int playerNo;
    s8 mode;
    s8 rotSpeed;
    s16 rotY;
    HU3D_MODELID effectModelId;
    s16 effectTime;
    float baseY;
    s16 time;
    s16 time2;
    ANIMDATA *effectAnim;
    int signModelId;
    OMOBJ *obj;
    HuVecF pos;
    HuVecF offset;
    HuVecF rot;
    HuVecF scale;
} STARWORK;

typedef struct StarDispWork {
    int modelId[4];
    int playerNo;
    int num;
    int mode;
    int unk1C;
    u16 time;
    u16 delay;
    HuVecF pos;
    HuVecF modelPos[4];
} STARDISPWORK;

static OMOBJ *starOMObj[STAR_OBJ_MAX];
static OMOBJ *starDispObj[4];
static OMOBJ *ztarOMObj[STAR_OBJ_MAX];

static int starAddNum = 1;
static s8 starGuideMotTbl[] = { 3, 12, 11, 23, -1 };
static s8 starMasuGuideMotTbl[] = { 3, 12, 11, 23, -1 };
static void (*starMasuFunc)(void);
static void (*starMoveHook)(void);
static ANIMDATA *starEffAnim2;
static ANIMDATA *starEffAnim1;
static OMOBJ *starGuideObj;
static int ztarMasuNext;
static int starMasuNext;
static int starMin;
static int lbl_802C0E88;
static int starMasuPrevType;
static int starNum;
static HUPROCESS *starFreeProc;
static HUPROCESS *starMasuProc;

static HuVecF starGuidePos = { -0.73f, -0.77f, -750.0f };

static int numberFileTbl[] = {
    DATANUM(DATA_board, 11),
    DATANUM(DATA_board, 12),
    DATANUM(DATA_board, 13),
    DATANUM(DATA_board, 14),
    DATANUM(DATA_board, 15),
    DATANUM(DATA_board, 16),
    DATANUM(DATA_board, 17),
    DATANUM(DATA_board, 18),
    DATANUM(DATA_board, 19),
    DATANUM(DATA_board, 20),
};

static HuVecF starMasuGuidePos = { -0.73f, -0.77f, -750.0f };

static const int SignMdlTbl[] = {
    DATANUM(DATA_board, 21),
    DATANUM(DATA_board, 22),
};

static void StarObjKill(OMOBJ *obj);
static int StarObjCreate(HuVecF *pos);
static void StarObjOMExec(OMOBJ *obj);
static void StarObjRotate(STARWORK *work, OMOBJ *obj);
static void StarAddAllProc(int *addNum, BOOL fastF, int *result);
static void StarDispUpdate(OMOBJ *obj);
static void StarDispObjUpdate(STARDISPWORK *work, int modelNo,
    float x, float y, float z, float scale);
static void StarObjShrinkIdleSet(OMOBJ *obj);
static void StarObjGrowSet(OMOBJ *obj);
static void StarObjShrinkSet(OMOBJ *obj);
static void StarObjGrowWait(OMOBJ *obj);
static void StarObjShrinkIdleWait(OMOBJ *obj);
static HU3D_MODELID StarObjEffCreate(ANIMDATA *anim);
static void StarObjEffKill(HU3D_MODELID modelId);
static void StarObjEffHook(HU3D_MODEL *modelP, MBPARTICLE *particleP, Mtx mtx);
static void StarPauseHook(BOOL pauseF);
static void StarMatHook(HU3D_DRAW_OBJ *drawObj, HSF_MATERIAL *material);
static void ZtarObjKill(OMOBJ *obj);
static int ZtarObjCreate(HuVecF *pos);
static void ZtarObjOMExec(OMOBJ *obj);
static void ZtarObjRotate(STARWORK *work, OMOBJ *obj);
static void ZtarObjShrinkIdleSet(OMOBJ *obj);
static void ZtarObjGrowSet(OMOBJ *obj);
static void ZtarObjShrinkSet(OMOBJ *obj);
static void ZtarObjGrowWait(OMOBJ *obj);
static void ZtarObjShrinkIdleWait(OMOBJ *obj);
static void ev_StarMasu(void);
static void ev_StarFreeMasu(void);
static void ev_StarMasuKill(void);
static void ev_StarFreeMasuKill(void);
static void ev_StarMasuRun(BOOL freeF);

void mbZtarObjInit(void);
void mbZtarObjClose(void);
void mbStarGetMain(int playerNo, HuVecF *pos, int num, BOOL focusF);
int mbStarDispCreate(int playerNo, HuVecF *pos, int num);
void mbZtarGetMain(int playerNo, HuVecF *pos, int num, BOOL focusF);
void mbStarMapViewProcExec(void);
void mbStarFlagReset(int no);
void mbStarNextNoSet(s8 nextNo);
void mbCoinAddExec(int playerNo, int coin);
void mbWipeFadeOut(void);
void mbWipeFadeIn(void);
void mbNormPosto3D(HuVecF *src, s16 cameraMask, HuVecF *dst);
void mbev_StarScroll(HuVecF *startPos, HuVecF *endPos, int time);
int mbGuideNoGet(void);
OMOBJ *mbGuideCreateFlag(HuVecF *pos, s8 *motTbl, BOOL screenF,
    BOOL altMtxF, BOOL layerF);
void mbGuideMotionNextSet(OMOBJ *obj, s16 motNo);
void mbGuideMotionShiftSet(OMOBJ *obj, s16 motNo, BOOL shiftF);
void mbGuideKill(OMOBJ *obj);
/* This owner consumes the target's signed 16-bit guide-model return view. */
MBMODELID mbGuideModelGet(OMOBJ *obj);
float mbSinDeg(float angle);
float mbAngleWrap(float angle);
int mbStatTeamMinValGet(int teamNo, int value, int max,
    int *addNum, int *result);

static const HuVecF starViewOfs = { 0.0f, 100.0f, 0.0f };
static const HuVecF lbl_8021AB24 = { 0.5f, -0.5f, -1.0f };

void mbStarReset(void)
{
    lbl_802C0E88 = 0;
}

void mbStarInit(void)
{
    memset(starOMObj, 0, sizeof(starOMObj));
    starMin = starNum - 1;
    if (starMin < 1) {
        starMin = 1;
    }
    starEffAnim1 = HuSprAnimRead(HuDataReadNum(EFFECT_ANM_glow, HU_MEMNUM_OVL));
    HuSprAnimLock(starEffAnim1);
    starEffAnim2 = HuSprAnimRead(HuDataReadNum(EFFECT_ANM_glow, HU_MEMNUM_OVL));
    HuSprAnimLock(starEffAnim2);
    starMasuProc = NULL;
    starFreeProc = NULL;
    starMoveHook = NULL;
    starMasuFunc = NULL;
    mbZtarObjInit();
}

void mbStarClose(void)
{
    int i;

    for (i = 0; i < STAR_OBJ_MAX; i++) {
        if (starOMObj[i]) {
            StarObjKill(starOMObj[i]);
            starOMObj[i] = NULL;
        }
    }
    if (starEffAnim1) {
        HuSprAnimKill(starEffAnim1);
        starEffAnim1 = NULL;
    }
    if (starEffAnim1) {
        HuSprAnimKill(starEffAnim1);
        starEffAnim1 = NULL;
    }
    mbZtarObjClose();
}

int mbStarObjCreate(void)
{
    int objNo;
    OMOBJ *obj;
    STARWORK *work;

    objNo = StarObjCreate(NULL);
    obj = starOMObj[objNo];
    work = obj->data;
    work->masuId = work->playerNo = -1;
    work->signF = 2;
    return objNo;
}

void mbStarObjPosSet(int objNo, float x, float y, float z)
{
    OMOBJ *obj = starOMObj[objNo];
    STARWORK *work = obj->data;

    work->pos.x = x;
    work->pos.y = y;
    work->pos.z = z;
}

void mbStarObjPosSetV(int objNo, const HuVecF *pos)
{
    mbStarObjPosSet(objNo, pos->x, pos->y, pos->z);
}

void mbStarObjRotSet(int objNo, float x, float y, float z)
{
    OMOBJ *obj = starOMObj[objNo];
    STARWORK *work = obj->data;

    work->rot.x = x;
    work->rot.y = y;
    work->rot.z = z;
}

void mbStarObjRotSetV(int objNo, const HuVecF *rot)
{
    mbStarObjRotSet(objNo, rot->x, rot->y, rot->z);
}

void mbStarObjScaleSet(int objNo, float x, float y, float z)
{
    OMOBJ *obj = starOMObj[objNo];
    STARWORK *work = obj->data;

    work->scale.x = x;
    work->scale.y = y;
    work->scale.z = z;
}

void mbStarObjScaleSetV(int objNo, const HuVecF *scale)
{
    mbStarObjScaleSet(objNo, scale->x, scale->y, scale->z);
}

void mbStarObjPosGet(int objNo, HuVecF *pos)
{
    OMOBJ *obj = starOMObj[objNo];
    STARWORK *work = obj->data;

    *pos = work->pos;
}

void mbStarObjRotGet(int objNo, HuVecF *rot)
{
    OMOBJ *obj = starOMObj[objNo];
    STARWORK *work = obj->data;

    *rot = work->rot;
}

void mbStarObjScaleGet(int objNo, HuVecF *scale)
{
    OMOBJ *obj = starOMObj[objNo];
    STARWORK *work = obj->data;

    *scale = work->scale;
}

void mbStarObjKill(int objNo)
{
    StarObjKill(starOMObj[objNo]);
}

void mbStarObjDispSet(int objNo, BOOL dispF)
{
    OMOBJ *obj = starOMObj[objNo];
    STARWORK *work;

    mbObjDispSet(obj->mdlId[0], dispF);
    work = obj->data;
    work->modelDispF = dispF;
    work->effectDispF = dispF;
}

void mbStarObjDispFlagSet(int objNo, BOOL dispF)
{
    OMOBJ *obj = starOMObj[objNo];
    STARWORK *work = obj->data;

    work->modelDispF = dispF;
}

void mbStarChestCreate(int objNo, int playerNo)
{
    OMOBJ *obj = starOMObj[objNo];
    STARWORK *work = obj->data;
    HuVecF pos;

    work->signF = FALSE;
    work->playerNo = playerNo;
    mbPlayerPosGet(playerNo, &pos);
    work->baseY = pos.y;
    mbStarGetMain(playerNo, NULL, starAddNum, TRUE);
}

static int StarObjCreate(HuVecF *pos)
{
    int i;

    for (i = 0; i < STAR_OBJ_MAX; i++) {
        if (starOMObj[i] == NULL) {
            OMOBJ *obj;
            STARWORK *work;
            HU3D_MODELID modelId;
            HU3D_MODEL *modelP;
            HSF_DATA *hsf;
            HSF_MATERIAL *material;
            MBPARTICLE *particleP;
            int j;

            obj = omAddObjEx(mbObjMan, 257, 1, 0, OM_GRP_NONE,
                StarObjOMExec);
            starOMObj[i] = obj;
            work = HuMemDirectMallocNum(HEAP_HEAP, sizeof(STARWORK),
                HU_MEMNUM_OVL);
            obj->data = work;
            memset(work, 0, sizeof(STARWORK));
            work->obj = obj;
            obj->mdlId[0] = mbObjCreate(
                mbBoardDataNumGet(DATANUM(DATA_board, 5)), NULL, TRUE);
            modelId = mbObjModelIDGet(obj->mdlId[0]);
            modelP = &Hu3DData[modelId];
            hsf = modelP->hsf;
            material = hsf->material;
            modelP->hiliteIdx = 0;
            Hu3DModelMatHookSet(modelId, StarMatHook);
            for (j = 0; j < hsf->materialNum; j++, material++) {
                material->flags |= HSF_MATERIAL_MATHOOK;
            }
            mbObjRotSet(obj->mdlId[0], -90.0f, 0.0f, 0.0f);
            mbObjZWriteOffSet(obj->mdlId[0], FALSE);
            mbObjLayerSet(obj->mdlId[0], 3);
            work->objNo = i;
            work->mode = STAR_MODE_IDLE;
            work->effectModelId = StarObjEffCreate(starEffAnim1);
            particleP = Hu3DData[work->effectModelId].hookData;
            particleP->hookData = work;
            work->modelDispF = TRUE;
            work->effectTime = 0;
            work->autoDispF = TRUE;
            work->effectDispF = TRUE;
            work->offset.x = work->offset.y = work->offset.z = 0.0f;
            work->rot.x = work->rot.y = work->rot.z = 0.0f;
            work->scale.x = work->scale.y = work->scale.z = 1.0f;
            omSetStatBit(obj, OM_STAT_MODELPAUSE);
            return i;
        }
    }
    return -1;
}

static inline int StarMasuNoGet(int masuId)
{
    int i;

    for (i = 0; i < STAR_OBJ_MAX; i++) {
        if (starOMObj[i]) {
            STARWORK *work = starOMObj[i]->data;

            if (work->masuId == masuId) {
                return i;
            }
        }
    }
    return -1;
}

static inline OMOBJ *StarMasuObjGet(int masuId)
{
    int objNo = StarMasuNoGet(masuId);

    if (objNo >= 0) {
        return starOMObj[objNo];
    }
    return NULL;
}

void mbStarMoveHookSet(void (*hook)(void))
{
    starMoveHook = hook;
}

void mbStarMasuFuncSet(void (*func)(void))
{
    starMasuFunc = func;
}

void mbStarMasuNextSet(s16 masuId)
{
    HuVecF pos;
    int objNo;
    OMOBJ *obj;
    STARWORK *work;

    starMasuPrevType = mbMasuTypeGet(masuId);
    mbMasuTypeSet(masuId, 7);
    mbMasuPosGet(masuId, &pos);
    pos.y += 300.0f;
    objNo = StarObjCreate(&pos);
    obj = starOMObj[objNo];
    work = obj->data;
    work->masuId = masuId;
    work->playerNo = -1;
    work->signF = TRUE;
    work->signModelId = mbObjCreate(
        mbBoardDataNumGet(DATANUM(DATA_board, 6)), NULL, FALSE);
    mbObjMotionTimeSet(work->signModelId, 0.0f);
    mbObjMotionSpeedSet(work->signModelId, 1.0f);
    mbObjAttrSet(work->signModelId, HU3D_MOTATTR_LOOP);
    mbObjLayerSet(work->signModelId, 3);
    work->baseY = pos.y - 300.0f;
    work->pos = pos;
    mbMasuCapsuleSet(masuId, MASU_NULL);
    starMasuNext = masuId;
}

void mbStarGetExec(int playerNo)
{
    HuVecF pos;
    int objNo;
    OMOBJ *obj;
    STARWORK *work;
    int seNo;

    mbPlayerPosGet(playerNo, &pos);
    pos.y += 300.0f;
    objNo = StarObjCreate(&pos);
    obj = starOMObj[objNo];
    work = obj->data;
    work->playerNo = playerNo;
    work->masuId = -1;
    work->signF = FALSE;
    work->baseY = pos.y - 300.0f;
    work->pos = pos;
    seNo = mbAudFXPlay(1095);
    mbAudFXPlay(1096);
    StarObjGrowSet(obj);
    StarObjGrowWait(obj);
    mbAudFXStop(seNo);
    mbStarGetMain(playerNo, NULL, starAddNum, TRUE);
}

void mbStarAddNumSet(int num)
{
    starAddNum = num;
}

void mbStarStub(void)
{
}

void mbev_StarMasu(int playerNo)
{
    mbMoveNumDispSet(playerNo, FALSE);
    starMasuProc = HuPrcChildCreate(ev_StarMasu, 0x2003, 0x2000, 0, mbMainProc);
    HuPrcDestructorSet2(starMasuProc, ev_StarMasuKill);
    while (starMasuProc != NULL) {
        HuPrcVSleep();
    }
    mbMoveNumDispSet(playerNo, TRUE);
}

void mbev_StarFreeMasu(int playerNo)
{
    mbMoveNumDispSet(playerNo, FALSE);
    starFreeProc = HuPrcChildCreate(ev_StarFreeMasu, 0x2003, 0x2000, 0, mbMainProc);
    HuPrcDestructorSet2(starFreeProc, ev_StarFreeMasuKill);
    while (starFreeProc != NULL) {
        HuPrcVSleep();
    }
    mbMoveNumDispSet(playerNo, TRUE);
}

static void ev_StarMasu(void)
{
    if (starMasuFunc != NULL) {
        starMasuFunc();
    } else {
        ev_StarMasuRun(FALSE);
    }
    HuPrcEnd();
}

static void ev_StarFreeMasu(void)
{
    ev_StarMasuRun(TRUE);
    HuPrcEnd();
}

static void ev_StarMasuKill(void)
{
    starMasuProc = NULL;
}

static void ev_StarFreeMasuKill(void)
{
    starFreeProc = NULL;
}

void mbStarGetMain(int playerNo, HuVecF *pos, int num, BOOL focusF)
{
    int time;
    OMOBJ *obj;
    STARWORK *work;
    int objNo;
    int masuId;
    int seNo;
    int no;
    int streamNo;
    int i;
    HuVecF playerPos;

    masuId = GwPlayer[playerNo].masuId;
    seNo = -1;
    if (pos == NULL) {
        obj = StarMasuObjGet(masuId);
        if (obj == NULL) {
            for (i = 0; i < STAR_OBJ_MAX; i++) {
                if (starOMObj[i]) {
                    STARWORK *objWork = starOMObj[i]->data;

                    if (objWork->playerNo == playerNo) {
                        objNo = i;
                        break;
                    }
                }
            }
            if (i >= STAR_OBJ_MAX) {
                objNo = -1;
            }
            if (objNo >= 0) {
                obj = starOMObj[objNo];
            } else {
                obj = NULL;
            }
        }
        if (obj == NULL) {
            mbPlayerPosGet(playerNo, &playerPos);
            playerPos.y += 300.0f;
            objNo = StarObjCreate(&playerPos);
            obj = starOMObj[objNo];
            work = obj->data;
            work->playerNo = playerNo;
            work->masuId = -1;
            work->signF = FALSE;
            work->baseY = playerPos.y - 300.0f;
            work->pos = playerPos;
            obj = starOMObj[objNo];
        }
        work = obj->data;
        no = work->no;
    } else {
        objNo = StarObjCreate(pos);
        obj = starOMObj[objNo];
        work = obj->data;
        work->playerNo = playerNo;
        work->masuId = -1;
        work->signF = FALSE;
        work->baseY = pos->y - 300.0f;
        work->pos = *pos;
        obj = starOMObj[objNo];
        work = obj->data;
        mbPlayerPosGet(playerNo, &playerPos);
        work->baseY = playerPos.y;
        no = -1;
        seNo = mbAudFXPlay(1095);
        mbAudFXPlay(1096);
        HuPrcVSleep();
    }
    if (work->signF == TRUE && work->effectDispF == FALSE) {
        int growSeNo;

        mbObjDispSet(obj->mdlId[0], TRUE);
        work->modelDispF = TRUE;
        work->effectDispF = TRUE;
        growSeNo = mbAudFXPlay(1095);
        mbAudFXPlay(1096);
        StarObjGrowSet(obj);
        StarObjGrowWait(obj);
        mbAudFXStop(growSeNo);
    }
    _SetFlag(FLAG_BOARD_TURN_NOSTART);
    mbMusFadeOutSpeed(0, 1000);
    HuPrcSleep(60);
    StarObjShrinkIdleSet(obj);
    StarObjShrinkIdleWait(obj);
    mbPlayerMotionSet(playerNo, 11, 0);
    time = 0;
    while (1) {
        if (time++ == 27) {
            StarObjShrinkSet(obj);
        }
        if (time == 30) {
            if (seNo >= 0) {
                mbAudFXStop(seNo);
            }
            mbAudFXPlay(1097);
            ((STARWORK *)starOMObj[work->objNo]->data)->effectDispF = FALSE;
        }
        HuPrcVSleep();
        if (mbPlayerMotionEndCheck(playerNo)) {
            break;
        }
    }
    mbPlayerMotIdleSet(playerNo);
    omVibrate((s16)playerNo, 20, 7, 3);
    streamNo = mbMusJinglePlay(39);
    mbPlayerWinLoseVoicePlay(playerNo, 7, 573);
    mbPlayerMotionShiftSet(playerNo, 7, 0.0f, 8.0f, 0);
    if (focusF) {
        HuVecF cameraRot;
        HuVecF objPos;
        float weight;
        float deltaZoom;
        float zoom;
        float zoomBase;

        mbCameraMoveOnSet(FALSE);
        zoom = 1600.0f;
        mbCameraRotGet(&cameraRot);
        cameraRot.x -= 15.0f;
        mbObjPosGet(obj->mdlId[0], &objPos);
        mbCameraMovePlayer((s16)playerNo, &cameraRot,
            (HuVecF *)&starViewOfs, zoom, -1.0f, 102);
        mbCameraMoveWait();
        zoomBase = mbCameraZoomGet();
        mbCameraFocusObjSet(MB_MODEL_NONE);
        for (time = 0; time < 30u; time++) {
            weight = time / 30.0f;
            deltaZoom = (1 + HuSin(weight * 720.0f)) * 0.5;
            zoom = zoomBase + (-500.0f * deltaZoom);
            mbCameraZoomSet(zoom);
            HuPrcVSleep();
        }
    }
    mbPlayerStarAdd(playerNo, num);
    GwSystem.starTotal = GwSystem.starTotal + num;
    if (no >= 0) {
        mbStarNextNoSet(no);
        mbStarFlagReset(no);
    }
    mbMusJingleWait(streamNo);
    mbPlayerMotionEndWait(playerNo);
    if (focusF) {
        mbCameraMoveWait();
    }
    HuPrcSleep(30);
    _ClearFlag(FLAG_BOARD_TURN_NOSTART);
    if (work->signF == FALSE) {
        for (i = 0; i < STAR_OBJ_MAX; i++) {
            if (starOMObj[i]) {
                STARWORK *objWork = starOMObj[i]->data;

                if (playerNo == objWork->playerNo) {
                    StarObjKill(starOMObj[i]);
                    break;
                }
            }
        }
    } else if (work->signF == TRUE) {
        masuId = GwPlayer[playerNo].masuId;
        for (i = 0; i < STAR_OBJ_MAX; i++) {
            if (starOMObj[i]) {
                STARWORK *objWork = starOMObj[i]->data;

                if (masuId == objWork->masuId) {
                    StarObjKill(starOMObj[i]);
                    break;
                }
            }
        }
    }
    HuPrcSleep(12);
    starAddNum = 1;
}

void mbStarGetPosExec(int playerNo, HuVecF *pos)
{
    mbStarGetMain(playerNo, pos, 1, TRUE);
}

static void StarObjOMExec(OMOBJ *obj)
{
    STARWORK *work = obj->data;
    float rotY;

    if (work->killF || mbExitCheck()) {
        if (work->signF == TRUE) {
            mbObjKill(work->signModelId);
        }
        StarObjEffKill(work->effectModelId);
        work->effectModelId = HU3D_MODELID_NONE;
        work->signF = -1;
        mbObjKill(obj->mdlId[0]);
        obj->mdlId[0] = MB_MODEL_NONE;
        omDelObjEx(HuPrcCurrentGet(), obj);
        starOMObj[work->objNo] = NULL;
        return;
    }

    if (!_CheckFlag(FLAG_BOARD_TUTORIAL) && work->signF == TRUE
        && work->effectDispF == FALSE) {
        BOOL dispF = TRUE;
        s16 masuId = GwPlayer[GwSystem.turnPlayerNo].masuId;

        if (work->masuId == masuId) {
            dispF = FALSE;
        }
        if (work->autoDispF == FALSE) {
            dispF = FALSE;
        }
        if (dispF) {
            mbObjDispSet(obj->mdlId[0], TRUE);
            work->modelDispF = TRUE;
            work->effectDispF = TRUE;
            StarObjGrowSet(obj);
        }
    }

    switch (work->mode) {
        case STAR_MODE_GROW:
        {
            float weight = work->time++ / 90.0f;

            work->scale.x = mbSinDeg(90.0f * weight);
            work->scale.y = work->scale.z = work->scale.x;
            if (work->time > 90) {
                work->rot.y = 0.0f;
                work->rotateF = FALSE;
                work->mode = STAR_MODE_IDLE;
                work->time = 0;
                return;
            }
            break;
        }

        case STAR_MODE_IDLE:
            if (work->signF == TRUE) {
                HuVecF pos;

                mbMasuPosGet(work->masuId, &pos);
                work->pos.x = pos.x;
                work->pos.z = pos.z;
            }
            work->offset.y = 100.0f
                * (0.2f * HuSin(4.0f * work->time));
            work->time++;
            break;

        case STAR_MODE_SHRINK_IDLE:
            work->mode = STAR_MODE_SHRINK_IDLE_DONE;
            break;

        case STAR_MODE_SHRINK:
            work->pos.y += -4.0f;
            if (work->pos.y > work->baseY + 80.0f) {
                if (work->rotY < 90) {
                    work->rotY += 2;
                    if (work->rotY > 90) {
                        work->rotY = 90;
                    }
                }
                rotY = work->rotY;
                work->scale.x = HuSin(rotY + 90.0f);
                if (work->scale.x <= 0.0f) {
                    work->scale.x = 0.001f;
                }
                work->scale.y = work->scale.z = work->scale.x;
            } else {
                work->killF = TRUE;
                mbObjDispSet(obj->mdlId[0], FALSE);
                if (work->signF == TRUE) {
                    mbObjDispSet(work->signModelId, FALSE);
                }
                return;
            }
            break;

        case STAR_MODE_SHRINK_IDLE_DONE:
            break;
    }

    StarObjRotate(work, obj);
    mbObjPosSet(obj->mdlId[0], work->pos.x + work->offset.x,
        work->pos.y + work->offset.y, work->pos.z + work->offset.z);
    mbObjRotSetV(obj->mdlId[0], &work->rot);
    mbObjScaleSetV(obj->mdlId[0], &work->scale);
    if (work->signF == TRUE) {
        mbObjPosSet(work->signModelId, work->pos.x + work->offset.x,
            work->pos.y + work->offset.y,
            work->pos.z + work->offset.z - 10.0f);
        mbObjRotSetV(work->signModelId, &work->rot);
        mbObjScaleSet(work->signModelId, 2.0f * work->scale.x,
            2.0f * work->scale.y, 2.0f * work->scale.z);
    }
}

static void StarObjKill(OMOBJ *obj)
{
    STARWORK *work = obj->data;

    work->killF = TRUE;
}

static void StarObjRotate(STARWORK *work, OMOBJ *obj)
{
    float rotSpeed;

    if (work->rotateF == FALSE) {
        return;
    }
    if (work->rotSpeed < 16.0f) {
        work->rotSpeed += 2.0f;
        if (work->rotSpeed > 16.0f) {
            work->rotSpeed = 16;
        }
    }
    OSs8tof32(&work->rotSpeed, &rotSpeed);
    if (work->mode == STAR_MODE_GROW) {
        work->rot.y = 720.0f * mbSinDeg(work->time);
    } else {
        work->rot.y = mbAngleWrap(work->rot.y + rotSpeed);
    }
}

static void StarObjShrinkIdleSet(OMOBJ *obj)
{
    STARWORK *work = obj->data;

    work->mode = STAR_MODE_SHRINK_IDLE;
    if (work->effectDispF == FALSE) {
        mbObjDispSet(obj->mdlId[0], TRUE);
        work->pos.y = work->baseY + 800.0f;
    }
}

static inline void StarObjGrowSet(OMOBJ *obj)
{
    STARWORK *work = obj->data;

    work->rotateF = TRUE;
    work->rotY = 0;
    work->time = 0;
    work->mode = STAR_MODE_GROW;
    work->modelDispF = TRUE;
    work->offset.y = 0.0f;
    work->effectDispF = TRUE;
    work->autoDispF = TRUE;
    if (work->signF == TRUE) {
        mbObjDispSet(work->signModelId, TRUE);
    }
}

static void StarObjShrinkSet(OMOBJ *obj)
{
    STARWORK *work = obj->data;

    work->rotateF = TRUE;
    work->rotY = 0;
    work->rotSpeed = 0;
    work->mode = STAR_MODE_SHRINK;
}

static void StarObjGrowWait(OMOBJ *obj)
{
    STARWORK *work = obj->data;

    while (work->mode != STAR_MODE_IDLE) {
        HuPrcVSleep();
    }
    HuPrcSleep(20);
}

static void StarObjShrinkIdleWait(OMOBJ *obj)
{
    STARWORK *work = obj->data;

    while (work->mode != STAR_MODE_SHRINK_IDLE_DONE) {
        HuPrcVSleep();
    }
}

static HU3D_MODELID StarObjEffCreate(ANIMDATA *anim)
{
    HU3D_MODELID modelId;

    modelId = mbParticleCreate(anim, 64);
    mbParticleHookSet(modelId, StarObjEffHook);
    Hu3DModelLayerSet(modelId, 5);
    return modelId;
}

static void StarObjEffKill(HU3D_MODELID modelId)
{
    if (modelId >= 0) {
        mbParticleKill(modelId);
    }
}

static void StarObjEffHook(
    HU3D_MODEL *modelP, MBPARTICLE *particleP, Mtx mtx)
{
    STARWORK *work = particleP->hookData;
    GXColor color = { 255, 255, 192, 192 };
    MBPARTICLEDATA *dataP;
    HuVecF dir;
    float rand;
    float weight;
    int i;

    if (particleP->mode == 0) {
        dataP = particleP->data;
        particleP->blendMode = MB_PARTICLE_BLEND_ADDCOL;
        for (i = 0; i < particleP->num; i++, dataP++) {
            dataP->vel.y = 0.0f;
            dataP->scale = 0.0f;
            dataP->color = color;
            dataP->time = 0;
            dataP->activeF = -1;
        }
        particleP->mode = 1;
    }

    dataP = particleP->data;
    for (i = 0; i < particleP->num; i++, dataP++) {
        if (work->modelDispF || dataP->activeF >= 0) {
            if (work->effectDispF == FALSE || work->autoDispF == FALSE) {
                dataP->vel.y = 0.0f;
                dataP->scale = 0.0f;
            } else if (dataP->activeF < 0) {
                rand = frandf();
                dir.x = frandf() - rand;
                rand = frandf();
                dir.y = frandf() - rand;
                rand = frandf();
                dir.z = frandf() - rand;
                VECNormalize(&dir, &dir);
                VECScale(&dir, &dir, 80.0f * work->scale.x);
                dataP->pos.x = work->pos.x + dir.x;
                dataP->pos.y = work->pos.y + dir.y;
                dataP->pos.z = work->pos.z + dir.z;
                dataP->vel.y = 0.0f;
                dataP->scale = 20.0f
                    + (work->scale.x * mbRandMod(20));
                dataP->activeF = 18 + mbRandMod(12);
                dataP->time = 0;
            }

            weight = (float)dataP->time++ / (float)dataP->activeF;
            dataP->pos.y += 0.016666668f * dataP->vel.y;
            dataP->vel.y += -16.333334f;
            dataP->color.a = 255.0f * (1.0f - weight);
            if (dataP->time > dataP->activeF) {
                dataP->scale = 0.0f;
                dataP->activeF = -1;
            }
        }
    }
}

void mbStarStub2(void)
{
}

void mbStarMapViewProcExec(void)
{
    OMOBJ *guideObj = NULL;
    s16 startMasuId;
    int starMasuNum;
    s16 starMasuList[12];
    int i;
    HuVecF startPos;
    HuVecF endPos;
    HuVecF cameraCenter;
    HuVecF cameraRot;
    HuVecF guidePos;
    float cameraZoom;
    int winNo;

    starMasuNum = mbMasuTypeListGet(7, starMasuList);
    if (starMasuNum <= 0) {
        return;
    }
    mbWipeFadeOut();
    mbMusPauseFadeOut(0, TRUE, 1000);
    HuPrcSleep(120);
    for (i = 0; i < 4; i++) {
        mbPlayerMotionShiftSet(i, 1, 0.0f, 1.0f, 0x40000001);
    }
    mbCameraZoomSet(2400.0f);
    mbCameraRotSet(325.0f, 0.0f, 0.0f);
    startMasuId = mbMasuFind_AttrIdGet(-1, 0x8000);
    mbMasuPosGet(startMasuId, &startPos);
    mbMasuPosGet(starMasuList[0], &endPos);
    mbNormPosto3D(&starGuidePos, 4, &guidePos);
    guideObj = mbGuideCreateFlag(&guidePos, starGuideMotTbl,
        TRUE, FALSE, TRUE);
    mbGuideMotionNextSet(guideObj, 1);
    mbStatusDispForceSetAll(FALSE);
    mbMusPlay(1, 12, 127, 0);
    mbWipeFadeIn();
    mbAudGuidePlay(952);
    mbGuideMotionShiftSet(guideObj, 12, TRUE);
    winNo = mbWinCreateTime(4, 0x270010, -1);
    mbWinPause((s16)winNo);
    mbev_StarScroll(&startPos, &endPos, 150);
    mbWinKill((s16)winNo);
    mbAudGuidePlay(952);
    mbGuideMotionShiftSet(guideObj, 12, TRUE);
    mbWinCreateTime(4, 0x27000E, -1);
    mbWinTopWait();
    mbWipeFadeOut();
    mbMusFadeOutSpeed(1, 1000);
    HuPrcSleep(60);
    mbStatusDispForceSetAll(TRUE);
    mbGuideKill(guideObj);
    guideObj = NULL;
    cameraZoom = mbOpeningZoomGet();
    mbOpeningRotGet(&cameraRot);
    mbOpeningPosGet(&cameraCenter);
    mbCameraZoomSet(cameraZoom);
    mbCameraRotSetV(&cameraRot);
    mbCameraCenterSetV(&cameraCenter);
    mbMusPauseFadeOut(0, FALSE, 1000);
    mbWipeFadeIn();
}

void mbStarMapViewExec(int playerNo, int no)
{
}

void mbStarNumSet(int num)
{
    starNum = num;
}

int mbStarNumGet(void)
{
    return starNum;
}

void mbStarFlagInit(void)
{
    GwSystem.starFlag = 0;
}

void mbStarFlagSet(int no)
{
    GwSystem.starFlag |= (1 << no);
}

void mbStarFlagReset(int no)
{
    GwSystem.starFlag &= ~(1 << no);
}

u8 mbStarFlagGet(void)
{
    return GwSystem.starFlag;
}

void mbStarNextNoSet(s8 nextNo)
{
    GwSystem.starPos = nextNo;
}

u8 mbStarNextNoGet(void)
{
    return GwSystem.starPos;
}

void mbStarNoSet(int no)
{
}

int mbStarNoRandGet(void)
{
    return -1;
}

void mbStarNoDispSet(void)
{
}

void mbStarMasuDispSet(int masuId, BOOL dispF)
{
    int objNo = StarMasuNoGet(masuId);

    if (objNo >= 0 && starOMObj[objNo]) {
        mbStarObjDispSet(objNo, dispF);
    }
}

void mbStarDispSetAll(BOOL dispF)
{
    int i;

    for (i = 0; i < STAR_OBJ_MAX; i++) {
        if (starOMObj[i]) {
            mbStarObjDispSet(i, dispF);
        }
    }
}

void mbStarObjDispSetAll(BOOL dispF)
{
    int i;

    for (i = 0; i < STAR_OBJ_MAX; i++) {
        if (starOMObj[i]) {
            STARWORK *work = starOMObj[i]->data;

            work->autoDispF = dispF;
        }
        if (starOMObj[i]) {
            mbStarObjDispSet(i, dispF);
        }
    }
}

int mbStarAddProcExec(int playerNo, int starNum, BOOL dispF, BOOL fastF)
{
    int starDiff;
    int i;
    int delay;
    int starChg;
    int starNew;
    s16 seId;

    starDiff = 0;
    if (starNum != 0) {
        if (abs(starNum) >= 50) {
            delay = 1;
        } else if (abs(starNum) >= 20) {
            delay = 3;
        } else {
            delay = 6;
        }
        starNew = starNum + mbPlayerStarGet(playerNo);
        if (starNew > 999) {
            starNum = 999 - mbPlayerStarGet(playerNo);
        } else if (starNew < 0) {
            starNum = -mbPlayerStarGet(playerNo);
        }
        starDiff = starNum;
        if (!fastF) {
            starChg = (starNum >= 0) ? 1 : -1;
            seId = (starChg > 0) ? 8 : 50;
            for (i = 0; i < abs(starNum); i++) {
                mbPlayerStarAdd(playerNo, starChg);
                mbAudFXPlay(seId);
                HuPrcSleep(delay);
            }
        } else {
            mbPlayerStarAdd(playerNo, starDiff);
        }
    }
    if (starDiff != 0 || dispF) {
        mbAudFXPlay(15);
        mbStarDispPlayerCreate(playerNo, starDiff);
        while (!mbStarDispCheck(playerNo)) {
            HuPrcVSleep();
        }
    }
    return starDiff;
}

int mbStarAddDispExec(int playerNo, int starNum, BOOL dispF, BOOL fastF)
{
    int starDiff;
    int i;
    int delay;
    int starChg;
    int starNew;
    s16 seId;

    if (abs(starNum) >= 50) {
        delay = 1;
    } else if (abs(starNum) >= 20) {
        delay = 3;
    } else {
        delay = 6;
    }
    starNew = starNum + mbPlayerStarGet(playerNo);
    if (starNew > 999) {
        starNum = 999 - mbPlayerStarGet(playerNo);
    } else if (starNew < 0) {
        starNum = -mbPlayerStarGet(playerNo);
    }
    starDiff = starNum;
    if (!fastF) {
        starChg = (starNum >= 0) ? 1 : -1;
        seId = (starChg > 0) ? 8 : 50;
        for (i = 0; i < abs(starNum); i++) {
            mbPlayerStarAdd(playerNo, starChg);
            mbAudFXPlay(seId);
            HuPrcSleep(delay);
        }
    } else {
        mbPlayerStarAdd(playerNo, starDiff);
    }
    if (starDiff != 0) {
        mbAudFXPlay(15);
    }
    if (dispF && starDiff != 0) {
        mbStarDispPlayerCreate(playerNo, starDiff);
        while (!mbStarDispCheck(playerNo)) {
            HuPrcVSleep();
        }
    }
    return starDiff;
}

int mbStarAddExec(int playerNo, int starNum)
{
    int starDiff;
    int i;
    int delay;
    int starChg;
    int starNew;
    s16 seId;

    if (abs(starNum) >= 50) {
        delay = 1;
    } else if (abs(starNum) >= 20) {
        delay = 3;
    } else {
        delay = 6;
    }
    starNew = starNum + mbPlayerStarGet(playerNo);
    if (starNew > 999) {
        starNum = 999 - mbPlayerStarGet(playerNo);
    } else if (starNew < 0) {
        starNum = -mbPlayerStarGet(playerNo);
    }
    starDiff = starNum;
    starChg = (starNum >= 0) ? 1 : -1;
    seId = (starChg > 0) ? 8 : 50;
    for (i = 0; i < abs(starNum); i++) {
        mbPlayerStarAdd(playerNo, starChg);
        mbAudFXPlay(seId);
        HuPrcSleep(delay);
    }
    if (starDiff != 0) {
        mbAudFXPlay(15);
    }
    return starDiff;
}

static void StarAddAllProc(int *addNum, BOOL fastF, int *result)
{
    int starNum[4];
    int delay[4];
    int time[4];
    int starChg[4];
    BOOL activeF[4];
    int playerNum;
    int i;

    for (i = 0; i < 4; i++) {
        starNum[i] = addNum[i];
        activeF[i] = FALSE;
    }
    if (!GWTeamFGet()) {
        for (i = 0; i < 4; i++) {
            int starNew = starNum[i] + mbPlayerStarGet(i);

            if (starNew > 999) {
                starNum[i] = 999 - mbPlayerStarGet(i);
            } else if (starNew < 0) {
                starNum[i] = -mbPlayerStarGet(i);
            }
            result[i] = starNum[i];
            activeF[i] = TRUE;
        }
    } else {
        for (i = 0; i < 2; i++) {
            playerNum = mbPlayerTeamFindPlayer(i, 0);
            activeF[playerNum] = TRUE;
            starNum[playerNum] = mbStatTeamMinValGet(i,
                mbPlayerGrpStarGet(i), 999, addNum, result);
        }
    }
    if (!fastF) {
        for (i = 0; i < 4; i++) {
            if (!activeF[i]) {
                continue;
            }
            if (abs(starNum[i]) >= 50) {
                delay[i] = 1;
            } else if (abs(starNum[i]) >= 20) {
                delay[i] = 3;
            } else {
                delay[i] = 6;
            }
            time[i] = delay[i];
            starChg[i] = (starNum[i] >= 0) ? 1 : -1;
        }
        do {
            playerNum = 0;
            for (i = 0; i < 4; i++) {
                if (!activeF[i] || starNum[i] == 0) {
                    continue;
                }
                if (--time[i] == 0) {
                    mbPlayerStarAdd(i, starChg[i]);
                    starNum[i] -= starChg[i];
                    mbAudFXPlay(8);
                    time[i] = delay[i];
                }
                playerNum++;
            }
            HuPrcVSleep();
        } while (playerNum != 0);
    } else {
        for (i = 0; i < 4; i++) {
            if (activeF[i]) {
                mbPlayerStarAdd(i, starNum[i]);
            }
        }
    }
    mbAudFXPlay(15);
}

void mbStarAddAllProcExecV(int *addNum, BOOL *dispF, BOOL fastF)
{
    int result[4];
    BOOL activeF[4];
    BOOL waitF;
    int i;

    StarAddAllProc(addNum, fastF, result);
    for (i = 0; i < 4; i++) {
        activeF[i] = FALSE;
        if (result[i] != 0 || dispF[i]) {
            mbStarDispPlayerCreate(i, result[i]);
            activeF[i] = TRUE;
        }
    }
    do {
        waitF = FALSE;
        for (i = 0; i < 4; i++) {
            if (activeF[i] && !mbStarDispCheck(i)) {
                waitF = TRUE;
            }
        }
        HuPrcVSleep();
    } while (waitF);
}

void mbStarAddAllProcExec(int num0, int num1, int num2, int num3,
    BOOL dispF, BOOL fastF)
{
    int addNum[4];
    int result[4];
    BOOL activeF[4];
    BOOL waitF;
    int i;

    addNum[0] = num0;
    addNum[1] = num1;
    addNum[2] = num2;
    addNum[3] = num3;
    StarAddAllProc(addNum, fastF, result);
    if (dispF) {
        for (i = 0; i < 4; i++) {
            if (result[i] != 0) {
                mbStarDispPlayerCreate(i, result[i]);
                activeF[i] = TRUE;
            }
        }
        do {
            waitF = FALSE;
            for (i = 0; i < 4; i++) {
                if (activeF[i] && !mbStarDispCheck(i)) {
                    waitF = TRUE;
                }
            }
            HuPrcVSleep();
        } while (waitF);
    }
}

void mbStarAddAllExec(int num0, int num1, int num2, int num3)
{
    int addNum[4];
    int result[4];

    addNum[0] = num0;
    addNum[1] = num1;
    addNum[2] = num2;
    addNum[3] = num3;
    StarAddAllProc(addNum, FALSE, result);
}

int mbStarDispPlayerCreate(int playerNo, int num)
{
    HuVecF pos;

    mbPlayerPosGet(playerNo, &pos);
    pos.y += 250.0f;
    mbStarDispCreate(playerNo, &pos, num);
    return playerNo;
}

int mbStarDispCreate(int playerNo, HuVecF *pos, int num)
{
    OMOBJ *obj;
    STARDISPWORK *work;
    HU3D_MODELID modelId;
    HU3D_MODEL *modelP;
    HSF_DATA *hsf;
    HSF_MATERIAL *material;
    HuVecF dispPos;
    BOOL negativeF;
    int digit10;
    int digit1;
    float motTime;
    int i;

    if (pos == NULL) {
        mbPlayerPosGet(playerNo, &dispPos);
        dispPos.y += 250.0f;
    } else {
        dispPos = *pos;
    }
    negativeF = (num < 0) ? TRUE : FALSE;
    digit10 = abs(num) / 10;
    digit1 = abs(num) - (digit10 * 10);
    if (digit10 > 9) {
        digit10 = 9;
    }
    if (digit1 > 9) {
        digit1 = 9;
    }
    motTime = negativeF ? 2.5f : 1.5f;
    obj = omAddObjEx(mbObjMan, -32768, 0, 0, OM_GRP_NONE,
        StarDispUpdate);
    starDispObj[playerNo] = obj;
    work = HuMemDirectMallocNum(HEAP_HEAP, sizeof(STARDISPWORK),
        HU_MEMNUM_OVL);
    obj->data = work;
    memset(work, 0, sizeof(STARDISPWORK));
    work->modelId[0] = mbObjCreate(
        mbBoardDataNumGet(SignMdlTbl[negativeF]), NULL, FALSE);
    work->modelId[1] = mbObjCreate(
        mbBoardDataNumGet(numberFileTbl[digit1]), NULL, FALSE);
    work->modelId[2] = mbObjCreate(
        mbBoardDataNumGet(numberFileTbl[digit10]), NULL, FALSE);
    work->modelId[3] = mbObjCreate(
        mbBoardDataNumGet(DATANUM(DATA_board, 5)), NULL, FALSE);
    modelId = mbObjModelIDGet(work->modelId[3]);
    modelP = &Hu3DData[modelId];
    hsf = modelP->hsf;
    material = hsf->material;
    modelP->hiliteIdx = 0;
    Hu3DModelMatHookSet(modelId, StarMatHook);
    for (i = 0; i < hsf->materialNum; i++, material++) {
        material->flags |= HSF_MATERIAL_MATHOOK;
    }
    for (i = 0; i < 3; i++) {
        mbObjPosSetV(work->modelId[i], &dispPos);
        mbObjScaleSet(work->modelId[i], 0.001f, 0.001f, 0.001f);
        mbObjMotionTimeSet(work->modelId[i], motTime);
        mbObjMotionSpeedSet(work->modelId[i], 0.0f);
        work->modelPos[i] = dispPos;
        mbObjLayerSet(work->modelId[i], 4);
    }
    mbObjPosSetV(work->modelId[3], &dispPos);
    work->modelPos[3] = dispPos;
    mbObjScaleSet(work->modelId[3], 0.001f, 0.001f, 0.001f);
    mbObjLayerSet(work->modelId[3], 4);
    if (digit10 == 0) {
        mbObjDispSet(work->modelId[2], FALSE);
    }
    if (num != 0) {
        if (num >= 0) {
            mbAudFXPlay(8);
        } else {
            mbAudFXPlay(50);
        }
    }
    work->playerNo = playerNo;
    work->num = num;
    work->mode = STAR_DISP_MODE_ON;
    work->unk1C = 0;
    work->time = 0;
    work->delay = 0;
    work->pos = dispPos;
    obj->trans.x = work->pos.x;
    obj->trans.y = work->pos.y;
    obj->trans.z = work->pos.z;
    obj->rot.x = 0.0f;
    obj->rot.y = 0.0f;
    obj->rot.z = 0.0f;
    obj->scale.x = 0.01f;
    obj->scale.y = 0.01f;
    obj->scale.z = 0.01f;
    return playerNo;
}

BOOL mbStarDispCheck(int playerNo)
{
    if (starDispObj[playerNo] != NULL) {
        return FALSE;
    }
    return TRUE;
}

static void StarDispUpdate(OMOBJ *obj)
{
    STARDISPWORK *work = obj->data;
    HuVecF modelPos;
    float time;
    float scale;
    float y;
    float spacing;
    float yOffset;
    float starX;
    float signX;
    float digit1X;
    float digit10X;
    float progress;
    float angleBase;
    float angle;
    float angleSin;
    u16 i;

    if (work->delay != 0) {
        work->delay--;
        return;
    }
    if (mbExitCheck()) {
        work->mode = STAR_DISP_MODE_KILL;
    }
    switch (work->mode) {
        case STAR_DISP_MODE_ON:
            OSu16tof32(&work->time, &time);
            scale = HuSin(time);
            obj->rot.y = 405.0f * scale;
            StarDispObjUpdate(work, 3, obj->trans.x, obj->trans.y,
                obj->trans.z, scale);
            mbObjRotYSet(work->modelId[3], obj->rot.y);
            if (work->time < 90) {
                work->time += 6;
            } else {
                work->mode++;
                work->time = 0;
                for (i = 0; i < 4; i++) {
                    StarDispObjUpdate(work, i, obj->trans.x, obj->trans.y,
                        obj->trans.z, scale);
                }
            }
            break;

        case STAR_DISP_MODE_MAIN:
            OSu16tof32(&work->time, &time);
            spacing = (work->num >= 10) ? 140.0f : 105.0f;
            yOffset = 200.0 * HuSin(2.0f * time);
            scale = HuSin(time);
            obj->rot.y = 45.0f + (315.0f * scale);
            if (work->num >= 10) {
                starX = obj->trans.x + (scale * -spacing);
                signX = obj->trans.x + ((scale * -spacing) / 3.0f);
                digit1X = obj->trans.x + (scale * spacing);
                digit10X = obj->trans.x + ((scale * spacing) / 3.0f);
            } else {
                signX = obj->trans.x;
                digit10X = obj->trans.x;
                digit1X = obj->trans.x + (scale * spacing);
                starX = obj->trans.x + (scale * -spacing);
            }
            StarDispObjUpdate(work, 3, starX, obj->trans.y + yOffset,
                obj->trans.z, 1.0f);
            StarDispObjUpdate(work, 0, signX, obj->trans.y + yOffset,
                obj->trans.z, 1.0f);
            StarDispObjUpdate(work, 1, digit1X, obj->trans.y + yOffset,
                obj->trans.z, 1.0f);
            StarDispObjUpdate(work, 2, digit10X, obj->trans.y + yOffset,
                obj->trans.z, 1.0f);
            mbObjRotYSet(work->modelId[3], obj->rot.y);
            mbObjRotYSet(work->modelId[0], obj->rot.y);
            mbObjRotYSet(work->modelId[1], obj->rot.y);
            mbObjRotYSet(work->modelId[2], obj->rot.y);
            if (work->time < 90) {
                work->time += 6;
            } else {
                obj->trans.y += yOffset;
                work->mode++;
                work->time = 0;
            }
            break;

        case STAR_DISP_MODE_NONE:
            OSu16tof32(&work->time, &time);
            scale = HuSin(time);
            if (work->num >= 10) {
                y = obj->trans.y + (-50.0f * scale);
            } else {
                y = obj->trans.y + (50.0f * scale);
            }
            for (i = 0; i < 4; i++) {
                modelPos = work->modelPos[i];
                StarDispObjUpdate(work, i, modelPos.x, y, modelPos.z,
                    1.0f);
            }
            if (work->time < 90) {
                work->time += 6;
            } else {
                work->mode++;
                work->time = 0;
                work->delay = 18;
                obj->scale.x = 1.0f;
                obj->scale.y = 1.0f;
                obj->trans.y = y;
            }
            break;

        case STAR_DISP_MODE_OFF:
            progress = (float)work->time / 24.0f;
            obj->rot.y = 90.0 + (270.0 * HuSin(90.0f * progress));
            angleBase = 180.0f * progress;
            for (i = 0; i < 4; i++) {
                angle = angleBase - (30.0f * (float)i);
                if (angle < 0.0f) {
                    angle = 0.0f;
                } else if (angle > 90.0f) {
                    angle = 90.0f;
                }
                if (i != 3) {
                    scale = 1.0f;
                } else {
                    scale = HuCos(angle);
                }
                angleSin = HuSin(angle);
                if (work->num >= 0) {
                    StarDispObjUpdate(work, i, work->modelPos[i].x,
                        obj->trans.y + (100.0f * angleSin),
                        work->modelPos[i].z, scale);
                } else {
                    StarDispObjUpdate(work, i, work->modelPos[i].x,
                        obj->trans.y + (-100.0f * angleSin),
                        work->modelPos[i].z, scale);
                }
                if (i != 3) {
                    mbObjAlphaSet(work->modelId[i], 255.0 * HuCos(angle));
                }
                mbObjRotYSet(work->modelId[i], obj->rot.y);
            }
            work->time++;
            if ((float)work->time > 24.0f) {
                for (i = 0; i < 4; i++) {
                    mbObjDispSet(work->modelId[i], FALSE);
                }
                work->mode++;
                work->time = 0;
            }
            break;

        case STAR_DISP_MODE_KILL:
            starDispObj[work->playerNo] = NULL;
            for (i = 0; i < 4; i++) {
                mbObjKill(work->modelId[i]);
            }
            HuMemDirectFree(obj->data);
            obj->data = NULL;
            omDelObjEx(mbObjMan, obj);
            break;
    }
}

static void StarDispObjUpdate(STARDISPWORK *work, int modelNo,
    float x, float y, float z, float scale)
{
    HuVecF pos;
    HuVecF pos2D;
    HuVecF cameraPos;
    Mtx cameraMtx;
    Mtx modelMtx;
    float distance;
    float scaleFix;

    work->modelPos[modelNo].x = x;
    work->modelPos[modelNo].y = y;
    work->modelPos[modelNo].z = z;
    pos = work->modelPos[modelNo];
    Hu3D3Dto2D(&pos, HU3D_CAM0, &pos2D);
    Hu3DCameraSet(0, cameraMtx);
    PSMTXIdentity(modelMtx);
    modelMtx[0][3] = pos.x;
    modelMtx[1][3] = pos.y;
    modelMtx[2][3] = pos.z;
    PSMTXConcat(cameraMtx, modelMtx, modelMtx);
    cameraPos.x = modelMtx[0][3];
    cameraPos.y = modelMtx[1][3];
    cameraPos.z = modelMtx[2][3];
    distance = PSVECMag(&cameraPos);
    scaleFix = 800.0f / distance;
    pos2D.z = 800.0f;
    Hu3D2Dto3D(&pos2D, HU3D_CAM0, &pos);
    mbObjScaleSet(work->modelId[modelNo], scale * scaleFix,
        scale * scaleFix, scale * scaleFix);
    mbObjPosSet(work->modelId[modelNo], pos.x, pos.y, pos.z);
}

static void ev_StarMasuRun(BOOL freeF)
{
    int guideNo = mbGuideNoGet();
    int speakerNo;
    int messNo;
    int playerNo;
    int winNo;
    s16 masuId;
    HuVecF startPos;
    HuVecF endPos;
    HuVecF guidePos;
    OMOBJ *obj;
    STARWORK *work;
    int seNo;

    starGuideObj = NULL;
    if (GwSystem.curTime == 0) {
        speakerNo = HUWIN_SPEAKER_SUN;
        messNo = 0;
    } else {
        speakerNo = HUWIN_SPEAKER_MOON;
        messNo = 1;
    }
    playerNo = GwSystem.turnPlayerNo;
    mbAudFXPlay(1098);
    mbPlayerMotIdleSet(playerNo);
    mbPlayerRotateStart(playerNo, 0, 15);
    while (!mbPlayerRotateCheck(playerNo)) {
        HuPrcVSleep();
    }
    mbCameraMoveWait();
    if (mbPlayerStarGet(playerNo) >= 999) {
        mbAudGuidePlay(951);
        mbWinCreate(MBWIN_TYPE_EVENT,
            0x27000A + messNo, speakerNo);
        mbWinTopWait();
        goto end;
    }
    if (!freeF && mbPlayerCoinGet(playerNo) < 20) {
        mbAudGuidePlay(951);
        mbWinCreate(MBWIN_TYPE_EVENT,
            0x270004 + messNo, speakerNo);
        mbWinTopWait();
        goto end;
    }
    mbStatusDispSetAll(FALSE);
    while (!mbStatusOffCheckAll()) {
        HuPrcVSleep();
    }
    mbAudGuidePlay(950);
    mbWinCreate(MBWIN_TYPE_EVENT, 0x270000 + messNo, speakerNo);
    mbWinTopInsertMesSet(mbPlayerNameMesGet(playerNo), 0);
    mbWinTopWait();
    mbStatusDispFocusSet(playerNo, TRUE);
    while (!mbStatusMoveCheck(playerNo)) {
        HuPrcVSleep();
    }
    if (!freeF) {
        mbAudGuidePlay(952);
        mbWinCreateChoice(1, 0x270002 + messNo, speakerNo, 0);
        if (GwPlayer[playerNo].comF) {
            if (mbPlayerCoinGet(playerNo) >= 20) {
                mbComChoiceUpSet();
            } else {
                mbComChoiceDownSet();
            }
        }
        mbWinTopWait();
        if (mbWinTopChoiceGet() == -1) {
            mbAudGuidePlay(951);
            mbWinCreate(MBWIN_TYPE_EVENT,
                0x270008 + messNo, speakerNo);
            mbWinTopWait();
            mbStatusDispFocusSet(playerNo, FALSE);
            while (!mbStatusMoveCheck(playerNo)) {
                HuPrcVSleep();
            }
            mbStatusDispSetAll(TRUE);
            goto end;
        }
        switch (mbWinTopChoiceGet()) {
            case 0:
                if (mbPlayerCoinGet(playerNo) < 20) {
                    mbAudGuidePlay(951);
                    mbWinCreate(MBWIN_TYPE_EVENT,
                        0x270004 + messNo, speakerNo);
                    mbWinTopWait();
                    mbStatusDispFocusSet(playerNo, FALSE);
                    while (!mbStatusMoveCheck(playerNo)) {
                        HuPrcVSleep();
                    }
                    mbStatusDispSetAll(TRUE);
                    goto end;
                }
                goto starBuy;

            case 1:
                mbAudGuidePlay(951);
                mbWinCreate(MBWIN_TYPE_EVENT,
                    0x270008 + messNo, speakerNo);
                mbWinTopWait();
                mbStatusDispFocusSet(playerNo, FALSE);
                while (!mbStatusMoveCheck(playerNo)) {
                    HuPrcVSleep();
                }
                mbStatusDispSetAll(TRUE);
                goto end;

            default:
starBuy:
                mbAudGuidePlay(952);
                mbWinCreate(MBWIN_TYPE_EVENT,
                    0x270006 + messNo, speakerNo);
                mbWinTopWait();
                mbCoinAddExec(playerNo, -20);
                break;
        }
    } else {
        mbWinCreate(MBWIN_TYPE_EVENT,
            0x270006 + messNo, speakerNo);
        mbWinTopWait();
    }
    masuId = GwPlayer[playerNo].masuId;
    mbMasuPosGet(masuId, &startPos);
    _SetFlag(FLAGNUM(FLAG_GROUP_COMMON, 28));
    mbStarGetMain(playerNo, NULL, 1, TRUE);
    mbWipeFadeOut();
    mbPlayerMotionShiftSet(playerNo, 1, 0.0f, 1.0f, 0x40000001);
    mbCameraPlayerViewSetFast(playerNo, 2);
    if (starMoveHook != NULL) {
        starMoveHook();
    }
    obj = StarMasuObjGet(starMasuNext);
    work = obj->data;
    work->time = 0;
    mbMasuPosGet(starMasuNext, &endPos);
    mbObjDispSet(obj->mdlId[0], FALSE);
    work->modelDispF = FALSE;
    mbObjDispSet(work->signModelId, FALSE);
    mbPlayerMotionShiftSet(playerNo, 1, 0.0f, 1.0f, 0x40000001);
    mbPauseHookPush(StarPauseHook);
    mbNormPosto3D(&starMasuGuidePos, 4, &guidePos);
    starGuideObj = mbGuideCreateFlag(&guidePos, starMasuGuideMotTbl,
        TRUE, FALSE, TRUE);
    mbGuideMotionNextSet(starGuideObj, 1);
    mbStatusDispForceSetAll(FALSE);
    mbWipeFadeIn();
    mbMusPlay(1, 12, 127, 0);
    mbAudGuidePlay(952);
    mbGuideMotionShiftSet(starGuideObj, 12, TRUE);
    winNo = mbWinCreateTime(4,
        0x27000C + messNo, HUWIN_SPEAKER_NULL);
    mbWinPause((s16)winNo);
    mbev_StarScroll(&startPos, &endPos, 120);
    mbWinKill((s16)winNo);
    mbObjDispSet(obj->mdlId[0], TRUE);
    seNo = mbAudFXPlay(1095);
    mbAudFXPlay(1096);
    StarObjGrowSet(obj);
    work = obj->data;
    while (work->mode != STAR_MODE_IDLE) {
        HuPrcVSleep();
    }
    HuPrcSleep(20);
    mbAudFXStop(seNo);
    mbAudGuidePlay(952);
    mbGuideMotionShiftSet(starGuideObj, 12, TRUE);
    mbWinCreateTime(4,
        0x27000E + messNo, HUWIN_SPEAKER_NULL);
    mbWinTopWait();
    mbMusFadeOutSpeed(1, 1000);
    mbWipeFadeOut();
    mbCameraPlayerViewSetFast(playerNo, 2);
    mbStatusDispForceSet(playerNo, FALSE);
    mbStatusDispForceSetAll(TRUE);
    mbStatusDispForceSetAll(TRUE);
    mbGuideKill(starGuideObj);
    starGuideObj = NULL;
    HuPrcSleep(60);
    mbMusBoardPlay();
    mbPauseHookPop(StarPauseHook);
    mbWipeFadeIn();

end:
    _ClearFlag(FLAGNUM(FLAG_GROUP_COMMON, 28));
    mbCameraFocusPlayerSet(GwSystem.turnPlayerNo);
    mbCameraMoveWait();
}

static void StarPauseHook(BOOL pauseF)
{
    int modelId;

    if (starGuideObj != NULL) {
        modelId = mbGuideModelGet(starGuideObj);
        if (pauseF != FALSE) {
            mbObjDispSet(modelId, TRUE);
        } else {
            mbObjDispSet(modelId, FALSE);
        }
    }
}

static void StarMatHook(HU3D_DRAW_OBJ *drawObj, HSF_MATERIAL *material)
{
    GXColor colorNew = { 255, 255, 255, 255 };
    HuVecF lightDir = lbl_8021AB24;
    int tevStageNum;
    int texGenNum;
    GXColor color;
    HuVecF pos;
    HuVecF dir;

    Hu3DGLightParamGet(0, &pos, &dir, &color);
    Hu3DGlobalLight[0].color = colorNew;
    Hu3DGlobalLight[0].dir = lightDir;
    mbObjStarTevStageSet(
        drawObj, material, &tevStageNum, &texGenNum);
    Hu3DGLightColorSet(0, color.r, color.g, color.b, color.a);
    Hu3DGLightPosSetV(0, &pos, &dir);
}

void mbZtarObjInit(void)
{
    memset(ztarOMObj, 0, sizeof(ztarOMObj));
}

void mbZtarObjClose(void)
{
    int i;

    for (i = 0; i < STAR_OBJ_MAX; i++) {
        if (ztarOMObj[i]) {
            ZtarObjKill(ztarOMObj[i]);
            ztarOMObj[i] = NULL;
        }
    }
}

static int ZtarObjCreate(HuVecF *pos)
{
    int i;

    for (i = 0; i < STAR_OBJ_MAX; i++) {
        if (ztarOMObj[i] == NULL) {
            OMOBJ *obj;
            STARWORK *work;
            HU3D_MODELID modelId;
            HU3D_MODEL *modelP;
            HSF_DATA *hsf;
            HSF_MATERIAL *material;
            MBPARTICLE *particleP;
            int j;

            obj = omAddObjEx(mbObjMan, 257, 1, 0, OM_GRP_NONE,
                ZtarObjOMExec);
            ztarOMObj[i] = obj;
            work = HuMemDirectMallocNum(HEAP_HEAP, sizeof(STARWORK),
                HU_MEMNUM_OVL);
            obj->data = work;
            memset(work, 0, sizeof(STARWORK));
            work->obj = obj;
            obj->mdlId[0] = mbObjCreate(
                DATANUM(DATA_capsulechar1, 0x20), NULL, TRUE);
            modelId = mbObjModelIDGet(obj->mdlId[0]);
            modelP = &Hu3DData[modelId];
            hsf = modelP->hsf;
            material = hsf->material;
            modelP->hiliteIdx = 0;
            Hu3DModelMatHookSet(modelId, StarMatHook);
            for (j = 0; j < hsf->materialNum; j++, material++) {
                material->flags |= HSF_MATERIAL_MATHOOK;
            }
            mbObjRotSet(obj->mdlId[0], -90.0f, 0.0f, 0.0f);
            mbObjZWriteOffSet(obj->mdlId[0], FALSE);
            mbObjLayerSet(obj->mdlId[0], 3);
            work->objNo = i;
            work->mode = STAR_MODE_IDLE;
            work->effectModelId = StarObjEffCreate(starEffAnim1);
            particleP = Hu3DData[work->effectModelId].hookData;
            particleP->hookData = work;
            work->modelDispF = TRUE;
            work->time = 0;
            work->effectDispF = TRUE;
            work->offset.x = work->offset.y = work->offset.z = 0.0f;
            work->rot.x = work->rot.y = work->rot.z = 0.0f;
            work->scale.x = work->scale.y = work->scale.z = 1.0f;
            omSetStatBit(obj, OM_STAT_MODELPAUSE);
            return i;
        }
    }
    return -1;
}

void mbZtarObjDispFlagSet(int objNo, BOOL dispF)
{
    OMOBJ *obj = ztarOMObj[objNo];
    STARWORK *work = obj->data;

    work->modelDispF = dispF;
}

void mbZtarMasuNextSet(s16 masuId)
{
    HuVecF pos;
    int objNo;
    OMOBJ *obj;
    STARWORK *work;

    starMasuPrevType = mbMasuTypeGet(masuId);
    mbMasuTypeSet(masuId, 10);
    mbMasuPosGet(masuId, &pos);
    pos.y += 300.0f;
    objNo = ZtarObjCreate(&pos);
    obj = ztarOMObj[objNo];
    work = obj->data;
    work->masuId = masuId;
    work->playerNo = -1;
    work->signF = TRUE;
    work->baseY = pos.y - 300.0f;
    work->pos = pos;
    mbMasuCapsuleSet(masuId, MASU_NULL);
    ztarMasuNext = masuId;
}

void mbZtarGetExec(int playerNo)
{
    HuVecF pos;
    int objNo;
    OMOBJ *obj;
    STARWORK *work;

    mbPlayerPosGet(playerNo, &pos);
    pos.y += 300.0f;
    objNo = ZtarObjCreate(&pos);
    obj = ztarOMObj[objNo];
    work = obj->data;
    work->playerNo = playerNo;
    work->masuId = -1;
    work->signF = FALSE;
    work->baseY = pos.y - 300.0f;
    work->pos = pos;
    ZtarObjGrowSet(obj);
    ZtarObjGrowWait(obj);
    mbZtarGetMain(playerNo, NULL, -1, TRUE);
}

void mbZtarGetMain(int playerNo, HuVecF *pos, int num, BOOL focusF)
{
    int time;
    OMOBJ *obj;
    STARWORK *work;
    int objNo;
    int masuId;
    int seNo;
    int no;
    int i;
    HuVecF playerPos;

    masuId = GwPlayer[playerNo].masuId;
    seNo = -1;
    _SetFlag(FLAG_BOARD_TURN_NOSTART);
    mbMusFadeOutSpeed(0, 1000);
    if (pos == NULL) {
        for (i = 0; i < STAR_OBJ_MAX; i++) {
            if (ztarOMObj[i]) {
                STARWORK *objWork = ztarOMObj[i]->data;

                if (objWork->masuId == masuId) {
                    objNo = i;
                    break;
                }
            }
        }
        if (i >= STAR_OBJ_MAX) {
            objNo = -1;
        }
        if (objNo >= 0) {
            obj = ztarOMObj[objNo];
        } else {
            obj = NULL;
        }
        if (obj == NULL) {
            for (i = 0; i < STAR_OBJ_MAX; i++) {
                if (ztarOMObj[i]) {
                    STARWORK *objWork = ztarOMObj[i]->data;

                    if (objWork->playerNo == playerNo) {
                        objNo = i;
                        break;
                    }
                }
            }
            if (i >= STAR_OBJ_MAX) {
                objNo = -1;
            }
            if (objNo >= 0) {
                obj = ztarOMObj[objNo];
            } else {
                obj = NULL;
            }
        }
        work = obj->data;
        no = work->no;
    } else {
        objNo = ZtarObjCreate(pos);
        obj = ztarOMObj[objNo];
        work = obj->data;
        work->playerNo = playerNo;
        work->masuId = -1;
        work->signF = FALSE;
        work->baseY = pos->y - 300.0f;
        work->pos = *pos;
        obj = ztarOMObj[objNo];
        work = obj->data;
        mbPlayerPosGet(playerNo, &playerPos);
        work->baseY = playerPos.y;
        no = -1;
        HuPrcVSleep();
    }
    ZtarObjShrinkIdleSet(obj);
    seNo = mbAudFXPlay(1122);
    ZtarObjShrinkIdleWait(obj);
    time = 0;
    do {
        if (time++ == 27) {
            ZtarObjShrinkSet(obj);
        }
        if (time == 30) {
            ((STARWORK *)ztarOMObj[work->objNo]->data)->effectDispF = FALSE;
        }
        HuPrcVSleep();
    } while (time < 60);
    mbAudFXStop(seNo);
    mbPlayerMotIdleSet(playerNo);
    omVibrate((s16)playerNo, 20, 7, 3);
    mbPlayerMotionShiftSet(playerNo, 8, 0.0f, 8.0f, 0);
    mbPlayerWinLoseVoicePlay(playerNo, 13, 585);
    if (mbPlayerStarGet(playerNo) >= 1) {
        int delay;
        int starNum;
        int addNum;
        int result;

        if (abs(num) >= 50) {
            delay = 1;
        } else if (abs(num) >= 20) {
            delay = 3;
        } else {
            delay = 6;
        }
        starNum = mbPlayerStarGet(playerNo) + num;
        addNum = num;
        if (starNum > 999) {
            addNum = 999 - mbPlayerStarGet(playerNo);
        } else if (starNum < 0) {
            addNum = -mbPlayerStarGet(playerNo);
        }
        mbPlayerStarAdd(playerNo, addNum);
        result = addNum;
        if (result != 0) {
            mbAudFXPlay(15);
        }
        if (result != 0) {
            HuVecF dispPos;

            mbPlayerPosGet(playerNo, &dispPos);
            dispPos.y += 250.0f;
            mbStarDispCreate(playerNo, &dispPos, result);
            while (!mbStarDispCheck(playerNo)) {
                HuPrcVSleep();
            }
        }
        GwSystem.starTotal = GwSystem.starTotal + num;
    } else {
        int coin = mbPlayerCoinGet(playerNo);

        if (coin >= 20) {
            mbCoinAddDispExec(playerNo, -20, TRUE, TRUE);
        } else if (coin > 0) {
            mbCoinAddDispExec(playerNo, -coin, TRUE, TRUE);
        }
    }
    mbPlayerMotionEndWait(playerNo);
    _ClearFlag(FLAG_BOARD_TURN_NOSTART);
    if (work->signF == FALSE) {
        for (i = 0; i < STAR_OBJ_MAX; i++) {
            if (ztarOMObj[i]) {
                STARWORK *objWork = ztarOMObj[i]->data;

                if (playerNo == objWork->playerNo) {
                    ZtarObjKill(ztarOMObj[i]);
                    break;
                }
            }
        }
    } else if (work->signF == TRUE) {
        masuId = GwPlayer[playerNo].masuId;
        for (i = 0; i < STAR_OBJ_MAX; i++) {
            if (ztarOMObj[i]) {
                STARWORK *objWork = ztarOMObj[i]->data;

                if (masuId == objWork->masuId) {
                    ZtarObjKill(ztarOMObj[i]);
                    break;
                }
            }
        }
    }
    HuPrcSleep(18);
}

static void ZtarObjOMExec(OMOBJ *obj)
{
    STARWORK *work = obj->data;
    float rotY;

    if (work->killF || mbExitCheck()) {
        StarObjEffKill(work->effectModelId);
        work->effectModelId = HU3D_MODELID_NONE;
        work->signF = -1;
        mbObjKill(obj->mdlId[0]);
        obj->mdlId[0] = MB_MODEL_NONE;
        omDelObjEx(HuPrcCurrentGet(), obj);
        ztarOMObj[work->objNo] = NULL;
        return;
    }

    switch (work->mode) {
        case STAR_MODE_GROW:
        {
            float weight = work->time++ / 90.0f;

            work->scale.x = mbSinDeg(90.0f * weight);
            work->scale.y = work->scale.z = work->scale.x;
            if (work->time > 90) {
                work->rot.y = 0.0f;
                work->rotateF = FALSE;
                work->mode = STAR_MODE_IDLE;
                work->time = 0;
                return;
            }
            break;
        }

        case STAR_MODE_IDLE:
            if (work->signF == TRUE) {
                HuVecF pos;

                mbMasuPosGet(work->masuId, &pos);
                work->pos.x = pos.x;
                work->pos.z = pos.z;
            }
            work->offset.y = 100.0f
                * (0.2f * HuSin(4.0f * work->time));
            work->time++;
            break;

        case STAR_MODE_SHRINK_IDLE:
            work->mode = STAR_MODE_SHRINK_IDLE_DONE;
            break;

        case STAR_MODE_SHRINK:
            work->pos.y += -4.0f;
            if (work->pos.y > work->baseY + 80.0f) {
                if (work->rotY < 90) {
                    work->rotY += 2;
                    if (work->rotY > 90) {
                        work->rotY = 90;
                    }
                }
                rotY = work->rotY;
                work->scale.x = HuSin(rotY + 90.0f);
                if (work->scale.x <= 0.0f) {
                    work->scale.x = 0.001f;
                }
                work->scale.y = work->scale.z = work->scale.x;
            } else {
                work->killF = TRUE;
                mbObjDispSet(obj->mdlId[0], FALSE);
                return;
            }
            break;

        case STAR_MODE_SHRINK_IDLE_DONE:
            break;
    }

    ZtarObjRotate(work, obj);
    mbObjPosSet(obj->mdlId[0], work->pos.x + work->offset.x,
        work->pos.y + work->offset.y, work->pos.z + work->offset.z);
    mbObjRotSetV(obj->mdlId[0], &work->rot);
    mbObjScaleSetV(obj->mdlId[0], &work->scale);
}

static void ZtarObjKill(OMOBJ *obj)
{
    STARWORK *work = obj->data;

    work->killF = TRUE;
}

static void ZtarObjRotate(STARWORK *work, OMOBJ *obj)
{
    float rotSpeed;

    if (work->rotateF == FALSE) {
        return;
    }
    if (work->rotSpeed < 16.0f) {
        work->rotSpeed += 2.0f;
        if (work->rotSpeed > 16.0f) {
            work->rotSpeed = 16;
        }
    }
    OSs8tof32(&work->rotSpeed, &rotSpeed);
    if (work->mode == STAR_MODE_GROW) {
        work->rot.y = 720.0f * mbSinDeg(work->time);
    } else {
        work->rot.y = mbAngleWrap(work->rot.y + rotSpeed);
    }
}

static void ZtarObjShrinkIdleSet(OMOBJ *obj)
{
    STARWORK *work = obj->data;

    work->mode = STAR_MODE_SHRINK_IDLE;
    if (work->effectDispF == FALSE) {
        mbObjDispSet(obj->mdlId[0], TRUE);
        work->pos.y = work->baseY + 800.0f;
    }
}

static void ZtarObjGrowSet(OMOBJ *obj)
{
    STARWORK *work = obj->data;

    work->rotateF = TRUE;
    work->rotY = 0;
    work->time = 0;
    work->mode = STAR_MODE_GROW;
    work->modelDispF = TRUE;
    work->offset.y = 0.0f;
    mbAudFXPlay(1118);
}

static void ZtarObjShrinkSet(OMOBJ *obj)
{
    STARWORK *work = obj->data;

    work->rotateF = TRUE;
    work->rotY = 0;
    work->rotSpeed = 0;
    work->mode = STAR_MODE_SHRINK;
}

static void ZtarObjGrowWait(OMOBJ *obj)
{
    STARWORK *work = obj->data;

    while (work->mode != STAR_MODE_IDLE) {
        HuPrcVSleep();
    }
    HuPrcSleep(20);
}

static void ZtarObjShrinkIdleWait(OMOBJ *obj)
{
    STARWORK *work = obj->data;

    while (work->mode != STAR_MODE_SHRINK_IDLE_DONE) {
        HuPrcVSleep();
    }
}
