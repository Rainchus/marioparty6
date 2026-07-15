#include "datadir_enum.h"

#include "dolphin/os.h"

#include "game/armem.h"
#include "game/audio.h"
#include "game/charman.h"
#include "game/flag.h"
#include "game/mgdata.h"
#include "game/object.h"
#include "game/wipe.h"
#include "game/window.h"
#include "msm_stream.h"

typedef void (*VoidFunc)(void);

extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];

extern void HuDataDirCloseAll(void);

extern int lbl_1_bss_0;
extern int lbl_1_bss_4;
extern HuVecF lbl_1_bss_19AC[4];
extern s32 lbl_1_bss_1A2C;
extern s16 lbl_1_bss_1A30[3];
extern HUWINID lbl_1_bss_1A40[4];

extern HuVecF lbl_1_data_28[6];
extern s16 lbl_1_data_70[6];
extern s32 lbl_1_data_7C;
extern char lbl_1_data_A1[];
extern char lbl_1_data_CF[];
extern char lbl_1_data_E0[];
extern char lbl_1_data_F2[];
extern char lbl_1_data_104[];
extern char lbl_1_data_112[];
extern char lbl_1_data_114[];
extern s16 lbl_1_data_150[2];
extern s32 lbl_1_data_154[2];
extern char lbl_1_data_2D2[];

void fn_1_A5D4(void);
void fn_1_9910(void);
s16 fn_1_BAB4(void);
void fn_1_E1FC(void);
s16 fn_1_E7B0(void);
void ObjectSetup(void);

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

void fn_1_2080(s16 winNo)
{
    s16 newWin;
    s16 oldWin;

    if (lbl_1_data_150[0] != -1 && lbl_1_data_150[0] != winNo) {
        oldWin = lbl_1_data_150[0];
        if (oldWin == 0) {
            HuWinDispOff(lbl_1_bss_1A40[oldWin]);
        } else {
            HuWinExClose(lbl_1_bss_1A40[oldWin]);
        }
    }
    if (lbl_1_data_150[0] == -1 || lbl_1_data_150[0] != winNo) {
        lbl_1_data_150[0] = winNo;
        lbl_1_data_154[0] = -1;
        newWin = lbl_1_data_150[0];
        if (newWin == 0) {
            HuWinDispOn(lbl_1_bss_1A40[newWin]);
        } else {
            HuWinExOpen(lbl_1_bss_1A40[newWin]);
        }
    }
}

void fn_1_21DC(void)
{
    s16 current;

    if (lbl_1_data_150[0] != -1) {
        current = lbl_1_data_150[0];
        if (current == 0) {
            HuWinDispOff(lbl_1_bss_1A40[current]);
        } else {
            HuWinExClose(lbl_1_bss_1A40[current]);
        }
    }
    lbl_1_data_150[0] = -1;
    lbl_1_data_154[0] = -1;
}

void fn_1_2288(void)
{
    s16 current;

    if (lbl_1_data_150[0] != -1) {
        current = lbl_1_data_150[0];
        HuWinMesWait(lbl_1_bss_1A40[current]);
    }
}

void fn_1_23E0(s16 winNo, s32 messNum, s16 speed)
{
    s32 currentMess;
    s16 currentWin;

    fn_1_2080(winNo);
    if (lbl_1_data_154[0] != messNum) {
        lbl_1_data_154[0] = messNum;
        currentMess = lbl_1_data_154[0];
        currentWin = lbl_1_data_150[0];
        HuWinAttrSet(lbl_1_bss_1A40[currentWin], 0x800);
        HuWinMesSet(lbl_1_bss_1A40[currentWin], currentMess);
        HuWinMesSpeedSet(lbl_1_bss_1A40[currentWin], speed);
        if (lbl_1_data_7C != currentMess) {
            lbl_1_data_7C = -1;
        }
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
char lbl_1_data_A1[] = ">>>>>>>>>> mdseldll :: objsetup!! <<<<<<<<<<\n";
char lbl_1_data_CF[] = "0x%x :: _effect\n";
char lbl_1_data_E0[] = "0x%x :: _gamemes\n";
char lbl_1_data_F2[] = "0x%x :: _mgconst\n";
char lbl_1_data_104[] = "0x%x :: _win\n";
char lbl_1_data_112[] = "\n";
char lbl_1_data_114[0x3C] = "\n>>>>>>>>>> mdseldll :: dllreturn or dllmove!! <<<<<<<<<<\n";
s16 lbl_1_data_150[2] = { -1, -1 };
s32 lbl_1_data_154[2] = { -1, -1 };
char lbl_1_data_2D2[] = "\n-----===== MARIO PARTY 6 :: MODE SELECT =====-----\n\n";

int lbl_1_bss_0;
int lbl_1_bss_4;
HuVecF lbl_1_bss_19AC[4];
s32 lbl_1_bss_1A2C;
s16 lbl_1_bss_1A30[3];
HUWINID lbl_1_bss_1A40[4];
