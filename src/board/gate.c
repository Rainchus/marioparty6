#include "game/board/gate.h"
#include "game/board/model.h"
#include "game/board/masu.h"
#include "game/board/camera.h"
#include "game/board/window.h"
#include "game/board/player.h"
#include "game/board/coin.h"
#include "game/board/audio.h"
#include "game/board/comchoice.h"

#include "game/memory.h"
#include "humath.h"

#define GATE_MAX 8

#define BOARD_HSF_battan DATANUM(DATA_board, 8)
#define BOARD_HSF_battanIdle DATANUM(DATA_board, 9)
#define BOARD_HSF_battanMove DATANUM(DATA_board, 10)

typedef struct GateBattanWork_s {
    unsigned doneF : 1;
    int playerNo;
    int mode;
    MBMODELID modelId;
    s16 masuId;
    float rotY;
    float masuAngle;
    float targetAngle;
    int no;
    s16 time;
    s16 maxTime;
    OMOBJ *obj;
} GATEBATTAN_WORK;

static GATEBATTAN_WORK *gateBattanDataP[GATE_MAX];

static int gateBattanSE;
static int gateBattanNum;

void mbGateInit(void)
{
    gateBattanNum = 0;
    memset(gateBattanDataP, 0, GATE_MAX*sizeof(GATEBATTAN_WORK *));
}

void mbGateClose(void)
{
    int i;
    for(i=0; i<gateBattanNum; i++) {
        mbGateBattanKill(i);
    }
}

void mbGateCreate(void)
{
    int i;
    for(i=0; i<mbMasuRawNumGet(); i++) {
        MASU *masuP = mbMasuGet(i+1);
        if(masuP->flag & MASU_FLAG_BATTAN) {
            mbGateBattanCreate(i+1);
        }
    }
}

static void GateBattanOMExec(OMOBJ *obj);

void mbGateBattanCreate(int masuId)
{
    GATEBATTAN_WORK *work;
    GATEBATTAN_WORK *temp;
    int i;

    HuVecF startPos;
    HuVecF endPos;
    HuVecF pos;
    HuVecF dir;
    int motTbl[] = {
        BOARD_HSF_battanIdle,
        BOARD_HSF_battanMove,
        HU_DATANUM_NONE,
    };
    s16 path[MASU_LINK_MAX];

    temp = HuMemDirectMallocNum(HEAP_HEAP, sizeof(GATEBATTAN_WORK), HU_MEMNUM_OVL);
    work = temp;
    gateBattanDataP[gateBattanNum] = work;
    memset(work, 0, sizeof(GATEBATTAN_WORK));
    for(i=0; motTbl[i] >= 0; i++) {
        motTbl[i] = mbBoardDataNumGet(motTbl[i]);
    }
    work->modelId = mbObjCreate(mbBoardDataNumGet(BOARD_HSF_battan), motTbl, FALSE);
    mbObjMotionSet(work->modelId, 1, HU3D_MOTATTR_LOOP);
    work->no = gateBattanNum;
    work->masuId = masuId;
    mbMasuPosGet(masuId, &startPos);
    mbMasuLinkTblGet(masuId, path);
    mbMasuPosGet(path[0], &endPos);
    VECSubtract(&endPos, &startPos, &dir);
    work->rotY = work->masuAngle = HuAtan(-dir.x, -dir.z);
    VECScale(&dir, &dir, 0.5f);
    VECAdd(&startPos, &dir, &pos);
    mbObjPosSetV(work->modelId, &pos);
    mbObjRotSet(work->modelId, 0, work->rotY, 0);
    work->obj = omAddObj(mbObjMan, 260, 0, 0, GateBattanOMExec);
    work->obj->work[0] = (u32)work;
    gateBattanNum++;
}

void mbGateBattanKill(int no)
{
    GATEBATTAN_WORK *work = gateBattanDataP[no];
    mbObjKill(work->modelId);
    HuMemDirectFree(work);
    gateBattanDataP[no] = NULL;
}

s16 mbev_GateMasu(int playerNo, s16 masuId, s16 masuNext)
{
    s16 path[MASU_LINK_MAX];

    int no;
    void *work;

    if(mbMasuAttrGet(masuId) & MASU_FLAG_BATTAN) {
        mbMasuLinkTblGet(masuId, path);
        no = mbev_GateBattanMasuFind(masuId);
        work = gateBattanDataP[no];
        if(masuNext == path[0] && !mbev_GateBattan(playerNo, masuId)) {
            masuNext = path[1];
        }
    }
    return masuNext;
}

int mbev_GateBattanMasuFind(s16 masuId)
{
    int i;
    for(i=0; i<gateBattanNum; i++) {
        GATEBATTAN_WORK *work = gateBattanDataP[i];
        if(work->masuId == masuId) {
            break;
        }
    }
    if(i < gateBattanNum) {
        return i;
    }
    return -1;
}

static void GateBattanMoveStart(GATEBATTAN_WORK *work, int playerNo);
static BOOL GateBattanMoveCheck(GATEBATTAN_WORK *work);

int mbev_GateBattan(int playerNo, int masuId)
{
    int result = 1;
    int no;
    GATEBATTAN_WORK *work;

    HuVecF modelPos;
    HuVecF playerPos;
    HuVecF dir;

    float angle;

    mbMoveNumDispSet(playerNo, FALSE);
    masuId = GwPlayer[playerNo].masuId;
    no = mbev_GateBattanMasuFind(masuId);
    work = gateBattanDataP[no];
    mbObjPosGet(work->modelId, &modelPos);
    mbPlayerPosGet(playerNo, &playerPos);
    VECSubtract(&modelPos, &playerPos, &dir);
    angle = 90-HuAtan(dir.z, dir.x);
    mbPlayerRotateStart(playerNo, angle, 15);
    while(!mbPlayerRotateCheck(playerNo)) {
        HuPrcVSleep();
    }
    mbPlayerMotIdleSet(playerNo);
    mbCameraPlayerViewSet(playerNo, MB_CAMERA_VIEW_ZOOMIN);
    mbCameraMoveWait();
    mbCameraFocusObjSet(MB_MODEL_NONE);
    if(mbPlayerCoinGet(playerNo) < 10) {
        mbAudFXPlay(0x3A0);
        mbWinCreate(MBWIN_TYPE_RESULT, MESS_BOARD_GATE_BATTAN_COIN, HUWIN_SPEAKER_BATTAN);
        mbWinTopWait();
        result = 0;
    } else {
        mbAudFXPlay(0x3A0);
        mbWinCreateChoice(MBWIN_TYPE_EVENT, MESS_BOARD_GATE_BATTAN_CHOICE, HUWIN_SPEAKER_BATTAN, 0);
        mbWinTopAttrReset(HUWIN_ATTR_NOCANCEL);
        if(GwPlayer[playerNo].comF) {
            mbComChoiceLeftSet();
        }
        mbWinTopWait();
        if(mbWinTopChoiceGet() == 1 || mbWinTopChoiceGet() == -1) {
            mbAudFXPlay(0x3A0);
            mbWinCreate(MBWIN_TYPE_RESULT, MESS_BOARD_GATE_BATTAN_NO, HUWIN_SPEAKER_BATTAN);
            mbWinTopWait();
            result = 0;
        } else {
            mbCoinAddDispExec(playerNo, -10, TRUE, TRUE);
            mbAudFXPlay(0x3A0);
            mbWinCreate(MBWIN_TYPE_RESULT, MESS_BOARD_GATE_BATTAN_YES, HUWIN_SPEAKER_BATTAN);
            mbWinTopWait();
            GateBattanMoveStart(work, playerNo);
            while(!GateBattanMoveCheck(work)) {
                HuPrcVSleep();
            }
        }
    }
    mbCameraPlayerViewSet(playerNo, MB_CAMERA_VIEW_WALK);
    mbCameraMoveWait();
    mbMoveNumDispSet(playerNo, TRUE);
    return result;
}

static void GateBattanOMExec(OMOBJ *obj)
{
    GATEBATTAN_WORK *work = (GATEBATTAN_WORK *)obj->work[0];
    float weight;
    float motTime;
    if(mbExitCheck()) {
        omDelObjEx(HuPrcCurrentGet(), obj);
        return;
    }
    switch(work->mode) {
        case 0:
            break;

        case 1:
            if(work->time > work->maxTime) {
                work->doneF = TRUE;
                work->mode = 2;
                mbObjMotionShiftSet(work->modelId, 1, 0, 10, HU3D_MOTATTR_LOOP);
                return;
            } else {
                weight = ((float)work->time++)/work->maxTime;
                obj->trans.x = obj->rot.x+(weight*(obj->scale.x-obj->rot.x));
                obj->trans.y = obj->rot.y+(weight*(obj->scale.y-obj->rot.y));
                obj->trans.z = obj->rot.z+(weight*(obj->scale.z-obj->rot.z));
                mbObjPosSetV(work->modelId, &obj->trans);
                mbAngleMoveTo(&work->rotY, work->targetAngle, 0.1f);
                mbObjRotSet(work->modelId, 0, work->rotY, 0);
            }
            break;

        case 2:
            if(work->masuId != GwPlayer[work->playerNo].masuId) {
                work->mode = 3;
                work->time = 0;
                work->maxTime = 120;
                work->targetAngle = 180+work->targetAngle;
                mbObjMotionShiftSet(work->modelId, 2, 0, 5, HU3D_MOTATTR_LOOP);
            }
            break;

        case 3:
            if(work->time > work->maxTime) {
                if(mbAngleMoveTo(&work->rotY, work->masuAngle, 0.2f)) {
                    work->mode = 0;
                    mbObjMotionShiftSet(work->modelId, 1, 0, 5, HU3D_MOTATTR_LOOP);
                    mbAudFXPlay(0x39F);
                }
                mbObjRotSet(work->modelId, 0, work->rotY, 0);
                return;
            } else {
                weight = ((float)work->time++)/work->maxTime;
                obj->trans.x = obj->scale.x+(weight*(obj->rot.x-obj->scale.x));
                obj->trans.y = obj->scale.y+(weight*(obj->rot.y-obj->scale.y));
                obj->trans.z = obj->scale.z+(weight*(obj->rot.z-obj->scale.z));
                mbObjPosSetV(work->modelId, &obj->trans);
                mbAngleMoveTo(&work->rotY, work->targetAngle, 0.1f);
                mbObjRotSet(work->modelId, 0, work->rotY, 0);
            }
            break;
    }
    if(work->mode == 1 || work->mode == 3) {
        motTime = mbObjMotionTimeGet(work->modelId);
        if(motTime == 5.0f || motTime == 30.0f) {
            mbAudFXPlay(0x39F);
        }
    }
}

static void GateBattanMoveStart(GATEBATTAN_WORK *work, int playerNo)
{
    s16 path[MASU_LINK_MAX];
    HuVecF masuPos;
    HuVecF dir;
    HuVecF startPos;
    HuVecF endPos;
    s16 masuId;

    work->doneF = FALSE;
    work->mode = 1;
    work->time = 0;
    work->maxTime = 120;
    work->playerNo = playerNo;
    mbObjMotionShiftSet(work->modelId, 2, 0, 5, HU3D_MOTATTR_LOOP);
    mbObjPosGet(work->modelId, &work->obj->rot);
    mbMasuPosGet(work->masuId, &masuPos);
    masuId = work->masuId;
    mbMasuLinkTblGet(masuId, path);
    mbMasuPosGet(path[0], &startPos);
    mbMasuPosGet(path[1], &endPos);
    VECSubtract(&startPos, &masuPos, &startPos);
    VECSubtract(&endPos, &masuPos, &endPos);
    VECScale(&startPos, &startPos, 0.5f);
    VECScale(&endPos, &endPos, 0.5f);
    work->obj->scale.x = masuPos.x+startPos.x+endPos.x;
    work->obj->scale.y = masuPos.y+startPos.y+endPos.y;
    work->obj->scale.z = masuPos.z+startPos.z+endPos.z;
    VECSubtract(&work->obj->scale, &work->obj->rot, &dir);
    work->targetAngle = HuAtan(dir.x, dir.z);
    mbAudFXPlay(0);
}

static BOOL GateBattanMoveCheck(GATEBATTAN_WORK *work)
{
    return work->doneF;
}
