#include "game/board/main.h"

#include "datanum/effect.h"
#include "game/board/audio.h"
#include "game/board/effect.h"
#include "game/board/masu.h"
#include "game/board/object.h"
#include "game/board/player.h"
#include "game/flag.h"
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

static OMOBJ *starOMObj[STAR_OBJ_MAX];
static OMOBJ *starDispObj[4];
static OMOBJ *ztarOMObj[STAR_OBJ_MAX];

static int starAddNum = 1;
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

static void StarObjKill(OMOBJ *obj);
static int StarObjCreate(HuVecF *pos);
static void StarObjOMExec(OMOBJ *obj);
static void StarObjRotate(STARWORK *work, OMOBJ *obj);
static int StarMasuNoGet(int masuId);
static void StarAddAllProc(int *addNum, BOOL dispF, int *result);
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
void mbStarDispCreate(int playerNo, HuVecF *pos, int num);
void mbZtarGetMain(int playerNo, HuVecF *pos, int num, BOOL focusF);
int mbGuideModelGet(OMOBJ *obj);
float mbSinDeg(float angle);
float mbAngleWrap(float angle);

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
    work->playerNo = work->masuId = -1;
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

static void StarObjGrowSet(OMOBJ *obj)
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

static int StarMasuNoGet(int masuId)
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

BOOL mbStarDispCheck(int playerNo)
{
    if (starDispObj[playerNo] != NULL) {
        return FALSE;
    }
    return TRUE;
}

static void StarPauseHook(BOOL pauseF)
{
    MBMODELID modelId;

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
    HuVecF pos;
    HuVecF dir;
    GXColor color;
    GXColor colorNew = { 255, 255, 255, 255 };
    int tevStageNum;
    int texGenNum;
    HuVecF lightDir = lbl_8021AB24;

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
