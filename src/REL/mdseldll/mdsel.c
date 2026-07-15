#include "datadir_enum.h"

#include "dolphin/mic.h"
#include "dolphin/os.h"

#include "game/armem.h"
#include "game/charman.h"
#include "game/flag.h"
#include "game/gamework.h"
#include "game/mgdata.h"
#include "game/object.h"
#include "game/sprite.h"
#include "game/wipe.h"
#include "game/window.h"
#include "msm_stream.h"

#define sind(x) sin((M_PI * (x)) / 180.0)

typedef void (*VoidFunc)(void);
typedef void (*MCResponseCallback)(u16 *response);

typedef struct Lbl1Bss1D4Entry {
    HU3D_MODELID modelId;
    s16 pad;
    s32 fxHandle;
} LBL_1_BSS_1D4_ENTRY;

typedef struct Lbl1Data8Entry {
    s16 groupNo;
    s16 memberNo;
    s16 animNo;
    s16 priority;
    s16 bank;
    s16 pad;
    HuVec2f pos;
    HuVec2f scale;
    float zRot;
} LBL_1_DATA_8_ENTRY;

typedef struct MdselBezierWork {
    u8 unk_00[4];
    float time;
    float duration;
    HuVecF control[3];
    u8 unk_30[0x58];
} MDSEL_BEZIER_WORK;

typedef struct Lbl1Bss8ACEntry {
    u8 unk_00[0x40];
    s16 unk_40;
    s16 unk_42;
    s16 unk_44;
    s16 unk_46;
    float unk_48;
    float unk_4C;
    u8 unk_50[0x38];
} LBL_1_BSS_8AC_ENTRY;

extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];

extern void HuDataDirCloseAll(void);
extern void HuAudFadeOut(s32 speed);
extern int HuAudFXPlay(int seId);
extern int HuAudFXPlayPan(int seId, int pan);
extern int HuAudFXPlayVolPan(int seId, int volume, int pan);
extern void HuAudFXStop(int seNo);
extern int HuAudSStreamPlay(s16 streamId);
extern void HuAudSStreamFadeOut(int streamNo, s32 speed);
extern s32 HuMCInit(s16 mountResult);
extern s32 HuMCMount(s32 chan);
extern void HuMCClose(void);
extern void HuMCListenerKill(void);
extern void HuMCContextKill(s16 context);
extern s16 HuMCContextCreate(char *path);
extern void HuMCListenerCreate(
    s16 context, MCResponseCallback callback, u8 property);

extern int lbl_1_bss_0;
extern int lbl_1_bss_4;
extern OMOBJMAN *lbl_1_bss_8;
extern OMOBJ *lbl_1_bss_C;
extern OMOBJ *lbl_1_bss_10;
extern OMOBJ *lbl_1_bss_14;
extern OMOBJ *lbl_1_bss_18;
extern OMOBJ *lbl_1_bss_1C;
extern OMOBJ *lbl_1_bss_20[2];
extern void *lbl_1_bss_28;
extern OMOBJ *lbl_1_bss_30;
extern OMOBJ *lbl_1_bss_34;
extern s16 lbl_1_bss_38;
extern float lbl_1_bss_40;
extern HU3D_MODELID lbl_1_bss_44[6][5];
extern ANIMDATA *lbl_1_bss_80[5];
extern HuVecF lbl_1_bss_94;
extern MDSEL_BEZIER_WORK lbl_1_bss_A0;
extern MDSEL_BEZIER_WORK lbl_1_bss_128;
extern LBL_1_BSS_1D4_ENTRY lbl_1_bss_1D4[15];
extern LBL_1_BSS_8AC_ENTRY lbl_1_bss_8AC[2];
extern LBL_1_BSS_8AC_ENTRY lbl_1_bss_9BC[30];
extern HuVecF lbl_1_bss_19AC[4];
extern s32 lbl_1_bss_19DC[4];
extern s32 lbl_1_bss_1A2C;
extern s16 lbl_1_bss_1A30[3];
extern HUSPRID lbl_1_bss_1A36[1];
extern HUSPR_GROUPID lbl_1_bss_1A38[1];
extern ANIMDATA *lbl_1_bss_1A3C[1];
extern HUWINID lbl_1_bss_1A40[4];
extern HU3D_LIGHTID lbl_1_bss_1A48[2];
extern s16 lbl_1_bss_1A4C;

extern u32 lbl_1_data_0[1];
extern s16 lbl_1_data_4[2];
extern LBL_1_DATA_8_ENTRY lbl_1_data_8[1];
extern HuVecF lbl_1_data_28[6];
extern s16 lbl_1_data_70[6];
extern s32 lbl_1_data_7C;
extern char lbl_1_data_80[];
extern char lbl_1_data_A1[];
extern char lbl_1_data_CF[];
extern char lbl_1_data_E0[];
extern char lbl_1_data_F2[];
extern char lbl_1_data_104[];
extern char lbl_1_data_112[];
extern char lbl_1_data_114[];
extern s16 lbl_1_data_150[2];
extern s32 lbl_1_data_154[2];
extern s16 lbl_1_data_15C;
extern char lbl_1_data_169[];
extern char lbl_1_data_1A8[];
extern char lbl_1_data_1E7[];
extern char lbl_1_data_223[];
extern char lbl_1_data_25E[];
extern char lbl_1_data_275[];
extern char lbl_1_data_27F[];
extern char lbl_1_data_2BD[];
extern s16 lbl_1_data_2D0;
extern char lbl_1_data_2D2[];
extern char lbl_1_data_308[];
extern char lbl_1_data_333[];
extern u32 lbl_1_data_35C[5];

void fn_1_A5D4(void);
void fn_1_0(HUWINID winId, u32 mess, s16 index);
void fn_1_1370(OMOBJ *obj);
void fn_1_5614(OMOBJ *obj);
void fn_1_607C(OMOBJ *obj);
void fn_1_651C(u16 *response);
void fn_1_EF48(HU3D_MODEL *model, HU3D_PARTICLE *particle, Mtx matrix);
void fn_1_F790(void);
void fn_1_2A78(s16 layerNo);
void fn_1_2828(void);
void fn_1_70BC(OMOBJ *obj);
void fn_1_7868(OMOBJ *obj);
void fn_1_809C(OMOBJ *obj);
void fn_1_92BC(OMOBJ *obj);
void fn_1_FEC0(HU3D_MODELID modelId, s32 fxNo, s16 volume, s16 pan);
void fn_1_9910(void);
s16 fn_1_BAB4(void);
void fn_1_E1FC(void);
s16 fn_1_E7B0(void);
void ObjectSetup(void);

void fn_1_0(HUWINID winId, u32 mess, s16 index)
{
    s32 messNum[3] = { 0xA0000, 0xA0001, -1 };
    s32 fxNum[16] = {
        0x3B5, 0x3B6, 0x3B7, 0x3B8, 0x3B9, 0x3BA, 0x3BB, -1,
        0x3AD, 0x3AE, 0x3AF, 0x3B0, 0x3B1, 0x3B2, 0x3B3, -1,
    };
    s16 i;

    index--;
    OSReport(lbl_1_data_80, index);
    if (lbl_1_data_7C != mess) {
        lbl_1_data_7C = mess;
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

void fn_1_40C(HUSPR_GROUPID groupId, s32 attr)
{
    s16 memberNo;
    HUSPR_GROUP *group = &HuSprGrpData[groupId];

    for (memberNo = 0; memberNo < group->sprNum; memberNo++) {
        HuSprAttrSet(groupId, memberNo, (u16)attr);
    }
}

inline void fn_1_40C(HUSPR_GROUPID groupId, s32 attr);

void fn_1_48C(HUSPR_GROUPID groupId, s32 attr)
{
    s16 memberNo;
    HUSPR_GROUP *group = &HuSprGrpData[groupId];

    for (memberNo = 0; memberNo < group->sprNum; memberNo++) {
        HuSprAttrReset(groupId, memberNo, (u16)attr);
    }
}

inline void fn_1_48C(HUSPR_GROUPID groupId, s32 attr);

float fn_1_50C(float arg0, float arg1, float arg2, float arg3)
{
    if (arg2 <= 0.0f) {
        return arg0;
    }
    if (arg2 >= arg3) {
        return arg1;
    }
    return arg0 + ((arg2 / arg3) * (arg1 - arg0));
}

float fn_1_550(float arg0, float arg1, float arg2)
{
    if (arg0 == arg1 || arg2 <= 1.0f) {
        return arg1;
    }
    return (arg1 + (arg0 * (arg2 - 1.0f))) / arg2;
}

void fn_1_598(HuVecF *dst, const HuVecF *src, float weight)
{
    dst->x = fn_1_550(dst->x, src->x, weight);
    dst->y = fn_1_550(dst->y, src->y, weight);
    dst->z = fn_1_550(dst->z, src->z, weight);
}

float fn_1_724(float arg0, float arg1, float time, float duration)
{
    if (time <= 0.0f) {
        return arg0;
    }
    if (time >= duration) {
        return arg1;
    }
    return arg0 + ((arg1 - arg0) * sind((90.0f / duration) * time));
}

float fn_1_80C(float arg0, float arg1, float time, float duration)
{
    if (time <= 0.0f) {
        return arg0;
    }
    if (time >= duration) {
        return arg0;
    }
    return arg0 + ((arg1 - arg0) * sind((360.0f / duration) * time));
}

inline float fn_1_80C(float arg0, float arg1, float time, float duration);

float fn_1_8E8(float arg0, float arg1, float time, float duration)
{
    if (time <= 0.0f) {
        return arg0;
    }
    if (time >= duration) {
        return arg0;
    }
    return arg0 + ((arg1 - arg0) * sind((180.0f / duration) * time));
}

inline float fn_1_8E8(float arg0, float arg1, float time, float duration);

float fn_1_9C4(float arg0, float arg1, float arg2, float arg3)
{
    float temp = 1.0f - arg3;

    return (arg2 * (arg3 * arg3))
        + ((arg0 * (temp * temp)) + ((arg1 * (temp * arg3)) * 2.0f));
}

void fn_1_A20(
    HuVecF *dst, const HuVecF *a, const HuVecF *b, const HuVecF *c, float t)
{
    dst->x = fn_1_9C4(a->x, b->x, c->x, t);
    dst->y = fn_1_9C4(a->y, b->y, c->y, t);
    dst->z = fn_1_9C4(a->z, b->z, c->z, t);
}

float fn_1_C28(float arg0, float arg1, float arg2)
{
    return (arg1 + (arg0 * (arg2 - 1.0f))) / arg2;
}

float fn_1_C48(float arg0, float arg1, float arg2, float arg3)
{
    if (arg2 <= 0.0f) {
        return arg0;
    }
    if (arg2 >= arg3) {
        return arg1;
    }
    return arg0 + ((arg2 / arg3) * (arg1 - arg0));
}

void fn_1_C8C(
    HU3D_MODELID modelId, HuVecF *start, HuVecF *end, float time,
    float duration)
{
    HuVecF modelPos;
    HuVecF modelRot;
    HuVecF pos;
    HuVecF rot;

    Hu3DModelPosGet(modelId, &modelPos);
    Hu3DModelRotGet(modelId, &modelRot);
    pos.x = fn_1_C48(start->x, end->x, time, duration);
    pos.y = fn_1_C48(start->y, end->y, time, duration);
    pos.z = fn_1_C48(start->z, end->z, time, duration);
    modelPos.x -= pos.x;
    modelPos.z -= pos.z;
    rot.y = -(180.0 * (atan2(modelPos.x, -modelPos.z) / M_PI));
    if (modelRot.y - rot.y > 180.0f) {
        modelRot.y -= 360.0f;
    } else if (modelRot.y - rot.y < -180.0f) {
        modelRot.y += 360.0f;
    }
    rot.x = modelRot.x;
    rot.y = fn_1_C28(modelRot.y, rot.y, 10.0f);
    rot.z = modelRot.z;
    Hu3DModelPosSetV(modelId, &pos);
    Hu3DModelRotSetV(modelId, &rot);
}

void fn_1_FEC(s16 arg0)
{
    float divisor = 4.0f;

    if (arg0 == -1) {
        lbl_1_bss_19AC[1].x = 0.0f;
        lbl_1_bss_19AC[1].y = 1860.0f;
        lbl_1_bss_19AC[1].z = 4180.0f;
        lbl_1_bss_19AC[3].x = 0.0f;
        lbl_1_bss_19AC[3].y = 317.0f;
        lbl_1_bss_19AC[3].z = 100.0f;
    } else {
        lbl_1_bss_19AC[1].x = lbl_1_data_28[arg0].x / divisor;
        lbl_1_bss_19AC[1].y = 1860.0f + lbl_1_data_28[arg0].y / divisor;
        lbl_1_bss_19AC[1].z = 4080.0f + lbl_1_data_28[arg0].z / divisor;
        lbl_1_bss_19AC[3].x = lbl_1_data_28[arg0].x / divisor;
        lbl_1_bss_19AC[3].y = 317.0f + lbl_1_data_28[arg0].y / divisor;
        lbl_1_bss_19AC[3].z = lbl_1_data_28[arg0].z / divisor;
    }
}

void fn_1_1B4(void)
{
    OMOVLHIS *history = omOvlHisGet(0);

    omOvlHisChg(0, history->ovl, 1, lbl_1_bss_1A30[0]);
    switch (lbl_1_bss_1A30[0]) {
        case 0:
            omOvlCall(DLL_mdpartydll, 0, 0);
            break;
        case 1:
            omOvlCall(DLL_mdsingdll, 0, 0);
            break;
        case 2:
            omOvlCall(DLL_mdminidll, 0, 0);
            break;
        case 3:
            omOvlCall(DLL_mdmicdll, 0, 0);
            break;
        case 4:
            omOvlCall(DLL_optiondll, 0, 0);
            break;
        case 5:
            omOvlCall(DLL_mdbankdll, 0, 0);
            break;
    }
}

void fn_1_1734(void)
{
    Hu3DCameraCreate(1);
    Hu3DCameraPerspectiveSet(1, 30.0f, 10.0f, 10000.0f, 1.2f);
    Hu3DCameraViewportSet(1, 0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
    Hu3DCameraPosSet(1, 0.0f, 1860.0f, 4080.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        317.0f, 0.0f);

    lbl_1_bss_19AC[0].x = lbl_1_bss_19AC[1].x = 0.0f;
    lbl_1_bss_19AC[0].y = lbl_1_bss_19AC[1].y = 1860.0f;
    lbl_1_bss_19AC[0].z = lbl_1_bss_19AC[1].z = 4480.0f;
    lbl_1_bss_19AC[2].x = lbl_1_bss_19AC[3].x = 0.0f;
    lbl_1_bss_19AC[2].y = lbl_1_bss_19AC[3].y = 317.0f;
    lbl_1_bss_19AC[2].z = lbl_1_bss_19AC[3].z = 0.0f;

    lbl_1_bss_20[0] =
        omAddObjEx(lbl_1_bss_8, 0x1000, 0, 0, -1, fn_1_1370);
}

inline void fn_1_1734(void);

void fn_1_1964(void)
{
    Hu3DCameraKill(1);
}

void fn_1_1988(void)
{
    HuVecF pos[2] = { { 0.0f, 1.0f, 1.0f }, { -1.0f, 1.0f, -1.0f } };
    HuVecF dir[2] = { { 0.0f, -1.0f, -1.0f }, { 1.0f, -1.0f, -1.0f } };
    GXColor color = { 255, 255, 255, 255 };
    s16 i;

    for (i = 0; i < 2; i++) {
        lbl_1_bss_1A48[i] = Hu3DGLightCreateV(&pos[i], &dir[i], &color);
        Hu3DGLightInfinitytSet(lbl_1_bss_1A48[i]);
        Hu3DGLightStaticSet(lbl_1_bss_1A48[i], TRUE);
    }
}

inline void fn_1_1988(void);

void fn_1_1AD8(void)
{
    s16 i;

    for (i = 0; i < 2; i++) {
        Hu3DGLightKill(lbl_1_bss_1A48[i]);
    }
}

void fn_1_1B30(s16 winNo)
{
    if (winNo == 0) {
        HuWinDispOn(lbl_1_bss_1A40[winNo]);
    } else {
        HuWinExOpen(lbl_1_bss_1A40[winNo]);
    }
}

inline void fn_1_1B30(s16 winNo);

void fn_1_1BA0(s16 winNo)
{
    if (winNo == 0) {
        HuWinDispOff(lbl_1_bss_1A40[winNo]);
    } else {
        HuWinExClose(lbl_1_bss_1A40[winNo]);
    }
}

inline void fn_1_1BA0(s16 winNo);

void fn_1_1C10(s16 winNo)
{
    HuWinMesWait(lbl_1_bss_1A40[winNo]);
}

inline void fn_1_1C10(s16 winNo);

s16 fn_1_1C4C(s16 winNo, s16 mode)
{
    s16 choice = 0;

    if (mode == 1) {
        HuWinAttrSet(lbl_1_bss_1A40[winNo], HUWIN_ATTR_NOCANCEL);
    } else {
        HuWinAttrReset(lbl_1_bss_1A40[winNo], HUWIN_ATTR_NOCANCEL);
    }
    choice = HuWinChoiceGet(lbl_1_bss_1A40[winNo], -1);
    if (mode == 2 && choice == -1) {
        choice = 1;
    }
    return choice;
}

inline s16 fn_1_1C4C(s16 winNo, s16 mode);

void fn_1_1D20(s16 winNo, s32 messNum, s16 speed)
{
    HuWinAttrSet(lbl_1_bss_1A40[winNo], HUWIN_ATTR_ALIGN_CENTER);
    HuWinMesSet(lbl_1_bss_1A40[winNo], messNum);
    HuWinMesSpeedSet(lbl_1_bss_1A40[winNo], speed);
    if (lbl_1_data_7C != messNum) {
        lbl_1_data_7C = -1;
    }
}

inline void fn_1_1D20(s16 winNo, s32 messNum, s16 speed);

void fn_1_1DDC(void)
{
    s16 i;

    HuWinInit(1);
    lbl_1_bss_1A40[0] = HuWinExCreateFrame(16.0f, 337.0f, 0x220, 0x2A, -1, 0);
    HuWinDispOff(lbl_1_bss_1A40[0]);
    HuWinBGTPLvlSet(lbl_1_bss_1A40[0], 0.0f);
    lbl_1_bss_1A40[1] = HuWinExCreateFrame(16.0f, 372.0f, 0x220, 0x44, -1, 0);
    HuWinDispOff(lbl_1_bss_1A40[1]);
    HuWinBGTPLvlSet(lbl_1_bss_1A40[1], 0.9f);
    lbl_1_bss_1A40[2] = HuWinExCreateFrame(16.0f, 372.0f, 0x220, 0x44, -1, 3);
    HuWinDispOff(lbl_1_bss_1A40[2]);
    HuWinBGTPLvlSet(lbl_1_bss_1A40[2], 0.9f);
    lbl_1_bss_1A40[3] = HuWinExCreateFrame(16.0f, 372.0f, 0x220, 0x44, -1, 4);
    HuWinDispOff(lbl_1_bss_1A40[3]);
    HuWinBGTPLvlSet(lbl_1_bss_1A40[3], 0.9f);

    for (i = 0; i < 4; i++) {
        winData[lbl_1_bss_1A40[i]].padMask = 1;
        HuWinCallbackSet(lbl_1_bss_1A40[i], (HUWIN_CALLBACK)fn_1_0);
        HuWinAttrSet(lbl_1_bss_1A40[i], HUWIN_ATTR_UPAUSE);
    }
}

inline void fn_1_1DDC(void);

void fn_1_2024(void)
{
    s16 i;

    for (i = 0; i < 4; i++) {
        HuWinExKill(lbl_1_bss_1A40[i]);
    }
    HuWinAllKill();
}

void fn_1_2080(s16 winNo)
{
    if (lbl_1_data_150[0] != -1 && lbl_1_data_150[0] != winNo) {
        fn_1_1BA0(lbl_1_data_150[0]);
    }
    if (lbl_1_data_150[0] == -1 || lbl_1_data_150[0] != winNo) {
        lbl_1_data_150[0] = winNo;
        lbl_1_data_154[0] = -1;
        fn_1_1B30(lbl_1_data_150[0]);
    }
}

void fn_1_21DC(void)
{
    if (lbl_1_data_150[0] != -1) {
        fn_1_1BA0(lbl_1_data_150[0]);
    }
    lbl_1_data_150[0] = -1;
    lbl_1_data_154[0] = -1;
}

void fn_1_2288(void)
{
    if (lbl_1_data_150[0] != -1) {
        fn_1_1C10(lbl_1_data_150[0]);
    }
}

s16 fn_1_22E8(s16 mode)
{
    if (lbl_1_data_150[0] != -1) {
        return fn_1_1C4C(lbl_1_data_150[0], mode);
    }
    return 0;
}

void fn_1_23E0(s16 winNo, s32 messNum, s16 speed)
{
    fn_1_2080(winNo);
    if (lbl_1_data_154[0] != messNum) {
        lbl_1_data_154[0] = messNum;
        fn_1_1D20(lbl_1_data_150[0], lbl_1_data_154[0], speed);
    }
}

void fn_1_25F8(s32 messNum)
{
    if (lbl_1_data_150[1] == -1) {
        lbl_1_data_150[1] = 0;
        lbl_1_data_154[1] = -1;
        fn_1_1B30(lbl_1_data_150[1]);
    }
    if (lbl_1_data_154[1] != messNum) {
        lbl_1_data_154[1] = messNum;
        fn_1_1D20(lbl_1_data_150[1], lbl_1_data_154[1], 0);
    }
}

void fn_1_277C(void)
{
    if (lbl_1_data_150[1] != -1) {
        fn_1_1BA0(lbl_1_data_150[1]);
    }
    lbl_1_data_150[1] = -1;
    lbl_1_data_154[1] = -1;
}

void fn_1_2828(void)
{
    LBL_1_DATA_8_ENTRY *desc = lbl_1_data_8;
    s16 i;

    for (i = 0; i < 1; i++) {
        lbl_1_bss_1A3C[i] = HuSprAnimRead(
            HuDataSelHeapReadNum(lbl_1_data_0[i], HU_MEMNUM_OVL, HEAP_MODEL));
    }
    for (i = 0; i < 1; i++) {
        lbl_1_bss_1A38[i] = HuSprGrpCreate(lbl_1_data_4[i]);
    }
    for (i = 0; i < 1; i++, desc++) {
        lbl_1_bss_1A36[i] =
            HuSprCreate(lbl_1_bss_1A3C[desc->animNo], desc->priority, desc->bank);
        HuSprGrpMemberSet(
            lbl_1_bss_1A38[desc->groupNo], desc->memberNo, lbl_1_bss_1A36[i]);
        HuSprPosSet(lbl_1_bss_1A38[desc->groupNo], desc->memberNo, desc->pos.x,
            desc->pos.y);
        HuSprScaleSet(lbl_1_bss_1A38[desc->groupNo], desc->memberNo, desc->scale.x,
            desc->scale.y);
        HuSprZRotSet(lbl_1_bss_1A38[desc->groupNo], desc->memberNo, desc->zRot);
    }
    for (i = 0; i < 1; i++) {
        fn_1_40C(lbl_1_bss_1A38[i], HUSPR_ATTR_DISPOFF);
    }
}

inline void fn_1_2828(void);

void fn_1_2A74(void)
{
}

void fn_1_30FC(void)
{
    if (lbl_1_bss_28) {
        Hu3DLayerHookSet(15, fn_1_2A78);
    }
}

void fn_1_313C(void)
{
    lbl_1_bss_28 = HuMemDirectMallocNum(
        HEAP_MODEL, GXGetTexBufferSize(640, 480, GX_TF_RGBA8, FALSE, 0), HU_MEMNUM_OVL);
}

void fn_1_318C(void)
{
    Hu3DLayerHookReset(15);
    if (lbl_1_bss_28) {
        HuMemDirectFree(lbl_1_bss_28);
    }
    lbl_1_bss_28 = NULL;
}

void fn_1_31E4(OMOBJ *obj)
{
    if (++obj->work[0] > 120) {
        Hu3DMotionShiftSet(lbl_1_bss_14->mdlId[0], lbl_1_bss_14->mtnId[0], 0.0f, 15.0f,
            HU3D_MOTATTR_LOOP);
        obj->objFunc = NULL;
    }
}

void fn_1_3274(void)
{
    OMOBJ *obj = lbl_1_bss_30;

    fn_1_FEC0(lbl_1_bss_14->mdlId[0], 0x3B6, 0x10, -1);
    Hu3DMotionShiftSet(lbl_1_bss_14->mdlId[0], lbl_1_bss_14->mtnId[4], 0.0f, 15.0f,
        HU3D_MOTATTR_LOOP);
    obj->work[0] = 0;
    obj->objFunc = fn_1_31E4;
}

void fn_1_3328(OMOBJ *obj)
{
    if (++obj->work[0] > 120) {
        Hu3DMotionShiftSet(lbl_1_bss_18->mdlId[0], lbl_1_bss_18->mtnId[0], 0.0f, 15.0f,
            HU3D_MOTATTR_LOOP);
        obj->objFunc = NULL;
    }
}

void fn_1_33B8(void)
{
    OMOBJ *obj = lbl_1_bss_30;

    fn_1_FEC0(lbl_1_bss_18->mdlId[0], 0x3AE, 0x10, -1);
    Hu3DMotionShiftSet(lbl_1_bss_18->mdlId[0], lbl_1_bss_18->mtnId[4], 0.0f, 15.0f,
        HU3D_MOTATTR_LOOP);
    obj->work[0] = 0;
    obj->objFunc = fn_1_3328;
}

void fn_1_46DC(void)
{
    OMOBJ *obj = lbl_1_bss_30;
    LBL_1_BSS_8AC_ENTRY *entry;
    s16 i;

    for (i = 0; i < 2; i++) {
        entry = &lbl_1_bss_8AC[i];
        entry->unk_40 = 400;
        entry->unk_48 = 0.0f;
        entry->unk_4C = 20.0f;
        entry->unk_46++;
        if (entry->unk_46 > 30) {
            entry->unk_46 = 30;
        }
        if (entry->unk_46 > 10) {
            fn_1_FEC0(obj->mdlId[i + 30], 0x3D7, 16, 100);
        } else if (entry->unk_46 > 5) {
            fn_1_FEC0(obj->mdlId[i + 30], 0x3D6, 16, 100);
        } else {
            fn_1_FEC0(obj->mdlId[i + 30], 0x3D5, 16, 100);
        }
        Hu3DMotionShiftSet(
            obj->mdlId[i + 30], obj->mtnId[32], 0.0f, 10.0f,
            HU3D_MOTATTR_LOOP);
    }
}

inline void fn_1_46DC(void);

void fn_1_5BEC(void)
{
}

void fn_1_5EA4(s16 index)
{
    if (lbl_1_bss_1D4[index].modelId != HU3D_MODELID_NONE) {
        if (lbl_1_bss_1D4[index].fxHandle > 0) {
            HuAudFXStop(lbl_1_bss_1D4[index].fxHandle);
        }
        lbl_1_bss_1D4[index].modelId = HU3D_MODELID_NONE;
        lbl_1_bss_1D4[index].fxHandle = -1;
    }
}

s16 fn_1_5F60(HU3D_MODELID modelId, s32 fxNo)
{
    s16 i;

    for (i = 0; i < 15; i++) {
        if (lbl_1_bss_1D4[i].modelId == HU3D_MODELID_NONE) {
            break;
        }
    }
    if (i == 15) {
        return -1;
    }
    lbl_1_bss_1D4[i].modelId = modelId;
    lbl_1_bss_1D4[i].fxHandle = HuAudFXPlay(fxNo);
    return i;
}

void fn_1_6018(void)
{
    s16 i;

    for (i = 0; i < 15; i++) {
        lbl_1_bss_1D4[i].modelId = HU3D_MODELID_NONE;
        lbl_1_bss_1D4[i].fxHandle = -1;
    }
}

inline void fn_1_6018(void);

void fn_1_6C04(void)
{
    s16 local = 0;

    lbl_1_bss_38 = 0;
    if (GwCommon.mic == 1) {
        HuMCInit(0);
        if (HuMCMount(1) != MIC_RESULT_READY) {
            OSReport(lbl_1_data_169);
            HuMCClose();
        } else {
            OSReport(lbl_1_data_1A8);
            if (GwCommon.mic != 1) {
                OSReport(lbl_1_data_1E7);
                HuMCClose();
            } else {
                OSReport(lbl_1_data_223);
                lbl_1_bss_1A4C = HuMCContextCreate(lbl_1_data_25E);
                HuMCListenerCreate(lbl_1_bss_1A4C, fn_1_651C, 1);
                lbl_1_bss_38 = 1;
                OSReport(lbl_1_data_275);
            }
        }
    }

    lbl_1_data_15C = 1;
    fn_1_6018();
    lbl_1_bss_30 = omAddObj(lbl_1_bss_8, 0x1000, 0x40, 0x40, fn_1_5614);
    lbl_1_bss_34 = omAddObj(lbl_1_bss_8, 0x1000, 0, 0, fn_1_607C);
}

inline void fn_1_6C04(void);

void fn_1_6DFC(void)
{
    lbl_1_data_15C = 0;
    if (lbl_1_bss_38 == 1) {
        HuMCListenerKill();
        HuMCContextKill(lbl_1_bss_1A4C);
        HuMCClose();
    }
}

BOOL fn_1_6E54(void)
{
    BOOL result = TRUE;

    if (GwCommon.mic != 1) {
        return FALSE;
    }
    if (lbl_1_bss_38 == 0) {
        HuMCInit(0);
        if (HuMCMount(1) != MIC_RESULT_READY) {
            OSReport(lbl_1_data_169);
            result = FALSE;
        } else {
            OSReport(lbl_1_data_27F);
            result = TRUE;
        }
        HuMCClose();
    } else {
        if (HuMCMount(1) != MIC_RESULT_READY) {
            OSReport(lbl_1_data_169);
            result = FALSE;
        } else {
            OSReport(lbl_1_data_27F);
            result = TRUE;
        }
    }
    return result;
}

void fn_1_6F40(OMOBJ *obj)
{
    s16 i;

    omSetStatBit(obj, OM_STAT_MODELPAUSE);
    for (i = 0; i < 3; i++) {
        obj->mdlId[i] = Hu3DModelCreateData(DATANUM(DATA_mdsel, 0) + i);
        obj->mtnId[i] = Hu3DMotionIDGet(obj->mdlId[i]);
        Hu3DMotionShiftSet(
            obj->mdlId[i], obj->mtnId[i], 0.0f, 0.0f, HU3D_MOTATTR_LOOP);
    }
    obj->objFunc = NULL;
}

void fn_1_702C(OMOBJ *obj)
{
    s16 i;

    if (obj) {
        for (i = 0; i < 3; i++) {
            Hu3DMotionKill(obj->mtnId[i]);
            Hu3DModelKill(obj->mdlId[i]);
        }
        omDelObjEx(lbl_1_bss_8, obj);
    }
    obj = NULL;
}

void fn_1_75A4(s16 arg0)
{
    OMOBJ *obj = lbl_1_bss_10;
    s16 i;

    if (arg0 == -1) {
        obj->work[0] = 99;
        for (i = 0; i < 6; i++) {
            Hu3DModelAttrReset(obj->mdlId[i], HU3D_MOTATTR_LOOP);
        }
    } else if (obj->work[0] != lbl_1_bss_1A30[0]) {
        obj->work[0] = lbl_1_bss_1A30[0];
        for (i = 0; i < 6; i++) {
            if (i == lbl_1_bss_1A30[0]) {
                Hu3DModelAttrSet(obj->mdlId[i], HU3D_MOTATTR_LOOP);
                Hu3DModelAttrReset(obj->mdlId[i], HU3D_MOTATTR_PAUSE);
            } else {
                Hu3DModelAttrReset(obj->mdlId[i], HU3D_MOTATTR_LOOP);
            }
        }
    }
}

inline void fn_1_75A4(s16 arg0);

void fn_1_76DC(OMOBJ *obj)
{
    s16 i;

    omSetStatBit(obj, OM_STAT_MODELPAUSE);
    for (i = 0; i < 6; i++) {
        obj->mdlId[i] = Hu3DModelCreateData(DATANUM(DATA_mdsel, 3) + i);
        obj->mtnId[i] = Hu3DMotionIDGet(obj->mdlId[i]);
        Hu3DMotionShiftSet(
            obj->mdlId[i], obj->mtnId[i], 0.0f, 0.0f, HU3D_MOTATTR_PAUSE);
    }
    obj->work[0] = 99;
    obj->objFunc = fn_1_70BC;
}

void fn_1_77D8(OMOBJ *obj)
{
    s16 i;

    if (obj) {
        for (i = 0; i < 6; i++) {
            Hu3DMotionKill(obj->mtnId[i]);
            Hu3DModelKill(obj->mdlId[i]);
        }
        omDelObjEx(lbl_1_bss_8, obj);
    }
    obj = NULL;
}

void fn_1_7EC4(void)
{
    OMOBJ *obj = lbl_1_bss_14;
    MDSEL_BEZIER_WORK *work = &lbl_1_bss_128;
    s16 index;
    float y;

    if (lbl_1_bss_1A30[1] != 2) {
        y = 200.0f;
        if (lbl_1_bss_1A30[0] == 5) {
            y = 400.0f;
        }
        index = lbl_1_bss_1A30[1] + (lbl_1_bss_1A30[2] * 3);
        work->control[0].x = -1150.0f;
        work->control[0].y = 350.0f;
        work->control[0].z = -1000.0f;
        work->control[1].x = -lbl_1_data_28[index].x;
        work->control[1].y = 2250.0f;
        work->control[1].z = 6000.0f;
        work->control[2].x = lbl_1_data_28[index].x;
        work->control[2].y = y;
        work->control[2].z = lbl_1_data_28[index].z;
        work->time = 0.0f;
        work->duration = 90.0f;
        Hu3DMotionShiftSet(obj->mdlId[0], obj->mtnId[1], 0.0f, 5.0f, 0);
        lbl_1_bss_19DC[0] = HuAudFXPlay(0x47E);
        obj->objFunc = fn_1_7868;
    }
}

inline void fn_1_7EC4(void);

void fn_1_86F8(void)
{
    OMOBJ *obj = lbl_1_bss_18;
    MDSEL_BEZIER_WORK *work = &lbl_1_bss_A0;
    float value;
    s16 index;

    if (lbl_1_bss_1A30[1] != 0) {
        value = 100.0f;
        if (lbl_1_bss_1A30[0] == 5) {
            value = 400.0f;
        }
        index = lbl_1_bss_1A30[1] + (3 * lbl_1_bss_1A30[2]);
        work->control[0].x = 1150.0f;
        work->control[0].y = 350.0f;
        work->control[0].z = -1000.0f;
        work->control[1].x = -lbl_1_data_28[index].x;
        work->control[1].y = 2250.0f;
        work->control[1].z = 6000.0f;
        work->control[2].x = lbl_1_data_28[index].x;
        work->control[2].y = value;
        work->control[2].z = lbl_1_data_28[index].z;
        work->time = 0.0f;
        work->duration = 90.0f;
        Hu3DMotionShiftSet(
            obj->mdlId[0], obj->mtnId[1], 0.0f, 5.0f, HU3D_MOTATTR_NONE);
        lbl_1_bss_19DC[1] = HuAudFXPlay(0x47C);
        obj->objFunc = fn_1_809C;
    }
}

inline void fn_1_86F8(void);

void fn_1_88D0(OMOBJ *obj)
{
    s16 i;

    omSetStatBit(obj, OM_STAT_MODELPAUSE);
    obj->mdlId[0] = Hu3DModelCreateData(DATANUM(DATA_mdsel, 0x13));
    for (i = 0; i < 5; i++) {
        obj->mtnId[i] =
            Hu3DJointMotionData(obj->mdlId[0], DATANUM(DATA_mdsel, 0x14) + i);
    }
    Hu3DModelLayerSet(obj->mdlId[0], 1);
    Hu3DMotionShiftSet(obj->mdlId[0], obj->mtnId[0], 0.0f, 0.0f, HU3D_MOTATTR_LOOP);
    Hu3DModelPosSet(obj->mdlId[0], -1150.0f, 350.0f, -1000.0f);
    Hu3DModelRotSet(obj->mdlId[0], 0.0f, 15.0f, 0.0f);
    Hu3DModelScaleSet(obj->mdlId[0], 5.0f, 5.0f, 5.0f);
    obj->objFunc = NULL;
}

void fn_1_8A58(OMOBJ *obj)
{
    s16 i;

    if (obj) {
        for (i = 0; i < 2; i++) {
            Hu3DMotionKill(obj->mtnId[i]);
        }
        Hu3DModelKill(obj->mdlId[0]);
        omDelObjEx(lbl_1_bss_8, obj);
    }
    obj = NULL;
}

void fn_1_8AE0(OMOBJ *obj)
{
    s16 i;

    omSetStatBit(obj, OM_STAT_MODELPAUSE);
    obj->mdlId[0] = Hu3DModelCreateData(DATANUM(DATA_mdsel, 0x19));
    obj->mdlId[1] = Hu3DModelCreateData(DATANUM(DATA_mdsel, 0x1A));
    for (i = 0; i < 5; i++) {
        obj->mtnId[i] =
            Hu3DJointMotionData(obj->mdlId[0], DATANUM(DATA_mdsel, 0x1B) + i);
    }
    Hu3DModelHookSet(obj->mdlId[0], lbl_1_data_2BD, obj->mdlId[1]);
    Hu3DModelLayerSet(obj->mdlId[0], 1);
    Hu3DMotionShiftSet(obj->mdlId[0], obj->mtnId[0], 0.0f, 0.0f, HU3D_MOTATTR_LOOP);
    Hu3DModelPosSet(obj->mdlId[0], 1150.0f, 350.0f, -1000.0f);
    Hu3DModelRotSet(obj->mdlId[0], 0.0f, -15.0f, 0.0f);
    Hu3DModelScaleSet(obj->mdlId[0], 5.0f, 5.0f, 5.0f);
    obj->objFunc = NULL;
}

void fn_1_8CA4(OMOBJ *obj)
{
    s16 i;

    if (obj) {
        Hu3DModelHookReset(obj->mdlId[0]);
        for (i = 0; i < 2; i++) {
            Hu3DMotionKill(obj->mtnId[i]);
        }
        Hu3DModelKill(obj->mdlId[0]);
        Hu3DModelKill(obj->mdlId[1]);
        omDelObjEx(lbl_1_bss_8, obj);
    }
    obj = NULL;
}

void fn_1_8D44(void)
{
    if (lbl_1_data_2D0 != lbl_1_bss_1A30[0]) {
        lbl_1_bss_40 = 0.0f;
        lbl_1_data_2D0 = lbl_1_bss_1A30[0];
    }
    HuSprBankSet(lbl_1_bss_1A38[0], 0, lbl_1_bss_1A30[0]);
    fn_1_48C(lbl_1_bss_1A38[0], HUSPR_ATTR_DISPOFF);
}

inline void fn_1_8D44(void);

void fn_1_8E38(void)
{
    lbl_1_data_2D0 = -1;
    fn_1_40C(lbl_1_bss_1A38[0], HUSPR_ATTR_DISPOFF);
}

inline void fn_1_8E38(void);

void fn_1_8EC8(void)
{
    HuVecF pos;
    HuVecF pos2D;
    s16 index;

    lbl_1_bss_40 = fn_1_550(lbl_1_bss_40, 1.0f, 5.0f);
    index = lbl_1_bss_1A30[1] + (3 * lbl_1_bss_1A30[2]);
    if (lbl_1_bss_1A30[2] == 1) {
        pos.x = lbl_1_data_28[index].x / 1.5f;
    } else {
        pos.x = lbl_1_data_28[index].x / 2.0f;
    }
    pos.y = lbl_1_data_28[index].y;
    if (index == 1) {
        pos.y = lbl_1_data_28[index].y - 200.0f;
    }
    pos.z = lbl_1_data_28[index].z;
    Hu3D3Dto2D(&pos, 1, &pos2D);
    if (lbl_1_bss_1A30[2] == 1) {
        pos2D.y -= 100.0f;
    }
    HuSprGrpPosSet(lbl_1_bss_1A38[0], pos2D.x, pos2D.y);
    HuSprGrpScaleSet(lbl_1_bss_1A38[0], lbl_1_bss_40, lbl_1_bss_40);
}

inline void fn_1_8EC8(void);

void fn_1_90FC(HuVecF *pos)
{
    lbl_1_bss_94.x = pos->x - 200.0f;
    lbl_1_bss_94.y = 500.0f + pos->y;
    lbl_1_bss_94.z = 350.0f + pos->z;
}

inline void fn_1_90FC(HuVecF *pos);

void fn_1_9160(HuVecF *pos)
{
    OMOBJ *obj = lbl_1_bss_1C;

    Hu3DModelPosSetV(obj->mdlId[0], pos);
    Hu3DModelRotSet(obj->mdlId[0], 0.0f, 0.0f, 180.0f);
    Hu3DModelScaleSet(obj->mdlId[0], 5.0f, 5.0f, 5.0f);
    fn_1_90FC(pos);
    Hu3DModelAttrReset(obj->mdlId[0], HU3D_ATTR_DISPOFF);
}

inline void fn_1_9160(HuVecF *pos);

void fn_1_927C(void)
{
    OMOBJ *obj = lbl_1_bss_1C;

    Hu3DModelAttrSet(obj->mdlId[0], HU3D_ATTR_DISPOFF);
}

inline void fn_1_927C(void);

void fn_1_97D4(OMOBJ *obj)
{
    omSetStatBit(obj, OM_STAT_MODELPAUSE);
    obj->mdlId[0] = Hu3DModelCreateData(DATANUM(DATA_mdsel, 9));
    obj->mtnId[0] = Hu3DMotionIDGet(obj->mdlId[0]);
    Hu3DModelLayerSet(obj->mdlId[0], 1);
    Hu3DMotionShiftSet(obj->mdlId[0], obj->mtnId[0], 0.0f, 0.0f, HU3D_MOTATTR_LOOP);

    fn_1_927C();
    obj->objFunc = fn_1_92BC;
}

void fn_1_98B0(OMOBJ *obj)
{
    if (obj) {
        Hu3DMotionKill(obj->mtnId[0]);
        Hu3DModelKill(obj->mdlId[0]);
        omDelObjEx(lbl_1_bss_8, obj);
    }
    obj = NULL;
}

void fn_1_ECAC(s16 groupNo, s16 show)
{
    s16 i;

    for (i = 0; i < 5; i++) {
        if (show) {
            Hu3DModelAttrReset(lbl_1_bss_44[groupNo][i], HU3D_ATTR_DISPOFF);
        } else {
            Hu3DModelAttrSet(lbl_1_bss_44[groupNo][i], HU3D_ATTR_DISPOFF);
        }
    }
}

void fn_1_ED60(s16 groupNo, HuVecF *pos, GXColor *color)
{
    s16 i;
    HU3D_MODEL *model;
    HU3D_PARTICLE *particle;

    for (i = 0; i < 5; i++) {
        model = &Hu3DData[lbl_1_bss_44[groupNo][i]];
        particle = model->hookData;
        particle->dataCnt = 1;
        if (color != NULL) {
            particle->pos.x = color->r;
            particle->pos.y = color->g;
            particle->pos.z = color->b;
        }
        if (pos != NULL) {
            particle->unk_10.x = pos->x;
            particle->unk_10.y = pos->y;
            particle->unk_10.z = pos->z;
        }
        Hu3DModelAttrReset(lbl_1_bss_44[groupNo][i], HU3D_ATTR_DISPOFF);
    }
}

inline void fn_1_ED60(s16 groupNo, HuVecF *pos, GXColor *color);

void fn_1_EEC8(s16 groupNo)
{
    s16 i;
    HU3D_MODEL *model;
    s16 *work;

    for (i = 0; i < 5; i++) {
        model = &Hu3DData[lbl_1_bss_44[groupNo][i]];
        work = model->hookData;
        *work = 0;
    }
}

void fn_1_F4E4(void)
{
    s16 particleCount[5] = { 10, 10, 10, 10, 256 };
    s16 groupNo;
    s16 particleNo;

    for (groupNo = 0; groupNo < 6; groupNo++) {
        for (particleNo = 0; particleNo < 5; particleNo++) {
            lbl_1_bss_44[groupNo][particleNo] = Hu3DParticleCreate(
                lbl_1_bss_80[particleNo], particleCount[particleNo]);
            Hu3DModelPosSet(
                lbl_1_bss_44[groupNo][particleNo], 0.0f, 0.0f, 0.0f);
            Hu3DModelScaleSet(
                lbl_1_bss_44[groupNo][particleNo], 1.0f, 1.0f, 1.0f);
            Hu3DModelAttrSet(
                lbl_1_bss_44[groupNo][particleNo], HU3D_ATTR_DISPOFF);
            Hu3DModelLayerSet(lbl_1_bss_44[groupNo][particleNo], 2);
            Hu3DParticleHookSet(lbl_1_bss_44[groupNo][particleNo], fn_1_EF48);
            Hu3DParticleBlendModeSet(
                lbl_1_bss_44[groupNo][particleNo], HU3D_PARTICLE_BLEND_ADDCOL);
        }
    }
}

inline void fn_1_F4E4(void);

void fn_1_F70C(void)
{
    s16 i;
    s16 j;

    for (i = 0; i < 6; i++) {
        for (j = 0; j < 5; j++) {
            Hu3DModelKill(lbl_1_bss_44[i][j]);
        }
    }
}

void fn_1_FA18(void)
{
    s16 j;
    s16 i;

    for (i = 0; i < 6; i++) {
        for (j = 0; j < 5; j++) {
            Hu3DModelKill(lbl_1_bss_44[i][j]);
        }
    }
}

void fn_1_FA9C(s16 groupNo, HuVecF *pos, s16 mode, s16 colorNo)
{
    GXColor colors[7] = {
        { 254, 77, 75, 0 },
        { 50, 127, 200, 0 },
        { 199, 175, 0, 0 },
        { 52, 192, 63, 0 },
        { 159, 93, 200, 0 },
        { 255, 114, 46, 0 },
        { 109, 207, 246, 0 },
    };

    if (mode == 1) {
        fn_1_ED60(groupNo, pos, &colors[colorNo]);
    } else if (mode == 0) {
        fn_1_EEC8(groupNo);
    } else if (mode == 2) {
        fn_1_ECAC(groupNo, FALSE);
    }
}

void fn_1_FDF8(HuVecF *pos, s32 fxNo)
{
    HuVecF screenPos;
    s16 pan;

    Hu3D3Dto2D(pos, 1, &screenPos);
    screenPos.x -= 288.0f;
    screenPos.x /= 288.0f;
    pan = 64.0f + (16.0f * screenPos.x);
    if (pan < 48) {
        pan = 48;
    }
    if (pan > 72) {
        pan = 72;
    }
    HuAudFXPlayPan(fxNo, pan);
}

void fn_1_FEC0(HU3D_MODELID modelId, s32 fxNo, s16 panRange, s16 volume)
{
    HuVecF modelPos;
    HuVecF screenPos;
    s16 pan;

    if (modelId != HU3D_MODELID_NONE) {
        Hu3DModelPosGet(modelId, &modelPos);
        Hu3D3Dto2D(&modelPos, 1, &screenPos);
        screenPos.x -= 288.0f;
        screenPos.x /= 288.0f;
        pan = 64.0f + (screenPos.x * panRange);
        if (pan < 64 - panRange) {
            pan = 64 - panRange;
        }
        if (pan > 64 + panRange) {
            pan = 64 + panRange;
        }
        if (volume == -1) {
            HuAudFXPlayPan(fxNo, pan);
        } else {
            HuAudFXPlayVolPan(fxNo, volume, pan);
        }
    } else {
        HuAudFXPlay(fxNo);
    }
}

void fn_1_2CC(void)
{
    CharDataClose(-1);
    HuARDirFree(DATA_board);
    HuARDirFree(DATA_board_us);
    HuARDirFree(DATA_capsule);
    OSReport(lbl_1_data_A1);
    OSReport(lbl_1_data_CF, 0x21);
    OSReport(lbl_1_data_E0, 0x24);
    OSReport(lbl_1_data_F2, 0x9B);
    OSReport(lbl_1_data_104, 0xF2);
    HuAMemDump();
    OSReport(lbl_1_data_112);
}

void fn_1_37C(void)
{
    OSReport(lbl_1_data_114);
    OSReport(lbl_1_data_CF, 0x21);
    OSReport(lbl_1_data_E0, 0x24);
    OSReport(lbl_1_data_F2, 0x9B);
    OSReport(lbl_1_data_104, 0xF2);
    HuAMemDump();
    OSReport(lbl_1_data_112);
}

void fn_1_A310(void)
{
    s16 result = 0;

    result = fn_1_E7B0();

    HuAudSStreamFadeOut(lbl_1_bss_1A2C, 1000);
    switch (result) {
        case -1:
            WipeCreate(WIPE_MODE_OUT, WIPE_TYPE_NORMAL, 60);
            WipeWait();
            HuAudFadeOut(1000);
            fn_1_9910();
            fn_1_37C();
            omOvlReturn(1);
            break;
        case 1:
            HuAudFXPlay(0x4AF);
            WipeCreate(WIPE_MODE_OUT, WIPE_TYPE_DISSOLVE_IN_BLUR, 60);
            WipeWait();
            HuAudFadeOut(1000);
            fn_1_9910();
            fn_1_37C();
            fn_1_1B4();
            break;
    }
    HuPrcEnd();
    for (;;) {
        HuPrcVSleep();
    }
}

void fn_1_A5D4(void)
{
    lbl_1_bss_8 = omInitObjMan(27, 0x2000);
    omGameSysInit(lbl_1_bss_8);
    fn_1_1734();
    fn_1_1988();
    fn_1_1DDC();
    fn_1_2828();
    lbl_1_bss_C =
        omAddObjEx(lbl_1_bss_8, 0x1000, 0x10, 0x10, -1, fn_1_6F40);
    lbl_1_bss_10 =
        omAddObjEx(lbl_1_bss_8, 0x1000, 0x10, 0x10, -1, fn_1_76DC);
    lbl_1_bss_14 =
        omAddObjEx(lbl_1_bss_8, 0x1000, 0x10, 0x10, -1, fn_1_88D0);
    lbl_1_bss_18 =
        omAddObjEx(lbl_1_bss_8, 0x1000, 0x10, 0x10, -1, fn_1_8AE0);
    lbl_1_bss_1C =
        omAddObjEx(lbl_1_bss_8, 0x1000, 0x10, 0x10, -1, fn_1_97D4);
    fn_1_6C04();
    fn_1_F790();
    HuPrcChildCreate(fn_1_A310, 0x3000, 0x3000, 0, lbl_1_bss_8);
}

void fn_1_F790(void)
{
    s16 i;

    for (i = 0; i < 5; i++) {
        lbl_1_bss_80[i] = HuSprAnimRead(
            HuDataSelHeapReadNum(lbl_1_data_35C[i], HU_MEMNUM_OVL, HEAP_MODEL));
    }
    fn_1_F4E4();
}

void fn_1_B0C8(void)
{
    HuDataDirCloseAll();
}

void ObjectSetup(void)
{
    OSReport(lbl_1_data_2D2);
    fn_1_B0C8();
    MgPauseExitF = FALSE;
    fn_1_2CC();
    _SetFlag(5);
    MgInstExitF = FALSE;
    MgPauseExitF = FALSE;
    MgExitReq = FALSE;
    _ClearFlag(FLAG_BOARD_INIT);
    _ClearFlag(FLAG_INST_MG_MODE);
    _ClearFlag(FLAG_INST_DECA);
    _ClearFlag(FLAG_INST_NO_HISCHG);
    _ClearFlag(FLAGNUM(FLAG_GROUP_SYSTEM, 4));
    lbl_1_bss_0 = omovlevtno;
    lbl_1_bss_4 = omovlstat;
    fn_1_A5D4();
}

BOOL fn_1_B304(void)
{
    HuPrcSleep(5);
    fn_1_FEC(-1);
    lbl_1_bss_1A2C = HuAudSStreamPlay(MSM_STREAM_MODESEL);
    WipeCreate(WIPE_MODE_IN, WIPE_TYPE_NORMAL, 60);
    WipeWait();
    return TRUE;
}

s16 fn_1_B414(void)
{
    s16 i;

    if (lbl_1_bss_0 == 0) {
        HuAudFXPlayPan(0x3B6, 0x30);
        HuAudFXPlayPan(0x3AE, 0x50);
        fn_1_23E0(1, 0xA0000, 1);
        fn_1_2288();
        fn_1_21DC();
        lbl_1_bss_1A30[0] = 0;
        lbl_1_bss_1A30[1] = 1;
        lbl_1_bss_1A30[2] = 1;
    } else {
        HuPrcSleep(10);
        lbl_1_bss_1A30[0] = (s16)lbl_1_bss_4;
        for (i = 0; i < 6; i++) {
            if (lbl_1_bss_4 == lbl_1_data_70[i]) {
                break;
            }
        }
        lbl_1_bss_1A30[1] = i % 3;
        lbl_1_bss_1A30[2] = i / 3;
    }
    return TRUE;
}

s16 fn_1_B804(void)
{
    HuAudFXPlayPan(0x3B8, 0x30);
    HuAudFXPlayPan(0x3B0, 0x50);
    fn_1_23E0(1, 0xA0001, 1);
    return fn_1_22E8(2);
}

s16 fn_1_E7B0(void)
{
    s16 result = 0;

    fn_1_B304();
    fn_1_B414();
    result = fn_1_BAB4();
    if (result != -1) {
        fn_1_E1FC();
    }
    return result;
}

u32 lbl_1_data_0[1] = { DATANUM(DATA_mdsel, 10) };
s16 lbl_1_data_4[2] = { 1, 0 };
LBL_1_DATA_8_ENTRY lbl_1_data_8[1] = {
    { 0, 0, 0, 0, 0, 0, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 0.0f },
};
HuVecF lbl_1_data_28[6] = {
    { -670.0f, 0.0f, -800.0f },
    { 0.0f, 200.0f, -1100.0f },
    { 650.0f, 0.0f, -780.0f },
    { -1156.0f, -200.0f, -27.0f },
    { 0.0f, 0.0f, 550.0f },
    { 1200.0f, 0.0f, 0.0f },
};
s16 lbl_1_data_70[6] = { 3, 5, 4, 1, 0, 2 };
s32 lbl_1_data_7C = -1;
char lbl_1_data_80[] = "# ========== win callback :: %d\n";
char lbl_1_data_A1[] = ">>>>>>>>>> mdseldll :: objsetup!! <<<<<<<<<<\n";
char lbl_1_data_CF[] = "0x%x :: _effect\n";
char lbl_1_data_E0[] = "0x%x :: _gamemes\n";
char lbl_1_data_F2[] = "0x%x :: _mgconst\n";
char lbl_1_data_104[] = "0x%x :: _win\n";
char lbl_1_data_112[] = "\n";
char lbl_1_data_114[0x3C] = "\n>>>>>>>>>> mdseldll :: dllreturn or dllmove!! <<<<<<<<<<\n";
s16 lbl_1_data_150[2] = { -1, -1 };
s32 lbl_1_data_154[2] = { -1, -1 };
s16 lbl_1_data_15C = 1;
char lbl_1_data_15E[] = "Result %d\n";
char lbl_1_data_169[] =
    "MIC SETTING >>> err!! <<< :: HuMCMount(B) != MIC_RESULT_READY\n";
char lbl_1_data_1A8[] =
    "MIC SETTING >>> check <<< :: HuMCMount(B) == MIC_RESULT_READY\n";
char lbl_1_data_1E7[] =
    "MIC SETTING >>> err!! <<< :: GwCommon.mic != HUMC_FLAG_USE\n";
char lbl_1_data_223[] =
    "MIC SETTING >>> ok!! <<< :: GwCommon.mic == HUMC_FLAG_USE\n";
char lbl_1_data_25E[] = "/mic/ctx/mselect_words";
char lbl_1_data_275[] = "mic ok!!\n";
char lbl_1_data_27F[] =
    "MIC SETTING >>> ok!! <<< :: HuMCMount(B) == MIC_RESULT_READY\n";
char lbl_1_data_2BD[0x13] = "gN01m1-itemhook_R";
s16 lbl_1_data_2D0 = -1;
char lbl_1_data_2D2[] = "\n-----===== MARIO PARTY 6 :: MODE SELECT =====-----\n\n";
char lbl_1_data_308[] = "MIC CHECK!! >> nouse!! << :: device err!!\n";
char lbl_1_data_333[] = "MIC CHECK!! >> use!! << :: device error\n";
u32 lbl_1_data_35C[5] = {
    DATANUM(DATA_mdsel, 0x25),
    DATANUM(DATA_mdsel, 0x25),
    DATANUM(DATA_mdsel, 0x25),
    DATANUM(DATA_mdsel, 0x25),
    DATANUM(DATA_mdsel, 0x25),
};

int lbl_1_bss_0;
int lbl_1_bss_4;
OMOBJMAN *lbl_1_bss_8;
OMOBJ *lbl_1_bss_C;
OMOBJ *lbl_1_bss_10;
OMOBJ *lbl_1_bss_14;
OMOBJ *lbl_1_bss_18;
OMOBJ *lbl_1_bss_1C;
OMOBJ *lbl_1_bss_20[2];
void *lbl_1_bss_28;
OMOBJ *lbl_1_bss_30;
OMOBJ *lbl_1_bss_34;
s16 lbl_1_bss_38;
s16 lbl_1_bss_3A;
s16 lbl_1_bss_3C;
s16 lbl_1_bss_3E;
float lbl_1_bss_40;
HU3D_MODELID lbl_1_bss_44[6][5];
ANIMDATA *lbl_1_bss_80[5];
HuVecF lbl_1_bss_94;
MDSEL_BEZIER_WORK lbl_1_bss_A0;
MDSEL_BEZIER_WORK lbl_1_bss_128;
s16 lbl_1_bss_1B0[0x12];
LBL_1_BSS_1D4_ENTRY lbl_1_bss_1D4[15];
s16 lbl_1_bss_24C[0x330];
LBL_1_BSS_8AC_ENTRY lbl_1_bss_8AC[2];
LBL_1_BSS_8AC_ENTRY lbl_1_bss_9BC[30];
HuVecF lbl_1_bss_19AC[4];
s32 lbl_1_bss_19DC[4];
u8 lbl_1_bss_19EC[0x3C];
u32 lbl_1_bss_1A28;
s32 lbl_1_bss_1A2C;
s16 lbl_1_bss_1A30[3];
HUSPRID lbl_1_bss_1A36[1];
HUSPR_GROUPID lbl_1_bss_1A38[1];
ANIMDATA *lbl_1_bss_1A3C[1];
HUWINID lbl_1_bss_1A40[4];
HU3D_LIGHTID lbl_1_bss_1A48[2];
s16 lbl_1_bss_1A4C;
