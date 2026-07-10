#include "dolphin.h"
#include "game/object.h"
#include "game/gamework.h"
#include "game/hu3d.h"
#include "game/audio.h"
#include "game/frand.h"
#include "game/pad.h"
#include "game/data.h"
#include "game/flag.h"
#include "game/wipe.h"
#include "game/printfunc.h"
#include "game/objdll.h"
#include "game/armem.h"
#include "game/memory.h"
#include "msm.h"

#include "math.h"
#include "string.h"

#pragma pool_data off

extern s32 rand8(void);

#define SM_PAGE_MAX 13
#define SM_PAGE_SIZE 10
#define SM_CHAR_MAX 11

#define SM_KEY_LEFT 0x0004
#define SM_KEY_RIGHT 0x0008
#define SM_KEY_UP 0x0001
#define SM_KEY_DOWN 0x0002

#define MG_TYPE_4P 0
#define MG_TYPE_1VS3 1
#define MG_TYPE_2VS2 2
#define MG_TYPE_BATTLE 3
#define MG_TYPE_KUPA 4
#define MG_TYPE_LAST 5
#define MG_TYPE_KETTOU 6

#define MG_FLAG_GRPORDER (1 << 5)

typedef struct MgData_s {
    /* 0x00 */ u16 ovl;
    /* 0x02 */ u8 type;
    /* 0x03 */ u8 unk_3;
    /* 0x04 */ u16 flag;
    /* 0x06 */ u8 unk_6[0x76];
} MGDATA; /* size = 0x7C */

extern MGDATA MgDataTbl[];
extern OMOVL GameMesOvlPrev;
extern s32 MgNoGet(s16 ovlNo);

#define GW_TYPE_MAN 0

typedef void (*VoidFunc)(void);
extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];

typedef struct sm_entry {
    u16 on;
    char *name;
    OMOVL ovl;
} SMEntry;

static char *smCharNameTbl[14] = {
    "Mario",
    "Luigi",
    "Peach",
    "Yoshi",
    "Wario",
    "Daisy",
    "Waluigi",
    "Kinopio",
    "Teresa",
    "Minikoopa",
    "Kinopiko",
    "MinikoopaR",
    "MinikoopaG",
    "MinikoopaB",
};

static SMEntry smPageData[SM_PAGE_MAX * SM_PAGE_SIZE] = {
    { TRUE, "601:PIKATTO HIPDROP", DLL_m601dll },
    { TRUE, "602:HAYAOSI MACHIGAI SAGASI", DLL_m602dll },
    { TRUE, "603:NICE SHOT!", DLL_m603dll },
    { TRUE, "604:PATA2 JUMP", DLL_m604dll },
    { TRUE, "605:NIGERO GORO2", DLL_m605dll },
    { TRUE, "606:JAMASUNNA RACE", DLL_m606dll },
    { TRUE, "607:DOTABATA POST", DLL_m607dll },
    { TRUE, "608:GURU2 SNOW BOARD JUMP", DLL_m608dll },
    { TRUE, "609:TAKI NI OCHIRUZO", DLL_m609dll },
    { TRUE, "610:RADICON RACE", DLL_m610dll },
    { TRUE, "611:DOKI2 SALVAGE", DLL_m611dll },
    { TRUE, "612:ROUTE WO SAGASE", DLL_m612dll },
    { TRUE, "613:KARE2 SIBAKARI", DLL_m613dll },
    { TRUE, "614:HOVERCRAFT BATTLE", DLL_m614dll },
    { TRUE, "615:TATAKE ONPU BALL", DLL_m615dll },
    { TRUE, "616:ERANDE UP", DLL_m616dll },
    { TRUE, "617:TOMOSE ROUSOKU", DLL_m617dll },
    { TRUE, "618:WATATTE LIFT", DLL_m618dll },
    { TRUE, "619:GOKUAKU GESSO", DLL_m619dll },
    { TRUE, "620:KURIBOH HOI2", DLL_m620dll },
    { TRUE, "621:SANBO ROYAL", DLL_m621dll },
    { TRUE, "622:SAGSE COIN BAKO", DLL_m622dll },
    { TRUE, "623:TURUTTO COIN ATUME", DLL_m623dll },
    { TRUE, "624:HAGURUMA WATARI", DLL_m624dll },
    { TRUE, "625:SUICHUU SURVIVAL", DLL_m625dll },
    { TRUE, "626:YUKI GASSEN", DLL_m626dll },
    { TRUE, "627:KOROGASE GORO2 BALL", DLL_m627dll },
    { TRUE, "628:4X4 ATTITTI", DLL_m628dll },
    { TRUE, "629:WATTE FUUSEN", DLL_m629dll },
    { TRUE, "630:3LINE DE UTE", DLL_m630dll },
    { TRUE, "631:GORO2 KAMINARI", DLL_m631dll },
    { TRUE, "632:HAKONIWA PANIC", DLL_m632dll },
    { TRUE, "633:BIRI2 LASER", DLL_m633dll },
    { TRUE, "634:MINNA DE OSOUJI", DLL_m634dll },
    { TRUE, "635:DAICON NUKI", DLL_m635dll },
    { TRUE, "636:DOT DE OEKAKI", DLL_m636dll },
    { TRUE, "637:REEL WO MAWASE", DLL_m637dll },
    { TRUE, "638:GONDOLA RACE", DLL_m638dll },
    { TRUE, "639:FUURYOKU HATUDEN", DLL_m639dll },
    { TRUE, "640:ROBOT FACTORY", DLL_m640dll },
    { TRUE, "641:TATAKE MOGURA", DLL_m641dll },
    { TRUE, "642:SEESAW GAME", DLL_m642dll },
    { TRUE, "643:KILLER NOBORI", DLL_m643dll },
    { TRUE, "644:DOURO KOUJI", DLL_m644dll },
    { TRUE, "645:MADO FUKI", DLL_m645dll },
    { TRUE, "646:MATOATE SCROLL", DLL_m646dll },
    { TRUE, "647:MUSI2 RACE", DLL_m647dll },
    { TRUE, "648:HASTLE SHOT", DLL_m648dll },
    { TRUE, "649:PITTARI STAMP", DLL_m649dll },
    { TRUE, "650:WAN2 RACE", DLL_m650dll },
    { TRUE, "651:BLACK HOLE", DLL_m651dll },
    { TRUE, "652:KUNE2 TSURU2 race", DLL_m652dll },
    { TRUE, "653:RADICON SUMOU", DLL_m653dll },
    { TRUE, "654:ATARI WO MITSUKERO", DLL_m654dll },
    { TRUE, "655:ERANDE TARZAN", DLL_m655dll },
    { TRUE, "656:SUBMARINE DERBY", DLL_m656dll },
    { TRUE, "657:GETSUMEN CHAKURIKU", DLL_m657dll },
    { TRUE, "658:PIKATTO WARP", DLL_m658dll },
    { TRUE, "659:GRID INSEKI YOKE", DLL_m659dll },
    { TRUE, "660:YUUREI GARDEN", DLL_m660dll },
    { TRUE, "661:OIKOME NIGASE", DLL_m661dll },
    { TRUE, "662:NOPPO NO KI", DLL_m662dll },
    { TRUE, "663:AOKI MORI", DLL_m663dll },
    { TRUE, "664:TAIGA BATTLE", DLL_m664dll },
    { TRUE, "665:BOSS BATTLE", DLL_m665dll },
    { TRUE, "666:3HIKI NO HEIHOH", DLL_m666dll },
    { TRUE, "667:MIC DE KAWASITE", DLL_m667dll },
    { TRUE, "668:PYON2 FIGHT", DLL_m668dll },
    { TRUE, "669:KURIBOH WO SUKUE", DLL_m669dll },
    { TRUE, "670:FRUITS VOICE", DLL_m670dll },
    { TRUE, "671:KOOPA NO TEKKYUU", DLL_m671dll },
    { TRUE, "672:KOOPA NO MEMAWASI", DLL_m672dll },
    { TRUE, "673:CRAYAMI DE GAHAHA", DLL_m673dll },
    { TRUE, "674:DONKEY NO TARUJUMP", DLL_m674dll },
    { TRUE, "675:YUSUTTE BANANA", DLL_m675dll },
    { TRUE, "676:KOROGATTE UHOHO", DLL_m676dll },
    { TRUE, "677:KOOPA NO DAIYOGEN", DLL_m677dll },
    { TRUE, "678TUNAGETE BLOCK", DLL_m678dll },
    { TRUE, "679:MEIRO DOUJOH", DLL_m679dll },
    { TRUE, "680:SHUFFLE FRUITS", DLL_m680dll },
    { TRUE, "681:YAJIRUSI TAISO", DLL_m681dll },
    { TRUE, "699:BASKET", DLL_m699dll },
    { FALSE, "***:", DLL_selmenuDLL },
    { FALSE, "***:", DLL_selmenuDLL },
    { FALSE, "***:", DLL_selmenuDLL },
    { FALSE, "***:", DLL_selmenuDLL },
    { FALSE, "***:", DLL_selmenuDLL },
    { FALSE, "***:", DLL_selmenuDLL },
    { FALSE, "***:", DLL_selmenuDLL },
    { FALSE, "***:", DLL_selmenuDLL },
    { TRUE, "***:BOARD W01", DLL_w01dll },
    { TRUE, "***:BOARD W02", DLL_w02dll },
    { TRUE, "***:BOARD W03", DLL_w03dll },
    { TRUE, "***:BOARD W04", DLL_w04dll },
    { TRUE, "***:BOARD W05", DLL_w05dll },
    { TRUE, "***:BOARD W06", DLL_w06dll },
    { TRUE, "***:BOARD W10", DLL_w10dll },
    { FALSE, "***:", DLL_selmenuDLL },
    { FALSE, "***:", DLL_selmenuDLL },
    { FALSE, "***:", DLL_selmenuDLL },
    { TRUE, "***:S01", DLL_s01dll },
    { TRUE, "***:S02", DLL_s02dll },
    { TRUE, "***:S03", DLL_s03dll },
    { TRUE, "***:BOARD W11", DLL_w11dll },
    { FALSE, "***:", DLL_selmenuDLL },
    { FALSE, "***:", DLL_selmenuDLL },
    { FALSE, "***:", DLL_selmenuDLL },
    { FALSE, "***:", DLL_selmenuDLL },
    { FALSE, "***:", DLL_selmenuDLL },
    { FALSE, "***:", DLL_selmenuDLL },
    { TRUE, "***:MODE SEL", DLL_mdseldll },
    { TRUE, "***:PARTY MODE", DLL_mdpartydll },
    { TRUE, "***:SINGLE MODE", DLL_mdsingdll },
    { TRUE, "***:OPTION", DLL_optiondll },
    { TRUE, "***:MINIGAME MODE", DLL_mdminidll },
    { TRUE, "***:MG DECATHRON", DLL_mgmdecathlondll },
    { TRUE, "***:MG FREE PLAY", DLL_mgmfreedll },
    { TRUE, "***:MG BATTLE", DLL_mgmbattledll },
    { TRUE, "***:MG TOURNAMENT", DLL_mgmtournamentdll },
    { TRUE, "***:MIC QUIZE", DLL_micquizdll },
    { TRUE, "***:MG RENSHO", DLL_mgmrenshodll },
    { TRUE, "***:MG BINGO", DLL_mgmbingodll },
    { TRUE, "***:MIRACLE BOOK", DLL_miraclebookdll },
    { TRUE, "***:ENDING", DLL_endingdll },
    { TRUE, "***:filesel", DLL_fileseldll },
    { TRUE, "***:MIC SEL", DLL_micquizseldll },
    { TRUE, "***:BOOT", DLL_bootdll },
    { TRUE, "***:OPTION", DLL_optiondll },
    { TRUE, "***:MIKEACT", DLL_mikeactdll },
    { TRUE, "***:MESS CHECK", DLL_meschkdll },
};

static u32 smCharSelCamBitTbl[] = { HU3D_CAM0, HU3D_CAM1, HU3D_CAM2, HU3D_CAM3 };

static u32 lbl_1_data_844[] = { 0, 1, 2, 3, 4, 5 };

static char *smDvdMusTbl[] = { "sound/mu_016a.dvd", "sound/mu_047a.dvd", "sound/mu_052a.dvd", "sound/mu_054a.dvd", "sound/mu_101a.dvd",
    "sound/mu_108a.dvd", "sound/mu002a.dvd", "" };

/* ---------------- .bss group A ---------------- */
static s16 smPage;
static s16 smCursorNoPrev[SM_PAGE_MAX];
static OMOBJ *smMainObj;
static OMOBJ *smOutViewObj;
static GW_PLAYER_CONF smPlayerConf[4];
static s16 smCursorNo;
static s16 smChar1Prev = -1;
static u16 smPadBtnDown;
static u16 smPadDStk;
static u16 smPadDStkDown;
static u16 smPadBtnDownAll[4];
static u16 smPadDStkAll[4];
static u16 smPadDStkDownAll[4];
static u16 smPadDStkAllPrev[4] = {};

/* forward declarations (address order) */
void ObjectSetup(void);                 /* ObjectSetup */
static void SMCopyConfig(GW_PLAYER_CONF *dst, GW_PLAYER_CONF *src); /* SMCopyConfig */
static void SMBtnRead(void);         /* SMBtnRead */
static void SMPagePrint(void);         /* SMPagePrint */
static void SMPageNoAdd(s16 num);      /* SMPageNoAdd */
static void SMCursorNoAdd(s16 num);      /* SMCursorNoAdd */
static void SMInit(OMOBJ *obj);   /* SMInit */
static void SMMain(OMOBJ *obj);  /* SMMain */
static void SMGroupSet(int pos);     /* SMGroupSet */
static s16 SMCharNoAdd(s16 playerNo, s16 num); /* SMCharNoAdd */
static void SMCharMdlKill(void);        /* SMCharMdlKill */
static void SMCharComSet(void);        /* SMCharComSet */
static void SMCharSelInit(OMOBJ *obj);  /* SMCharSelInit */
static void SMCharSelMain(OMOBJ *obj);  /* SMCharSelMain */
static void SMExit(OMOBJ *obj);  /* SMExit */
static void SMPlayerConfPrint(void);        /* SMPlayerConfPrint */
static void SMPlayerConfInit(OMOBJ *obj);  /* SMPlayerConfInit */
static void SMPlayerConfMain(OMOBJ *obj);  /* SMPlayerConfMain */
static void SMRandMain(OMOBJ *obj);  /* SMRandMain */
static void SMStub(void);        /* SMStub */
static void SMSound3DInit(OMOBJ *obj);  /* SMSound3DInit */
static void SMSound3DExec(OMOBJ *obj);  /* SMSound3DExec */
static void SMSound3DPrint(void);        /* SMSound3DPrint */

int _prolog(void)
{
    const VoidFunc *ctor;

    for (ctor = _ctors; *ctor != 0; ctor++) {
        (*ctor)();
    }

    ObjectSetup();
    return 0;
}

void _epilog(void)
{
    const VoidFunc *dtor;

    for (dtor = _dtors; *dtor != 0; dtor++) {
        (*dtor)();
    }
}

void ObjectSetup(void)
{
    static char *funcId = "SMOBJECTSETUP\n";
    int i;
    OMOBJMAN *objman;
    HuVecF pos, dir;

    OSReport("ObjectSetup:%08x\n", ObjectSetup);
    OSReport("\n\n\n******* SMObjectSetup *********\n");
    OSReport(funcId);
    objman = omInitObjMan(50, 8192);
    for (i = 0; i < GW_PLAYER_MAX; i++) {
        if (GwPlayerConf[i].charNo >= SM_CHAR_MAX) {
            break;
        }
    }
    if (i != GW_PLAYER_MAX) {
        for (i = 0; i < GW_PLAYER_MAX; i++) {
            GwPlayerConf[i].charNo = i;
        }
    }
    smMainObj = omAddObjEx(objman, 100, 0, 0, -1, SMInit);
    smOutViewObj = omAddObjEx(objman, 32730, 0, 0, -1, omOutViewMulti);
    omAddObjEx(objman, 100, 0, 0, -1, SMRandMain);
    smOutViewObj->work[0] = GW_PLAYER_MAX;
    for (i = 0; i < GW_PLAYER_MAX; i++) {
        CRotM[i].x = -20.0f;
        CRotM[i].y = 0.0f;
        CRotM[i].z = 0.0f;
        CenterM[i].x = 0.0f;
        CenterM[i].y = 50.0f;
        CenterM[i].z = 0.0f;
        CZoomM[i] = 500.0f;
        Hu3DCameraCreate(smCharSelCamBitTbl[i]);
        Hu3DCameraPerspectiveSet(smCharSelCamBitTbl[i], 45.0f, 20.0f, 25000.0f, 4.0f / 3.0f);
        Hu3DCameraViewportSet(smCharSelCamBitTbl[i], (i % 2) * 320.0f, (i / 2) * 240.0f, 320.0f, 240.0f, 0.0f, 1.0f);
    }
    Hu3DGLightCreate(0.0f, 0.0f, 100.0f, 0.0f, 0.0f, -1.0f, 255, 255, 255);
    pos.x = 0.0f;
    pos.y = 0.0f;
    pos.z = 200.0f;
    dir.x = dir.y = 0.0f;
    dir.z = -1.0f;
    SetDefLight(&pos, &dir, 255, 255, 255, 64, 64, 64, 255, 255, 255);
    WipeCreate(WIPE_MODE_IN, WIPE_TYPE_NORMAL, 5);
    GwSystem.subGameNo = -1;
    _ClearFlag(0x10000);
    GwSystem.turnNo = 1;
    GwSystem.turnMax = 20;
    HuMemHeapDump(HuMemHeapPtrGet(HEAP_MODEL), -1);
}

static void SMCopyConfig(GW_PLAYER_CONF *dst, GW_PLAYER_CONF *src)
{
    int i;
    for (i = 0; i < 4; i++) {
        *dst++ = *src++;
    }
}

static void SMBtnRead(void)
{
    int i;
    smPadDStk = smPadDStkDown = smPadBtnDown = 0;
    for (i = 0; i < 4; i++) {
        smPadDStkAll[i] = smPadDStkDownAll[i] = smPadBtnDownAll[i] = 0;
        if ((HuPadDStkRep[i] | HuPadBtn[i]) & PAD_BUTTON_LEFT) {
            smPadDStkAll[i] |= SM_KEY_LEFT;
        }
        if ((HuPadDStkRep[i] | HuPadBtn[i]) & PAD_BUTTON_RIGHT) {
            smPadDStkAll[i] |= SM_KEY_RIGHT;
        }
        if ((HuPadDStkRep[i] | HuPadBtn[i]) & PAD_BUTTON_UP) {
            smPadDStkAll[i] |= SM_KEY_UP;
        }
        if ((HuPadDStkRep[i] | HuPadBtn[i]) & PAD_BUTTON_DOWN) {
            smPadDStkAll[i] |= SM_KEY_DOWN;
        }
        smPadBtnDownAll[i] |= HuPadBtnDown[i];
        smPadDStkDownAll[i] = (smPadDStkAll[i] ^ smPadDStkAllPrev[i]) & smPadDStkAll[i];
        smPadDStkAllPrev[i] = smPadDStkAll[i];
        smPadDStk |= smPadDStkAll[i];
        smPadDStkDown |= smPadDStkDownAll[i];
        smPadBtnDown |= smPadBtnDownAll[i];
    }
}

static void SMPagePrint(void)
{
    int i;
    SMEntry *entry;
    fontcolor = FONT_COLOR_YELLOW;
    print8(200, 160, 2.5f, "PAGE:%d/%d", smPage + 1, SM_PAGE_MAX);
    for (i = 0; i < SM_PAGE_SIZE; i++) {
        entry = &smPageData[(smPage * SM_PAGE_SIZE) + i];
        if (entry->on == 1) {
            if (i == smCursorNo) {
                fontcolor = FONT_COLOR_CYAN;
            }
            else {
                fontcolor = FONT_COLOR_GREEN;
            }
        }
        else {
            fontcolor = FONT_COLOR_DARK_GREEN;
        }
        print8(100, (float)((i + 10) * 8) * 2.5f, 2.5f, "%s", entry->name);
    }
}

static void SMPageNoAdd(s16 num)
{
    s16 page;
    int i;
    u16 on;
    s16 pagePrev;
    on = FALSE;
    page = smPage;
    pagePrev = page;
    do {
        page += num;
        if (page >= SM_PAGE_MAX) {
            page = 0;
        }
        else if (page < 0) {
            page = SM_PAGE_MAX - 1;
        }
        for (i = 0; i < SM_PAGE_SIZE; i++) {
            if (smPageData[(page * SM_PAGE_SIZE) + i].on == 1) {
                on = TRUE;
                break;
            }
        }
    } while (!on);
    smPage = page;
    if (pagePrev == -1) {
        smCursorNo = -1;
        SMCursorNoAdd(1);
        smCursorNoPrev[smPage] = smCursorNo;
    }
    else {
        smCursorNoPrev[pagePrev] = smCursorNo;
        smCursorNo = smCursorNoPrev[smPage];
    }
}

static void SMCursorNoAdd(s16 num)
{
    s16 pos;
    pos = smCursorNo;
    do {
        pos += num;
        if (pos >= SM_PAGE_SIZE) {
            pos = 0;
        }
        else if (pos < 0) {
            pos = SM_PAGE_SIZE - 1;
        }
    } while (!smPageData[(smPage * SM_PAGE_SIZE) + pos].on);
    smCursorNo = pos;
    (void)pos;
}

static void SMInit(OMOBJ *obj)
{
    int i, j;
    OSReport("SMinit:%08x\n", SMInit);
    omDLLDBGOut();
    Hu3DBGColorSet(0, 0, 0);
    SMCopyConfig(smPlayerConf, GwPlayerConf);
    for (i = 0; i < GW_PLAYER_MAX; i++) {
        for (j = i + 1; j < GW_PLAYER_MAX; j++) {
            if (smPlayerConf[i].charNo == smPlayerConf[j].charNo) {
                smPlayerConf[j].charNo = SMCharNoAdd(j, -1);
            }
        }
    }
    smPage = -1;
    for (i = 0; i < SM_PAGE_MAX; i++) {
        for (j = 0; j < SM_PAGE_SIZE; j++) {
            if (smPageData[(i * SM_PAGE_SIZE) + j].on == 1) {
                break;
            }
        }
        if (j == SM_PAGE_SIZE) {
            smCursorNoPrev[i] = -1;
        }
        else {
            smCursorNoPrev[i] = j;
        }
    }
    SMPageNoAdd(1);
    if (GameMesOvlPrev != DLL_NONE) {
        for (i = 0; i < SM_PAGE_MAX * SM_PAGE_SIZE; i++) {
            if (GameMesOvlPrev == smPageData[i].ovl && smPageData[i].on == 1) {
                smPage = i / SM_PAGE_SIZE;
                smCursorNo = i % SM_PAGE_SIZE;
                smCursorNoPrev[smPage] = smCursorNo;
                break;
            }
        }
    }
    obj->objFunc = SMMain;
}

static void SMMain(OMOBJ *obj)
{
    SMBtnRead();
    SMPagePrint();
    if (smPadDStkDown & SM_KEY_UP) {
        SMCursorNoAdd(-1);
        SMGroupSet((smPage * SM_PAGE_SIZE) + smCursorNo);
        return;
    }
    if (smPadDStkDown & SM_KEY_DOWN) {
        SMCursorNoAdd(1);
        SMGroupSet((smPage * SM_PAGE_SIZE) + smCursorNo);
        return;
    }
    if (smPadDStkDown & SM_KEY_LEFT) {
        SMPageNoAdd(-1);
        SMGroupSet((smPage * SM_PAGE_SIZE) + smCursorNo);
        return;
    }
    if (smPadDStkDown & SM_KEY_RIGHT) {
        SMPageNoAdd(1);
        SMGroupSet((smPage * SM_PAGE_SIZE) + smCursorNo);
        return;
    }
    if ((smPadBtnDown & PAD_BUTTON_A) || (smPadBtnDown & PAD_BUTTON_START)) {
        obj->objFunc = SMCharSelInit;
        return;
    }
    if (smPadBtnDown & PAD_BUTTON_Y) {
        obj->objFunc = SMPlayerConfInit;
        return;
    }
    if (smPadBtnDown & PAD_TRIGGER_Z) {
        obj->objFunc = SMSound3DInit;
        return;
    }
    if (smPadBtnDown & PAD_BUTTON_X) {
        omOvlReturnEx(0, 1);
    }
}

static void SMGroupSet(int pos)
{
    int i;
    int j;
    OMOVL ovl;
    ovl = smPageData[pos].ovl;
    for (i = 0;; i++) {
        if (MgDataTbl[i].ovl == ovl) {
            break;
        }
        if (MgDataTbl[i].ovl == (u16)DLL_NONE) {
            for (i = 0; i < GW_PLAYER_MAX; i++) {
                smPlayerConf[i].grpNo = i;
            }
            return;
        }
    }
    switch (MgDataTbl[i].type) {
        case MG_TYPE_4P:
        case MG_TYPE_BATTLE:
        case MG_TYPE_KUPA:
        case MG_TYPE_LAST:
        case MG_TYPE_KETTOU:
            for (i = 0; i < GW_PLAYER_MAX; i++) {
                smPlayerConf[i].grpNo = i;
            }
            break;

        case MG_TYPE_1VS3:
            smPlayerConf[0].grpNo = 0;
            for (i = 1; i < GW_PLAYER_MAX; i++) {
                smPlayerConf[i].grpNo = 1;
            }
            break;

        case MG_TYPE_2VS2:
            for (j = 0; j < GW_PLAYER_MAX / 2; j++) {
                smPlayerConf[j].grpNo = 0;
            }
            for (j = GW_PLAYER_MAX / 2; j < GW_PLAYER_MAX; j++) {
                smPlayerConf[j].grpNo = 1;
            }
            if (MgDataTbl[i].flag & MG_FLAG_GRPORDER) {
                for (i = 0; i < GW_PLAYER_MAX; i++) {
                    smPlayerConf[i].grpNo = i;
                }
            }
            break;
    }
}

/* ---------------- .bss group B ---------------- */
static HU3D_MODELID smCharMdlId[SM_CHAR_MAX];
static s16 smCharSelEndF[4];
static s16 smCharOnF[SM_CHAR_MAX];

static int smCharFileTbl[14] = {
    0x00CC0000, 0x00CC0002, 0x00CC0004, 0x00CC0006, 0x00CC0008, 0x00CC000A, 0x00CC000C,
    0x00CC000E, 0x00CC0010, 0x00CC0012, 0x00CC0014, 0x00CC0016, 0x00CC0018, 0x00CC001A,
};

static int smCharMotFileTbl[14] = {
    0x00CC0001, 0x00CC0003, 0x00CC0005, 0x00CC0007, 0x00CC0009, 0x00CC000B, 0x00CC000D,
    0x00CC000F, 0x00CC0011, 0x00CC0013, 0x00CC0015, 0x00CC0017, 0x00CC0019, 0x00CC001B,
};

static s16 SMCharNoAdd(s16 playerNo, s16 num)
{
    int i;
    s16 charNo;
    for (i = 0; i < SM_CHAR_MAX; i++) {
        smCharOnF[i] = 0;
    }
    for (i = 0; i < 4; i++) {
        if (smPlayerConf[i].type) {
            continue;
        }
        smCharOnF[smPlayerConf[i].charNo] = 1;
    }
    if (!num) {
        return;
    }
    charNo = smPlayerConf[playerNo].charNo;
    do {
        charNo += num;
        if (charNo < 0) {
            charNo = SM_CHAR_MAX - 1;
        }
        if (charNo >= SM_CHAR_MAX) {
            charNo = 0;
        }
    } while (smCharOnF[charNo] != 0);
    return charNo;
}

static void SMCharMdlKill(void)
{
    int i;
    for (i = 0; i < SM_CHAR_MAX; i++) {
        Hu3DModelKill(smCharMdlId[i]);
    }
}

static void SMCharComSet(void)
{
    int i;
    int j;
    int num;
    int charNo[SM_CHAR_MAX];
    smCharOnF[4] = 1;
    for (i = 0; i < 4; i++) {
        num = 0;
        for (j = 0; j < SM_CHAR_MAX; j++) {
            if (smCharOnF[j]) {
                continue;
            }
            charNo[num++] = j;
        }
        if (smPlayerConf[i].type == 1) {
            smPlayerConf[i].charNo = charNo[rand8() % num];
            smCharOnF[smPlayerConf[i].charNo] = 1;
        }
    }
}

static void SMCharSelInit(OMOBJ *obj)
{
    int i;
    for (i = 0; i < SM_CHAR_MAX; i++) {
        void *data = HuDataSelHeapReadNum(smCharFileTbl[i], HU_MEMNUM_OVL, HEAP_MODEL);
        smCharMdlId[i] = Hu3DModelCreate(data);
        Hu3DModelPosSet(smCharMdlId[i], 0.0f, 0.0f, 0.0f);
        Hu3DModelAttrSet(smCharMdlId[i], HU3D_MOTATTR_LOOP);
        Hu3DMotionSet(smCharMdlId[i], Hu3DJointMotionData(smCharMdlId[i], smCharMotFileTbl[i]));
    }
    for (i = 0; i < GW_PLAYER_MAX; i++) {
        smCharSelEndF[i] = FALSE;
    }
    SMCharNoAdd(0, 0);
    obj->objFunc = SMCharSelMain;
}

static void SMCharSelMain(OMOBJ *obj)
{
    int i;

    s16 port;
    int j;
    s16 w;
    s16 x;
    s16 y;
    s16 manDoneNum;
    s16 manNum;

    for (i = 0; i < SM_CHAR_MAX; i++) {
        Hu3DModelAttrSet(smCharMdlId[i], HU3D_ATTR_DISPOFF);
    }
    for (i = 0; i < GW_PLAYER_MAX; i++) {
        if (smPlayerConf[i].type == GW_TYPE_MAN) {
            Hu3DModelAttrReset(smCharMdlId[smPlayerConf[i].charNo], HU3D_ATTR_DISPOFF);
            Hu3DModelCameraSet(smCharMdlId[smPlayerConf[i].charNo], smCharSelCamBitTbl[i]);
        }
    }
    for (i = 0, manDoneNum = 0, manNum = 0; i < GW_PLAYER_MAX; i++) {
        if (smCharSelEndF[i] == TRUE) {
            manDoneNum++;
        }
        if (smPlayerConf[i].type == GW_TYPE_MAN) {
            manNum++;
        }
    }
    if (manDoneNum == manNum) {
        SMCharNoAdd(0, 0);
        SMCharComSet();
        smPlayerConf[0].charNo = 4;
        smPlayerConf[1].charNo = 11;
        smPlayerConf[2].charNo = 12;
        smPlayerConf[3].charNo = 13;
        SMCopyConfig(GwPlayerConf, smPlayerConf);
        CharDataClose(-1);
        OSReport("%d,%d,%d,%d\n", GwPlayerConf[0].charNo, GwPlayerConf[1].charNo, GwPlayerConf[2].charNo, GwPlayerConf[3].charNo);
        if (!CharMotionAMemPGet(GwPlayerConf[0].charNo)) {
            CharMotionInit(GwPlayerConf[0].charNo);
        }
        if (!CharMotionAMemPGet(GwPlayerConf[1].charNo)) {
            CharMotionInit(GwPlayerConf[1].charNo);
        }
        if (!CharMotionAMemPGet(GwPlayerConf[2].charNo)) {
            CharMotionInit(GwPlayerConf[2].charNo);
        }
        if (!CharMotionAMemPGet(GwPlayerConf[3].charNo)) {
            CharMotionInit(GwPlayerConf[3].charNo);
        }
        WipeCreate(WIPE_MODE_OUT, WIPE_TYPE_NORMAL, 20);
        obj->objFunc = SMExit;
    }
    SMBtnRead();
    for (i = 0; i < GW_PLAYER_MAX; i++) {
        port = smPlayerConf[i].padNo;
        if (!smCharSelEndF[i]) {
            if (smPadDStkDownAll[port] & SM_KEY_LEFT) {
                smPlayerConf[port].charNo = SMCharNoAdd(i, -1);
            }
            if (smPadDStkDownAll[port] & SM_KEY_RIGHT) {
                smPlayerConf[port].charNo = SMCharNoAdd(i, 1);
            }
            if (smPadBtnDownAll[port] & PAD_BUTTON_A) {
                smCharSelEndF[i] = TRUE;
            }
        }
        if (HuPadBtnDown[port] & PAD_BUTTON_B) {
            if (manDoneNum != 0) {
                smCharSelEndF[i] = 0;
            }
            else {
                SMCharMdlKill();
                obj->objFunc = SMMain;
                return;
            }
        }
        if (smPadBtnDownAll[port] & PAD_BUTTON_START) {
            for (j = 0; j < GW_PLAYER_MAX; j++) {
                if (!smPlayerConf[j].type) {
                    smCharSelEndF[j] = TRUE;
                }
            }
            return;
        }
        if (!smPlayerConf[i].type) {
            w = strlen(smCharNameTbl[smPlayerConf[i].charNo]);
            w *= 16;
            x = ((320 - w) / 2) + ((i % 2) * 320);
            y = 176 + ((i / 2) * 240);
            if (smCharSelEndF[i] == TRUE) {
                fontcolor = FONT_COLOR_WHITE;
                print8(x, y, 2.0f,
                    "\xFD\x08"
                    "%s",
                    smCharNameTbl[smPlayerConf[i].charNo]);
            }
            else {
                fontcolor = FONT_COLOR_GREEN;
                print8(x, y, 2.0f,
                    "\xFD\x05"
                    "%s",
                    smCharNameTbl[smPlayerConf[i].charNo]);
            }
        }
        else {
            w = 72;
            x = ((320 - w) / 2) + ((i % 2) * 320);
            y = 108 + ((i / 2) * 240);
            fontcolor = FONT_COLOR_YELLOW;
            print8(x, y, 3.0f,
                "\xFD\x07"
                "COM");
        }
    }
}

static void SMExit(OMOBJ *obj)
{
    int mg;
    if (WipeCheck()) {
        return;
    }
    while (HuARDMACheck())
        ;
    mg = MgNoGet(smPageData[(smPage * SM_PAGE_SIZE) + smCursorNo].ovl);
    GwSystem.mgNo = mg;
    OSReport("mgNo=%d\n", mg);
    _ClearFlag(0x1000E);
    _ClearFlag(0x30002);
    _SetFlag(0x5);
    if (HuPadBtn[0] & PAD_BUTTON_A) {
        GwMgNightF = 1;
    }
    else {
        GwMgNightF = 0;
    }
    if (mg == -1) {
        omOvlCallEx(smPageData[(smPage * SM_PAGE_SIZE) + smCursorNo].ovl, 1, 0, 0);
    }
    else {
        omOvlCallEx(DLL_instdll, 1, 0, 0);
    }
    GameMesOvlPrev = smPageData[(smPage * SM_PAGE_SIZE) + smCursorNo].ovl;
    smChar1Prev = smCursorNo;
}

static char *smPlayerConfStrTbl[] = { "PLAYER:%d", " PAD%d:%s", "PADNO:%d", "  GRP:%d", "  DIF:%s" };

/* ---------------- .bss group C ---------------- */
static s16 smPlayerConfEditF;
static s16 smPlayerConfNo;
static s16 smPlayerConfChoiceNo;

#define DO_HILITE(option)                                                                                                                              \
    do {                                                                                                                                               \
        s32 color;                                                                                                                                     \
        if (smPlayerConfChoiceNo == option) {                                                                                                          \
            color = fontcolor = FONT_COLOR_CYAN;                                                                                                        \
        }                                                                                                                                              \
        else {                                                                                                                                         \
            color = fontcolor = FONT_COLOR_GREEN;                                                                                                       \
        }                                                                                                                                              \
        (void)color;                                                                                                                                   \
    } while (0)

static void SMPlayerConfPrint(void)
{
    int i;
    s16 x;
    s16 y;
    s16 w;
    char *typeStr[] = { "HUMAN", "COM" };
    char *diffStr[] = { "EASY", "NORMAL", "HARD", "VERYHARD" };

    for (i = 0; i < GW_PLAYER_MAX; i++) {
        w = 128;
        x = ((320 - w) / 2) + ((i % 2) * 320);
        y = ((i / 2) * 240) + 48;
        if (smPlayerConfEditF == 0 && smPlayerConfNo == i) {
            fontcolor = FONT_COLOR_CYAN;
        }
        else {
            fontcolor = FONT_COLOR_GREEN;
        }
        print8(x, y, 2.0f, smPlayerConfStrTbl[0], i);
        if (smPlayerConfEditF == 0) {
            fontcolor = FONT_COLOR_DARK_GREEN;
            print8(((i % 2) * 320) + 64, ((i / 2) * 240) + 80, 2.0f, smPlayerConfStrTbl[1], smPlayerConf[i].padNo, typeStr[smPlayerConf[i].type]);
            print8(((i % 2) * 320) + 64, ((i / 2) * 240) + 96, 2.0f, smPlayerConfStrTbl[2], smPlayerConf[i].padNo);
            print8(((i % 2) * 320) + 64, ((i / 2) * 240) + 112, 2.0f, smPlayerConfStrTbl[3], smPlayerConf[i].grpNo);
            print8(((i % 2) * 320) + 64, ((i / 2) * 240) + 128, 2.0f, smPlayerConfStrTbl[4], diffStr[smPlayerConf[i].comDif]);
        }
        else if (smPlayerConfEditF == 1 && smPlayerConfNo == i) {
            DO_HILITE(0);
            print8(((i % 2) * 320) + 64, ((i / 2) * 240) + 80, 2.0f, smPlayerConfStrTbl[1], smPlayerConf[i].padNo, typeStr[smPlayerConf[i].type]);
            DO_HILITE(1);
            print8(((i % 2) * 320) + 64, ((i / 2) * 240) + 96, 2.0f, smPlayerConfStrTbl[2], smPlayerConf[i].padNo);
            DO_HILITE(2);
            print8(((i % 2) * 320) + 64, ((i / 2) * 240) + 112, 2.0f, smPlayerConfStrTbl[3], smPlayerConf[i].grpNo);
            DO_HILITE(3);
            print8(((i % 2) * 320) + 64, ((i / 2) * 240) + 128, 2.0f, smPlayerConfStrTbl[4], diffStr[smPlayerConf[i].comDif]);
        }
        else {
            fontcolor = FONT_COLOR_DARK_GREEN;
            print8(((i % 2) * 320) + 64, ((i / 2) * 240) + 80, 2.0f, smPlayerConfStrTbl[1], smPlayerConf[i].padNo, typeStr[smPlayerConf[i].type]);
            print8(((i % 2) * 320) + 64, ((i / 2) * 240) + 96, 2.0f, smPlayerConfStrTbl[2], smPlayerConf[i].padNo);
            print8(((i % 2) * 320) + 64, ((i / 2) * 240) + 112, 2.0f, smPlayerConfStrTbl[3], smPlayerConf[i].grpNo);
            print8(((i % 2) * 320) + 64, ((i / 2) * 240) + 128, 2.0f, smPlayerConfStrTbl[4], diffStr[smPlayerConf[i].comDif]);
        }
    }
}

#undef DO_HILITE

static void SMPlayerConfInit(OMOBJ *obj)
{
    smPlayerConfEditF = 0;
    smPlayerConfNo = 0;
    smPlayerConfChoiceNo = 0;
    obj->objFunc = SMPlayerConfMain;
}

static void SMPlayerConfMain(OMOBJ *obj)
{
    int offset;
    SMPlayerConfPrint();
    SMBtnRead();
    if (!smPlayerConfEditF) {
        if (smPadDStkDown & SM_KEY_LEFT) {
            offset = ((smPlayerConfNo & 0x1) - 1) & 0x1;
            smPlayerConfNo = (smPlayerConfNo & 0xFFFE) + offset;
        }
        else {
            if (smPadDStkDown & SM_KEY_RIGHT) {
                offset = ((smPlayerConfNo & 0x1) + 1) & 0x1;
                smPlayerConfNo = (smPlayerConfNo & 0xFFFE) + offset;
            }
            else if (smPadDStkDown & SM_KEY_DOWN) {
                if ((smPlayerConfNo += 2) >= 4) {
                    smPlayerConfNo -= 4;
                }
            }
            else if (smPadDStkDown & SM_KEY_UP) {
                if ((smPlayerConfNo -= 2) < 0) {
                    smPlayerConfNo += 4;
                }
            }
        }
        if (smPadBtnDown & PAD_BUTTON_A) {
            smPlayerConfEditF = 1;
            return;
        }
        if ((smPadBtnDown & PAD_BUTTON_B) || (smPadBtnDown & PAD_BUTTON_Y)) {
            obj->objFunc = SMMain;
            return;
        }
        if (smPadBtnDown & PAD_BUTTON_START) {
            int i;
            for (i = 0; i < 100; i++) {
                int order1 = frandmod(GW_PLAYER_MAX);
                int order2 = frandmod(GW_PLAYER_MAX);
                int temp = smPlayerConf[order1].grpNo;
                smPlayerConf[order1].grpNo = smPlayerConf[order2].grpNo;
                smPlayerConf[order2].grpNo = temp;
            }
        }
    }
    else if (smPadDStkDown & SM_KEY_UP) {
        if (--smPlayerConfChoiceNo < 0) {
            smPlayerConfChoiceNo = 3;
        }
    }
    else if (smPadDStkDown & SM_KEY_DOWN) {
        if (++smPlayerConfChoiceNo > 3) {
            smPlayerConfChoiceNo = 0;
        }
    }
    else if (smPadDStkDown & SM_KEY_LEFT) {
        switch (smPlayerConfChoiceNo) {
            case 0:
                smPlayerConf[smPlayerConfNo].type ^= 1;
                break;

            case 1:
                if (--smPlayerConf[smPlayerConfNo].padNo < 0) {
                    smPlayerConf[smPlayerConfNo].padNo = 3;
                }
                break;

            case 2:
                if (--smPlayerConf[smPlayerConfNo].grpNo < 0) {
                    smPlayerConf[smPlayerConfNo].grpNo = 3;
                }
                break;

            case 3:
                if (--smPlayerConf[smPlayerConfNo].comDif < 0) {
                    smPlayerConf[smPlayerConfNo].comDif = 3;
                }
                break;
        };
    }
    else if (smPadDStkDown & SM_KEY_RIGHT) {
        switch (smPlayerConfChoiceNo) {
            case 0:
                smPlayerConf[smPlayerConfNo].type ^= 1;
                break;

            case 1:
                if (++smPlayerConf[smPlayerConfNo].padNo > 3) {
                    smPlayerConf[smPlayerConfNo].padNo = 0;
                }
                break;

            case 2:
                if (++smPlayerConf[smPlayerConfNo].grpNo > 3) {
                    smPlayerConf[smPlayerConfNo].grpNo = 0;
                }
                break;

            case 3:
                if (++smPlayerConf[smPlayerConfNo].comDif > 3) {
                    smPlayerConf[smPlayerConfNo].comDif = 0;
                }
                break;
        }
    }
    else if ((smPadBtnDown & PAD_BUTTON_A) || (smPadBtnDown & PAD_BUTTON_B)) {
        smPlayerConfEditF = 0;
    }
    else if (smPadBtnDown & PAD_BUTTON_Y) {
        obj->objFunc = SMMain;
    }
}

static void SMRandMain(OMOBJ *obj)
{
    rand8();
}

static void SMStub(void) {}

/* ---------------- .bss group D ---------------- */
static s16 smEmiCompDataNo;
static s16 smEmiCompVal;
static s16 smSound3DNo;
s16 lbl_1_bss_0;

static void SMSound3DInit(OMOBJ *obj)
{
    MSMSE *se = msmSeGetIndexPtr(smEmiCompDataNo);
    smEmiCompVal = se->comp;
    obj->objFunc = SMSound3DExec;
}

static void SMSound3DExec(OMOBJ *obj)
{
    float speed;
    MSMSE *se;

    SMBtnRead();
    if (smPadDStkDown & SM_KEY_UP) {
        smSound3DNo--;
        if (smSound3DNo < 0) {
            smSound3DNo = 7;
        }
    }
    if (smPadDStkDown & SM_KEY_DOWN) {
        smSound3DNo++;
        if (smSound3DNo >= 8) {
            smSound3DNo = 0;
        }
    }
    if (smPadDStkDown & (SM_KEY_LEFT | SM_KEY_RIGHT)) {
        if (smPadDStkDown & SM_KEY_RIGHT) {
            speed = 10;
        }
        else {
            speed = -10;
        }
        if (HuPadBtn[0] & PAD_TRIGGER_Z) {
            speed *= 10.0f;
        }
        switch (smSound3DNo) {
            case 0:
                Snd3DDistOffset += speed;
                break;

            case 1:
                Snd3DSpeedOffset += speed;
                break;

            case 2:
                Snd3DStartDisOffset += speed;
                break;

            case 3:
                Snd3DFrontSurDisOffset += speed;
                break;

            case 4:
                Snd3DBackSurDisOffset += speed;
                break;

            case 5:
                smEmiCompDataNo += speed / 10.0f;
                if (smEmiCompDataNo < 0) {
                    smEmiCompDataNo = 0;
                }
                se = msmSeGetIndexPtr(smEmiCompDataNo);
                smEmiCompVal = se->comp;
                break;

            case 6:
                smEmiCompVal += speed / 10.0f;
                if (smEmiCompVal > 127) {
                    smEmiCompVal = 127;
                }
                if (smEmiCompVal < -127) {
                    smEmiCompVal = -127;
                }
                se = msmSeGetIndexPtr(smEmiCompDataNo);
                se->comp = smEmiCompVal;
                break;

            case 7:
                musicOffF = (musicOffF) ? 0 : 1;
                if (musicOffF) {
                    msmMusSetMasterVolume(0);
                }
                else {
                    msmMusSetMasterVolume(127);
                }
                break;
        }
    }
    if (smPadBtnDown & PAD_BUTTON_START) {
        switch (smSound3DNo) {
            case 0:
                Snd3DDistOffset = 0;
                break;

            case 1:
                Snd3DSpeedOffset = 0;
                break;

            case 2:
                Snd3DStartDisOffset = 0;
                break;

            case 3:
                Snd3DFrontSurDisOffset = 0;
                break;

            case 4:
                Snd3DBackSurDisOffset = 0;
                break;

            case 5:
                smEmiCompDataNo = 0;
                break;

            case 6:
                smEmiCompVal = 0;
                break;

            case 7:
                musicOffF = 0;
                break;
        }
    }

    if (smPadBtnDown & PAD_BUTTON_B) {
        obj->objFunc = SMMain;
    }
    SMSound3DPrint();
}

#define DO_HILITE(pos)                                                                                                                                 \
    do {                                                                                                                                               \
        if (smSound3DNo == pos) {                                                                                                                      \
            fontcolor = FONT_COLOR_GREEN;                                                                                                              \
        }                                                                                                                                              \
        else {                                                                                                                                         \
            fontcolor = FONT_COLOR_DARK_GREEN;                                                                                                         \
        }                                                                                                                                              \
    } while (0)

static void SMSound3DPrint(void)
{
    char *onOffStr[] = { " ON", "OFF" };
    fontcolor = FONT_COLOR_YELLOW;
    print8(200, 64, 2.0f, "3DSound Config.");
    DO_HILITE(0);
    print8(140, 96, 2.0f, "Max Distance   %5.1f", Snd3DDistOffset);
    DO_HILITE(1);
    print8(140, 112, 2.0f, "Sound Speed    %5.1f", Snd3DSpeedOffset);
    DO_HILITE(2);
    print8(140, 128, 2.0f, "Start Distance %5.1f", Snd3DStartDisOffset);
    DO_HILITE(3);
    print8(140, 144, 2.0f, "Front Distance %5.1f", Snd3DFrontSurDisOffset);
    DO_HILITE(4);
    print8(140, 160, 2.0f, "Back Distance  %5.1f", Snd3DBackSurDisOffset);
    DO_HILITE(5);
    print8(140, 176, 2.0f, "emiComp DataNo  %04d", smEmiCompDataNo);
    DO_HILITE(6);
    print8(140, 192, 2.0f, "emiComp VAL      %3d", smEmiCompVal);
    DO_HILITE(7);
    print8(140, 208, 2.0f, "Music            %s", onOffStr[(musicOffF) ? 1 : 0]);
}

#undef DO_HILITE
