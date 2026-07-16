#include <string.h>

#include "datadir_enum.h"

#include "dolphin/os.h"

#include "game/armem.h"
#include "game/charman.h"
#include "game/flag.h"
#include "game/gamework.h"
#include "game/hu3d.h"
#include "game/object.h"
#include "game/sprite.h"
#include "game/window.h"

#define sind(x) sin((M_PI * (x)) / 180.0)

typedef void (*VoidFunc)(void);

typedef struct MdCameraWork_s MDCAMERA_WORK;
typedef void (*MDCAMERA_CALLBACK)(OMOBJ *obj, MDCAMERA_WORK *camera);

struct MdCameraWork_s {
    OMOBJ *obj;
    HuVecF center;
    HuVecF unk_10;
    HuVecF rot;
    HuVecF unk_28;
    float zoom;
    float unk_38;
    MDCAMERA_CALLBACK callback;
    u8 unk_40[0x10];
};

typedef struct Lbl1DataC0Entry {
    s16 groupNo;
    s16 memberNo;
    s16 animNo;
    s16 priority;
    s16 bank;
    s16 pad;
    HuVec2f pos;
    HuVec2f scale;
    float zRot;
} LBL_1_DATA_C0_ENTRY;

typedef struct Lbl1Bss288Entry {
    HU3D_MODELID modelId;
    HU3D_ANIMID animId[4];
    u8 unk_A[0x2E];
} LBL_1_BSS_288_ENTRY;

typedef struct Lbl1Bss1C {
    OMOBJ *obj;
    u8 unk_4[4];
} LBL_1_BSS_1C;

extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];

extern void HuDataDirCloseAll(void);
extern int HuAudFXPlay(int seId);
extern int HuAudFXPlayPan(int seId, int pan);

extern s16 lbl_1_bss_28;
extern s16 lbl_1_bss_2A;
extern s16 lbl_1_bss_2C;
extern OMOBJMAN *lbl_1_bss_0;
extern OMOBJ *lbl_1_bss_4;
extern OMOBJ *lbl_1_bss_8;
extern OMOBJ *lbl_1_bss_C;
extern OMOBJ *lbl_1_bss_10;
extern OMOBJ *lbl_1_bss_14;
extern OMOBJ *lbl_1_bss_18;
extern LBL_1_BSS_1C lbl_1_bss_1C;
extern OMOBJ *lbl_1_bss_24;
extern HUWINID lbl_1_bss_104[2];
extern LBL_1_BSS_288_ENTRY lbl_1_bss_288[25];
extern ANIMDATA *lbl_1_bss_800[41];
extern HUSPRID lbl_1_bss_8A4[58];
extern HUSPR_GROUPID lbl_1_bss_918[18];
extern ANIMDATA *lbl_1_bss_93C[32];
extern MDCAMERA_WORK lbl_1_bss_A10;
extern HUWINID lbl_1_bss_A60[4];
extern HU3D_LIGHTID lbl_1_bss_A68[2];

extern s32 lbl_1_data_BC8;
extern u32 lbl_1_data_1C[32];
extern s16 lbl_1_data_9C[18];
extern LBL_1_DATA_C0_ENTRY lbl_1_data_C0[58];
extern u32 lbl_1_data_800[41];
extern char lbl_1_data_BCC[];
extern s16 lbl_1_data_BEE[7];
extern char lbl_1_data_C66[];
extern char lbl_1_data_C77[];
extern char lbl_1_data_C89[];
extern char lbl_1_data_C9B[];
extern char lbl_1_data_CA9[];
extern char lbl_1_data_CDC[];
extern char lbl_1_data_D0C[];
extern char lbl_1_data_DF2[];
extern char lbl_1_data_DFA[];
extern char lbl_1_data_E02[];
extern char lbl_1_data_E0A[];
extern s16 lbl_1_data_DA2[3];
extern s32 lbl_1_data_DA8[3];
extern char lbl_1_data_ED8[];

void fn_1_0(HUWINID winId, u32 mess, s16 index);
void fn_1_1248(void);
void fn_1_12F8(void);
void fn_1_2D1D4(void);
void fn_1_2DD38(void);
void fn_1_2DFD4(OMOBJ *obj, MDCAMERA_WORK *camera);
void ObjectSetup(void);
void fn_1_32D0(OMOBJ *obj);
void fn_1_467C(void);
void fn_1_605C(OMOBJ *obj);
void fn_1_7064(OMOBJ *obj);
void fn_1_7ED4(OMOBJ *obj);
void fn_1_861C(OMOBJ *obj);
void fn_1_9218(OMOBJ *obj);
void fn_1_A5D8(OMOBJ *obj);
void fn_1_B4C8(OMOBJ *obj);
void fn_1_D748(void);
void fn_1_2CE00(void);
void fn_1_44DCC(OMOBJMAN *objman);

void fn_1_0(HUWINID winId, u32 mess, s16 index)
{
    s32 messNum[3] = { 0xD0000, 0xD0025, -1 };
    s32 fxNum[16] = {
        0x3B5, 0x3B6, 0x3B7, 0x3B8, 0x3B9, 0x3BA, 0x3BB, -1,
        0x3AD, 0x3AE, 0x3AF, 0x3B0, 0x3B1, 0x3B2, 0x3B3, -1,
    };
    s16 i;

    index--;
    OSReport(lbl_1_data_BCC, index);
    if (lbl_1_data_BC8 != mess) {
        lbl_1_data_BC8 = mess;
        for (i = 0;; i++) {
            if (messNum[i] == -1) {
                HuAudFXPlay(fxNum[index]);
                break;
            }
            if (mess == messNum[i]) {
                if (index >= 8) {
                    HuAudFXPlayPan(fxNum[index], 0x50);
                } else {
                    HuAudFXPlayPan(fxNum[index], 0x30);
                }
                break;
            }
        }
    }
}

void fn_1_1248(void)
{
    CharDataClose(-1);
    HuARDirFree(DATA_board);
    HuARDirFree(DATA_board_us);
    HuARDirFree(DATA_capsule);
    OSReport(lbl_1_data_CDC);
    OSReport(lbl_1_data_C66, 0x21);
    OSReport(lbl_1_data_C77, 0x24);
    OSReport(lbl_1_data_C89, 0x9B);
    OSReport(lbl_1_data_C9B, 0xF2);
    HuAMemDump();
    OSReport(lbl_1_data_CA9);
}

void fn_1_12F8(void)
{
    CharDataClose(-1);
    HuARDirFree(DATA_board);
    HuARDirFree(DATA_board_us);
    HuARDirFree(DATA_capsule);
    OSReport(lbl_1_data_D0C);
    OSReport(lbl_1_data_C66, 0x21);
    OSReport(lbl_1_data_C77, 0x24);
    OSReport(lbl_1_data_C89, 0x9B);
    OSReport(lbl_1_data_C9B, 0xF2);
    HuAMemDump();
    OSReport(lbl_1_data_CA9);
}

float fn_1_13A8(float arg0, float arg1, float arg2, float arg3)
{
    float temp = 1.0f - arg3;

    return (arg2 * (arg3 * arg3))
        + ((arg0 * (temp * temp)) + ((arg1 * (temp * arg3)) * 2.0f));
}

void fn_1_1404(
    HuVecF *dst, const HuVecF *a, const HuVecF *b, const HuVecF *c, float t)
{
    dst->x = fn_1_13A8(a->x, b->x, c->x, t);
    dst->y = fn_1_13A8(a->y, b->y, c->y, t);
    dst->z = fn_1_13A8(a->z, b->z, c->z, t);
}

inline void fn_1_1404(
    HuVecF *dst, const HuVecF *a, const HuVecF *b, const HuVecF *c, float t);

float fn_1_160C(float arg0, float arg1, float arg2)
{
    if (arg0 == arg1) {
        return arg1;
    }
    return (arg1 + (arg0 * (arg2 - 1.0f))) / arg2;
}

void fn_1_163C(HuVecF *dst, const HuVecF *src, float weight)
{
    dst->x = fn_1_160C(dst->x, src->x, weight);
    dst->y = fn_1_160C(dst->y, src->y, weight);
    dst->z = fn_1_160C(dst->z, src->z, weight);
}

inline void fn_1_163C(HuVecF *dst, const HuVecF *src, float weight);

float fn_1_1780(float arg0, float arg1, float time, float duration)
{
    if (time <= 0.0f) {
        return arg0;
    }
    if (time >= duration) {
        return arg1;
    }
    return arg0 + ((arg1 - arg0) * sind((90.0f / duration) * time));
}

float fn_1_1868(float arg0, float arg1, float arg2, float arg3)
{
    if (arg2 <= 0.0f) {
        return arg0;
    }
    if (arg2 >= arg3) {
        return arg1;
    }
    return arg0 + ((arg2 / arg3) * (arg1 - arg0));
}

void fn_1_18AC(
    HU3D_MODELID modelId, HuVecF *start, HuVecF *end, float time,
    float duration)
{
    HuVecF modelPos;
    HuVecF modelRot;
    HuVecF pos;
    HuVecF rot;

    Hu3DModelPosGet(modelId, &modelPos);
    Hu3DModelRotGet(modelId, &modelRot);
    pos.x = fn_1_1868(start->x, end->x, time, duration);
    pos.y = fn_1_1868(start->y, end->y, time, duration);
    pos.z = fn_1_1868(start->z, end->z, time, duration);
    modelPos.x -= pos.x;
    modelPos.z -= pos.z;
    rot.y = -(180.0 * (atan2(modelPos.x, -modelPos.z) / M_PI));
    if (modelRot.y - rot.y > 180.0f) {
        modelRot.y -= 360.0f;
    } else if (modelRot.y - rot.y < -180.0f) {
        modelRot.y += 360.0f;
    }
    rot.x = modelRot.x;
    rot.y = fn_1_160C(modelRot.y, rot.y, 10.0f);
    rot.z = modelRot.z;
    Hu3DModelPosSetV(modelId, &pos);
    Hu3DModelRotSetV(modelId, &rot);
}

inline void fn_1_18AC(
    HU3D_MODELID modelId, HuVecF *start, HuVecF *end, float time,
    float duration);

void fn_1_1C14(HU3D_MODELID modelId, float rotY)
{
    HuVecF modelRot;
    HuVecF rot;

    Hu3DModelRotGet(modelId, &modelRot);
    rot.y = rotY;
    if (modelRot.y - rot.y > 180.0f) {
        modelRot.y -= 360.0f;
    } else if (modelRot.y - rot.y < -180.0f) {
        modelRot.y += 360.0f;
    }
    rot.x = modelRot.x;
    rot.y = fn_1_160C(modelRot.y, rot.y, 10.0f);
    rot.z = modelRot.z;
    Hu3DModelRotSetV(modelId, &rot);
}

inline void fn_1_1C14(HU3D_MODELID modelId, float rotY);

void fn_1_1D54(
    HU3D_MODELID modelId, HuVecF *start, HuVecF *end, float time,
    float duration, float rotY)
{
    if (time <= duration) {
        fn_1_18AC(modelId, start, end, time, duration);
    } else {
        fn_1_1C14(modelId, rotY);
    }
}

float fn_1_21D4(float arg0, float arg1, float time, float duration)
{
    if (time <= 0.0f || time >= duration) {
        return arg0;
    }
    return arg0 + ((arg1 - arg0) * sind((180.0f / duration) * time));
}

void fn_1_22A8(
    HU3D_MODELID modelId, HuVecF *a, HuVecF *b, HuVecF *c, float time,
    float duration)
{
    HuVecF modelPos;
    HuVecF modelRot;
    HuVecF pos;

    fn_1_1404(
        &pos, a, b, c, fn_1_1868(0.0f, 1.0f, time, duration));
    Hu3DModelPosGet(modelId, &modelPos);
    Hu3DModelRotGet(modelId, &modelRot);
    modelPos.x -= pos.x;
    modelPos.z -= pos.z;
    modelPos.y = -(180.0 * (atan2(modelPos.x, -modelPos.z) / M_PI));
    if (modelRot.y - modelPos.y > 180.0f) {
        modelRot.y -= 360.0f;
    } else if (modelRot.y - modelPos.y < -180.0f) {
        modelRot.y += 360.0f;
    }
    modelRot.y = fn_1_160C(modelRot.y, modelPos.y, 10.0f);
    Hu3DModelPosSet(modelId, pos.x, pos.y, pos.z);
    Hu3DModelRotSet(modelId, 0.0f, modelRot.y, 0.0f);
}

void fn_1_2810(MDCAMERA_WORK *camera)
{
    memcpy(&camera->center, &camera->unk_10, sizeof(HuVecF));
    memcpy(&camera->rot, &camera->unk_28, sizeof(HuVecF));
    camera->zoom = camera->unk_38;
}

inline void fn_1_2810(MDCAMERA_WORK *camera);

void fn_1_2860(MDCAMERA_WORK *camera)
{
    memcpy(&camera->unk_10, &camera->center, sizeof(HuVecF));
    memcpy(&camera->unk_28, &camera->rot, sizeof(HuVecF));
    camera->unk_38 = camera->zoom;
}

inline void fn_1_2860(MDCAMERA_WORK *camera);

void fn_1_28B0(MDCAMERA_WORK *camera, float weight)
{
    fn_1_163C(&camera->center, &camera->unk_10, weight);
    fn_1_163C(&camera->rot, &camera->unk_28, weight);
    camera->zoom = fn_1_160C(camera->zoom, camera->unk_38, weight);
}

inline void fn_1_28B0(MDCAMERA_WORK *camera, float weight);

void fn_1_2B64(MDCAMERA_CALLBACK callback)
{
    lbl_1_bss_A10.callback = callback;
}

inline void fn_1_2B64(MDCAMERA_CALLBACK callback);

void fn_1_3284(OMOBJ *obj, MDCAMERA_WORK *camera)
{
    if (camera->callback) {
        camera->callback(obj, camera);
    }
}

inline void fn_1_3284(OMOBJ *obj, MDCAMERA_WORK *camera);

void fn_1_32D0(OMOBJ *obj)
{
    MDCAMERA_WORK *camera = &lbl_1_bss_A10;

    fn_1_3284(obj, camera);
    Center.x = camera->center.x;
    Center.y = camera->center.y;
    Center.z = camera->center.z;
    CRot.x = camera->rot.x;
    CRot.y = camera->rot.y;
    CRot.z = camera->rot.z;
    CZoom = camera->zoom;
    omOutView(obj);
}

void fn_1_33A0(MDCAMERA_CALLBACK callback)
{
    MDCAMERA_WORK *camera = &lbl_1_bss_A10;

    Hu3DCameraCreate(1);
    Hu3DCameraPerspectiveSet(1, 30.0f, 10.0f, 10000.0f, 1.2f);
    Hu3DCameraViewportSet(
        1, 0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
    memset(camera, 0, sizeof(MDCAMERA_WORK));
    camera->callback = callback;
    camera->center.x = 0.0f;
    camera->center.y = 65.0f;
    camera->center.z = -800.0f;
    camera->rot.x = -7.25f;
    camera->rot.y = 0.0f;
    camera->rot.z = 0.0f;
    if (lbl_1_bss_28 == 0) {
        camera->zoom = 2650.0f;
    } else {
        camera->zoom = 2150.0f;
    }
    camera->obj =
        omAddObjEx(lbl_1_bss_0, 0x7FDA, 0, 0, -1, fn_1_32D0);
}

inline void fn_1_33A0(MDCAMERA_CALLBACK callback);

void fn_1_353C(void)
{
    MDCAMERA_WORK *camera = &lbl_1_bss_A10;

    Hu3DCameraKill(1);
    if (camera->obj) {
        omDelObjEx(lbl_1_bss_0, camera->obj);
    }
    camera->obj = NULL;
}

void fn_1_3598(void)
{
    lbl_1_bss_A68[0] = Hu3DGLightCreate(
        0.0f, 1.0f, 1.0f, 0.0f, -1.0f, -1.0f, 255, 255, 255);
    Hu3DGLightInfinitytSet(lbl_1_bss_A68[0]);
    Hu3DGLightStaticSet(lbl_1_bss_A68[0], TRUE);
    lbl_1_bss_A68[1] = Hu3DGLightCreate(
        -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 255, 255, 255);
    Hu3DGLightInfinitytSet(lbl_1_bss_A68[1]);
    Hu3DGLightStaticSet(lbl_1_bss_A68[1], TRUE);
}

inline void fn_1_3598(void);

void fn_1_36C4(void)
{
    Hu3DGLightKill(lbl_1_bss_A68[0]);
    Hu3DGLightKill(lbl_1_bss_A68[1]);
}

void fn_1_3700(s16 winNo)
{
    if (winNo == 0) {
        HuWinDispOn(lbl_1_bss_A60[winNo]);
    } else {
        HuWinExOpen(lbl_1_bss_A60[winNo]);
    }
}

inline void fn_1_3700(s16 winNo);

void fn_1_3770(s16 winNo)
{
    if (winNo == 0) {
        HuWinDispOff(lbl_1_bss_A60[winNo]);
    } else {
        HuWinExClose(lbl_1_bss_A60[winNo]);
    }
}

inline void fn_1_3770(s16 winNo);

void fn_1_37E0(s16 winNo)
{
    HuWinMesWait(lbl_1_bss_A60[winNo]);
}

inline void fn_1_37E0(s16 winNo);

s16 fn_1_381C(s16 winNo, s16 mode)
{
    s16 choice = 0;

    if (mode == 1) {
        HuWinAttrSet(lbl_1_bss_A60[winNo], HUWIN_ATTR_NOCANCEL);
    } else {
        HuWinAttrReset(lbl_1_bss_A60[winNo], HUWIN_ATTR_NOCANCEL);
    }
    choice = HuWinChoiceGet(lbl_1_bss_A60[winNo], -1);
    if (mode == 2 && choice == -1) {
        choice = 1;
    }
    return choice;
}

inline s16 fn_1_381C(s16 winNo, s16 mode);

void fn_1_38F0(s16 winNo, s32 messNum, s16 speed)
{
    HuWinAttrSet(lbl_1_bss_A60[winNo], HUWIN_ATTR_ALIGN_CENTER);
    HuWinMesSet(lbl_1_bss_A60[winNo], messNum);
    HuWinMesSpeedSet(lbl_1_bss_A60[winNo], speed);
    if (lbl_1_data_BC8 != messNum) {
        lbl_1_data_BC8 = -1;
    }
}

inline void fn_1_38F0(s16 winNo, s32 messNum, s16 speed);

void fn_1_3A1C(void)
{
    s16 i;

    HuWinInit(1);
    lbl_1_bss_A60[0] =
        HuWinExCreateFrame(16.0f, 337.0f, 0x220, 0x2A, -1, 0);
    HuWinDispOff(lbl_1_bss_A60[0]);
    HuWinBGTPLvlSet(lbl_1_bss_A60[0], 0.0f);
    lbl_1_bss_A60[1] =
        HuWinExCreateFrame(16.0f, 372.0f, 0x220, 0x44, -1, 5);
    HuWinDispOff(lbl_1_bss_A60[1]);
    HuWinBGTPLvlSet(lbl_1_bss_A60[1], 0.9f);
    lbl_1_bss_A60[2] =
        HuWinExCreateFrame(16.0f, 372.0f, 0x220, 0x44, -1, 3);
    HuWinDispOff(lbl_1_bss_A60[2]);
    HuWinBGTPLvlSet(lbl_1_bss_A60[2], 0.9f);
    lbl_1_bss_A60[3] =
        HuWinExCreateFrame(16.0f, 372.0f, 0x220, 0x44, -1, 4);
    HuWinDispOff(lbl_1_bss_A60[3]);
    HuWinBGTPLvlSet(lbl_1_bss_A60[3], 0.9f);

    for (i = 0; i < 4; i++) {
        winData[lbl_1_bss_A60[i]].padMask = 1;
        HuWinCallbackSet(lbl_1_bss_A60[i], (HUWIN_CALLBACK)fn_1_0);
    }
}

inline void fn_1_3A1C(void);

void fn_1_3C44(void)
{
    s16 i;

    for (i = 0; i < 4; i++) {
        HuWinExKill(lbl_1_bss_A60[i]);
    }
    HuWinAllKill();
}

void fn_1_3CA0(s16 winNo)
{
    if (lbl_1_data_DA2[0] != -1 && lbl_1_data_DA2[0] != winNo) {
        fn_1_3770(lbl_1_data_DA2[0]);
    }
    if (lbl_1_data_DA2[0] == -1 || lbl_1_data_DA2[0] != winNo) {
        lbl_1_data_DA2[0] = winNo;
        lbl_1_data_DA8[0] = -1;
        lbl_1_data_DA8[1] = -1;
        fn_1_3700(lbl_1_data_DA2[0]);
    }
}

void fn_1_3E0C(void)
{
    if (lbl_1_data_DA2[0] != -1) {
        fn_1_3770(lbl_1_data_DA2[0]);
    }
    lbl_1_data_DA2[0] = -1;
    lbl_1_data_DA8[0] = -1;
    lbl_1_data_DA8[1] = -1;
}

void fn_1_3EC8(void)
{
    if (lbl_1_data_DA2[0] != -1) {
        fn_1_37E0(lbl_1_data_DA2[0]);
    }
}

s16 fn_1_3F28(s16 mode)
{
    if (lbl_1_data_DA2[0] != -1) {
        return fn_1_381C(lbl_1_data_DA2[0], mode);
    }
    return 0;
}

void fn_1_4020(s16 winNo, s32 messNum, s16 speed)
{
    fn_1_3CA0(winNo);
    if (lbl_1_data_DA8[0] != messNum) {
        lbl_1_data_DA8[0] = messNum;
        lbl_1_data_DA8[1] = -1;
        fn_1_38F0(lbl_1_data_DA2[0], lbl_1_data_DA8[0], speed);
    }
}

void fn_1_444C(s32 messNum)
{
    if (lbl_1_data_DA2[1] == -1) {
        lbl_1_data_DA2[1] = 0;
        lbl_1_data_DA8[2] = -1;
        fn_1_3700(lbl_1_data_DA2[1]);
    }
    if (lbl_1_data_DA8[2] != messNum) {
        lbl_1_data_DA8[2] = messNum;
        fn_1_38F0(lbl_1_data_DA2[1], lbl_1_data_DA8[2], 0);
    }
}

void fn_1_45D0(void)
{
    if (lbl_1_data_DA2[1] != -1) {
        fn_1_3770(lbl_1_data_DA2[1]);
    }
    lbl_1_data_DA2[1] = -1;
    lbl_1_data_DA8[2] = -1;
}

void fn_1_467C(void)
{
    HuVecF shadowPos = { 0.0f, 3000.0f, 600.0f };
    HuVecF shadowUp = { 0.0f, 1.0f, 0.0f };
    HuVecF shadowTarget = { 0.0f, 0.0f, 0.0f };

    Hu3DShadowCreate(30.0f, 10.0f, 10000.0f);
    Hu3DShadowPosSet(&shadowPos, &shadowUp, &shadowTarget);
}

inline void fn_1_467C(void);

void fn_1_4734(HUSPR_GROUPID groupId, s32 attr)
{
    s16 memberNo;
    HUSPR_GROUP *group = &HuSprGrpData[groupId];

    for (memberNo = 0; memberNo < group->sprNum; memberNo++) {
        HuSprAttrSet(groupId, memberNo, (u16)attr);
    }
}

inline void fn_1_4734(HUSPR_GROUPID groupId, s32 attr);

void fn_1_47B4(HUSPR_GROUPID groupId, s32 attr)
{
    s16 memberNo;
    HUSPR_GROUP *group = &HuSprGrpData[groupId];

    for (memberNo = 0; memberNo < group->sprNum; memberNo++) {
        HuSprAttrReset(groupId, memberNo, (u16)attr);
    }
}

inline void fn_1_47B4(HUSPR_GROUPID groupId, s32 attr);

void fn_1_4834(void)
{
    LBL_1_DATA_C0_ENTRY *desc;
    s16 i;

    for (i = 0; i < 32; i++) {
        lbl_1_bss_93C[i] = HuSprAnimRead(
            HuDataSelHeapReadNum(
                lbl_1_data_1C[i], HU_MEMNUM_OVL, HEAP_MODEL));
    }
    for (i = 0; i < 17; i++) {
        lbl_1_bss_918[i] = HuSprGrpCreate(lbl_1_data_9C[i]);
    }
    for (i = 0, desc = lbl_1_data_C0; i < 58; i++, desc++) {
        lbl_1_bss_8A4[i] = HuSprCreate(
            lbl_1_bss_93C[desc->animNo], desc->priority, desc->bank);
        HuSprGrpMemberSet(lbl_1_bss_918[desc->groupNo], desc->memberNo,
            lbl_1_bss_8A4[i]);
        HuSprPosSet(lbl_1_bss_918[desc->groupNo], desc->memberNo,
            desc->pos.x, desc->pos.y);
        HuSprScaleSet(lbl_1_bss_918[desc->groupNo], desc->memberNo,
            desc->scale.x, desc->scale.y);
        HuSprZRotSet(lbl_1_bss_918[desc->groupNo], desc->memberNo,
            desc->zRot);
    }
    for (i = 0; i < 17; i++) {
        fn_1_4734(lbl_1_bss_918[i], HUSPR_ATTR_DISPOFF);
    }
}

inline void fn_1_4834(void);

void fn_1_4A80(void)
{
}

void fn_1_7ED4(OMOBJ *obj)
{
    s16 i;

    omSetStatBit(obj, OM_STAT_MODELPAUSE);
    for (i = 0; i < 11; i++) {
        obj->mdlId[i] =
            Hu3DModelCreateData(DATANUM(DATA_mdparty, 0x5E) + i);
        obj->mtnId[2 * i] = Hu3DJointMotionData(
            obj->mdlId[i], DATANUM(DATA_mdparty, 0x69) + i);
        obj->mtnId[(2 * i) + 1] = Hu3DJointMotionData(
            obj->mdlId[i], DATANUM(DATA_mdparty, 0x74) + i);
        Hu3DModelAttrSet(obj->mdlId[i], HU3D_ATTR_DISPOFF);
        Hu3DModelLayerSet(obj->mdlId[i], 1);
        Hu3DMotionShiftSet(obj->mdlId[i], obj->mtnId[2 * i],
            0.0f, 0.0f, HU3D_MOTATTR_LOOP);
        Hu3DModelShadowSet(obj->mdlId[i]);
    }
    obj->objFunc = NULL;
}

void fn_1_8074(OMOBJ *obj)
{
    s16 i;

    if (obj) {
        for (i = 0; i < 11; i++) {
            Hu3DMotionKill(obj->mtnId[2 * i]);
            Hu3DMotionKill(obj->mtnId[(2 * i) + 1]);
            Hu3DModelKill(obj->mdlId[i]);
        }
        omDelObjEx(lbl_1_bss_0, obj);
    }
    obj = NULL;
}

inline void fn_1_8074(OMOBJ *obj);

void fn_1_8124(s16 arg0, s16 arg1)
{
    s16 digit = 0;

    if (arg0 <= arg1) {
        HuSprGrpPosSet(lbl_1_bss_918[12], 288.0f, 280.0f);
        fn_1_47B4(lbl_1_bss_918[12], HUSPR_ATTR_DISPOFF);

        digit = arg0 / 10;
        HuSprBankSet(lbl_1_bss_918[12], 0, digit);
        if (digit == 0) {
            HuSprAttrSet(lbl_1_bss_918[12], 0, HUSPR_ATTR_DISPOFF);
        }
        digit = arg0 % 10;
        HuSprBankSet(lbl_1_bss_918[12], 1, digit);

        digit = arg1 / 10;
        HuSprBankSet(lbl_1_bss_918[12], 2, digit);
        if (digit == 0) {
            HuSprAttrSet(lbl_1_bss_918[12], 2, HUSPR_ATTR_DISPOFF);
        }
        digit = arg1 % 10;
        HuSprBankSet(lbl_1_bss_918[12], 3, digit);
    }
}

void fn_1_8318(void)
{
    fn_1_4734(lbl_1_bss_918[12], HUSPR_ATTR_DISPOFF);
}

void fn_1_8398(OMOBJ *obj)
{
    float alpha = fn_1_1868(1.0f, 0.0f, obj->work[0], 10.0f);

    HuSprGrpTPLvlSet(lbl_1_bss_918[0], alpha);
    if (++obj->work[0] > 10) {
        fn_1_4734(lbl_1_bss_918[0], HUSPR_ATTR_DISPOFF);
        obj->objFunc = NULL;
    }
}

void fn_1_852C(void)
{
    if (lbl_1_bss_28 == 0) {
        HuSprGrpPosSet(lbl_1_bss_918[0], 288.0f, 120.0f);
        fn_1_47B4(lbl_1_bss_918[0], HUSPR_ATTR_DISPOFF);
    }
}

inline void fn_1_852C(void);

void fn_1_85E8(void)
{
    OMOBJ *obj = lbl_1_bss_4;

    obj->work[0] = 0;
    obj->objFunc = fn_1_8398;
}

void fn_1_861C(OMOBJ *obj)
{
    s16 i;

    omSetStatBit(obj, OM_STAT_MODELPAUSE);
    for (i = 0; i < 3; i++) {
        obj->mdlId[i] = Hu3DModelCreateData(DATA_mdparty + i);
        obj->mtnId[i] = Hu3DMotionIDGet(obj->mdlId[i]);
        Hu3DModelLayerSet(obj->mdlId[i], 1);
        Hu3DMotionShiftSet(obj->mdlId[i], obj->mtnId[i],
            0.0f, 0.0f, HU3D_MOTATTR_LOOP);
        Hu3DModelShadowMapSet(obj->mdlId[i]);
    }
    fn_1_852C();
    obj->objFunc = NULL;
}

void fn_1_87BC(OMOBJ *obj)
{
    s16 i;

    if (obj) {
        for (i = 0; i < 3; i++) {
            Hu3DMotionKill(obj->mtnId[i]);
            Hu3DModelKill(obj->mdlId[i]);
        }
        omDelObjEx(lbl_1_bss_0, obj);
    }
    obj = NULL;
}

inline void fn_1_87BC(OMOBJ *obj);

void fn_1_9A24(void)
{
    s16 i;

    for (i = 0; i < 2; i++) {
        HuWinExKill(lbl_1_bss_104[i]);
    }
}

void fn_1_A880(void)
{
    s16 i;

    for (i = 0; i < 2; i++) {
        HuWinExKill(lbl_1_bss_104[i]);
    }
}

void fn_1_D748(void)
{
    char *bitmapName[4] = {
        lbl_1_data_DF2,
        lbl_1_data_DFA,
        lbl_1_data_E02,
        lbl_1_data_E0A,
    };
    s16 i;
    s16 j;

    for (i = 0; i < 41; i++) {
        lbl_1_bss_800[i] = HuSprAnimRead(
            HuDataSelHeapReadNum(
                lbl_1_data_800[i], HU_MEMNUM_OVL, HEAP_MODEL));
    }
    for (i = 0; i < 25; i++) {
        memset(&lbl_1_bss_288[i], 0, sizeof(LBL_1_BSS_288_ENTRY));
        if (i == 0) {
            lbl_1_bss_288[i].modelId = Hu3DModelCreate(
                HuDataSelHeapReadNum(0x970024, HU_MEMNUM_OVL, HEAP_MODEL));
        } else {
            lbl_1_bss_288[i].modelId =
                Hu3DModelLink(lbl_1_bss_288[0].modelId);
        }
        for (j = 0; j < 4; j++) {
            lbl_1_bss_288[i].animId[j] = Hu3DAnimCreate(
                lbl_1_bss_800[0], lbl_1_bss_288[i].modelId,
                bitmapName[j]);
        }
        Hu3DModelLayerSet(lbl_1_bss_288[i].modelId, 1);
        Hu3DModelAttrSet(
            lbl_1_bss_288[i].modelId, HU3D_ATTR_DISPOFF);
    }
    lbl_1_bss_24 =
        omAddObjEx(lbl_1_bss_0, 0x1000, 0x10, 0x10, -1, NULL);
}

inline void fn_1_D748(void);

void fn_1_2D1D4(void)
{
    lbl_1_bss_0 = omInitObjMan(27, 0x2000);
    omGameSysInit(lbl_1_bss_0);
    fn_1_33A0(fn_1_2DFD4);
    fn_1_3598();
    fn_1_3A1C();
    fn_1_467C();
    fn_1_4834();
    fn_1_D748();
    fn_1_44DCC(lbl_1_bss_0);
    lbl_1_bss_4 =
        omAddObjEx(lbl_1_bss_0, 0x1000, 0x10, 0x10, -1, fn_1_861C);
    lbl_1_bss_8 =
        omAddObjEx(lbl_1_bss_0, 0x1000, 0x10, 0x10, -1, fn_1_9218);
    lbl_1_bss_C =
        omAddObjEx(lbl_1_bss_0, 0x1000, 0x10, 0x10, -1, fn_1_605C);
    lbl_1_bss_10 =
        omAddObjEx(lbl_1_bss_0, 0x1000, 0x10, 0x10, -1, fn_1_7064);
    lbl_1_bss_14 =
        omAddObjEx(lbl_1_bss_0, 0x1000, 0x10, 0x20, -1, fn_1_7ED4);
    lbl_1_bss_18 =
        omAddObjEx(lbl_1_bss_0, 0x1000, 0x10, 0x10, -1, fn_1_A5D8);
    lbl_1_bss_1C.obj =
        omAddObjEx(lbl_1_bss_0, 0x1000, 0x10, 0x10, -1, fn_1_B4C8);
    HuPrcChildCreate(fn_1_2CE00, 0x3000, 0x3000, 0, lbl_1_bss_0);
}

void fn_1_2DD38(void)
{
    HuDataDirCloseAll();
}

void ObjectSetup(void)
{
    OSReport(lbl_1_data_ED8);
    fn_1_2DD38();
    fn_1_1248();
    _ClearFlag(FLAG_BOARD_INIT);
    _ClearFlag(FLAGNUM(FLAG_GROUP_COMMON, 0x23));
    lbl_1_bss_2A = FALSE;
    lbl_1_bss_2C = FALSE;
    if (GWBankFlagGet(2)) {
        lbl_1_bss_2A = TRUE;
    }
    if (GWBankFlagGet(3)) {
        lbl_1_bss_2C = TRUE;
    }
    lbl_1_data_BEE[0] = TRUE;
    lbl_1_data_BEE[1] = TRUE;
    lbl_1_data_BEE[2] = TRUE;
    lbl_1_data_BEE[3] = TRUE;
    lbl_1_data_BEE[4] = TRUE;
    lbl_1_data_BEE[5] = FALSE;
    if (GWBankFlagGet(0x33)) {
        lbl_1_data_BEE[5] = TRUE;
    }
    lbl_1_bss_28 = omovlevtno;
    fn_1_2D1D4();
}

int _prolog(void)
{
    const VoidFunc *ctors = _ctors;

    while (*ctors) {
        (**ctors)();
        ctors++;
    }
    ObjectSetup();
    return 0;
}

void _epilog(void)
{
    const VoidFunc *dtors = _dtors;

    while (*dtors) {
        (**dtors)();
        dtors++;
    }
}

void fn_1_2DFD4(OMOBJ *obj, MDCAMERA_WORK *camera)
{
    camera->unk_10.x = 0.0f;
    camera->unk_10.y = 65.0f;
    camera->unk_10.z = -800.0f;
    camera->unk_28.x = -7.25f;
    camera->unk_28.y = 0.0f;
    camera->unk_28.z = 0.0f;
    camera->unk_38 = 2150.0f;
    fn_1_28B0(camera, 15.0f);
}

s32 lbl_1_data_BC8 = -1;
char lbl_1_data_BCC[] = "# ========== win callback :: %d\n";
s16 lbl_1_data_BEE[7] = { 0, 0, 0, 0, 0, 0, 0 };
char lbl_1_data_C66[] = "0x%x :: _effect\n";
char lbl_1_data_C77[] = "0x%x :: _gamemes\n";
char lbl_1_data_C89[] = "0x%x :: _mgconst\n";
char lbl_1_data_C9B[] = "0x%x :: _win\n";
char lbl_1_data_CA9[] = "\n";
char lbl_1_data_CDC[] = ">>>>>>>>>> mdpartydll :: objsetup!! <<<<<<<<<<\n";
char lbl_1_data_D0C[] = "\n>>>>>>>>>> mdpartydll :: ovlreturn!! <<<<<<<<<<\n";
s16 lbl_1_data_DA2[3] = { -1, -1, 0 };
s32 lbl_1_data_DA8[3] = { -1, -1, -1 };
char lbl_1_data_ED8[] = "\n-----===== MARIO PARTY 6 :: PARTY MODE =====-----\n\n";

OMOBJMAN *lbl_1_bss_0;
OMOBJ *lbl_1_bss_4;
OMOBJ *lbl_1_bss_8;
OMOBJ *lbl_1_bss_C;
OMOBJ *lbl_1_bss_10;
OMOBJ *lbl_1_bss_14;
OMOBJ *lbl_1_bss_18;
LBL_1_BSS_1C lbl_1_bss_1C;
OMOBJ *lbl_1_bss_24;
s16 lbl_1_bss_28;
s16 lbl_1_bss_2A;
s16 lbl_1_bss_2C;
HUWINID lbl_1_bss_104[2];
LBL_1_BSS_288_ENTRY lbl_1_bss_288[25];
ANIMDATA *lbl_1_bss_800[41];
HUSPRID lbl_1_bss_8A4[58];
HUSPR_GROUPID lbl_1_bss_918[18];
ANIMDATA *lbl_1_bss_93C[32];
MDCAMERA_WORK lbl_1_bss_A10;
HUWINID lbl_1_bss_A60[4];
HU3D_LIGHTID lbl_1_bss_A68[2];
