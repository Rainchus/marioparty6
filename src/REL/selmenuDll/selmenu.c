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

typedef void (*VoidFunc)(void);
extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];

typedef struct sm_entry {
    u16 on;
    char *name;
    OMOVL ovl;
} SMEntry;

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

void fn_1_A0(void) {}

static void fn_1_568(GW_PLAYER_CONF *dst, GW_PLAYER_CONF *src)
{
    int i;
    for (i = 0; i < 4; i++) {
        *dst++ = *src++;
    }
}

static void fn_1_5C8(void) {}

static void fn_1_8EC(void) {}

static void fn_1_A5C(s16 num) {}

static void fn_1_BDC(s16 num) {}

static void fn_1_C64(OMOBJ *obj) {}

static void fn_1_10D8(OMOBJ *obj) {}

static void fn_1_1B50(int pos) {}

/* ---------------- .bss group B ---------------- */
static HU3D_MODELID smCharMdlId[SM_CHAR_MAX];
static s16 smCharSelEndF[4];
static s16 smCharOnF[SM_CHAR_MAX];

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

static void fn_1_1EAC(void) {}

static void fn_1_1FC8(OMOBJ *obj) {}

static void fn_1_21C0(OMOBJ *obj) {}

static void fn_1_2F80(OMOBJ *obj) {}

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
