#include <string.h>

#include "datadir_enum.h"

#include "dolphin/os.h"

#include "game/armem.h"
#include "game/charman.h"
#include "game/flag.h"
#include "game/gamework.h"
#include "game/hu3d.h"
#include "game/object.h"
#include "game/pad.h"
#include "game/sprite.h"
#include "game/window.h"
#include "game/wipe.h"

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
    float unk_40;
    u8 unk_44[0xC];
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

typedef struct Lbl1DataE12Entry {
    u8 unk_0[0x10];
    s16 unk_10;
} LBL_1_DATA_E12_ENTRY;

typedef struct Lbl1Bss288Entry {
    HU3D_MODELID modelId;
    HU3D_ANIMID animId[4];
    s16 unk_A;
    HuVecF pos;
    HuVecF rot;
    HuVecF scale;
    s16 unk_30;
    u8 unk_32[6];
} LBL_1_BSS_288_ENTRY;

typedef struct Lbl1Bss1C {
    OMOBJ *obj;
    u8 unk_4[4];
} LBL_1_BSS_1C;

typedef struct Lbl1Bss228Entry {
    s16 state;
    s16 pad;
    HuVecF pos;
    HuVecF unk_10;
    HuVecF unk_1C;
    float time;
    float duration;
} LBL_1_BSS_228_ENTRY;

typedef struct Lbl1Bss1C8Entry {
    s16 state;
    s16 pad;
    HuVecF pos;
    HuVecF unk_10;
    HuVecF unk_1C;
    float time;
    float duration;
} LBL_1_BSS_1C8_ENTRY;

typedef struct Lbl1Bss9BCEntry {
    s16 unk_0;
    s16 unk_2;
    s16 unk_4;
    s16 unk_6;
    s16 chrSel;
    s16 unk_A;
    s16 unk_C;
} LBL_1_BSS_9BC_ENTRY;

typedef struct Lbl1Bss9F4 {
    s16 unk_0;
    s16 unk_2;
    s16 unk_4;
    s16 unk_6;
    s16 unk_8;
    s16 unk_A;
    s16 unk_C;
    s16 unk_E;
    s32 unk_10;
    s32 unk_14;
    s16 unk_18;
    s16 unk_1A;
} LBL_1_BSS_9F4;

extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];

extern void HuDataDirCloseAll(void);
extern int HuAudFXPlay(int seId);
extern int HuAudFXPlayPan(int seId, int pan);
extern int HuAudSStreamPlay(s16 streamId);

extern s16 lbl_1_bss_28;
extern s16 lbl_1_bss_2A;
extern s16 lbl_1_bss_2C;
extern s16 lbl_1_bss_2E;
extern s16 lbl_1_bss_34;
extern float lbl_1_bss_38[6];
extern HuVecF lbl_1_bss_50[4];
extern HuVecF lbl_1_bss_80;
extern HuVecF lbl_1_bss_8C[2];
extern HuVecF lbl_1_bss_A4[4];
extern HuVecF lbl_1_bss_D4[4];
extern OMOBJMAN *lbl_1_bss_0;
extern OMOBJ *lbl_1_bss_4;
extern OMOBJ *lbl_1_bss_8;
extern OMOBJ *lbl_1_bss_C;
extern OMOBJ *lbl_1_bss_10;
extern OMOBJ *lbl_1_bss_14;
extern OMOBJ *lbl_1_bss_18;
extern LBL_1_BSS_1C lbl_1_bss_1C;
extern OMOBJ *lbl_1_bss_24;
extern OMOBJ *lbl_1_bss_30;
extern HUWINID lbl_1_bss_104[2];
extern LBL_1_BSS_1C8_ENTRY lbl_1_bss_108[4];
extern LBL_1_BSS_1C8_ENTRY lbl_1_bss_1C8[2];
extern LBL_1_BSS_228_ENTRY lbl_1_bss_228[2];
extern LBL_1_BSS_288_ENTRY lbl_1_bss_288[25];
extern ANIMDATA *lbl_1_bss_800[41];
extern HUSPRID lbl_1_bss_8A4[58];
extern HUSPR_GROUPID lbl_1_bss_918[18];
extern ANIMDATA *lbl_1_bss_93C[32];
extern LBL_1_BSS_9BC_ENTRY lbl_1_bss_9BC[4];
extern LBL_1_BSS_9F4 lbl_1_bss_9F4;
extern MDCAMERA_WORK lbl_1_bss_A10;
extern HUWINID lbl_1_bss_A60[4];
extern HU3D_LIGHTID lbl_1_bss_A68[2];
extern s32 lbl_1_bss_A6C[5];

extern s32 lbl_1_data_0;
extern s32 lbl_1_data_BC8;
extern s16 lbl_1_data_4[3][4];
extern u32 lbl_1_data_1C[32];
extern s16 lbl_1_data_9C[18];
extern LBL_1_DATA_C0_ENTRY lbl_1_data_C0[58];
extern u32 lbl_1_data_800[41];
extern HuVecF lbl_1_data_8A4[67];
extern char lbl_1_data_BCC[];
extern s16 lbl_1_data_BEE[7];
extern u32 lbl_1_data_BFC[14];
extern char lbl_1_data_C34[];
extern char lbl_1_data_C66[];
extern char lbl_1_data_C77[];
extern char lbl_1_data_C89[];
extern char lbl_1_data_C9B[];
extern char lbl_1_data_CA9[];
extern char lbl_1_data_CAB[];
extern char lbl_1_data_CDC[];
extern char lbl_1_data_D0C[];
extern char lbl_1_data_DB4[];
extern char lbl_1_data_DC6[];
extern char lbl_1_data_DD8[];
extern char lbl_1_data_DDF[];
extern char lbl_1_data_DF2[];
extern char lbl_1_data_DFA[];
extern char lbl_1_data_E02[];
extern char lbl_1_data_E0A[];
extern LBL_1_DATA_E12_ENTRY lbl_1_data_E12[11];
extern s16 lbl_1_data_DA2[3];
extern s32 lbl_1_data_DA8[3];
extern s16 lbl_1_data_DE6[6];
extern char lbl_1_data_ED8[];

void fn_1_0(HUWINID winId, u32 mess, s16 index);
void fn_1_1B4(void);
void fn_1_2B0(void);
void fn_1_400(void);
void fn_1_64C(void);
void fn_1_718(void);
void fn_1_E30(void);
void fn_1_353C(void);
void fn_1_4581C(void);
void fn_1_6284(OMOBJ *obj);
void fn_1_72C4(OMOBJ *obj);
void fn_1_109C(s16 arg0);
void fn_1_1248(void);
void fn_1_12F8(void);
void fn_1_2D1D4(void);
void fn_1_2DD38(void);
void fn_1_2DFD4(OMOBJ *obj, MDCAMERA_WORK *camera);
void fn_1_2E338(OMOBJ *obj, MDCAMERA_WORK *camera);
void fn_1_2E560(OMOBJ *obj, MDCAMERA_WORK *camera);
s32 fn_1_2E5D4(void);
s16 fn_1_32904(void);
s16 fn_1_329A4(s16 arg0);
s16 fn_1_335F0(s16 *arg0, s16 *arg1, s16 *arg2);
s16 fn_1_33724(s16 arg0);
s16 fn_1_34D10(s16 arg0);
s16 fn_1_34FC4(s16 *arg0, s16 *arg1, s16 *arg2);
s16 fn_1_350F8(s16 arg0);
s16 fn_1_36264(s16 arg0);
void fn_1_37260(s16 *arg0, s16 *arg1, s16 *arg2);
void ObjectSetup(void);
void fn_1_32D0(OMOBJ *obj);
void fn_1_467C(void);
void fn_1_4A84(OMOBJ *obj);
void fn_1_4EDC(OMOBJ *obj);
void fn_1_50EC(OMOBJ *obj);
void fn_1_5208(OMOBJ *obj);
void fn_1_5324(OMOBJ *obj);
void fn_1_5A18(OMOBJ *obj);
void fn_1_6324(OMOBJ *obj);
void fn_1_6A20(OMOBJ *obj);
void fn_1_737C(OMOBJ *obj);
void fn_1_605C(OMOBJ *obj);
void fn_1_7064(OMOBJ *obj);
void fn_1_9658(s16 winNo, u32 messNum);
void fn_1_A8D8(s16 memberNo, HuVecF *pos, float xOffset, float yOffset);
void fn_1_A970(s16 memberNo, HuVecF *pos, float xOffset, float yOffset);
void fn_1_AA94(s16 memberNo);
void fn_1_AAF0(s16 memberNo);
void fn_1_AD14(s16 modelNo, HuVecF *pos);
void fn_1_7ED4(OMOBJ *obj);
void fn_1_861C(OMOBJ *obj);
void fn_1_9218(OMOBJ *obj);
void fn_1_9F50(OMOBJ *obj);
void fn_1_A5D8(OMOBJ *obj);
void fn_1_B008(OMOBJ *obj);
void fn_1_B4C8(OMOBJ *obj);
s16 fn_1_B748(
    s16 arg0, s16 arg1, s16 arg2, s16 arg3, s16 *arg4, s16 arg5);
void fn_1_D748(void);
void fn_1_10518(OMOBJ *obj);
void fn_1_12364(OMOBJ *obj);
void fn_1_12B18(OMOBJ *obj);
void fn_1_12D7C(OMOBJ *obj);
void fn_1_12EC8(OMOBJ *obj);
void fn_1_14344(OMOBJ *obj);
void fn_1_14F64(OMOBJ *obj);
void fn_1_150D0(OMOBJ *obj);
void fn_1_156B4(OMOBJ *obj);
void fn_1_160A0(OMOBJ *obj);
void fn_1_16A8C(OMOBJ *obj);
void fn_1_16E84(OMOBJ *obj);
void fn_1_1733C(s16 arg0);
void fn_1_17730(OMOBJ *obj);
void fn_1_17E10(OMOBJ *obj);
void fn_1_1807C(OMOBJ *obj);
void fn_1_21628(OMOBJ *obj);
void fn_1_219C8(OMOBJ *obj);
void fn_1_21C9C(OMOBJ *obj);
void fn_1_22010(OMOBJ *obj);
void fn_1_228E8(OMOBJ *obj);
void fn_1_22EB8(OMOBJ *obj);
void fn_1_23224(OMOBJ *obj);
void fn_1_23A00(OMOBJ *obj);
void fn_1_26328(OMOBJ *obj);
void fn_1_2C874(void);
void fn_1_2CE00(void);
void fn_1_2EBEC(void);
s16 fn_1_2EF24(void);
extern s16 fn_1_3EAC8(s16 arg0);
void fn_1_44DCC(OMOBJMAN *objman);
void fn_1_43F3C(s16 arg0, HuVecF *arg1, s16 arg2);
void fn_1_45F3C(s16 arg0, HuVecF *arg1, s16 arg2, s16 arg3);

extern void mbSaveInit(s32 boardNo);
extern void mbSavePartyInit(
    s32, s32, s32, s32, s32, s32, s32, s32);

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

void fn_1_1B4(void)
{
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
}

inline void fn_1_1B4(void);

void fn_1_2B0(void)
{
    s16 i;
    s32 status;

    for (i = 0; i < 4; i++) {
        if (CharMotionAMemPGet(GwPlayer[i].charNo) == 0) {
            break;
        }
    }
    if (i != 4) {
        CharDataClose(-1);
        for (i = 0; i < 4; i++) {
            status = HuDataDirReadAsync(
                CharDataDirTbl[GwPlayer[i].charNo][4]);
            if (status != -1) {
                while (!HuDataGetAsyncStat(status)) {
                    HuPrcVSleep();
                }
            }
            CharMotionInit(GwPlayer[i].charNo);
            HuDataDirClose(CharDataDirTbl[GwPlayer[i].charNo][4]);
        }
    }
}

inline void fn_1_2B0(void);

void fn_1_400(void)
{
    s32 dataDir[2] = { DATA_board_us, DATA_capsule };
    s16 i;
    s32 status;

    fn_1_2B0();
    for (i = 0; i < 2; i++) {
        status = HuDataDirReadAsync(dataDir[i]);
        if (status != -1) {
            while (!HuDataGetAsyncStat(status)) {
                HuPrcVSleep();
            }
        }
        HuAR_MRAMtoARAM(dataDir[i]);
        while (HuARDMACheck()) {
            HuPrcVSleep();
        }
        HuDataDirClose(dataDir[i]);
    }
    status = HuDataDirReadAsync(lbl_1_data_BFC[GwSystem.boardNo]);
    if (status != -1) {
        while (!HuDataGetAsyncStat(status)) {
            HuPrcVSleep();
        }
    }
    lbl_1_bss_2E = TRUE;
    HuPrcEnd();
    while (TRUE) {
        HuPrcVSleep();
    }
}

void fn_1_64C(void)
{
    lbl_1_bss_2E = FALSE;
    OSReport(lbl_1_data_C34);
    OSReport(lbl_1_data_C66, 0x21);
    OSReport(lbl_1_data_C77, 0x24);
    OSReport(lbl_1_data_C89, 0x9B);
    OSReport(lbl_1_data_C9B, 0xF2);
    HuAMemDump();
    OSReport(lbl_1_data_CA9);
    HuDataDirClose(DATA_mdparty);
    HuPrcChildCreate(fn_1_400, 0x100, 0x4000, 0, lbl_1_bss_0);
}

void fn_1_718(void)
{
    s16 i;

    lbl_1_bss_9F4.unk_A++;
    lbl_1_bss_9F4.unk_A %= 2;
    GwSystem.tagF = lbl_1_bss_9F4.unk_0;
    GwSystem.bonusStarF = lbl_1_bss_9F4.unk_A;
    GwSystem.mgPack = lbl_1_bss_9F4.unk_8;
    GwSystem.turnNo = TRUE;
    GwSystem.turnMax = (lbl_1_bss_9F4.unk_6 * 5) + 10;
    GwSystem.boardNo = lbl_1_bss_9F4.unk_4;
    GwCommon.partyMgPack = GwSystem.mgPack;
    GwCommon.confTag = lbl_1_bss_9F4.unk_0;
    GwCommon.confTurnNum = (lbl_1_bss_9F4.unk_6 * 5) + 10;
    GwCommon.partyMgPack = lbl_1_bss_9F4.unk_8;
    GwCommon.confBonusStar = lbl_1_bss_9F4.unk_A;
    GwCommon.lastBoard = lbl_1_bss_9F4.unk_4;

    for (i = 0; i < 4; i++) {
        GwPlayer[i].comF = lbl_1_bss_9BC[i].unk_4;
        GwPlayer[i].comDif = lbl_1_bss_9BC[i].unk_6;
        GwPlayer[i].charNo = lbl_1_bss_9BC[i].chrSel;
        GwPlayer[i].padNo = lbl_1_bss_9BC[i].unk_A;
        GwPlayer[i].handicap = lbl_1_bss_9BC[i].unk_C;
        GwPlayer[i].team = FALSE;
    }
    if (GwSystem.tagF) {
        for (i = 0; i < 4; i++) {
            GwPlayer[i].handicap = 0;
        }
        GwPlayer[lbl_1_data_4[lbl_1_bss_9F4.unk_E][0]].handicap =
            lbl_1_bss_9BC[0].unk_C;
        GwPlayer[lbl_1_data_4[lbl_1_bss_9F4.unk_E][2]].handicap =
            lbl_1_bss_9BC[1].unk_C;
        GwPlayer[lbl_1_data_4[lbl_1_bss_9F4.unk_E][0]].team = FALSE;
        GwPlayer[lbl_1_data_4[lbl_1_bss_9F4.unk_E][1]].team = FALSE;
        GwPlayer[lbl_1_data_4[lbl_1_bss_9F4.unk_E][2]].team = TRUE;
        GwPlayer[lbl_1_data_4[lbl_1_bss_9F4.unk_E][3]].team = TRUE;
    }
    for (i = 0; i < 4; i++) {
        GwPlayerConf[i].grpNo = FALSE;
        GwPlayerConf[i].type = GwPlayer[i].comF;
        GwPlayerConf[i].comDif = GwPlayer[i].comDif;
        GwPlayerConf[i].charNo = GwPlayer[i].charNo;
        GwPlayerConf[i].padNo = GwPlayer[i].padNo;
        GwPlayerConf[i].grpNo = GwPlayer[i].team;
    }
    _ClearFlag(FLAG_BOARD_TUTORIAL);
    mbSavePartyInit(
        GwSystem.tagF, GwSystem.bonusStarF, GwSystem.mgPack,
        GwSystem.turnMax, GwPlayer[0].handicap, GwPlayer[1].handicap,
        GwPlayer[2].handicap, GwPlayer[3].handicap);
    mbSaveInit(GwSystem.boardNo);
}

void fn_1_E30(void)
{
    s16 i;

    for (i = 0; i < 4; i++) {
        GwPlayer[i].comF = TRUE;
        GwPlayer[i].comDif = 0;
        GwPlayer[i].charNo = i;
        GwPlayer[i].padNo = i;
        GwPlayer[i].handicap = 0;
        GwPlayer[i].team = FALSE;
    }
    for (i = 0; i < 4; i++) {
        GwPlayerConf[i].grpNo = FALSE;
        GwPlayerConf[i].type = GwPlayer[i].comF;
        GwPlayerConf[i].comDif = GwPlayer[i].comDif;
        GwPlayerConf[i].charNo = GwPlayer[i].charNo;
        GwPlayerConf[i].padNo = GwPlayer[i].padNo;
        GwPlayerConf[i].grpNo = GwPlayer[i].team;
    }
    mbSaveInit(9);
    GwSystem.boardNo = 6;
}

void fn_1_109C(s16 arg0)
{
    OMOVL boardDll[6] = {
        DLL_w01dll,
        DLL_w02dll,
        DLL_w03dll,
        DLL_w04dll,
        DLL_w05dll,
        DLL_w06dll,
    };

    do {
        HuPrcVSleep();
    } while (!lbl_1_bss_2E);
    OSReport(lbl_1_data_CAB);
    OSReport(lbl_1_data_C66, 0x21);
    OSReport(lbl_1_data_C77, 0x24);
    OSReport(lbl_1_data_C89, 0x9B);
    OSReport(lbl_1_data_C9B, 0xF2);
    HuAMemDump();
    OSReport(lbl_1_data_CA9);

    switch (arg0) {
        case 0: {
            OMOVLHIS *history = omOvlHisGet(0);

            omOvlHisChg(
                0, boardDll[GwSystem.boardNo], history->evtno,
                history->stat);
            omOvlGotoEx(boardDll[GwSystem.boardNo], TRUE, 0, 0);
            break;
        }
        case 1: {
            OMOVLHIS *history = omOvlHisGet(0);

            omOvlHisChg(0, history->ovl, 1, 0);
            omOvlCallEx(DLL_w10dll, TRUE, 0, 0);
            break;
        }
    }
}

inline void fn_1_109C(s16 arg0);

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

inline void fn_1_12F8(void);

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

inline float fn_1_160C(float arg0, float arg1, float arg2);

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

inline float fn_1_1868(float arg0, float arg1, float arg2, float arg3);

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

void fn_1_26FC(void)
{
    LBL_1_BSS_9BC_ENTRY *entry;
    s16 i;

    for (i = 0, entry = lbl_1_bss_9BC; i < 4; i++, entry++) {
        entry->unk_0 = 0;
        entry->unk_2 = 0;
        entry->unk_6 = 0;
        entry->chrSel = i;
        entry->unk_A = i;
    }
}

void fn_1_275C(void)
{
    LBL_1_BSS_9BC_ENTRY *entry;
    s16 playerCount = lbl_1_bss_9F4.unk_2;
    s16 i;

    lbl_1_bss_9BC[0].unk_4 = 0;
    playerCount--;
    for (i = 1, entry = &lbl_1_bss_9BC[1]; i < 4; i++, entry++) {
        if (playerCount > 0 && HuPadStatGet(i) == 0) {
            entry->unk_4 = 0;
            playerCount--;
        } else {
            entry->unk_4 = 1;
        }
    }
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

inline void fn_1_353C(void);

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

inline void fn_1_36C4(void);

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

void fn_1_39AC(s16 winNo, s32 messNum, s16 insertPos)
{
    HuWinHomeClear(lbl_1_bss_A60[winNo]);
    HuWinInsertMesSet(lbl_1_bss_A60[winNo], messNum, insertPos);
}

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

inline void fn_1_3C44(void);

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

inline void fn_1_3CA0(s16 winNo);

void fn_1_3E0C(void)
{
    if (lbl_1_data_DA2[0] != -1) {
        fn_1_3770(lbl_1_data_DA2[0]);
    }
    lbl_1_data_DA2[0] = -1;
    lbl_1_data_DA8[0] = -1;
    lbl_1_data_DA8[1] = -1;
}

inline void fn_1_3E0C(void);

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

inline s16 fn_1_3F28(s16 mode);

void fn_1_4020(s16 winNo, s32 messNum, s16 speed)
{
    fn_1_3CA0(winNo);
    if (lbl_1_data_DA8[0] != messNum) {
        lbl_1_data_DA8[0] = messNum;
        lbl_1_data_DA8[1] = -1;
        fn_1_38F0(lbl_1_data_DA2[0], lbl_1_data_DA8[0], speed);
    }
}

inline void fn_1_4020(s16 winNo, s32 messNum, s16 speed);

void fn_1_4258(s16 winNo, s32 messNum, s16 insertPos)
{
    s32 insertedMess;
    s16 activeWin;

    fn_1_3CA0(winNo);
    if (lbl_1_data_DA8[1] != messNum) {
        lbl_1_data_DA8[0] = -1;
        lbl_1_data_DA8[1] = messNum;
        insertedMess = lbl_1_data_DA8[1];
        activeWin = lbl_1_data_DA2[0];
        HuWinHomeClear(lbl_1_bss_A60[activeWin]);
        HuWinInsertMesSet(
            lbl_1_bss_A60[activeWin], insertedMess, insertPos);
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

void fn_1_4730(void)
{
}

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

void fn_1_4EDC(OMOBJ *obj)
{
    OMOBJ *obj0 = lbl_1_bss_C;
    OMOBJ *obj1 = lbl_1_bss_10;
    LBL_1_BSS_228_ENTRY *work0 = &lbl_1_bss_228[0];
    LBL_1_BSS_228_ENTRY *work1 = &lbl_1_bss_228[1];

    if (obj0 != NULL) {
        Hu3DModelObjPosGet(obj0->mdlId[0], lbl_1_data_DB4, &work0->pos);
        fn_1_43F3C(0, &work0->pos, 1);
        work0->time = 0.0f;
    }
    if (obj1 != NULL) {
        Hu3DModelObjPosGet(obj1->mdlId[0], lbl_1_data_DB4, &work1->pos);
        fn_1_43F3C(1, &work1->pos, 1);
        work1->time = 0.0f;
    }
    if (++obj->work[0] > 30) {
        obj->work[0] = 0;
        obj->objFunc = fn_1_4A84;
    }
}

void fn_1_4FE8(void)
{
    Hu3DMotionShiftSet(lbl_1_bss_C->mdlId[0], lbl_1_bss_C->mtnId[4],
        0.0f, 5.0f, 0);
    Hu3DMotionShiftSet(lbl_1_bss_10->mdlId[0], lbl_1_bss_10->mtnId[4],
        0.0f, 5.0f, 0);
    lbl_1_bss_30 =
        omAddObjEx(lbl_1_bss_0, 0x1000, 0x10, 0x10, -1, fn_1_4EDC);
    lbl_1_bss_30->work[0] = 0;
    HuAudFXPlay(0x49F);
    HuAudFXPlay(0x4A0);
}

void fn_1_50EC(OMOBJ *obj)
{
    Hu3DMotionSpeedSet(obj->mdlId[0], 2.0f);
    if (obj->work[3]++ > 30) {
        obj->objFunc = NULL;
        Hu3DMotionSpeedSet(obj->mdlId[0], 1.0f);
        Hu3DMotionShiftSet(obj->mdlId[0], obj->mtnId[0], 0.0f, 15.0f,
            HU3D_MOTATTR_LOOP);
    }
}

void fn_1_5194(void)
{
    OMOBJ *obj = lbl_1_bss_C;

    Hu3DMotionShiftSet(
        obj->mdlId[0], obj->mtnId[3], 0.0f, 10.0f, 0);
    obj->work[3] = 0;
    obj->objFunc = fn_1_50EC;
}

void fn_1_5208(OMOBJ *obj)
{
    Hu3DMotionSpeedSet(obj->mdlId[0], 2.0f);
    if (obj->work[3]++ > 30) {
        obj->objFunc = NULL;
        Hu3DMotionSpeedSet(obj->mdlId[0], 1.0f);
        Hu3DMotionShiftSet(obj->mdlId[0], obj->mtnId[0], 0.0f, 15.0f,
            HU3D_MOTATTR_LOOP);
    }
}

void fn_1_52B0(void)
{
    OMOBJ *obj = lbl_1_bss_10;

    Hu3DMotionShiftSet(
        obj->mdlId[0], obj->mtnId[3], 0.0f, 10.0f, 0);
    obj->work[3] = 0;
    obj->objFunc = fn_1_5208;
}

inline void fn_1_52B0(void);

void fn_1_58A0(void)
{
    LBL_1_BSS_1C8_ENTRY *work = &lbl_1_bss_1C8[0];
    OMOBJ *obj = lbl_1_bss_C;

    work->state = 0;
    work->time = 0.0f;
    work->duration = 20.0f;
    Hu3DModelPosGet(obj->mdlId[0], &work->pos);
    work->pos.x = -325.0f;
    work->pos.y = 0.0f;
    work->pos.z = 100.0f;
    Hu3DModelPosGet(obj->mdlId[0], &work->unk_10);
    work->unk_10.x = 100.0f;
    work->unk_10.y = 600.0f;
    work->unk_10.z = 200.0f;
    Hu3DModelPosGet(obj->mdlId[0], &work->unk_1C);
    work->unk_1C.x = 100.0f;
    work->unk_1C.y = 225.0f;
    work->unk_1C.z = -1000.0f;
    Hu3DMotionShiftSet(
        obj->mdlId[0], obj->mtnId[2], 0.0f, 0.0f, 0);
    lbl_1_bss_A6C[0] = HuAudFXPlay(0x47E);
    obj->objFunc = fn_1_5324;
}

s32 fn_1_5FB0(void)
{
    OMOBJ *obj = lbl_1_bss_C;
    HuVecF pos;

    Hu3DModelPosGet(obj->mdlId[0], &pos);
    if (pos.x < -100.0f) {
        return FALSE;
    }
    obj->work[0] = 0;
    obj->objFunc = fn_1_5A18;
    Hu3DMotionShiftSet(obj->mdlId[0], obj->mtnId[1], 0.0f, 0.0f,
        HU3D_MOTATTR_LOOP);
    return TRUE;
}

inline s32 fn_1_5FB0(void);

void fn_1_605C(OMOBJ *obj)
{
    omSetStatBit(obj, OM_STAT_MODELPAUSE);
    obj->mdlId[0] = Hu3DModelCreate(
        HuDataReadNum(DATANUM(DATA_mdparty, 0x4F), HU_MEMNUM_OVL));
    obj->mtnId[0] = Hu3DJointMotion(obj->mdlId[0],
        HuDataReadNum(DATANUM(DATA_mdparty, 0x50), HU_MEMNUM_OVL));
    obj->mtnId[1] = Hu3DJointMotion(obj->mdlId[0],
        HuDataReadNum(DATANUM(DATA_mdparty, 0x51), HU_MEMNUM_OVL));
    obj->mtnId[2] = Hu3DJointMotion(obj->mdlId[0],
        HuDataReadNum(DATANUM(DATA_mdparty, 0x52), HU_MEMNUM_OVL));
    obj->mtnId[3] = Hu3DJointMotion(obj->mdlId[0],
        HuDataReadNum(DATANUM(DATA_mdparty, 0x53), HU_MEMNUM_OVL));
    obj->mtnId[4] = Hu3DJointMotion(obj->mdlId[0],
        HuDataReadNum(DATANUM(DATA_mdparty, 0x54), HU_MEMNUM_OVL));
    obj->mtnId[5] = Hu3DJointMotion(obj->mdlId[0],
        HuDataReadNum(DATANUM(DATA_mdparty, 0x55), HU_MEMNUM_OVL));
    Hu3DModelPosSet(obj->mdlId[0], -100.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(obj->mdlId[0], 0.0f, 0.0f, 0.0f);
    Hu3DModelScaleSet(obj->mdlId[0], 1.25f, 1.25f, 1.25f);
    Hu3DModelLayerSet(obj->mdlId[0], 1);
    Hu3DMotionShiftSet(obj->mdlId[0], obj->mtnId[0], 0.0f, 0.0f,
        HU3D_MOTATTR_LOOP);
    Hu3DModelShadowSet(obj->mdlId[0]);
    obj->objFunc = NULL;
}

void fn_1_6284(OMOBJ *obj)
{
    if (obj) {
        Hu3DMotionKill(obj->mtnId[0]);
        Hu3DMotionKill(obj->mtnId[1]);
        Hu3DMotionKill(obj->mtnId[2]);
        Hu3DMotionKill(obj->mtnId[3]);
        Hu3DMotionKill(obj->mtnId[4]);
        Hu3DMotionKill(obj->mtnId[5]);
        Hu3DModelKill(obj->mdlId[0]);
        omDelObjEx(lbl_1_bss_0, obj);
    }
    obj->objFunc = NULL;
}

inline void fn_1_6284(OMOBJ *obj);

void fn_1_68A4(void)
{
    LBL_1_BSS_1C8_ENTRY *work = &lbl_1_bss_1C8[1];
    OMOBJ *obj = lbl_1_bss_10;

    work->state = 0;
    work->time = 0.0f;
    work->duration = 20.0f;
    Hu3DModelPosGet(obj->mdlId[0], &work->pos);
    work->pos.x = 325.0f;
    work->pos.y = 0.0f;
    work->pos.z = 100.0f;
    Hu3DModelPosGet(obj->mdlId[0], &work->unk_10);
    work->unk_10.x = -100.0f;
    work->unk_10.y = 400.0f;
    work->unk_10.z = 200.0f;
    Hu3DModelPosGet(obj->mdlId[0], &work->unk_1C);
    work->unk_1C.x = -100.0f;
    work->unk_1C.y = 125.0f;
    work->unk_1C.z = -1000.0f;
    Hu3DMotionShiftSet(
        obj->mdlId[0], obj->mtnId[2], 0.0f, 0.0f, 0);
    lbl_1_bss_A6C[1] = HuAudFXPlay(0x47C);
    obj->objFunc = fn_1_6324;
}

s32 fn_1_6FB8(void)
{
    OMOBJ *obj = lbl_1_bss_10;
    HuVecF pos;

    Hu3DModelPosGet(obj->mdlId[0], &pos);
    if (pos.x > 100.0f) {
        return FALSE;
    }
    obj->work[0] = 0;
    obj->objFunc = fn_1_6A20;
    Hu3DMotionShiftSet(obj->mdlId[0], obj->mtnId[1], 0.0f, 0.0f,
        HU3D_MOTATTR_LOOP);
    return TRUE;
}

inline s32 fn_1_6FB8(void);

void fn_1_7064(OMOBJ *obj)
{
    omSetStatBit(obj, OM_STAT_MODELPAUSE);
    obj->mdlId[0] = Hu3DModelCreate(
        HuDataReadNum(DATANUM(DATA_mdparty, 0x56), HU_MEMNUM_OVL));
    obj->mdlId[1] = Hu3DModelCreate(
        HuDataReadNum(DATANUM(DATA_mdparty, 0x57), HU_MEMNUM_OVL));
    obj->mtnId[0] = Hu3DJointMotion(obj->mdlId[0],
        HuDataReadNum(DATANUM(DATA_mdparty, 0x58), HU_MEMNUM_OVL));
    obj->mtnId[1] = Hu3DJointMotion(obj->mdlId[0],
        HuDataReadNum(DATANUM(DATA_mdparty, 0x59), HU_MEMNUM_OVL));
    obj->mtnId[2] = Hu3DJointMotion(obj->mdlId[0],
        HuDataReadNum(DATANUM(DATA_mdparty, 0x5A), HU_MEMNUM_OVL));
    obj->mtnId[3] = Hu3DJointMotion(obj->mdlId[0],
        HuDataReadNum(DATANUM(DATA_mdparty, 0x5B), HU_MEMNUM_OVL));
    obj->mtnId[4] = Hu3DJointMotion(obj->mdlId[0],
        HuDataReadNum(DATANUM(DATA_mdparty, 0x5C), HU_MEMNUM_OVL));
    obj->mtnId[5] = Hu3DJointMotion(obj->mdlId[0],
        HuDataReadNum(DATANUM(DATA_mdparty, 0x5D), HU_MEMNUM_OVL));
    Hu3DModelHookSet(obj->mdlId[0], lbl_1_data_DC6, obj->mdlId[1]);
    Hu3DModelPosSet(obj->mdlId[0], 100.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(obj->mdlId[0], 0.0f, 0.0f, 0.0f);
    Hu3DModelScaleSet(obj->mdlId[0], 1.25f, 1.25f, 1.25f);
    Hu3DModelLayerSet(obj->mdlId[0], 1);
    Hu3DMotionShiftSet(obj->mdlId[0], obj->mtnId[0], 0.0f, 0.0f,
        HU3D_MOTATTR_LOOP);
    Hu3DModelShadowSet(obj->mdlId[0]);
    obj->objFunc = NULL;
}

void fn_1_72C4(OMOBJ *obj)
{
    if (obj) {
        Hu3DModelHookReset(obj->mdlId[1]);
        Hu3DMotionKill(obj->mtnId[0]);
        Hu3DMotionKill(obj->mtnId[1]);
        Hu3DMotionKill(obj->mtnId[2]);
        Hu3DMotionKill(obj->mtnId[3]);
        Hu3DMotionKill(obj->mtnId[4]);
        Hu3DMotionKill(obj->mtnId[5]);
        Hu3DModelKill(obj->mdlId[0]);
        Hu3DModelKill(obj->mdlId[1]);
        omDelObjEx(lbl_1_bss_0, obj);
    }
    obj->objFunc = NULL;
}

inline void fn_1_72C4(OMOBJ *obj);

void fn_1_7900(void)
{
    LBL_1_BSS_9BC_ENTRY *desc;
    OMOBJ *obj = lbl_1_bss_14;
    LBL_1_BSS_1C8_ENTRY *work;
    s16 i;

    for (i = 0, desc = &lbl_1_bss_9BC[i]; i < 4; i++, desc++) {
        work = &lbl_1_bss_108[i];
        work->state = 1;
        work->time = 0.0f;
        work->duration = 90.0f;
        Hu3DModelPosGet(obj->mdlId[desc->chrSel], &work->pos);
        Hu3DModelPosGet(obj->mdlId[desc->chrSel], &work->unk_10);
        work->unk_10.x += 0.5f * work->unk_10.x;
        work->unk_10.y = 550.0f;
        work->unk_10.z = 100.0f;
        Hu3DModelPosGet(obj->mdlId[desc->chrSel], &work->unk_1C);
        work->unk_1C.y = 225.0f;
        work->unk_1C.z = -800.0f;
        Hu3DMotionShiftSet(obj->mdlId[desc->chrSel],
            obj->mtnId[(2 * desc->chrSel) + 1], 0.0f, 10.0f,
            HU3D_MOTATTR_LOOP);
    }
    HuAudFXPlay(0x4A2);
    obj->objFunc = fn_1_737C;
}

void fn_1_7ABC(OMOBJ *obj)
{
    s16 i;
    LBL_1_BSS_9BC_ENTRY *desc;
    float scale;

    for (i = 0, desc = &lbl_1_bss_9BC[i]; i < 4; i++, desc++) {
        scale = fn_1_1868(0.0f, 1.0f, obj->work[0], obj->work[1]);
        Hu3DModelScaleSet(
            obj->mdlId[desc->chrSel], scale, scale, scale);
    }
    if ((obj->work[0] += 1.0f) > obj->work[1]) {
        obj->objFunc = NULL;
    }
}

void fn_1_7C88(void)
{
    OMOBJ *obj = lbl_1_bss_14;
    LBL_1_BSS_9BC_ENTRY *desc;
    HuVecF pos;
    s16 i;

    for (i = 0, desc = &lbl_1_bss_9BC[i]; i < 4; i++, desc++) {
        Hu3DModelPosGet(lbl_1_bss_288[desc->chrSel].modelId, &pos);
        if (desc->unk_4 != 0) {
            fn_1_45F3C(i, &pos, 4, 0);
        } else {
            fn_1_45F3C(i, &pos, desc->unk_A, 0);
        }
    }
    for (i = 0, desc = &lbl_1_bss_9BC[i]; i < 4; i++, desc++) {
        Hu3DModelPosGet(lbl_1_bss_288[desc->chrSel].modelId, &pos);
        pos.y -= 70.0f;
        Hu3DModelPosSetV(obj->mdlId[desc->chrSel], &pos);
        Hu3DModelScaleSet(obj->mdlId[desc->chrSel], 0.0f, 0.0f, 0.0f);
        Hu3DModelAttrReset(obj->mdlId[desc->chrSel], HU3D_ATTR_DISPOFF);
    }
    obj->work[0] = 0;
    obj->work[1] = 15;
    obj->objFunc = fn_1_7ABC;
    HuPrcSleep(15);
    HuAudFXPlay(0x4A1);
    for (i = 0, desc = &lbl_1_bss_9BC[i]; i < 4; i++, desc++) {
        Hu3DModelPosGet(lbl_1_bss_288[desc->chrSel].modelId, &pos);
        if (desc->unk_4 != 0) {
            fn_1_45F3C(i, &pos, 4, 1);
        } else {
            fn_1_45F3C(i, &pos, desc->unk_A, 1);
        }
    }
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

void fn_1_884C(s16 memberNo)
{
    OMOBJ *obj = lbl_1_bss_8;

    HuSprGrpPosSet(lbl_1_bss_918[1], 288.0f, 60.0f);
    HuSprGrpTPLvlSet(lbl_1_bss_918[1], 0.0f);
    fn_1_4734(lbl_1_bss_918[1], HUSPR_ATTR_DISPOFF);
    HuSprAttrReset(
        lbl_1_bss_918[1], memberNo, HUSPR_ATTR_DISPOFF);
    obj->work[2] = 1;
    obj->work[3] = 0;
}

inline void fn_1_884C(s16 memberNo);

void fn_1_8950(void)
{
    OMOBJ *obj = lbl_1_bss_8;

    obj->work[2] = 2;
    obj->work[3] = 0;
}

inline void fn_1_8950(void);

void fn_1_8980(void)
{
    HuSprGrpPosSet(lbl_1_bss_918[1], 288.0f, 60.0f);
    HuSprGrpTPLvlSet(lbl_1_bss_918[1], 0.0f);
}

inline void fn_1_8980(void);

void fn_1_89E0(OMOBJ *obj)
{
    float alpha;

    switch (obj->work[2]) {
        case 1:
            alpha = fn_1_1868(
                0.0f, 1.0f, obj->work[3], 10.0f);
            HuSprGrpTPLvlSet(lbl_1_bss_918[1], alpha);
            if (++obj->work[3] > 10) {
                obj->work[2] = 0;
            }
            break;
        case 2:
            alpha = fn_1_1868(
                1.0f, 0.0f, obj->work[3], 10.0f);
            HuSprGrpTPLvlSet(lbl_1_bss_918[1], alpha);
            if (++obj->work[3] > 10) {
                obj->work[2] = 0;
            }
            break;
    }
}

void fn_1_8C28(s16 animNo)
{
    OMOBJ *obj = lbl_1_bss_8;

    Hu3DAnimAnimSet(
        obj->mtnId[2], lbl_1_bss_93C[(2 * animNo) + 3]);
    Hu3DAnimAnimSet(
        obj->mtnId[3], lbl_1_bss_93C[(2 * animNo) + 4]);
    obj->work[0] = 1;
    obj->work[1] = 0;
}

inline void fn_1_8C28(s16 animNo);

void fn_1_8CCC(void)
{
    OMOBJ *obj = lbl_1_bss_8;

    obj->work[0] = 2;
    obj->work[1] = 0;
    fn_1_8950();
}

inline void fn_1_8CCC(void);

void fn_1_8D20(OMOBJ *obj)
{
    HuVecF rot;
    float modelAlpha;
    s16 i;

    for (i = 0; i < 2; i++) {
        Hu3DModelRotGet(obj->mdlId[i], &rot);
        rot.z -= 0.1f;
        if (rot.z < 0.0f) {
            rot.z += 360.0f;
        }
        Hu3DModelRotSetV(obj->mdlId[i], &rot);
    }
    switch (obj->work[0]) {
        case 1:
            modelAlpha = fn_1_1868(
                1.0f, 0.0f, obj->work[1], 30.0f);
            Hu3DModelTPLvlSet(obj->mdlId[0], modelAlpha);
            if (++obj->work[1] > 30) {
                obj->work[0] = 0;
            }
            break;
        case 2:
            modelAlpha = fn_1_1868(
                0.0f, 1.0f, obj->work[1], 30.0f);
            Hu3DModelTPLvlSet(obj->mdlId[0], modelAlpha);
            if (++obj->work[1] > 30) {
                obj->work[0] = 0;
            }
            break;
    }
    fn_1_89E0(obj);
}

void fn_1_9218(OMOBJ *obj)
{
    s16 i;

    omSetStatBit(obj, OM_STAT_MODELPAUSE);
    for (i = 0; i < 2; i++) {
        if (i == 0) {
            obj->mdlId[i] = Hu3DModelCreate(
                HuDataSelHeapReadNum(
                    DATANUM(DATA_mdparty, 3), HU_MEMNUM_OVL,
                    HEAP_MODEL));
        } else {
            obj->mdlId[i] = Hu3DModelLink(obj->mdlId[0]);
        }
        Hu3DModelPosSet(
            obj->mdlId[i], 0.0f, -100.0f, -850.0f - i);
        Hu3DModelRotSet(obj->mdlId[i], 0.0f, 0.0f, 0.0f);
        Hu3DModelAttrSet(obj->mdlId[i], HU3D_MOTATTR_PAUSE);
        Hu3DModelLayerSet(obj->mdlId[i], 0);
        obj->mtnId[2 * i] = Hu3DAnimCreate(
            lbl_1_bss_93C[1], obj->mdlId[i], lbl_1_data_DD8);
        obj->mtnId[(2 * i) + 1] = Hu3DAnimCreate(
            lbl_1_bss_93C[2], obj->mdlId[i], lbl_1_data_DDF);
    }
    HuSprGrpPosSet(lbl_1_bss_918[1], 288.0f, 60.0f);
    HuSprGrpTPLvlSet(lbl_1_bss_918[1], 0.0f);
    obj->objFunc = fn_1_8D20;
}

void fn_1_945C(OMOBJ *obj)
{
    s16 i;

    if (obj) {
        for (i = 0; i < 2; i++) {
            Hu3DAnimKill(obj->mtnId[2 * i]);
            Hu3DAnimKill(obj->mtnId[(2 * i) + 1]);
            obj->mtnId[2 * i] = -1;
            obj->mtnId[(2 * i) + 1] = -1;
        }
        for (i = 1; i >= 0; i--) {
            Hu3DModelKill(obj->mdlId[i]);
        }
        omDelObjEx(lbl_1_bss_0, obj);
    }
    obj = NULL;
}

inline void fn_1_945C(OMOBJ *obj);

u32 fn_1_9558(s16 charNo1, s16 charNo2)
{
    s8 pair[55][2] = {
        { 0, 1 }, { 0, 2 }, { 0, 3 }, { 0, 4 }, { 0, 5 },
        { 0, 6 }, { 0, 7 }, { 0, 8 }, { 0, 9 }, { 0, 10 },
        { 1, 2 }, { 1, 3 }, { 1, 4 }, { 1, 5 }, { 1, 6 },
        { 1, 7 }, { 1, 8 }, { 1, 9 }, { 1, 10 }, { 2, 3 },
        { 2, 4 }, { 2, 5 }, { 2, 6 }, { 2, 7 }, { 2, 8 },
        { 2, 9 }, { 2, 10 }, { 3, 4 }, { 3, 5 }, { 3, 6 },
        { 3, 7 }, { 3, 8 }, { 3, 9 }, { 3, 10 }, { 4, 5 },
        { 4, 6 }, { 4, 7 }, { 4, 8 }, { 4, 9 }, { 4, 10 },
        { 5, 6 }, { 5, 7 }, { 5, 8 }, { 5, 9 }, { 5, 10 },
        { 6, 7 }, { 6, 8 }, { 6, 9 }, { 6, 10 }, { 7, 8 },
        { 7, 9 }, { 7, 10 }, { 8, 9 }, { 8, 10 }, { 9, 10 },
    };
    s8 i;

    for (i = 0; i < 55; i++) {
        if ((charNo1 == pair[i][0] && charNo2 == pair[i][1])
            || (charNo1 == pair[i][1] && charNo2 == pair[i][0])) {
            break;
        }
    }
    if (i == 55) {
        return 0x30037;
    }
    return 0x30000 + i;
}

inline u32 fn_1_9558(s16 charNo1, s16 charNo2);

void fn_1_9658(s16 winNo, u32 messNum)
{
    HuWinAttrSet(lbl_1_bss_104[winNo], HUWIN_ATTR_ALIGN_CENTER);
    HuWinMesSet(lbl_1_bss_104[winNo], messNum);
    HuWinMesSpeedSet(lbl_1_bss_104[winNo], 0);
}

inline void fn_1_9658(s16 winNo, u32 messNum);

void fn_1_96E4(u32 messNum1, u32 messNum2)
{
    HuWinDispOn(lbl_1_bss_104[0]);
    HuWinDispOn(lbl_1_bss_104[1]);
    fn_1_9658(0, messNum1);
    fn_1_9658(1, messNum2);
    fn_1_47B4(lbl_1_bss_918[13], HUSPR_ATTR_DISPOFF);
}

inline void fn_1_96E4(u32 messNum1, u32 messNum2);

void fn_1_9804(void)
{
    HuWinDispOff(lbl_1_bss_104[0]);
    HuWinDispOff(lbl_1_bss_104[1]);
    fn_1_4734(lbl_1_bss_918[13], HUSPR_ATTR_DISPOFF);
}

inline void fn_1_9804(void);

void fn_1_98A4(void)
{
    HuVecF pos[2] = {
        { 150.0f, 120.0f, 0.0f },
        { 426.0f, 120.0f, 0.0f },
    };
    s16 i;

    for (i = 0; i < 2; i++) {
        lbl_1_bss_104[i] = HuWinExCreateFrame(
            pos[i].x - 124.0f, 6.0f + pos[i].y, 240, 42, -1, 0);
        HuWinDispOff(lbl_1_bss_104[i]);
        HuWinBGTPLvlSet(lbl_1_bss_104[i], 0.0f);
        HuWinPriSet(lbl_1_bss_104[i], 0);
        HuSprPosSet(lbl_1_bss_918[13], i, pos[i].x, pos[i].y);
    }
}

inline void fn_1_98A4(void);

void fn_1_9A24(void)
{
    s16 i;

    for (i = 0; i < 2; i++) {
        HuWinExKill(lbl_1_bss_104[i]);
    }
}

inline void fn_1_9A24(void);

void fn_1_9A7C(s16 playerNo, s16 bank)
{
    lbl_1_bss_D4[playerNo].x = 0.85f;
    fn_1_47B4(
        lbl_1_bss_918[playerNo + 4], HUSPR_ATTR_DISPOFF);
    HuSprBankSet(lbl_1_bss_918[playerNo + 4], 0, bank);
}

inline void fn_1_9A7C(s16 playerNo, s16 bank);

void fn_1_9B68(s16 arg0)
{
    lbl_1_bss_D4[arg0].x = 0.0f;
}

void fn_1_9B90(s16 arg0, s16 arg1)
{
    HuSprBankSet(lbl_1_bss_918[arg0 + 4], 0, arg1);
}

inline void fn_1_9B90(s16 arg0, s16 arg1);

void fn_1_9BDC(s16 playerNo, s16 bank)
{
    lbl_1_bss_A4[playerNo].x = 0.85f;
    fn_1_47B4(
        lbl_1_bss_918[playerNo + 8], HUSPR_ATTR_DISPOFF);
    HuSprBankSet(lbl_1_bss_918[playerNo + 8], 2, bank);
}

inline void fn_1_9BDC(s16 playerNo, s16 bank);

void fn_1_9CC8(s16 arg0)
{
    lbl_1_bss_A4[arg0].x = 0.0f;
}

void fn_1_9CF0(s16 arg0, s16 arg1)
{
    HuSprBankSet(lbl_1_bss_918[arg0 + 8], 2, arg1);
}

void fn_1_9D3C(s16 playerNo, s16 bank)
{
    lbl_1_bss_8C[playerNo].x = 0.85f;
    fn_1_47B4(
        lbl_1_bss_918[playerNo + 14], HUSPR_ATTR_DISPOFF);
    HuSprBankSet(lbl_1_bss_918[playerNo + 14], 2, bank);
}

inline void fn_1_9D3C(s16 playerNo, s16 bank);

void fn_1_9E28(s16 arg0)
{
    lbl_1_bss_8C[arg0].x = 0.0f;
}

void fn_1_9E50(s16 arg0, s16 arg1)
{
    HuSprBankSet(lbl_1_bss_918[arg0 + 14], 2, arg1);
}

void fn_1_9E9C(void)
{
    lbl_1_bss_80.x = 0.5f;
    fn_1_47B4(lbl_1_bss_918[16], HUSPR_ATTR_DISPOFF);
}

inline void fn_1_9E9C(void);

void fn_1_9F34(void)
{
    lbl_1_bss_80.x = 0.0f;
}

inline void fn_1_9F34(void);

void fn_1_A5D8(OMOBJ *obj)
{
    s16 i;

    for (i = 0; i < 4; i++) {
        lbl_1_bss_D4[i].x = lbl_1_bss_D4[i].y = 0.0f;
        lbl_1_bss_A4[i].x = lbl_1_bss_A4[i].y = 0.0f;
    }
    for (i = 0; i < 2; i++) {
        lbl_1_bss_8C[i].x = lbl_1_bss_8C[i].y = 0.0f;
    }
    lbl_1_bss_80.x = lbl_1_bss_80.y = 0.0f;
    obj->objFunc = fn_1_9F50;
    fn_1_98A4();
}

void fn_1_A880(void)
{
    fn_1_9A24();
}

inline void fn_1_A880(void);

void fn_1_A8D8(
    s16 memberNo, HuVecF *pos3D, float xOffset, float yOffset)
{
    HuVecF pos2D = { 0.0f, 0.0f, 0.0f };

    if (pos3D) {
        Hu3D3Dto2D(pos3D, 1, &pos2D);
    }
    HuSprPosSet(lbl_1_bss_918[2], memberNo,
        pos2D.x + xOffset, pos2D.y + yOffset);
}

inline void fn_1_A8D8(
    s16 memberNo, HuVecF *pos3D, float xOffset, float yOffset);

void fn_1_A970(
    s16 memberNo, HuVecF *pos3D, float xOffset, float yOffset)
{
    fn_1_A8D8(memberNo, pos3D, xOffset, yOffset);
    HuSprScaleSet(lbl_1_bss_918[2], memberNo, 2.0f, 2.0f);
    HuSprAttrReset(lbl_1_bss_918[2], memberNo, HUSPR_ATTR_DISPOFF);
    lbl_1_bss_38[memberNo] = 2.0f;
    lbl_1_data_DE6[memberNo] = 1;
}

inline void fn_1_A970(
    s16 memberNo, HuVecF *pos3D, float xOffset, float yOffset);

void fn_1_AA94(s16 memberNo)
{
    HuSprAttrSet(lbl_1_bss_918[2], memberNo, HUSPR_ATTR_DISPOFF);
    lbl_1_data_DE6[memberNo] = 0;
}

inline void fn_1_AA94(s16 memberNo);

void fn_1_AAF0(s16 memberNo)
{
    lbl_1_bss_38[memberNo] = 2.0f;
    if (lbl_1_data_DE6[memberNo] == 1) {
        HuAudFXPlay(0);
    }
}

inline void fn_1_AAF0(s16 memberNo);

void fn_1_AB64(void)
{
    s16 i;

    for (i = 0; i < 6; i++) {
        lbl_1_bss_38[i] =
            fn_1_160C(lbl_1_bss_38[i], 1.0f, 5.0f);
        HuSprScaleSet(lbl_1_bss_918[2], i,
            lbl_1_bss_38[i], lbl_1_bss_38[i]);
    }
}

void fn_1_AC88(void)
{
    s16 i;

    for (i = 0; i < 6; i++) {
        fn_1_AA94(i);
        lbl_1_data_DE6[i] = 0;
    }
}

void fn_1_AD14(s16 modelNo, HuVecF *pos)
{
    lbl_1_bss_50[modelNo].x = pos->x - 50.0f;
    lbl_1_bss_50[modelNo].y = 50.0f + pos->y;
    lbl_1_bss_50[modelNo].z = 50.0f + pos->z;
}

inline void fn_1_AD14(s16 modelNo, HuVecF *pos);

void fn_1_AD9C(s32 modelNo, HuVecF *pos, s16 bank)
{
    OMOBJ *obj = lbl_1_bss_1C.obj;

    Hu3DModelPosSet(
        obj->mdlId[(s16)modelNo], pos->x, pos->y, pos->z);
    Hu3DModelRotSet(
        obj->mdlId[(s16)modelNo], 0.0f, 0.0f, 180.0f);
    Hu3DModelScaleSet(
        obj->mdlId[(s16)modelNo], 1.0f, 1.0f, 1.0f);
    fn_1_AD14(modelNo, pos);
    Hu3DModelAttrReset(
        obj->mdlId[(s16)modelNo], HU3D_ATTR_DISPOFF);
    HuSprScaleSet(lbl_1_bss_918[3], modelNo, 0.8f, 0.8f);
    HuSprAttrReset(lbl_1_bss_918[3], modelNo, HUSPR_ATTR_DISPOFF);
    HuSprBankSet(lbl_1_bss_918[3], modelNo, bank);
}

inline void fn_1_AD9C(s32 modelNo, HuVecF *pos, s16 bank);

void fn_1_AF70(s32 modelNo)
{
    OMOBJ *obj = lbl_1_bss_1C.obj;

    Hu3DModelAttrSet(obj->mdlId[(s16)modelNo], HU3D_ATTR_DISPOFF);
    HuSprScaleSet(lbl_1_bss_918[3], modelNo, 0.8f, 0.8f);
    HuSprAttrSet(lbl_1_bss_918[3], modelNo, HUSPR_ATTR_DISPOFF);
}

inline void fn_1_AF70(s32 modelNo);

void fn_1_B4C8(OMOBJ *obj)
{
    s16 i;

    omSetStatBit(obj, OM_STAT_MODELPAUSE);
    for (i = 0; i < 4; i++) {
        if (i == 0) {
            obj->mdlId[i] = Hu3DModelCreate(
                HuDataSelHeapReadNum(
                    DATANUM(DATA_mdparty, 0x1A), HU_MEMNUM_OVL,
                    HEAP_MODEL));
            obj->mtnId[i] = Hu3DMotionIDGet(obj->mdlId[0]);
        } else {
            obj->mdlId[i] = Hu3DModelLink(obj->mdlId[0]);
        }
        Hu3DModelLayerSet(obj->mdlId[i], 1);
        Hu3DMotionShiftSet(obj->mdlId[i], obj->mtnId[0],
            0.0f, 0.0f, HU3D_MOTATTR_LOOP);
        fn_1_AF70(i);
    }
    fn_1_AC88();
    obj->objFunc = fn_1_B008;
}

void fn_1_B6C0(OMOBJ *obj)
{
    s16 i;

    if (obj) {
        Hu3DMotionKill(obj->mtnId[0]);
        for (i = 3; i >= 0; i--) {
            Hu3DModelKill(obj->mdlId[i]);
        }
        omDelObjEx(lbl_1_bss_0, obj);
    }
    obj = NULL;
}

inline void fn_1_B6C0(OMOBJ *obj);

s16 fn_1_BB30(s16 arg0, s16 *arg1, s16 arg2, s16 arg3)
{
    s16 order[12][3] = {
        { 2, 1, 3 },
        { 6, 7, 5 },
        { 0, 7, 1 },
        { 4, 5, 3 },
        { 1, 2, 0 },
        { 3, 2, 4 },
        { 7, 6, 0 },
        { 5, 6, 4 },
        { 1, 0, 2 },
        { 7, 0, 6 },
        { 3, 4, 2 },
        { 5, 4, 6 },
    };

    return fn_1_B748(arg0, order[arg2][0], order[arg2][1],
        order[arg2][2], arg1, arg3);
}

void fn_1_C158(s16 modelNo, s16 animNo, s16 dataNo, s16 bank)
{
    LBL_1_BSS_288_ENTRY *entry = &lbl_1_bss_288[modelNo];

    Hu3DAnimAnimSet(entry->animId[animNo], lbl_1_bss_800[dataNo]);
    if (bank != -1) {
        Hu3DAnimBankSet(entry->animId[animNo], bank);
    }
}

inline void fn_1_C158(s16 modelNo, s16 animNo, s16 dataNo, s16 bank);

void fn_1_C200(s16 modelNo, s16 dataNo)
{
    LBL_1_BSS_288_ENTRY *entry;
    s16 i;

    for (i = 0; i < 4; i++) {
        entry = &lbl_1_bss_288[modelNo];
        Hu3DAnimAnimSet(entry->animId[i], lbl_1_bss_800[dataNo]);
    }
}

void fn_1_D3E0(s16 modelNo, float angle, s16 time, s16 duration)
{
    LBL_1_BSS_288_ENTRY *entry = &lbl_1_bss_288[modelNo];
    float rotY;

    if (time == 0) {
        Hu3DModelRotGet(entry->modelId, &entry->rot);
        if (entry->rot.y - angle > 180.0f) {
            entry->rot.y -= 360.0f;
        } else if (entry->rot.y - angle < -180.0f) {
            entry->rot.y += 360.0f;
        }
    }
    rotY = fn_1_1868(entry->rot.y, 0.0f, time, duration);
    Hu3DModelRotSet(entry->modelId, 0.0f, rotY, 0.0f);
}

inline void fn_1_D3E0(
    s16 modelNo, float angle, s16 time, s16 duration);

void fn_1_D648(s16 arg0)
{
    lbl_1_bss_24->mtnId[0] = arg0;
}

s16 fn_1_D660(void)
{
    return lbl_1_bss_24->mtnId[0];
}

BOOL fn_1_D678(void)
{
    OMOBJ *obj = lbl_1_bss_24;

    if (obj->work[0] >= 10) {
        return TRUE;
    }
    return FALSE;
}

void fn_1_D6B0(OMOBJ_FUNC callback)
{
    OMOBJ *obj = lbl_1_bss_24;

    lbl_1_bss_24->mtnId[0] = 0;
    obj->work[0] = 0;
    obj->work[1] = 0;
    obj->work[2] = 0;
    obj->work[3] = 0;
    obj->objFunc = callback;
    while (lbl_1_bss_24->mtnId[0] == 0) {
        HuPrcVSleep();
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

void fn_1_D978(void)
{
    s16 i;
    s16 j;

    if (lbl_1_bss_24) {
        lbl_1_bss_24->mtnId[0] = -1;
        for (i = 24; i >= 0; i--) {
            for (j = 0; j < 4; j++) {
                Hu3DAnimKill(lbl_1_bss_288[i].animId[j]);
            }
            Hu3DModelKill(lbl_1_bss_288[i].modelId);
        }
    }
    lbl_1_bss_24 = NULL;
}

inline void fn_1_D978(void);

void fn_1_114C4(OMOBJ *obj)
{
    obj->work[0] = 10;
    obj->objFunc = fn_1_10518;
    lbl_1_bss_24->mtnId[0] = 1;
}

void fn_1_114F4(OMOBJ *obj)
{
    HuSprAttrSet(lbl_1_bss_918[2], 4, HUSPR_ATTR_DISPOFF);
    lbl_1_data_DE6[4] = 0;
    HuSprAttrSet(lbl_1_bss_918[2], 5, HUSPR_ATTR_DISPOFF);
    lbl_1_data_DE6[5] = 0;
    obj->objFunc = NULL;
    lbl_1_bss_24->mtnId[0] = 1;
}

void fn_1_12260(s16 arg0)
{
    HuVecF rot;
    LBL_1_BSS_288_ENTRY *entry = &lbl_1_bss_288[arg0];

    Hu3DModelRotGet(entry->modelId, &rot);
    rot.y += 6.0f;
    if (rot.y > 360.0f) {
        rot.y -= 360.0f;
    }
    Hu3DModelRotSetV(entry->modelId, &rot);
}

void fn_1_122FC(s16 arg0)
{
    LBL_1_BSS_288_ENTRY *entry = &lbl_1_bss_288[arg0];

    Hu3DModelRotSet(entry->modelId, 0.0f, 0.0f, 0.0f);
}

inline void fn_1_122FC(s16 arg0);

void fn_1_12B18(OMOBJ *obj)
{
    s16 i;
    LBL_1_BSS_9BC_ENTRY *entry;

    for (i = 0, entry = lbl_1_bss_9BC; i < 4; i++, entry++) {
        if (entry->unk_4 == 0 && entry->unk_0 == 0) {
            lbl_1_data_E12[entry->chrSel].unk_10 = 1;
            fn_1_AD9C(
                i, &lbl_1_data_8A4[entry->chrSel + 14], entry->unk_A);
        }
    }
    obj->work[0] = 10;
    obj->objFunc = fn_1_12364;
    lbl_1_bss_24->mtnId[0] = 1;
}

void fn_1_12D7C(OMOBJ *obj)
{
    s16 i;
    LBL_1_BSS_9BC_ENTRY *entry;

    for (i = 0, entry = lbl_1_bss_9BC; i < 4; i++, entry++) {
        if (entry->unk_4 == 0 && entry->unk_0 == 0) {
            fn_1_AF70(i);
        }
    }
    for (i = 0; i < 11; i++) {
        fn_1_122FC(i);
    }
    obj->objFunc = NULL;
    lbl_1_bss_24->mtnId[0] = 1;
}

void fn_1_12EC8(OMOBJ *obj)
{
    LBL_1_BSS_9BC_ENTRY *entry = &lbl_1_bss_9BC[0];

    entry->unk_0 = 0;
    lbl_1_bss_D4[0].x = 0.0f;
    fn_1_C158((s16)entry->chrSel, 0, (s16)entry->chrSel, 0);
    fn_1_C158((s16)entry->chrSel, 2, (s16)entry->chrSel, 0);
    obj->objFunc = NULL;
    lbl_1_bss_24->mtnId[0] = 1;
}

void fn_1_14344(OMOBJ *obj)
{
    s16 i;
    LBL_1_BSS_9BC_ENTRY *entry;

    for (i = 0, entry = lbl_1_bss_9BC; i < 4; i++, entry++) {
        if (entry->unk_4 == 1) {
            if (entry->unk_0 == 0) {
                lbl_1_data_E12[entry->chrSel].unk_10 = -1;
                fn_1_AF70(i);
                break;
            } else if (entry->unk_0 == 1) {
                fn_1_AA94(4);
                fn_1_AA94(5);
                break;
            }
        }
    }
    for (i = 0; i < 11; i++) {
        fn_1_122FC(i);
    }
    obj->objFunc = NULL;
    lbl_1_bss_24->mtnId[0] = 1;
}

void fn_1_14F64(OMOBJ *obj)
{
    s16 i;
    LBL_1_BSS_9BC_ENTRY *entry;

    for (i = 0, entry = lbl_1_bss_9BC; i < 4; i++, entry++) {
        if (entry->unk_4 == 1 && entry->unk_0 == 0) {
            lbl_1_data_E12[entry->chrSel].unk_10 = -1;
            fn_1_AF70(i);
            break;
        }
    }
    for (i = 0; i < 11; i++) {
        fn_1_122FC(i);
    }
    obj->objFunc = NULL;
    lbl_1_bss_24->mtnId[0] = 1;
}

void fn_1_16A8C(OMOBJ *obj)
{
    LBL_1_BSS_9BC_ENTRY *player;
    LBL_1_BSS_288_ENTRY *entry;
    float value;
    s32 modelNo;
    s16 i;
    s16 time;

    if (obj->work[0] == 0) {
        for (i = 0; i < 4; i++) {
            lbl_1_bss_D4[i].x = 0.0f;
        }
    }
    for (i = 0, player = lbl_1_bss_9BC; i < 4; i++, player++) {
        time = obj->work[0];
        modelNo = (s16)player->chrSel;
        entry = &lbl_1_bss_288[modelNo];
        if (time == 0) {
            if (lbl_1_bss_34 == 0) {
                HuAudFXPlay(0x35);
                lbl_1_bss_34 = 1;
            }
            Hu3DModelPosGet(entry->modelId, &entry->pos);
            Hu3DModelRotGet(entry->modelId, &entry->rot);
            Hu3DModelScaleGet(entry->modelId, &entry->scale);
        } else {
            lbl_1_bss_34 = 0;
        }
        value = fn_1_1868(
            entry->rot.y, entry->rot.y - 540.0f, time, 10.0f);
        Hu3DModelRotSet(entry->modelId, 0.0f, value, 0.0f);
        value = fn_1_1868(entry->scale.x, 0.0f, time, 10.0f);
        Hu3DModelScaleSet(entry->modelId, value, value, 1.0f);
        if (time <= 10) {
            Hu3DModelAttrReset(entry->modelId, HU3D_ATTR_DISPOFF);
        }
    }
    if (obj->work[0]++ > 10) {
        obj->objFunc = NULL;
        lbl_1_bss_24->mtnId[0] = 1;
    }
}

void fn_1_1733C(s16 arg0)
{
    HuVecF value;
    LBL_1_BSS_288_ENTRY *entry = &lbl_1_bss_288[arg0 + 11];

    Hu3DModelPosGet(entry->modelId, &value);
    fn_1_163C(&value, &lbl_1_data_8A4[arg0 + 7], 5.0f);
    Hu3DModelPosSetV(entry->modelId, &value);
    if (entry->unk_30++ > 30) {
        entry->unk_30 = 35;
        Hu3DModelRotGet(entry->modelId, &value);
        value.y += 3.0f;
        if (value.y >= 360.0f) {
            value.y -= 360.0f;
        }
        Hu3DModelRotSetV(entry->modelId, &value);
    }
    Hu3DModelScaleGet(entry->modelId, &value);
    value.y = value.x = fn_1_160C(value.x, 2.25f, 5.0f);
    Hu3DModelScaleSetV(entry->modelId, &value);
    Hu3DModelLayerSet(entry->modelId, 2);
}

void fn_1_1765C(s16 arg0)
{
    LBL_1_BSS_288_ENTRY *entry = &lbl_1_bss_288[arg0 + 11];

    Hu3DModelPosSetV(entry->modelId, &lbl_1_data_8A4[arg0 + 1]);
    Hu3DModelRotSet(entry->modelId, 0.0f, 0.0f, 0.0f);
    Hu3DModelScaleSet(entry->modelId, 1.25f, 1.25f, 1.0f);
    entry->unk_30 = 0;
    Hu3DModelLayerSet(entry->modelId, 1);
}

void fn_1_17E10(OMOBJ *obj)
{
    s16 i;

    for (i = 0; i < 6; i++) {
        lbl_1_bss_288[i + 11].unk_30 = 0;
    }
    fn_1_A970(0, &lbl_1_data_8A4[0], -240.0f, 0.0f);
    fn_1_A970(1, &lbl_1_data_8A4[0], 240.0f, 0.0f);
    obj->work[0] = 10;
    obj->objFunc = fn_1_17730;
    lbl_1_bss_24->mtnId[0] = 1;
}

void fn_1_1807C(OMOBJ *obj)
{
    s16 i;

    if (obj->work[0] == 0) {
        fn_1_AA94(0);
        fn_1_AA94(1);
    }
    for (i = 0; i < 6; i++) {
        lbl_1_bss_288[i + 11].unk_30 = 0;
        fn_1_D3E0(i + 11, 0.0f, obj->work[0], 10);
    }
    if (obj->work[0]++ > 10) {
        obj->objFunc = NULL;
        lbl_1_bss_24->mtnId[0] = 1;
    }
}

void fn_1_18A10(OMOBJ *obj)
{
    OMOBJ *workObj;
    OMOBJ *workObj2;

    if (obj->work[0] == 0) {
        workObj = lbl_1_bss_8;
        workObj->work[0] = 2;
        workObj->work[1] = 0;
        workObj2 = lbl_1_bss_8;
        workObj2->work[2] = 2;
        workObj2->work[3] = 0;
    }
    if (obj->work[0]++ > 30) {
        obj->objFunc = NULL;
        lbl_1_bss_24->mtnId[0] = 1;
    }
}

void fn_1_1A124(OMOBJ *obj)
{
    HuSprAttrSet(lbl_1_bss_918[2], 2, HUSPR_ATTR_DISPOFF);
    lbl_1_data_DE6[2] = 0;
    HuSprAttrSet(lbl_1_bss_918[2], 3, HUSPR_ATTR_DISPOFF);
    lbl_1_data_DE6[3] = 0;
    HuSprAttrSet(lbl_1_bss_918[2], 4, HUSPR_ATTR_DISPOFF);
    lbl_1_data_DE6[4] = 0;
    HuSprAttrSet(lbl_1_bss_918[2], 5, HUSPR_ATTR_DISPOFF);
    lbl_1_data_DE6[5] = 0;
    obj->objFunc = NULL;
    lbl_1_bss_24->mtnId[0] = 1;
}

void fn_1_21628(OMOBJ *obj)
{
    LBL_1_BSS_9BC_ENTRY *entry;
    HuVecF rot;
    s16 i;

    if (obj->work[0]++ > 10) {
        if (HuPadDStkRep[0] & PAD_BUTTON_LEFT) {
            lbl_1_bss_9F4.unk_E--;
            if (lbl_1_bss_9F4.unk_E < 0) {
                lbl_1_bss_9F4.unk_E += 3;
            }
            obj->work[0] = 0;
            obj->work[1] = 0;
            fn_1_AAF0(4);
        } else if (HuPadDStkRep[0] & PAD_BUTTON_RIGHT) {
            lbl_1_bss_9F4.unk_E++;
            if (lbl_1_bss_9F4.unk_E > 2) {
                lbl_1_bss_9F4.unk_E -= 3;
            }
            obj->work[0] = 0;
            obj->work[1] = 1;
            fn_1_AAF0(5);
        }
    }
    if (obj->work[0] < 10) {
        for (i = 0; i < 4; i++) {
            entry = &lbl_1_bss_9BC[
                lbl_1_data_4[lbl_1_bss_9F4.unk_E][i]];
            Hu3DModelRotGet(
                lbl_1_bss_288[entry->chrSel].modelId, &rot);
            if (obj->work[1]) {
                rot.y += 18.0f;
                if (rot.y > 90.0f) {
                    Hu3DModelPosSetV(
                        lbl_1_bss_288[entry->chrSel].modelId,
                        &lbl_1_data_8A4[i + 49]);
                }
            } else {
                rot.y -= 18.0f;
                if (rot.y < -90.0f) {
                    Hu3DModelPosSetV(
                        lbl_1_bss_288[entry->chrSel].modelId,
                        &lbl_1_data_8A4[i + 49]);
                }
            }
            Hu3DModelRotSetV(
                lbl_1_bss_288[entry->chrSel].modelId, &rot);
        }
    } else {
        for (i = 0; i < 4; i++) {
            entry = &lbl_1_bss_9BC[
                lbl_1_data_4[lbl_1_bss_9F4.unk_E][i]];
            Hu3DModelRotGet(
                lbl_1_bss_288[entry->chrSel].modelId, &rot);
            rot.y = 0.0f;
            Hu3DModelRotSetV(
                lbl_1_bss_288[entry->chrSel].modelId, &rot);
        }
    }
}

void fn_1_219C8(OMOBJ *obj)
{
    HuVecF pos;

    Hu3DModelPosGet(
        lbl_1_bss_288[lbl_1_bss_9BC[0].chrSel].modelId, &pos);
    pos.x = 0.0f;
    fn_1_A970(4, &pos, -210.0f, 0.0f);
    fn_1_A970(5, &pos, 210.0f, 0.0f);
    obj->work[0] = 10;
    obj->objFunc = fn_1_21628;
    lbl_1_bss_24->mtnId[0] = 1;
}

void fn_1_21C04(OMOBJ *obj)
{
    HuSprAttrSet(lbl_1_bss_918[2], 4, HUSPR_ATTR_DISPOFF);
    lbl_1_data_DE6[4] = 0;
    HuSprAttrSet(lbl_1_bss_918[2], 5, HUSPR_ATTR_DISPOFF);
    lbl_1_data_DE6[5] = 0;
    obj->objFunc = NULL;
    lbl_1_bss_24->mtnId[0] = 1;
}

void fn_1_21C9C(OMOBJ *obj)
{
    LBL_1_BSS_9BC_ENTRY *entry;
    s16 i;

    lbl_1_bss_9F4.unk_E = 0;
    lbl_1_bss_9F4.unk_10 = 0x30037;
    lbl_1_bss_9F4.unk_14 = 0x30037;
    if (obj->work[0] == 0) {
        for (i = 0; i < 4; i++) {
            entry = &lbl_1_bss_9BC[i];
            fn_1_C158(entry->chrSel, 0, entry->chrSel, 1);
            fn_1_C158(entry->chrSel, 1, entry->chrSel, 1);
            fn_1_C158(entry->chrSel, 2, entry->chrSel, 1);
            fn_1_C158(entry->chrSel, 3, entry->chrSel, 1);
        }
        fn_1_96E4(
            lbl_1_bss_9F4.unk_10, lbl_1_bss_9F4.unk_14);
        fn_1_9E9C();
    }
    if (obj->work[0]++ > 10) {
        obj->objFunc = NULL;
        lbl_1_bss_24->mtnId[0] = 1;
    }
}

void fn_1_22010(OMOBJ *obj)
{
    LBL_1_BSS_9BC_ENTRY *entry;
    s16 i;

    lbl_1_bss_9F4.unk_10 = 0x30037;
    lbl_1_bss_9F4.unk_14 = 0x30037;
    if (obj->work[0] == 0) {
        for (i = 0; i < 4; i++) {
            entry = &lbl_1_bss_9BC[i];
            fn_1_C158(entry->chrSel, 0, entry->chrSel, 1);
            fn_1_C158(entry->chrSel, 2, entry->chrSel, 1);
        }
        fn_1_9658(0, lbl_1_bss_9F4.unk_10);
        fn_1_9658(1, lbl_1_bss_9F4.unk_14);
    }
    if (obj->work[0]++ > 10) {
        obj->objFunc = NULL;
        lbl_1_bss_24->mtnId[0] = 1;
    }
}

void fn_1_227E4(OMOBJ *obj)
{
    HUSPR_GROUP *group;
    s16 groupId;
    s16 i;

    if (obj->work[0] == 0) {
        HuWinDispOff(lbl_1_bss_104[0]);
        HuWinDispOff(lbl_1_bss_104[1]);
        groupId = lbl_1_bss_918[13];
        group = &HuSprGrpData[groupId];
        for (i = 0; i < group->sprNum; i++) {
            HuSprAttrSet(groupId, i, HUSPR_ATTR_DISPOFF);
        }
        lbl_1_bss_80.x = 0.0f;
    }
    if (obj->work[0]++ > 10) {
        obj->objFunc = NULL;
        lbl_1_bss_24->mtnId[0] = 1;
    }
}

void fn_1_22EB8(OMOBJ *obj)
{
    LBL_1_BSS_9BC_ENTRY *entry;
    HuVecF pos;
    s16 i;

    for (i = 0; i < 4; i++) {
        entry = &lbl_1_bss_9BC[
            lbl_1_data_4[lbl_1_bss_9F4.unk_E][i]];
        if (entry->unk_4 == 1 && entry->unk_0 == 1) {
            Hu3DModelPosGet(
                lbl_1_bss_288[entry->chrSel].modelId, &pos);
            fn_1_A970(4, &pos, -40.0f, -45.0f);
            fn_1_A970(5, &pos, 40.0f, -45.0f);
            fn_1_9B90(entry->unk_A, entry->unk_6 + 5);
            break;
        }
    }
    obj->work[0] = 10;
    obj->objFunc = fn_1_228E8;
    lbl_1_bss_24->mtnId[0] = 1;
}

void fn_1_2318C(OMOBJ *obj)
{
    HuSprAttrSet(lbl_1_bss_918[2], 4, HUSPR_ATTR_DISPOFF);
    lbl_1_data_DE6[4] = 0;
    HuSprAttrSet(lbl_1_bss_918[2], 5, HUSPR_ATTR_DISPOFF);
    lbl_1_data_DE6[5] = 0;
    obj->objFunc = NULL;
    lbl_1_bss_24->mtnId[0] = 1;
}

void fn_1_23224(OMOBJ *obj)
{
    s16 i;
    LBL_1_BSS_9BC_ENTRY *entry;

    for (i = 0; i < 4; i++) {
        entry = &lbl_1_bss_9BC[i];
        if (entry->unk_4 == 1) {
            entry->unk_6 = 1;
        }
    }
    obj->objFunc = NULL;
    lbl_1_bss_24->mtnId[0] = 1;
}

void fn_1_239DC(OMOBJ *obj)
{
    obj->objFunc = NULL;
    lbl_1_bss_24->mtnId[0] = 1;
}

void fn_1_23A00(OMOBJ *obj)
{
    LBL_1_BSS_9BC_ENTRY *entry;
    s16 i;

    for (i = 0; i < 4; i++) {
        entry = &lbl_1_bss_9BC[i];
        if (entry->unk_4 == 1) {
            entry->unk_6 = 1;
            fn_1_9B90(i, 4);
        }
    }
    obj->objFunc = NULL;
    lbl_1_bss_24->mtnId[0] = 1;
}

void fn_1_249A4(OMOBJ *obj)
{
    HuSprAttrSet(lbl_1_bss_918[2], 2, HUSPR_ATTR_DISPOFF);
    lbl_1_data_DE6[2] = 0;
    HuSprAttrSet(lbl_1_bss_918[2], 3, HUSPR_ATTR_DISPOFF);
    lbl_1_data_DE6[3] = 0;
    HuSprAttrSet(lbl_1_bss_918[2], 4, HUSPR_ATTR_DISPOFF);
    lbl_1_data_DE6[4] = 0;
    HuSprAttrSet(lbl_1_bss_918[2], 5, HUSPR_ATTR_DISPOFF);
    lbl_1_data_DE6[5] = 0;
    obj->objFunc = NULL;
    lbl_1_bss_24->mtnId[0] = 1;
}

void fn_1_28044(OMOBJ *obj)
{
    HuVecF pos;

    Hu3DModelPosGet(
        lbl_1_bss_288[lbl_1_bss_9F4.unk_18 + 21].modelId, &pos);
    fn_1_A970(2, &pos, 0.0f, -65.0f);
    fn_1_A970(3, &pos, 0.0f, 65.0f);
    pos.x = 0.0f;
    fn_1_A970(4, &pos, -220.0f, 0.0f);
    fn_1_A970(5, &pos, 220.0f, 0.0f);
    obj->work[0] = 10;
    obj->objFunc = fn_1_26328;
    lbl_1_bss_24->mtnId[0] = 1;
}

void fn_1_28434(OMOBJ *obj)
{
    HuSprAttrSet(lbl_1_bss_918[2], 2, HUSPR_ATTR_DISPOFF);
    lbl_1_data_DE6[2] = 0;
    HuSprAttrSet(lbl_1_bss_918[2], 3, HUSPR_ATTR_DISPOFF);
    lbl_1_data_DE6[3] = 0;
    HuSprAttrSet(lbl_1_bss_918[2], 4, HUSPR_ATTR_DISPOFF);
    lbl_1_data_DE6[4] = 0;
    HuSprAttrSet(lbl_1_bss_918[2], 5, HUSPR_ATTR_DISPOFF);
    lbl_1_data_DE6[5] = 0;
    obj->objFunc = NULL;
    lbl_1_bss_24->mtnId[0] = 1;
}

void fn_1_2C874(void)
{
    fn_1_87BC(lbl_1_bss_4);
    fn_1_945C(lbl_1_bss_8);
    fn_1_6284(lbl_1_bss_C);
    fn_1_72C4(lbl_1_bss_10);
    fn_1_8074(lbl_1_bss_14);
    fn_1_A880();
    fn_1_B6C0(lbl_1_bss_1C.obj);
    fn_1_4581C();
    fn_1_D978();
    fn_1_3C44();
    fn_1_36C4();
    fn_1_353C();
}

void fn_1_2CD6C(OMOBJ *obj)
{
    if (!WipeCheck()) {
        fn_1_2C874();
        omOvlReturnEx(1, 1);
    }
}

void fn_1_2CDA8(OMOBJ *obj)
{
    if (omSysExitReq != 0) {
        WipeCreate(WIPE_MODE_OUT, WIPE_TYPE_NORMAL, 60);
        obj->objFunc = fn_1_2CD6C;
    }
}

void fn_1_2CE00(void)
{
    s16 result = 0;

    result = fn_1_3EAC8(0);
    if (lbl_1_data_0 != -1) {
        HuAudSStreamFadeOut(lbl_1_data_0, 1000);
        lbl_1_data_0 = -1;
    }
    switch (result) {
        case 0:
        case 1:
            WipeCreate(WIPE_MODE_OUT, WIPE_TYPE_NORMAL, 60);
            while (WipeCheck()) {
                HuPrcVSleep();
            }
            break;
        case 2:
        case 3:
            WipeCreate(WIPE_MODE_OUT, WIPE_TYPE_NORMAL, 60);
            while (WipeCheck()) {
                HuPrcVSleep();
            }
            break;
    }
    fn_1_2C874();
    switch (result) {
        case 0:
            fn_1_12F8();
            omOvlReturnEx(1, 1);
            break;
        case 1:
            fn_1_109C(1);
            break;
        case 2:
        case 3:
            fn_1_109C(0);
            break;
    }
    HuPrcEnd();
    while (TRUE) {
        HuPrcVSleep();
    }
}

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

void fn_1_2E338(OMOBJ *obj, MDCAMERA_WORK *camera)
{
    camera->center.y =
        fn_1_1868(camera->unk_10.y, 275.0f, camera->unk_40, 60.0f);
    camera->center.z =
        fn_1_1868(camera->unk_10.z, -2900.0f, camera->unk_40, 60.0f);
    camera->rot.x =
        fn_1_1868(camera->unk_28.x, 0.0f, camera->unk_40, 60.0f);
    camera->unk_40 += 1.0f;
}

void fn_1_2E560(OMOBJ *obj, MDCAMERA_WORK *camera)
{
    camera->unk_40 = 0.0f;
    fn_1_2810(camera);
    fn_1_2B64(fn_1_2E338);
}

s32 fn_1_2E5D4(void)
{
    HuPrcSleep(5);
    lbl_1_data_0 = HuAudSStreamPlay(4);
    WipeCreate(WIPE_MODE_IN, WIPE_TYPE_NORMAL, 60);
    while (WipeCheck()) {
        HuPrcVSleep();
    }
    return TRUE;
}

void fn_1_2EBEC(void)
{
    HuVecF pos;

    Hu3DModelPosGet(lbl_1_bss_C->mdlId[0], &pos);
    if (pos.x > -200) {
        fn_1_3E0C();
        Hu3DMotionShiftSet(lbl_1_bss_C->mdlId[0],
            lbl_1_bss_C->mtnId[5], 0.0f, 30.0f, 0);
        Hu3DMotionShiftSet(lbl_1_bss_10->mdlId[0],
            lbl_1_bss_10->mtnId[5], 0.0f, 30.0f, 0);
        HuPrcSleep(60);
        fn_1_5FB0();
        fn_1_6FB8();
        fn_1_3E0C();
        HuPrcSleep(90);
    }
}

s16 fn_1_2EF24(void)
{
    fn_1_52B0();
    fn_1_4020(2, 0xD0008, 1);
    return fn_1_3F28(2);
}

s16 fn_1_32904(void)
{
    s16 count;
    s16 i;

    for (i = 0, count = 0; i < 4; i++) {
        if (lbl_1_bss_9BC[i].unk_0 == 1) {
            count++;
        }
    }
    if (lbl_1_bss_9F4.unk_2 <= count) {
        return 0;
    }
    if (lbl_1_bss_9BC[0].unk_0 == 0) {
        return 1;
    }
    return -1;
}

s16 fn_1_335F0(s16 *arg0, s16 *arg1, s16 *arg2)
{
    s16 i;
    s16 count = 0;

    *arg0 = 0;
    *arg1 = 0;
    *arg2 = 3;
    for (i = 0; i < 4; i++) {
        if (lbl_1_bss_9BC[i].unk_4 == 1) {
            *arg1 = i;
            break;
        }
    }
    for (i = 0; i < 4; i++) {
        if (lbl_1_bss_9BC[i].unk_4 == 1
            && lbl_1_bss_9BC[i].unk_0 != 2) {
            break;
        }
    }
    *arg0 = i;
    for (i = 0; i < 4; i++) {
        if (lbl_1_bss_9BC[i].unk_4 == 1
            && lbl_1_bss_9BC[i].unk_0 != 2) {
            count++;
        }
    }
    return count;
}

s16 fn_1_34D10(s16 arg0)
{
    s16 result = 0;
    s16 flag = 0;

    if (arg0 != 0) {
        OMOBJ *obj = lbl_1_bss_24;

        lbl_1_bss_24->mtnId[0] = 0;
        obj->work[0] = 0;
        obj->work[1] = 0;
        obj->work[2] = 0;
        obj->work[3] = 0;
        obj->objFunc = fn_1_16A8C;
        while (lbl_1_bss_24->mtnId[0] == 0) {
            HuPrcVSleep();
        }
    }
    {
        OMOBJ *obj = lbl_1_bss_24;

        lbl_1_bss_24->mtnId[0] = 0;
        obj->work[0] = 0;
        obj->work[1] = 0;
        obj->work[2] = 0;
        obj->work[3] = 0;
        obj->objFunc = fn_1_150D0;
        while (lbl_1_bss_24->mtnId[0] == 0) {
            HuPrcVSleep();
        }
    }
    switch (lbl_1_bss_9F4.unk_2) {
        case 0:
            result = fn_1_33724(0);
            break;
        case 4:
            result = fn_1_329A4(0);
            break;
        default:
            while (TRUE) {
                HuPrcVSleep();
                result = fn_1_329A4(flag);
                if (result != 0) {
                    break;
                }
                result = fn_1_33724(0);
                if (result != 1) {
                    break;
                }
                flag = 1;
            }
            break;
    }
    if (result == 0) {
        OMOBJ *obj = lbl_1_bss_24;

        lbl_1_bss_24->mtnId[0] = 0;
        obj->work[0] = 0;
        obj->work[1] = 0;
        obj->work[2] = 0;
        obj->work[3] = 0;
        obj->objFunc = fn_1_156B4;
        while (lbl_1_bss_24->mtnId[0] == 0) {
            HuPrcVSleep();
        }
    } else if (result == 1) {
        OMOBJ *obj = lbl_1_bss_24;

        lbl_1_bss_24->mtnId[0] = 0;
        obj->work[0] = 0;
        obj->work[1] = 0;
        obj->work[2] = 0;
        obj->work[3] = 0;
        obj->objFunc = fn_1_16E84;
        while (lbl_1_bss_24->mtnId[0] == 0) {
            HuPrcVSleep();
        }
    }
    return result;
}

s16 fn_1_34FC4(s16 *arg0, s16 *arg1, s16 *arg2)
{
    s16 i;
    s16 count = 0;

    *arg0 = 0;
    *arg1 = 0;
    *arg2 = 3;
    for (i = 0; i < 4; i++) {
        if (lbl_1_bss_9BC[i].unk_4 == 1) {
            *arg1 = i;
            break;
        }
    }
    for (i = 0; i < 4; i++) {
        if (lbl_1_bss_9BC[i].unk_4 == 1
            && lbl_1_bss_9BC[i].unk_0 != 1) {
            break;
        }
    }
    *arg0 = i;
    for (i = 0; i < 4; i++) {
        if (lbl_1_bss_9BC[i].unk_4 == 1
            && lbl_1_bss_9BC[i].unk_0 != 1) {
            count++;
        }
    }
    return count;
}

s16 fn_1_36264(s16 arg0)
{
    s16 result = 0;
    s16 flag = 0;

    if (arg0 != 0) {
        OMOBJ *obj = lbl_1_bss_24;

        lbl_1_bss_24->mtnId[0] = 0;
        obj->work[0] = 0;
        obj->work[1] = 0;
        obj->work[2] = 0;
        obj->work[3] = 0;
        obj->objFunc = fn_1_16A8C;
        while (lbl_1_bss_24->mtnId[0] == 0) {
            HuPrcVSleep();
        }
    }
    {
        OMOBJ *obj = lbl_1_bss_24;

        lbl_1_bss_24->mtnId[0] = 0;
        obj->work[0] = 0;
        obj->work[1] = 0;
        obj->work[2] = 0;
        obj->work[3] = 0;
        obj->objFunc = fn_1_150D0;
        while (lbl_1_bss_24->mtnId[0] == 0) {
            HuPrcVSleep();
        }
    }
    switch (lbl_1_bss_9F4.unk_2) {
        case 0:
            result = fn_1_350F8(0);
            break;
        case 4:
            result = fn_1_329A4(0);
            break;
        default:
            while (TRUE) {
                HuPrcVSleep();
                result = fn_1_329A4(flag);
                if (result != 0) {
                    break;
                }
                result = fn_1_350F8(0);
                if (result != 1) {
                    break;
                }
                flag = 1;
            }
            break;
    }
    if (result == 0) {
        OMOBJ *obj = lbl_1_bss_24;

        lbl_1_bss_24->mtnId[0] = 0;
        obj->work[0] = 0;
        obj->work[1] = 0;
        obj->work[2] = 0;
        obj->work[3] = 0;
        obj->objFunc = fn_1_160A0;
        while (lbl_1_bss_24->mtnId[0] == 0) {
            HuPrcVSleep();
        }
    } else if (result == 1) {
        OMOBJ *obj = lbl_1_bss_24;

        lbl_1_bss_24->mtnId[0] = 0;
        obj->work[0] = 0;
        obj->work[1] = 0;
        obj->work[2] = 0;
        obj->work[3] = 0;
        obj->objFunc = fn_1_16E84;
        while (lbl_1_bss_24->mtnId[0] == 0) {
            HuPrcVSleep();
        }
    }
    return result;
}

void fn_1_37260(s16 *arg0, s16 *arg1, s16 *arg2)
{
    LBL_1_BSS_9BC_ENTRY *entry;
    s16 i;

    *arg0 = 0;
    *arg1 = 0;
    *arg2 = 3;
    for (i = 0; i < 4; i++) {
        entry = &lbl_1_bss_9BC[lbl_1_data_4[lbl_1_bss_9F4.unk_E][i]];
        if (entry->unk_4 == 1) {
            *arg1 = i;
            break;
        }
    }
    for (i = 0; i < 4; i++) {
        entry = &lbl_1_bss_9BC[lbl_1_data_4[lbl_1_bss_9F4.unk_E][i]];
        if (entry->unk_4 == 1 && entry->unk_0 != 2) {
            break;
        }
    }
    *arg0 = i;
}

s32 lbl_1_data_0 = -1;
s16 lbl_1_data_4[3][4] = {
    { 0, 1, 2, 3 },
    { 0, 2, 1, 3 },
    { 0, 3, 1, 2 },
};
s32 lbl_1_data_BC8 = -1;
char lbl_1_data_BCC[] = "# ========== win callback :: %d\n";
s16 lbl_1_data_BEE[7] = { 0, 0, 0, 0, 0, 0, 0 };
u32 lbl_1_data_BFC[14] = {
    DATA_w01,
    DATA_w02,
    DATA_w03,
    DATA_w04,
    DATA_w05,
    DATA_w06,
    DATA_w10,
    DATA_w01n,
    DATA_w02n,
    DATA_w03n,
    DATA_w04n,
    DATA_w05n,
    DATA_w06n,
    DATA_w10n,
};
char lbl_1_data_C34[] =
    "\n>>>>>>>>>> mdpartydll :: READ INIT!! <<<<<<<<<<\n";
char lbl_1_data_C66[] = "0x%x :: _effect\n";
char lbl_1_data_C77[] = "0x%x :: _gamemes\n";
char lbl_1_data_C89[] = "0x%x :: _mgconst\n";
char lbl_1_data_C9B[] = "0x%x :: _win\n";
char lbl_1_data_CA9[] = "\n";
char lbl_1_data_CAB[] =
    "\n>>>>>>>>>> mdpartydll :: READ END!! <<<<<<<<<<\n";
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
s16 lbl_1_bss_2E;
OMOBJ *lbl_1_bss_30;
s16 lbl_1_bss_34;
float lbl_1_bss_38[6];
HuVecF lbl_1_bss_50[4];
HuVecF lbl_1_bss_80;
HuVecF lbl_1_bss_8C[2];
HuVecF lbl_1_bss_A4[4];
HuVecF lbl_1_bss_D4[4];
HUWINID lbl_1_bss_104[2];
LBL_1_BSS_1C8_ENTRY lbl_1_bss_108[4];
LBL_1_BSS_1C8_ENTRY lbl_1_bss_1C8[2];
LBL_1_BSS_228_ENTRY lbl_1_bss_228[2];
LBL_1_BSS_288_ENTRY lbl_1_bss_288[25];
ANIMDATA *lbl_1_bss_800[41];
HUSPRID lbl_1_bss_8A4[58];
HUSPR_GROUPID lbl_1_bss_918[18];
ANIMDATA *lbl_1_bss_93C[32];
LBL_1_BSS_9BC_ENTRY lbl_1_bss_9BC[4];
LBL_1_BSS_9F4 lbl_1_bss_9F4;
MDCAMERA_WORK lbl_1_bss_A10;
HUWINID lbl_1_bss_A60[4];
HU3D_LIGHTID lbl_1_bss_A68[2];
s32 lbl_1_bss_A6C[5];
