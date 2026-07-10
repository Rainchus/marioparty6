/* REL/fileseldll/saveload.c — memory-card save/load layer (reconstructed original TU).
 *
 * TU-split evidence: FileBoxInit's sprintf carries NO `crclr cr1eq` while the module's
 * filesel.c sprintf calls (fn_1_36C4) do — this TU never saw a varargs sprintf
 * prototype, i.e. NO stdio.h here (mirrors MP5 saveload.c: fileseldll.h pad audio
 * wipe main sreset mpsystem win — no stdio.h, no string.h, no math.h).
 * strncmp/memcpy/memset/sprintf are implicit declarations, as in the original.
 * OSReport stays varargs via dolphin.h (target B63C has crclr on both OSReport calls).
 *
 * 23 functions, contiguous 0xB5B4..0xE404, 1:1 in-order with MP5 saveload.c:
 *   B5B4 FileCommonInit   B63C FileBoxInit     BF9C FileCheckCardSpace
 *   C098 FileCardWarning  C278 FileTestOpen    C2F0 FileCardErrorExec
 *   C898 FileCardOpen     C9D0 FileCardRead    CA58 FileCardClose
 *   CAA4 FileCardMount    CBC8 FileCardFormat  CDA8 FileCardLoad
 *   CFF4 FileCardCopy     D348 FileClear       D448 FileCardWrite
 *   D558 FileSave         D90C FileSaveMesOpen DA18 FileStatusMesOpen
 *   DB5C FileStatusMesClose  DBA0 FileCardMesOpen  DD24 FileCardMesClose
 *   DD84 FileMessOut      E210 FileCardChoice
 */
/* Block math.h (pulled in transitively): its "extern inline" sqrtf emits weak
 * _half/_three local-static pool data into this TU's .rodata. The original saveload
 * object's copy was discarded at link (deduped against filesel.c's earlier copy), so
 * the split target saveload object has none. This TU uses no math.h functions.
 * #pragma pool_data off keeps per-symbol @ha/@l addressing (no rodata.0 base pooling). */
#define _MATH_H
#pragma pool_data off

#include "dolphin.h"
#include "game/card.h"
#include "game/process.h"
#include "game/window.h"
#include "game/sprite.h"
#include "game/gamework.h"
#include "game/audio.h"
#include "game/pad.h"
#include "game/memory.h"
#include "game/main.h"
#include "game/wipe.h"

/* shared filesel work entry (owned by filesel.c .bss; fileseldll.h-equivalent view) */
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

/* ============ module .bss/.data/.rodata owned by other units (ref by dtk name) ============ */
extern FILESEL_WORK lbl_1_bss_D8[];    /* filesel.c: save-box display state             */
extern s16 lbl_1_bss_3AC[];            /* filesel.c: window ids                          */
extern s16 lbl_1_bss_42E;              /* filesel.c: sprite group id                     */

u32 lbl_1_data_568 = 0xFFFFFFFF;       /* this TU: save-mes id (fileMesId), init -1 */


/* ==================== main-DOL globals (no header) ==================== */
extern s16 curSlotNo;
extern s16 SLWinId;
extern char SLSaveFileName[];
extern char SLEraseStr[];
extern u8 saveBuf[][0xA000];
extern s64 SLSerialNo[];
extern s32 SR_ExecResetMenu;
extern CARDFileInfo curFileInfo;

/* ==================== save-lib funcs (no header; this TU's original decl shapes) ==================== */
s32 SLSaveFlagGet(void);
void SLWinIdSet(s16 winId);
s32 SLSerialNoCheck(void);
void SLSerialNoGet(void);
s32 SLStatSet(s32 stat);
void SLCheckSumBoxAllSet(void);
void SLSaveBackup(void);
s32 SLCurBoxNoSet(s32 boxNo);        /* s32: same shape filesel.c declares (FileselSelect uses
                                        the result); the discarded-result ghost at B63C's
                                        box-loop call is required for its regalloc        */
void SLSaveDataMake(s32 arg, OSTime *time); /* void, as in MP5 saveload.h and filesel.c   */
u16 SLCheckSumGet(s32 start, s32 len);
void SLCurSlotNoSet(s32 slotNo);
void SLSaveEmptySet(s16 slot, s16 idx);
s32 SLBoxDataOffsetGet(s16 boxNo);
s32 SLCheckSumCheck(void);
void SLBoxBackupLoad(s16 boxNo);
void SLCommonLoad(void);

/* NOTE: no stdio.h/string.h — strncmp/memcpy/memset AND sprintf are all implicit
 * declarations (original TU).  Implicit sprintf: int-returning non-prototype — no
 * crclr (not varargs), and its discarded int result is a ghost vreg at the call,
 * which B63C's scratch-register allocation requires (see fn comment). */

/* ==================== cross-TU callee (filesel.c defines it void) ==================== */
void fn_1_36C4(s16 a, s16 b);

/* ==================== forward decls: functions defined in this TU ==================== */
void FileCommonInit(void);
s32 FileBoxInit(s16 arg);
s32 FileCheckCardSpace(void);
s32 FileCardWarning(s16 winId);
s32 FileTestOpen(void);
s32 FileCardErrorExec(s32 err, s16 winId);
s32 FileCardOpen(const char *fileName);
s32 FileCardRead(s32 length, void *addr);
s32 FileCardClose(void);
s32 FileCardMount(s16 slot);
s32 FileCardFormat(s16 arg);
s32 FileCardLoad(void);
s32 FileCardCopy(const char *fileName, s32 size, void *addr);
s32 FileClear(s16 arg);
s32 FileCardWrite(s32 length, const void *addr);
s32 FileSave(void);
s32 FileSaveMesOpen(s16 winId_in, s32 arg1);
HUWINID FileStatusMesOpen(u32 mesId, u32 insMes0, u32 insMes1, s16 arg3);
void FileStatusMesClose(s16 winId);
HUWINID FileCardMesOpen(u32 messNum, u32 insMesNum1, u32 insMesNum2, s16 posY);
void FileCardMesClose(s16 winId);
s16 FileMessOut(s16 mode);
s32 FileCardChoice(s16 mesNo, s16 winId);

/* ======================================================================== */
/* functions in strict ascending target-address order */
/* ======================================================================== */

/* 0xB5B4 */
void FileCommonInit(void)
{
    s16 i;

    for (i = 0; i < 18; i++) {
        HuSprAttrSet(lbl_1_bss_42E, i, 4);
    }
    for (i = 0; i < 3; i++) {
        HuWinDispOff(lbl_1_bss_3AC[i]);
    }
}

/* 0xB63C  FileBoxInit — MP5-saveload.c-shaped reconstruction (while(1) retry loop,
 * else-if chain with cardError: label, block-scoped char buf[8] in the repair-window
 * loop, MP5 declaration order: brokenFlag=0 first, aggregates boxStatus/time at top,
 * buf innermost).  100% — full-module objdiff match.
 *
 * ★ Regalloc key (the last 0.2% was 23 volatile-scratch rows in 6 clusters): MWCC
 * -O0 scratch picks depend on the WHOLE function's discarded-result ghost-vreg set,
 * not just local code.  The exact decl combo that lands every tie (found by a full
 * 2^9 sweep of the free return-type knobs; only 3/512 combos give 0 diff rows, all
 * object-identical):
 *   - implicit int sprintf (no decl — no stdio.h) -> ghost at the repair-loop call;
 *   - s32 SLCurBoxNoSet (same as filesel.c)       -> ghost at the box-loop call;
 *   - void SLSaveDataMake / void fn_1_36C4 (their real shapes) -> NO ghosts;
 *   - every other SL* callee void (MP5 saveload.h shapes)      -> NO ghosts.
 * Alternate identical combos: {s32 SLBoxBackupLoad + s32 SLCommonLoad + void sprintf}
 * and {s32 SLBoxBackupLoad + implicit sprintf}; this one is chosen because it agrees
 * with filesel.c's decls and fn_1_36C4's actual void definition.
 * NOTE: multi-use string literals ("SAVE"/"EMPT") must stay extern lbl_1_data_* —
 * inlining them makes MWCC CSE/hoist the pool address into a callee-saved reg
 * (result shifts r29->r28, 95%). */
s32 FileBoxInit(s16 arg)
{
    s32 brokenFlag = 0;
    s16 i;
    s16 winId;
    s32 result;
    u16 *p;
    s32 boxStatus[3];
    OSTime time;

    UnMountCnt = 0;
    if (arg == -1) {
        winId = HuWinWarningCreate(-10000.0f, 160.0f, 478, 94);
    } else {
        winId = arg;
    }
    SLWinIdSet(winId);
    SLCurSlotNoSet(0);
    while (1) {
        result = FileCardLoad();
        if (result == -4) {
            for (i = 0; i < 3; i++) {
                lbl_1_bss_D8[i].unk_0 = 0;
            }
            result = FileCheckCardSpace();
            if (result == 0) {
                break;
            } else {
                goto cardError;
            }
        } else if (result != 0) {
        cardError:
            result = FileCardErrorExec(result, winId);
            if (result == -3 || result == -4 || result == -5) {
                continue;
            }
            if (result == -0x4d2) {
                HuWinWarningOpen(winId);
                HuWinAttrSet(winId, 0x10);
                HuWinMesSet(winId, 0x90025);
                HuWinMesWait(winId);
                result = HuWinChoiceGet(winId, 0);
                HuWinWarningClose(winId);
                if (result != 0) {
                    continue;
                }
                result = -2;
            }
            goto cleanup;
        } else if (strncmp((char *)saveBuf[curSlotNo], "ERASE", 5) == 0) {
            time = OSGetTime();
            SLSaveDataMake(1, &time);
            for (i = 0; i < 3; i++) {
                SLSaveEmptySet(curSlotNo, i);
                lbl_1_bss_D8[i].unk_0 = 0;
            }
            break;
        } else {
            brokenFlag = 0;
            for (i = 0; i < 3; i++) {
                boxStatus[i] = 0;
            }
            p = (u16 *)(saveBuf[curSlotNo] + 0x2040);
            if (*p != SLCheckSumGet(0, 0x2040)) {
                OSReport("IconBanner Area Broken!\n");
                time = OSGetTime();
                SLSaveDataMake(0, &time);
                brokenFlag = 1;
            }
            for (i = 0; i < 3; i++) {
                SLCurBoxNoSet(i);
                if (strncmp((char *)(saveBuf[curSlotNo] + SLBoxDataOffsetGet(i)), "SAVE", 4) == 0 ||
                    strncmp((char *)(saveBuf[curSlotNo] + SLBoxDataOffsetGet(i + 3)), "SAVE", 4) == 0) {
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
                } else if (strncmp((char *)(saveBuf[curSlotNo] + SLBoxDataOffsetGet(i)), "EMPT", 4) != 0 &&
                           strncmp((char *)(saveBuf[curSlotNo] + SLBoxDataOffsetGet(i + 3)), "EMPT", 4) != 0) {
                    boxStatus[i] = 3;
                    SLSaveEmptySet(curSlotNo, i);
                } else if (strncmp((char *)(saveBuf[curSlotNo] + SLBoxDataOffsetGet(i)), "EMPT", 4) == 0 ||
                           strncmp((char *)(saveBuf[curSlotNo] + SLBoxDataOffsetGet(i + 3)), "EMPT", 4) == 0) {
                    SLSaveEmptySet(curSlotNo, i);
                }
                SLCommonLoad();
                if (strncmp((char *)&GwCommon, "SAVE", 4) == 0) {
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
                break;
            }
            HuWinWarningOpen(winId);
            if (boxStatus[0] != 2 && boxStatus[0] != 3 && boxStatus[1] != 2 && boxStatus[1] != 3 &&
                boxStatus[2] != 2 && boxStatus[2] != 3) {
                HuWinInsertMesSet(winId, curSlotNo + 0x90036, 0);
                HuWinMesSet(winId, 0x90034);
                HuWinMesWait(winId);
            } else {
                for (i = 0; i < 3; i++) {
                    char buf[8];
                    sprintf(buf, (char *)(result = (s32)"%d"), i + 1);
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
            result = FileSaveMesOpen(-1, 0x9002A);
        }
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
            if (result != 0) {
                continue;
            }
            result = -2;
            goto cleanup;
        }
        if (result == -1) {
            goto cleanup;
        }
        if (result == -4) {
            continue;
        }
        if (result == -5) {
            continue;
        }
        break;
    }
    result = 0;
    for (i = 0; i < 3; i++) {
        fn_1_36C4(i, i);
    }
cleanup:
    if (arg == -1) {
        HuWinWarningClose(winId);
        SLWinIdSet(-1);
        HuWinWarningKill(winId);
    }
    return result;
}

/* 0xBF9C */
s32 FileCheckCardSpace(void)
{
    s32 result;
    u32 byteNotUsed;
    u32 filesNotUsed;

    result = FileCardMount(curSlotNo);
    if (result < 0) {
        return result;
    }
    result = HuCardSectorSizeGet(curSlotNo);
    if (result < 0 && result != 0x2000) {
        FileMessOut(8);
        return -0x80;
    }
    result = HuCardFreeSpaceGet(curSlotNo, &byteNotUsed, &filesNotUsed);
    if (filesNotUsed == 0 && byteNotUsed < 0xa000) {
        FileMessOut(4);
        return -9;
    }
    if (filesNotUsed == 0) {
        FileMessOut(2);
        return -9;
    }
    if (byteNotUsed < 0xa000) {
        FileMessOut(3);
        return -9;
    }
    return 0;
}

/* 0xC098 */
s32 FileCardWarning(s16 winId)
{
    s32 warnId;
    s32 ret;
    s32 code;
    s32 status;
    u32 filesNotUsed;
    u32 byteNotUsed;

    if (winId == -1) {
        warnId = HuWinWarningCreate(-10000.0f, 160.0f, 478, 94);
    } else {
        warnId = winId;
    }
    SLWinIdSet(warnId);
    ret = HuCardMount(curSlotNo);
    if (ret == 0 && (ret = HuCardOpen(curSlotNo, SLSaveFileName, &curFileInfo)) == 0) {
        ret = 0;
    } else {
        status = FileCardMount(curSlotNo);
        if (status < 0) {
            code = status;
        } else {
            status = HuCardSectorSizeGet(curSlotNo);
            if (status < 0 && status != 0x2000) {
                FileMessOut(8);
                code = -0x80;
            } else {
                status = HuCardFreeSpaceGet(curSlotNo, &byteNotUsed, &filesNotUsed);
                if (filesNotUsed == 0 && byteNotUsed < 0xA000) {
                    FileMessOut(4);
                    code = -9;
                } else if (filesNotUsed == 0) {
                    FileMessOut(2);
                    code = -9;
                } else if (byteNotUsed < 0xA000) {
                    FileMessOut(3);
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
            ret = FileCardErrorExec(ret, warnId);
        }
    }
    if (winId == -1) {
        SLWinIdSet(-1);
        HuWinWarningKill(warnId);
    }
    return ret;
}

/* 0xC278 */
s32 FileTestOpen(void)
{
    s32 ret;

    if ((ret = HuCardMount(curSlotNo)) == 0 &&
        (ret = HuCardOpen(curSlotNo, SLSaveFileName, &curFileInfo)) == 0) {
        return 1;
    }
    return 0;
}

/* 0xC2F0 */
s32 FileCardErrorExec(s32 err, s16 winId)
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
            result = FileCardChoice(0x17, winId);
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
                    result = FileCardFormat(curSlotNo);
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
                FileMessOut(5);
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
            result = FileCardChoice(0x1d, winId);
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
    result = FileCardChoice(0x15, winId);
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
s32 FileCardOpen(const char *fileName)
{
    s32 ret;

    if (SLSaveFlagGet() == 0) {
        return 0;
    }
    ret = FileCardMount(curSlotNo);
    if (ret < 0) {
        return ret;
    }
    ret = HuCardOpen(curSlotNo, fileName, &curFileInfo);
    if (ret == -4) {
        return -4;
    }
    if (ret == -2) {
        FileMessOut(7);
        return -0x80;
    }
    if (ret == -0x80) {
        FileMessOut(1);
        return -0x80;
    }
    if (ret == -3) {
        FileMessOut(0);
        return -3;
    }
    if (ret == -6) {
        ret = HuCardSectorSizeGet(curSlotNo);
        if (ret > 0 && ret != 0x2000) {
            FileMessOut(8);
            return -2;
        }
        UnMountCnt = 0;
        FileMessOut(5);
        return -6;
    }
    return 0;
}

/* 0xC9D0 */
s32 FileCardRead(s32 length, void *addr)
{
    s32 ret;

    if (SLSaveFlagGet() == 0) {
        return 0;
    }
    SLSerialNoGet();
    ret = HuCardRead(&curFileInfo, addr, length, 0);
    if (ret == -3) {
        FileMessOut(0);
    } else if (ret < 0) {
        FileMessOut(1);
    }
    return ret;
}

/* 0xCA58 */
s32 FileCardClose(void)
{
    s32 ret;

    if (SLSaveFlagGet() == 0) {
        return 0;
    }
    ret = HuCardClose(&curFileInfo);
    return ret;
}

/* 0xCAA4 (unused s16 slot param per resolution 2; body uses global curSlotNo) */
/* dont_inline: target keeps `bl FileCardMount` in all callers (CFF4/CDA8/D558 are
   defined after this point and -inline auto would otherwise inline it). */
#pragma dont_inline on
s32 FileCardMount(s16 slot)
{
    s32 ret;

    ret = HuCardMount(curSlotNo);
    if (ret == -2) {
        FileMessOut(7);
        return ret;
    }
    if (ret == -0x80) {
        FileMessOut(1);
        return -0x80;
    }
    if (ret == -3) {
        FileMessOut(0);
        return -3;
    }
    if (ret == -6) {
        ret = HuCardSectorSizeGet(curSlotNo);
        if (ret > 0 && ret != 0x2000) {
            FileMessOut(8);
            return -2;
        }
        UnMountCnt = 0;
        FileMessOut(5);
        return -6;
    }
    ret = HuCardSectorSizeGet(curSlotNo);
    if (ret < 0) {
        FileMessOut(1);
        return ret;
    }
    if (ret != 0x2000) {
        FileMessOut(8);
        return -2;
    }
    return 0;
}
#pragma dont_inline off

/* 0xCBC8 */
s32 FileCardFormat(s16 arg)
{
    s16 ret;
    s16 win;
    OSTime time;

    if (UnMountCnt & (1 << curSlotNo)) {
        FileMessOut(0xc);
        UnMountCnt = 0;
        return 0;
    }
    win = FileCardMesOpen(0x9001b, arg + 0x90036, -1, 0x46);
    HuPrcSleep(0x1e);
    if (UnMountCnt & (1 << curSlotNo)) {
        FileCardMesClose(win);
        FileMessOut(0xc);
        UnMountCnt = 0;
        return 0;
    }
    ret = HuCardFormat(curSlotNo);
    SLSerialNo[curSlotNo] = 0;
    if (ret < 0) {
        FileCardMesClose(win);
    }
    if (ret == -128) {
        FileMessOut(6);
        FileMessOut(1);
        return -128;
    }
    if (ret == -3) {
        FileMessOut(0);
        return -3;
    }
    if (ret == -2) {
        FileMessOut(7);
        return ret;
    }
    SLSerialNoGet();
    FileCardMesClose(win);
    SLCurBoxNoSet(0);
    time = OSGetTime();
    SLSaveDataMake(0, &time);
    SLCheckSumBoxAllSet();
    return ret;
}

/* 0xCDA8 */
s32 FileCardLoad(void)
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
        rc = FileCardMount(curSlotNo);
        if (rc < 0) {
            err = rc;
        } else {
            rc = HuCardOpen(curSlotNo, SLSaveFileName, &curFileInfo);
            if (rc == -4) {
                err = -4;
            } else if (rc == -2) {
                FileMessOut(7);
                err = -0x80;
            } else if (rc == -0x80) {
                FileMessOut(1);
                err = -0x80;
            } else if (rc == -3) {
                FileMessOut(0);
                err = -3;
            } else if (rc == -6) {
                rc = HuCardSectorSizeGet(curSlotNo);
                if (rc > 0 && rc != 0x2000) {
                    FileMessOut(8);
                    err = -2;
                } else {
                    UnMountCnt = 0;
                    FileMessOut(5);
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
                FileMessOut(0);
            } else if (rd < 0) {
                FileMessOut(1);
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
s32 FileCardCopy(const char *fileName, s32 size, void *addr)
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
    ret = FileCardMount(curSlotNo);
    if (ret < 0) {
        return ret;
    }
    ret = HuCardSectorSizeGet(curSlotNo);
    if (ret < 0 && ret != 0x2000) {
        FileMessOut(8);
        return -0x80;
    }
    ret = HuCardFreeSpaceGet(curSlotNo, &byteNotUsed, &filesNotUsed);
    if (filesNotUsed == 0 && size > byteNotUsed) {
        FileMessOut(4);
        return -9;
    }
    if (filesNotUsed == 0) {
        FileMessOut(2);
        return -9;
    }
    if (size > byteNotUsed) {
        FileMessOut(3);
        return -9;
    }
    warnA = FileCardMesOpen(0x90007, curSlotNo + 0x90036, -1, 0xa0);
    warnB = FileStatusMesOpen(0x9002a, curSlotNo + 0x90036, -1, 0x46);
    HuSRDisableF = 1;
    ret = HuCardCreate(curSlotNo, fileName, size, &curFileInfo);
    if (ret < 0) {
        FileCardMesClose(warnA);
        FileCardMesClose(warnB);
        HuSRDisableF = 0;
    }
    if (ret == -3) {
        FileMessOut(0);
        return ret;
    }
    if (ret == -6) {
        FileMessOut(5);
        return ret;
    }
    if (ret < 0) {
        FileMessOut(1);
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
        FileCardMesClose(warnA);
        FileCardMesClose(warnB);
        HuSRDisableF = 0;
    }
    if (ret == -3) {
        FileMessOut(0);
        return ret;
    }
    if (ret == -6) {
        FileMessOut(5);
        return ret;
    }
    if (ret < 0) {
        FileMessOut(1);
        return ret;
    }
    ret = SLStatSet(0);
    HuSRDisableF = 0;
    FileCardMesClose(warnA);
    FileCardMesClose(warnB);
    if (ret < 0) {
        return ret;
    }
    return 0;
}

/* 0xD348 (saveBuf[curSlotNo] per resolution 5) */
s32 FileClear(s16 arg)
{
    s32 winId;
    s32 ret;

    if (arg == -1) {
        winId = HuWinWarningCreate(-10000.0f, 160.0f, 478, 94);
    } else {
        winId = arg;
    }
    SLWinIdSet(winId);
    do {
        ret = FileCardCopy(SLSaveFileName, 0xA000, saveBuf[curSlotNo]);
        if (ret == 0) {
            ret = 0;
            break;
        }
        ret = FileCardErrorExec(ret, winId);
    } while (ret == -3);
    if (arg == -1) {
        SLWinIdSet(-1);
        HuWinWarningKill(winId);
    }
    return ret;
}

/* 0xD448 */
s32 FileCardWrite(s32 length, const void *addr)
{
    s32 winId;
    s32 ret;

    if (SLSaveFlagGet() == 0) {
        return 0;
    }
    if (lbl_1_data_568 != -1) {
        winId = (s16)FileStatusMesOpen(lbl_1_data_568, curSlotNo + 0x90036, -1, 0x46);
    }
    HuSRDisableF = 1;
    HuPrcSleep(0x3c);
    SLSerialNoGet();
    ret = HuCardWriteIdle(&curFileInfo, addr, length, 0);
    if (lbl_1_data_568 != -1) {
        FileStatusMesClose(winId);
    }
    if (ret == 0) {
        ret = SLStatSet(0);
    }
    HuSRDisableF = 0;
    return ret;
}

/* 0xD558 (saveBuf[curSlotNo] per resolution 5) */
s32 FileSave(void)
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
        ret = FileCardMount(curSlotNo);
        if (ret < 0) {
            stat = ret;
        } else {
            ret = HuCardOpen(curSlotNo, SLSaveFileName, &curFileInfo);
            if (ret == -4) {
                stat = -4;
            } else if (ret == -2) {
                FileMessOut(7);
                stat = -0x80;
            } else if (ret == -0x80) {
                FileMessOut(1);
                stat = -0x80;
            } else if (ret == -3) {
                FileMessOut(0);
                stat = -3;
            } else if (ret == -6) {
                ret = HuCardSectorSizeGet(curSlotNo);
                if (ret > 0 && ret != 0x2000) {
                    FileMessOut(8);
                    stat = -2;
                } else {
                    UnMountCnt = 0;
                    FileMessOut(5);
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
            FileMessOut(9);
        } else {
            rv = FileCardCopy(SLSaveFileName, 0xa000, saveBuf[curSlotNo]);
            if (rv >= 0) {
                SLSerialNoGet();
            }
        }
    } else if (rv >= 0) {
        if (SLSerialNoCheck() == 0) {
            FileMessOut(9);
        } else {
            winId1 = (s16)FileCardMesOpen(0x9000b, curSlotNo + 0x90036, -1, 0x46);
            buf = saveBuf[curSlotNo];
            if (SLSaveFlagGet() == 0) {
                wret = 0;
            } else {
                if ((u32)lbl_1_data_568 != 0xffffffff) {
                    winId2 = (s16)FileStatusMesOpen(lbl_1_data_568, curSlotNo + 0x90036, -1, 0x46);
                }
                HuSRDisableF = 1;
                HuPrcSleep(0x3c);
                SLSerialNoGet();
                wstat = HuCardWriteIdle(&curFileInfo, buf, 0xa000, 0);
                if ((u32)lbl_1_data_568 != 0xffffffff) {
                    FileStatusMesClose(winId2);
                }
                if (wstat == 0) {
                    wstat = SLStatSet(0);
                }
                HuSRDisableF = 0;
                wret = wstat;
            }
            rv = wret;
            FileCardMesClose(winId1);
            if (rv == -3) {
                FileMessOut(0);
            } else if (rv == -2) {
                FileMessOut(7);
            } else if (rv == -6) {
                rv = HuCardSectorSizeGet(curSlotNo);
                if (rv > 0 && rv != 0x2000) {
                    FileMessOut(8);
                } else {
                    FileMessOut(5);
                    return -6;
                }
            } else if (rv < 0) {
                FileMessOut(1);
            }
        }
    }
    if (SLSaveFlagGet() != 0) {
        closeRet = HuCardClose(&curFileInfo);
    }
    return rv;
}

/* 0xD90C */
s32 FileSaveMesOpen(s16 winId_in, s32 arg1)
{
    s32 winId;
    s32 ret;

    if (winId_in == -1) {
        winId = HuWinWarningCreate(-10000.0f, 160.0f, 0x1de, 0x5e);
    } else {
        winId = winId_in;
    }
    SLWinIdSet(winId);
    lbl_1_data_568 = arg1;
    do {
        ret = FileSave();
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
        ret = FileCardErrorExec(ret, winId);
    } while (ret == -3);
    if (winId_in == -1) {
        SLWinIdSet(-1);
        HuWinWarningKill(winId);
    }
    lbl_1_data_568 = -1;
    return ret;
}

/* 0xDA18 */
HUWINID FileStatusMesOpen(u32 mesId, u32 insMes0, u32 insMes1, s16 arg3)
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
void FileStatusMesClose(s16 winId)
{
    if (winId >= 0) {
        HuWinWarningClose(winId);
        HuWinWarningKill(winId);
    }
}

/* 0xDBA0 */
HUWINID FileCardMesOpen(u32 messNum, u32 insMesNum1, u32 insMesNum2, s16 posY)
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
        winId = ((HUWINID (*)(f32, f32, int, int))HuWinWarningCreate)(-10000.0f, posY, (int)maxSize.x, (int)maxSize.y);
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
void FileCardMesClose(s16 winId)
{
    if (SLWinId != winId && winId >= 0) {
        HuWinWarningClose(winId);
        HuWinWarningKill(winId);
    }
}

/* 0xDD84 */
s16 FileMessOut(s16 mode)
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
        pos[0] = 478.0f;
        pos[1] = 94.0f;
        warnId = HuWinWarningCreate(-10000.0f, 160.0f, pos[0], pos[1]);
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
s32 FileCardChoice(s16 arg0, s16 arg1)
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
        winId = HuWinWarningCreate(-10000.0f, 160.0f, (s16)maxSize.x, (s16)maxSize.y);
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
