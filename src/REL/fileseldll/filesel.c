#include "dolphin.h"
#include "game/card.h"
#include "game/process.h"
#include "game/window.h"

typedef void (*VoidFunc)(void);

typedef struct filesel_work {
    /* 0x00 */ u8 unk_00[0x62];
    /* 0x62 */ u8 unk_62;
    /* 0x63 */ u8 unk_63;
    /* 0x64 */ Vec unk_64;
    /* 0x70 */ u8 unk_70[0x10];
} FILESEL_WORK; /* size = 0x80 */

extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];

void fn_1_A0(void);
void fn_1_6A8C(void);
extern s16 lbl_1_bss_2E0;
extern FILESEL_WORK lbl_1_bss_D8[];
extern CARDFileInfo curFileInfo;

s32 SLSaveFlagGet(void);

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

void fn_1_C60(void)
{
    GXSetPixelFmt(1, GX_ZC_LINEAR);
    fn_1_6A8C();
}

void fn_1_C8C(void)
{
    GXSetPixelFmt(0, GX_ZC_LINEAR);
}

void fn_1_179C(void)
{
    HuWinExOpen(lbl_1_bss_2E0);
    HuPrcEnd();
}

void fn_1_DB5C(s16 winId)
{
    if (winId >= 0) {
        HuWinWarningClose(winId);
        HuWinWarningKill(winId);
    }
}

void fn_1_3C80(s16 id, Vec *pos)
{
    FILESEL_WORK *p = &lbl_1_bss_D8[id];

    p->unk_64 = *pos;
}

void fn_1_3CC0(s16 id, f32 x, f32 y, f32 z)
{
    FILESEL_WORK *p = &lbl_1_bss_D8[id];

    p->unk_64.x = x;
    p->unk_64.y = y;
    p->unk_64.z = z;
}

void fn_1_3CF4(s16 id, s32 flag)
{
    lbl_1_bss_D8[id].unk_62 = (u8)flag;
}

s32 fn_1_CA58(void)
{
    s32 ret;

    if (SLSaveFlagGet() == 0) {
        return 0;
    }
    ret = HuCardClose(&curFileInfo);
    return ret;
}
