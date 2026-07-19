#include "game/object.h"
#include "game/hu3d.h"
#include "game/pad.h"
#include "game/wipe.h"
#include "game/gamework.h"
#include "game/printfunc.h"

#include "datanum/saf.h"
#include "datanum/mario.h"

typedef void (*VoidFunc)(void);
extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];

extern void ObjectSetup(void);

int _prolog(void) {
    const VoidFunc* ctors = _ctors;
    while (*ctors != 0) {
        (**ctors)();
        ctors++;
    }
    ObjectSetup();
    return 0;
}

void _epilog(void) {
    const VoidFunc* dtors = _dtors;
    while (*dtors != 0) {
        (**dtors)();
        dtors++;
    }
}

#define MOTION_MAX 216

static char *MotNameTbl[MOTION_MAX] = {
    "c000m1_300",
    "c000m1_301",
    "c000m1_302",
    "c000m1_303",
    "c000m1_304",
    "c000m1_305",
    "c000m1_306",
    "c000m1_307",
    "c000m1_308",
    "c000m1_309",
    "c000m1_310",
    "c000m1_311",
    "c000m1_312",
    "c000m1_313",
    "c000m1_314",
    "c000m1_315",
    "c000m1_316",
    "c000m1_317",
    "c000m1_318",
    "c000m1_319",
    "c000m1_320",
    "c000m1_321",
    "c000m1_322",
    "c000m1_323",
    "c000m1_324",
    "c000m1_325",
    "c000m1_326",
    "c000m1_327",
    "c000m1_328",
    "c000m1_332",
    "c000m1_333",
    "c000m1_336",
    "c000m1_398",
    "c000m1_339",
    "c000m1_340",
    "c000m1_341",
    "c000m1_343",
    "c000m1_346",
    "c000m1_347",
    "c000m1_349",
    "c000m1_350",
    "c000m1_351",
    "c000m1_352",
    "c000m1_354",
    "c000m1_355",
    "c000m1_356",
    "c000m1_357",
    "c000m1_358",
    "c000m1_359",
    "c000m1_360",
    "c000m1_361",
    "c000m1_362",
    "c000m1_364",
    "c000m1_365",
    "c000m1_367",
    "c000m1_368",
    "c000m1_369",
    "c000m1_370",
    "c000m1_371",
    "c000m1_372",
    "c000m1_374",
    "c000m1_376",
    "c000m1_377",
    "c000m1_380",
    "c000m1_381",
    "c000m1_382",
    "c000m1_383",
    "c000m1_385",
    "c000m1_386",
    "c000m1_396",
    "c000m1_399",
    "c000m1_402",
    "c000m1_403",
    "c000m1_404",
    "c000m1_405",
    "c000m1_406",
    "c000m1_409",
    "c000m1_410",
    "c000m1_411",
    "c000m1_412",
    "c000m1_413",
    "c000m1_419",
    "c000m1_420",
    "c000m1_421",
    "c000m1_422",
    "c000m1_423",
    "c000m1_424",
    "c000m1_425",
    "c000m1_427",
    "c000m1_433",
    "c000m1_434",
    "c000m1_435",
    "c000m1_436",
    "c000m1_437",
    "c000m1_438",
    "c000m1_439",
    "c000m1_440",
    "c000m1_441",
    "c000m1_442",
    "c000m1_443",
    "c000m1_444",
    "c000m1_446",
    "c000m1_453",
    "c000m1_454",
    "c000m1_455",
    "c000m1_456",
    "c000m1_457",
    "c000m1_458",
    "c000m1_464",
    "c000m1_468",
    "c000m1_476",
    "c000m1_477",
    "c000m1_478",
    "c000m1_480",
    "c000m1_481",
    "c000m1_483",
    "c000m1_484",
    "c000m1_485",
    "c000m1_487",
    "c000m1_488",
    "c000m1_489",
    "c000m1_490",
    "c000m1_491",
    "c000m1_493",
    "c000m1_494",
    "c000m1_505",
    "c000m1_506",
    "c000m1_507",
    "c000m1_508",
    "c000m1_509",
    "c000m1_510",
    "c000m1_526",
    "c000m1_600",
    "c000m1_601",
    "c000m1_602",
    "c000m1_603",
    "c000m1_604",
    "c000m1_605",
    "c000m1_606",
    "c000m1_607",
    "c000m1_608",
    "c000m1_609",
    "c000m1_610",
    "c000m1_611",
    "c000m1_612",
    "c000m1_613",
    "c000m1_614",
    "c000m1_615",
    "c000m1_616",
    "c000m1_617",
    "c000m1_618",
    "c000m1_619",
    "c000m1_620",
    "c000m1_621",
    "c000m1_622",
    "c000m1_623",
    "c000m1_624",
    "c000m1_625",
    "c000m1_626",
    "c000m1_627",
    "c000m1_628",
    "c000m1_629",
    "c000m1_630",
    "c000m1_631",
    "c000m1_632",
    "c000m1_633",
    "c000m1_634",
    "c000m1_635",
    "c000m1_636",
    "c000m1_637",
    "c000m1_638",
    "c000m1_639",
    "c000m1_640",
    "c000m1_641",
    "c000m1_642",
    "c000m1_643",
    "c000m1_644",
    "c000m1_645",
    "c000m1_646",
    "c000m1_647",
    "c000m1_648",
    "c000m1_649",
    "c000m1_650",
    "c000m1_651",
    "c000m1_652",
    "c000m1_653",
    "c000m1_654",
    "c000m1_655",
    "c000m1_656",
    "c000m1_657",
    "c000m1_658",
    "c000m1_659",
    "c000m1_660",
    "c000m1_661",
    "c000m1_662",
    "c000m1_663",
    "c000m1_664",
    "c000m1_665",
    "c000m1_666",
    "c000m1_667",
    "c000m1_668",
    "c000m1_669",
    "c000m1_670",
    "c000m1_671",
    "c000m1_672",
    "c000m1_673",
    "c000m1_674",
    "c000m1_675",
    "c000m1_676",
    "c000m1_677",
    "c000m1_678",
    "c000m1_679",
    "c000m1_680",
    "c000m1_681",
    "c000m1_682",
    "c000m1_eye"
};

static OMOBJ *CameraObj;
static OMOBJ *OutViewObj;
u32 lbl_1_bss_50[8];
u32 lbl_1_bss_48[2];
int TimingHookMode;
OMOBJMAN *MotChkObjMan;
HU3D_LIGHTID MotChkLightId;
static int CharMdlId[4];
static int CharMotId[4];
static int CharMotIdleId[4];
static int CharNo;
static int CharMotNo;
static int CharMdlNo;

void CameraMain(OMOBJ *obj);
void MotChkMain(void);

void ObjectSetup(void)
{
    OSReport("******* Motion Check ObjectSetup *********\n");
    MotChkObjMan = omInitObjMan(50, 8192);
    CRot.x = -20;
    CRot.y = 0;
    CRot.z = 0;
    Center.x = 0;
    Center.y = 100;
    Center.z = 0;
    CZoom = 400;
    Hu3DCameraCreate(HU3D_CAM0);
    Hu3DCameraPerspectiveSet(HU3D_CAM0, 45, 20, 15000, 1.2f);
    Hu3DCameraViewportSet(HU3D_CAM0, 0, 0, 640, 480, 0, 1);
    OutViewObj = omAddOutViewObj(MotChkObjMan);
    CameraObj = omAddObj(MotChkObjMan, 0, 64, 0, CameraMain);
    HuPrcChildCreate(MotChkMain, 100, 12288, 0, MotChkObjMan);
    MotChkLightId = Hu3DGLightCreate(1000, 1000, 1000, -1, -1, -1, 192, 192, 192);
    Hu3DAmbColorSet(0.5f, 0.5f, 0.5f);
    Hu3DShineSet(TRUE);
    Hu3DGLightInfinitytSet(MotChkLightId);
}

static void CreateChar(s16 charNo, s16 motNo, s16 mdlNo);
static void LoadCharMotion(s16 charNo, s16 motNo, s16 mdlNo);
static void KillChar(s16 charNo);

void MotChkMain(void)
{
    s16 i;
    HuVecF shadowPos, shadowTarget, shadowUp;
    BOOL upF;
    
    WipeCreate(WIPE_MODE_IN, WIPE_TYPE_NORMAL, 20);
    WipeWait();
    i = Hu3DModelCreateData(SAF_HSF_cube);
    Hu3DModelPosSet(i, 0, 200, 0);
    Hu3DModelShadowMapSet(i);
    HuDataDirClose(DATA_saf);
    Hu3DShadowCreate(10, 20, 2000);
    shadowPos.x = 200;
    shadowPos.y = 1000;
    shadowPos.z = 200;
    shadowUp.y = 1;
    shadowUp.x = shadowUp.z = 0;
    shadowTarget.x = shadowTarget.y = shadowTarget.z = 0;
    Hu3DShadowPosSet(&shadowPos, &shadowUp, &shadowTarget);
    Hu3DShadowTPLvlSet(0.6f);
    CharMotNo = 0;
    CharNo = GwPlayerConf[0].charNo;
    CharMdlNo = 1;
    upF = FALSE;
    CreateChar(CharNo, CharMotNo, CharMdlNo);
    while(1) {
        if(HuPadBtnRep[0] & PAD_BUTTON_A) {
            CharMotNo++;
            if(CharMotNo >= MOTION_MAX) {
                CharMotNo = 0;
            }
            LoadCharMotion(CharNo, CharMotNo, CharMdlNo);
        } else if(HuPadBtnRep[0] & PAD_BUTTON_B) {
            CharMotNo--;
            if(CharMotNo < 0) {
                CharMotNo = MOTION_MAX-1;
            }
            LoadCharMotion(CharNo, CharMotNo, CharMdlNo);
        } else if(HuPadBtnDown[0] & PAD_TRIGGER_Z) {
            CharMdlNo++;
            if(CharMdlNo >= 4) {
                CharMdlNo = 0;
            }
            for(i=0; i<4; i++) {
                Hu3DModelDispOff(CharMdlId[i]);
            }
            Hu3DModelDispOn(CharMdlId[CharMdlNo]);
        } else if(HuPadBtnRep[0] & PAD_BUTTON_X) {
            KillChar(CharNo);
            CharNo++;
            if(CharNo > GW_CHARA_KINOPIKO) {
                CharNo = 0;
            }
            CreateChar(CharNo, CharMotNo, CharMdlNo);
        } else if(HuPadBtnRep[0] & PAD_BUTTON_Y) {
            KillChar(CharNo);
            CharNo--;
            if(CharNo < 0) {
                CharNo = GW_CHARA_KINOPIKO;
            }
            CreateChar(CharNo, CharMotNo, CharMdlNo);
        }
        if(HuPadBtn[0] & PAD_BUTTON_LEFT) {
            Hu3DAmbColorSet(1, 1, 1);
            Hu3DShineSet(FALSE);
            Hu3DGLightColorSet(MotChkLightId, 255, 255, 255, 255);
        } else {
            Hu3DAmbColorSet(0.5f, 0.5f, 0.5f);
            Hu3DShineSet(TRUE);
            Hu3DGLightColorSet(MotChkLightId, 128, 128, 128, 255);
        }
        if(HuPadBtnDown[0] == PAD_BUTTON_UP) {
            upF = TRUE;
        }
        fontcolor = FONT_COLOR_WHITE;
        print8(16, 24, 2.0f, "%03d:%s", CharMotNo, MotNameTbl[CharMotNo]);
        print8(16, 40, 2.0f, "MODEL%d", CharMdlNo);
        if(TimingHookMode == 1) {
            print8(608, 416, 2.0f, "*");
        } else if(TimingHookMode == 2) {
            print8(608, 416, 2.0f, "-");
        }
        TimingHookMode = 0;
        HuPrcVSleep();
    }
}

unsigned int CharDirTbl[GW_CHARA_KINOPIKO+1] = {
    DATA_mario,
    DATA_luigi,
    DATA_peach,
    DATA_yoshi,
    DATA_wario,
    DATA_daisy,
    DATA_waluigi,
    DATA_kinopio,
    DATA_teresa,
    DATA_minikoopa,
    DATA_kinopiko
};

#define CHAR_FILE(filename) MARIO_HSF_##filename
#define CHAR_DATANUM(charNo, file) (CharDirTbl[charNo]|FILENUM(file))

static void CharTimingHook(HU3D_MODELID modelId, HU3D_MOTIONID motId, BOOL lagF);

static void CreateChar(s16 charNo, s16 motNo, s16 mdlNo)
{
    s16 i;
    CharMdlId[0] = Hu3DModelCreateData(CHAR_DATANUM(charNo, MARIO_HSF_c000m0));
    CharMdlId[1] = Hu3DModelCreateData(CHAR_DATANUM(charNo, MARIO_HSF_c000m1));
    CharMdlId[2] = Hu3DModelCreateData(CHAR_DATANUM(charNo, MARIO_HSF_c000m2));
    CharMdlId[3] = Hu3DModelCreateData(CHAR_DATANUM(charNo, MARIO_HSF_c000m3));
    CharMotId[0] = Hu3DJointMotionData(CharMdlId[0], CHAR_DATANUM(charNo, MARIO_HSF_c000m1_300+motNo));
    CharMotId[1] = Hu3DJointMotionData(CharMdlId[1], CHAR_DATANUM(charNo, MARIO_HSF_c000m1_300+motNo));
    CharMotId[2] = Hu3DJointMotionData(CharMdlId[2], CHAR_DATANUM(charNo, MARIO_HSF_c000m1_300+motNo));
    CharMotId[3] = Hu3DJointMotionData(CharMdlId[3], CHAR_DATANUM(charNo, MARIO_HSF_c000m1_300+motNo));
    CharMotIdleId[0] = Hu3DJointMotionData(CharMdlId[0], CHAR_DATANUM(charNo, MARIO_HSF_c000m1_300));
    CharMotIdleId[1] = Hu3DJointMotionData(CharMdlId[1], CHAR_DATANUM(charNo, MARIO_HSF_c000m1_300));
    CharMotIdleId[2] = Hu3DJointMotionData(CharMdlId[2], CHAR_DATANUM(charNo, MARIO_HSF_c000m1_300));
    CharMotIdleId[3] = Hu3DJointMotionData(CharMdlId[3], CHAR_DATANUM(charNo, MARIO_HSF_c000m1_300));
    Hu3DMotionSet(CharMdlId[0], CharMotId[0]);
    Hu3DMotionSet(CharMdlId[1], CharMotId[1]);
    Hu3DMotionSet(CharMdlId[2], CharMotId[2]);
    Hu3DMotionSet(CharMdlId[3], CharMotId[3]);
    for(i=0; i<4; i++) {
        Hu3DModelDispOff(CharMdlId[i]);
        Hu3DModelMotLoopOn(CharMdlId[i]);
        Hu3DMotionAttrSet(CharMotId[i], HU3D_ATTR_MOT_RESET_LOCK);
        Hu3DModelShadowSet(CharMdlId[i]);
        Hu3DMotionTimingHookSet(CharMdlId[i], CharTimingHook);
    }
    Hu3DModelDispOn(CharMdlId[mdlNo]);
    TimingHookMode = 0;
    
}

static void CharTimingHook(HU3D_MODELID modelId, HU3D_MOTIONID motId, BOOL lagF)
{
    TimingHookMode = (lagF) ? 1 : 2;
}

static void LoadCharMotion(s16 charNo, s16 motNo, s16 mdlNo)
{
    s16 i;
    for(i=0; i<4; i++) {
        Hu3DMotionSet(CharMdlId[i], CharMotIdleId[i]);
    }
    for(i=0; i<4; i++) {
        Hu3DMotionKill(CharMotId[i]);
    }
    CharMotId[0] = Hu3DJointMotionData(CharMdlId[0], CHAR_DATANUM(charNo, MARIO_HSF_c000m1_300+motNo));
    CharMotId[1] = Hu3DJointMotionData(CharMdlId[1], CHAR_DATANUM(charNo, MARIO_HSF_c000m1_300+motNo));
    CharMotId[2] = Hu3DJointMotionData(CharMdlId[2], CHAR_DATANUM(charNo, MARIO_HSF_c000m1_300+motNo));
    CharMotId[3] = Hu3DJointMotionData(CharMdlId[3], CHAR_DATANUM(charNo, MARIO_HSF_c000m1_300+motNo));
    Hu3DMotionSet(CharMdlId[0], CharMotId[0]);
    Hu3DMotionSet(CharMdlId[1], CharMotId[1]);
    Hu3DMotionSet(CharMdlId[2], CharMotId[2]);
    Hu3DMotionSet(CharMdlId[3], CharMotId[3]);
    for(i=0; i<4; i++) {
        Hu3DModelDispOff(CharMdlId[i]);
        Hu3DModelMotLoopOn(CharMdlId[i]);
        Hu3DMotionAttrSet(CharMotId[i], HU3D_ATTR_MOT_RESET_LOCK);
    }
    Hu3DModelDispOn(CharMdlId[mdlNo]);
}

static void KillChar(s16 charNo)
{
    s16 i;
    HuDataDirClose(CharDirTbl[charNo]);
    for(i=0; i<4; i++) {
        Hu3DModelKill(CharMdlId[i]);
    }
    for(i=0; i<4; i++) {
        Hu3DMotionKill(CharMotId[i]);
        Hu3DMotionKill(CharMotIdleId[i]);
    }
}

void CameraMain(OMOBJ *obj)
{
    HuVecF pos;
    HuVecF offset;
    HuVecF dir;
    HuVecF yOfs;
    float rotZ;
    s8 stickPos;
    
    if(HuPadBtnDown[0] & PAD_BUTTON_START) {
        if(lbl_1_bss_50) {
            HuPrcKill((HUPROCESS *)lbl_1_bss_50);
        }
        omOvlReturn(1);
        return;
    }
    CRot.x += HuPadStkY[0]/20;
    CRot.y += HuPadStkX[0]/20;
    CZoom += HuPadTrigL[0]/2;
    CZoom -= HuPadTrigR[0]/2;
    pos.x = Center.x + (CZoom * (HuSin(CRot.y) * HuCos(CRot.x)));
    pos.y = (Center.y + (CZoom * -HuSin(CRot.x)));
    pos.z = (Center.z + (CZoom * (HuCos(CRot.y) * HuCos(CRot.x))));
    offset.x = Center.x - pos.x;
    offset.y = Center.y - pos.y;
    offset.z = Center.z - pos.z;
    dir.x = (HuSin(CRot.y) * HuSin(CRot.x));
    dir.y = HuCos(CRot.x);
    dir.z = (HuCos(CRot.y) * HuSin(CRot.x));
    rotZ = CRot.z;
    yOfs.x = dir.x * (offset.x * offset.x + (1.0f - offset.x * offset.x) * HuCos(rotZ))
        + dir.y * (offset.x * offset.y * (1.0f - HuCos(rotZ)) - offset.z * HuSin(rotZ))
        + dir.z * (offset.x * offset.z * (1.0f - HuCos(rotZ)) + offset.y * HuSin(rotZ));

    yOfs.y = dir.y * (offset.y * offset.y + (1.0f - offset.y * offset.y) * HuCos(rotZ))
        + dir.x * (offset.x * offset.y * (1.0f - HuCos(rotZ)) + offset.z * HuSin(rotZ))
        + dir.z * (offset.y * offset.z * (1.0f - HuCos(rotZ)) - offset.x * HuSin(rotZ));

    yOfs.z = dir.z * (offset.z * offset.z + (1.0f - offset.z * offset.z) * HuCos(rotZ))
        + (dir.x * (offset.x * offset.z * (1.0 - HuCos(rotZ)) - offset.y * HuSin(rotZ))
        + dir.y * (offset.y * offset.z * (1.0 - HuCos(rotZ)) + offset.x * HuSin(rotZ)));
    VECCrossProduct(&dir, &offset, &offset);
    VECNormalize(&offset, &offset);
    stickPos = (HuPadSubStkX[0] & 0xF8);
    if (stickPos != 0) {
        Center.x += 0.05f * (offset.x * stickPos);
        Center.y += 0.05f * (offset.y * stickPos);
        Center.z += 0.05f * (offset.z * stickPos);
    }
    VECNormalize(&yOfs, &offset);
    stickPos = -(HuPadSubStkY[0] & 0xF8);
    if (stickPos != 0) {
        Center.x += 0.05f * (offset.x * stickPos);
        Center.y += 0.05f * (offset.y * stickPos);
        Center.z += 0.05f * (offset.z * stickPos);
    }
}