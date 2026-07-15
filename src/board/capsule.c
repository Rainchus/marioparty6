#include "game/board/masu.h"
#include "game/board/object.h"
#include "game/process.h"

#define CAPSULE_OBJ_COLOR_MAX 128

typedef void (*CAPSULE_THROW_HOOK)(BOOL startF);

typedef struct CapsuleObjColor_s {
    s16 flag;
    MBMODELID mdlId;
    MBMODELID mdlId2;
    u8 layer;
    HuVecF pos;
    HuVecF rot;
    HuVecF scale;
} CAPSULE_OBJ_COLOR;

static HUPROCESS *capsulePlayerThrowProc;
static CAPSULE_THROW_HOOK capsuleThrowHook;
static int capsuleUseEffMode[4];
static HuVecF capsuleUseEffPos[4];
static s16 capsuleNum[33][2];
static CAPSULE_OBJ_COLOR capsuleObjColorData[CAPSULE_OBJ_COLOR_MAX];

void MBCapsuleStub1(void)
{
}

BOOL MBCapsuleStub2(void)
{
    return FALSE;
}

int mbCapEffUseModeGet(int playerNo)
{
    return capsuleUseEffMode[playerNo];
}

BOOL mbCapEffUsePosGet(int playerNo, HuVecF *pos)
{
    if (capsuleUseEffMode[playerNo] < 1) {
        return FALSE;
    }
    *pos = capsuleUseEffPos[playerNo];
    return TRUE;
}

BOOL mbCapPlayerThrowCheck(void)
{
    return capsulePlayerThrowProc == NULL;
}

void mbCapThrowHookSet(CAPSULE_THROW_HOOK hook)
{
    capsuleThrowHook = hook;
}

void mbCapAutoThrowEnd(void)
{
}

void mbCapNumInc(int capsuleNo, int mode)
{
    capsuleNum[capsuleNo][mode]++;
}

s16 mbCapValueTypeGet(s16 value)
{
    return (u8)value;
}

s16 mbCapMasuTypeGet(s16 masuId)
{
    return (u8)mbMasuCapsuleGet(masuId);
}

s16 mbCapValuePlayerGet(s16 value)
{
    return (u16)value >> 8;
}

s16 mbCapMasuPlayerGet2(s16 masuId)
{
    return mbMasuCapsuleGet(masuId) >> 8;
}

void mbCapMasuPlayerSet(s16 masuId, s16 playerNo)
{
    s16 capsuleNo = mbMasuCapsuleGet(masuId);

    capsuleNo |= (playerNo & 0xFF) << 8;
    mbMasuCapsuleSet(masuId, capsuleNo);
}

void mbCapMasuPlayerTypeSet(s16 masuId, s16 capsuleNo, s16 playerNo)
{
    mbMasuCapsuleSet(masuId, capsuleNo);
    mbCapMasuPlayerSet(masuId, playerNo);
}

s16 mbCapMasuPlayerGet(s16 masuId)
{
    return mbMasuCapsuleGet(masuId) >> 8;
}

static int CapObjColorSearch(int id)
{
    CAPSULE_OBJ_COLOR *obj;
    int i;

    for (obj = capsuleObjColorData, i = 0; i < CAPSULE_OBJ_COLOR_MAX; i++, obj++) {
        if (obj->flag && obj->mdlId == id) {
            break;
        }
    }
    if (i < CAPSULE_OBJ_COLOR_MAX) {
        return i;
    } else {
        return -1;
    }
}

void mbCapObjColorKill(int id)
{
    CAPSULE_OBJ_COLOR *obj;
    int idx;

    if ((idx = CapObjColorSearch(id)) != -1) {
        obj = &capsuleObjColorData[idx];
        if (obj->flag) {
            mbObjKill(obj->mdlId);
            obj->mdlId = MB_MODEL_NONE;
            obj->mdlId2 = MB_MODEL_NONE;
            obj->flag = FALSE;
        }
    }
}

void mbCapObjColorPosSet(int id, float posX, float posY, float posZ)
{
    CAPSULE_OBJ_COLOR *obj;
    int idx;

    if ((idx = CapObjColorSearch(id)) != -1) {
        obj = &capsuleObjColorData[idx];
        obj->pos.x = posX;
        obj->pos.y = posY;
        obj->pos.z = posZ;
        mbObjPosSetV(obj->mdlId, &obj->pos);
    }
}

void mbCapObjColorRotSet(int id, float rotX, float rotY, float rotZ)
{
    CAPSULE_OBJ_COLOR *obj;
    int idx;

    if ((idx = CapObjColorSearch(id)) != -1) {
        obj = &capsuleObjColorData[idx];
        obj->rot.x = rotX;
        obj->rot.y = rotY;
        obj->rot.z = rotZ;
        mbObjRotSetV(obj->mdlId, &obj->rot);
    }
}

void mbCapObjColorScaleSet(int id, float scaleX, float scaleY, float scaleZ)
{
    CAPSULE_OBJ_COLOR *obj;
    int idx;

    if ((idx = CapObjColorSearch(id)) != -1) {
        obj = &capsuleObjColorData[idx];
        obj->scale.x = scaleX;
        obj->scale.y = scaleY;
        obj->scale.z = scaleZ;
        mbObjScaleSetV(obj->mdlId, &obj->scale);
    }
}

void mbCapObjColorPosSetV(int id, HuVecF *pos)
{
    CAPSULE_OBJ_COLOR *obj;
    int idx;

    if ((idx = CapObjColorSearch(id)) != -1) {
        obj = &capsuleObjColorData[idx];
        mbCapObjColorPosSet(id, pos->x, pos->y, pos->z);
    }
}

void mbCapObjColorRotSetV(int id, HuVecF *rot)
{
    CAPSULE_OBJ_COLOR *obj;
    int idx;

    if ((idx = CapObjColorSearch(id)) != -1) {
        obj = &capsuleObjColorData[idx];
        mbCapObjColorRotSet(id, rot->x, rot->y, rot->z);
    }
}

void mbCapObjColorScaleSetV(int id, HuVecF *scale)
{
    CAPSULE_OBJ_COLOR *obj;
    int idx;

    if ((idx = CapObjColorSearch(id)) != -1) {
        obj = &capsuleObjColorData[idx];
        mbCapObjColorScaleSet(id, scale->x, scale->y, scale->z);
    }
}

void mbCapObjColorPosGet(int id, HuVecF *pos)
{
    CAPSULE_OBJ_COLOR *obj;
    int idx;

    if ((idx = CapObjColorSearch(id)) != -1) {
        obj = &capsuleObjColorData[idx];
        *pos = obj->pos;
    }
}

void mbCapObjColorRotGet(int id, HuVecF *rot)
{
    CAPSULE_OBJ_COLOR *obj;
    int idx;

    if ((idx = CapObjColorSearch(id)) != -1) {
        obj = &capsuleObjColorData[idx];
        *rot = obj->rot;
    }
}

void mbCapObjColorScaleGet(int id, HuVecF *scale)
{
    CAPSULE_OBJ_COLOR *obj;
    int idx;

    if ((idx = CapObjColorSearch(id)) != -1) {
        obj = &capsuleObjColorData[idx];
        *scale = obj->scale;
    }
}

void mbCapObjColorLayerSet(int id, u8 layer)
{
    CAPSULE_OBJ_COLOR *obj;
    int idx;

    if ((idx = CapObjColorSearch(id)) != -1) {
        obj = &capsuleObjColorData[idx];
        obj->layer = layer;
        mbObjLayerSet(obj->mdlId, obj->layer);
    }
}

u8 mbCapObjColorLayerGet(int id)
{
    CAPSULE_OBJ_COLOR *obj;
    int idx;

    if ((idx = CapObjColorSearch(id)) == -1) {
        return 0;
    }
    obj = &capsuleObjColorData[idx];
    return obj->layer;
}

void mbCapObjColorAttrSet(int id, u32 attr)
{
    CAPSULE_OBJ_COLOR *obj;
    int idx;

    if ((idx = CapObjColorSearch(id)) != -1) {
        obj = &capsuleObjColorData[idx];
        mbObjAttrSet(obj->mdlId, attr);
    }
}

void mbCapObjColorAttrReset(int id, u32 attr)
{
    CAPSULE_OBJ_COLOR *obj;
    int idx;

    if ((idx = CapObjColorSearch(id)) != -1) {
        obj = &capsuleObjColorData[idx];
        mbObjAttrReset(obj->mdlId, attr);
    }
}

void mbCapObjColorDispSet(int id, BOOL dispF)
{
    CAPSULE_OBJ_COLOR *obj;
    int idx;

    if ((idx = CapObjColorSearch(id)) != -1) {
        obj = &capsuleObjColorData[idx];
        mbObjDispSet(obj->mdlId, dispF);
    }
}

void mbCapObjColorAlphaSet(int id, u8 alpha)
{
    CAPSULE_OBJ_COLOR *obj;
    int idx;

    if ((idx = CapObjColorSearch(id)) != -1) {
        obj = &capsuleObjColorData[idx];
        mbObjAlphaSet(obj->mdlId, alpha);
    }
}

void mbCapObjColorMtxSet(int id, Mtx *mtx)
{
    CAPSULE_OBJ_COLOR *obj;
    int idx;

    if ((idx = CapObjColorSearch(id)) != -1) {
        obj = &capsuleObjColorData[idx];
        mbObjMtxSet(obj->mdlId, mtx);
    }
}

void mbCapObjColorMtxGet(int id, Mtx *mtx)
{
    CAPSULE_OBJ_COLOR *obj;
    int idx;

    if ((idx = CapObjColorSearch(id)) != -1) {
        obj = &capsuleObjColorData[idx];
        mbObjMtxGet(obj->mdlId, mtx);
    }
}
