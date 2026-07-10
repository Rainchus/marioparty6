/* REL/fileseldll/filename.c — save-name-entry keyboard TU (split out of the dtk-merged
 * filesel.c per the MP5 fileseldll/filename.c structure: NameEnterInit / NameEnterMain /
 * NameEnterClose + static helpers). Contiguous .text 0x8B94..0xB5B4 (7 fns). Its rodata is
 * its OWN fresh 0x240-based pool (with its own int->float magic double) — that separate pool
 * is exactly what made the merged filesel.c rodata look like it had a "dedup cluster break"
 * at 0x240; it was the filesel.o/filename.o object boundary. */
/* Block math.h: its "extern inline" sqrtf uses `static const double _half/_three`
 * which emit WEAK COMDAT statics into every including TU's .rodata head. The original
 * filename object's copy was discarded at link (deduped against filesel.c's earlier
 * copy), so the split target filename object has none. filename's sqrt uses REGULAR
 * pool 0.5/3.0 (target lbl_1_rodata_288/290) — provide sqrtf below with literals. */
#define _MATH_H
#pragma pool_data off

#include "dolphin.h"
#include "game/card.h"
#include "game/window.h"
#include "game/sprite.h"
#include "game/gamework.h"
#include "game/audio.h"
#include "game/pad.h"
#include "game/data.h"
#include "game/memory.h"
#include "game/main.h"
#include "string.h"

#pragma cplusplus on
extern inline float sqrtf(float x)
{
    volatile float y;
    if (x > 0.0f)
    {
        double guess = __frsqrte((double)x);
        guess = 0.5 * guess * (3.0 - guess * guess * x);
        guess = 0.5 * guess * (3.0 - guess * guess * x);
        guess = 0.5 * guess * (3.0 - guess * guess * x);
        y = (float)(x * guess);
        return y;
    }
    return x;
}
#pragma cplusplus reset

/* cross-boundary FILESEL_WORK view (owned by filesel.c .bss; only unk_44 used here) */
typedef struct filesel_work {
    /* 0x00 */ s32 unk_0;
    /* 0x04 */ s16 unk_4;
    /* 0x06 */ s16 unk_6;
    /* 0x08 */ s16 unk_8;
    /* 0x0A */ s16 unk_A;
    /* 0x0C */ s16 unk_C;
    /* 0x0E */ s16 unk_E;
    /* 0x10 */ s16 unk_10;
    /* 0x12 */ s16 unk_12;
    /* 0x14 */ s16 unk_14;
    /* 0x16 */ s16 unk_16;
    /* 0x18 */ s16 unk_18;
    /* 0x1A */ s16 unk_1A;
    /* 0x1C */ OSCalendarTime unk_1C;
    /* 0x44 */ char unk_44[0x11];
    /* 0x55 */ u8 unk_55[0x03];
    /* 0x58 */ ANIMDATA *unk_58;
    /* 0x5C */ void *unk_5C;
    /* 0x60 */ u8 unk_60[2];
    /* 0x62 */ u8 unk_62;
    /* 0x63 */ u8 unk_63;
    /* 0x64 */ Vec unk_64;
    /* 0x70 */ Vec unk_70;
    /* 0x7C */ void *unk_7C;
} FILESEL_WORK; /* size = 0x80 */

/* lbl_1_bss_348 element, 6 bytes (name-grid key: x,y,val) */
typedef struct b234_point { s16 x; s16 y; u16 flag; } B234Point;
/* fn_1_B234 out rect */
typedef struct b234_rect { f32 x0; f32 y0; f32 x1; f32 y1; } B234Rect;
/* lbl_1_data_48C[] element (NameEnterInit sprite table), stride 0xC */
typedef struct data48c {
    /* 0x0 */ s32 unk_0;
    /* 0x4 */ u8  unk_4;
    /* 0x5 */ u8  pad_5;
    /* 0x6 */ s16 unk_6;
    /* 0x8 */ s16 unk_8;
    /* 0xA */ s16 unk_A;
} DATA48C; /* size 0xC */


/* ==================== this TU .data / .bss (ref by dtk name until data pass) ==================== */
extern DATA48C lbl_1_data_48C[];
extern B234Point *lbl_1_data_47C[];
extern B234Point *lbl_1_bss_348;
extern char lbl_1_bss_34C[];
extern s16 lbl_1_bss_37E;
extern u16 lbl_1_bss_380[];
extern s16 lbl_1_bss_3AC[];
extern u8 lbl_1_bss_3B2[][0x11];
extern s16 lbl_1_bss_42A;
extern s16 lbl_1_bss_42C;
extern s16 lbl_1_bss_42E;

/* ==================== cross-boundary: owned by filesel.c (extern here) ==================== */
extern FILESEL_WORK lbl_1_bss_D8[];
extern s16 lbl_1_bss_2DE;
extern s16 lbl_1_bss_2E0;

/* ==================== forward decls ==================== */
void NameEnterInit(void);
void fn_1_8F34(s32 arg0, s16 idx);
s32 NameEnterMain(s16 no);
s16 fn_1_AAEC(s16 winId);
s16 fn_1_ABF8(s16 id, f32 refX, f32 refY, f32 dirX, f32 dirY);
void fn_1_B234(B234Rect *r);
void NameEnterClose(void);
void FileCommonInit(void); /* saveload.c (cross-TU; pair by name) */

void NameEnterInit(void)
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
    HuSprGrpPosSet(grp, 288.0f, 200.0f);
    for (i = 0; i < 2; i++) {
        HUWIN *w;
        lbl_1_bss_3AC[i] = HuWinCreate(40.0f, 110.0f, 0x1f4, 0xfa, 0);
        w = &winData[lbl_1_bss_3AC[i]];
        HuWinBGTPLvlSet(lbl_1_bss_3AC[i], 0.0f);
        anim = HuSprAnimRead(HuDataSelHeapReadNum(i + 0x23001F, 0x10000000, 2));
        if (i == 0) {
            HuWinAnimSet(lbl_1_bss_3AC[i], anim, 0, 250.0f, 90.0f);
        } else {
            HuWinAnimSet(lbl_1_bss_3AC[i], anim, 0, 250.0f, 72.0f);
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
        24.0f + ((f32)lbl_1_bss_348[idx].x - 288.0f),
        ((f32)lbl_1_bss_348[idx].y - 200.0f) - 12.0f);
    if (btn >= 0x100 && btn <= 0x500) {
        HuSprAttrSet(lbl_1_bss_42E, 1, 4);
    } else {
        HuSprAttrReset(lbl_1_bss_42E, 1, 4);
        HuSprPosSet(lbl_1_bss_42E, 1,
            10.0f + ((f32)lbl_1_bss_348[idx].x - 288.0f),
            12.0f + ((f32)lbl_1_bss_348[idx].y - 200.0f));
    }
}

/* 0x916C  (enter-save-name grid main loop; largest fn in the module).
 * MUST precede fn_1_AAEC/ABF8/B234/B530/B5B4 so -inline auto keeps real bl calls.
 * Two match-critical spellings: the L/R page scans use `(sel + 1) * 0x100` (the MUL
 * node canonicalizes to the target's cmpw operand order — NOT `(sel + 1) << 8`), and
 * the SJIS re-pack loop init `for (j = k = 0; j < count; k++, j++)` (k bumps first). */
s32 NameEnterMain(s16 no)
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
    HuSprPosSet(lbl_1_bss_42E, 0, 24.0f + ((f32)lbl_1_bss_348[cur].x - 288.0f),
                (f32)lbl_1_bss_348[cur].y - 200.0f - 12.0f);
    if (code0 >= 0x100 && code0 <= 0x500) {
        HuSprAttrSet(lbl_1_bss_42E, 1, 4);
    } else {
        HuSprAttrReset(lbl_1_bss_42E, 1, 4);
        HuSprPosSet(lbl_1_bss_42E, 1, 10.0f + ((f32)lbl_1_bss_348[cur].x - 288.0f),
                    12.0f + ((f32)lbl_1_bss_348[cur].y - 200.0f));
    }

    HuSprBankSet(lbl_1_bss_42E, 3, 8);
    HuSprBankSet(lbl_1_bss_42E, 4, 8);
    HuSprBankSet(lbl_1_bss_42E, 5, 8);
    HuSprBankSet(lbl_1_bss_42E, 6, 2);
    HuSprBankSet(lbl_1_bss_42E, 7, 4);
    HuAudFXPlay(0);
    NameEnterClose();
    HuWinMesSet(lbl_1_bss_2E0, 0x90009);
    HuWinMesSet(lbl_1_bss_2DE, 0x10002);
    HuWinDispOn(lbl_1_bss_2DE);
    winId = HuWinCreate(-10000.0f, 70.0f, 0xb0, 0x1a, 0);
    HuWinBGTPLvlSet(winId, 0.0f);
    HuWinMesSpeedSet(winId, 0);
    HuWinPriSet(winId, 0x28);
    HuWinMesSet(winId, (u32)lbl_1_bss_34C);

    for (lbl_1_bss_42C = 0; lbl_1_data_47C[lbl_1_bss_42C] != 0; lbl_1_bss_42C++) {
        ;
    }
    fn_1_B234(&rect);

L_95A8:
    dirX = dirY = 0.0f;
    if (HuPadDStkRep[0] & 1) {
        dirX = -1.0f;
    } else if (HuPadDStkRep[0] & 2) {
        dirX = 1.0f;
    }
    if (HuPadDStkRep[0] & 8) {
        dirY = -1.0f;
    } else if (HuPadDStkRep[0] & 4) {
        dirY = 1.0f;
    }
    if (!dirX && !dirY) {
        goto L_99B4;
    }
    newIdx = fn_1_ABF8(cur, (f32)lbl_1_bss_348[cur].x, (f32)lbl_1_bss_348[cur].y, dirX, dirY);
    if (newIdx == -1) {
        if (dirX) {
            dirY = 0.0f;
            if (dirX < 0.0f) {
                refX = rect.x1;
            } else {
                refX = rect.x0;
            }
            refY = (f32)lbl_1_bss_348[cur].y;
        } else {
            dirX = 0.0f;
            if (dirY < 0.0f) {
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
        HuSprPosSet(lbl_1_bss_42E, 0, 24.0f + ((f32)lbl_1_bss_348[cur].x - 288.0f),
                    (f32)lbl_1_bss_348[cur].y - 200.0f - 12.0f);
        if (code2 >= 0x100 && code2 <= 0x500) {
            HuSprAttrSet(lbl_1_bss_42E, 1, 4);
        } else {
            HuSprAttrReset(lbl_1_bss_42E, 1, 4);
            HuSprPosSet(lbl_1_bss_42E, 1, 10.0f + ((f32)lbl_1_bss_348[cur].x - 288.0f),
                        12.0f + ((f32)lbl_1_bss_348[cur].y - 200.0f));
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
    HuSprPosSet(lbl_1_bss_42E, 0, 24.0f + ((f32)lbl_1_bss_348[cur].x - 288.0f),
                (f32)lbl_1_bss_348[cur].y - 200.0f - 12.0f);
    if (code3 >= 0x100 && code3 <= 0x500) {
        HuSprAttrSet(lbl_1_bss_42E, 1, 4);
    } else {
        HuSprAttrReset(lbl_1_bss_42E, 1, 4);
        HuSprPosSet(lbl_1_bss_42E, 1, 10.0f + ((f32)lbl_1_bss_348[cur].x - 288.0f),
                    12.0f + ((f32)lbl_1_bss_348[cur].y - 200.0f));
    }
    HuPrcVSleep();
    goto L_95A8;

L_AA78:
    HuWinHomeClear(lbl_1_bss_2DE);
    HuWinHomeClear(lbl_1_bss_2E0);
    HuWinKill(winId);
    HuAudFXPlay(0);
    FileCommonInit();
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
void NameEnterClose(void)
{
    s16 i;

    for (i = 0; i < 18; i++) {
        HuSprAttrReset(lbl_1_bss_42E, i, 4);
        HuSprTPLvlSet(lbl_1_bss_42E, i, 1.0f);
    }
    HuWinDispOn(lbl_1_bss_3AC[0]);
}

/* ==================== this TU .data definitions ==================== */
B234Point lbl_1_data_218[61] = {{80, 140, 0x41}, {110, 140, 0x42}, {140, 140, 0x43}, {170, 140, 0x44}, {200, 140, 0x45}, {230, 140, 0x46}, {260, 140, 0x47}, {290, 140, 0x48}, {320, 140, 0x49}, {350, 140, 0x4A}, {380, 140, 0x4B}, {410, 140, 0x4C}, {440, 140, 0x4D}, {470, 140, 0x4E}, {80, 176, 0x4F}, {110, 176, 0x50}, {140, 176, 0x51}, {170, 176, 0x52}, {200, 176, 0x53}, {230, 176, 0x54}, {260, 176, 0x55}, {290, 176, 0x56}, {320, 176, 0x57}, {350, 176, 0x58}, {380, 176, 0x59}, {410, 176, 0x5A}, {440, 176, 0x20}, {470, 176, 0x20}, {80, 212, 0x61}, {110, 212, 0x62}, {140, 212, 0x63}, {170, 212, 0x64}, {200, 212, 0x65}, {230, 212, 0x66}, {260, 212, 0x67}, {290, 212, 0x68}, {320, 212, 0x69}, {350, 212, 0x6A}, {380, 212, 0x6B}, {410, 212, 0x6C}, {440, 212, 0x6D}, {470, 212, 0x6E}, {80, 248, 0x6F}, {110, 248, 0x70}, {140, 248, 0x71}, {170, 248, 0x72}, {200, 248, 0x73}, {230, 248, 0x74}, {260, 248, 0x75}, {290, 248, 0x76}, {320, 248, 0x77}, {350, 248, 0x78}, {380, 248, 0x79}, {410, 248, 0x7A}, {440, 248, 0xC2}, {470, 248, 0xC3}, {120, 293, 0x100}, {200, 293, 0x200}, {348, 293, 0x400}, {468, 293, 0x500}, {0, 0, 0x0}};
B234Point lbl_1_data_386[41] = {{114, 136, 0x31}, {144, 136, 0x32}, {174, 136, 0x33}, {204, 136, 0x34}, {234, 136, 0x35}, {264, 136, 0x36}, {294, 136, 0x37}, {324, 136, 0x38}, {354, 136, 0x39}, {384, 136, 0x30}, {414, 136, 0x86}, {444, 136, 0x20}, {114, 172, 0x5B}, {144, 172, 0x5C}, {174, 172, 0x7E}, {204, 172, 0x7F}, {234, 172, 0x3C}, {264, 172, 0x3D}, {294, 172, 0x5F}, {324, 172, 0x5D}, {354, 172, 0x5E}, {384, 172, 0x82}, {414, 172, 0x85}, {444, 172, 0x7D}, {114, 208, 0xE03}, {144, 208, 0xE04}, {174, 208, 0xE05}, {204, 208, 0xE06}, {234, 208, 0xE09}, {264, 208, 0xE07}, {294, 208, 0xE01}, {324, 208, 0xE0B}, {354, 208, 0x3B}, {384, 208, 0x3A}, {414, 208, 0xE15}, {444, 208, 0xE16}, {120, 293, 0x100}, {200, 293, 0x200}, {348, 293, 0x400}, {468, 293, 0x500}, {0, 0, 0x0}};
B234Point *lbl_1_data_47C[4] = {lbl_1_data_218, lbl_1_data_386, NULL, NULL};
DATA48C lbl_1_data_48C[18] = {
    {0x00230016, 0x00, 0x00, 100, 0, 0},
    {0x0023001C, 0x01, 0x00, 120, 0, 0},
    {0x0023001C, 0x06, 0x00, 120, 4, -110},
    {0x0023001C, 0x08, 0x00, 120, -156, 107},
    {0x0023001C, 0x08, 0x00, 120, -76, 107},
    {0x0023001C, 0x08, 0x00, 120, -1000, -1000},
    {0x0023001C, 0x02, 0x00, 120, 60, 105},
    {0x0023001C, 0x04, 0x00, 120, 180, 105},
    {0x00230014, 0x00, 0x00, 100, -190, -120},
    {0x00230014, 0x01, 0x00, 100, 190, -120},
    {0x00230015, 0x00, 0x00, 100, -230, -120},
    {0x00230015, 0x02, 0x00, 100, 230, -120},
    {0x0023001E, 0x00, 0x00, 100, 60, 105},
    {0x0023001E, 0x02, 0x00, 100, 180, 105},
    {0x0023001E, 0x04, 0x00, 100, -156, 105},
    {0x0023001E, 0x06, 0x00, 100, -76, 105},
    {0x0023001E, 0x06, 0x00, 100, -1000, -1000},
    {0x00230017, 0x00, 0x00, 125, 0, -4},
};

/* ==================== this TU .bss (REVERSE definition order — MWCC assigns .bss reverse) ==================== */
s16 lbl_1_bss_42E;
s16 lbl_1_bss_42C;
s16 lbl_1_bss_42A;
u8 lbl_1_bss_3B2[7][0x11];
s16 lbl_1_bss_3AC[3];
u16 lbl_1_bss_380[22];
s16 lbl_1_bss_37E;
char lbl_1_bss_34C[0x32];
B234Point *lbl_1_bss_348;
