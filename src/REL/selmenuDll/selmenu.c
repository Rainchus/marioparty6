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

typedef void (*VoidFunc)(void);
extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];

typedef struct sm_entry {
    u16 on;
    char *name;
    OMOVL ovl;
} SMEntry;

static SMEntry smPageData[SM_PAGE_MAX * SM_PAGE_SIZE] = {
    { TRUE, "601:PIKATTO HIPDROP", DLL_m601dll },
    { TRUE, "602:HAYAOSI MACHIGAI SAGASI", DLL_m602dll },
    { TRUE, "603:NICE SHOT!", DLL_m603dll },
    { TRUE, "604:PATA2 JUMP", DLL_m604dll },
    { TRUE, "605:NIGERO GORO2", DLL_m605dll },
    { TRUE, "606:JAMASUNNA RACE", DLL_m606dll },
    { TRUE, "607:DOTABATA POS", DLL_m607dll },
    { TRUE, "608:GURU2 SNOW BOARD JUM", DLL_m608dll },
    { TRUE, "609:TAKI NI OCHIRUZO", DLL_m609dll },
    { TRUE, "610:RADICON RACE", DLL_m610dll },
    { TRUE, "611:DOKI2 SALVAG", DLL_m611dll },
    { TRUE, "612:ROUTE WO SAGASE", DLL_m612dll },
    { TRUE, "613:KARE2 SIBAKARI", DLL_m613dll },
    { TRUE, "614:HOVERCRAFT BATTL", DLL_m614dll },
    { TRUE, "615:TATAKE ONPU BALL", DLL_m615dll },
    { TRUE, "616:ERANDE U", DLL_m616dll },
    { TRUE, "617:TOMOSE ROUSOKU", DLL_m617dll },
    { TRUE, "618:WATATTE LIFT", DLL_m618dll },
    { TRUE, "619:GOKUAKU GESS", DLL_m619dll },
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
    { TRUE, "636:DOT DE OEKAK", DLL_m636dll },
    { TRUE, "637:REEL WO MAWASE", DLL_m637dll },
    { TRUE, "638:GONDOLA RACE", DLL_m638dll },
    { TRUE, "639:FUURYOKU HATUDEN", DLL_m639dll },
    { TRUE, "640:ROBOT FACTOR", DLL_m640dll },
    { TRUE, "641:TATAKE MOGUR", DLL_m641dll },
    { TRUE, "642:SEESAW GAME", DLL_m642dll },
    { TRUE, "643:KILLER NOBOR", DLL_m643dll },
    { TRUE, "644:DOURO KOUJI", DLL_m644dll },
    { TRUE, "645:MADO FUK", DLL_m645dll },
    { TRUE, "646:MATOATE SCROLL", DLL_m646dll },
    { TRUE, "647:MUSI2 RACE", DLL_m647dll },
    { TRUE, "648:HASTLE SHOT", DLL_m648dll },
    { TRUE, "649:PITTARI STAM", DLL_m649dll },
    { TRUE, "650:WAN2 RAC", DLL_m650dll },
    { TRUE, "651:BLACK HOLE", DLL_m651dll },
    { TRUE, "652:KUNE2 TSURU2 rac", DLL_m652dll },
    { TRUE, "653:RADICON SUMO", DLL_m653dll },
    { TRUE, "654:ATARI WO MITSUKERO", DLL_m654dll },
    { TRUE, "655:ERANDE TARZA", DLL_m655dll },
    { TRUE, "656:SUBMARINE DERBY", DLL_m656dll },
    { TRUE, "657:GETSUMEN CHAKURIKU", DLL_m657dll },
    { TRUE, "658:PIKATTO WARP", DLL_m658dll },
    { TRUE, "659:GRID INSEKI YOKE", DLL_m659dll },
    { TRUE, "660:YUUREI GARDE", DLL_m660dll },
    { TRUE, "661:OIKOME NIGAS", DLL_m661dll },
    { TRUE, "662:NOPPO NO KI", DLL_m662dll },
    { TRUE, "663:AOKI MOR", DLL_m663dll },
    { TRUE, "664:TAIGA BATTLE", DLL_m664dll },
    { TRUE, "665:BOSS BATTLE", DLL_m665dll },
    { TRUE, "666:3HIKI NO HEIHOH", DLL_m666dll },
    { TRUE, "667:MIC DE KAWASITE", DLL_m667dll },
    { TRUE, "668:PYON2 FIGHT", DLL_m668dll },
    { TRUE, "669:KURIBOH WO SUKUE", DLL_m669dll },
    { TRUE, "670:FRUITS VOICE", DLL_m670dll },
    { TRUE, "671:KOOPA NO TEKKYUU", DLL_m671dll },
    { TRUE, "672:KOOPA NO MEMAWAS", DLL_m672dll },
    { TRUE, "673:CRAYAMI DE GAHAH", DLL_m673dll },
    { TRUE, "674:DONKEY NO TARUJUMP", DLL_m674dll },
    { TRUE, "675:YUSUTTE BANANA", DLL_m675dll },
    { TRUE, "676:KOROGATTE UHOHO", DLL_m676dll },
    { TRUE, "677:KOOPA NO DAIYOGE", DLL_m677dll },
    { TRUE, "678TUNAGETE BLOC", DLL_m678dll },
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
    { TRUE, "***:BOARD W0", DLL_w01dll },
    { TRUE, "***:BOARD W0", DLL_w02dll },
    { TRUE, "***:BOARD W0", DLL_w03dll },
    { TRUE, "***:BOARD W0", DLL_w04dll },
    { TRUE, "***:BOARD W0", DLL_w05dll },
    { TRUE, "***:BOARD W0", DLL_w06dll },
    { TRUE, "***:BOARD W1", DLL_w10dll },
    { FALSE, "***:", DLL_selmenuDLL },
    { FALSE, "***:", DLL_selmenuDLL },
    { FALSE, "***:", DLL_selmenuDLL },
    { TRUE, "***:S01", DLL_s01dll },
    { TRUE, "***:S02", DLL_s02dll },
    { TRUE, "***:S03", DLL_s03dll },
    { TRUE, "***:BOARD W1", DLL_w11dll },
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
    { TRUE, "***:MINIGAME MOD", DLL_mdminidll },
    { TRUE, "***:MG DECATHRON", DLL_mgmdecathlondll },
    { TRUE, "***:MG FREE PLAY", DLL_mgmfreedll },
    { TRUE, "***:MG BATTL", DLL_mgmbattledll },
    { TRUE, "***:MG TOURNAMEN", DLL_mgmtournamentdll },
    { TRUE, "***:MIC QUIZ", DLL_micquizdll },
    { TRUE, "***:MG RENSH", DLL_mgmrenshodll },
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
void fn_1_A0(void);                 /* ObjectSetup */
static void fn_1_568(GW_PLAYER_CONF *dst, GW_PLAYER_CONF *src); /* SMCopyConfig */
static void fn_1_5C8(void);         /* SMBtnRead */
static void fn_1_8EC(void);         /* SMPagePrint */
static void fn_1_A5C(s16 num);      /* SMPageNoAdd */
static void fn_1_BDC(s16 num);      /* SMCursorNoAdd */
static void fn_1_C64(OMOBJ *obj);   /* SMInit */
static void fn_1_10D8(OMOBJ *obj);  /* SMMain */
static void fn_1_1B50(int pos);     /* SMGroupSet */
static s16 fn_1_1D4C(s16 playerNo, s16 num); /* SMCharNoAdd */
static void fn_1_1E5C(void);        /* SMCharMdlKill */
static void fn_1_1EAC(void);        /* SMCharComSet */
static void fn_1_1FC8(OMOBJ *obj);  /* SMCharSelInit */
static void fn_1_21C0(OMOBJ *obj);  /* SMCharSelMain */
static void fn_1_2F80(OMOBJ *obj);  /* SMExit */
static void fn_1_313C(void);        /* SMPlayerConfPrint */
static void fn_1_3940(OMOBJ *obj);  /* SMPlayerConfInit */
static void fn_1_3980(OMOBJ *obj);  /* SMPlayerConfMain */
static void fn_1_4380(OMOBJ *obj);  /* SMRandMain */
static void fn_1_43A0(void);        /* SMStub */
static void fn_1_43A4(OMOBJ *obj);  /* SMSound3DInit */
static void fn_1_4408(OMOBJ *obj);  /* SMSound3DExec */
static void fn_1_4C5C(void);        /* SMSound3DPrint */
static void fn_1_502C(void);

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

void fn_1_A0(void)
{
    static char *funcId = "SMOBJECTSETUP\n";
    int i;
    OMOBJMAN *objman;
    HuVecF pos, dir;

    OSReport("ObjectSetup:%08x\n", fn_1_A0);
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
    smMainObj = omAddObjEx(objman, 100, 0, 0, -1, fn_1_C64);
    smOutViewObj = omAddObjEx(objman, 32730, 0, 0, -1, omOutViewMulti);
    omAddObjEx(objman, 100, 0, 0, -1, fn_1_4380);
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

static void fn_1_568(GW_PLAYER_CONF *dst, GW_PLAYER_CONF *src)
{
    int i;
    for (i = 0; i < 4; i++) {
        *dst++ = *src++;
    }
}

static void fn_1_5C8(void)
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

static void fn_1_8EC(void)
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

static void fn_1_A5C(s16 num)
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
        fn_1_BDC(1);
        smCursorNoPrev[smPage] = smCursorNo;
    }
    else {
        smCursorNoPrev[pagePrev] = smCursorNo;
        smCursorNo = smCursorNoPrev[smPage];
    }
}

static void fn_1_BDC(s16 num)
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

static void fn_1_C64(OMOBJ *obj)
{
    int i, j;
    OSReport("SMinit:%08x\n", fn_1_C64);
    omDLLDBGOut();
    Hu3DBGColorSet(0, 0, 0);
    fn_1_568(smPlayerConf, GwPlayerConf);
    for (i = 0; i < GW_PLAYER_MAX; i++) {
        for (j = i + 1; j < GW_PLAYER_MAX; j++) {
            if (smPlayerConf[i].charNo == smPlayerConf[j].charNo) {
                smPlayerConf[j].charNo = fn_1_1D4C(j, -1);
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
    fn_1_A5C(1);
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
    obj->objFunc = fn_1_10D8;
}

static void fn_1_10D8(OMOBJ *obj)
{
    fn_1_5C8();
    fn_1_8EC();
    if (smPadDStkDown & SM_KEY_UP) {
        fn_1_BDC(-1);
        fn_1_1B50((smPage * SM_PAGE_SIZE) + smCursorNo);
        return;
    }
    if (smPadDStkDown & SM_KEY_DOWN) {
        fn_1_BDC(1);
        fn_1_1B50((smPage * SM_PAGE_SIZE) + smCursorNo);
        return;
    }
    if (smPadDStkDown & SM_KEY_LEFT) {
        fn_1_A5C(-1);
        fn_1_1B50((smPage * SM_PAGE_SIZE) + smCursorNo);
        return;
    }
    if (smPadDStkDown & SM_KEY_RIGHT) {
        fn_1_A5C(1);
        fn_1_1B50((smPage * SM_PAGE_SIZE) + smCursorNo);
        return;
    }
    if ((smPadBtnDown & PAD_BUTTON_A) || (smPadBtnDown & PAD_BUTTON_START)) {
        obj->objFunc = fn_1_1FC8;
        return;
    }
    if (smPadBtnDown & PAD_BUTTON_Y) {
        obj->objFunc = fn_1_3940;
        return;
    }
    if (smPadBtnDown & PAD_TRIGGER_Z) {
        obj->objFunc = fn_1_43A4;
        return;
    }
    if (smPadBtnDown & PAD_BUTTON_X) {
        omOvlReturnEx(0, 1);
    }
}

static void fn_1_1B50(int pos)
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

static s16 fn_1_1D4C(s16 playerNo, s16 num)
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

static void fn_1_1E5C(void)
{
    int i;
    for (i = 0; i < SM_CHAR_MAX; i++) {
        Hu3DModelKill(smCharMdlId[i]);
    }
}

static void fn_1_1EAC(void)
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

static void fn_1_1FC8(OMOBJ *obj)
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
    fn_1_1D4C(0, 0);
    obj->objFunc = fn_1_21C0;
}

static void fn_1_21C0(OMOBJ *obj) {}

static void fn_1_2F80(OMOBJ *obj)
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

static void fn_1_313C(void) {}

/* ---------------- .bss group C ---------------- */
static s16 smPlayerConfEditF;
static s16 smPlayerConfNo;
static s16 smPlayerConfChoiceNo;

static void fn_1_3940(OMOBJ *obj)
{
    smPlayerConfEditF = 0;
    smPlayerConfNo = 0;
    smPlayerConfChoiceNo = 0;
    obj->objFunc = fn_1_3980;
}

static void fn_1_3980(OMOBJ *obj) {}

static void fn_1_4380(OMOBJ *obj)
{
    rand8();
}

static void fn_1_43A0(void) {}

/* ---------------- .bss group D ---------------- */
static s16 smEmiCompDataNo;
static s16 smEmiCompVal;
static s16 smSound3DNo;
s16 lbl_1_bss_0;

static void fn_1_43A4(OMOBJ *obj)
{
    MSMSE *se = msmSeGetIndexPtr(smEmiCompDataNo);
    smEmiCompVal = se->comp;
    obj->objFunc = fn_1_4408;
}

static void fn_1_4408(OMOBJ *obj) {}

static void fn_1_4C5C(void) {}

static void fn_1_502C(void) {}
