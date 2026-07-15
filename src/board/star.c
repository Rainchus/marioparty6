#include "game/board/main.h"

#include "datanum/effect.h"
#include "game/board/audio.h"
#include "game/board/effect.h"
#include "game/board/masu.h"
#include "game/board/object.h"
#include "game/board/player.h"
#include "game/memory.h"
#include "game/sprite.h"

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
static void ZtarObjKill(OMOBJ *obj);
static int ZtarObjCreate(HuVecF *pos);
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

void mbStarMoveHookSet(void (*hook)(void))
{
    starMoveHook = hook;
}

void mbStarMasuFuncSet(void (*func)(void))
{
    starMasuFunc = func;
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

static void StarObjKill(OMOBJ *obj)
{
    STARWORK *work = obj->data;

    work->killF = TRUE;
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

static void ZtarObjKill(OMOBJ *obj)
{
    STARWORK *work = obj->data;

    work->killF = TRUE;
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
