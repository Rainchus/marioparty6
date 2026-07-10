#include "dolphin.h"
#include "game/card.h"
#include "game/process.h"
#include "game/window.h"
#include "game/sprite.h"
#include "game/hu3d.h"
#include "game/object.h"
#include "game/gamework.h"
#include "game/audio.h"
#include "game/frand.h"
#include "game/pad.h"
#include "game/data.h"
#include "game/memory.h"
#include "game/main.h"
#include "game/wipe.h"
#include "math.h"
#include "string.h"
#include "stdio.h"

typedef void (*VoidFunc)(void);

typedef struct filesel_work {
    /* 0x00 */ s32 unk_0;
    /* 0x04 */ s16 unk_4;          /* giant3 fn_1_36C4: sprite-group id (was unk_04[4]) */
    /* 0x06 */ s16 unk_6;          /* giant3 fn_1_36C4: HuWin winId */
    /* 0x08 */ s16 unk_8;
    /* 0x0A */ s16 unk_A;          /* giant4 fn_1_3D14: Hu3D model id (was unk_0A[8]) */
    /* 0x0C */ s16 unk_C;          /* giant4 fn_1_3D14: Hu3D model id */
    /* 0x0E */ s16 unk_E;          /* giant4 fn_1_3D14: Hu3D model id */
    /* 0x10 */ s16 unk_10;         /* giant4 fn_1_3D14: Hu3D model id */
    /* 0x12 */ s16 unk_12;
    /* 0x14 */ s16 unk_14;
    /* 0x16 */ s16 unk_16;
    /* 0x18 */ s16 unk_18;
    /* 0x1A */ s16 unk_1A;         /* giant3 fn_1_36C4: Hu3DAnimAnimSet target (was unk_1A[2]) */
    /* 0x1C */ OSCalendarTime unk_1C;
    /* 0x44 */ char unk_44[0x11];
    /* 0x55 */ u8 unk_55[0x03];
    /* 0x58 */ ANIMDATA *unk_58; /* fn_1_2C6C: HuSprAnimMake result (p->unk_58->bmp->data) */
    /* 0x5C */ void *unk_5C;
    /* 0x60 */ u8 unk_60[2];
    /* 0x62 */ u8 unk_62;
    /* 0x63 */ u8 unk_63;
    /* 0x64 */ Vec unk_64;
    /* 0x70 */ Vec unk_70;         /* giant4 fn_1_3D14: wave offset (was u8[0x10]) */
    /* 0x7C */ void *unk_7C;       /* giant4 fn_1_3D14: HUPROCESS* owner */
} FILESEL_WORK; /* size = 0x80 */

/* lbl_1_bss_348 element (fn_1_8F34 / fn_1_ABF8 / fn_1_B234), 6 bytes */
typedef struct b234_point { s16 x; s16 y; u16 flag; } B234Point;
/* fn_1_B234 out rect */
typedef struct b234_rect { f32 x0; f32 y0; f32 x1; f32 y1; } B234Rect;

/* fn_1_7E58 arg-chain views (only used offsets matter) */
typedef struct f7e58_q { u8 pad_00[0x18]; GXColor color; f32 unk_1c; f32 unk_20; } F7E58_Q;
typedef struct f7e58_p { u8 pad_00[0x120]; F7E58_Q *unk_120; } F7E58_P;
typedef struct f7e58_arg { F7E58_P *unk_00; u8 pad_04[8]; Mtx unk_0c; } F7E58_Arg;

/* lbl_1_bss_18 element (fn_1_8510 / fn_1_8688 / fn_1_82F0), stride 0x28 */
typedef struct bss18_entry {
    /* 0x00 */ Vec unk_0;
    /* 0x0C */ Vec unk_c;
    /* 0x18 */ GXColor unk_18;
    /* 0x1C */ f32 unk_1c;
    /* 0x20 */ f32 unk_20;
    /* 0x24 */ f32 unk_24;
} BSS18_ENTRY; /* size = 0x28 */

/* fn_1_7B64 rodata block */
typedef struct vec5 { Vec v[5]; } VEC5;

/* lbl_1_data_48C[] element (fn_1_8B94 sprite table), stride 0xC */
typedef struct data48c {
    /* 0x0 */ s32 unk_0;
    /* 0x4 */ u8  unk_4;
    /* 0x5 */ u8  pad_5;
    /* 0x6 */ s16 unk_6;
    /* 0x8 */ s16 unk_8;
    /* 0xA */ s16 unk_A;
} DATA48C; /* size 0xC */

/* ==================== module .bss/.data (no header; ref by dtk name) ==================== */
extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];

extern s16 lbl_1_bss_2E0;
extern FILESEL_WORK lbl_1_bss_D8[];
extern CARDFileInfo curFileInfo;

extern OMOBJ *lbl_1_bss_8;
extern void *lbl_1_bss_C;
extern ANIMDATA *lbl_1_bss_10;
extern ANIMDATA *lbl_1_bss_14;
extern BSS18_ENTRY lbl_1_bss_18[];
extern HU3D_PARMANID lbl_1_bss_98[];
extern HU3D_ANIMID lbl_1_bss_A2[];
extern ANIMDATA *lbl_1_bss_C8;
extern ANIMDATA *lbl_1_bss_CC[];
extern HU3D_MODELID lbl_1_bss_2D8[];
extern OMOBJMAN *lbl_1_bss_340;
extern OMOBJ *lbl_1_bss_344;
extern B234Point *lbl_1_bss_348;
extern char lbl_1_bss_34C[];
extern s16 lbl_1_bss_3AC[];
extern u8 lbl_1_bss_3B2[][0x11];
extern s16 lbl_1_bss_42A;
extern s16 lbl_1_bss_42E;
extern Vec lbl_1_bss_68[];

extern OM_CAMERA_VIEW lbl_1_data_0;
extern OM_CAMERA_VIEW lbl_1_data_1C;
extern HU3D_PARMAN_PARAM lbl_1_data_E8;
extern char lbl_1_data_138[];
extern char lbl_1_data_144[];
extern char lbl_1_data_1FD[0x17];
extern DATA48C lbl_1_data_48C[];
extern char lbl_1_data_56C[];
extern char lbl_1_data_572[];
extern char lbl_1_data_58B[];
extern char lbl_1_data_590[];
extern char lbl_1_data_59F[];
extern char lbl_1_data_5A4[];
extern char lbl_1_data_B8[];
extern char lbl_1_data_E2[];
extern u32 lbl_1_data_568;

/* module .rodata pool (no header; ref by dtk name — relocations pair by name) */
extern const f32 lbl_1_rodata_10, lbl_1_rodata_14, lbl_1_rodata_18, lbl_1_rodata_1C;
extern const f32 lbl_1_rodata_20, lbl_1_rodata_24, lbl_1_rodata_28, lbl_1_rodata_2C;
extern const f32 lbl_1_rodata_30, lbl_1_rodata_34, lbl_1_rodata_38, lbl_1_rodata_3C;
extern const f32 lbl_1_rodata_40, lbl_1_rodata_44, lbl_1_rodata_48, lbl_1_rodata_4C;
extern const f32 lbl_1_rodata_50, lbl_1_rodata_54, lbl_1_rodata_58, lbl_1_rodata_5C;
extern const f32 lbl_1_rodata_60, lbl_1_rodata_64, lbl_1_rodata_68, lbl_1_rodata_6C;
extern const f32 lbl_1_rodata_70, lbl_1_rodata_74;
extern const f32 lbl_1_rodata_C0;
extern const double lbl_1_rodata_80, lbl_1_rodata_88, lbl_1_rodata_90, lbl_1_rodata_98;
extern const double lbl_1_rodata_108, lbl_1_rodata_190, lbl_1_rodata_1B0;
extern const f32 lbl_1_rodata_AC, lbl_1_rodata_184, lbl_1_rodata_1A8, lbl_1_rodata_1C0;
extern const double lbl_1_rodata_1B8, lbl_1_rodata_1C8, lbl_1_rodata_F8, lbl_1_rodata_100;
extern const Vec lbl_1_rodata_19C;
extern char lbl_1_data_1F3[];
extern const f32 lbl_1_rodata_118, lbl_1_rodata_11C, lbl_1_rodata_120, lbl_1_rodata_124;
extern const double lbl_1_rodata_130, lbl_1_rodata_138, lbl_1_rodata_148;
extern const f32 lbl_1_rodata_17C;
extern const VEC5 lbl_1_rodata_1D0;
extern const double lbl_1_rodata_210;
extern const f32 lbl_1_rodata_218, lbl_1_rodata_220, lbl_1_rodata_224, lbl_1_rodata_228;
extern const f32 lbl_1_rodata_22C, lbl_1_rodata_230, lbl_1_rodata_234;
extern const GXColor lbl_1_rodata_238;
extern const f32 lbl_1_rodata_240, lbl_1_rodata_244, lbl_1_rodata_248, lbl_1_rodata_24C;
extern const f32 lbl_1_rodata_250, lbl_1_rodata_254, lbl_1_rodata_258, lbl_1_rodata_25C;
extern const f32 lbl_1_rodata_268, lbl_1_rodata_26C, lbl_1_rodata_270, lbl_1_rodata_280;
extern const f32 lbl_1_rodata_2C0, lbl_1_rodata_2C4, lbl_1_rodata_2D0, lbl_1_rodata_2D4;

/* ===== newly integrated giant functions: module .bss/.data/.rodata (ref by dtk name) ===== */
extern s16 lbl_1_bss_2DE;              /* HUWINID (giant2 fn_1_5244, giant3, giant5 fn_1_CB4) */
extern Vec lbl_1_bss_2E4[];            /* Vec[] scratch, stride 0xC (giant5 fn_1_CB4 writes [0..2]) */
extern ANIMDATA *lbl_1_bss_A8[];       /* ANIMDATA*[8] anim table (giant3 fn_1_36C4) */
extern s32 lbl_1_bss_0;                /* 457C enable flag (CameraMove moveF static) (giant5 fn_1_457C) */

extern char *lbl_1_data_88[];          /* bone-name string ptr table for Hu3DModelObjMtxGet (giant5 fn_1_CB4) */
extern Vec lbl_1_data_154[];           /* Vec[3] target offsets (giant5 fn_1_CB4) */
extern char *lbl_1_data_190[];         /* object-name strings for Hu3DModelObjMtxGet (giant4 fn_1_3D14) */
extern char lbl_1_data_1E6[];          /* "%02d" (giant3 fn_1_36C4) */
extern char lbl_1_data_1EB[];          /* "%04d" (giant3 fn_1_36C4) */
extern char lbl_1_data_1F0[];          /* "%d"   (giant3 fn_1_36C4) */

extern const f32 lbl_1_rodata_78, lbl_1_rodata_C4, lbl_1_rodata_D0, lbl_1_rodata_E8;
extern const f32 lbl_1_rodata_15C, lbl_1_rodata_178;                                  /* giant4 fn_1_3D14 */
extern const f32 lbl_1_rodata_140, lbl_1_rodata_150, lbl_1_rodata_154, lbl_1_rodata_158; /* giant3 fn_1_36C4 */
extern const double lbl_1_rodata_160, lbl_1_rodata_168, lbl_1_rodata_170;             /* giant4 fn_1_3D14 */

/* ===== final integration wave: fn_1_916C / fn_1_17CC / fn_1_2C6C (ref by dtk name) ===== */
extern s16 lbl_1_bss_37E;              /* fn_1_916C: entered-char count (name length)   */
extern u16 lbl_1_bss_380[];            /* fn_1_916C: temp buffer of entered char codes  */
extern s16 lbl_1_bss_42C;              /* fn_1_916C: number of grid pages               */
extern B234Point *lbl_1_data_47C[];    /* fn_1_916C: array of ptrs to grid-point tables */
extern const f32 lbl_1_rodata_274, lbl_1_rodata_278, lbl_1_rodata_27C; /* fn_1_916C */
extern const f32 lbl_1_rodata_EC;      /* fn_1_17CC: 130.0f                             */
extern const double lbl_1_rodata_F0;   /* fn_1_17CC: sin(130deg) denominator            */
extern int lbl_1_data_98[];            /* fn_1_2C6C: 8 data-file ids (preload loop)     */
extern char lbl_1_data_1A0[];          /* fn_1_2C6C: Hu3DAnimCreate bmpName             */
extern char lbl_1_data_1AD[];          /* fn_1_2C6C: Hu3DAnimCreate bmpName             */
extern char lbl_1_data_1BB[];          /* fn_1_2C6C: Hu3DAnimCreate bmpName             */
extern char lbl_1_data_1C8[];          /* fn_1_2C6C: Hu3DModelObjPtrGet objName         */
extern char lbl_1_data_1DD[];          /* fn_1_2C6C: strcpy src                         */
extern const f32 lbl_1_rodata_128, lbl_1_rodata_12C; /* fn_1_2C6C */

/* main-DOL globals (no header) */
extern s16 curSlotNo;
extern s16 SLWinId;
extern char SLSaveFileName[];
extern char SLEraseStr[];
extern u8 saveBuf[][0xA000];
extern s64 SLSerialNo[];
extern s32 SR_ExecResetMenu;

/* ==================== save-lib / mem-card funcs (no header) ==================== */
s32 SLSaveFlagGet(void);
void SLSaveFlagSet(s32 flag);
void SLWinIdSet(s16 winId);
s32 SLSerialNoCheck(void);
void SLSerialNoGet(void);
s32 SLStatSet(s32 stat);
void SLCheckSumBoxAllSet(void);
void SLSaveBackup(void);
s32 SLCurBoxNoSet(s32 boxNo);   /* fn_1_17CC: MUST return a value (non-void) — that is the match */
void SLSaveDataMake(s32 arg, OSTime *time);
u16 SLCheckSumGet(s32 start, s32 len);
void SLCurSlotNoSet(s32 slotNo);
void SLSaveEmptySet(s16 slot, s16 idx);
s32 SLBoxDataOffsetGet(s16 boxNo);
s32 SLCheckSumCheck(void);
void SLBoxBackupLoad(s16 boxNo);
void SLCommonLoad(void);
void SLCommonSet(void);   /* fn_1_17CC: no repo header declares this — TU-local extern */

s32 HuMCProbe(s32 chan);
void HuMCMicSet(s32 flag);
void HuMCInit(s32 chan);
s32 HuMCMount(s32 chan);
void HuMCClose(void);
s32 HuMCMicGet(void);

void HuSprTexLoad(ANIMDATA *anim, s16 bmpNo, s16 texMapId, s32 wrapS, s32 wrapT, s32 filter);

/* ==================== forward decls: functions defined in this TU ==================== */
void fn_1_A0(void);
void fn_1_374(void);
void fn_1_680(void);
void fn_1_6E8(void);
void fn_1_C60(void);
void fn_1_C8C(void);
void fn_1_179C(void);
void fn_1_2654(s16 arg);
void fn_1_2A40(void);
void fn_1_35AC(void);
void fn_1_3C80(s16 id, Vec *pos);
void fn_1_3CC0(s16 id, f32 x, f32 y, f32 z);
void fn_1_3CF4(s16 id, s32 flag);
void fn_1_50DC(void (*func)(void), s32 arg1);
void fn_1_5130(void);
void fn_1_6314(s16 id, s32 flag);
void fn_1_63A0(s16 id, s32 flag);
s32 fn_1_642C(s16 arg0);
s32 fn_1_69B0(void);
void fn_1_6A8C(void);
void fn_1_7B64(void);
void fn_1_7A0C(void);
void fn_1_7DEC(void);
void fn_1_7E58(F7E58_Arg *arg);
void fn_1_82F0(OMOBJ *obj);
void fn_1_8510(s16 id, Vec *pos);
void fn_1_8688(OMOBJ *p);
void fn_1_8A34(void);
void fn_1_8F34(s32 arg0, s16 idx);
s16 fn_1_AAEC(s16 winId);
s16 fn_1_ABF8(s16 id, f32 refX, f32 refY, f32 dirX, f32 dirY);
void fn_1_B234(B234Rect *r);
void fn_1_B530(void);
void fn_1_B5B4(void);
s32 fn_1_BF9C(void);
s32 fn_1_C098(s16 winId);
s32 fn_1_C278(void);
s32 fn_1_C2F0(s32 err, s16 winId);
s32 fn_1_C898(const char *fileName);
s32 fn_1_C9D0(s32 length, void *addr);
s32 fn_1_CA58(void);
s32 fn_1_CAA4(s16 slot);
s32 fn_1_CBC8(s16 arg);
s32 fn_1_CDA8(void);
s32 fn_1_CFF4(const char *fileName, s32 size, void *addr);
s32 fn_1_D348(s16 arg);
s32 fn_1_D448(s32 length, const void *addr);
s32 fn_1_D558(void);
s32 fn_1_D90C(s16 winId_in, s32 arg1);
HUWINID fn_1_DA18(u32 mesId, u32 insMes0, u32 insMes1, s16 arg3);
void fn_1_DB5C(s16 winId);
HUWINID fn_1_DBA0(u32 messNum, u32 insMesNum1, u32 insMesNum2, s16 posY);
void fn_1_DD24(s16 winId);
s16 fn_1_DD84(s16 mode);
void fn_1_3D14(void);         /* defined later; called by fn_1_2C6C (keep bl, no inline) */
s32 fn_1_5244(s16 arg);       /* defined later; called by fn_1_17CC (keep bl, no inline) */
s32 fn_1_916C(s16 no);        /* defined later; called by fn_1_17CC (keep bl, no inline) */

/* ==================== forward decls: callees NOT in this TU (pair by name) ==================== */
void fn_1_2C6C(void);
void fn_1_15AC(void);
void fn_1_36C4(s16 a, s16 b);
void fn_1_6E54(s16 a, s32 b);
void fn_1_8B94(void);
void fn_1_CB4(s32 arg);
s32 fn_1_17CC(void);
s32 fn_1_B63C(s16 arg);
s32 fn_1_E210(s16 mesNo, s16 winId);

/* ======================================================================== */
/* module entry */
/* ======================================================================== */

int _prolog(void)
{
    const VoidFunc *ctor;

    for (ctor = _ctors; *ctor != 0; ctor++) {
        (*ctor)();
    }

    fn_1_A0();
    return 0;
}

void _epilog(void)
{
    const VoidFunc *dtor;

    for (dtor = _dtors; *dtor != 0; dtor++) {
        (*dtor)();
    }
}

/* ======================================================================== */
/* functions in strict ascending target-address order */
/* ======================================================================== */

/* 0x00A0 */
void fn_1_A0(void)
{
    HU3D_LIGHTID lightId;
    s16 lang;

    OSReport(lbl_1_data_B8);
    lbl_1_bss_340 = omInitObjMan(0x32, 0x200);
    omGameSysInit(lbl_1_bss_340);
    lang = GwLanguage;
    GwCommon.languageNo = lang;
    lightId = (int)Hu3DGLightCreate(lbl_1_rodata_10, lbl_1_rodata_14, lbl_1_rodata_18,
                                    lbl_1_rodata_1C, lbl_1_rodata_20, lbl_1_rodata_24,
                                    0xff, 0xff, 0xff);
    Hu3DGLightInfinitytSet(lightId);
    lbl_1_bss_344 = omAddObjEx(lbl_1_bss_340, 0x7fda, 0, 0, -1, omOutViewMulti);
    lbl_1_bss_344->work[0] = 2;
    Hu3DCameraCreate(3);
    Hu3DCameraPerspectiveSet(1, lbl_1_rodata_28, lbl_1_rodata_2C, lbl_1_rodata_30, lbl_1_rodata_34);
    Hu3DCameraViewportSet(1, lbl_1_rodata_38, lbl_1_rodata_38, lbl_1_rodata_3C,
                          lbl_1_rodata_40, lbl_1_rodata_38, lbl_1_rodata_1C);
    Hu3DCameraPerspectiveSet(2, lbl_1_rodata_44, lbl_1_rodata_2C, lbl_1_rodata_30, lbl_1_rodata_34);
    Hu3DCameraViewportSet(2, lbl_1_rodata_38, lbl_1_rodata_38, lbl_1_rodata_3C,
                          lbl_1_rodata_40, lbl_1_rodata_38, lbl_1_rodata_1C);
    omCameraViewSetMulti(1, &lbl_1_data_0);
    omCameraViewSetMulti(2, &lbl_1_data_1C);
    HuPrcChildCreate(fn_1_374, 0x100, 0x3000, 0, HuPrcCurrentGet());
    Hu3DBGColorSet(0, 0, 0);
    HuWinInit(1);
    HuWinMesLanguageSet(GwLanguage);
    HuWinMesRead();
    HuAudSStreamPlay(2);
    OSReport(lbl_1_data_E2, 0xcb2);
}

/* 0x0374 */
void fn_1_374(void)
{
    s32 flag;
    s16 i;
    s32 status;

    flag = 1;
    fn_1_6E8();
    fn_1_2C6C();
    SLSaveFlagSet(1);
    WipeCreate(1, 0, 0x3c);
    while (WipeCheck()) {
        HuPrcVSleep();
    }
    status = fn_1_B63C(-1);
    if (status == -2) {
        goto L_56C;
    }
    if (status == -1) {
        HuAudFXPlay(0x48f);
        fn_1_CB4(0);
        GwCommon = GwCommonOrig;
        fn_1_6E54(0, 0);
        SLSaveFlagSet(0);
        flag = 0;
        goto L_56C;
    }
    HuAudFXPlay(0x48f);
    fn_1_CB4(1);
L_454:
    status = fn_1_17CC();
    if (status == -0x4d2) {
        goto L_56C;
    }
    if (status == -3) {
        status = fn_1_B63C(-1);
        if (status == -2) {
            goto L_56C;
        }
        if (status == -1) {
            GwCommon = GwCommonOrig;
            fn_1_6E54(0, 0);
            SLSaveFlagSet(0);
            goto L_568;
        }
        for (i = 0; i < 3; i++) {
            fn_1_36C4(i, i);
        }
        goto L_454;
    }
    if (status == -1) {
        GwCommon = GwCommonOrig;
        fn_1_6E54(0, 0);
        SLSaveFlagSet(0);
        flag = 0;
        goto L_56C;
    }
    SLSaveFlagSet(1);
    fn_1_6E54(status, 1);
L_568:
    flag = 0;
L_56C:
    HuDataDirClose(0xF20000);
    HuDataDirClose(0x210000);
    HuDataDirClose(0x230000);
    WipeCreate(2, 0, 0x3c);
    HuAudSStreamAllFadeOut(0x3e8);
    while (WipeCheck()) {
        HuPrcVSleep();
    }
    if (SLSaveFlagGet() == 0) {
        fn_1_680();
    }
    GXSetPixelFmt(0, GX_ZC_LINEAR);
    for (i = 0; i < 4; i++) {
        HuMemDirectFree(lbl_1_bss_D8[i].unk_58);
    }
    if (flag != 0) {
        omOvlReturnEx(1, 1);
    } else {
        if (OSGetSoundMode() == 0) {
            msmSysSetOutputMode(0);
        } else if (GwCommon.outputMode == 2) {
            msmSysSetOutputMode(2);
        } else {
            msmSysSetOutputMode(1);
        }
        omOvlCallEx(0x5d, 1, 0, 0);
    }
    for (;;) {
        HuPrcVSleep();
    }
}

/* 0x0680 */
void fn_1_680(void)
{
    if (HuMCProbe(1) != 0) {
        HuMCMicSet(0);
    } else {
        HuMCMicSet(1);
        HuMCInit(0);
        if (HuMCMount(1) != 0) {
            HuMCMicSet(0);
        }
        HuMCClose();
    }
    HuMCMicGet();
}

/* 0x06E8 */
void fn_1_6E8(void)
{
    Vec camPos;
    Vec camTarget;
    Vec camUp;
    s16 i;
    ANIMDATA *parAnim;

    Hu3DShadowCreate(lbl_1_rodata_48, lbl_1_rodata_2C, lbl_1_rodata_4C);
    camPos.x = lbl_1_rodata_38;
    camPos.y = lbl_1_rodata_18;
    camPos.z = lbl_1_rodata_50;
    camUp.y = lbl_1_rodata_1C;
    camUp.x = camUp.z = lbl_1_rodata_38;
    camTarget.x = lbl_1_rodata_38;
    camTarget.y = lbl_1_rodata_54;
    camTarget.z = lbl_1_rodata_38;
    Hu3DShadowPosSet(&camPos, &camUp, &camTarget);
    Hu3DShadowTPLvlSet(lbl_1_rodata_38);

    lbl_1_bss_2D8[1] = Hu3DModelCreate(HuDataSelHeapReadNum(0x230000, 0x10000000, 2));
    Hu3DModelCameraSet(lbl_1_bss_2D8[1], 2);
    Hu3DModelLayerSet(lbl_1_bss_2D8[1], 1);
    Hu3DMotionSpeedSet(lbl_1_bss_2D8[1], lbl_1_rodata_58);
    Hu3DModelPosSet(lbl_1_bss_2D8[1], lbl_1_rodata_38, lbl_1_rodata_38, lbl_1_rodata_5C);
    Hu3DModelAttrSet(lbl_1_bss_2D8[1], 0x40000001);

    lbl_1_bss_2D8[0] = Hu3DModelCreate(HuDataSelHeapReadNum(0x230001, 0x10000000, 2));
    Hu3DModelCameraSet(lbl_1_bss_2D8[0], 2);
    Hu3DModelLayerSet(lbl_1_bss_2D8[0], 1);
    Hu3DModelAttrSet(lbl_1_bss_2D8[0], 0x40000002);
    Hu3DModelClusterAttrSet(lbl_1_bss_2D8[0], 0, 0xC0000002);
    Hu3DModelPosSet(lbl_1_bss_2D8[0], lbl_1_rodata_38, lbl_1_rodata_38, lbl_1_rodata_60);
    Hu3DModelRotSet(lbl_1_bss_2D8[0], lbl_1_rodata_64, lbl_1_rodata_68, lbl_1_rodata_6C);
    Hu3DModelShadowMapSet(lbl_1_bss_2D8[0]);

    lbl_1_bss_2D8[2] = Hu3DModelCreate(HuDataSelHeapReadNum(0x230004, 0x10000000, 2));
    Hu3DModelCameraSet(lbl_1_bss_2D8[2], 2);
    Hu3DModelLayerSet(lbl_1_bss_2D8[2], 1);
    Hu3DModelAttrSet(lbl_1_bss_2D8[2], 1);
    Hu3DModelShadowSet(lbl_1_bss_2D8[2]);

    lbl_1_bss_C8 = HuSprAnimRead(HuDataSelHeapReadNum(0x23001D, 0x10000000, 2));
    lbl_1_bss_A2[0] = Hu3DAnimCreate(lbl_1_bss_C8, lbl_1_bss_2D8[2], lbl_1_data_138);
    Hu3DAnimBankSet(lbl_1_bss_A2[0], 1);
    lbl_1_bss_A2[1] = Hu3DAnimCreate(lbl_1_bss_C8, lbl_1_bss_2D8[2], lbl_1_data_144);
    Hu3DAnimBankSet(lbl_1_bss_A2[1], 0);

    HuPrcChildCreate(fn_1_15AC, 0x100, 0x1000, 0, HuPrcCurrentGet());
    fn_1_8B94();
    Hu3DCameraLayerHookSet(1, 0, (HU3D_LAYER_HOOK)fn_1_C60);
    Hu3DCameraLayerHookSet(2, 0, (HU3D_LAYER_HOOK)fn_1_C8C);
    omAddObjEx(lbl_1_bss_340, 0x1000, 0x10, 0x10, -1, fn_1_8688);

    parAnim = HuSprAnimRead(HuDataSelHeapReadNum(0x210000, 0x10000000, 2));
    for (i = 0; i < 5; i++) {
        lbl_1_bss_98[i] = Hu3DParManCreate(parAnim, 0x64, &lbl_1_data_E8);
        Hu3DParManAttrSet(lbl_1_bss_98[i], 0x64);
        Hu3DParticleBlendModeSet(Hu3DParManModelIDGet(lbl_1_bss_98[i]), 1);
        Hu3DParManRotSet(lbl_1_bss_98[i], lbl_1_rodata_70, lbl_1_rodata_38, lbl_1_rodata_38);
        Hu3DModelCameraSet(Hu3DParManModelIDGet(lbl_1_bss_98[i]), 2);
        Hu3DModelLayerSet(Hu3DParManModelIDGet(lbl_1_bss_98[i]), 4);
        Hu3DParManAttrSet(lbl_1_bss_98[i], 1);
    }
}

/* 0x0C60 */
void fn_1_C60(void)
{
    GXSetPixelFmt(1, GX_ZC_LINEAR);
    fn_1_6A8C();
}

/* 0x0C8C */
void fn_1_C8C(void)
{
    GXSetPixelFmt(0, GX_ZC_LINEAR);
}

/* 0x0CB4 (MUST precede fn_1_179C/3C80/3CF4/50DC so -inline auto keeps real bl calls) */
void fn_1_CB4(s32 mode)
{
    Mtx objMtx;
    Vec rot;
    Vec pos;
    Vec trans;
    Vec rotV;
    f32 t;
    f32 total;
    f32 rate;
    f32 rate2;
    s16 i;
    FILESEL_WORK *w;

    lbl_1_bss_2E0 = HuWinExCreate(16.0f, 360.0f, 0x220, 0x50, -1);
    HuWinAttrSet(lbl_1_bss_2E0, 0x800);
    HuWinPadMaskSet(lbl_1_bss_2E0, 1);
    for (i = 0; i < 3; i++) {
        fn_1_36C4(i, i);
        fn_1_3CF4(i, 2);
    }
    total = 10.0f + Hu3DMotionMaxTimeGet(lbl_1_bss_D8[i].unk_8);
    rot.x = -20.0f;
    rot.y = -30.0f;
    rot.z = 30.0f;
    pos.x = 0.0f;
    pos.y = 0.0f;
    pos.z = -2000.0f;
    HuPrcSleep(10);
    for (t = 0.0f; t < total; t += 1.0f) {
        HuPrcVSleep();
        if (10.0f == t) {
            Hu3DModelAttrReset(lbl_1_bss_2D8[0], 0x40000002);
            Hu3DModelClusterAttrReset(lbl_1_bss_2D8[0], 0, 0xC0000002);
        }
        rate = sin(3.141592653589793 * (90.0 * ((1.0 + t) / total)) / 180.0);
        if (t < 0.7 * total) {
            rate2 = sin(3.141592653589793 * (90.0 * ((1.0 + t) / (0.7 * total))) / 180.0);
        } else {
            rate2 = 1.0f;
        }
        Hu3DModelPosSet(lbl_1_bss_2D8[0],
            pos.x + -pos.x * rate,
            pos.y + (50.0f - pos.y) * rate,
            pos.z + -pos.z * rate);
        Hu3DModelRotSet(lbl_1_bss_2D8[0],
            rot.x + (340.0f - rot.x) * rate,
            rot.y + (360.0f - rot.y) * rate2,
            rot.z + -rot.z * rate2);
        if (mode != 0) {
            if (t >= 70.0f) {
                if (70.0f == t) {
                    Hu3DModelAttrReset(lbl_1_bss_2D8[2], 1);
                    for (i = 0; i < 3; i++) {
                        Hu3DModelAttrReset(lbl_1_bss_D8[i].unk_8, 1);
                    }
                }
                rate = sin(3.141592653589793 * (90.0 * ((t - 70.0f) / (total - 70.0f))) / 180.0);
                Hu3DModelObjMtxGet(lbl_1_bss_2D8[0], lbl_1_data_88[0], objMtx);
                Hu3DMtxTransGet(objMtx, &trans);
                Hu3DMtxRotGet(objMtx, &rotV);
                trans.x = trans.x + -260.0f * rate;
                trans.y = trans.y + -50.0f * rate;
                trans.z = trans.z + 500.0 * sin(3.141592653589793 * (180.0 * ((t - 70.0f) / (total - 70.0f))) / 180.0);
                Hu3DModelPosSetV(lbl_1_bss_2D8[2], &trans);
                rotV.x = rotV.x * (1.0 - rate);
                rotV.y = rotV.y * (1.0 - rate);
                rotV.z = rotV.z * (1.0 - rate);
                Hu3DModelRotSetV(lbl_1_bss_2D8[2], &rotV);
                Hu3DModelScaleSet(lbl_1_bss_2D8[2], 1.2f, 1.2f, 1.2f);
                for (i = 0; i < 3; i++) {
                    w = &lbl_1_bss_D8[i];
                    Hu3DModelObjMtxGet(lbl_1_bss_2D8[0], lbl_1_data_88[i + 1], objMtx);
                    Hu3DMtxTransGet(objMtx, &trans);
                    Hu3DMtxRotGet(objMtx, &rotV);
                    trans.x = trans.x + rate * (lbl_1_data_154[i].x - trans.x);
                    trans.y = trans.y + rate * (lbl_1_data_154[i].y - trans.y);
                    trans.z = trans.z + rate * (lbl_1_data_154[i].z - trans.z);
                    lbl_1_bss_2E4[i] = trans;
                    fn_1_3C80(i, &trans);
                    rotV.x = rotV.x * (1.0 - rate);
                    rotV.y = rotV.y * (1.0 - rate);
                    rotV.z = rotV.z * (1.0 - rate);
                    Hu3DModelRotSetV(w->unk_8, &rotV);
                    Hu3DModelScaleSet(w->unk_8, 1.5f, 1.5f, 1.5f);
                }
            }
            if (t > total - 30.0f) {
                rate = (t - (total - 30.0f)) / 30.0f;
                Hu3DShadowTPLvlSet(0.5f * rate);
            }
            if (t == total - 10.0f) {
                fn_1_50DC(fn_1_179C, 0);
            }
        }
    }
    if (mode != 0) {
        lbl_1_bss_2DE = HuWinCreate(16.0f, 326.0f, 0x220, 0x2a, 0);
        HuWinAttrSet(lbl_1_bss_2DE, 0x800);
        HuWinPriSet(lbl_1_bss_2DE, 0x64);
        HuWinBGTPLvlSet(lbl_1_bss_2DE, 0.0f);
        HuWinMesSpeedSet(lbl_1_bss_2DE, 0);
    }
}

/* 0x15AC */
void fn_1_15AC(void)
{
    Mtx mtx;
    f32 a;
    f32 b;

    a = 0.0f;
    b = 45.0f;
    while (1) {
        a += 2.0f;
        if (a > 360.0f) {
            a -= 360.0f;
        }
        b += 1.1;
        if (b > 360.0f) {
            b -= 360.0f;
        }
        PSMTXTrans(mtx, 0.0f, (f32)(3.0 * sin(3.141592653589793 * a / 180.0)), 0.0f);
        mtxRotCat(mtx, 0.0f, 0.0f, (f32)sin(3.141592653589793 * b / 180.0));
        Hu3DModelMtxSet(lbl_1_bss_2D8[0], &mtx);
        PSMTXTrans(mtx, (f32)sin(3.141592653589793 * a / 180.0), (f32)(3.0 * sin(3.141592653589793 * b / 180.0)), 0.0f);
        Hu3DModelMtxSet(lbl_1_bss_2D8[2], &mtx);
        HuPrcVSleep();
    }
}

/* 0x179C */
void fn_1_179C(void)
{
    HuWinExOpen(lbl_1_bss_2E0);
    HuPrcEnd();
}

/* 0x17CC  (file-slot select main loop).
 * THE match hinges on the extern prototype `s32 SLCurBoxNoSet(s32);` (non-void): the discarded
 * result vreg advances the volatile-scratch allocation from the call site onward, matching the
 * target's r0/r4/r5 numbering. Body is otherwise giant3's reconstruction, unchanged.
 * Placed before all in-TU callees so -inline auto keeps real bl calls. */
s32 fn_1_17CC(void)
{
    s16 sel;
    s16 i;
    s16 j;
    s16 count;
    s16 dir;
    s32 ret;
    s32 fxId;
    FILESEL_WORK *w;
    Vec scale;
    Vec sv[3];
    OSTime time;
    f32 t;

    sel = 0;
    for (i = 0; i < 3; i++) {
        sv[i] = lbl_1_bss_2E4[i];
    }
    Hu3DModelScaleGet(lbl_1_bss_D8[0].unk_8, &scale);
    if (scale.x < lbl_1_rodata_E8) {
        for (i = 1; i <= 5; i++) {
            t = (f32)i / lbl_1_rodata_28;
            for (j = 0; j < 3; j++) {
                w = &lbl_1_bss_D8[j];
                Hu3DModelScaleSet(w->unk_8, lbl_1_rodata_C0 * t, lbl_1_rodata_C0 * t, lbl_1_rodata_C0 * t);
            }
            HuPrcVSleep();
        }
    }
L_192C:
    for (i = count = 0; i < 3; i++) {
        if (lbl_1_bss_D8[i].unk_0 != 0) {
            count++;
        }
        if (sel == i) {
            fn_1_3CF4(i, 0);
        } else {
            fn_1_3CF4(i, 1);
        }
    }
    Hu3DModelScaleGet(lbl_1_bss_D8[sel].unk_8, &scale);
    if (scale.x < lbl_1_rodata_D0) {
        for (i = 1; i <= 0xa; i++) {
            w = &lbl_1_bss_D8[sel];
            t = (f32)i / lbl_1_rodata_44;
            fn_1_3CC0(sel, sv[sel].x, sv[sel].y, lbl_1_rodata_2C * t + sv[sel].z);
            t = (f32)(sin(lbl_1_rodata_80 * (lbl_1_rodata_EC * t) / lbl_1_rodata_98) * (lbl_1_rodata_90 / sin(lbl_1_rodata_F0)));
            Hu3DModelScaleSet(w->unk_8,
                (f32)(lbl_1_rodata_F8 + lbl_1_rodata_100 * t),
                (f32)(lbl_1_rodata_F8 + lbl_1_rodata_100 * t),
                (f32)(lbl_1_rodata_F8 + lbl_1_rodata_100 * t));
            if (i == 5) {
                w->unk_12 = 1;
            }
            HuPrcVSleep();
        }
    }
    HuWinMesSet(lbl_1_bss_2E0, 0x90008);
    HuWinMesWait(lbl_1_bss_2E0);
    HuWinMesSet(lbl_1_bss_2DE, 0x10004);
    HuWinDispOn(lbl_1_bss_2DE);
    for (;;) {
        HuPrcVSleep();
        if (UnMountCnt != 0) {
            HuWinMesSet(lbl_1_bss_2E0, 0x90026);
            HuWinMesWait(lbl_1_bss_2E0);
            UnMountCnt = 0;
            goto L_241C;
        }
        dir = 0;
        if (HuPadBtnDown[0] & 0x1100) {
            HuAudFXPlay(1);
            HuWinDispOff(lbl_1_bss_2DE);
            SLCurBoxNoSet(sel);
            if (lbl_1_bss_D8[sel].unk_0 != 0) {
                if (count >= 3) {
                    HuWinMesSet(lbl_1_bss_2E0, 0x9003c);
                } else {
                    HuWinMesSet(lbl_1_bss_2E0, 0x9003d);
                }
                HuWinMesWait(lbl_1_bss_2E0);
                ret = HuWinChoiceGet(lbl_1_bss_2E0, -1);
                if (UnMountCnt != 0) {
                    HuWinMesSet(lbl_1_bss_2E0, 0x90026);
                    HuWinMesWait(lbl_1_bss_2E0);
                    UnMountCnt = 0;
                    goto L_241C;
                }
                if (ret == -1) {
                    goto L_192C;
                }
                if (ret == 0) {
                    SLCommonLoad();
                    goto L_2414;
                }
                if ((count >= 3 && ret == 1) || (count < 3 && ret == 2)) {
                    lbl_1_bss_D8[sel].unk_12 = 2;
                    ret = fn_1_642C(sel);
                    lbl_1_bss_D8[sel].unk_12 = 1;
                    if (ret == -0x4d2) {
                        goto L_241C;
                    }
                    if (ret == -4) {
                        goto L_241C;
                    }
                    if (ret == -5) {
                        goto L_241C;
                    }
                    if (ret == -1) {
                        goto L_254C;
                    }
                    goto L_192C;
                } else {
                    lbl_1_bss_D8[sel].unk_12 = 3;
                    fn_1_63A0(sel, 1);
                    ret = fn_1_5244(sel);
                    fn_1_63A0(sel, 0);
                    lbl_1_bss_D8[sel].unk_12 = 1;
                    if (ret == -4) {
                        goto L_241C;
                    }
                    if (ret == -5) {
                        goto L_241C;
                    }
                    if (ret == -1) {
                        goto L_254C;
                    }
                    goto L_192C;
                }
            } else {
                UnMountCnt = 0;
                fn_1_69B0();
                fn_1_2654(-1);
                ret = fn_1_916C(sel);
                fn_1_2A40();
                if (ret == -0x4d2) {
                    goto L_192C;
                }
                if (UnMountCnt != 0) {
                    HuWinMesSet(lbl_1_bss_2E0, 0x90026);
                    HuWinMesWait(lbl_1_bss_2E0);
                    UnMountCnt = 0;
                    goto L_241C;
                }
                if (fn_1_C278() == 0) {
                    time = OSGetTime();
                    SLSaveDataMake(1, &time);
                    SLCommonSet();
                    for (i = 0; i < 3; i++) {
                        if (i != sel) {
                            SLSaveEmptySet(curSlotNo, i);
                        }
                    }
                    fxId = HuAudFXPlay(0x48d);
                    ret = fn_1_D348(-1);
                    HuAudFXStop(fxId);
                    if (ret == -0x4d2) {
                        goto L_241C;
                    }
                    if (ret == -4) {
                        goto L_241C;
                    }
                    if (ret == -5) {
                        goto L_241C;
                    }
                    if (ret == -1) {
                        goto L_254C;
                    }
                } else {
                    time = OSGetTime();
                    SLSaveDataMake(0, &time);
                    SLCommonSet();
                    fxId = HuAudFXPlay(0x48d);
                    ret = fn_1_D90C(-1, 0x9002a);
                    HuAudFXStop(fxId);
                    if (ret == -0x4d2) {
                        goto L_241C;
                    }
                    if (ret == -4) {
                        goto L_241C;
                    }
                    if (ret == -5) {
                        goto L_241C;
                    }
                    if (ret == -1) {
                        goto L_254C;
                    }
                }
                lbl_1_bss_D8[sel].unk_0 = 1;
                lbl_1_bss_D8[sel].unk_14 = -1;
                lbl_1_bss_D8[sel].unk_16 = 0;
                OSTicksToCalendarTime(GwCommon.time, &lbl_1_bss_D8[sel].unk_1C);
                fn_1_36C4(sel, sel);
                goto L_2414;
            }
        } else {
            if (HuPadBtnDown[0] & 0x200) {
                HuAudFXPlay(3);
                sel = -0x4d2;
                goto L_2414;
            }
            if (HuPadDStkRep[0] & 1) {
                dir = -1;
            }
            if (HuPadDStkRep[0] & 2) {
                dir = 1;
            }
            if (dir == 0) {
                continue;
            }
            if (sel + dir < 0) {
                continue;
            }
            if (sel + dir >= 3) {
                continue;
            }
            HuAudFXPlay(0x483);
            fn_1_3CF4(sel + dir, 0);
            fn_1_3CF4(sel, 1);
            for (i = 1; i <= 0xa; i++) {
                t = (f32)i / lbl_1_rodata_44;
                t = (f32)(sin(lbl_1_rodata_80 * (lbl_1_rodata_EC * t) / lbl_1_rodata_98) * (lbl_1_rodata_90 / sin(lbl_1_rodata_F0)));
                w = &lbl_1_bss_D8[sel + dir];
                Hu3DModelScaleSet(w->unk_8,
                    (f32)(lbl_1_rodata_F8 + lbl_1_rodata_100 * t),
                    (f32)(lbl_1_rodata_F8 + lbl_1_rodata_100 * t),
                    (f32)(lbl_1_rodata_F8 + lbl_1_rodata_100 * t));
                fn_1_3CC0(sel + dir, sv[sel + dir].x, sv[sel + dir].y, lbl_1_rodata_2C * t + sv[sel + dir].z);
                t = (f32)(lbl_1_rodata_90 - t);
                w = &lbl_1_bss_D8[sel];
                Hu3DModelScaleSet(w->unk_8,
                    (f32)(lbl_1_rodata_F8 + lbl_1_rodata_100 * t),
                    (f32)(lbl_1_rodata_F8 + lbl_1_rodata_100 * t),
                    (f32)(lbl_1_rodata_F8 + lbl_1_rodata_100 * t));
                fn_1_3CC0(sel, sv[sel].x, sv[sel].y, lbl_1_rodata_2C * t + sv[sel].z);
                if (i == 5) {
                    lbl_1_bss_D8[sel + dir].unk_12 = 1;
                    lbl_1_bss_D8[sel].unk_12 = 0;
                }
                HuPrcVSleep();
            }
            sel += dir;
        }
    }
L_2414:
    return sel;
L_241C:
    HuWinHomeClear(lbl_1_bss_2E0);
    HuWinDispOff(lbl_1_bss_2DE);
    for (i = 1; i <= 0xa; i++) {
        t = (f32)cos(lbl_1_rodata_80 * (lbl_1_rodata_88 * ((double)i / lbl_1_rodata_108)) / lbl_1_rodata_98);
        for (j = 0; j < 3; j++) {
            w = &lbl_1_bss_D8[j];
            Hu3DModelScaleSet(w->unk_8, t, t, t);
        }
        HuPrcVSleep();
    }
    for (i = 0; i < 3; i++) {
        lbl_1_bss_D8[i].unk_12 = 0;
    }
    HuPrcSleep(0x1e);
    return -3;
L_254C:
    HuWinHomeClear(lbl_1_bss_2E0);
    for (i = 1; i <= 0xa; i++) {
        t = (f32)cos(lbl_1_rodata_80 * (lbl_1_rodata_88 * ((double)i / lbl_1_rodata_108)) / lbl_1_rodata_98);
        for (j = 0; j < 3; j++) {
            w = &lbl_1_bss_D8[j];
            Hu3DModelScaleSet(w->unk_8, t, t, t);
        }
        HuPrcVSleep();
    }
    return -1;
}

/* 0x2654 */
void fn_1_2654(s16 arg)
{
    f32 scale;
    s16 i;

    for (i = 0; i < 3; i++) {
        Hu3DModelPosGet(lbl_1_bss_D8[i].unk_8, &lbl_1_bss_68[i]);
    }
    Hu3DModelPosGet(*(s16 *)((u8 *)lbl_1_bss_2D8 + 4), &lbl_1_bss_68[i]);
    if (arg == -1) {
        for (i = 1; i <= 10; i++) {
            scale = lbl_1_rodata_90 - cos(lbl_1_rodata_80 * (lbl_1_rodata_88 * ((double)i / lbl_1_rodata_108)) / lbl_1_rodata_98);
            fn_1_3CC0(0, lbl_1_bss_68[0].x, lbl_1_bss_68[0].y + lbl_1_rodata_118 * scale, lbl_1_bss_68[0].z);
            fn_1_3CC0(1, lbl_1_bss_68[1].x, lbl_1_bss_68[1].y + lbl_1_rodata_118 * scale, lbl_1_bss_68[1].z);
            fn_1_3CC0(2, lbl_1_bss_68[2].x, lbl_1_bss_68[2].y + lbl_1_rodata_118 * scale, lbl_1_bss_68[2].z);
            Hu3DModelPosSet(*(s16 *)((u8 *)lbl_1_bss_2D8 + 4), lbl_1_bss_68[3].x - lbl_1_rodata_11C * scale, lbl_1_bss_68[3].y, lbl_1_bss_68[3].z);
            HuPrcVSleep();
        }
    } else {
        for (i = 1; i <= 10; i++) {
            scale = lbl_1_rodata_90 - cos(lbl_1_rodata_80 * (lbl_1_rodata_88 * ((double)i / lbl_1_rodata_108)) / lbl_1_rodata_98);
            switch (arg) {
            case 0:
                fn_1_3CC0(0, lbl_1_bss_68[0].x, lbl_1_bss_68[0].y + lbl_1_rodata_120 * scale, lbl_1_bss_68[0].z);
                break;
            case 1:
                fn_1_3CC0(1, lbl_1_bss_68[1].x, lbl_1_bss_68[1].y - lbl_1_rodata_124 * scale, lbl_1_bss_68[1].z);
                break;
            case 2:
                fn_1_3CC0(2, lbl_1_bss_68[2].x, lbl_1_bss_68[2].y + lbl_1_rodata_120 * scale, lbl_1_bss_68[2].z);
                break;
            }
            HuPrcVSleep();
        }
    }
}

/* 0x2A40 */
void fn_1_2A40(void)
{
    Vec v14;
    Vec v8;
    Vec v20[4];
    f32 scale;
    s16 i;
    s16 j;

    for (i = 0; i < 3; i++) {
        Hu3DModelPosGet(lbl_1_bss_D8[i].unk_8, &v14);
        PSVECSubtract(&v14, &lbl_1_bss_68[i], &v20[i]);
    }
    Hu3DModelPosGet(*(s16 *)((u8 *)lbl_1_bss_2D8 + 4), &v14);
    PSVECSubtract(&v14, &lbl_1_bss_68[i], &v20[i]);
    for (i = 1; i <= 10; i++) {
        scale = lbl_1_rodata_90 - sin(lbl_1_rodata_80 * (lbl_1_rodata_88 * ((double)i / lbl_1_rodata_108)) / lbl_1_rodata_98);
        for (j = 0; j < 3; j++) {
            PSVECScale(&v20[j], &v14, scale);
            PSVECAdd(&lbl_1_bss_68[j], &v14, &v8);
            fn_1_3C80(j, &v8);
        }
        PSVECScale(&v20[j], &v14, scale);
        PSVECAdd(&lbl_1_bss_68[j], &v14, &v8);
        Hu3DModelPosSetV(*(s16 *)((u8 *)lbl_1_bss_2D8 + 4), &v8);
        HuPrcVSleep();
    }
}

/* 0x2C6C  (build the 4 file-slot windows/sprites/models).
 * unk_58 is ANIMDATA* (p->unk_58->bmp->data). The `i = i;` no-op emits nothing but gives
 * `i` +2 allocator refs so it outranks `grp` for callee-saved r29 (i=r29/grp=r28 as target);
 * without it MWCC 2.6 -O0,p swaps the pair (99.69). Do not delete it. */
void fn_1_2C6C(void)
{
    FILESEL_WORK *p;
    s16 j;
    HUSPR_GROUPID grp;
    s16 i;
    HSF_DATA *hsf;
    HUSPRID spr;
    HUWINID win;
    ANIMDATA *animB;
    HSF_OBJECT *obj;
    ANIMDATA *animC;
    ANIMDATA *animA;

    HuSprExecLayerCameraSet(0x40, 2, 1);
    HuSprExecLayerCameraSet(0x78, 1, 8);
    animA = HuSprAnimRead(HuDataSelHeapReadNum(0x230008, 0x10000000, 2));
    grp = HuSprGrpCreate(1);
    spr = HuSprCreate(animA, 0, 0);
    HuSprGrpMemberSet(grp, 0, spr);
    HuSprGrpDrawNoSet(grp, 0x40);
    HuSprGrpPosSet(grp, lbl_1_rodata_128, lbl_1_rodata_12C);
    HuSprGrpScaleSet(grp, lbl_1_rodata_2C, lbl_1_rodata_2C);
    animB = HuSprAnimRead(HuDataSelHeapReadNum(0x230007, 0x10000000, 2));
    animC = HuSprAnimRead(HuDataSelHeapReadNum(0x230006, 0x10000000, 2));
    lbl_1_bss_CC[0] = HuSprAnimRead(HuDataSelHeapReadNum(0x230009, 0x10000000, 2));
    lbl_1_bss_CC[1] = HuSprAnimRead(HuDataSelHeapReadNum(0x23000B, 0x10000000, 2));
    lbl_1_bss_CC[2] = HuSprAnimRead(HuDataSelHeapReadNum(0x23000A, 0x10000000, 2));
    for (i = 0; i < 8; i++) {
        lbl_1_bss_A8[i] = HuSprAnimRead(HuDataSelHeapReadNum(lbl_1_data_98[i], 0x10000000, 2));
    }
    i = i;
    for (i = 0; i < 4; i++) {
        p = &lbl_1_bss_D8[i];
        win = HuWinCreate((f32)(lbl_1_rodata_130 * (i & 1)), (f32)(lbl_1_rodata_138 * (i / 2)), 0xc0, 0x70, 0);
        p->unk_6 = win;
        p->unk_70.x = p->unk_70.y = p->unk_70.z = lbl_1_rodata_38;
        HuWinScaleSet(win, lbl_1_rodata_C0, lbl_1_rodata_C0);
        HuWinBGTPLvlSet(win, lbl_1_rodata_38);
        HuWinAttrSet(win, 0x801);
        p->unk_58 = HuSprAnimMake(0x140, 0xa8, 2);
        p->unk_5C = p->unk_58->bmp->data = HuMemDirectMallocNum(0, GXGetTexBufferSize(0x140, 0xa8, 5, 0, 0), 0x10000000);
        HuWinDrawNoSet(win, 0x78);
        HuWinMesSpeedSet(win, 0);
        HuWinMesSet(win, 0x4a0009);
        grp = HuSprGrpCreate(0x11);
        p->unk_4 = grp;
        HuSprGrpPosSet(grp, (f32)(lbl_1_rodata_130 * (i & 1)), (f32)(lbl_1_rodata_138 * (i / 2)));
        HuSprGrpScaleSet(grp, lbl_1_rodata_C0, lbl_1_rodata_C0);
        for (j = 0; j < 10; j++) {
            spr = HuSprCreate(animB, 0x5f, 0);
            HuSprGrpMemberSet(grp, j, spr);
            HuSprPosSet(grp, j, (f32)(j * 12 + 42), lbl_1_rodata_AC);
            HuSprAttrReset(grp, j, 8);
        }
        spr = HuSprCreate(animC, 0x5f, 0);
        HuSprGrpMemberSet(grp, 0xa, spr);
        HuSprPosSet(grp, 0xa, lbl_1_rodata_AC, lbl_1_rodata_140);
        HuSprAttrReset(grp, 0xa, 8);
        for (j = 0; j < 5; j++) {
            spr = HuSprCreate(animB, 0x5f, 0);
            HuSprGrpMemberSet(grp, j + 0xc, spr);
            HuSprPosSet(grp, j + 0xc, (f32)(j * 12 + 0x58), lbl_1_rodata_140);
            HuSprAttrReset(grp, j + 0xc, 8);
        }
        HuSprGrpDrawNoSet(grp, 0x78);
        p->unk_8 = Hu3DModelCreate(HuDataSelHeapReadNum(0x230002, 0x10000000, 2));
        Hu3DModelCameraSet(p->unk_8, 2);
        Hu3DModelLayerSet(p->unk_8, 1);
        Hu3DModelAttrSet(p->unk_8, 0x40000001);
        j = Hu3DMotionMaxTimeGet(p->unk_8);
        Hu3DMotionTimeSet(p->unk_8, (f32)(s32)frandmod((s16)j));
        p->unk_18 = Hu3DAnimCreate(lbl_1_bss_CC[0], p->unk_8, lbl_1_data_1A0);
        p->unk_1A = Hu3DAnimCreate(lbl_1_bss_A8[0], p->unk_8, lbl_1_data_1AD);
        Hu3DModelShadowSet(p->unk_8);
        p->unk_A = Hu3DModelCreate(HuDataSelHeapReadNum(0x230003, 0x10000000, 2));
        Hu3DModelCameraSet(p->unk_A, 2);
        Hu3DModelLayerSet(p->unk_A, 2);
        Hu3DAnimCreate(p->unk_58, p->unk_A, lbl_1_data_1BB);
        p->unk_7C = HuPrcChildCreate(fn_1_3D14, 0x10, 0x3000, 0, HuPrcCurrentGet());
        obj = Hu3DModelObjPtrGet(p->unk_8, lbl_1_data_1C8);
        strcpy(obj->name, lbl_1_data_190[i]);
        p->unk_C = Hu3DModelCreate(HuDataSelHeapReadNum(0x23001A, 0x10000000, 2));
        Hu3DModelCameraSet(p->unk_C, 2);
        Hu3DModelLayerSet(p->unk_C, 2);
        Hu3DModelAttrSet(p->unk_C, 1);
        p->unk_E = Hu3DModelCreate(HuDataSelHeapReadNum(0x23001B, 0x10000000, 2));
        Hu3DModelCameraSet(p->unk_E, 2);
        Hu3DModelLayerSet(p->unk_E, 2);
        Hu3DModelAttrSet(p->unk_E, 1);
        p->unk_10 = Hu3DModelCreate(HuDataSelHeapReadNum(0x23001B, 0x10000000, 2));
        Hu3DModelCameraSet(p->unk_10, 2);
        Hu3DModelLayerSet(p->unk_10, 2);
        Hu3DModelAttrSet(p->unk_10, 1);
        hsf = Hu3DData[p->unk_10].hsf;
        for (j = 0; j < hsf->materialNum; j++) {
            hsf->material[j].litColor[0] = hsf->material[j].color[0] = 0xf1;
            hsf->material[j].litColor[1] = hsf->material[j].color[1] = 0x7b;
            hsf->material[j].litColor[2] = hsf->material[j].color[2] = 0x12;
        }
        p->unk_12 = 0;
        Hu3DModelAttrSet(p->unk_8, 1);
        p->unk_0 = 1;
        strcpy(p->unk_44, lbl_1_data_1DD);
        OSTicksToCalendarTime(OSGetTime(), &p->unk_1C);
        p->unk_16 = 0xa;
        fn_1_36C4(i, i % 3);
    }
    Hu3DCameraLayerHookSet(1, 1, (HU3D_LAYER_HOOK)fn_1_35AC);
}

/* 0x35AC */
void fn_1_35AC(void)
{
    s16 i;

    for (i = 0; i < 4; i++) {
        FILESEL_WORK *w = &lbl_1_bss_D8[i];
        Hu3DFbCopyExec(lbl_1_rodata_148 * (lbl_1_rodata_130 * (i & 1)),
                       lbl_1_rodata_138 * (i / 2),
                       0x140, 0xa8, 5, 0, w->unk_5C);
    }
}

/* 0x36C4 */
void fn_1_36C4(s16 a, s16 b)
{
    FILESEL_WORK *p = &lbl_1_bss_D8[a];
    s16 sprId = p->unk_4;
    OSCalendarTime *q = &p->unk_1C;
    u8 i;
    u8 len;
    char buf[12];

    p->unk_60[0] = (u8)(b + 0x31);
    p->unk_60[1] = 0;
    HuWinInsertMesSet(p->unk_6, (u32)p->unk_60, 0);
    if (p->unk_0 == 0) {
        p->unk_14 = -1;
        p->unk_44[0] = 0;
        HuWinInsertMesSet(p->unk_6, 0x90033, 1);
        HuWinMesSet(p->unk_6, 0x90042);
        for (i = 0; i < 0xa; i++) {
            HuSprAttrReset(sprId, i, 4);
            HuSprBankSet(sprId, i, 0xc);
        }
        HuSprBankSet(sprId, 2, 0xb);
        HuSprBankSet(sprId, 5, 0xb);
        HuSprBankSet(sprId, 0xc, 0xa);
        HuSprBankSet(sprId, 0xc, 0xe);
        HuSprBankSet(sprId, 0xd, 0);
        Hu3DAnimAnimSet(p->unk_1A, lbl_1_bss_A8[0]);
        HuSprBankSet(sprId, 0xd, 0);
        for (i = 0; i < 3; i++) {
            HuSprAttrSet(sprId, i + 0xe, 4);
        }
        HuSprPosSet(sprId, 0xa, lbl_1_rodata_150, lbl_1_rodata_140);
        HuSprPosSet(sprId, 0xc, lbl_1_rodata_154, lbl_1_rodata_140);
        HuSprPosSet(sprId, 0xd, lbl_1_rodata_158, lbl_1_rodata_140);
    } else {
        for (i = 0; i < 0xf; i++) {
            HuSprAttrReset(sprId, i, 4);
        }
        HuSprBankSet(sprId, 2, 0xb);
        HuSprBankSet(sprId, 5, 0xb);
        sprintf(buf, lbl_1_data_1E6, q->mon + 1);
        for (i = 0; i < 2; i++) {
            HuSprBankSet(sprId, i, buf[i] & 0xf);
        }
        sprintf(buf, lbl_1_data_1E6, q->mday);
        for (i = 0; i < 2; i++) {
            HuSprBankSet(sprId, i + 3, buf[i] & 0xf);
        }
        sprintf(buf, lbl_1_data_1EB, q->year);
        for (i = 0; i < 4; i++) {
            HuSprBankSet(sprId, i + 6, buf[i] & 0xf);
        }
        HuWinInsertMesSet(p->unk_6, (u32)p->unk_44, 1);
        HuWinMesSet(p->unk_6, 0x90042);
        HuSprBankSet(sprId, 0xc, 0xe);
        for (i = 0; i < 5; i++) {
            HuSprAttrSet(sprId, i + 0xd, 4);
        }
        if (p->unk_16 > 0x270f) {
            p->unk_16 = 0x270f;
        }
        sprintf(buf, lbl_1_data_1F0, p->unk_16);
        len = strlen(buf);
        HuSprPosSet(sprId, 0xa, (f32)(0x6c - (len * 0xc + 0x24) / 2), lbl_1_rodata_140);
        HuSprPosSet(sprId, 0xc, (f32)(0x7e - (len * 0xc + 0x24) / 2), lbl_1_rodata_140);
        for (i = 0; i < 4; i++) {
            if (buf[i] == 0) {
                break;
            }
            HuSprBankSet(sprId, i + 0xd, buf[i] & 0xf);
            HuSprAttrReset(sprId, i + 0xd, 4);
            HuSprPosSet(sprId, i + 0xd,
                        (f32)(0x7e - (len * 0xc + 0x24) / 2 + (i + 1) * 0xc),
                        lbl_1_rodata_140);
        }
        if (p->unk_14 == -1) {
            Hu3DAnimAnimSet(p->unk_1A, lbl_1_bss_A8[1]);
        } else {
            Hu3DAnimAnimSet(p->unk_1A, lbl_1_bss_A8[p->unk_14 + 2]);
        }
    }
}

/* 0x3C80 */
void fn_1_3C80(s16 id, Vec *pos)
{
    FILESEL_WORK *p = &lbl_1_bss_D8[id];

    p->unk_64 = *pos;
}

/* 0x3CC0 */
void fn_1_3CC0(s16 id, f32 x, f32 y, f32 z)
{
    FILESEL_WORK *p = &lbl_1_bss_D8[id];

    p->unk_64.x = x;
    p->unk_64.y = y;
    p->unk_64.z = z;
}

/* 0x3CF4 */
void fn_1_3CF4(s16 id, s32 flag)
{
    lbl_1_bss_D8[id].unk_62 = (u8)flag;
}

/* 0x3D14 */
void fn_1_3D14(void)
{
    s16 prevState;
    HUPROCESS *proc;
    FILESEL_WORK *p;
    HU3D_MODEL *dst;
    HU3D_MODEL *src;
    Mtx mtx;
    s16 i;
    s16 idx;
    f32 angleX;
    f32 angleY;
    f32 wavePhase;
    f32 delta;
    f32 amp;

    proc = HuPrcCurrentGet();
    angleX = lbl_1_rodata_38;
    angleY = lbl_1_rodata_38;
    wavePhase = lbl_1_rodata_38;
    delta = lbl_1_rodata_38;
    amp = lbl_1_rodata_38;
    for (i = 0; i < 4; i++) {
        p = &lbl_1_bss_D8[i];
        if (p->unk_7C == proc) {
            break;
        }
    }
    idx = i;
    angleX = (f32)(s32)frandmod(0x5a);
    angleY = (f32)(s32)frandmod(0xb4);
    delta = lbl_1_rodata_E8 * (f32)(s32)frandmod(0x64);
    while (1) {
        switch (p->unk_62) {
        case 0:
            amp -= lbl_1_rodata_58;
            if (amp < lbl_1_rodata_38) {
                amp = lbl_1_rodata_38;
            }
            break;
        case 1:
            amp += lbl_1_rodata_E8;
            if (amp > lbl_1_rodata_1C) {
                amp = lbl_1_rodata_1C;
            }
            break;
        }
        angleX += lbl_1_rodata_1C + delta;
        if (angleX > lbl_1_rodata_78) {
            angleX -= lbl_1_rodata_78;
        }
        angleY += lbl_1_rodata_D0 + delta;
        if (angleY > lbl_1_rodata_15C) {
            angleY -= lbl_1_rodata_15C;
        }
        if (p->unk_62 != 2) {
            Hu3DModelRotSet(p->unk_8, lbl_1_rodata_38, lbl_1_rodata_38,
                amp * (lbl_1_rodata_160 * sin(lbl_1_rodata_80 * angleX / lbl_1_rodata_98)));
            p->unk_70.x = amp * (lbl_1_rodata_168 * sin(lbl_1_rodata_80 * angleY / lbl_1_rodata_98));
            p->unk_70.y = amp * (lbl_1_rodata_168 * sin(lbl_1_rodata_80 * (angleY * lbl_1_rodata_C4) / lbl_1_rodata_98));
        }
        Hu3DModelPosSet(p->unk_8, p->unk_64.x + p->unk_70.x, p->unk_64.y + p->unk_70.y,
            p->unk_64.z + p->unk_70.z);
        if ((Hu3DModelAttrGet(p->unk_8) & 1) == 0) {
            Hu3DModelAttrReset(p->unk_A, 1);
            Hu3DModelObjMtxGet(p->unk_8, lbl_1_data_190[i], mtx);
            Hu3DModelMtxSet(p->unk_A, &mtx);
            src = &Hu3DData[p->unk_8];
            dst = &Hu3DData[p->unk_C];
            PSMTXCopy(dst->mtx, src->mtx);
            dst->pos = src->pos;
            dst->scale = src->scale;
            dst->rot = src->rot;
            dst = &Hu3DData[p->unk_E];
            PSMTXCopy(dst->mtx, src->mtx);
            dst->pos = src->pos;
            dst->scale = src->scale;
            dst->rot = src->rot;
            dst = &Hu3DData[p->unk_10];
            PSMTXCopy(dst->mtx, src->mtx);
            dst->pos = src->pos;
            dst->scale = src->scale;
            dst->rot = src->rot;
        } else {
            Hu3DModelAttrSet(p->unk_A, 1);
        }
        switch (p->unk_12) {
        case 0:
            Hu3DModelAttrSet(p->unk_C, 1);
            Hu3DModelAttrSet(p->unk_E, 1);
            Hu3DModelAttrSet(p->unk_10, 1);
            break;
        case 1:
            Hu3DModelAttrReset(p->unk_C, 1);
            Hu3DModelAttrSet(p->unk_E, 1);
            Hu3DModelAttrSet(p->unk_10, 1);
            break;
        case 2:
            Hu3DModelAttrSet(p->unk_C, 1);
            Hu3DModelAttrReset(p->unk_E, 1);
            Hu3DModelAttrSet(p->unk_10, 1);
            break;
        case 3:
            Hu3DModelAttrSet(p->unk_C, 1);
            Hu3DModelAttrSet(p->unk_E, 1);
            Hu3DModelAttrReset(p->unk_10, 1);
            break;
        }
        if (prevState != p->unk_12) {
            wavePhase = lbl_1_rodata_38;
        }
        src = &Hu3DData[p->unk_C];
        PSVECScale(&src->scale, &src->scale,
            lbl_1_rodata_90 + lbl_1_rodata_170 * sin(lbl_1_rodata_80 * wavePhase / lbl_1_rodata_98));
        Hu3DModelTPLvlSet(p->unk_C,
            sin(lbl_1_rodata_80 * (lbl_1_rodata_D0 * wavePhase) / lbl_1_rodata_98));
        src = &Hu3DData[p->unk_E];
        PSVECScale(&src->scale, &src->scale,
            lbl_1_rodata_90 + lbl_1_rodata_170 * sin(lbl_1_rodata_80 * wavePhase / lbl_1_rodata_98));
        Hu3DModelTPLvlSet(p->unk_E,
            lbl_1_rodata_90 - sin(lbl_1_rodata_80 * wavePhase / lbl_1_rodata_98));
        src = &Hu3DData[p->unk_10];
        PSVECScale(&src->scale, &src->scale,
            lbl_1_rodata_90 + lbl_1_rodata_170 * sin(lbl_1_rodata_80 * wavePhase / lbl_1_rodata_98));
        Hu3DModelTPLvlSet(p->unk_10,
            lbl_1_rodata_90 - sin(lbl_1_rodata_80 * wavePhase / lbl_1_rodata_98));
        if (p->unk_12 == 2) {
            wavePhase += lbl_1_rodata_178;
        } else {
            wavePhase += lbl_1_rodata_D0;
        }
        if (wavePhase > lbl_1_rodata_70) {
            wavePhase -= lbl_1_rodata_70;
        }
        prevState = p->unk_12;
        HuPrcVSleep();
    }
}

/* 0x457C (clone of bootDll CameraMove retargeted to camera index [1]) */
void fn_1_457C(OMOBJ *object)
{
    HuVecF eye;
    HuVecF delta;
    HuVecF up;
    HuVecF roll;
    s8 subStick;
    float rotZ;

    if (!lbl_1_bss_0) {
        return;
    }

    CRotM[1].y += 0.1f * (float)HuPadStkX[0];
    CRotM[1].x += 0.1f * (float)HuPadStkY[0];
    CZoomM[1] += (float)(HuPadTrigL[0] / 2);
    CZoomM[1] -= (float)(HuPadTrigR[0] / 2);
    if (CZoomM[1] < 100.0f) {
        CZoomM[1] = 100.0f;
    }

    eye.x = CenterM[1].x + (CZoomM[1] *
        (sin(3.141592653589793 * CRotM[1].y / 180.0) *
         cos(3.141592653589793 * CRotM[1].x / 180.0)));
    eye.y = CenterM[1].y + (CZoomM[1] *
        -sin(3.141592653589793 * CRotM[1].x / 180.0));
    eye.z = CenterM[1].z + (CZoomM[1] *
        (cos(3.141592653589793 * CRotM[1].y / 180.0) *
         cos(3.141592653589793 * CRotM[1].x / 180.0)));

    delta.x = CenterM[1].x - eye.x;
    delta.y = CenterM[1].y - eye.y;
    delta.z = CenterM[1].z - eye.z;

    up.x = sin(3.141592653589793 * CRotM[1].y / 180.0) *
           sin(3.141592653589793 * CRotM[1].x / 180.0);
    up.y = cos(3.141592653589793 * CRotM[1].x / 180.0);
    up.z = cos(3.141592653589793 * CRotM[1].y / 180.0) *
           sin(3.141592653589793 * CRotM[1].x / 180.0);

    rotZ = CRotM[1].z;

    roll.x = (up.x * ((delta.x * delta.x) +
               ((1.0f - (delta.x * delta.x)) *
                cos(3.141592653589793 * rotZ / 180.0)))) +
             (up.y * (((delta.x * delta.y) *
               (1.0 - cos(3.141592653589793 * rotZ / 180.0))) -
               (delta.z * sin(3.141592653589793 * rotZ / 180.0)))) +
             (up.z * (((delta.x * delta.z) *
               (1.0 - cos(3.141592653589793 * rotZ / 180.0))) +
               (delta.y * sin(3.141592653589793 * rotZ / 180.0))));

    roll.y = (up.x * (((delta.x * delta.y) *
               (1.0 - cos(3.141592653589793 * rotZ / 180.0))) +
               (delta.z * sin(3.141592653589793 * rotZ / 180.0)))) +
             (up.y * ((delta.y * delta.y) +
               ((1.0f - (delta.y * delta.y)) *
                cos(3.141592653589793 * rotZ / 180.0)))) +
             (up.z * (((delta.y * delta.z) *
               (1.0 - cos(3.141592653589793 * rotZ / 180.0))) -
               (delta.x * sin(3.141592653589793 * rotZ / 180.0))));

    roll.z = (up.x * (((delta.x * delta.z) *
               (1.0 - cos(3.141592653589793 * rotZ / 180.0))) -
               (delta.y * sin(3.141592653589793 * rotZ / 180.0)))) +
             (up.y * (((delta.y * delta.z) *
               (1.0 - cos(3.141592653589793 * rotZ / 180.0))) +
               (delta.x * sin(3.141592653589793 * rotZ / 180.0)))) +
             (up.z * ((delta.z * delta.z) +
               ((1.0f - (delta.z * delta.z)) *
                cos(3.141592653589793 * rotZ / 180.0))));

    PSVECCrossProduct(&up, &delta, &delta);
    PSVECNormalize(&delta, &delta);

    subStick = HuPadSubStkX[0] & 0xF8;
    if (subStick != 0) {
        CenterM[1].x += (delta.x * (float)subStick) * 0.05f;
        CenterM[1].y += (delta.y * (float)subStick) * 0.05f;
        CenterM[1].z += (delta.z * (float)subStick) * 0.05f;
    }

    PSVECNormalize(&roll, &delta);

    subStick = -(HuPadSubStkY[0] & 0xF8);
    if (subStick != 0) {
        CenterM[1].x += (delta.x * (float)subStick) * 0.05f;
        CenterM[1].y += (delta.y * (float)subStick) * 0.05f;
        CenterM[1].z += (delta.z * (float)subStick) * 0.05f;
    }
}

/* 0x50DC */
void fn_1_50DC(void (*func)(void), s32 arg1)
{
    HUPROCESS *proc = HuPrcChildCreate(func, 0x10, 0x3000, 0, HuPrcCurrentGet());
    proc->property = (void *)arg1;
}

/* 0x5130 */
void fn_1_5130(void)
{
    HUPROCESS *proc;
    s32 bank;
    s16 i;
    f32 angle;

    proc = HuPrcCurrentGet();
    bank = (s32)proc->property;
    for (i = 1; i <= 10; i++) {
        angle = (float)i / 10.0f;
        if (i == 5) {
            Hu3DAnimBankSet(lbl_1_bss_A2[0], (u16)bank);
            Hu3DAnimBankSet(lbl_1_bss_A2[1], (u16)bank);
        }
        Hu3DModelRotSet(lbl_1_bss_2D8[2], 0.0f, 180.0f * angle, 0.0f);
        HuPrcVSleep();
    }
    HuPrcEnd();
}

/* 0x5244 */
s32 fn_1_5244(s16 arg)
{
    HUPROCESS *proc;
    HUPROCESS *procB;
    Vec pos[3];
    FILESEL_WORK *wpa;
    FILESEL_WORK *wpb;
    FILESEL_WORK *wpc;
    FILESEL_WORK *wpd;
    FILESEL_WORK *wpe;
    FILESEL_WORK *wpf;
    s16 cur;
    s16 prev;
    s16 i;
    s16 dir;
    s32 ret;
    s16 emptyCnt;
    s32 sndH;
    f32 scale;
    f32 sx;
    f32 sy;
    f32 sz;
    f32 nx;
    f32 ny;
    f32 nz;
    f32 px;
    f32 py;
    f32 pz;
    f32 rx;
    f32 ry;
    f32 rz;
    f32 ex;
    f32 ey;
    f32 ez;
    HUPROCESS *procC;
    f32 qx;
    f32 qy;
    f32 qz;
    HUPROCESS *procD;

    cur = 0;
    proc = HuPrcChildCreate(fn_1_5130, 0x10, 0x3000, 0, HuPrcCurrentGet());
    proc->property = (void *)2;
    for (i = 0; i < 3; i++) {
        Hu3DModelPosGet(lbl_1_bss_D8[i].unk_8, &pos[i]);
    }
    HuWinInsertMesSet(lbl_1_bss_2E0, (u32)lbl_1_bss_D8[arg].unk_44, 0);
    HuWinMesSet(lbl_1_bss_2E0, 0x9000e);
    HuWinMesWait(lbl_1_bss_2E0);
    HuWinMesSet(lbl_1_bss_2DE, 0x10002);
    HuWinDispOn(lbl_1_bss_2DE);
    for (cur = 0; cur < 3; cur++) {
        if (lbl_1_bss_D8[cur].unk_0 == 0) {
            break;
        }
    }
    lbl_1_bss_D8[cur].unk_62 = 0;
    for (emptyCnt = 0, i = emptyCnt; i < 3; i++) {
        if (lbl_1_bss_D8[i].unk_0 == 0) {
            emptyCnt++;
        }
    }
    for (i = 1; i <= 5; i++) {
        scale = (f32)i / 5.0f;
        Hu3DModelScaleSet(lbl_1_bss_D8[cur].unk_8, (f32)(1.5 + 0.5 * scale), (f32)(1.5 + 0.5 * scale), (f32)(1.5 + 0.5 * scale));
        sz = pos[cur].z + 20.0f * scale;
        sy = pos[cur].y;
        sx = pos[cur].x;
        wpa = &lbl_1_bss_D8[cur];
        wpa->unk_64.x = sx;
        wpa->unk_64.y = sy;
        wpa->unk_64.z = sz;
        HuPrcVSleep();
    }
    lbl_1_bss_D8[cur].unk_12 = 1;
    prev = cur;
    for (;;) {
        if (emptyCnt > 1) {
            dir = 0;
            if (HuPadDStkRep[0] & 1) {
                dir = -1;
            } else if (HuPadDStkRep[0] & 2) {
                dir = 1;
            }
            if (dir != 0) {
                HuAudFXPlay(0x483);
                prev = cur;
                cur += dir;
                if (cur < 0) {
                    cur = 2;
                } else if (cur >= 3) {
                    cur = 0;
                }
                while (lbl_1_bss_D8[cur].unk_0 != 0) {
                    cur += dir;
                    if (cur < 0) {
                        cur = 2;
                    } else if (cur >= 3) {
                        cur = 0;
                    }
                }
                for (i = 1; i <= 5; i++) {
                    scale = sin(3.141592653589793 * (130.0 * (i / 5.0)) / 180.0) * (1.0 / sin(2.2689280275926285));
                    scale = 1.5 + 0.5 * scale;
                    Hu3DModelScaleSet(lbl_1_bss_D8[cur].unk_8, scale, scale, scale);
                    scale = i / 5.0;
                    nz = pos[cur].z + 20.0f * scale;
                    ny = pos[cur].y;
                    nx = pos[cur].x;
                    wpb = &lbl_1_bss_D8[cur];
                    wpb->unk_64.x = nx;
                    wpb->unk_64.y = ny;
                    wpb->unk_64.z = nz;
                    scale = (f32)(1.5 + 0.5 * (1.0 - i / 5.0));
                    Hu3DModelScaleSet(lbl_1_bss_D8[prev].unk_8, scale, scale, scale);
                    scale = 1.0 - i / 5.0;
                    pz = pos[prev].z + 20.0f * scale;
                    py = pos[prev].y;
                    px = pos[prev].x;
                    wpc = &lbl_1_bss_D8[prev];
                    wpc->unk_64.x = px;
                    wpc->unk_64.y = py;
                    wpc->unk_64.z = pz;
                    HuPrcVSleep();
                }
                lbl_1_bss_D8[cur].unk_12 = 1;
                lbl_1_bss_D8[prev].unk_12 = 0;
                lbl_1_bss_D8[cur].unk_62 = 0;
                lbl_1_bss_D8[prev].unk_62 = 1;
            }
        }
        if (UnMountCnt != 0) {
            UnMountCnt = 0;
            lbl_1_bss_D8[cur].unk_12 = 0;
            HuWinHomeClear(lbl_1_bss_2DE);
            procB = HuPrcChildCreate(fn_1_5130, 0x10, 0x3000, 0, HuPrcCurrentGet());
            procB->property = (void *)1;
            return -4;
        }
        if (HuPadBtnDown[0] & 0x100) {
            HuAudFXPlay(1);
            goto confirm;
        }
        if (HuPadBtnDown[0] & 0x200) {
            HuAudFXPlay(3);
            goto cancel;
        }
        HuPrcVSleep();
        continue;
    confirm:
        HuWinHomeClear(lbl_1_bss_2DE);
        HuWinHomeClear(lbl_1_bss_2E0);
        for (i = 1; i <= 20; i++) {
            scale = (f32)(1.5 * (1.0 - sin(3.141592653589793 * (90.0 * (i / 20.0)) / 180.0)));
            Hu3DModelScaleSet(lbl_1_bss_D8[cur].unk_8, 1.5f * scale, 1.5f * scale, 1.5f * scale);
            HuPrcVSleep();
        }
        lbl_1_bss_D8[cur].unk_12 = 0;
        memcpy(&saveBuf[curSlotNo][SLBoxDataOffsetGet(cur)], &saveBuf[curSlotNo][SLBoxDataOffsetGet(arg)], 0xcb2);
        sndH = HuAudFXPlay(0x484);
        ret = fn_1_D90C(-1, 0x90028);
        HuAudFXStop(sndH);
        HuAudFXPlay(0x485);
        if (ret == -0x4d2) {
            ret = -4;
        }
        if (ret == 0) {
            lbl_1_bss_D8[cur].unk_0 = 1;
            lbl_1_bss_D8[cur].unk_14 = lbl_1_bss_D8[arg].unk_14;
            lbl_1_bss_D8[cur].unk_1C = lbl_1_bss_D8[arg].unk_1C;
            lbl_1_bss_D8[cur].unk_16 = lbl_1_bss_D8[arg].unk_16;
            memcpy(lbl_1_bss_D8[cur].unk_44, lbl_1_bss_D8[arg].unk_44, 0x11);
            fn_1_36C4(cur, cur);
            HuAudFXPlay(0x488);
            rz = pos[cur].z;
            ry = pos[cur].y;
            rx = pos[cur].x;
            wpd = &lbl_1_bss_D8[cur];
            wpd->unk_64.x = rx;
            wpd->unk_64.y = ry;
            wpd->unk_64.z = rz;
            for (i = 1; i <= 10; i++) {
                scale = (f32)(1.5 * (i / 10.0));
                Hu3DModelScaleSet(lbl_1_bss_D8[cur].unk_8, scale, scale, scale);
                HuPrcVSleep();
            }
            lbl_1_bss_D8[cur].unk_62 = 1;
            HuWinMesSet(lbl_1_bss_2E0, 0x90010);
            HuWinMesWait(lbl_1_bss_2E0);
        } else {
            ez = pos[cur].z;
            ey = pos[cur].y;
            ex = pos[cur].x;
            wpe = &lbl_1_bss_D8[cur];
            wpe->unk_64.x = ex;
            wpe->unk_64.y = ey;
            wpe->unk_64.z = ez;
            for (i = 1; i <= 5; i++) {
                scale = (f32)(1.5 * (i / 5.0));
                Hu3DModelScaleSet(lbl_1_bss_D8[cur].unk_8, scale, scale, scale);
                HuPrcVSleep();
            }
            lbl_1_bss_D8[cur].unk_62 = 1;
        }
        procC = HuPrcChildCreate(fn_1_5130, 0x10, 0x3000, 0, HuPrcCurrentGet());
        procC->property = (void *)1;
        return ret;
    cancel:
        HuWinHomeClear(lbl_1_bss_2DE);
        lbl_1_bss_D8[cur].unk_12 = 0;
        for (i = 1; i <= 5; i++) {
            scale = (f32)(1.5 * (1.0 + 0.5 * (1.0 - i / 5.0)));
            Hu3DModelScaleSet(lbl_1_bss_D8[cur].unk_8, scale, scale, scale);
            scale = 1.0 - i / 5.0;
            qz = pos[cur].z + 20.0f * scale;
            qy = pos[cur].y;
            qx = pos[cur].x;
            wpf = &lbl_1_bss_D8[cur];
            wpf->unk_64.x = qx;
            wpf->unk_64.y = qy;
            wpf->unk_64.z = qz;
            HuPrcVSleep();
        }
        lbl_1_bss_D8[cur].unk_62 = 1;
        procD = HuPrcChildCreate(fn_1_5130, 0x10, 0x3000, 0, HuPrcCurrentGet());
        procD->property = (void *)1;
        return -0x4d2;
    }
}

/* 0x6314 */
void fn_1_6314(s16 id, s32 flag)
{
    if (flag != 0) {
        Hu3DAnimAnimSet(lbl_1_bss_D8[id].unk_18, lbl_1_bss_CC[1]);
    } else {
        Hu3DAnimAnimSet(lbl_1_bss_D8[id].unk_18, lbl_1_bss_CC[0]);
    }
}

/* 0x63A0 */
void fn_1_63A0(s16 id, s32 flag)
{
    if (flag != 0) {
        Hu3DAnimAnimSet(lbl_1_bss_D8[id].unk_18, lbl_1_bss_CC[2]);
    } else {
        Hu3DAnimAnimSet(lbl_1_bss_D8[id].unk_18, lbl_1_bss_CC[0]);
    }
}

/* 0x642C */
s32 fn_1_642C(s16 arg0)
{
    HUWIN *w;
    s32 ret;
    int fxId;
    int i;
    HUPROCESS *proc1;
    HUPROCESS *proc2;
    HUPROCESS *proc3;
    HUPROCESS *proc4;
    f32 s;

    w = &winData[lbl_1_bss_2E0];
    proc1 = HuPrcChildCreate(fn_1_5130, 0x10, 0x3000, 0, HuPrcCurrentGet());
    proc1->property = (void *)3;
    Hu3DAnimAnimSet(lbl_1_bss_D8[arg0].unk_18, lbl_1_bss_CC[1]);
    HuWinInsertMesSet(lbl_1_bss_2E0, (u32)&lbl_1_bss_D8[arg0].unk_44, 0);
    HuWinMesSet(lbl_1_bss_2E0, 0x90013);
    HuWinMesWait(lbl_1_bss_2E0);
    HuWinChoiceSet(lbl_1_bss_2E0, -1);
    while (w->stat != 0) {
        if (UnMountCnt != 0) {
            HuWinMesSet(lbl_1_bss_2E0, 0x90026);
            HuWinMesWait(lbl_1_bss_2E0);
            UnMountCnt = 0;
            proc2 = HuPrcChildCreate(fn_1_5130, 0x10, 0x3000, 0, HuPrcCurrentGet());
            proc2->property = (void *)1;
            Hu3DAnimAnimSet(lbl_1_bss_D8[arg0].unk_18, lbl_1_bss_CC[0]);
            return -4;
        }
        HuPrcVSleep();
    }
    ret = w->choice;
    HuWinHomeClear(lbl_1_bss_2E0);
    if (UnMountCnt != 0) {
        HuWinMesSet(lbl_1_bss_2E0, 0x90026);
        HuWinMesWait(lbl_1_bss_2E0);
        UnMountCnt = 0;
        proc3 = HuPrcChildCreate(fn_1_5130, 0x10, 0x3000, 0, HuPrcCurrentGet());
        proc3->property = (void *)1;
        Hu3DAnimAnimSet(lbl_1_bss_D8[arg0].unk_18, lbl_1_bss_CC[0]);
        return -4;
    }
    if (ret == 0) {
        fxId = HuAudFXPlay(0x486);
        SLSaveEmptySet(curSlotNo, arg0);
        ret = fn_1_D90C(-1, 0x90029);
        HuAudFXStop(fxId);
        fxId = HuAudFXPlay(0x487);
        if (ret == 0) {
            HuAudFXPlay(0x489);
            for (i = 1; i <= 0x14; i++) {
                s = lbl_1_rodata_90 - sin(lbl_1_rodata_80 * (lbl_1_rodata_88 * ((double)i / lbl_1_rodata_190)) / lbl_1_rodata_98);
                Hu3DModelScaleSet(lbl_1_bss_D8[arg0].unk_8, lbl_1_rodata_C0 * s, lbl_1_rodata_C0 * s, lbl_1_rodata_C0 * s);
                HuPrcVSleep();
            }
            HuPrcSleep(0x1e);
            lbl_1_bss_D8[arg0].unk_0 = 0;
            lbl_1_bss_D8[arg0].unk_0 = 0;
            fn_1_36C4(arg0, arg0);
            lbl_1_bss_D8[arg0].unk_12 = 1;
            Hu3DAnimAnimSet(lbl_1_bss_D8[arg0].unk_18, lbl_1_bss_CC[0]);
            for (i = 1; i <= 0xa; i++) {
                s = (double)i / lbl_1_rodata_108;
                Hu3DModelScaleSet(lbl_1_bss_D8[arg0].unk_8, lbl_1_rodata_C0 * s, lbl_1_rodata_C0 * s, lbl_1_rodata_C0 * s);
                HuPrcVSleep();
            }
            HuWinMesSet(lbl_1_bss_2E0, 0x90016);
            HuWinMesWait(lbl_1_bss_2E0);
        }
    } else {
        ret = 0;
    }
    proc4 = HuPrcChildCreate(fn_1_5130, 0x10, 0x3000, 0, HuPrcCurrentGet());
    proc4->property = (void *)1;
    Hu3DAnimAnimSet(lbl_1_bss_D8[arg0].unk_18, lbl_1_bss_CC[0]);
    return ret;
}

/* 0x69B0 */
s32 fn_1_69B0(void)
{
    GwCommon = GwCommonOrig;
    if (OSGetSoundMode() == 0) {
        GwCommon.outputMode = 0;
    } else {
        GwCommon.outputMode = 1;
    }
    if (HuMCProbe(1) != 0) {
        HuMCMicSet(0);
    } else {
        HuMCMicSet(1);
        HuMCInit(0);
        if (HuMCMount(1) != 0) {
            HuMCMicSet(0);
        }
        HuMCClose();
    }
    return HuMCMicGet();
}

/* 0x6A8C */
void fn_1_6A8C(void)
{
    HU3D_CAMERA *cam = &Hu3DCamera[Hu3DCameraNo];
    Mtx44 proj;
    Mtx view;
    Vec camPos;
    Vec target;
    Vec up;
    f32 nearz;

    C_MTXPerspective(proj, cam->fov, cam->aspect, cam->near, cam->far);
    GXSetProjection(proj, 0);
    GXSetViewport(cam->viewportX, cam->viewportY, cam->viewportW, cam->viewportH, cam->viewportNear, cam->viewportFar);
    GXSetScissor(cam->scissorX, cam->scissorY, cam->scissorW, cam->scissorH);
    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxAttrFmt(0, 9, 1, 4, 0);
    GXSetBlendMode(1, 4, 4, 5);
    GXSetTevOrder(0, 0xff, 0xff, 0);
    GXSetTevColorIn(0, 0xf, 0xf, 0xf, 0xc);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaIn(0, 7, 7, 7, 7);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetNumTexGens(0);
    GXSetNumTevStages(1);
    GXSetCullMode(0);
    GXSetAlphaCompare(7, 0, 1, 7, 0);
    GXSetZMode(1, 7, 1);
    GXSetColorUpdate(0);

    camPos.x = camPos.y = camPos.z = 0.0f;
    target.x = target.y = 0.0f;
    target.z = -100.0f;
    up.x = up.z = 0.0f;
    up.y = 1.0f;
    C_MTXLookAt(view, &camPos, &up, &target);
    GXLoadPosMtxImm(view, 0);

    nearz = cam->far - 1.0f;
    camPos.x = camPos.y = nearz * tan(3.141592653589793 * (cam->fov / 2.0f) / 180.0);
    camPos.x = camPos.x * cam->aspect;

    GXBegin(0x80, 0, 4);
    GXPosition3f32(-camPos.x, -camPos.y, -nearz);
    GXPosition3f32(camPos.x, -camPos.y, -nearz);
    GXPosition3f32(camPos.x, camPos.y, -nearz);
    GXPosition3f32(-camPos.x, camPos.y, -nearz);

    GXSetColorUpdate(1);
}

/* 0x6E54 */
void fn_1_6E54(s16 a, s32 b)
{
    HUPROCESS *proc1;
    HUPROCESS *proc2;
    FILESEL_WORK *wp;
    HUPROCESS *proc3;
    FILESEL_WORK *bp;
    Vec vpos;
    Vec scale30;
    Vec pos24;
    Vec pos18;
    Vec rot0c;
    s16 i;
    s16 j;
    s32 dir;
    f32 phase;
    f32 sv;
    f32 dirf;

    vpos = lbl_1_rodata_19C;
    Hu3DModelPosGet(lbl_1_bss_2D8[0], &pos18);
    Hu3DModelRotGet(lbl_1_bss_2D8[0], &rot0c);
    rot0c.x = (f32)((s32)rot0c.x % 360);
    if (rot0c.x > lbl_1_rodata_184) {
        rot0c.x -= lbl_1_rodata_78;
    }
    rot0c.y = (f32)((s32)rot0c.y % 360);
    if (rot0c.y > lbl_1_rodata_184) {
        rot0c.y -= lbl_1_rodata_78;
    }
    rot0c.z = (f32)((s32)rot0c.z % 360);
    if (rot0c.z > lbl_1_rodata_184) {
        rot0c.z -= lbl_1_rodata_78;
    }
    if (b == 0) {
        HuWinDispOff(lbl_1_bss_2DE);
        HuWinExClose(lbl_1_bss_2E0);
        Hu3DModelScaleGet(lbl_1_bss_2D8[2], &scale30);
        if (scale30.x > lbl_1_rodata_17C) {
            for (i = 1; i <= 10; i++) {
                phase = (f32)i / lbl_1_rodata_44;
                Hu3DModelRotSet(lbl_1_bss_2D8[2], lbl_1_rodata_38, lbl_1_rodata_1A8 * phase, lbl_1_rodata_38);
                sv = 1.0 - phase;
                Hu3DModelScaleSet(lbl_1_bss_2D8[2], sv, sv, sv);
                HuPrcVSleep();
            }
        }
        for (i = 1; i <= 20; i++) {
            phase = (f32)sin(lbl_1_rodata_80 * (lbl_1_rodata_88 * ((f32)i / lbl_1_rodata_2C)) / lbl_1_rodata_98);
            scale30.x = (1.0 - phase) * pos18.x;
            scale30.y = (1.0 - phase) * pos18.y;
            scale30.z = (1.0 - phase) * pos18.z;
            Hu3DModelPosSetV(lbl_1_bss_2D8[0], &scale30);
            scale30.x = (1.0 - phase) * rot0c.x;
            scale30.y = (1.0 - phase) * rot0c.y;
            scale30.z = (1.0 - phase) * rot0c.z;
            Hu3DModelRotSetV(lbl_1_bss_2D8[0], &scale30);
            HuPrcVSleep();
        }
        proc1 = HuPrcChildCreate(fn_1_7A0C, 0x10, 0x3000, 0, HuPrcCurrentGet());
        proc1->property = (void *)0;
        HuPrcSleep(0x28);
        HuAudFXPlay(0x48c);
        fn_1_8510(0, &vpos);
    } else {
        for (i = 0; i < 3; i++) {
            lbl_1_bss_D8[a].unk_62 = 0;
        }
        HuWinDispOff(lbl_1_bss_2DE);
        HuWinExClose(lbl_1_bss_2E0);
        for (i = 0; i < 3; i++) {
            lbl_1_bss_D8[a].unk_62 = 2;
        }
        for (i = 1; i <= 10; i++) {
            phase = (f32)i / lbl_1_rodata_44;
            for (j = 0; j < 3; j++) {
                if (j != a) {
                    bp = &lbl_1_bss_D8[j];
                    Hu3DModelRotSet(bp->unk_8, lbl_1_rodata_38, lbl_1_rodata_1A8 * phase, lbl_1_rodata_38);
                    sv = lbl_1_rodata_F8 * (1.0 - phase);
                    Hu3DModelScaleSet(bp->unk_8, sv, sv, sv);
                }
            }
            Hu3DModelRotSet(lbl_1_bss_2D8[2], lbl_1_rodata_38, lbl_1_rodata_1A8 * phase, lbl_1_rodata_38);
            sv = 1.0 - phase;
            Hu3DModelScaleSet(lbl_1_bss_2D8[2], sv, sv, sv);
            HuPrcVSleep();
        }
        bp = &lbl_1_bss_D8[a];
        Hu3DModelPosGet(bp->unk_8, &pos24);
        OSReport(lbl_1_data_1F3, pos24.x, pos24.y, pos24.z);
        if ((s32)frandmod(2) == 0) {
            dir = 1;
        } else {
            dir = -1;
        }
        dirf = (f32)dir;
        for (i = 1; i <= 80; i++) {
            if (i <= 20) {
                phase = (f32)sin(lbl_1_rodata_80 * (lbl_1_rodata_88 * ((f32)i / lbl_1_rodata_2C)) / lbl_1_rodata_98);
                scale30.x = (1.0 - phase) * pos18.x;
                scale30.y = (1.0 - phase) * pos18.y;
                scale30.z = (1.0 - phase) * pos18.z;
                Hu3DModelPosSetV(lbl_1_bss_2D8[0], &scale30);
                scale30.x = (1.0 - phase) * rot0c.x;
                scale30.y = (1.0 - phase) * rot0c.y;
                scale30.z = (1.0 - phase) * rot0c.z;
                Hu3DModelRotSetV(lbl_1_bss_2D8[0], &scale30);
                Hu3DShadowTPLvlSet(lbl_1_rodata_100 * (1.0 - phase));
            }
            if (i == 10) {
                proc2 = HuPrcChildCreate(fn_1_7B64, 0x10, 0x3000, 0, HuPrcCurrentGet());
                proc2->property = (void *)a;
                HuAudFXPlay(0x48b);
            }
            if (i > 10) {
                phase = ((f32)i - lbl_1_rodata_44) / lbl_1_rodata_AC;
                scale30.x = (1.0 - phase) * pos24.x;
                scale30.y = (1.0 - phase) * pos24.y + lbl_1_rodata_1B0 * sin(lbl_1_rodata_80 * (lbl_1_rodata_184 * phase) / lbl_1_rodata_98);
                scale30.z = pos24.z + lbl_1_rodata_1B8 * sin(lbl_1_rodata_80 * (lbl_1_rodata_184 * phase) / lbl_1_rodata_98) - lbl_1_rodata_1C0 * phase;
                wp = &lbl_1_bss_D8[a];
                wp->unk_64 = scale30;
                Hu3DModelRotSet(bp->unk_8, lbl_1_rodata_38, lbl_1_rodata_40 * phase, 45.0f * phase * dirf);
                sv = lbl_1_rodata_1C8 + lbl_1_rodata_1C8 * (1.0 - phase);
                Hu3DModelScaleSet(bp->unk_8, sv, sv, sv);
            }
            if (i == 40) {
                proc3 = HuPrcChildCreate(fn_1_7A0C, 0x10, 0x3000, 0, HuPrcCurrentGet());
                proc3->property = (void *)0;
            }
            if (i == 75) {
                HuAudFXPlay(0x48c);
                fn_1_8510(0, &vpos);
            }
            HuPrcVSleep();
        }
    }
}

/* 0x7A0C */
void fn_1_7A0C(void)
{
    HU3D_CAMERA *cam = &Hu3DCamera[1];
    f32 zoom0 = CZoomM[1];
    s16 i;
    f32 t;

    for (i = 0; i < 100.0; i++) {
        t = i / 100.0;
        t = 1.0 - cos(3.14159265358979323846 * (90.0 * t) / 180.0);
        CZoomM[1] = zoom0 + (200.0f - zoom0) * t;
        HuPrcVSleep();
    }
    HuPrcEnd();
}

/* 0x7B64 */
void fn_1_7B64(void)
{
    HUPROCESS *proc;
    s32 idx;
    s32 i;
    Mtx objMtx;
    Vec dir;
    Vec trans;
    Mtx tmpMtx;
    VEC5 arr;

    proc = HuPrcCurrentGet();
    idx = (s32)proc->property;
    arr = lbl_1_rodata_1D0;

    for (i = 0; i < 5; i++) {
        Hu3DParManAttrReset(lbl_1_bss_98[i], 1);
    }

    while (1) {
        Hu3DModelObjMtxGet(lbl_1_bss_D8[idx].unk_8, lbl_1_data_1FD, objMtx);
        Hu3DMtxTransGet(objMtx, &trans);
        if (trans.z > lbl_1_rodata_38) {
            for (i = 0; i < 5; i++) {
                PSMTXTrans(tmpMtx, arr.v[i].x, arr.v[i].y, arr.v[i].z);
                PSMTXConcat(objMtx, tmpMtx, tmpMtx);
                Hu3DMtxTransGet(tmpMtx, &dir);
                Hu3DParManPosSet(lbl_1_bss_98[i], dir.x, dir.y, dir.z);
                PSVECSubtract(&dir, &trans, &dir);
                if (dir.z * dir.z + (dir.x * dir.x + dir.y * dir.y) <= lbl_1_rodata_210) {
                    dir.x = dir.y = dir.z = lbl_1_rodata_38;
                } else {
                    PSVECNormalize(&dir, &dir);
                }
                Hu3DParManVecSet(lbl_1_bss_98[i], dir.x, lbl_1_rodata_218, dir.z);
            }
        } else {
            for (i = 0; i < 5; i++) {
                Hu3DParManAttrSet(lbl_1_bss_98[i], 1);
            }
        }
        HuPrcVSleep();
    }
}

/* 0x7DEC */
void fn_1_7DEC(void)
{
    if (lbl_1_bss_C != 0) {
        GXSetTexCopySrc(0, 0, 0x280, 0x1e0);
        GXSetTexCopyDst(0x280, 0x1e0, 4, 0);
        GXCopyTex(lbl_1_bss_C, 0);
    }
}

/* 0x7E58 */
void fn_1_7E58(F7E58_Arg *arg)
{
    HU3D_CAMERA *cam = &Hu3DCamera[Hu3DCameraNo];
    F7E58_Q *q;
    Mtx tmpMtx;
    Mtx scaleMtx;
    Mtx texMtx;
    Mtx lightMtx;
    Mtx invMtx;
    GXTexObj texObj;
    f32 transS;

    q = arg->unk_00->unk_120;
    q->unk_1c = q->unk_1c + q->unk_20;

    GXInitTexObj(&texObj, lbl_1_bss_C, 0x280, 0x1e0, 4, 0, 0, 0);
    GXInitTexObjLOD(&texObj, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
    GXLoadTexObj(&texObj, 1);
    HuSprTexLoad(lbl_1_bss_10, 0, 2, 1, 1, 1);
    HuSprTexLoad(lbl_1_bss_14, 0, 3, 1, 1, 1);
    GXSetNumTexGens(3);
    GXSetNumTevStages(2);

    transS = ((f32)cam->scissorX + (f32)cam->scissorW / 2.0f) / 640.0f;
    C_MTXLightPerspective(lightMtx, cam->fov, 1.2f, 0.5f, -0.5f, transS, 0.5f);
    PSMTXInverse(Hu3DCameraMtx, invMtx);
    PSMTXConcat(invMtx, arg->unk_0c, tmpMtx);
    PSMTXConcat(lightMtx, Hu3DCameraMtx, texMtx);
    PSMTXConcat(texMtx, tmpMtx, texMtx);
    GXLoadTexMtxImm(texMtx, 0x21, 0);
    GXSetTexCoordGen2(0, 0, 0, 0x21, 0, 0x7d);
    PSMTXTrans(tmpMtx, 0.0f, q->unk_1c, 0.0f);
    PSMTXScale(scaleMtx, 0.8f, 0.8f, 1.0f);
    PSMTXConcat(scaleMtx, tmpMtx, texMtx);
    GXLoadTexMtxImm(texMtx, 0x1e, 1);
    GXSetTexCoordGen2(1, 1, 4, 0x1e, 0, 0x7d);
    GXSetTexCoordGen2(2, 1, 4, 0x3c, 0, 0x7d);

    GXSetTevColor(1, q->color);
    GXSetTevOrder(0, 0, 1, 0);
    GXSetTevColorIn(0, 0xf, 0xf, 0xf, 8);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaIn(0, 7, 7, 7, 6);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevOrder(1, 1, 0, 0);
    GXSetTevColorIn(1, 0xf, 8, 2, 0);
    GXSetTevColorOp(1, 0, 0, 0, 1, 0);
    GXSetTevAlphaIn(1, 7, 7, 7, 6);
    GXSetTevAlphaOp(1, 0, 0, 0, 1, 0);
    GXSetNumIndStages(1);
    GXSetIndTexOrder(0, 1, 2);
    GXSetIndTexCoordScale(0, 0, 0);
    GXSetTevIndWarp(0, 0, 1, 0, 1);
    GXSetIndTexMtx(1, (f32 (*)[3])q, -1);
}

/* 0x82F0 */
void fn_1_82F0(OMOBJ *obj)
{
    BSS18_ENTRY *e;
    s16 i;
    f32 t;

    for (i = 0, e = lbl_1_bss_18; i < 2; i++, e++) {
        if (e->unk_24 <= lbl_1_rodata_38) {
            continue;
        }
        e->unk_0.x = lbl_1_rodata_38;
        e->unk_0.y = lbl_1_rodata_38;
        e->unk_0.z = lbl_1_rodata_220 * e->unk_24 / lbl_1_rodata_224;
        e->unk_c.x = lbl_1_rodata_38;
        e->unk_c.y = lbl_1_rodata_38;
        e->unk_c.z = lbl_1_rodata_220 * e->unk_24 / lbl_1_rodata_224;
        e->unk_20 = lbl_1_rodata_228 * e->unk_24 / lbl_1_rodata_224;
        t = (e->unk_24 - lbl_1_rodata_22C) / lbl_1_rodata_230;
        if (t >= lbl_1_rodata_38) {
            e->unk_18.r = 0;
            e->unk_18.g = lbl_1_rodata_74 * t;
            e->unk_18.b = lbl_1_rodata_234 * t;
        }
        e->unk_24 = e->unk_24 - lbl_1_rodata_1C;
        if (e->unk_24 <= lbl_1_rodata_38) {
            Hu3DLayerHookReset(0xa);
            Hu3DModelAttrSet(obj->mdlId[i], 1);
            obj->objFunc = 0;
        }
    }
}

/* 0x8510 */
void fn_1_8510(s16 id, Vec *pos)
{
    OMOBJ *p = lbl_1_bss_8;

    if (p == 0) {
        return;
    }
    if (id >= 2 || id < 0) {
        return;
    }
    lbl_1_bss_18[id].unk_1c = lbl_1_rodata_38;
    lbl_1_bss_18[id].unk_24 = lbl_1_rodata_224;
    Hu3DModelDispOn(p->mdlId[id]);
    Hu3DModelPosSet(p->mdlId[id], pos->x, pos->y, pos->z);
    Hu3DModelRotSet(p->mdlId[id], lbl_1_rodata_70, lbl_1_rodata_38, lbl_1_rodata_38);
    Hu3DModelScaleSet(p->mdlId[id], lbl_1_rodata_1C, lbl_1_rodata_1C, lbl_1_rodata_1C);
    Hu3DLayerHookSet(0xa, (HU3D_LAYER_HOOK)fn_1_7DEC);
    p->objFunc = fn_1_82F0;
}

/* 0x8688 */
void fn_1_8688(OMOBJ *p)
{
    GXColor color;
    GXColor *cp;
    s16 i;
    HU3D_MODEL *mdl = NULL;

    omSetStatBit(p, 0x100);
    lbl_1_bss_10 = HuSprAnimRead(HuDataSelHeapReadNum(0x230019, 0x10000000, 2));
    lbl_1_bss_14 = HuSprAnimRead(HuDataSelHeapReadNum(0x230019, 0x10000000, 2));
    lbl_1_bss_C = HuMemDirectMallocNum(2, GXGetTexBufferSize(0x280, 0x1e0, 4, 0, 0), 0x10000000);
    for (i = 0; i < 2; i++) {
        p->mdlId[i] = Hu3DModelCreate(HuDataSelHeapReadNum(0x230018, 0x10000000, 2));
        Hu3DModelPosSet(p->mdlId[i], lbl_1_rodata_38, lbl_1_rodata_38, lbl_1_rodata_38);
        Hu3DModelRotSet(p->mdlId[i], lbl_1_rodata_70, lbl_1_rodata_38, lbl_1_rodata_38);
        Hu3DModelScaleSet(p->mdlId[i], lbl_1_rodata_17C, lbl_1_rodata_17C, lbl_1_rodata_17C);
        Hu3DModelCameraSet(p->mdlId[i], 2);
        Hu3DModelLayerSet(p->mdlId[i], 3);
        Hu3DModelAttrSet(p->mdlId[i], 1);
        Hu3DModelMatHookSet(p->mdlId[i], (HU3D_MAT_HOOK)fn_1_7E58);
        lbl_1_bss_18[i].unk_0.x = lbl_1_bss_18[i].unk_0.y = lbl_1_bss_18[i].unk_0.z =
            lbl_1_bss_18[i].unk_c.x = lbl_1_bss_18[i].unk_c.y = lbl_1_bss_18[i].unk_c.z =
                lbl_1_rodata_38;
        color = lbl_1_rodata_238;
        cp = &color;
        lbl_1_bss_18[i].unk_18 = *cp;
        lbl_1_bss_18[i].unk_1c = lbl_1_rodata_38;
        lbl_1_bss_18[i].unk_20 = lbl_1_rodata_38;
        mdl = &Hu3DData[p->mdlId[i]];
        mdl->hookData = &lbl_1_bss_18[i];
    }
    p->objFunc = NULL;
    lbl_1_bss_8 = p;
}

/* 0x8A34 */
void fn_1_8A34(void)
{
    OMOBJ *obj = lbl_1_bss_8;
    HU3D_MODEL *m = 0;
    s16 i;

    if (obj != 0) {
        Hu3DLayerHookReset(0xa);
        for (i = 0; i < 2; i++) {
            m = &Hu3DData[obj->mdlId[i]];
            m->hookData = 0;
            Hu3DModelKill(obj->mdlId[i]);
        }
        omDelObjEx(lbl_1_bss_340, obj);
    }
    obj = 0;
    if (lbl_1_bss_10 != 0) {
        HuSprAnimKill(lbl_1_bss_10);
    }
    lbl_1_bss_10 = 0;
    if (lbl_1_bss_14 != 0) {
        HuSprAnimKill(lbl_1_bss_14);
    }
    lbl_1_bss_14 = 0;
    if (lbl_1_bss_C != 0) {
        HuMemDirectFree(lbl_1_bss_C);
    }
    lbl_1_bss_C = 0;
}

/* 0x8B94 */
void fn_1_8B94(void)
{
    HUSPR_GROUPID grp;
    ANIMDATA *anim;
    HUSPRID spr;
    s16 k;
    DATA48C *d;
    u8 *msg;
    s16 i;

    grp = HuSprGrpCreate(0x12);
    lbl_1_bss_42E = grp;
    for (i = 0; i < 0x12; i++) {
        d = &lbl_1_data_48C[i];
        anim = HuSprAnimRead(HuDataSelHeapReadNum(d->unk_0, 0x10000000, 2));
        spr = HuSprCreate(anim, d->unk_6, d->unk_4);
        HuSprGrpMemberSet(grp, i, spr);
        HuSprPosSet(grp, i, d->unk_8, d->unk_A);
        HuSprAttrSet(grp, i, 4);
    }
    HuSprGrpPosSet(grp, lbl_1_rodata_240, lbl_1_rodata_244);
    for (i = 0; i < 2; i++) {
        HUWIN *w;
        lbl_1_bss_3AC[i] = HuWinCreate(lbl_1_rodata_248, lbl_1_rodata_24C, 0x1f4, 0xfa, 0);
        w = &winData[lbl_1_bss_3AC[i]];
        HuWinBGTPLvlSet(lbl_1_bss_3AC[i], lbl_1_rodata_250);
        anim = HuSprAnimRead(HuDataSelHeapReadNum(i + 0x23001F, 0x10000000, 2));
        if (i == 0) {
            HuWinAnimSet(lbl_1_bss_3AC[i], anim, 0, lbl_1_rodata_254, lbl_1_rodata_258);
        } else {
            HuWinAnimSet(lbl_1_bss_3AC[i], anim, 0, lbl_1_rodata_254, lbl_1_rodata_25C);
        }
        HuWinDispOff(lbl_1_bss_3AC[i]);
        HuWinPriSet(lbl_1_bss_3AC[i], 0x6e);
    }
    lbl_1_bss_42A = 0;
    for (k = 0; k < 7; k++) {
        for (i = 0; i < 0x11; i++) {
            lbl_1_bss_3B2[k][i] = 0;
        }
    }
    for (k = 0; k < 7; k++) {
        msg = (u8 *)HuWinMesPtrGet(k);
        i = 0;
        while (*msg != 0) {
            if (*msg >= '0') {
                lbl_1_bss_3B2[k][i++] = *msg;
            }
            msg++;
        }
    }
}

/* 0x8F34 */
void fn_1_8F34(s32 arg0, s16 idx)
{
    u16 btn;

    btn = lbl_1_bss_348[idx].flag;
    HuSprPosSet(lbl_1_bss_42E, 0,
        lbl_1_rodata_268 + ((f32)lbl_1_bss_348[idx].x - lbl_1_rodata_240),
        ((f32)lbl_1_bss_348[idx].y - lbl_1_rodata_244) - lbl_1_rodata_26C);
    if (btn >= 0x100 && btn <= 0x500) {
        HuSprAttrSet(lbl_1_bss_42E, 1, 4);
    } else {
        HuSprAttrReset(lbl_1_bss_42E, 1, 4);
        HuSprPosSet(lbl_1_bss_42E, 1,
            lbl_1_rodata_270 + ((f32)lbl_1_bss_348[idx].x - lbl_1_rodata_240),
            lbl_1_rodata_26C + ((f32)lbl_1_bss_348[idx].y - lbl_1_rodata_244));
    }
}

/* 0x916C  (enter-save-name grid main loop; largest fn in the module).
 * MUST precede fn_1_AAEC/ABF8/B234/B530/B5B4 so -inline auto keeps real bl calls.
 * Two match-critical spellings: the L/R page scans use `(sel + 1) * 0x100` (the MUL
 * node canonicalizes to the target's cmpw operand order — NOT `(sel + 1) << 8`), and
 * the SJIS re-pack loop init `for (j = k = 0; j < count; k++, j++)` (k bumps first). */
s32 fn_1_916C(s16 no)
{
    B234Rect rect;
    s16 cur;
    s16 j;
    s16 k;
    s16 sel;
    HUWINID winId;
    s32 flag26;
    s16 newIdx;
    s16 lastSel;
    u16 code1;
    s32 ret;
    u16 code0;
    u16 code2;
    u16 code3;
    s16 count;
    f32 dirX;
    f32 dirY;
    f32 refX;
    f32 refY;

    sel = 0;
    lastSel = sel;
    cur = 0;
    flag26 = 0;
    lbl_1_bss_37E = 0;
    lbl_1_bss_34C[0] = 0xbf;
    lbl_1_bss_34C[1] = 0;
    lbl_1_bss_348 = lbl_1_data_47C[sel];

    code0 = lbl_1_bss_348[cur].flag;
    HuSprPosSet(lbl_1_bss_42E, 0, lbl_1_rodata_268 + ((f32)lbl_1_bss_348[cur].x - lbl_1_rodata_240),
                (f32)lbl_1_bss_348[cur].y - lbl_1_rodata_244 - lbl_1_rodata_26C);
    if (code0 >= 0x100 && code0 <= 0x500) {
        HuSprAttrSet(lbl_1_bss_42E, 1, 4);
    } else {
        HuSprAttrReset(lbl_1_bss_42E, 1, 4);
        HuSprPosSet(lbl_1_bss_42E, 1, lbl_1_rodata_270 + ((f32)lbl_1_bss_348[cur].x - lbl_1_rodata_240),
                    lbl_1_rodata_26C + ((f32)lbl_1_bss_348[cur].y - lbl_1_rodata_244));
    }

    HuSprBankSet(lbl_1_bss_42E, 3, 8);
    HuSprBankSet(lbl_1_bss_42E, 4, 8);
    HuSprBankSet(lbl_1_bss_42E, 5, 8);
    HuSprBankSet(lbl_1_bss_42E, 6, 2);
    HuSprBankSet(lbl_1_bss_42E, 7, 4);
    HuAudFXPlay(0);
    fn_1_B530();
    HuWinMesSet(lbl_1_bss_2E0, 0x90009);
    HuWinMesSet(lbl_1_bss_2DE, 0x10002);
    HuWinDispOn(lbl_1_bss_2DE);
    winId = HuWinCreate(lbl_1_rodata_274, lbl_1_rodata_278, 0xb0, 0x1a, 0);
    HuWinBGTPLvlSet(winId, lbl_1_rodata_250);
    HuWinMesSpeedSet(winId, 0);
    HuWinPriSet(winId, 0x28);
    HuWinMesSet(winId, (u32)lbl_1_bss_34C);

    for (lbl_1_bss_42C = 0; lbl_1_data_47C[lbl_1_bss_42C] != 0; lbl_1_bss_42C++) {
        ;
    }
    fn_1_B234(&rect);

L_95A8:
    dirX = dirY = lbl_1_rodata_250;
    if (HuPadDStkRep[0] & 1) {
        dirX = lbl_1_rodata_27C;
    } else if (HuPadDStkRep[0] & 2) {
        dirX = lbl_1_rodata_280;
    }
    if (HuPadDStkRep[0] & 8) {
        dirY = lbl_1_rodata_27C;
    } else if (HuPadDStkRep[0] & 4) {
        dirY = lbl_1_rodata_280;
    }
    if (dirX == lbl_1_rodata_250 && dirY == lbl_1_rodata_250) {
        goto L_99B4;
    }
    newIdx = fn_1_ABF8(cur, (f32)lbl_1_bss_348[cur].x, (f32)lbl_1_bss_348[cur].y, dirX, dirY);
    if (newIdx == -1) {
        if (dirX != lbl_1_rodata_250) {
            dirY = lbl_1_rodata_250;
            if (dirX < lbl_1_rodata_250) {
                refX = rect.x1;
            } else {
                refX = rect.x0;
            }
            refY = (f32)lbl_1_bss_348[cur].y;
        } else {
            dirX = lbl_1_rodata_250;
            if (dirY < lbl_1_rodata_250) {
                refY = rect.y1;
            } else {
                refY = rect.y0;
            }
            refX = (f32)lbl_1_bss_348[cur].x;
        }
        newIdx = fn_1_ABF8(cur, refX, refY, dirX, dirY);
    }
    if (newIdx == -1) {
        goto L_99B4;
    }
    HuAudFXPlay(0);
    cur = newIdx;
    code1 = lbl_1_bss_348[cur].flag;
    HuSprBankSet(lbl_1_bss_42E, 3, 8);
    HuSprBankSet(lbl_1_bss_42E, 4, 8);
    HuSprBankSet(lbl_1_bss_42E, 5, 8);
    HuSprBankSet(lbl_1_bss_42E, 6, 2);
    HuSprBankSet(lbl_1_bss_42E, 7, 4);
    if (code1 >= 0x100 && code1 <= 0x500) {
        flag26 = 1;
        switch (code1) {
        case 0x100:
            HuSprBankSet(lbl_1_bss_42E, 3, 9);
            break;
        case 0x200:
            HuSprBankSet(lbl_1_bss_42E, 4, 9);
            break;
        case 0x300:
            HuSprBankSet(lbl_1_bss_42E, 5, 9);
            break;
        case 0x400:
            HuSprBankSet(lbl_1_bss_42E, 6, 3);
            break;
        case 0x500:
            HuSprBankSet(lbl_1_bss_42E, 7, 5);
            break;
        }
    } else {
        flag26 = 0;
    }

L_99B4:
    if (HuPadBtnDown[0] & 0x1000) {
        for (cur = 0; lbl_1_bss_348[cur].flag != 0 && lbl_1_bss_348[cur].flag != 0x500; cur++) {
        }
        HuSprBankSet(lbl_1_bss_42E, 3, 8);
        HuSprBankSet(lbl_1_bss_42E, 4, 8);
        HuSprBankSet(lbl_1_bss_42E, 5, 8);
        HuSprBankSet(lbl_1_bss_42E, 6, 2);
        HuSprBankSet(lbl_1_bss_42E, 7, 5);
        flag26 = 1;
        j = fn_1_AAEC(winId);
        if (j == 0) {
            goto L_A3D8;
        }
        lbl_1_bss_37E = j;
        count = j;
        for (j = k = 0; j < count; k++, j++) {
            if (lbl_1_bss_34C[j + 1] == 0x80 || lbl_1_bss_34C[j + 1] == 0x81) {
                lbl_1_bss_380[k] = (u16)((lbl_1_bss_34C[j] << 8) | lbl_1_bss_34C[j + 1]);
                lbl_1_bss_37E--;
                j++;
            } else {
                lbl_1_bss_380[k] = lbl_1_bss_34C[j];
            }
        }
        goto L_A3D8;
    }
    if (HuPadBtnDown[0] & 0x100) {
        if (lbl_1_bss_348[cur].flag >= 0x100 && lbl_1_bss_348[cur].flag <= 0x500) {
            goto L_E84;
        }
        if (lbl_1_bss_37E >= 8) {
            goto L_E84;
        }
        lbl_1_bss_380[lbl_1_bss_37E] = lbl_1_bss_348[cur].flag;
        lbl_1_bss_37E++;
        for (j = k = 0; j < lbl_1_bss_37E; j++) {
            if (lbl_1_bss_380[j] > 0xff) {
                lbl_1_bss_34C[k++] = (u8)(lbl_1_bss_380[j] >> 8);
                lbl_1_bss_34C[k++] = lbl_1_bss_380[j] & 0xff;
            } else {
                lbl_1_bss_34C[k++] = lbl_1_bss_380[j] & 0xff;
            }
        }
        if (lbl_1_bss_37E < 8) {
            lbl_1_bss_34C[k] = 0xbf;
            lbl_1_bss_34C[k + 1] = 0;
        } else {
            lbl_1_bss_34C[k] = 0;
        }
        HuWinMesSet(winId, (u32)lbl_1_bss_34C);
        if (lbl_1_bss_37E < 8) {
            goto L_A088;
        }
        for (cur = 0; lbl_1_bss_348[cur].flag != 0 && lbl_1_bss_348[cur].flag != 0x500; cur++) {
        }
        HuSprBankSet(lbl_1_bss_42E, 7, 5);
        flag26 = 1;
        goto L_A088;
    L_E84:
        if (lbl_1_bss_348[cur].flag == 0x100) {
            sel = 0;
            goto L_A088;
        }
        if (lbl_1_bss_348[cur].flag == 0x200) {
            sel = 1;
            goto L_A088;
        }
        if (lbl_1_bss_348[cur].flag == 0x300) {
            sel = 2;
            goto L_A088;
        }
        if (lbl_1_bss_348[cur].flag == 0x400) {
            goto L_A0B0;
        }
        if (lbl_1_bss_348[cur].flag != 0x500) {
            goto L_A088;
        }
        for (j = 0; lbl_1_bss_34C[j] != 0; j++) {
            if (lbl_1_bss_34C[j] == 0xbf) {
                lbl_1_bss_34C[j] = 0;
            }
        }
        HuAudFXPlay(2);
        HuSprBankSet(lbl_1_bss_42E, 0xd, 3);
        HuPrcSleep(0xa);
        HuSprBankSet(lbl_1_bss_42E, 0xd, 2);
        if (fn_1_AAEC(winId) != 0) {
            HuPrcSleep(0x1e);
        }
        for (j = 0;; j++) {
            *((s8 *)&GwCommon.name[j]) = lbl_1_bss_D8[no].unk_44[j] = lbl_1_bss_34C[j];
            if (lbl_1_bss_34C[j] == 0) {
                break;
            }
        }
        ret = 0;
        goto L_AA78;
    L_A088:
        HuAudFXPlay(1);
        goto L_A3D8;
    }
    if (HuPadBtnDown[0] & 0x200) {
    L_A0B0:
        lbl_1_bss_37E--;
        if (lbl_1_bss_37E < 0) {
            ret = -0x4d2;
            goto L_AA78;
        }
        HuAudFXPlay(3);
        for (j = k = 0; j < lbl_1_bss_37E; j++) {
            if (lbl_1_bss_380[j] > 0xff) {
                lbl_1_bss_34C[k++] = (u8)(lbl_1_bss_380[j] >> 8);
                lbl_1_bss_34C[k++] = lbl_1_bss_380[j] & 0xff;
            } else {
                lbl_1_bss_34C[k++] = lbl_1_bss_380[j] & 0xff;
            }
        }
        lbl_1_bss_34C[k] = 0xbf;
        lbl_1_bss_34C[k + 1] = 0;
        HuWinMesSet(winId, (u32)lbl_1_bss_34C);
        HuSprBankSet(lbl_1_bss_42E, 0xc, 1);
        HuPrcSleep(5);
        HuSprBankSet(lbl_1_bss_42E, 0xc, 0);
        goto L_A3D8;
    }
    if (HuPadBtnDown[0] & 0x20) {
        sel++;
        if (sel >= lbl_1_bss_42C) {
            sel = 0;
        }
        for (cur = 0; lbl_1_bss_348[cur].flag != 0; cur++) {
            if (lbl_1_bss_348[cur].flag == (sel + 1) * 0x100) {
                break;
            }
        }
        flag26 = 0;
        HuSprBankSet(lbl_1_bss_42E, 0xb, 3);
        goto L_A3D8;
    }
    if (HuPadBtnDown[0] & 0x40) {
        sel--;
        if (sel < 0) {
            sel = lbl_1_bss_42C - 1;
        }
        for (cur = 0; lbl_1_bss_348[cur].flag != 0; cur++) {
            if (lbl_1_bss_348[cur].flag == (sel + 1) * 0x100) {
                break;
            }
        }
        flag26 = 0;
        HuSprBankSet(lbl_1_bss_42E, 0xa, 1);
    }

L_A3D8:
    if (sel != lastSel) {
        HuAudFXPlay(0x48a);
        HuSprBankSet(lbl_1_bss_42E, 3, 8);
        HuSprBankSet(lbl_1_bss_42E, 4, 8);
        HuSprBankSet(lbl_1_bss_42E, 5, 8);
        HuSprBankSet(lbl_1_bss_42E, 6, 2);
        HuSprBankSet(lbl_1_bss_42E, 7, 4);
        if (sel == 0) {
            HuSprBankSet(lbl_1_bss_42E, 0xe, 5);
            HuSprBankSet(lbl_1_bss_42E, 3, 9);
        } else if (sel == 1) {
            HuSprBankSet(lbl_1_bss_42E, 0xf, 7);
            HuSprBankSet(lbl_1_bss_42E, 4, 9);
        } else {
            HuSprBankSet(lbl_1_bss_42E, 0x10, 9);
            HuSprBankSet(lbl_1_bss_42E, 5, 9);
        }
        k = lbl_1_bss_348[cur].flag;
        HuWinDispOff(lbl_1_bss_3AC[lastSel]);
        lbl_1_bss_348 = lbl_1_data_47C[sel];
        fn_1_B234(&rect);
        HuWinDispOn(lbl_1_bss_3AC[sel]);
        for (cur = 0; lbl_1_bss_348[cur].flag != 0; cur++) {
            if (k == lbl_1_bss_348[cur].flag) {
                break;
            }
        }
        code2 = lbl_1_bss_348[cur].flag;
        HuSprPosSet(lbl_1_bss_42E, 0, lbl_1_rodata_268 + ((f32)lbl_1_bss_348[cur].x - lbl_1_rodata_240),
                    (f32)lbl_1_bss_348[cur].y - lbl_1_rodata_244 - lbl_1_rodata_26C);
        if (code2 >= 0x100 && code2 <= 0x500) {
            HuSprAttrSet(lbl_1_bss_42E, 1, 4);
        } else {
            HuSprAttrReset(lbl_1_bss_42E, 1, 4);
            HuSprPosSet(lbl_1_bss_42E, 1, lbl_1_rodata_270 + ((f32)lbl_1_bss_348[cur].x - lbl_1_rodata_240),
                        lbl_1_rodata_26C + ((f32)lbl_1_bss_348[cur].y - lbl_1_rodata_244));
        }
        lastSel = sel;
        HuPrcSleep(0xa);
        HuSprBankSet(lbl_1_bss_42E, 0xa, 0);
        HuSprBankSet(lbl_1_bss_42E, 0xb, 2);
        HuSprBankSet(lbl_1_bss_42E, 0xe, 4);
        HuSprBankSet(lbl_1_bss_42E, 0xf, 6);
    }

L_A868:
    code3 = lbl_1_bss_348[cur].flag;
    HuSprPosSet(lbl_1_bss_42E, 0, lbl_1_rodata_268 + ((f32)lbl_1_bss_348[cur].x - lbl_1_rodata_240),
                (f32)lbl_1_bss_348[cur].y - lbl_1_rodata_244 - lbl_1_rodata_26C);
    if (code3 >= 0x100 && code3 <= 0x500) {
        HuSprAttrSet(lbl_1_bss_42E, 1, 4);
    } else {
        HuSprAttrReset(lbl_1_bss_42E, 1, 4);
        HuSprPosSet(lbl_1_bss_42E, 1, lbl_1_rodata_270 + ((f32)lbl_1_bss_348[cur].x - lbl_1_rodata_240),
                    lbl_1_rodata_26C + ((f32)lbl_1_bss_348[cur].y - lbl_1_rodata_244));
    }
    HuPrcVSleep();
    goto L_95A8;

L_AA78:
    HuWinHomeClear(lbl_1_bss_2DE);
    HuWinHomeClear(lbl_1_bss_2E0);
    HuWinKill(winId);
    HuAudFXPlay(0);
    fn_1_B5B4();
    HuPrcVSleep();
    return ret;
}

/* 0xAAEC */
s16 fn_1_AAEC(s16 winId)
{
    s16 count;
    s16 i;

    for (i = count = 0; lbl_1_bss_34C[i] != 0; i++) {
        if (lbl_1_bss_34C[i] != 0 && lbl_1_bss_34C[i] != 0x10 && lbl_1_bss_34C[i] != 0x20) {
            count++;
        }
    }
    if (count == 0) {
        strcpy(lbl_1_bss_34C, (char *)lbl_1_bss_3B2[frandmod(7)]);
        HuWinMesSet(winId, (u32)lbl_1_bss_34C);
        return (s16)strlen(lbl_1_bss_34C);
    }
    return 0;
}

/* 0xABF8 */
s16 fn_1_ABF8(s16 id, f32 refX, f32 refY, f32 dirX, f32 dirY)
{
    f32 ndx, ndy;
    f32 len;
    f32 px, py;
    f32 dist;
    f32 angDist;
    f32 best;
    s16 i;
    s16 bestIdx;

    len = sqrtf(dirX * dirX + dirY * dirY);
    ndx = dirX / len;
    ndy = dirY / len;
    best = 409600.0f;
    bestIdx = -1;
    for (i = 0; lbl_1_bss_348[i].flag != 0; i++) {
        if (i != id) {
            px = (f32)lbl_1_bss_348[i].x - refX;
            py = (f32)lbl_1_bss_348[i].y - refY;
            dist = sqrtf(px * px + py * py);
            px = px / dist;
            py = py / dist;
            angDist = sqrtf((px - ndx) * (px - ndx) + (py - ndy) * (py - ndy));
            if (lbl_1_bss_348[id].flag >= 0x100 && lbl_1_bss_348[id].flag <= 0x500 &&
                lbl_1_bss_348[i].flag >= 0x100 && lbl_1_bss_348[i].flag <= 0x500 &&
                angDist < 0.1 && dist < 55.0f + best) {
                best = dist;
                bestIdx = i;
            } else if (angDist < 0.8 && dist < best) {
                best = dist;
                bestIdx = i;
            }
        }
    }
    return bestIdx;
}

/* 0xB234 */
void fn_1_B234(B234Rect *r)
{
    s16 i;

    r->x0 = r->y0 = 10000.0f;
    r->x1 = r->y1 = -10000.0f;
    for (i = 0; lbl_1_bss_348[i].flag != 0; i++) {
        if ((f32)lbl_1_bss_348[i].x < r->x0) {
            r->x0 = (f32)lbl_1_bss_348[i].x;
        }
        if ((f32)lbl_1_bss_348[i].y < r->y0) {
            r->y0 = (f32)lbl_1_bss_348[i].y;
        }
        if ((f32)lbl_1_bss_348[i].x > r->x1) {
            r->x1 = (f32)lbl_1_bss_348[i].x;
        }
        if ((f32)lbl_1_bss_348[i].y > r->y1) {
            r->y1 = (f32)lbl_1_bss_348[i].y;
        }
    }
    r->x0 -= 20.0f;
    r->y0 -= 20.0f;
    r->x1 += 20.0f;
    r->y1 += 20.0f;
}

/* 0xB530 */
void fn_1_B530(void)
{
    s16 i;

    for (i = 0; i < 18; i++) {
        HuSprAttrReset(lbl_1_bss_42E, i, 4);
        HuSprTPLvlSet(lbl_1_bss_42E, i, lbl_1_rodata_280);
    }
    HuWinDispOn(lbl_1_bss_3AC[0]);
}

/* 0xB5B4 */
void fn_1_B5B4(void)
{
    s16 i;

    for (i = 0; i < 18; i++) {
        HuSprAttrSet(lbl_1_bss_42E, i, 4);
    }
    for (i = 0; i < 3; i++) {
        HuWinDispOff(lbl_1_bss_3AC[i]);
    }
}

/* 0xB63C  (99.80 documented ceiling — residual is 23 pure scratch-color picks in 6 windows).
 * Do NOT change: the sprintf function-pointer cast (void, non-variadic: kills crclr, target
 * has none); the (result = ...) fmt routing (target's addi r29 / mr r4,r29 fixup); the 3 typed
 * cast-calls on SLSaveDataMake/fn_1_36C4 (discarded-result ghost vregs, call-site-local so
 * fn_1_5244 stays 100.00); decl order: OSTime time first, brokenFlag = 0 initializer, u16 *p. */
s32 fn_1_B63C(s16 arg)
{
    OSTime time;
    char buf[8];
    s32 boxStatus[3];
    s16 winId;
    s32 result;
    s16 i;
    s32 brokenFlag = 0;
    u16 *p;

    UnMountCnt = 0;
    if (arg == -1) {
        winId = HuWinWarningCreate(-10000.0f, 160.0f, 478, 94);
    } else {
        winId = arg;
    }
    SLWinIdSet(winId);
    SLCurSlotNoSet(0);
L_B6B8:
    result = fn_1_CDA8();
    if (result == -4) {
        for (i = 0; i < 3; i++) {
            lbl_1_bss_D8[i].unk_0 = 0;
        }
        result = fn_1_BF9C();
        if (result != 0) goto L_B718;
        goto L_BF34;
    } else if (result == 0) {
        goto L_B79C;
    }
L_B718:
    result = fn_1_C2F0(result, winId);
    if (result == -3) goto L_B6B8;
    if (result == -4) goto L_B6B8;
    if (result == -5) goto L_B6B8;
    if (result == -0x4d2) {
        HuWinWarningOpen(winId);
        HuWinAttrSet(winId, 0x10);
        HuWinMesSet(winId, 0x90025);
        HuWinMesWait(winId);
        result = HuWinChoiceGet(winId, 0);
        HuWinWarningClose(winId);
        if (result != 0) goto L_B6B8;
        result = -2;
    }
    goto L_BF5C;
L_B79C:
    if (strncmp((char *)saveBuf[curSlotNo], lbl_1_data_56C, 5) == 0) {
        time = OSGetTime();
        ((s32 (*)(s32, OSTime *))SLSaveDataMake)(1, &time);
        for (i = 0; i < 3; i++) {
            SLSaveEmptySet(curSlotNo, i);
            lbl_1_bss_D8[i].unk_0 = 0;
        }
        goto L_BF34;
    }
    brokenFlag = 0;
    for (i = 0; i < 3; i++) {
        boxStatus[i] = 0;
    }
    p = (u16 *)(saveBuf[curSlotNo] + 0x2040);
    if (*p != SLCheckSumGet(0, 0x2040)) {
        OSReport("IconBanner Area Broken!\n");
        time = OSGetTime();
        ((s32 (*)(s32, OSTime *))SLSaveDataMake)(0, &time);
        brokenFlag = 1;
    }
    for (i = 0; i < 3; i++) {
        /* void-view cast: keep the TU-wide `s32 SLCurBoxNoSet` (required by fn_1_17CC) but
         * confine that flip away from B63C — the target has no discarded-result ghost vreg
         * here, so the void view restores B63C's coloring (99.35 -> ~99.80). Same call-site
         * cast technique B63C uses for SLSaveDataMake/fn_1_36C4. */
        ((void (*)(s32))SLCurBoxNoSet)(i);
        if (strncmp((char *)(saveBuf[curSlotNo] + SLBoxDataOffsetGet(i)), lbl_1_data_58B, 4) == 0 ||
            strncmp((char *)(saveBuf[curSlotNo] + SLBoxDataOffsetGet(i + 3)), lbl_1_data_58B, 4) == 0) {
            if (SLCheckSumCheck() == 0) {
                OSReport("Box%d Broken!\n", i);
                SLBoxBackupLoad(i);
                if (SLCheckSumCheck() == 0) {
                    SLSaveEmptySet(curSlotNo, i);
                    boxStatus[i] = 2;
                } else {
                    boxStatus[i] = 1;
                }
            }
        } else if (strncmp((char *)(saveBuf[curSlotNo] + SLBoxDataOffsetGet(i)), lbl_1_data_59F, 4) != 0 &&
                   strncmp((char *)(saveBuf[curSlotNo] + SLBoxDataOffsetGet(i + 3)), lbl_1_data_59F, 4) != 0) {
            boxStatus[i] = 3;
            SLSaveEmptySet(curSlotNo, i);
        } else if (strncmp((char *)(saveBuf[curSlotNo] + SLBoxDataOffsetGet(i)), lbl_1_data_59F, 4) == 0 ||
                   strncmp((char *)(saveBuf[curSlotNo] + SLBoxDataOffsetGet(i + 3)), lbl_1_data_59F, 4) == 0) {
            SLSaveEmptySet(curSlotNo, i);
        }
        SLCommonLoad();
        if (strncmp((char *)&GwCommon, lbl_1_data_58B, 4) == 0) {
            lbl_1_bss_D8[i].unk_0 = 1;
            OSTicksToCalendarTime(GwCommon.time, &lbl_1_bss_D8[i].unk_1C);
            memcpy(lbl_1_bss_D8[i].unk_44, GwCommon.name, 0x11);
            lbl_1_bss_D8[i].unk_16 = GWBankStarGet();
            lbl_1_bss_D8[i].unk_14 = GwCommon.lastBoard;
        } else {
            lbl_1_bss_D8[i].unk_0 = 0;
        }
        GwCommon.languageNo = GwLanguage;
    }
    if (brokenFlag == 0 && boxStatus[0] == 0 && boxStatus[1] == 0 && boxStatus[2] == 0) {
        goto L_BF34;
    }
    HuWinWarningOpen(winId);
    if (boxStatus[0] != 2 && boxStatus[0] != 3 && boxStatus[1] != 2 && boxStatus[1] != 3 &&
        boxStatus[2] != 2 && boxStatus[2] != 3) {
        HuWinInsertMesSet(winId, curSlotNo + 0x90036, 0);
        HuWinMesSet(winId, 0x90034);
        HuWinMesWait(winId);
    } else {
        for (i = 0; i < 3; i++) {
            ((void (*)(char *, char *, s32))sprintf)(buf, (char *)(result = (s32)lbl_1_data_5A4), i + 1);
            if (boxStatus[i] == 1) {
                HuWinInsertMesSet(winId, (u32)buf, 0);
                HuWinMesSet(winId, 0x90024);
                HuWinMesWait(winId);
            } else if (boxStatus[i] == 2) {
                HuWinInsertMesSet(winId, (u32)buf, 0);
                HuWinMesSet(winId, 0x90023);
                HuWinMesWait(winId);
            } else if (boxStatus[i] == 3) {
                HuWinInsertMesSet(winId, curSlotNo + 0x90036, 0);
                HuWinInsertMesSet(winId, (u32)buf, 1);
                HuWinMesSet(winId, 0x90039);
                HuWinMesWait(winId);
            }
        }
    }
    HuWinWarningClose(winId);
    UnMountCnt = 0;
    result = fn_1_D90C(-1, 0x9002A);
    if (UnMountCnt != 0) {
        result = -4;
        UnMountCnt = 0;
    }
    if (result == -0x4d2) {
        HuWinWarningOpen(winId);
        HuWinAttrSet(winId, 0x10);
        HuWinMesSet(winId, 0x90025);
        HuWinMesWait(winId);
        result = HuWinChoiceGet(winId, 0);
        HuWinWarningClose(winId);
        if (result != 0) goto L_B6B8;
        result = -2;
        goto L_BF5C;
    }
    if (result == -1) goto L_BF5C;
    if (result == -4) goto L_B6B8;
    if (result == -5) goto L_B6B8;
L_BF34:
    result = 0;
    for (i = 0; i < 3; i++) {
        ((s32 (*)(s16, s16))fn_1_36C4)(i, i);
    }
L_BF5C:
    if (arg == -1) {
        HuWinWarningClose(winId);
        SLWinIdSet(-1);
        HuWinWarningKill(winId);
    }
    return result;
}

/* 0xBF9C */
s32 fn_1_BF9C(void)
{
    s32 result;
    u32 byteNotUsed;
    u32 filesNotUsed;

    result = fn_1_CAA4(curSlotNo);
    if (result < 0) {
        return result;
    }
    result = HuCardSectorSizeGet(curSlotNo);
    if (result < 0 && result != 0x2000) {
        fn_1_DD84(8);
        return -0x80;
    }
    result = HuCardFreeSpaceGet(curSlotNo, &byteNotUsed, &filesNotUsed);
    if (filesNotUsed == 0 && byteNotUsed < 0xa000) {
        fn_1_DD84(4);
        return -9;
    }
    if (filesNotUsed == 0) {
        fn_1_DD84(2);
        return -9;
    }
    if (byteNotUsed < 0xa000) {
        fn_1_DD84(3);
        return -9;
    }
    return 0;
}

/* 0xC098 */
s32 fn_1_C098(s16 winId)
{
    s32 warnId;
    s32 ret;
    s32 code;
    s32 status;
    u32 filesNotUsed;
    u32 byteNotUsed;

    if (winId == -1) {
        warnId = HuWinWarningCreate(lbl_1_rodata_2C0, lbl_1_rodata_2C4, 478, 94);
    } else {
        warnId = winId;
    }
    SLWinIdSet(warnId);
    ret = HuCardMount(curSlotNo);
    if (ret == 0 && (ret = HuCardOpen(curSlotNo, SLSaveFileName, &curFileInfo)) == 0) {
        ret = 0;
    } else {
        status = fn_1_CAA4(curSlotNo);
        if (status < 0) {
            code = status;
        } else {
            status = HuCardSectorSizeGet(curSlotNo);
            if (status < 0 && status != 0x2000) {
                fn_1_DD84(8);
                code = -0x80;
            } else {
                status = HuCardFreeSpaceGet(curSlotNo, &byteNotUsed, &filesNotUsed);
                if (filesNotUsed == 0 && byteNotUsed < 0xA000) {
                    fn_1_DD84(4);
                    code = -9;
                } else if (filesNotUsed == 0) {
                    fn_1_DD84(2);
                    code = -9;
                } else if (byteNotUsed < 0xA000) {
                    fn_1_DD84(3);
                    code = -9;
                } else {
                    code = 0;
                }
            }
        }
        ret = code;
        if (ret == 0) {
            ret = 0;
        } else {
            ret = fn_1_C2F0(ret, warnId);
        }
    }
    if (winId == -1) {
        SLWinIdSet(-1);
        HuWinWarningKill(warnId);
    }
    return ret;
}

/* 0xC278 */
s32 fn_1_C278(void)
{
    s32 ret;

    if ((ret = HuCardMount(curSlotNo)) == 0 &&
        (ret = HuCardOpen(curSlotNo, SLSaveFileName, &curFileInfo)) == 0) {
        return 1;
    }
    return 0;
}

/* 0xC2F0 */
s32 fn_1_C2F0(s32 err, s16 winId)
{
    s32 result;

    if (UnMountCnt != 0) {
        HuWinWarningOpen(winId);
        HuWinInsertMesSet(winId, curSlotNo + 0x90036, 0);
        HuWinMesSet(winId, 0x90026);
        HuWinMesWait(winId);
        HuWinWarningClose(winId);
        UnMountCnt = 0;
        return -4;
    }
    UnMountCnt = 0;
    if (err == -6) {
        for (;;) {
            result = fn_1_E210(0x17, winId);
            if (UnMountCnt != 0 && result == 2) {
                HuWinWarningOpen(winId);
                HuWinInsertMesSet(winId, curSlotNo + 0x90036, 0);
                HuWinMesSet(winId, 0x90026);
                HuWinMesWait(winId);
                HuWinWarningClose(winId);
                UnMountCnt = 0;
                return -4;
            }
            if (result == -0x4d2) {
                HuWinWarningClose(winId);
                UnMountCnt = 0;
                return -0x4d2;
            }
            if (result == 2) {
                HuWinWarningOpen(winId);
                HuWinInsertMesSet(winId, curSlotNo + 0x90036, 0);
                HuWinMesSet(winId, 0x9001A);
                HuWinMesWait(winId);
                result = HuWinChoiceGet(winId, 1);
                if (result == 0) {
                    result = fn_1_CBC8(curSlotNo);
                    HuWinWarningClose(winId);
                    return -3;
                }
                if (UnMountCnt != 0) {
                    HuWinInsertMesSet(winId, curSlotNo + 0x90036, 0);
                    HuWinMesSet(winId, 0x90026);
                    HuWinMesWait(winId);
                    HuWinWarningClose(winId);
                    UnMountCnt = 0;
                    return -4;
                }
                fn_1_DD84(5);
                continue;
            }
            if (result == 1) {
                HuWinWarningClose(winId);
                UnMountCnt = 0;
                return -1;
            }
            HuWinWarningOpen(winId);
            HuWinInsertMesSet(winId, curSlotNo + 0x90036, 0);
            HuWinMesSet(winId, 0x9001F);
            HuWinMesWait(winId);
            while ((HuPadBtnDown[0] & 0x100) == 0) {
                HuPrcVSleep();
            }
            HuAudFXPlay(1);
            HuWinWarningClose(winId);
            UnMountCnt = 0;
            return -5;
        }
    }
    if (err == -9) {
        for (;;) {
            result = fn_1_E210(0x1d, winId);
            if (result == -0x4d2) {
                HuWinWarningClose(winId);
                UnMountCnt = 0;
                return -0x4d2;
            }
            if (result == 1) {
                HuWinWarningClose(winId);
                UnMountCnt = 0;
                return -1;
            }
            if (result == 8) {
                HuWinWarningOpen(winId);
                HuWinMesSet(winId, 0x90020);
                HuWinMesWait(winId);
                result = HuWinChoiceGet(winId, 1);
                if (result != 0) {
                    UnMountCnt = 0;
                    continue;
                }
                WipeCreate(2, 0, 0x3c);
                HuAudSStreamAllFadeOut(0x3e8);
                while (WipeCheck()) {
                    HuPrcVSleep();
                }
                HuSRDisableF = 0;
                SR_ExecResetMenu = 1;
                for (;;) {
                    HuPrcVSleep();
                }
            }
            HuWinWarningOpen(winId);
            HuWinInsertMesSet(winId, curSlotNo + 0x90036, 0);
            HuWinMesSet(winId, 0x9001F);
            HuWinMesWait(winId);
            while ((HuPadBtnDown[0] & 0x100) == 0) {
                HuPrcVSleep();
            }
            HuAudFXPlay(1);
            HuWinWarningClose(winId);
            UnMountCnt = 0;
            return -5;
        }
    }
    result = fn_1_E210(0x15, winId);
    if (result == -0x4d2) {
        HuWinWarningClose(winId);
        UnMountCnt = 0;
        return -0x4d2;
    }
    if (result == 1) {
        HuWinWarningClose(winId);
        UnMountCnt = 0;
        return -1;
    }
    HuWinWarningOpen(winId);
    HuWinInsertMesSet(winId, curSlotNo + 0x90036, 0);
    HuWinMesSet(winId, 0x9001F);
    HuWinMesWait(winId);
    while ((HuPadBtnDown[0] & 0x100) == 0) {
        HuPrcVSleep();
    }
    HuAudFXPlay(1);
    HuWinWarningClose(winId);
    UnMountCnt = 0;
    return -5;
}

/* 0xC898 */
s32 fn_1_C898(const char *fileName)
{
    s32 ret;

    if (SLSaveFlagGet() == 0) {
        return 0;
    }
    ret = fn_1_CAA4(curSlotNo);
    if (ret < 0) {
        return ret;
    }
    ret = HuCardOpen(curSlotNo, fileName, &curFileInfo);
    if (ret == -4) {
        return -4;
    }
    if (ret == -2) {
        fn_1_DD84(7);
        return -0x80;
    }
    if (ret == -0x80) {
        fn_1_DD84(1);
        return -0x80;
    }
    if (ret == -3) {
        fn_1_DD84(0);
        return -3;
    }
    if (ret == -6) {
        ret = HuCardSectorSizeGet(curSlotNo);
        if (ret > 0 && ret != 0x2000) {
            fn_1_DD84(8);
            return -2;
        }
        UnMountCnt = 0;
        fn_1_DD84(5);
        return -6;
    }
    return 0;
}

/* 0xC9D0 */
s32 fn_1_C9D0(s32 length, void *addr)
{
    s32 ret;

    if (SLSaveFlagGet() == 0) {
        return 0;
    }
    SLSerialNoGet();
    ret = HuCardRead(&curFileInfo, addr, length, 0);
    if (ret == -3) {
        fn_1_DD84(0);
    } else if (ret < 0) {
        fn_1_DD84(1);
    }
    return ret;
}

/* 0xCA58 */
s32 fn_1_CA58(void)
{
    s32 ret;

    if (SLSaveFlagGet() == 0) {
        return 0;
    }
    ret = HuCardClose(&curFileInfo);
    return ret;
}

/* 0xCAA4 (unused s16 slot param per resolution 2; body uses global curSlotNo) */
/* dont_inline: target keeps `bl fn_1_CAA4` in all callers (CFF4/CDA8/D558 are
   defined after this point and -inline auto would otherwise inline it). */
#pragma dont_inline on
s32 fn_1_CAA4(s16 slot)
{
    s32 ret;

    ret = HuCardMount(curSlotNo);
    if (ret == -2) {
        fn_1_DD84(7);
        return ret;
    }
    if (ret == -0x80) {
        fn_1_DD84(1);
        return -0x80;
    }
    if (ret == -3) {
        fn_1_DD84(0);
        return -3;
    }
    if (ret == -6) {
        ret = HuCardSectorSizeGet(curSlotNo);
        if (ret > 0 && ret != 0x2000) {
            fn_1_DD84(8);
            return -2;
        }
        UnMountCnt = 0;
        fn_1_DD84(5);
        return -6;
    }
    ret = HuCardSectorSizeGet(curSlotNo);
    if (ret < 0) {
        fn_1_DD84(1);
        return ret;
    }
    if (ret != 0x2000) {
        fn_1_DD84(8);
        return -2;
    }
    return 0;
}
#pragma dont_inline off

/* 0xCBC8 */
s32 fn_1_CBC8(s16 arg)
{
    s16 ret;
    s16 win;
    OSTime time;

    if (UnMountCnt & (1 << curSlotNo)) {
        fn_1_DD84(0xc);
        UnMountCnt = 0;
        return 0;
    }
    win = fn_1_DBA0(0x9001b, arg + 0x90036, -1, 0x46);
    HuPrcSleep(0x1e);
    if (UnMountCnt & (1 << curSlotNo)) {
        fn_1_DD24(win);
        fn_1_DD84(0xc);
        UnMountCnt = 0;
        return 0;
    }
    ret = HuCardFormat(curSlotNo);
    SLSerialNo[curSlotNo] = 0;
    if (ret < 0) {
        fn_1_DD24(win);
    }
    if (ret == -128) {
        fn_1_DD84(6);
        fn_1_DD84(1);
        return -128;
    }
    if (ret == -3) {
        fn_1_DD84(0);
        return -3;
    }
    if (ret == -2) {
        fn_1_DD84(7);
        return ret;
    }
    SLSerialNoGet();
    fn_1_DD24(win);
    SLCurBoxNoSet(0);
    time = OSGetTime();
    SLSaveDataMake(0, &time);
    SLCheckSumBoxAllSet();
    return ret;
}

/* 0xCDA8 */
s32 fn_1_CDA8(void)
{
    s32 rc;
    s32 err;
    s32 ret;
    s32 rd;
    u16 *p;
    s32 status;
    u16 sum;
    u8 *buf;

    if (SLSaveFlagGet() == 0) {
        err = 0;
    } else {
        rc = fn_1_CAA4(curSlotNo);
        if (rc < 0) {
            err = rc;
        } else {
            rc = HuCardOpen(curSlotNo, SLSaveFileName, &curFileInfo);
            if (rc == -4) {
                err = -4;
            } else if (rc == -2) {
                fn_1_DD84(7);
                err = -0x80;
            } else if (rc == -0x80) {
                fn_1_DD84(1);
                err = -0x80;
            } else if (rc == -3) {
                fn_1_DD84(0);
                err = -3;
            } else if (rc == -6) {
                rc = HuCardSectorSizeGet(curSlotNo);
                if (rc > 0 && rc != 0x2000) {
                    fn_1_DD84(8);
                    err = -2;
                } else {
                    UnMountCnt = 0;
                    fn_1_DD84(5);
                    err = -6;
                }
            } else {
                err = 0;
            }
        }
    }
    ret = err;
    if (ret >= 0) {
        buf = saveBuf[curSlotNo];
        if (SLSaveFlagGet() == 0) {
            status = 0;
        } else {
            SLSerialNoGet();
            rd = HuCardRead(&curFileInfo, buf, 0xa000, 0);
            if (rd == -3) {
                fn_1_DD84(0);
            } else if (rd < 0) {
                fn_1_DD84(1);
            }
            status = rd;
        }
        ret = status;
        if (SLSaveFlagGet() != 0) {
            s32 closeRet = HuCardClose(&curFileInfo);
        }
        if (ret >= 0) {
            p = (u16 *)&saveBuf[curSlotNo][0xcb2];
            sum = SLCheckSumGet(0, 0xcb2);
            *p == sum;
        }
    }
    return ret;
}

/* 0xCFF4 */
s32 fn_1_CFF4(const char *fileName, s32 size, void *addr)
{
    s32 warnA;
    s32 warnB;
    s32 ret;
    void *buf;
    u32 byteNotUsed;
    u32 filesNotUsed;

    if (SLSaveFlagGet() == 0) {
        return 0;
    }
    SLCheckSumBoxAllSet();
    SLSaveBackup();
    ret = fn_1_CAA4(curSlotNo);
    if (ret < 0) {
        return ret;
    }
    ret = HuCardSectorSizeGet(curSlotNo);
    if (ret < 0 && ret != 0x2000) {
        fn_1_DD84(8);
        return -0x80;
    }
    ret = HuCardFreeSpaceGet(curSlotNo, &byteNotUsed, &filesNotUsed);
    if (filesNotUsed == 0 && size > byteNotUsed) {
        fn_1_DD84(4);
        return -9;
    }
    if (filesNotUsed == 0) {
        fn_1_DD84(2);
        return -9;
    }
    if (size > byteNotUsed) {
        fn_1_DD84(3);
        return -9;
    }
    warnA = fn_1_DBA0(0x90007, curSlotNo + 0x90036, -1, 0xa0);
    warnB = fn_1_DA18(0x9002a, curSlotNo + 0x90036, -1, 0x46);
    HuSRDisableF = 1;
    ret = HuCardCreate(curSlotNo, fileName, size, &curFileInfo);
    if (ret < 0) {
        fn_1_DD24(warnA);
        fn_1_DD24(warnB);
        HuSRDisableF = 0;
    }
    if (ret == -3) {
        fn_1_DD84(0);
        return ret;
    }
    if (ret == -6) {
        fn_1_DD84(5);
        return ret;
    }
    if (ret < 0) {
        fn_1_DD84(1);
        return ret;
    }
    SLSerialNoGet();
    buf = HuMemDirectMalloc(0, size);
    memset(buf, size, 0);
    memcpy(buf, SLEraseStr, 6);
    ret = HuCardWriteIdle(&curFileInfo, buf, size, 0);
    if (ret == 0) {
        ret = HuCardWriteIdle(&curFileInfo, addr, size, 0);
    }
    HuMemDirectFree(buf);
    if (ret < 0) {
        fn_1_DD24(warnA);
        fn_1_DD24(warnB);
        HuSRDisableF = 0;
    }
    if (ret == -3) {
        fn_1_DD84(0);
        return ret;
    }
    if (ret == -6) {
        fn_1_DD84(5);
        return ret;
    }
    if (ret < 0) {
        fn_1_DD84(1);
        return ret;
    }
    ret = SLStatSet(0);
    HuSRDisableF = 0;
    fn_1_DD24(warnA);
    fn_1_DD24(warnB);
    if (ret < 0) {
        return ret;
    }
    return 0;
}

/* 0xD348 (saveBuf[curSlotNo] per resolution 5) */
s32 fn_1_D348(s16 arg)
{
    s32 winId;
    s32 ret;

    if (arg == -1) {
        winId = HuWinWarningCreate(lbl_1_rodata_2C0, lbl_1_rodata_2C4, 478, 94);
    } else {
        winId = arg;
    }
    SLWinIdSet(winId);
    do {
        ret = fn_1_CFF4(SLSaveFileName, 0xA000, saveBuf[curSlotNo]);
        if (ret == 0) {
            ret = 0;
            break;
        }
        ret = fn_1_C2F0(ret, winId);
    } while (ret == -3);
    if (arg == -1) {
        SLWinIdSet(-1);
        HuWinWarningKill(winId);
    }
    return ret;
}

/* 0xD448 */
s32 fn_1_D448(s32 length, const void *addr)
{
    s32 winId;
    s32 ret;

    if (SLSaveFlagGet() == 0) {
        return 0;
    }
    if (lbl_1_data_568 != -1) {
        winId = (s16)fn_1_DA18(lbl_1_data_568, curSlotNo + 0x90036, -1, 0x46);
    }
    HuSRDisableF = 1;
    HuPrcSleep(0x3c);
    SLSerialNoGet();
    ret = HuCardWriteIdle(&curFileInfo, addr, length, 0);
    if (lbl_1_data_568 != -1) {
        fn_1_DB5C(winId);
    }
    if (ret == 0) {
        ret = SLStatSet(0);
    }
    HuSRDisableF = 0;
    return ret;
}

/* 0xD558 (saveBuf[curSlotNo] per resolution 5) */
s32 fn_1_D558(void)
{
    s32 rv;
    s32 ret;
    s32 stat;
    s32 wstat;
    s32 wret;
    s32 winId1;
    s32 winId2;
    u8 *buf;
    s32 closeRet;

    SLCheckSumBoxAllSet();
    SLSaveBackup();
    if (SLSaveFlagGet() == 0) {
        stat = 0;
    } else {
        ret = fn_1_CAA4(curSlotNo);
        if (ret < 0) {
            stat = ret;
        } else {
            ret = HuCardOpen(curSlotNo, SLSaveFileName, &curFileInfo);
            if (ret == -4) {
                stat = -4;
            } else if (ret == -2) {
                fn_1_DD84(7);
                stat = -0x80;
            } else if (ret == -0x80) {
                fn_1_DD84(1);
                stat = -0x80;
            } else if (ret == -3) {
                fn_1_DD84(0);
                stat = -3;
            } else if (ret == -6) {
                ret = HuCardSectorSizeGet(curSlotNo);
                if (ret > 0 && ret != 0x2000) {
                    fn_1_DD84(8);
                    stat = -2;
                } else {
                    UnMountCnt = 0;
                    fn_1_DD84(5);
                    stat = -6;
                }
            } else {
                stat = 0;
            }
        }
    }
    rv = stat;
    if (rv == -4) {
        if (SLSerialNoCheck() == 0) {
            fn_1_DD84(9);
        } else {
            rv = fn_1_CFF4(SLSaveFileName, 0xa000, saveBuf[curSlotNo]);
            if (rv >= 0) {
                SLSerialNoGet();
            }
        }
    } else if (rv >= 0) {
        if (SLSerialNoCheck() == 0) {
            fn_1_DD84(9);
        } else {
            winId1 = (s16)fn_1_DBA0(0x9000b, curSlotNo + 0x90036, -1, 0x46);
            buf = saveBuf[curSlotNo];
            if (SLSaveFlagGet() == 0) {
                wret = 0;
            } else {
                if ((u32)lbl_1_data_568 != 0xffffffff) {
                    winId2 = (s16)fn_1_DA18(lbl_1_data_568, curSlotNo + 0x90036, -1, 0x46);
                }
                HuSRDisableF = 1;
                HuPrcSleep(0x3c);
                SLSerialNoGet();
                wstat = HuCardWriteIdle(&curFileInfo, buf, 0xa000, 0);
                if ((u32)lbl_1_data_568 != 0xffffffff) {
                    fn_1_DB5C(winId2);
                }
                if (wstat == 0) {
                    wstat = SLStatSet(0);
                }
                HuSRDisableF = 0;
                wret = wstat;
            }
            rv = wret;
            fn_1_DD24(winId1);
            if (rv == -3) {
                fn_1_DD84(0);
            } else if (rv == -2) {
                fn_1_DD84(7);
            } else if (rv == -6) {
                rv = HuCardSectorSizeGet(curSlotNo);
                if (rv > 0 && rv != 0x2000) {
                    fn_1_DD84(8);
                } else {
                    fn_1_DD84(5);
                    return -6;
                }
            } else if (rv < 0) {
                fn_1_DD84(1);
            }
        }
    }
    if (SLSaveFlagGet() != 0) {
        closeRet = HuCardClose(&curFileInfo);
    }
    return rv;
}

/* 0xD90C */
s32 fn_1_D90C(s16 winId_in, s32 arg1)
{
    s32 winId;
    s32 ret;

    if (winId_in == -1) {
        winId = HuWinWarningCreate(lbl_1_rodata_2C0, lbl_1_rodata_2C4, 0x1de, 0x5e);
    } else {
        winId = winId_in;
    }
    SLWinIdSet(winId);
    lbl_1_data_568 = arg1;
    do {
        ret = fn_1_D558();
        if (SLSerialNoCheck() != 0) {
            if (ret == 0) {
                ret = 0;
                break;
            }
        } else {
            if (ret == 0) {
                ret = -5;
                break;
            }
        }
        ret = fn_1_C2F0(ret, winId);
    } while (ret == -3);
    if (winId_in == -1) {
        SLWinIdSet(-1);
        HuWinWarningKill(winId);
    }
    lbl_1_data_568 = -1;
    return ret;
}

/* 0xDA18 */
HUWINID fn_1_DA18(u32 mesId, u32 insMes0, u32 insMes1, s16 arg3)
{
    HuVec2f maxSize;
    HUWINID winId;

    if (insMes0 != 0xFFFFFFFFU) {
        HuWinInsertMesSizeGet(insMes0, 0);
    }
    if (insMes1 != 0xFFFFFFFFU) {
        HuWinInsertMesSizeGet(insMes1, 1);
    }
    HuWinMesMaxSizeGet(1, &maxSize, mesId);
    winId = HuWinWarningCreate(-10000.0f, (f32)arg3, maxSize.x, maxSize.y);
    HuWinWarningOpen(winId);
    if (insMes0 != 0xFFFFFFFFU) {
        HuWinInsertMesSet(winId, insMes0, 0);
    }
    if (insMes1 != 0xFFFFFFFFU) {
        HuWinInsertMesSet(winId, insMes1, 1);
    }
    HuWinMesSet(winId, mesId);
    HuWinMesWait(winId);
    return winId;
}

/* 0xDB5C */
void fn_1_DB5C(s16 winId)
{
    if (winId >= 0) {
        HuWinWarningClose(winId);
        HuWinWarningKill(winId);
    }
}

/* 0xDBA0 */
HUWINID fn_1_DBA0(u32 messNum, u32 insMesNum1, u32 insMesNum2, s16 posY)
{
    HUWINID winId;
    HuVec2f maxSize;

    if (SLWinId == -1) {
        HuWinInit(1);
    }
    if (insMesNum1 != -1) {
        HuWinInsertMesSizeGet(insMesNum1, 0);
    }
    if (insMesNum2 != -1) {
        HuWinInsertMesSizeGet(insMesNum2, 1);
    }
    HuWinMesMaxSizeGet(1, &maxSize, messNum);
    if (SLWinId == -1) {
        winId = ((HUWINID (*)(f32, f32, int, int))HuWinWarningCreate)(lbl_1_rodata_2C0, posY, (int)maxSize.x, (int)maxSize.y);
    } else {
        winId = SLWinId;
    }
    HuWinWarningOpen(winId);
    if (insMesNum1 != -1) {
        HuWinInsertMesSet(winId, insMesNum1, 0);
    }
    if (insMesNum2 != -1) {
        HuWinInsertMesSet(winId, insMesNum2, 1);
    }
    HuWinMesSet(winId, messNum);
    HuWinMesWait(winId);
    return winId;
}

/* 0xDD24 */
void fn_1_DD24(s16 winId)
{
    if (SLWinId != winId && winId >= 0) {
        HuWinWarningClose(winId);
        HuWinWarningKill(winId);
    }
}

/* 0xDD84 */
s16 fn_1_DD84(s16 mode)
{
    s16 warnId;
    s16 choice;
    u32 mesId;
    u32 insertMesId;
    s32 flag;
    HUWIN *winP;
    f32 pos[2];

    choice = -1;
    insertMesId = 0;
    flag = 0;
    if (SLWinId == -1) {
        HuWinInit(1);
    }
    switch (mode) {
    case 0:
        mesId = 0x90001;
        HuWinInsertMesSizeGet(curSlotNo + 0x90036, 0);
        insertMesId = curSlotNo + 0x90036;
        UnMountCnt = 0;
        break;
    case 1:
        mesId = 0x90004;
        HuWinInsertMesSizeGet(curSlotNo + 0x90036, 0);
        insertMesId = curSlotNo + 0x90036;
        break;
    case 2:
        HuWinInsertMesSizeGet(curSlotNo + 0x90036, 0);
        insertMesId = curSlotNo + 0x90036;
        mesId = 0x90006;
        break;
    case 3:
        mesId = 0x90005;
        HuWinInsertMesSizeGet(curSlotNo + 0x90036, 0);
        insertMesId = curSlotNo + 0x90036;
        break;
    case 4:
        mesId = 0x90035;
        HuWinInsertMesSizeGet(curSlotNo + 0x90036, 0);
        insertMesId = curSlotNo + 0x90036;
        break;
    case 5:
        HuWinInsertMesSizeGet(curSlotNo + 0x90036, 0);
        insertMesId = curSlotNo + 0x90036;
        mesId = 0x90002;
        break;
    case 6:
        mesId = 0x9001c;
        break;
    case 7:
        HuWinInsertMesSizeGet(curSlotNo + 0x90036, 0);
        insertMesId = curSlotNo + 0x90036;
        mesId = 0x90003;
        break;
    case 8:
        HuWinInsertMesSizeGet(curSlotNo + 0x90036, 0);
        insertMesId = curSlotNo + 0x90036;
        mesId = 0x90021;
        break;
    case 9:
        mesId = 0x90044;
        break;
    case 10:
        mesId = 0x90018;
        flag = 1;
        break;
    case 11:
        HuWinInsertMesSizeGet(curSlotNo + 0x90036, 0);
        insertMesId = curSlotNo + 0x90036;
        mesId = 0x9003a;
        break;
    case 12:
        mesId = 0x90026;
        break;
    }
    if (SLWinId == -1) {
        pos[0] = lbl_1_rodata_2D0;
        pos[1] = lbl_1_rodata_2D4;
        warnId = HuWinWarningCreate(lbl_1_rodata_2C0, lbl_1_rodata_2C4, pos[0], pos[1]);
    } else {
        warnId = SLWinId;
    }
    winP = &winData[warnId];
    winP->padMask = 1;
    if (insertMesId != 0) {
        HuWinInsertMesSet(warnId, insertMesId, 0);
    }
    HuWinAttrSet(warnId, 0x10);
    HuWinWarningOpen(warnId);
    HuWinMesSet(warnId, mesId);
    HuWinMesWait(warnId);
    if (flag != 0) {
        choice = HuWinChoiceGet(warnId, 1);
        if (mode == 5 && choice == 0) {
            HuWinInsertMesSet(warnId, curSlotNo + 0x90036, 0);
            HuWinMesSet(warnId, 0x9001a);
            HuWinMesWait(warnId);
            choice = HuWinChoiceGet(warnId, 1);
        }
    }
    if (mode == 0xb) {
        while ((HuPadBtnDown[0] & 0x100) == 0) {
            HuPrcVSleep();
        }
        HuAudFXPlay(1);
    }
    if (SLWinId == -1) {
        HuWinWarningClose(warnId);
        HuWinWarningKill(warnId);
    }
    return choice;
}

/* 0xE210 */
s32 fn_1_E210(s16 arg0, s16 arg1)
{
    s16 choices[10];
    HuVec2f maxSize;
    s32 msgId;
    HUWINID winId;
    s16 choice;
    s16 v;
    HUWIN *w;

    v = arg0 & ~0x10;
    if (v == 7) {
        msgId = 0x9003f;
        choices[0] = 1;
        choices[1] = 4;
        choices[2] = 2;
    } else if (v == 0xd) {
        msgId = 0x90040;
        choices[0] = 1;
        choices[1] = 4;
        choices[2] = 8;
    } else if (v == 5) {
        msgId = 0x90041;
        choices[0] = 1;
        choices[1] = 4;
    }
    HuWinMesMaxSizeGet(1, &maxSize, msgId);
    w = &winData[arg1];
    if ((f32)w->winH < maxSize.y) {
        HuWinWarningClose(arg1);
        winId = HuWinWarningCreate(lbl_1_rodata_2C0, lbl_1_rodata_2C4, (s16)maxSize.x, (s16)maxSize.y);
    } else {
        winId = arg1;
    }
    HuWinWarningOpen(winId);
    if ((arg0 & 0x10) == 0) {
        HuWinAttrSet(winId, 0x10);
    }
    HuWinMesSet(winId, msgId);
    HuWinMesWait(winId);
    choice = HuWinChoiceGet(winId, 0);
    if (arg1 != winId) {
        HuWinWarningClose(winId);
        HuWinWarningKill(winId);
    }
    if (choice == -1) {
        return -0x4d2;
    }
    return choices[choice];
}

/* ======================================================================== */
/* 0xE404  fn_1_E404 == __cvt_fp2unsigned (MetroWerks runtime helper, INLINE ASM). */
/* Saturating f64 -> u32.  Its own rodata pool `lbl_1_rodata_2D8` is DEFINED here  */
/* (global, non-static: the lis/ori reloc pairs by symbol NAME).  Named fn_1_E404  */
/* so it name-pairs the target symbol.  NOTE for parent (REL-link endgame): add a  */
/* symbols.txt alias `__cvt_fp2unsigned = .text:0xE404;` and extend the fileseldll  */
/* .rodata split 0x2D8 -> 0x2F0 (+0x18) for these 3 f64 constants. See e404/deps.md. */
/* ======================================================================== */
const f64 lbl_1_rodata_2D8[3] = { 0.0, 4294967296.0, 2147483648.0 };

asm u32 fn_1_E404(register f64 d)
{
	nofralloc
	stwu    r1,-16(r1)
	lis     r4, lbl_1_rodata_2D8@h
	ori     r4, r4, lbl_1_rodata_2D8@l
	li      r3,0
	lfd     fp0,0(r4)
	lfd     fp3,8(r4)
	lfd     fp4,16(r4)
	fcmpu   cr0,fp1,fp0
	fcmpu   cr6,fp1,fp3
	blt     cr0, @exit
	addi    r3,r3,-1
	bge     cr6,@exit
	fcmpu   cr7,fp1,fp4
	fmr     fp2,fp1
	blt     cr7,@1
	fsub    fp2,fp1,fp4
@1	fctiwz  fp2,fp2
	stfd    fp2,8(r1)
	lwz     r3,12(r1)
	blt     cr7,@exit
	addis   r3,r3,-0x8000
@exit:
	addi    r1,r1,16
	blr
}
