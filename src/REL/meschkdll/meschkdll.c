#include "dolphin.h"
#include "game/gamework.h"
#include "game/object.h"
#include "game/pad.h"
#include "game/process.h"
#include "game/window.h"
#include "game/wipe.h"

typedef void (*VoidFunc)(void);

extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];

static void fn_1_110(void);
static void fn_1_188(void);
static void fn_1_828(u32 messNum, HuVec2f *size);

static OMOBJMAN *objman;

void ObjectSetup(void);

int _prolog(void)
{
    const VoidFunc *ctors = _ctors;

    while (*ctors != 0) {
        (**ctors)();
        ctors++;
    }
    ObjectSetup();
    return 0;
}

void _epilog(void)
{
    const VoidFunc *dtors = _dtors;

    while (*dtors != 0) {
        (**dtors)();
        dtors++;
    }
}

void ObjectSetup(void)
{
    OSReport("******* Message Checker *********\n");
    objman = omInitObjMan(50, 0x2000);
    HuWinInit(0);
    HuPrcChildCreate(fn_1_110, 1000, 0x3000, 0, objman);
}

/* Writable: the checker normalizes '_' and strips any filename suffix in place. */
char *lbl_1_data_494[] = {
    "001_chara_name",
    "002_sys_guide",
    "003_map_name",
    "004_tag_name",
    "005_mgpack_name",
    "006_mg_name",
    "007_staff_name",
    "008_sbank_item",
    "009_mbook_page",
    "010_file_select",
    "012_all_main_menu",
    "013_opening",
    "014_ending",
    "020_party_setting",
    "021_party_results",
    "030_single_setting",
    "031_single_result",
    "040_mgm_main_menu",
    "041_mgm_free",
    "042_mgm_katinuki",
    "043_mgm_tournament",
    "044_mgm_decathlon",
    "045_mgm_renshou",
    "046_mgm_bingo",
    "060_sbank_main_menu",
    "061_option_single",
    "062_option_sound",
    "070_mic_setting",
    "071_micquiz",
    "072_micquiz_i",
    "073_micquiz_q",
    "074_micquiz_tutorial",
    "075_micquiz_moriage",
    "076_quiz_chara",
    "077_micquiz_intro",
    "079_quiz_lv",
    "090_micgo",
    "094_micgosuport",
    "200_Board_ope",
    "201_Board_star",
    "204_Board_pause",
    "206_Board_gate",
    "208_Board_tutorial",
    "209_Board_single",
    "210_Board_opening",
    "211_Board_snpc",
    "215_Board_blast5",
    "220_Board_w01",
    "221_Board_w02",
    "222_Board_w03",
    "223_Board_w04",
    "224_Board_w05",
    "225_Board_w06",
    "230_Capsule_Ex01",
    "231_Capsule_Ex02",
    "234_Capsule_Ex99",
    "235_Capsule_Ex98",
    "241_ShopEvent",
    "242_CapsuleMasu",
    "243_TeresaMasu",
    "244_MiracleMasu",
    "245_KettouMasu",
    "246_DonkeyMasu",
    "247_KoopaMasu",
    "248_mgc_battle",
    "500_mg_inst",
    "598_mg_inst_sys",
    "600_option_mess",
    "647_mess",
    "665_mic_min",
    "677_message",
    "679_mg_mess",
    "LANGUAGE",
    "mic_retyping",
    "saf_test",
    NULL,
};

u32 lbl_1_data_5C4[] = {
    0x00480001,
    0x00480001,
    0x00480004,
    0x00480007,
    0x0048000A,
    0x0048000D,
};

static void fn_1_110(void)
{
    WipeCreate(WIPE_MODE_IN, WIPE_TYPE_NORMAL, 30);
    while (WipeCheck()) {
        HuPrcVSleep();
    }

    fn_1_188();

    WipeCreate(WIPE_MODE_OUT, WIPE_TYPE_NORMAL, 30);
    while (WipeCheck()) {
        HuPrcVSleep();
    }

    omOvlReturnEx(1, 1);
    HuPrcEnd();
    while (TRUE) {
        HuPrcVSleep();
    }
}

static void fn_1_188(void)
{
    HuVec2f size;
    HUWINID titleWin;
    HUWINID dirWin;
    HUWINID langWin;
    HUWINID messWin;
    HUWIN *messWinData;
    char messNoText[8];
    char *name;
    u32 messNum;
    s16 dirNo;
    s16 messNo;
    s16 nextMessNo;
    s16 messMax;
    s16 key;
    s16 dimension;
    BOOL previousDir;
    BOOL hasControl;

    for (dirNo = 0; lbl_1_data_494[dirNo]; dirNo++) {
        name = lbl_1_data_494[dirNo];
        while (*name != '\0') {
            if (*name == '_') {
                *name = '=';
            }
            if (*name == '.') {
                *name = '\0';
            }
            name++;
        }
    }

    titleWin = HuWinCreate(478.0f, 32.0f, 0x52, 0x2A, 0);
    HuWinAttrSet(titleWin, HUWIN_ATTR_ALIGN_CENTER);
    HuWinMesSpeedSet(titleWin, 0);
    HuWinScaleSet(titleWin, 0.8f, 0.8f);

    dirWin = HuWinCreate(24.0f, 32.0f, 0x20A, 0x2A, 0);
    HuWinMesSpeedSet(dirWin, 0);
    HuWinScaleSet(dirWin, 0.8f, 0.8f);

    langWin = HuWinCreate(350.0f, 32.0f, 0x15A, 0x2A, 0);
    HuWinMesSpeedSet(langWin, 0);
    HuWinScaleSet(langWin, 0.8f, 0.8f);
    HuWinBGTPLvlSet(langWin, 0.0f);
    HuWinMesSet(langWin, lbl_1_data_5C4[GwLanguage]);

    nextMessNo = 0;
    dirNo = nextMessNo;
    previousDir = FALSE;
    while (lbl_1_data_494[dirNo]) {
        HuWinMesSet(dirWin, MESSNUM_PTR(lbl_1_data_494[dirNo]));
        messMax = HuWinMesMaxNumGet((u32)dirNo << 16);
        if (!previousDir) {
            nextMessNo = 0;
        } else {
            previousDir = FALSE;
            nextMessNo = messMax - 1;
        }

        for (messNo = nextMessNo; messNo < messMax; messNo++) {
            sprintf(messNoText, "%d", messNo);
            HuWinMesSet(titleWin, MESSNUM_PTR(messNoText));

            messNum = ((u32)dirNo << 16) | messNo;
            fn_1_828(messNum, &size);
            if (size.x == 255.0f) {
                HuWinMesMaxSizeGet(1, &size, messNum);
            } else {
                dimension = size.x;
                size.x = (dimension * 21 + 31) & 0xFFF0;
                dimension = size.y;
                size.y = (dimension * 26 + 31) & 0xFFF0;
            }

            messWin = HuWinCreate(HUWIN_POS_CENTER, 200.0f, size.x, size.y, 0);
            HuWinMesSpeedSet(messWin, 0);

            hasControl = FALSE;
            name = HuWinMesPtrGet(messNum);
            while (*name != '\0') {
                if (*name == 0xFF) {
                    hasControl = TRUE;
                }
                name++;
            }

            messWinData = &winData[messWin];
            messWinData->pushKey = PAD_BUTTON_START | PAD_BUTTON_X | PAD_BUTTON_Y |
                PAD_BUTTON_A | PAD_BUTTON_B | PAD_TRIGGER_L | PAD_TRIGGER_R;
            HuWinMesSet(messWin, messNum);
            while (messWinData->stat != HUWIN_STAT_NONE) {
                HuPadBtnDown[0] = HuPadBtnRep[0];
                HuPrcVSleep();
            }

            if (!hasControl) {
                while (!(HuPadBtnRep[0] &
                    (PAD_BUTTON_START | PAD_BUTTON_X | PAD_BUTTON_Y | PAD_BUTTON_A |
                        PAD_BUTTON_B | PAD_TRIGGER_L | PAD_TRIGGER_R))) {
                    HuPrcVSleep();
                }
                messWinData->activePadKey = HuPadBtnRep[0];
            }

            key = messWinData->activePadKey;
            HuWinKill(messWin);
            if (key & PAD_BUTTON_START) {
                return;
            }
            if (key & PAD_TRIGGER_R) {
                break;
            }
            if (key & PAD_TRIGGER_L) {
                dirNo -= 2;
                if (dirNo < 0) {
                    dirNo = -1;
                }
                break;
            }
            if (key & (PAD_BUTTON_X | PAD_BUTTON_Y)) {
                if (key & PAD_BUTTON_X) {
                    GwLanguage++;
                    if (GwLanguage > 5) {
                        GwLanguage = 1;
                    }
                } else {
                    GwLanguage--;
                    if (GwLanguage <= 0) {
                        GwLanguage = 5;
                    }
                }
                GWLanguageSet(GwLanguage);
                GwCommonOrig.languageNo = GwLanguage;
                HuWinMesLanguageSet(GwLanguage);
                HuWinMesRead();
                HuWinMesSet(langWin, lbl_1_data_5C4[GwLanguage]);
                messNo--;
            } else if (key & PAD_BUTTON_B) {
                messNo -= 2;
                if (messNo < 0) {
                    messNo = -1;
                }
                dirNo -= 2;
                if (dirNo < 0) {
                    dirNo = -1;
                }
                previousDir = TRUE;
                break;
            }
            HuPrcVSleep();
        }

        dirNo++;
        if (!lbl_1_data_494[dirNo]) {
            dirNo = 0;
        }
        HuPrcVSleep();
    }
}

static void fn_1_828(u32 messNum, HuVec2f *size)
{
    u32 dirNo = messNum >> 16;
    u32 entryNo = messNum & 0xFFFF;
    u32 *data = messDataPtr;

    if (dirNo >= data[0]) {
        OSReport("Error: Message Dir Over\n");
    }
    data++;
    data += data[dirNo] >> 2;

    if (entryNo >= data[0]) {
        OSReport("Error: Message Number Over\n");
    }
    data++;
    data += data[entryNo] >> 2;

    size->x = (float)(data[0] >> 16);
    size->y = (float)(data[0] & 0xFFFF);
}
