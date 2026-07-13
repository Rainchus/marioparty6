#include "game/board/masu.h"
#include "game/board/branch.h"
#include "game/data.h"
#include "game/gamework.h"
#include "game/hsfex.h"
#include "game/hu3d.h"
#include <string.h>

static int masuLayer;
static s16 masuNum[MASU_LAYER_MAX];
static MASU *masuData[MASU_LAYER_MAX];
static BOOL masuDispF;
static u16 masuDispAttrMask[MASU_LAYER_MAX];
static u32 masuDispMAttrMask[MASU_LAYER_MAX];
static MASUPATHCHECKHOOK masuev_LinkTblHook;

#define DATA_READ16(ptr, dst) do { \
    (dst) = *(u16 *)(ptr); \
    (ptr) = (u8 *)(ptr) + sizeof(u16); \
} while (0)

#define DATA_READLINK(ptr, dst) do { \
    (dst) = *(u16 *)(ptr) + 1; \
    (ptr) = (u8 *)(ptr) + sizeof(u16); \
} while (0)

#define DATA_READ32(ptr, dst) do { \
    (dst) = *(u32 *)(ptr); \
    (ptr) = (u8 *)(ptr) + sizeof(u32); \
} while (0)

#define DATA_READCOUNT(data, ptr, dst) do { \
    (dst) = *(s32 *)(data); \
    (ptr) = (u8 *)(data) + sizeof(s32); \
} while (0)

#define DATA_READVEC(ptr, dst) do { \
    memcpy(&(dst), (ptr), sizeof(HuVecF)); \
    (ptr) = (u8 *)(ptr) + sizeof(HuVecF); \
} while (0)

BOOL mbMasuDataRead(int dataNum)
{
    MASU *masuP;
    void *ptr;
    int i;
    int j;
    int *data;

    data = HuDataSelHeapReadNum(dataNum, HU_MEMNUM_OVL, HEAP_MODEL);
    DATA_READCOUNT(data, ptr, masuNum[masuLayer]);

    masuP = &masuData[masuLayer][0];
    masuP->pos.x = masuP->pos.y = masuP->pos.z = 0.0f;
    masuP->pos.z = 100000.0f;
    masuP->rot.x = masuP->rot.y = masuP->rot.z = 0.0f;
    masuP->scale.x = masuP->scale.y = masuP->scale.z = 1.0f;
    PSMTXIdentity(masuP->matrix);
    masuP->linkNum = 0;
    masuP->capsuleNo = MASU_NULL;

    masuP++;
    for (i = 0; i < masuNum[masuLayer]; i++, masuP++) {
        masuP->id = i + 1;
        DATA_READVEC(ptr, masuP->pos);
        DATA_READVEC(ptr, masuP->rot);
        DATA_READVEC(ptr, masuP->scale);
        DATA_READ16(ptr, masuP->flag);
        DATA_READ32(ptr, masuP->mAttr);
        DATA_READ16(ptr, masuP->type);
        masuP->capsuleNo = GwSystem.masuCapsule[masuP->id];
        DATA_READ16(ptr, masuP->linkNum);
        for (j = 0; j < masuP->linkNum; j++) {
            DATA_READLINK(ptr, masuP->linkTbl[j]);
        }
    }
    HuDataClose(data);
    return TRUE;
}

#undef DATA_READ16
#undef DATA_READLINK
#undef DATA_READ32
#undef DATA_READCOUNT
#undef DATA_READVEC

int mbMasuNumGet(void)
{
    return masuNum[masuLayer] + 1;
}

int mbMasuRawNumGet(void)
{
    return masuNum[masuLayer];
}

MASU *mbMasuGet(s16 id)
{
    return &masuData[masuLayer][id];
}

void mbMasuLayerSet(int layer)
{
    masuLayer = layer;
}

int mbMasuLayerGet(void)
{
    return masuLayer;
}

u16 mbMasuAttrGet(s16 id)
{
    return masuData[masuLayer][id].flag;
}

void mbMasuAttrSet(s16 id, u16 attr)
{
    masuData[masuLayer][id].flag = attr;
}

u32 mbMasuMAttrGet(s16 id)
{
    return masuData[masuLayer][id].mAttr;
}

void mbMasuMAttrSet(s16 id, u32 attr)
{
    masuData[masuLayer][id].mAttr = attr;
}

u16 mbMasuTypeGet(s16 id)
{
    return masuData[masuLayer][id].type;
}

void mbMasuTypeSet(s16 id, int type)
{
    masuData[masuLayer][id].type = type;
}

s16 mbMasuCapsuleGet(s16 id)
{
    return masuData[masuLayer][id].capsuleNo;
}

void mbMasuPosGet(s16 id, HuVecF *pos)
{
    MASU *masuP = &masuData[masuLayer][id];

    if (!masuP->useMtxF) {
        *pos = masuP->pos;
    } else {
        pos->x = masuP->matrix[0][3];
        pos->y = masuP->matrix[1][3];
        pos->z = masuP->matrix[2][3];
    }
}

void mbMasuPosSet(s16 id, float x, float y, float z)
{
    MASU *masuP = &masuData[masuLayer][id];

    masuP->pos.x = x;
    masuP->pos.y = y;
    masuP->pos.z = z;
}

void mbMasuPosSetV(s16 id, HuVecF *pos)
{
    mbMasuPosSet(id, pos->x, pos->y, pos->z);
}

void mbMasuRotGet(s16 id, HuVecF *rot)
{
    MASU *masuP = &masuData[masuLayer][id];

    if (masuP->useMtxF) {
        Hu3DMtxRotGet(masuP->matrix, rot);
    } else {
        *rot = masuP->rot;
    }
}

void mbMasuRotSet(s16 id, float x, float y, float z)
{
    MASU *masuP = &masuData[masuLayer][id];

    masuP->rot.x = x;
    masuP->rot.y = y;
    masuP->rot.z = z;
}

void mbMasuRotSetV(s16 id, HuVecF *rot)
{
    mbMasuRotSet(id, rot->x, rot->y, rot->z);
}

void mbMasuMtxGet(s16 id, Mtx matrix)
{
    MASU *masuP = &masuData[masuLayer][id];

    if (masuP->useMtxF) {
        PSMTXCopy(masuP->matrix, matrix);
    } else {
        Mtx rot;
        Mtx trans;

        mtxRot(rot, masuP->rot.x, masuP->rot.y, masuP->rot.z);
        PSMTXTrans(trans, masuP->pos.x, masuP->pos.y, masuP->pos.z);
        PSMTXConcat(trans, rot, matrix);
    }
}

void mbMasuMtxSet(s16 id, Mtx matrix)
{
    MASU *masuP = &masuData[masuLayer][id];

    if (matrix != NULL) {
        masuP->useMtxF = TRUE;
        PSMTXCopy(matrix, masuP->matrix);
    } else {
        masuP->useMtxF = FALSE;
    }
}

BOOL mbMasuDispGet(void)
{
    return masuDispF;
}

void mbMasuDispSet(BOOL dispF)
{
    masuDispF = dispF;
}

void mbMasuDispAttrSet(u16 attr)
{
    masuDispAttrMask[masuLayer] |= attr;
}

void mbMasuDispAttrReset(u16 attr)
{
    masuDispAttrMask[masuLayer] &= ~attr;
}

u16 mbMasuDispAttrGet(void)
{
    return masuDispAttrMask[masuLayer];
}

void mbMasuDispMAttrSet(u32 attr)
{
    masuDispMAttrMask[masuLayer] |= attr;
}

void mbMasuDispMAttrReset(u32 attr)
{
    masuDispMAttrMask[masuLayer] &= ~attr;
}

u32 mbMasuDispMAttrGet(void)
{
    return masuDispMAttrMask[masuLayer];
}

s16 mbMasuLinkGet(s16 id, int linkNo)
{
    MASU *masuP = &masuData[masuLayer][id];

    return masuP->linkTbl[linkNo];
}

s16 mbMasuLinkNumGet(s16 id)
{
    MASU *masuP = &masuData[masuLayer][id];

    return masuP->linkNum;
}

int mbMasuLinkTblGet(s16 id, s16 *linkTbl)
{
    return mbMasuLinkTblGet2(id, linkTbl, FALSE);
}

int mbMasuLinkTblGet2(s16 id, s16 *linkTbl, BOOL hookF)
{
    MASU *masuP = &masuData[masuLayer][id];
    int linkNum;
    int i;

    if (masuev_LinkTblHook != NULL && hookF) {
        linkNum = masuev_LinkTblHook(id, masuP->mAttr, linkTbl, hookF);
        if (linkNum > 0) {
            return linkNum;
        }
    }
    linkNum = 0;
    for (i = 0; i < masuP->linkNum; i++) {
        MASU *linkMasuP = mbMasuGet(masuP->linkTbl[i]);

        if ((linkMasuP->flag & mbBranchAttrGet()) != 0
            || (linkMasuP->mAttr & mbBranchMAttrGet()) != 0) {
            continue;
        }
        if (linkTbl) {
            linkTbl[linkNum] = masuP->linkTbl[i];
        }
        linkNum++;
    }
    return linkNum;
}

s16 mbMasuAttrFindLink(s16 id, u16 attr)
{
    MASU *masuP = &masuData[masuLayer][id];
    int i;

    for (i = 0; i < masuP->linkNum; i++) {
        MASU *linkMasuP = mbMasuGet(masuP->linkTbl[i]);

        if ((linkMasuP->flag & attr) != 0) {
            return masuP->linkTbl[i];
        }
    }
    return MASU_NULL;
}

s16 mbMasuAttrMatchFindLink(s16 id, u16 attr, u16 mask)
{
    MASU *masuP = &masuData[masuLayer][id];
    int i;

    for (i = 0; i < masuP->linkNum; i++) {
        MASU *linkMasuP = mbMasuGet(masuP->linkTbl[i]);

        if (attr == (linkMasuP->flag & mask)) {
            return masuP->linkTbl[i];
        }
    }
    return MASU_NULL;
}

s16 mbMasuMAttrFindLink(s16 id, u32 attr)
{
    MASU *masuP = &masuData[masuLayer][id];
    int i;

    for (i = 0; i < masuP->linkNum; i++) {
        MASU *linkMasuP = mbMasuGet(masuP->linkTbl[i]);

        if ((linkMasuP->mAttr & attr) != 0) {
            return masuP->linkTbl[i];
        }
    }
    return MASU_NULL;
}

s16 mbMasuMAttrMatchFindLink(s16 id, u32 attr, u32 mask)
{
    MASU *masuP = &masuData[masuLayer][id];
    int i;

    for (i = 0; i < masuP->linkNum; i++) {
        MASU *linkMasuP = mbMasuGet(masuP->linkTbl[i]);

        if (attr == (linkMasuP->mAttr & mask)) {
            return masuP->linkTbl[i];
        }
    }
    return MASU_NULL;
}

s16 mbMasuTypeFindLink(s16 id, int type)
{
    MASU *masuP = &masuData[masuLayer][id];
    int i;

    for (i = 0; i < masuP->linkNum; i++) {
        MASU *linkMasuP = mbMasuGet(masuP->linkTbl[i]);

        if (linkMasuP->type == type) {
            return masuP->linkTbl[i];
        }
    }
    return MASU_NULL;
}

s16 mbMasuLinkParentGet(s16 id, s16 *linkTbl)
{
    MASU *masuP;
    int i;
    int j;
    int linkNum = 0;

    for (masuP = &masuData[masuLayer][1], i = 0;
         i < masuNum[masuLayer];
         i++, masuP++) {
        for (j = 0; j < masuP->linkNum; j++) {
            if (masuP->linkTbl[j] == id) {
                linkTbl[linkNum++] = i + 1;
            }
        }
    }
    return linkNum;
}
