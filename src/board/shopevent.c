#include "game/board/main.h"
#include "game/board/audio.h"
#include "game/board/masu.h"
#include "game/board/object.h"
#include "game/board/pause.h"
#include "game/board/player.h"
#include "game/memory.h"

#include "humath.h"
#include "string.h"

typedef void (*MBSHOPOBJHOOK)(int modelId, int shopNo);

typedef struct MBSHOPWORK {
    int playerNo;
    int shopNo;
} MBSHOPWORK;

typedef struct MBSHOPOMWORK {
    int modelId;
    int shopNo;
    int masuId;
    int masuLinkId;
    BOOL pathF;
    int masuEndId;
    int unk18;
    int unk1C;
    BOOL modelMotionF;
    BOOL motionExecF;
    BOOL openF;
    int backModelId[8];
    int backMotNo[8];
    BOOL backDispF[8];
    HuVecF masuPos;
    HuVecF capsulePos;
    HuVecF shopPos;
} MBSHOPOMWORK;

static HuVecF ev_ShopCapsulePlayer[3][3] = {
    {
        { 0.0f, 0.0f, 0.0f },
        { 0.0f, 0.0f, 0.0f },
        { 0.0f, 0.0f, 0.0f },
    },
    {
        { 90.0f, 50.0f, 0.0f },
        { -90.0f, 50.0f, 0.0f },
        { 0.0f, 0.0f, 0.0f },
    },
    {
        { 90.0f, 50.0f, 0.0f },
        { 0.0f, 0.0f, 0.0f },
        { -90.0f, 50.0f, 0.0f },
    },
};

static HuVecF ev_ShopLightPos = { -10000.0f, 10000.0f, -10000.0f };
static HuVecF ev_ShopLightDir = { 1.0f, -1.0f, -1.0f };
static int ev_ShopSprFileTbl[4] = {
    DATANUM(DATA_board, 0x26),
    DATANUM(DATA_board, 0x23),
    DATANUM(DATA_board, 0x23),
    DATANUM(DATA_board, 0x23),
};
static HuVecF ev_ShopWinPos = { 288.0f, 176.0f, 0.0f };
static HuVecF ev_ShopCapsulePos[3][3] = {
    {
        { 288.0f, 170.0f, 1000.0f },
        { 0.0f, 0.0f, 0.0f },
        { 0.0f, 0.0f, 0.0f },
    },
    {
        { 214.0f, 170.0f, 1000.0f },
        { 362.0f, 170.0f, 1000.0f },
        { 0.0f, 0.0f, 0.0f },
    },
    {
        { 192.0f, 170.0f, 1000.0f },
        { 288.0f, 170.0f, 1000.0f },
        { 384.0f, 170.0f, 1000.0f },
    },
};

static OMOBJ *ev_ShopOMObj[GW_PLAYER_MAX];
static GXColor ev_ShopLightColor = { 255, 255, 255, 255 };

void mbev_ShopCreate(int dataNum, int motDataNum);
void mbev_ShopBackMotCreate(int dataNum, int motDataNum, int motNo, BOOL linkF, char *hookName);
static void ev_ShopOMExec(OMOBJ *obj);
static void ev_ShopOpenSet(int shopNo, BOOL openF);
void ev_Shop(MBSHOPWORK *work);
static int ev_ShopMesGet(int messNo);
void mbev_ShopExObjHookSet(MBSHOPOBJHOOK hook);

static BOOL ev_ShopEnableF;
static MBSHOPOBJHOOK ev_ShopExObjHook;
static int ev_ShopNum;

void mbev_ShopEnableSet(BOOL enableF)
{
    ev_ShopEnableF = enableF;
}

void mbev_ShopInit(int dataNum)
{
    mbev_ShopExObjHookSet(NULL);
    mbev_ShopCreate(dataNum, -1);
}

void mbev_ShopExInit(int dataNum, MBSHOPOBJHOOK hook)
{
    mbev_ShopExObjHookSet(hook);
    mbev_ShopCreate(dataNum, -1);
}

void mbev_ShopCreate(int dataNum, int motDataNum)
{
    HuVecF shopPos;
    HuVecF masuPos;
    HuVecF dir;
    int masuTbl[3];
    int motDataNumTbl[2];
    int masuId;
    int i;
    int shopNo;

    ev_ShopEnableF = TRUE;
    for (masuId = 0; masuId < 3; masuId++) {
        ev_ShopOMObj[masuId] = NULL;
    }
    shopNo = 0;
    motDataNumTbl[0] = motDataNum;
    motDataNumTbl[1] = -1;
    for (masuId = 1; masuId < mbMasuNumGet(); masuId++) {
        int linkMasuId;
        int shopLinkMasuId;
        int shopMasuId;
        int nextMasuId;
        MBSHOPOMWORK *work;
        OMOBJ *obj;

        if (mbMasuTypeGet(masuId) != 9) {
            continue;
        }
        linkMasuId = mbMasuAttrFindLink(masuId, 0x20);
        if (linkMasuId < 0) {
            break;
        }
        nextMasuId = mbMasuAttrFindLink(linkMasuId, 0x20);
        work = HuMemDirectMallocNum(HEAP_HEAP, sizeof(MBSHOPOMWORK), HU_MEMNUM_OVL);
        memset(work, 0, sizeof(MBSHOPOMWORK));
        shopMasuId = masuId;
        shopLinkMasuId = linkMasuId;
        mbMasuPosGet(shopMasuId, &shopPos);
        mbMasuPosGet(shopLinkMasuId, &masuPos);
        if (nextMasuId != -1) {
            work->pathF = TRUE;
            masuTbl[0] = shopMasuId;
            masuTbl[1] = shopLinkMasuId;
            masuTbl[2] = nextMasuId;
            while ((nextMasuId = mbMasuAttrFindLink(masuTbl[2], 0x20)) > 0) {
                masuTbl[0] = masuTbl[1];
                masuTbl[1] = masuTbl[2];
                masuTbl[2] = nextMasuId;
            }
            mbMasuPosGet(masuTbl[1], &work->shopPos);
            mbMasuPosGet(masuTbl[2], &work->masuPos);
            shopPos = work->shopPos;
            masuPos = work->masuPos;
            work->masuEndId = masuTbl[1];
        } else {
            work->pathF = FALSE;
            work->masuEndId = -1;
            PSVECSubtract(&shopPos, &masuPos, &dir);
            if (PSVECMag(&dir) > 0.0f) {
                PSVECNormalize(&dir, &dir);
            }
            PSVECScale(&dir, &dir, 100.0f);
            PSVECAdd(&masuPos, &dir, &work->shopPos);
            work->shopPos.y = masuPos.y;
            mbMasuPosGet(shopLinkMasuId, &work->masuPos);
        }
        PSVECSubtract(&masuPos, &shopPos, &dir);
        if (PSVECMag(&dir) > 0.0f) {
            PSVECNormalize(&dir, &dir);
        }
        PSVECScale(&dir, &dir, 100.0f);
        PSVECAdd(&masuPos, &dir, &work->capsulePos);
        if (GwSystem.curTime) {
            work->capsulePos.y += 30.000002f;
        }
        obj = ev_ShopOMObj[shopNo] = omAddObjEx(mbObjMan, -32768, 0, 0,
            OM_GRP_NONE, ev_ShopOMExec);
        obj->data = work;
        if (dataNum <= 0) {
            work->modelId = -1;
        } else if (motDataNum != -1) {
            work->modelId = mbObjCreate(dataNum, motDataNumTbl, TRUE);
            work->modelMotionF = TRUE;
        } else {
            work->modelId = mbObjCreate(dataNum, NULL, TRUE);
            work->modelMotionF = FALSE;
        }
        PSVECSubtract(&shopPos, &masuPos, &dir);
        if (work->modelId != -1) {
            mbObjPosSetV(work->modelId, &masuPos);
            mbObjRotSet(work->modelId, 0.0f, HuAtan(dir.x, dir.z), 0.0f);
            mbObjMotionSpeedSet(work->modelId, 0.0f);
        }
        work->shopNo = shopNo;
        work->masuId = shopMasuId;
        work->masuLinkId = shopLinkMasuId;
        work->unk18 = 0;
        work->unk1C = 0;
        work->motionExecF = FALSE;
        work->openF = FALSE;
        if (ev_ShopExObjHook != NULL) {
            ev_ShopExObjHook(work->modelId, shopNo);
        }
        for (i = 0; i < 8; i++) {
            work->backModelId[i] = -1;
            work->backMotNo[i] = 0;
            work->backDispF[i] = FALSE;
        }
        shopNo++;
    }
    ev_ShopNum = shopNo;
}

void mbev_ShopExObjHookSet(MBSHOPOBJHOOK hook)
{
    ev_ShopExObjHook = hook;
}

void mbev_ShopBackCreate(int dataNum, int motDataNum, int motNo, BOOL linkF)
{
    mbev_ShopBackMotCreate(dataNum, motDataNum, motNo, linkF, 0);
}

void mbev_ShopBackMotCreate(int dataNum, int motDataNum, int motNo, BOOL linkF, char *hookName)
{
    int motDataNumTbl[2];
    HuVecF pos;
    HuVecF rot;
    int i;
    int j;

    for (i = 0; i < 3; i++) {
        OMOBJ *obj = ev_ShopOMObj[i];
        MBSHOPOMWORK *work;

        if (obj == NULL) {
            continue;
        }
        work = obj->data;
        for (j = 0; j < 8; j++) {
            if (work->backModelId[j] == -1) {
                break;
            }
        }
        if (motDataNum >= 0) {
            motDataNumTbl[0] = motDataNum;
            motDataNumTbl[1] = -1;
            work->backModelId[j] = mbObjCreate(dataNum, motDataNumTbl, linkF);
        } else {
            work->backModelId[j] = mbObjCreate(dataNum, NULL, linkF);
        }
        if (work->modelId != -1) {
            mbObjPosGet(work->modelId, &pos);
            mbObjRotGet(work->modelId, &rot);
        } else {
            pos.x = pos.y = pos.z = 0.0f;
            rot.x = rot.y = rot.z = 0.0f;
        }
        if (hookName == NULL) {
            mbObjPosSetV(work->backModelId[j], &pos);
            mbObjRotSetV(work->backModelId[j], &rot);
        } else if (work->modelId != -1) {
            mbObjHookSet(work->modelId, hookName, work->backModelId[j]);
        }
        work->backMotNo[j] = motNo;
        switch (work->backMotNo[j]) {
            case 0:
            case 1:
                mbObjMotionSpeedSet(work->backModelId[j], 0.0f);
                break;
            case 2:
            case 3:
                mbObjMotionSpeedSet(work->backModelId[j], 0.0f);
                break;
            case 4:
                mbObjAttrSet(work->backModelId[j], HU3D_MOTATTR_LOOP);
                break;
        }
        if (work->backMotNo[j] == 1 || work->backMotNo[j] == 3) {
            work->backDispF[j] = TRUE;
        }
        if (!work->backDispF[j]) {
            mbObjDispSet(work->backModelId[j], FALSE);
        }
    }
}

static void ev_ShopOMExec(OMOBJ *obj)
{
    MBSHOPOMWORK *work = obj->data;
    int i;

    if (mbExitCheck() || ev_ShopOMObj[work->shopNo] == NULL) {
        omDelObjEx(mbObjMan, obj);
        return;
    }
    if (work->motionExecF && work->modelId != -1) {
        if (work->openF) {
            if (mbObjMotionTimeGet(work->modelId) >= mbObjMotionMaxTimeGet(work->modelId)) {
                work->motionExecF = FALSE;
            }
        } else {
            if (work->modelMotionF) {
                if (mbObjMotionTimeGet(work->modelId) >= mbObjMotionMaxTimeGet(work->modelId)) {
                    work->motionExecF = FALSE;
                }
            } else if (mbObjMotionTimeGet(work->modelId) <= 0.0f) {
                work->motionExecF = FALSE;
            }
            if (!work->motionExecF) {
                for (i = 0; i < 8; i++) {
                    if (work->backModelId[i] != -1 && !work->backDispF[i]) {
                        mbObjDispSet(work->backModelId[i], FALSE);
                    }
                }
            }
        }
    }
}

static void ev_ShopOpenSet(int shopNo, BOOL openF)
{
    OMOBJ *obj = ev_ShopOMObj[shopNo];
    MBSHOPOMWORK *work = obj->data;
    int i;

    if (work->modelId == -1) {
        return;
    }
    work->motionExecF = TRUE;
    work->openF = openF;
    if (work->openF) {
        mbAudFXPlay(0x477);
        mbObjMotionSet(work->modelId, 0, 0);
        mbObjMotionSpeedSet(work->modelId, 1.0f);
        for (i = 0; i < 8; i++) {
            if (work->backModelId[i] == -1) {
                continue;
            }
            switch (work->backMotNo[i]) {
                case 0:
                case 1:
                case 2:
                case 3:
                    mbObjMotionTimeSet(work->backModelId[i], 0.0f);
                    mbObjMotionSpeedSet(work->backModelId[i], 1.0f);
                    mbObjDispSet(work->backModelId[i], TRUE);
                    break;
                default:
                    mbObjDispSet(work->backModelId[i], TRUE);
                    break;
            }
        }
    } else {
        mbAudFXPlay(0x478);
        if (work->modelMotionF) {
            mbObjMotionSet(work->modelId, 1, 0);
            mbObjMotionSpeedSet(work->modelId, 1.0f);
        } else {
            mbObjMotionSpeedSet(work->modelId, -1.0f);
        }
        for (i = 0; i < 8; i++) {
            if (work->backModelId[i] == -1) {
                continue;
            }
            switch (work->backMotNo[i]) {
                case 0:
                case 1:
                    mbObjMotionSpeedSet(work->backModelId[i], -1.0f);
                    break;
                case 2:
                case 3:
                    mbObjMotionTimeSet(work->backModelId[i], 0.0f);
                    mbObjMotionSpeedSet(work->backModelId[i], 1.0f);
                    break;
            }
        }
    }
}

int mbev_Shop(int playerNo, int shopNo)
{
    MBSHOPWORK *work;
    void *workP;

    mbMoveNumDispSet(playerNo, FALSE);
    workP = HuMemDirectMallocNum(HEAP_HEAP, sizeof(MBSHOPWORK), HU_MEMNUM_OVL);
    work = workP;
    memset(work, 0, sizeof(MBSHOPWORK));
    work->playerNo = playerNo;
    work->shopNo = shopNo;
    mbPauseDisableSet(TRUE);
    ev_Shop(work);
    HuMemDirectFree(work);
    mbPauseDisableSet(FALSE);
    mbMoveNumDispSet(playerNo, TRUE);
    return 0;
}

static int ev_ShopMesGet(int messNo)
{
    if (!GwSystem.curTime) {
        return messNo;
    }
    return messNo + 14;
}
