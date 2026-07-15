#include "datadir_enum.h"

#include "dolphin/mic.h"
#include "dolphin/os.h"

#include "game/armem.h"
#include "game/charman.h"
#include "game/flag.h"
#include "game/gamework.h"
#include "game/mgdata.h"
#include "game/object.h"
#include "game/pad.h"
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
    s16 unk_00;
    s16 unk_02;
    float unk_04;
    float unk_08;
    HuVecF unk_0C;
    float unk_18;
    float unk_1C;
    float unk_20;
    float unk_24;
    float unk_28;
    u8 unk_2C[4];
    float unk_30;
    u8 unk_34[0xC];
    s16 unk_40;
    s16 unk_42;
    s16 unk_44;
    s16 unk_46;
    float unk_48;
    float unk_4C;
    float unk_50;
    u8 unk_54[4];
    HuVecF unk_58;
    HuVecF unk_64;
    u8 unk_70[0x18];
} LBL_1_BSS_8AC_ENTRY;

typedef struct Lbl1Bss24CEntry {
    s16 unk_00;
    s16 unk_02;
    float unk_04;
    float unk_08;
    HuVecF unk_0C;
    HuVecF unk_18;
    u8 unk_24[0xC];
    float unk_30;
    float unk_34;
    u8 unk_38[0x50];
} LBL_1_BSS_24C_ENTRY;

typedef struct MdselMicResponse {
    s16 status;
    s16 unk_02;
    s16 count;
    s16 pad;
    s16 *result;
    s32 unk_0C;
} MDSEL_MIC_RESPONSE;

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
extern void HuMCMicSet(s32 flag);
extern void HuMCListenerKill(void);
extern void HuMCContextKill(s16 context);
extern s16 HuMCContextCreate(char *path);
extern void HuMCListenerCreate(
    s16 context, MCResponseCallback callback, u8 property);
extern s32 frandmod(s32 modulus);
extern s32 rand8(void);

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
extern float lbl_1_bss_2C;
extern OMOBJ *lbl_1_bss_30;
extern OMOBJ *lbl_1_bss_34;
extern s16 lbl_1_bss_38;
extern s16 lbl_1_bss_3A;
extern s16 lbl_1_bss_3C;
extern s16 lbl_1_bss_3E;
extern float lbl_1_bss_40;
extern HU3D_MODELID lbl_1_bss_44[6][5];
extern ANIMDATA *lbl_1_bss_80[5];
extern HuVecF lbl_1_bss_94;
extern MDSEL_BEZIER_WORK lbl_1_bss_A0;
extern MDSEL_BEZIER_WORK lbl_1_bss_128;
extern s16 lbl_1_bss_1B0[6][3];
extern LBL_1_BSS_24C_ENTRY lbl_1_bss_24C[12];
extern LBL_1_BSS_1D4_ENTRY lbl_1_bss_1D4[15];
extern LBL_1_BSS_8AC_ENTRY lbl_1_bss_8AC[2];
extern LBL_1_BSS_8AC_ENTRY lbl_1_bss_9BC[30];
extern HuVecF lbl_1_bss_19AC[4];
extern s32 lbl_1_bss_19DC[4];
extern s16 lbl_1_bss_19EC[30];
extern s32 lbl_1_bss_1A28;
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
extern char lbl_1_data_15E[];
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
void fn_1_651C(MDSEL_MIC_RESPONSE *response);
void fn_1_EF48(HU3D_MODEL *model, HU3D_PARTICLE *particle, Mtx matrix);
void fn_1_F790(void);
void fn_1_2A78(s16 layerNo);
void fn_1_2828(void);
void fn_1_346C(OMOBJ *obj);
void fn_1_485C(void);
void fn_1_4CF4(OMOBJ *obj);
void fn_1_5290(void);
void fn_1_5BF0(void);
void fn_1_5EA4(s16 index);
s16 fn_1_5F60(HU3D_MODELID modelId, s32 fxNo);
void fn_1_70BC(OMOBJ *obj);
void fn_1_7868(OMOBJ *obj);
void fn_1_809C(OMOBJ *obj);
void fn_1_FA9C(s16 groupNo, HuVecF *pos, s16 mode, s16 colorNo);
void fn_1_92BC(OMOBJ *obj);
void fn_1_FDF8(HuVecF *pos, s32 fxNo);
void fn_1_FEC0(HU3D_MODELID modelId, s32 fxNo, s16 volume, s16 pan);
void fn_1_FA18(void);
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

inline void fn_1_598(HuVecF *dst, const HuVecF *src, float weight);

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

inline void fn_1_C8C(
    HU3D_MODELID modelId, HuVecF *start, HuVecF *end, float time,
    float duration);

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

void fn_1_11D4(void)
{
    float divisor;
    float unused;
    float y;
    s16 index;

    divisor = 1.0f;
    unused = 2.0f;
    y = 250.0f;
    index = lbl_1_bss_1A30[1] + (3 * lbl_1_bss_1A30[2]);
    if (lbl_1_bss_1A30[0] == 5) {
        y = 400.0f;
    }
    lbl_1_bss_19AC[1].x = lbl_1_data_28[index].x;
    lbl_1_bss_19AC[1].y = y + lbl_1_data_28[index].y;
    lbl_1_bss_19AC[1].z = 1000.0f + lbl_1_data_28[index].z;
    lbl_1_bss_19AC[3].x = lbl_1_data_28[index].x / divisor;
    lbl_1_bss_19AC[3].y = y + (lbl_1_data_28[index].y / divisor);
    lbl_1_bss_19AC[3].z = lbl_1_data_28[index].z / divisor;
}

inline void fn_1_11D4(void);

void fn_1_1370(OMOBJ *obj)
{
    fn_1_598(&lbl_1_bss_19AC[0], &lbl_1_bss_19AC[1], 15.0f);
    fn_1_598(&lbl_1_bss_19AC[2], &lbl_1_bss_19AC[3], 15.0f);
    Hu3DCameraPosSet(
        1, lbl_1_bss_19AC[0].x, lbl_1_bss_19AC[0].y,
        lbl_1_bss_19AC[0].z, 0.0f, 1.0f, 0.0f,
        lbl_1_bss_19AC[2].x, lbl_1_bss_19AC[2].y,
        lbl_1_bss_19AC[2].z);
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

void fn_1_2A78(s16 layerNo)
{
    float unused;
    GXTexObj texObj;
    Mtx44 projection;
    Mtx trans;
    Mtx rot;
    Mtx model;

    if (lbl_1_bss_28 != NULL) {
        MTXOrtho(projection, 0.0f, 480.0f, 0.0f, 640.0f, 0.0f, 10.0f);
        GXSetProjection(projection, GX_ORTHOGRAPHIC);
        GXInitTexObj(
            &texObj, lbl_1_bss_28, 640, 480, GX_TF_RGBA8, GX_CLAMP,
            GX_CLAMP, GX_FALSE);
        GXInitTexObjLOD(
            &texObj, GX_LINEAR, GX_LINEAR, 0.0f, 0.0f, 0.0f, GX_FALSE,
            GX_FALSE, GX_ANISO_1);
        GXLoadTexObj(&texObj, GX_TEXMAP0);
        GXClearVtxDesc();
        GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
        GXSetVtxAttrFmt(
            GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
        GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
        GXSetVtxAttrFmt(
            GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
        GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
        GXSetVtxAttrFmt(
            GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(
            GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY,
            GX_FALSE, GX_PTIDENTITY);
        GXSetNumChans(1);
        GXSetChanCtrl(
            GX_COLOR0A0, GX_FALSE, GX_SRC_VTX, GX_SRC_VTX, 0,
            GX_DF_CLAMP, GX_AF_NONE);
        GXSetNumTevStages(1);
        GXSetTevOrder(
            GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
        GXSetTevColorIn(
            GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_TEXC, GX_CC_RASC,
            GX_CC_ZERO);
        GXSetTevColorOp(
            GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
            GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaIn(
            GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_TEXA, GX_CA_RASA,
            GX_CA_ZERO);
        GXSetTevAlphaOp(
            GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
            GX_TRUE, GX_TEVPREV);

        MTXTrans(trans, -320.0f, -240.0f, 0.0f);
        MTXRotRad(rot, 'Z', MTXDegToRad(lbl_1_bss_2C / 10.0f));
        MTXConcat(rot, trans, model);
        mtxTransCat(model, 320.0f, 240.0f, 0.0f);
        GXLoadPosMtxImm(model, GX_PNMTX0);
        unused = fn_1_724(0.0f, 1.0f, ++lbl_1_bss_2C, 60.0f);

        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        GXPosition3f32(-50.0f, -50.0f, 0.0f);
        GXColor4u8(128, 128, 128, 160);
        GXTexCoord2f32(0.0f, 0.0f);
        GXPosition3f32(690.0f, -50.0f, 0.0f);
        GXColor4u8(128, 128, 128, 160);
        GXTexCoord2f32(1.0f, 0.0f);
        GXPosition3f32(690.0f, 530.0f, 0.0f);
        GXColor4u8(128, 128, 128, 160);
        GXTexCoord2f32(1.0f, 1.0f);
        GXPosition3f32(-50.0f, 530.0f, 0.0f);
        GXColor4u8(128, 128, 128, 160);
        GXTexCoord2f32(0.0f, 1.0f);

        if (lbl_1_bss_28) {
            Hu3DFbCopyExec(
                0, 0, 640, 480, GX_TF_RGBA8, GX_FALSE, lbl_1_bss_28);
        }
    }
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

inline void fn_1_3274(void);

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

inline void fn_1_33B8(void);

void fn_1_346C(OMOBJ *obj)
{
    LBL_1_BSS_8AC_ENTRY *entry;
    HuVecF pos;
    s16 i;

    for (i = 0; i < 30; i++) {
        entry = &lbl_1_bss_9BC[i];
        if (entry->unk_00 == 0) {
            continue;
        }
        entry->unk_30 -= 1.0f;
        if (entry->unk_30 == 0.0f) {
            lbl_1_bss_19EC[i] = fn_1_5F60(obj->mdlId[i], 0x3C3);
        }
        entry->unk_0C.x += entry->unk_18;
        pos.x = entry->unk_0C.x;
        pos.y = entry->unk_0C.y
            + fn_1_80C(
                0.0f, entry->unk_1C, entry->unk_04, entry->unk_08);
        pos.z = entry->unk_0C.z;
        Hu3DModelPosSetV(obj->mdlId[i], &pos);
        if (++entry->unk_04 > entry->unk_08) {
            entry->unk_04 = 0.0f;
        }
        if (pos.x > 2000.0f) {
            fn_1_5EA4(lbl_1_bss_19EC[i]);
            entry->unk_00 = 0;
            Hu3DModelAttrSet(obj->mdlId[i], HU3D_ATTR_DISPOFF);
        }
        pos.y = fn_1_80C(
            0.0f, entry->unk_20, entry->unk_24, entry->unk_28);
        Hu3DModelRotSet(obj->mdlId[i], 0.0f, 90.0f + pos.y, 0.0f);
        if (++entry->unk_24 > entry->unk_28) {
            entry->unk_20 = frandmod(60);
            entry->unk_24 = 0.0f;
            entry->unk_28 = frandmod(120) + 60;
        }
    }
    for (i = 0; i < 30; i++) {
        entry = &lbl_1_bss_9BC[i];
        if (entry->unk_00 != 0) {
            break;
        }
    }
    if (i == 30) {
        obj->objFunc = NULL;
    }
}

void fn_1_3910(void)
{
    OMOBJ *obj = lbl_1_bss_30;
    LBL_1_BSS_8AC_ENTRY *entry;
    s16 i;

    for (i = 0; i < 30; i++) {
        entry = &lbl_1_bss_9BC[i];
        entry->unk_00 = (rand8() % 2) + 1;
        entry->unk_30 = rand8() % 10;
        entry->unk_0C.x = -2000.0f - frandmod(500);
        entry->unk_0C.y = 700.0f + frandmod(200);
        entry->unk_0C.z = frandmod(3000) - 500;
        entry->unk_04 = frandmod(120);
        entry->unk_08 = frandmod(120) + 120;
        entry->unk_18 = 5.0f + (i % 10);
        entry->unk_1C = 100.0f + frandmod(200);
        entry->unk_20 = frandmod(60);
        entry->unk_24 = 0.0f;
        entry->unk_28 = frandmod(120) + 60;
        Hu3DMotionShiftSet(
            obj->mdlId[i], obj->mtnId[0], 0.0f, 0.0f,
            HU3D_MOTATTR_LOOP);
        Hu3DModelAttrReset(obj->mdlId[i], HU3D_ATTR_DISPOFF);
        Hu3DModelPosSetV(obj->mdlId[i], &entry->unk_0C);
        Hu3DModelRotSet(obj->mdlId[i], 0.0f, 90.0f, 0.0f);
    }
    obj->objFunc = fn_1_346C;
}

inline void fn_1_3910(void);

void fn_1_3CC0(void)
{
    OMOBJ *obj = lbl_1_bss_30;
    LBL_1_BSS_8AC_ENTRY *entry;
    HuVecF rot;
    s16 i;

    for (i = 0; i < 2; i++) {
        entry = &lbl_1_bss_8AC[i];
        entry->unk_44++;
        if (entry->unk_44 > 180) {
            entry->unk_44 = 0;
            entry->unk_46--;
            if (entry->unk_46 < 0) {
                entry->unk_46 = 0;
            }
        }
        if (entry->unk_4C <= 100.0f && entry->unk_40 == 100
            && rand8() % 6 == 0) {
            fn_1_FEC0(obj->mdlId[i + 30], 0x3D4, 16, 100);
        }
        switch (entry->unk_40) {
            case 0:
                entry->unk_40 = 100;
                entry->unk_48 = 0.0f;
                if (entry->unk_42 == 0) {
                    entry->unk_4C = (rand8() % 100) + 120;
                } else {
                    entry->unk_4C = (rand8() % 30) + 60;
                }
                if (entry->unk_42 == 2) {
                    if (rand8() % 2 == 0) {
                        entry->unk_42 = 0;
                    } else {
                        entry->unk_42 = 1;
                    }
                }
                Hu3DModelPosGet(obj->mdlId[i + 30], &entry->unk_58);
                Hu3DModelPosGet(obj->mdlId[i + 30], &entry->unk_64);
                if (entry->unk_58.x > 0.0f) {
                    if (i == 0) {
                        entry->unk_64.x = (frandmod(200) + 400) * -1;
                    } else {
                        entry->unk_64.x = (frandmod(100) + 400) * -1;
                    }
                } else {
                    if (i == 0) {
                        entry->unk_64.x = frandmod(200) + 400;
                    } else {
                        entry->unk_64.x = frandmod(100) + 400;
                    }
                }
                Hu3DMotionShiftSet(
                    obj->mdlId[i + 30], obj->mtnId[31], 0.0f, 8.0f,
                    HU3D_MOTATTR_LOOP);
                break;
            case 100:
                fn_1_C8C(
                    obj->mdlId[i + 30], &entry->unk_58, &entry->unk_64,
                    entry->unk_48, entry->unk_4C);
                if (++entry->unk_48 > entry->unk_4C) {
                    entry->unk_40 = 200;
                    entry->unk_48 = 0.0f;
                    entry->unk_50 = frandmod(360) - 180;
                    if (entry->unk_42 == 1) {
                        entry->unk_40 = 0;
                        entry->unk_42 = 2;
                    }
                }
                break;
            case 200:
                Hu3DModelRotGet(obj->mdlId[i + 30], &rot);
                rot.y = fn_1_C28(rot.y, entry->unk_50, 30.0f);
                Hu3DModelRotSetV(obj->mdlId[i + 30], &rot);
                entry->unk_42 = 0;
                if (++entry->unk_48 > 30.0f) {
                    Hu3DMotionShiftSet(
                        obj->mdlId[i + 30], obj->mtnId[30], 0.0f, 8.0f,
                        HU3D_MOTATTR_LOOP);
                    entry->unk_40 = 300;
                    entry->unk_48 = 0.0f;
                    entry->unk_4C = rand8() + 120;
                }
                break;
            case 300:
                if (++entry->unk_48 > entry->unk_4C) {
                    if (rand8() % 2 == 0) {
                        Hu3DMotionShiftSet(
                            obj->mdlId[i + 30], obj->mtnId[31], 0.0f,
                            8.0f, HU3D_MOTATTR_LOOP);
                        entry->unk_40 = 200;
                        entry->unk_48 = 0.0f;
                        entry->unk_50 = frandmod(360) - 180;
                    } else {
                        entry->unk_40 = 0;
                    }
                }
                break;
            case 400:
                if (++entry->unk_48 > entry->unk_4C) {
                    entry->unk_40 = 0;
                    entry->unk_42 = 1;
                }
                break;
        }
    }
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

void fn_1_485C(void)
{
    LBL_1_BSS_24C_ENTRY *entry;
    OMOBJ *obj;
    HuVecF pos;
    s16 i;

    i = 9;
    entry = &lbl_1_bss_24C[9];
    obj = lbl_1_bss_30;
    Hu3DModelDispOn(obj->mdlId[i + 35]);
    Hu3DModelPosGet(obj->mdlId[i + 35], &pos);
    if (lbl_1_bss_3C == 1) {
        pos.y = fn_1_C28(pos.y, -400.0f, 10.0f);
        entry->unk_34 = 0.0f;
    } else {
        pos.y = fn_1_C28(pos.y, -200.0f, 10.0f);
    }
    if (entry->unk_34 == 1.0f) {
        pos.y = fn_1_C28(pos.y, -400.0f, 10.0f);
    }
    Hu3DModelPosSet(obj->mdlId[i + 35], pos.x, pos.y, pos.z);
    if (lbl_1_bss_3C == 0) {
        if (lbl_1_bss_24C[0].unk_00 == 1) {
            Hu3DModelPosSet(obj->mdlId[i + 35], -800.0f, pos.y, -100.0f);
        } else {
            Hu3DModelPosSet(obj->mdlId[i + 35], 820.0f, pos.y, -120.0f);
        }
    }
    Hu3DModelRotGet(obj->mdlId[i + 35], &pos);
    pos.y = fn_1_C28(pos.y, entry->unk_30, 15.0f);
    Hu3DModelRotSet(obj->mdlId[i + 35], -40.0f, pos.y, 0.0f);
    Hu3DModelScaleSet(obj->mdlId[i + 35], 1.5f, 1.5f, 1.5f);
    if (++entry->unk_04 > entry->unk_08) {
        entry->unk_04 = 0.0f;
        entry->unk_08 = rand8() + 90;
        entry->unk_30 = frandmod(90) - 45;
        entry->unk_34 = 0.0f;
        if (rand8() % 2 == 0) {
            entry->unk_34 = 1.0f;
        }
    }
}

inline void fn_1_485C(void);

void fn_1_4CF4(OMOBJ *obj)
{
    LBL_1_BSS_24C_ENTRY *unused;
    LBL_1_BSS_24C_ENTRY *entry;
    HuVecF pos;
    s16 i;

    unused = lbl_1_bss_24C;
    for (i = 0; i < lbl_1_bss_3A; i++) {
        entry = &lbl_1_bss_24C[i];
        if (entry->unk_04 == 0.0f) {
            fn_1_FEC0(obj->mdlId[i + 35], 0x4AB, 16, -1);
        }
        pos.x = fn_1_C48(
            entry->unk_0C.x, entry->unk_18.x, entry->unk_04,
            entry->unk_08);
        pos.z = fn_1_C48(
            entry->unk_0C.z, entry->unk_18.z, entry->unk_04,
            entry->unk_08);
        pos.y = fn_1_8E8(-300.0f, 1000.0f, entry->unk_04, entry->unk_08);
        Hu3DModelPosSet(obj->mdlId[i + 35], pos.x, pos.y, pos.z);
        if (entry->unk_00 == 0) {
            pos.x = fn_1_C48(-90.0f, 90.0f, entry->unk_04, entry->unk_08);
            Hu3DModelRotSet(obj->mdlId[i + 35], pos.x, 90.0f, 0.0f);
        } else {
            pos.x = fn_1_C48(-90.0f, 90.0f, entry->unk_04, entry->unk_08);
            Hu3DModelRotSet(obj->mdlId[i + 35], pos.x, -90.0f, 0.0f);
        }
        if (entry->unk_04 == entry->unk_08 - 5.0f) {
            fn_1_FEC0(obj->mdlId[i + 35], 0x4AC, 16, -1);
        }
        if (++entry->unk_04 > entry->unk_08) {
            Hu3DModelDispOff(obj->mdlId[i + 35]);
        }
    }
    if (lbl_1_bss_24C[lbl_1_bss_3A - 1].unk_04
        > lbl_1_bss_24C[lbl_1_bss_3A - 1].unk_08) {
        lbl_1_bss_3C = 0;
        obj->objFunc = NULL;
    }
}

void fn_1_5290(void)
{
    LBL_1_BSS_24C_ENTRY *unused;
    LBL_1_BSS_24C_ENTRY *entry;
    s16 i;
    OMOBJ *obj;
    s16 count;
    s16 mode;

    obj = lbl_1_bss_30;
    unused = lbl_1_bss_24C;
    mode = lbl_1_bss_24C[0].unk_00;
    mode++;
    mode %= 2;
    count = (rand8() % 8) + 1;
    lbl_1_bss_3A = count;
    lbl_1_bss_3C = 1;
    for (i = 0; i < count; i++) {
        entry = &lbl_1_bss_24C[i];
        entry->unk_04 = ((-5 * i) - 20) - (rand8() % 3);
        entry->unk_08 = 120.0f;
        entry->unk_00 = mode;
        if (entry->unk_00 == 0) {
            entry->unk_0C.x = -800.0f;
            entry->unk_0C.y = 0.0f;
            entry->unk_0C.z = -100.0f;
            entry->unk_18.x = (800.0f + frandmod(100)) - 50.0f;
            entry->unk_18.y = 0.0f;
            entry->unk_18.z = (-150.0f + frandmod(100)) - 50.0f;
        } else {
            entry->unk_18.x = -800.0f;
            entry->unk_18.y = 0.0f;
            entry->unk_18.z = -100.0f;
            entry->unk_0C.x = (800.0f + frandmod(100)) - 50.0f;
            entry->unk_0C.y = 0.0f;
            entry->unk_0C.z = (-150.0f + frandmod(100)) - 50.0f;
        }
        Hu3DModelDispOn(obj->mdlId[i + 35]);
        if (i > 0) {
            Hu3DModelScaleSet(obj->mdlId[i + 35], 1.0f, 1.0f, 1.0f);
        }
    }
    obj->objFunc = fn_1_4CF4;
}

void fn_1_5614(OMOBJ *obj)
{
    s16 i;
    s16 base;

    omSetStatBit(obj, OM_STAT_MODELPAUSE);
    for (i = 0; i < 30; i++) {
        if (i == 0) {
            obj->mdlId[i] = Hu3DModelCreateData(DATANUM(DATA_mdsel, 0x11));
            obj->mtnId[i] = Hu3DJointMotionData(
                obj->mdlId[0], DATANUM(DATA_mdsel, 0x12));
        } else {
            obj->mdlId[i] = Hu3DModelLink(obj->mdlId[0]);
        }
        Hu3DModelDispOff(obj->mdlId[i]);
        Hu3DModelPosSet(
            obj->mdlId[i], frandmod(1000) - 500, frandmod(1000),
            -frandmod(1000));
        Hu3DModelLayerSet(obj->mdlId[i], 1);
        Hu3DMotionShiftSet(
            obj->mdlId[i], obj->mtnId[0], 0.0f, 0.0f,
            HU3D_MOTATTR_LOOP);
    }

    base = 30;
    for (i = 0; i < 2; i++) {
        obj->mdlId[i + base] =
            Hu3DModelCreateData(DATANUM(DATA_mdsel, 0xB));
        if (i == 0) {
            obj->mtnId[30] = Hu3DJointMotionData(
                obj->mdlId[30], DATANUM(DATA_mdsel, 0xC));
            obj->mtnId[31] = Hu3DJointMotionData(
                obj->mdlId[30], DATANUM(DATA_mdsel, 0xD));
            obj->mtnId[32] = Hu3DJointMotionData(
                obj->mdlId[30], DATANUM(DATA_mdsel, 0xE));
        }
        Hu3DModelPosSet(
            obj->mdlId[i + base], frandmod(1000) - 500, 0.0f,
            -350.0f + (300.0f * i));
        Hu3DModelScaleSet(obj->mdlId[i + base], 1.5f, 1.5f, 1.5f);
        Hu3DModelLayerSet(obj->mdlId[i + base], 1);
        Hu3DMotionShiftSet(
            obj->mdlId[i + base], obj->mtnId[30], 0.0f, 0.0f,
            HU3D_MOTATTR_LOOP);
    }

    base = 35;
    for (i = 0; i < 10; i++) {
        obj->mdlId[i + 35] =
            Hu3DModelCreateData(DATANUM(DATA_mdsel, 0xF));
        obj->mtnId[i + 35] = Hu3DJointMotionData(
            obj->mdlId[i + 35], DATANUM(DATA_mdsel, 0x10));
        Hu3DModelPosSet(obj->mdlId[i + 35], 0.0f, 500.0f, 0.0f);
        Hu3DModelScaleSet(obj->mdlId[i + 35], 1.5f, 1.5f, 1.5f);
        Hu3DModelDispOff(obj->mdlId[i + 35]);
        Hu3DModelLayerSet(obj->mdlId[i + 35], 1);
        Hu3DMotionShiftSet(
            obj->mdlId[i + 35], obj->mtnId[i + 35], 0.0f, 0.0f,
            HU3D_MOTATTR_LOOP);
    }
    lbl_1_bss_24C[0].unk_00 = 1;
    obj->objFunc = NULL;
}

void fn_1_5BEC(void)
{
}

void fn_1_5BF0(void)
{
    HuVecF modelPos;
    HuVecF screenPos;
    s16 pan;
    s16 panRange = 32;
    s16 volume;
    s16 i;

    for (i = 0; i < 15; i++) {
        if (lbl_1_bss_1D4[i].modelId == HU3D_MODELID_NONE) {
            continue;
        }

        Hu3DModelPosGet(lbl_1_bss_1D4[i].modelId, &modelPos);
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

        Hu3DModelPosGet(lbl_1_bss_1D4[i].modelId, &modelPos);
        modelPos.x = (modelPos.x < 0.0f) ? -modelPos.x : modelPos.x;
        if (modelPos.x < 1500.0f) {
            volume = 127;
        } else {
            volume = 127.0f -
                (127.0f * ((modelPos.x - 1500.0f) / 500.0f));
        }
        if (volume > 127) {
            volume = 127;
        }
        if (volume < 0) {
            volume = 0;
        }

        if (lbl_1_bss_1D4[i].fxHandle > 0) {
            HuAudFXPanning(lbl_1_bss_1D4[i].fxHandle, pan);
            HuAudFXVolSet(lbl_1_bss_1D4[i].fxHandle, volume);
            i == 0;
        }
    }
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

void fn_1_607C(OMOBJ *obj)
{
    fn_1_5BF0();
    fn_1_3CC0();
    fn_1_485C();
}

void fn_1_651C(MDSEL_MIC_RESPONSE *response)
{
    if (response->status != 0 || response->count == 0) {
        return;
    }
    if (lbl_1_bss_30->objFunc != NULL) {
        return;
    }
    if (lbl_1_data_15C == 0) {
        return;
    }
    switch (response->result[0]) {
        case 4:
            fn_1_3910();
            break;
        case 2:
            fn_1_46DC();
            break;
        case 3:
            fn_1_5290();
            break;
        case 0:
            fn_1_3274();
            break;
        case 1:
            fn_1_33B8();
            break;
    }
    {
        s16 i;

        for (i = 0; i < response->count; i++) {
            OSReport(lbl_1_data_15E, response->result[i]);
        }
    }
}

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
                HuMCListenerCreate(lbl_1_bss_1A4C, (MCResponseCallback)fn_1_651C, 1);
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

inline void fn_1_6DFC(void);

s16 fn_1_6E54(void)
{
    s16 result = TRUE;

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

inline void fn_1_702C(OMOBJ *obj);

void fn_1_70BC(OMOBJ *obj)
{
    float motionTime;
    s16 i;

    for (i = 0; i < 6; i++) {
        motionTime = Hu3DMotionTimeGet(obj->mdlId[i]);
        if (motionTime < 5.0f) {
            lbl_1_bss_1B0[i][0] = 0;
            lbl_1_bss_1B0[i][1] = 0;
            lbl_1_bss_1B0[i][2] = 0;
        }
        switch (i) {
            case 0:
                if (lbl_1_bss_1B0[0][0] == 0 && motionTime > 80.0f) {
                    lbl_1_bss_1B0[0][0] = 1;
                    HuAudFXPlay(0x4A4);
                }
                break;
            case 1:
                if (lbl_1_bss_1B0[1][0] == 0 && motionTime > 60.0f) {
                    lbl_1_bss_1B0[1][0] = 1;
                    fn_1_FDF8(&lbl_1_data_28[3], 0x4A5);
                }
                break;
            case 2:
                if (lbl_1_bss_1B0[2][0] == 0 && motionTime > 10.0f) {
                    lbl_1_bss_1B0[2][0] = 1;
                    fn_1_FDF8(&lbl_1_data_28[5], 0x4A6);
                }
                break;
            case 3:
                if (lbl_1_bss_1B0[3][0] == 0 && motionTime > 10.0f) {
                    lbl_1_bss_1B0[3][0] = 1;
                    fn_1_FDF8(&lbl_1_data_28[0], 0x4A9);
                }
                if (lbl_1_bss_1B0[3][1] == 0 && motionTime > 50.0f) {
                    lbl_1_bss_1B0[3][1] = 1;
                    fn_1_FDF8(&lbl_1_data_28[0], 0x4A9);
                }
                if (lbl_1_bss_1B0[3][2] == 0 && motionTime > 130.0f) {
                    lbl_1_bss_1B0[3][2] = 1;
                    fn_1_FDF8(&lbl_1_data_28[0], 0x4A9);
                }
                break;
            case 4:
                if (lbl_1_bss_1B0[4][0] == 0 && motionTime > 10.0f) {
                    lbl_1_bss_1B0[4][0] = 1;
                    fn_1_FDF8(&lbl_1_data_28[2], 0x4A7);
                }
                if (lbl_1_bss_1B0[4][1] == 0 && motionTime > 90.0f) {
                    lbl_1_bss_1B0[4][1] = 1;
                    fn_1_FDF8(&lbl_1_data_28[2], 0x4A8);
                }
                break;
            case 5:
                if (lbl_1_bss_1B0[5][0] == 0 && motionTime > 5.0f
                    && motionTime < 195.0f) {
                    lbl_1_bss_1B0[5][0] = 1;
                    lbl_1_bss_1A28 = HuAudFXPlay(0x4AA);
                }
                if (lbl_1_bss_1B0[5][0] == 1
                    && (motionTime < 5.0f || motionTime > 195.0f)) {
                    lbl_1_bss_1B0[5][0] = 0;
                    HuAudFXStop(lbl_1_bss_1A28);
                }
                break;
        }
    }
    for (i = 0; i < 6; i++) {
        if (i != lbl_1_bss_1A30[0] || lbl_1_bss_3E != 1) {
            Hu3DModelAttrReset(obj->mdlId[i], HU3D_MOTATTR_LOOP);
        }
    }
    for (i = 0; i < 6; i++) {
        if (rand8() == 32) {
            Hu3DModelAttrSet(obj->mdlId[i], HU3D_MOTATTR_LOOP);
            Hu3DModelAttrReset(obj->mdlId[i], HU3D_MOTATTR_PAUSE);
            break;
        }
    }
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

inline void fn_1_77D8(OMOBJ *obj);

void fn_1_7868(OMOBJ *obj)
{
    MDSEL_BEZIER_WORK *work = &lbl_1_bss_128;
    HuVecF pos;
    HuVecF modelPos;
    HuVecF modelRot;
    HuVecF scale;
    float modelScale;

    Hu3DModelPosGet(obj->mdlId[0], &modelPos);
    Hu3DModelRotGet(obj->mdlId[0], &modelRot);
    modelScale = fn_1_50C(
        5.0f, 2.0f, work->time - (work->duration / 2.0f),
        work->duration / 2.0f);
    modelScale = fn_1_50C(5.0f, 0.25f, work->time, work->duration);
    Hu3DModelScaleSet(obj->mdlId[0], modelScale, modelScale, modelScale);
    fn_1_A20(
        &pos, &work->control[0], &work->control[1], &work->control[2],
        fn_1_50C(0.0f, 1.0f, work->time, work->duration));
    Hu3DModelPosSetV(obj->mdlId[0], &pos);
    modelPos.x -= pos.x;
    modelPos.z -= pos.z;
    modelPos.y = -(180.0 * (atan2(modelPos.x, -modelPos.z) / M_PI));
    if (modelRot.y - modelPos.y > 180.0f) {
        modelRot.y -= 360.0f;
    } else if (modelRot.y - modelPos.y < -180.0f) {
        modelRot.y += 360.0f;
    }
    modelRot.y = fn_1_550(modelRot.y, modelPos.y, 15.0f);
    Hu3DModelRotSetV(obj->mdlId[0], &modelRot);
    Hu3DModelPosGet(obj->mdlId[0], &pos);
    Hu3DModelScaleGet(obj->mdlId[0], &scale);
    fn_1_FA9C(4, &pos, 1, 5);
    if (work->time == 45.0f) {
        HuAudFXStop(lbl_1_bss_19DC[0]);
        lbl_1_bss_19DC[2] = HuAudFXPlay(0x47D);
    }
    if (++work->time > work->duration) {
        HuAudFXStop(lbl_1_bss_19DC[2]);
        fn_1_FA9C(4, &pos, 0, 5);
        obj->objFunc = NULL;
    }
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

void fn_1_809C(OMOBJ *obj)
{
    MDSEL_BEZIER_WORK *work = &lbl_1_bss_A0;
    HuVecF pos;
    HuVecF modelPos;
    HuVecF modelRot;
    HuVecF scale;
    float modelScale;

    Hu3DModelPosGet(obj->mdlId[0], &modelPos);
    Hu3DModelRotGet(obj->mdlId[0], &modelRot);
    modelScale = fn_1_50C(
        5.0f, 2.0f, work->time - (work->duration / 2.0f),
        work->duration / 2.0f);
    modelScale = fn_1_50C(5.0f, 0.25f, work->time, work->duration);
    Hu3DModelScaleSet(obj->mdlId[0], modelScale, modelScale, modelScale);
    fn_1_A20(
        &pos, &work->control[0], &work->control[1], &work->control[2],
        fn_1_50C(0.0f, 1.0f, work->time, work->duration));
    Hu3DModelPosSetV(obj->mdlId[0], &pos);
    modelPos.x -= pos.x;
    modelPos.z -= pos.z;
    modelPos.y = -(180.0 * (atan2(modelPos.x, -modelPos.z) / M_PI));
    if (modelRot.y - modelPos.y > 180.0f) {
        modelRot.y -= 360.0f;
    } else if (modelRot.y - modelPos.y < -180.0f) {
        modelRot.y += 360.0f;
    }
    modelRot.y = fn_1_550(modelRot.y, modelPos.y, 15.0f);
    Hu3DModelRotSetV(obj->mdlId[0], &modelRot);
    Hu3DModelPosGet(obj->mdlId[0], &pos);
    Hu3DModelScaleGet(obj->mdlId[0], &scale);
    fn_1_FA9C(5, &pos, 1, 6);
    if (work->time == 45.0f) {
        HuAudFXStop(lbl_1_bss_19DC[1]);
        lbl_1_bss_19DC[3] = HuAudFXPlay(0x47B);
    }
    if (++work->time > work->duration) {
        HuAudFXStop(lbl_1_bss_19DC[3]);
        fn_1_FA9C(5, &pos, 0, 6);
        obj->objFunc = NULL;
    }
}

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

inline void fn_1_8A58(OMOBJ *obj);

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

inline void fn_1_8CA4(OMOBJ *obj);

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

void fn_1_92BC(OMOBJ *obj)
{
    HuVecF pos;

    Hu3DModelPosGet(obj->mdlId[0], &pos);
    fn_1_598(&pos, &lbl_1_bss_94, 3.0f);
    Hu3DModelPosSetV(obj->mdlId[0], &pos);
    Hu3DModelRotGet(obj->mdlId[0], &pos);
    pos.z = fn_1_550(pos.z, 0.0f, 5.0f);
    Hu3DModelRotSetV(obj->mdlId[0], &pos);
    Hu3DModelScaleGet(obj->mdlId[0], &pos);
    pos.x = pos.y = pos.z = fn_1_550(pos.x, 1.5f, 3.0f);
    Hu3DModelScaleSetV(obj->mdlId[0], &pos);
    fn_1_8EC8();
}

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

inline void fn_1_98B0(OMOBJ *obj);

void fn_1_9910(void)
{
    fn_1_702C(lbl_1_bss_C);
    fn_1_77D8(lbl_1_bss_10);
    fn_1_8A58(lbl_1_bss_14);
    fn_1_8CA4(lbl_1_bss_18);
    fn_1_98B0(lbl_1_bss_1C);
    fn_1_FA18();
    fn_1_6DFC();
    fn_1_2828();
    fn_1_1DDC();
    fn_1_1988();
    fn_1_1734();
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

inline s16 fn_1_B804(void);

s16 fn_1_BAB4(void)
{
    s16 index;
    s16 inputDelay;
    s16 result;
    s16 previousColumn;

    inputDelay = 10;
    result = 0;
    previousColumn = 1;

restart:
    HuPrcVSleep();
    index = lbl_1_bss_1A30[1] + (3 * lbl_1_bss_1A30[2]);
    lbl_1_bss_1A30[0] = lbl_1_data_70[index];
    fn_1_FEC(index);
    fn_1_9160(&lbl_1_data_28[index]);
    fn_1_25F8(0x10002);
    fn_1_23E0(1, 0xA0002 + lbl_1_bss_1A30[0], 0);
    fn_1_8D44();
    lbl_1_bss_3E = 1;

    for (;;) {
        HuPrcVSleep();
        fn_1_75A4(0);
        index = lbl_1_bss_1A30[1] + (3 * lbl_1_bss_1A30[2]);
        lbl_1_bss_1A30[0] = lbl_1_data_70[index];
        fn_1_FEC(index);
        fn_1_90FC(&lbl_1_data_28[index]);
        fn_1_8D44();

        if (lbl_1_bss_1A30[1] == 0) {
            if (previousColumn != lbl_1_bss_1A30[1]) {
                previousColumn = lbl_1_bss_1A30[1];
                Hu3DMotionShiftSet(lbl_1_bss_14->mdlId[0],
                    lbl_1_bss_14->mtnId[3], 0.0f, 5.0f,
                    HU3D_MOTATTR_LOOP);
                Hu3DMotionShiftSet(lbl_1_bss_18->mdlId[0],
                    lbl_1_bss_18->mtnId[2], 0.0f, 15.0f,
                    HU3D_MOTATTR_LOOP);
                Hu3DMotionShiftStartEndSet(
                    lbl_1_bss_18->mdlId[0], 20.0f, 100.0f);
            }
            fn_1_23E0(1, 0xA0002 + lbl_1_bss_1A30[0], 0);
        } else if (lbl_1_bss_1A30[1] == 2) {
            if (previousColumn != lbl_1_bss_1A30[1]) {
                previousColumn = lbl_1_bss_1A30[1];
                Hu3DMotionShiftSet(lbl_1_bss_14->mdlId[0],
                    lbl_1_bss_14->mtnId[2], 0.0f, 15.0f,
                    HU3D_MOTATTR_LOOP);
                Hu3DMotionShiftSet(lbl_1_bss_18->mdlId[0],
                    lbl_1_bss_18->mtnId[3], 0.0f, 5.0f,
                    HU3D_MOTATTR_LOOP);
                Hu3DMotionShiftStartEndSet(
                    lbl_1_bss_14->mdlId[0], 20.0f, 100.0f);
            }
            fn_1_23E0(1, 0xA0002 + lbl_1_bss_1A30[0], 0);
        } else {
            if (previousColumn != lbl_1_bss_1A30[1]) {
                previousColumn = lbl_1_bss_1A30[1];
                Hu3DMotionShiftSet(lbl_1_bss_14->mdlId[0],
                    lbl_1_bss_14->mtnId[0], 0.0f, 15.0f,
                    HU3D_MOTATTR_LOOP);
                Hu3DMotionShiftSet(lbl_1_bss_18->mdlId[0],
                    lbl_1_bss_18->mtnId[0], 0.0f, 15.0f,
                    HU3D_MOTATTR_LOOP);
            }
            fn_1_23E0(1, 0xA0002 + lbl_1_bss_1A30[0], 0);
        }

        if (++inputDelay < 5) {
            continue;
        }
        inputDelay = 11;

        if (HuPadDStkRep[0] & PAD_BUTTON_LEFT) {
            if (lbl_1_bss_1A30[1] > 0) {
                HuAudFXPlay(0);
                lbl_1_bss_1A30[1]--;
                inputDelay = 0;
            } else if (lbl_1_bss_1A30[2] == 0) {
                HuAudFXPlay(0);
                lbl_1_bss_1A30[2] = 1;
                inputDelay = 0;
            }
        } else if (HuPadDStkRep[0] & PAD_BUTTON_RIGHT) {
            if (lbl_1_bss_1A30[1] < 2) {
                HuAudFXPlay(0);
                lbl_1_bss_1A30[1]++;
                inputDelay = 0;
            } else if (lbl_1_bss_1A30[2] == 0) {
                HuAudFXPlay(0);
                lbl_1_bss_1A30[2] = 1;
                inputDelay = 0;
            }
        } else if (HuPadDStkRep[0] & PAD_BUTTON_DOWN) {
            if (lbl_1_bss_1A30[2] < 1) {
                HuAudFXPlay(0);
                lbl_1_bss_1A30[2]++;
                inputDelay = 0;
            }
        } else if (HuPadDStkRep[0] & PAD_BUTTON_UP) {
            if (lbl_1_bss_1A30[2] > 0) {
                HuAudFXPlay(0);
                lbl_1_bss_1A30[2]--;
                inputDelay = 0;
            }
        } else if (HuPadBtnDown[0] & PAD_BUTTON_A) {
            if (lbl_1_bss_1A30[0] == 0 || lbl_1_bss_1A30[0] == 1) {
                if (GwCommon.mic == 1 && !fn_1_6E54()) {
                    OSReport(lbl_1_data_308);
                    HuMCMicSet(0);
                }
                HuAudFXPlay(2);
                if (lbl_1_bss_1A30[0] == 0 || lbl_1_bss_1A30[0] == 5) {
                    HuAudFXPlayPan(0x3B6, 0x30);
                    HuAudFXPlayPan(0x3AE, 0x50);
                } else if (lbl_1_bss_1A30[0] == 1
                    || lbl_1_bss_1A30[0] == 3) {
                    HuAudFXPlay(0x3B6);
                } else {
                    HuAudFXPlay(0x3AE);
                }
                result = 1;
                break;
            }

            if (lbl_1_bss_1A30[0] == 3) {
                if (GwCommon.mic == 2) {
                    HuAudFXPlay(2);
                    HuAudFXPlay(0x3B6);
                    result = 1;
                    break;
                }
                if (GwCommon.mic == 0) {
                    HuAudFXPlay(4);
                    fn_1_23E0(1, 0xA0009, 1);
                    fn_1_2288();
                    fn_1_21DC();
                } else if (!fn_1_6E54()) {
                    OSReport(lbl_1_data_333);
                    HuAudFXPlay(4);
                    fn_1_23E0(1, 0xA0008, 1);
                    fn_1_2288();
                    fn_1_21DC();
                } else {
                    HuAudFXPlay(2);
                    HuAudFXPlay(0x3B6);
                    result = 1;
                    break;
                }
            } else {
                HuAudFXPlay(2);
                if (lbl_1_bss_1A30[0] == 0 || lbl_1_bss_1A30[0] == 5) {
                    HuAudFXPlayPan(0x3B6, 0x30);
                    HuAudFXPlayPan(0x3AE, 0x50);
                } else if (lbl_1_bss_1A30[0] == 1
                    || lbl_1_bss_1A30[0] == 3) {
                    HuAudFXPlay(0x3B6);
                } else {
                    HuAudFXPlay(0x3AE);
                }
                result = 1;
                break;
            }
        } else if (HuPadBtnDown[0] & PAD_BUTTON_B) {
            HuAudFXPlay(3);
            result = -1;
            break;
        }

        index = lbl_1_bss_1A30[1] + (3 * lbl_1_bss_1A30[2]);
        lbl_1_bss_1A30[0] = lbl_1_data_70[index];
        fn_1_8D44();
    }

    lbl_1_bss_3E = 0;
    if (result == -1) {
        previousColumn = -1;
        Hu3DMotionShiftSet(lbl_1_bss_14->mdlId[0],
            lbl_1_bss_14->mtnId[0], 0.0f, 30.0f, HU3D_MOTATTR_LOOP);
        Hu3DMotionShiftSet(lbl_1_bss_18->mdlId[0],
            lbl_1_bss_18->mtnId[0], 0.0f, 30.0f, HU3D_MOTATTR_LOOP);
        fn_1_75A4(-1);
    }
    fn_1_FEC(-1);
    fn_1_927C();
    fn_1_8E38();
    fn_1_277C();

    if (result == -1) {
        if (fn_1_B804()) {
            goto restart;
        }
        result = -1;
    }
    return result;
}

void fn_1_E1FC(void)
{
    MDSEL_BEZIER_WORK *workA;
    MDSEL_BEZIER_WORK *workB;
    OMOBJ *objA;
    OMOBJ *objB;
    s16 indexA;
    s16 indexB;
    s16 windowNo;
    float yA;
    float yB;

    lbl_1_data_15C = 0;
    HuPrcVSleep(60);
    if (lbl_1_data_150[0] != -1) {
        windowNo = lbl_1_data_150[0];
        if (windowNo == 0) {
            HuWinDispOff(lbl_1_bss_1A40[windowNo]);
        } else {
            HuWinExClose(lbl_1_bss_1A40[windowNo]);
        }
    }
    lbl_1_data_150[0] = -1;
    lbl_1_data_154[0] = -1;

    objA = lbl_1_bss_14;
    workA = &lbl_1_bss_128;
    if (lbl_1_bss_1A30[1] != 2) {
        yA = 200.0f;
        if (lbl_1_bss_1A30[0] == 5) {
            yA = 400.0f;
        }
        indexA = lbl_1_bss_1A30[1] + (3 * lbl_1_bss_1A30[2]);
        workA->control[0].x = -1150.0f;
        workA->control[0].y = 350.0f;
        workA->control[0].z = -1000.0f;
        workA->control[1].x = -lbl_1_data_28[indexA].x;
        workA->control[1].y = 2250.0f;
        workA->control[1].z = 6000.0f;
        workA->control[2].x = lbl_1_data_28[indexA].x;
        workA->control[2].y = yA;
        workA->control[2].z = lbl_1_data_28[indexA].z;
        workA->time = 0.0f;
        workA->duration = 90.0f;
        Hu3DMotionShiftSet(
            objA->mdlId[0], objA->mtnId[1], 0.0f, 5.0f, 0);
        lbl_1_bss_19DC[0] = HuAudFXPlay(0x47E);
        objA->objFunc = fn_1_7868;
    }

    HuPrcSleep(10);
    objB = lbl_1_bss_18;
    workB = &lbl_1_bss_A0;
    if (lbl_1_bss_1A30[1] != 0) {
        yB = 100.0f;
        if (lbl_1_bss_1A30[0] == 5) {
            yB = 400.0f;
        }
        indexB = lbl_1_bss_1A30[1] + (3 * lbl_1_bss_1A30[2]);
        workB->control[0].x = 1150.0f;
        workB->control[0].y = 350.0f;
        workB->control[0].z = -1000.0f;
        workB->control[1].x = -lbl_1_data_28[indexB].x;
        workB->control[1].y = 2250.0f;
        workB->control[1].z = 6000.0f;
        workB->control[2].x = lbl_1_data_28[indexB].x;
        workB->control[2].y = yB;
        workB->control[2].z = lbl_1_data_28[indexB].z;
        workB->time = 0.0f;
        workB->duration = 90.0f;
        Hu3DMotionShiftSet(
            objB->mdlId[0], objB->mtnId[1], 0.0f, 5.0f, 0);
        lbl_1_bss_19DC[1] = HuAudFXPlay(0x47C);
        objB->objFunc = fn_1_809C;
    }

    HuPrcSleep(80);
    fn_1_11D4();
    HuPrcSleep(50);
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

void fn_1_EF48(HU3D_MODEL *model, HU3D_PARTICLE *particle, Mtx matrix)
{
    HU3D_PARTICLE_DATA *data;
    s16 i;
    s16 spawnCount = 0;
    float color;
    float random;

    if (particle->count == 0) {
        for (i = 0, data = particle->data; i < particle->maxCnt; i++, data++) {
            data->time = 0;
        }
        particle->dataCnt = 1;
        particle->pos.x = 255.0f;
        particle->pos.y = 255.0f;
        particle->pos.z = 255.0f;
    }

    for (i = 0, data = particle->data; i < particle->maxCnt; i++, data++) {
        if (data->time == 0 && particle->dataCnt == 1 && spawnCount < 1) {
            spawnCount++;
            data->time = 1;
            data->vel.x = 0.0f;
            data->vel.y = frandmod(30) + 30;
            data->accel.x = frandmod(100) - 50;
            data->accel.y = -frandmod(100) - 50;
            data->accel.z = frandmod(100) - 50;
            PSVECNormalize(&data->accel, &data->accel);
            data->accel.x *= 2.0f;
            data->accel.z *= 2.0f;
            data->colorIdx = frandmod(10) + 5;
            data->pos.x = particle->unk_10.x + 3 * (frandmod(100) - 50);
            data->pos.y = particle->unk_10.y;
            data->pos.z = particle->unk_10.z + 3 * (frandmod(100) - 50);
            random = frandmod(32);
            color = particle->pos.x + random;
            if (color > 255.0f) {
                color = 255.0f;
            }
            data->color.r = color;
            color = particle->pos.y + random;
            if (color > 255.0f) {
                color = 255.0f;
            }
            data->color.g = color;
            color = particle->pos.z + random;
            if (color > 255.0f) {
                color = 255.0f;
            }
            data->color.b = color;
            data->color.a = 0;
        } else if (data->time == 1) {
            data->pos.y += data->colorIdx;
            data->colorIdx += data->accel.y;
            if (rand8() % 5 == 0) {
                data->zRot = MTXDegToRad(frandmod(360));
                data->color.a = frandmod(127) + 128;
            }
            random = fn_1_C48(1.0f, 0.0f, data->vel.x, data->vel.y);
            data->scale = 100.0f * random;
            if (++data->vel.x > data->vel.y) {
                data->time = 0;
                data->scale = 0.0f;
                data->color.a = 0;
            }
        }
    }
    DCFlushRangeNoSync(
        particle->data, particle->maxCnt * sizeof(HU3D_PARTICLE_DATA));
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

void fn_1_F790(void)
{
    s16 i;

    for (i = 0; i < 5; i++) {
        lbl_1_bss_80[i] = HuSprAnimRead(
            HuDataSelHeapReadNum(lbl_1_data_35C[i], HU_MEMNUM_OVL, HEAP_MODEL));
    }
    fn_1_F4E4();
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

s16 lbl_1_bss_1A4C;
HU3D_LIGHTID lbl_1_bss_1A48[2];
HUWINID lbl_1_bss_1A40[4];
ANIMDATA *lbl_1_bss_1A3C[1];
HUSPR_GROUPID lbl_1_bss_1A38[1];
HUSPRID lbl_1_bss_1A36[1];
s16 lbl_1_bss_1A30[3];
s32 lbl_1_bss_1A2C;
s32 lbl_1_bss_1A28;
s16 lbl_1_bss_19EC[30];
s32 lbl_1_bss_19DC[4];
HuVecF lbl_1_bss_19AC[4];
LBL_1_BSS_8AC_ENTRY lbl_1_bss_9BC[30];
LBL_1_BSS_8AC_ENTRY lbl_1_bss_8AC[2];
LBL_1_BSS_24C_ENTRY lbl_1_bss_24C[12];
LBL_1_BSS_1D4_ENTRY lbl_1_bss_1D4[15];
s16 lbl_1_bss_1B0[6][3];
MDSEL_BEZIER_WORK lbl_1_bss_128;
MDSEL_BEZIER_WORK lbl_1_bss_A0;
HuVecF lbl_1_bss_94;
ANIMDATA *lbl_1_bss_80[5];
HU3D_MODELID lbl_1_bss_44[6][5];
float lbl_1_bss_40;
s16 lbl_1_bss_3E;
s16 lbl_1_bss_3C;
s16 lbl_1_bss_3A;
s16 lbl_1_bss_38;
OMOBJ *lbl_1_bss_34;
OMOBJ *lbl_1_bss_30;
float lbl_1_bss_2C;
void *lbl_1_bss_28;
OMOBJ *lbl_1_bss_20[2];
OMOBJ *lbl_1_bss_1C;
OMOBJ *lbl_1_bss_18;
OMOBJ *lbl_1_bss_14;
OMOBJ *lbl_1_bss_10;
OMOBJ *lbl_1_bss_C;
OMOBJMAN *lbl_1_bss_8;
int lbl_1_bss_4;
int lbl_1_bss_0;
