#include "dolphin.h"
#include "dolphin/os/OSReset.h"
#include "game/audio.h"
#include "game/data.h"
#include "game/gamework.h"
#include "game/hu3d.h"
#include "game/init.h"
#include "game/main.h"
#include "game/object.h"
#include "game/pad.h"
#include "game/process.h"
#include "game/sprite.h"
#include "game/window.h"
#include "game/wipe.h"
#include "math.h"

typedef void (*VoidFunc)(void);

extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];
extern u8 nintendoData[];
extern u32 warningData[];
extern u32 warningPressStartData[];
extern s16 lbl_1_bss_4;
extern s16 TitleMdlId[4];
extern s16 BootLightId[3];
extern float BootCameraZoom[2];
extern HuVecF BootCameraCenter[2];
extern HuVecF BootCameraRot[2];
extern OMOBJMAN *BootObjMan;
extern OMOBJ *OutViewObj;

enum {
    NINTENDO_SPRITE_DATA = DATANUM(DATA_title, 0x01),
    PROGRESSIVE_MSG_SPRITE_DATA = DATANUM(DATA_title, 0x06),
    PROGRESSIVE_ON_SPRITE_DATA = DATANUM(DATA_title, 0x07),
    PROGRESSIVE_OFF_SPRITE_DATA = DATANUM(DATA_title, 0x08),
    OPENING_WAIT_SPRITE_DATA = DATANUM(DATA_title, 0x09),
    PROGRESSIVE_CURSOR_SPRITE_DATA = DATANUM(DATA_title, 0x0A),
    PROGRESSIVE_CURSOR_SEL_SPRITE_DATA = DATANUM(DATA_title, 0x0B),
    TITLE_HSF_PARTY = DATANUM(DATA_title, 0x13),
    TITLE_HSF_STAR = DATANUM(DATA_title, 0x14),
    TITLE_HSF_SIX = DATANUM(DATA_title, 0x15),

    WARNING_SPRITE_MEMBER = 0,
    PRESS_START_SPRITE_MEMBER = 1,
    WARNING_SPRITE_MEMBER_COUNT = 2,

    WARNING_SPRITE_PRIORITY = 0,
    WARNING_SPRITE_BANK = 0,
    TITLE_WIPE_FRAMES = 30,
    TITLE_STREAM_DELAY_FRAMES = 10,
    TITLE_INPUT_WAIT_FRAMES = 0x834,
    TITLE_CONFIRM_WAIT_FRAMES = 20,
    TITLE_CONFIRM_SE = 0x49C,
    INITIAL_WAIT_FRAMES = 90,
    MAX_INPUT_WAIT_FRAMES = 0xDB6,
    BLINK_PERIOD_FRAMES = 30,
    BOOT_PAD_SKIP_MASK = 0x1100,
};

void BootExec(void);
void BootProgExec(void);
void ObjectSetup(void);
void CameraOutView(OMOBJ *object);
void CameraMove(OMOBJ *object);
void BootTitleCreate(void);
s32 BootTitleExec(s32 firstRun);
void BootWarningExec(void);
void *NintendoDataDecode(void);
void CharInit(void);
void GameMesInit(void);
void OpeningCreate(void);
void OpeningExec(s32 arg);
void OpeningKill(void);

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
    s32 i;
    OMOVLHIS *history;

    OSReport("******* Boot ObjectSetup *********\n");
    BootObjMan = omInitObjMan(50, 8192);
    BootCameraRot[0].x = -5.0f;
    BootCameraRot[0].y = 0.0f;
    BootCameraRot[0].z = 0.0f;
    BootCameraCenter[0].x = 0.0f;
    BootCameraCenter[0].y = 105.0f;
    BootCameraCenter[0].z = -54.0f;
    BootCameraZoom[0] = 636.0f;
    Hu3DCameraCreate(1);
    Hu3DCameraPerspectiveSet(1, 45.0f, 20.0f, 15000.0f, 1.2f);
    Hu3DCameraViewportSet(1, 0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
    HuPrcCreate(BootExec, 100, 12288, 0);
    OutViewObj = omAddObjEx(BootObjMan, 0x7FDA, 0, 0, OM_GRP_NONE, CameraOutView);
    omAddObjEx(BootObjMan, 0, 32, 32, OM_GRP_NONE, CameraMove);
    BootLightId[0] = Hu3DGLightCreate(0.0f, 10.0f, 1000.0f,
                                      0.0f, -0.01f, -1.0f,
                                      64, 64, 96);
    Hu3DGLightInfinitytSet(BootLightId[0]);
    BootLightId[1] = Hu3DGLightCreate(0.0f, 2000.0f, 100.0f,
                                      0.0f, -1.0f, -0.05f,
                                      160, 160, 160);
    Hu3DGLightInfinitytSet(BootLightId[1]);
    Hu3DBGColorSet(0, 0, 0);
    history = omOvlHisGet(0);
    omOvlHisChg(0, history->ovl, 1, history->stat);
    for (i = 0; i < GW_PLAYER_MAX; i++) {
        GwPlayerConf[i].charNo = GwPlayerConf[i].padNo = i;
    }
}

void BootExec(void)
{
    HUPROCESS *process;
    HUSPR_GROUPID group;
    HUSPRID nintendoSprite;
    HUSPRID titleSprite;
    HUSPRID waitSprite;
    ANIMDATA *anim;
    s32 firstRun;
    s16 asyncStat;
    s16 frame;
    OSTick start;
    void *buf;

    process = HuPrcCurrentGet();
    firstRun = FALSE;
    asyncStat = HU_DATA_STAT_NONE;

    if (omovlevtno == 0 && VIGetTvFormat() != VI_MPAL) {
        BootProgExec();
    }
    if (omovlevtno == 0) {
        BootWarningExec();
    }

    group = HuSprGrpCreate(3);
    anim = HuSprAnimRead(NintendoDataDecode());
    nintendoSprite = HuSprCreate(anim, 0, 0);
    HuSprGrpMemberSet(group, 0, nintendoSprite);
    HuSprPosSet(group, 0, 288.0f, 240.0f);
    HuSprAttrSet(group, 0, HUSPR_ATTR_DISPOFF);

    if (omovlevtno != 0) {
        HuAudSndGrpSetSet(MSM_GRP_MENU);

        anim = HuSprAnimRead(HuDataSelHeapReadNum(NINTENDO_SPRITE_DATA, HU_MEMNUM_OVL, HEAP_MODEL));
        titleSprite = HuSprCreate(anim, 0, 0);
        HuSprGrpMemberSet(group, 1, titleSprite);
        HuSprPosSet(group, 1, 288.0f, 240.0f);
        HuSprAttrSet(group, 1, HUSPR_ATTR_DISPOFF);

        waitSprite = HuSprCreate(HuSprAnimRead(HuDataSelHeapReadNum(OPENING_WAIT_SPRITE_DATA,
                                                                    HU_MEMNUM_OVL, HEAP_MODEL)), 0, 0);
        HuSprGrpMemberSet(group, 2, waitSprite);
        HuSprPosSet(group, 2, 288.0f, 206.0f);
        HuSprAttrSet(group, 2, HUSPR_ATTR_DISPOFF | HUSPR_ATTR_LINEAR);

        HuWinInit(1);
        BootTitleCreate();
        OpeningCreate();
        NintendoDispF = TRUE;
        firstRun = TRUE;
        goto title_exec;
    } else {
        for (frame = 0; frame < GW_PLAYER_MAX; frame++) {
            GwPlayerConf[frame].padNo = frame;
        }

        HuSprAttrReset(group, 0, HUSPR_ATTR_DISPOFF);
        WipeCreate(WIPE_MODE_IN, WIPE_TYPE_NORMAL, 30);
        while (WipeCheck()) {
            HuPrcVSleep();
        }
        WipeColorSet(255, 255, 255);

        if (NintendoDispF == FALSE) {
            start = OSGetTick();
            CharInit();
            HuWindowInit();
            GameMesInit();
            HuWinInit(1);
            asyncStat = HuDataDirReadAsync(DATA_title);
            while (OSTicksToMilliseconds(OSGetTick() - start) < 1500) {
                HuPrcVSleep();
            }
            OSReport("%dms\n", OSTicksToMilliseconds(OSGetTick() - start));
        } else {
            for (frame = 0; frame < 90; frame++) {
                if (HuPadBtnDown[0] & BOOT_PAD_SKIP_MASK) {
                    break;
                }
                HuPrcVSleep();
            }
        }

        WipeCreate(WIPE_MODE_OUT, WIPE_TYPE_NORMAL, 30);
        while (WipeCheck()) {
            HuPrcVSleep();
        }

        if (NintendoDispF == FALSE) {
            start = OSGetTick();
            while (!HuDataGetAsyncStat(asyncStat)) {
                HuPrcVSleep();
            }
            BootTitleCreate();

            anim = HuSprAnimRead(HuDataSelHeapReadNum(NINTENDO_SPRITE_DATA, HU_MEMNUM_OVL, HEAP_MODEL));
            titleSprite = HuSprCreate(anim, 0, 0);
            HuSprGrpMemberSet(group, 1, titleSprite);
            HuSprPosSet(group, 1, 288.0f, 240.0f);
            HuSprAttrSet(group, 1, HUSPR_ATTR_DISPOFF);

            waitSprite = HuSprCreate(HuSprAnimRead(HuDataSelHeapReadNum(OPENING_WAIT_SPRITE_DATA,
                                                                        HU_MEMNUM_OVL, HEAP_MODEL)), 0, 0);
            HuSprGrpMemberSet(group, 2, waitSprite);
            HuSprPosSet(group, 2, 288.0f, 206.0f);
            HuSprAttrSet(group, 2, HUSPR_ATTR_DISPOFF);

            while (OSTicksToMilliseconds(OSGetTick() - start) < 1000) {
                HuPrcVSleep();
            }
            OSReport("%dms\n", OSTicksToMilliseconds(OSGetTick() - start));
        } else {
            HuPrcSleep(60);
        }

        HuSprAttrSet(group, 0, HUSPR_ATTR_DISPOFF);
        HuSprAttrReset(group, 1, HUSPR_ATTR_DISPOFF);
        WipeCreate(WIPE_MODE_IN, WIPE_TYPE_NORMAL, 30);
        while (WipeCheck()) {
            HuPrcVSleep();
        }

        if (NintendoDispF == FALSE) {
            start = OSGetTick();
            OpeningCreate();
            while (OSTicksToMilliseconds(OSGetTick() - start) < 1500) {
                HuPrcVSleep();
            }
            OSReport("%dms\n", OSTicksToMilliseconds(OSGetTick() - start));
        } else {
            for (frame = 0; frame < 90; frame++) {
                if (HuPadBtnDown[0] & BOOT_PAD_SKIP_MASK) {
                    break;
                }
                HuPrcVSleep();
            }
        }

        WipeColorSet(255, 255, 255);
        WipeCreate(WIPE_MODE_OUT, WIPE_TYPE_NORMAL, 30);
        while (WipeCheck()) {
            HuPrcVSleep();
        }
        HuPrcSleep(60);

        HuSprAttrSet(group, 0, HUSPR_ATTR_DISPOFF);
        HuSprAttrSet(group, 1, HUSPR_ATTR_DISPOFF);
        HuSprAttrReset(group, 2, HUSPR_ATTR_DISPOFF);
        WipeCreate(WIPE_MODE_IN, WIPE_TYPE_NORMAL, 30);
        while (WipeCheck()) {
            HuPrcVSleep();
        }

        if (NintendoDispF == FALSE) {
            HuPrcSleep(60);
        } else {
            for (frame = 0; frame < 60; frame++) {
                if (HuPadBtnDown[0] & BOOT_PAD_SKIP_MASK) {
                    break;
                }
                HuPrcVSleep();
            }
        }

        WipeCreate(WIPE_MODE_OUT, WIPE_TYPE_NORMAL, 30);
        while (WipeCheck()) {
            HuPrcVSleep();
        }
        HuSprAttrSet(group, 2, HUSPR_ATTR_DISPOFF);

        if (NintendoDispF == FALSE) {
            HuDataDirClose(DATA_title);
            buf = HuMemDirectMallocNum(HEAP_DVD, msmSysGetSampSize(FALSE), HU_MEMNUM_OVL);
            msmSysLoadGroup(0, buf, FALSE);
            HuMemDirectFree(buf);
            msmSysSetGroupLoadMode(TRUE);
            HuAudSndGrpSetSet(MSM_GRP_MENU);
        } else {
            HuPrcSleep(90);
        }
    }

opening_exec:
    OpeningExec(1);
    NintendoDispF = TRUE;
    HuSprAttrSet(group, 0, HUSPR_ATTR_DISPOFF);
    HuSprAttrSet(group, 1, HUSPR_ATTR_DISPOFF);
    HuSprAttrSet(group, 2, HUSPR_ATTR_DISPOFF);

title_exec:
    if (BootTitleExec(firstRun) == 0) {
        firstRun = FALSE;
        HuPrcSleep(30);
        goto opening_exec;
    }

    HuPrcSleep(60);
    OpeningKill();
    HuDataDirClose(DATA_title);
    HuAudSStreamAllFadeOut(1000);
    asyncStat = HuDataDirReadAsync(DATA_filesel);
    WipeCreate(WIPE_MODE_OUT, WIPE_TYPE_NORMAL, 60);
    while (WipeCheck()) {
        HuPrcVSleep();
    }
    HuPrcSleep(30);
    while (!HuDataGetAsyncStat(asyncStat)) {
        HuPrcVSleep();
    }

    if (omSysExitReq != 0) {
        omOvlReturnEx(1, TRUE);
    } else {
        omOvlCallEx(DLL_fileseldll, TRUE, 0, 0);
    }

    for (frame = 0; frame < GW_PLAYER_MAX; frame++) {
        GwPlayerConf[frame].charNo = -1;
    }

    HuPrcEnd();
    while (TRUE) {
        HuPrcVSleep();
    }
}

void BootProgExec(void)
{
    static u16 posTbl[] = { 221, 311, 357, 311 };
    HUSPR_GROUPID group;
    HUSPRID sprite;
    s16 frame;
    ANIMDATA *anim;
    s16 selection;

    selection = 0;
    if (OSGetResetCode() != 0) {
        return;
    }

    if (VIGetDTVStatus() != 1) {
        RenderMode = &GXNtsc480IntDf;
        VIConfigure(&GXNtsc480IntDf);
        VIFlush();
        return;
    }

    if (!OSGetProgressiveMode()) {
        if (!((HuPadBtn[0] | HuPadBtn[1] | HuPadBtn[2] | HuPadBtn[3]) & PAD_BUTTON_B)) {
            return;
        }
    }

    if (OSGetProgressiveMode() == 1) {
        RenderMode = &GXNtsc480IntDf;
        VIConfigure(&GXNtsc480IntDf);
        VIFlush();
    }

    group = HuSprGrpCreate(3);

    anim = HuSprAnimRead(HuDataSelHeapReadNum(PROGRESSIVE_MSG_SPRITE_DATA, HU_MEMNUM_OVL, HEAP_MODEL));
    sprite = HuSprCreate(anim, 0, 0);
    HuSprGrpMemberSet(group, 0, sprite);
    HuSprPosSet(group, 0, 288.0f, 240.0f);

    anim = HuSprAnimRead(HuDataSelHeapReadNum(PROGRESSIVE_CURSOR_SPRITE_DATA, HU_MEMNUM_OVL, HEAP_MODEL));
    sprite = HuSprCreate(anim, 0, 0);
    HuSprGrpMemberSet(group, 1, sprite);
    HuSprPosSet(group, 1, posTbl[selection * 2], posTbl[selection * 2 + 1]);

    anim = HuSprAnimRead(HuDataSelHeapReadNum(PROGRESSIVE_CURSOR_SEL_SPRITE_DATA, HU_MEMNUM_OVL, HEAP_MODEL));
    sprite = HuSprCreate(anim, 0, 0);
    HuSprGrpMemberSet(group, 2, sprite);
    HuSprPosSet(group, 2, posTbl[selection * 2], posTbl[selection * 2 + 1]);
    HuSprAttrSet(group, 2, HUSPR_ATTR_DISPOFF);

    WipeCreate(WIPE_MODE_IN, WIPE_TYPE_NORMAL, 30);
    while (WipeCheck()) {
        HuPrcVSleep();
    }

    for (frame = 0; frame < 600; frame++) {
        if ((HuPadDStkRep[0] | HuPadDStkRep[1] | HuPadDStkRep[2] | HuPadDStkRep[3]) & 3) {
            selection ^= 1;
            HuSprPosSet(group, 1, posTbl[selection * 2], posTbl[selection * 2 + 1]);
            frame = 0;
        }

        if ((HuPadBtnDown[0] | HuPadBtnDown[1] | HuPadBtnDown[2] | HuPadBtnDown[3]) & PAD_BUTTON_A) {
            HuSprAttrSet(group, 1, HUSPR_ATTR_DISPOFF);
            HuSprPosSet(group, 2, posTbl[selection * 2], posTbl[selection * 2 + 1]);
            HuSprAttrReset(group, 2, HUSPR_ATTR_DISPOFF);
            break;
        }
        HuPrcVSleep();
    }

    WipeCreate(WIPE_MODE_OUT, WIPE_TYPE_NORMAL, 20);
    while (WipeCheck()) {
        HuPrcVSleep();
    }

    if (selection == 0) {
        OSSetProgressiveMode(1);
        RenderMode = &GXNtsc480Prog;
        VIConfigure(&GXNtsc480Prog);
        VIFlush();
        HuPrcSleep(100);
    } else {
        OSSetProgressiveMode(0);
        GXSetCopyFilter(RenderMode->aa, RenderMode->sample_pattern, GX_TRUE, RenderMode->vfilter);
    }

    HuSprGrpKill(group);

    group = HuSprGrpCreate(1);
    if (selection == 0) {
        anim = HuSprAnimRead(HuDataSelHeapReadNum(PROGRESSIVE_ON_SPRITE_DATA, HU_MEMNUM_OVL, HEAP_MODEL));
    } else {
        anim = HuSprAnimRead(HuDataSelHeapReadNum(PROGRESSIVE_OFF_SPRITE_DATA, HU_MEMNUM_OVL, HEAP_MODEL));
    }
    sprite = HuSprCreate(anim, 0, 0);
    HuSprGrpMemberSet(group, 0, sprite);
    HuSprPosSet(group, 0, 288.0f, 240.0f);

    WipeCreate(WIPE_MODE_IN, WIPE_TYPE_NORMAL, 20);
    while (WipeCheck()) {
        HuPrcVSleep();
    }

    HuPrcSleep(60);
    for (frame = 0; frame < 180; frame++) {
        if ((HuPadBtnDown[0] | HuPadBtnDown[1] | HuPadBtnDown[2] | HuPadBtnDown[3]) & PAD_BUTTON_A) {
            break;
        }
        HuPrcVSleep();
    }

    WipeCreate(WIPE_MODE_OUT, WIPE_TYPE_NORMAL, 30);
    while (WipeCheck()) {
        HuPrcVSleep();
    }
    HuSprGrpKill(group);
    HuPrcSleep(30);
}

void CameraOutView(OMOBJ *object)
{
    static u16 cameraMask[] = { 1, 2 };
    s16 i;
    float rotX;
    float rotY;
    float rotZ;
    HuVecF pos;
    HuVecF target;
    HuVecF up;

    for (i = 0; i < 1; i++) {
        rotX = BootCameraRot[i].x;
        rotY = BootCameraRot[i].y;
        rotZ = BootCameraRot[i].z;

        pos.x = BootCameraCenter[i].x + (BootCameraZoom[i] *
            (sin(3.141592653589793 * rotY / 180.0) *
             cos(3.141592653589793 * rotX / 180.0)));
        pos.y = BootCameraCenter[i].y + (BootCameraZoom[i] *
            -sin(3.141592653589793 * rotX / 180.0));
        pos.z = BootCameraCenter[i].z + (BootCameraZoom[i] *
            (cos(3.141592653589793 * rotY / 180.0) *
             cos(3.141592653589793 * rotX / 180.0)));

        target.x = BootCameraCenter[i].x;
        target.y = BootCameraCenter[i].y;
        target.z = BootCameraCenter[i].z;

        up.x = sin(3.141592653589793 * rotY / 180.0) *
               sin(3.141592653589793 * rotX / 180.0);
        up.y = cos(3.141592653589793 * rotX / 180.0);
        up.z = cos(3.141592653589793 * rotY / 180.0) *
               sin(3.141592653589793 * rotX / 180.0);

        Hu3DCameraPosSet(cameraMask[i],
                         pos.x, pos.y, pos.z,
                         up.x, up.y, up.z,
                         target.x, target.y, target.z);
    }
}

void CameraMove(OMOBJ *object)
{
    static s32 moveF;
    HuVecF eye;
    HuVecF delta;
    HuVecF up;
    HuVecF roll;
    s8 subStick;
    float rotZ;

    if (!moveF) {
        return;
    }

    BootCameraRot[0].y += 0.1f * (float)HuPadStkX[0];
    BootCameraRot[0].x += 0.1f * (float)HuPadStkY[0];
    BootCameraZoom[0] += (float)(HuPadTrigL[0] / 2);
    BootCameraZoom[0] -= (float)(HuPadTrigR[0] / 2);
    if (BootCameraZoom[0] < 100.0f) {
        BootCameraZoom[0] = 100.0f;
    }

    eye.x = BootCameraCenter[0].x + (BootCameraZoom[0] *
        (sin(3.141592653589793 * BootCameraRot[0].y / 180.0) *
         cos(3.141592653589793 * BootCameraRot[0].x / 180.0)));
    eye.y = BootCameraCenter[0].y + (BootCameraZoom[0] *
        -sin(3.141592653589793 * BootCameraRot[0].x / 180.0));
    eye.z = BootCameraCenter[0].z + (BootCameraZoom[0] *
        (cos(3.141592653589793 * BootCameraRot[0].y / 180.0) *
         cos(3.141592653589793 * BootCameraRot[0].x / 180.0)));

    delta.x = BootCameraCenter[0].x - eye.x;
    delta.y = BootCameraCenter[0].y - eye.y;
    delta.z = BootCameraCenter[0].z - eye.z;

    up.x = sin(3.141592653589793 * BootCameraRot[0].y / 180.0) *
           sin(3.141592653589793 * BootCameraRot[0].x / 180.0);
    up.y = cos(3.141592653589793 * BootCameraRot[0].x / 180.0);
    up.z = cos(3.141592653589793 * BootCameraRot[0].y / 180.0) *
           sin(3.141592653589793 * BootCameraRot[0].x / 180.0);

    rotZ = BootCameraRot[0].z;

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
        BootCameraCenter[0].x += (delta.x * (float)subStick) * 0.05f;
        BootCameraCenter[0].y += (delta.y * (float)subStick) * 0.05f;
        BootCameraCenter[0].z += (delta.z * (float)subStick) * 0.05f;
    }

    PSVECNormalize(&roll, &delta);

    subStick = -(HuPadSubStkY[0] & 0xF8);
    if (subStick != 0) {
        BootCameraCenter[0].x += (delta.x * (float)subStick) * 0.05f;
        BootCameraCenter[0].y += (delta.y * (float)subStick) * 0.05f;
        BootCameraCenter[0].z += (delta.z * (float)subStick) * 0.05f;
    }
}

void BootTitleCreate(void)
{
    TitleMdlId[0] = Hu3DModelCreateData(TITLE_HSF_PARTY);
    TitleMdlId[1] = Hu3DModelCreateData(TITLE_HSF_SIX);
    TitleMdlId[2] = Hu3DModelCreateData(TITLE_HSF_STAR);

    Hu3DModelDispOff(TitleMdlId[0]);
    Hu3DModelDispOff(TitleMdlId[1]);
    Hu3DModelDispOff(TitleMdlId[2]);
    Hu3DModelLayerSet(TitleMdlId[0], 1);
}

s32 BootTitleExec(s32 firstRun)
{
    s16 frame;

    Hu3DCameraPerspectiveSet(1, 20.0f, 20.0f, 5000.0f, 1.2f);
    Hu3DCameraViewportSet(1, 0.0f, 0.0f,
                          640.0f, 480.0f, 0.0f, 1.0f);

    BootCameraRot[0].x = 0.0f;
    BootCameraRot[0].y = 0.0f;
    BootCameraRot[0].z = 0.0f;
    BootCameraCenter[0].x = 0.0f;
    BootCameraCenter[0].y = 0.0f;
    BootCameraCenter[0].z = 0.0f;
    BootCameraZoom[0] = 1000.0f;

    Hu3DModelAttrReset(TitleMdlId[2], HU3D_ATTR_DISPOFF);
    Hu3DMotionTimeSet(TitleMdlId[2], 0.0f);
    Hu3DMotionStartEndSet(TitleMdlId[2], 0.0f, 71.0f);
    Hu3DModelAttrReset(TitleMdlId[2], HU3D_MOTATTR_LOOP);
    Hu3DModelAttrReset(TitleMdlId[1], HU3D_ATTR_DISPOFF);
    Hu3DMotionTimeSet(TitleMdlId[1], 0.0f);
    Hu3DModelAttrSet(TitleMdlId[0], HU3D_MOTATTR_LOOP);
    Hu3DMotionStartEndSet(TitleMdlId[0], 0.0f, 59.0f);

    if (!firstRun) {
        WipeCreate(WIPE_MODE_IN, WIPE_TYPE_NORMAL, TITLE_WIPE_FRAMES);
        HuPrcSleep(TITLE_STREAM_DELAY_FRAMES);
        HuAudSStreamPlay(MSM_STREAM_TEST);
        WipeWait();
        while (Hu3DMotionTimeGet(TitleMdlId[2]) < 70.0f) {
            HuPrcVSleep();
        }
        Hu3DMotionStartEndSet(TitleMdlId[2], 70.0f, 500.0f);
    } else {
        Hu3DMotionTimeSet(TitleMdlId[2], 71.0f);
        Hu3DMotionStartEndSet(TitleMdlId[2], 70.0f, 500.0f);
        Hu3DMotionTimeSet(TitleMdlId[1], 70.0f);
        WipeCreate(WIPE_MODE_IN, WIPE_TYPE_NORMAL, TITLE_WIPE_FRAMES);
        HuPrcSleep(TITLE_STREAM_DELAY_FRAMES);
        HuAudSStreamPlay(MSM_STREAM_TEST);
        WipeWait();
    }

    Hu3DModelAttrSet(TitleMdlId[2], HU3D_MOTATTR_LOOP);
    Hu3DModelAttrReset(TitleMdlId[0], HU3D_ATTR_DISPOFF);
    for (frame = 0; frame < TITLE_INPUT_WAIT_FRAMES; frame++) {
        if (HuPadBtnDown[0] & PAD_BUTTON_START) {
            HuAudFXPlay(TITLE_CONFIRM_SE);
            Hu3DMotionStartEndSet(TitleMdlId[0], 0.0f, 90.0f);
            Hu3DModelAttrReset(TitleMdlId[0], HU3D_MOTATTR_LOOP);
            Hu3DMotionTimeSet(TitleMdlId[0], 60.0f);
            HuPrcSleep(TITLE_CONFIRM_WAIT_FRAMES);
            return 1;
        }
        HuPrcVSleep();
    }

    WipeColorSet(255, 255, 255);
    WipeCreate(WIPE_MODE_OUT, WIPE_TYPE_NORMAL, TITLE_WIPE_FRAMES);
    WipeWait();
    Hu3DModelAttrSet(TitleMdlId[0], HU3D_ATTR_DISPOFF);
    Hu3DModelAttrSet(TitleMdlId[2], HU3D_ATTR_DISPOFF);
    return 0;
}

void *NintendoDataDecode(void)
{
    u32 *src;
    u32 size;
    void *dst;
    s32 decodeType;

    src = (u32 *)nintendoData;
    size = *src++;
    dst = HuMemDirectMalloc(HEAP_MODEL, size);
    decodeType = *src++;
    if (dst) {
        HuDecodeData(src, dst, size, decodeType);
    }

    return dst;
}

void *WarningDataDecode(u32 *src)
{
    u32 size;
    void *dst;
    s32 decodeType;

    size = *src++;
    dst = HuMemDirectMalloc(HEAP_MODEL, size);
    decodeType = *src++;
    if (dst) {
        HuDecodeData(src, dst, size, decodeType);
    }

    return dst;
}

void BootWarningExec(void)
{
    HUSPR_GROUPID group;
    HUSPRID sprite;
    s16 phaseSign;
    ANIMDATA *anim;
    u32 *warningPacked;
    u32 warningSize;
    void *warningRawData;
    void *warningAnimData;
    s32 warningDecodeType;
    u32 *pressStartPacked;
    u32 pressStartSize;
    void *pressStartRawData;
    void *pressStartAnimData;
    s32 pressStartDecodeType;
    s16 frame;
    float scaleX;
    float scaleY;
    float tpLevel;
    float tpLevelStack[1];

    scaleX = 0.9473684430122375f;
    scaleY = 1.0714285373687744f;

    group = HuSprGrpCreate(WARNING_SPRITE_MEMBER_COUNT);

    warningPacked = warningData;
    warningSize = *warningPacked++;
    warningRawData = HuMemDirectMalloc(HEAP_MODEL, warningSize);
    warningDecodeType = *warningPacked++;
    if (warningRawData) {
        HuDecodeData(warningPacked, warningRawData, warningSize, warningDecodeType);
    }
    warningAnimData = warningRawData;
    anim = HuSprAnimRead(warningAnimData);
    sprite = HuSprCreate(anim, WARNING_SPRITE_PRIORITY, WARNING_SPRITE_BANK);
    HuSprGrpMemberSet(group, WARNING_SPRITE_MEMBER, sprite);
    HuSprPosSet(group, WARNING_SPRITE_MEMBER, 288.0f, 224.0f * scaleY);
    HuSprScaleSet(group, WARNING_SPRITE_MEMBER, scaleX, scaleY);

    pressStartPacked = warningPressStartData;
    pressStartSize = *pressStartPacked++;
    pressStartRawData = HuMemDirectMalloc(HEAP_MODEL, pressStartSize);
    pressStartDecodeType = *pressStartPacked++;
    if (pressStartRawData) {
        HuDecodeData(pressStartPacked, pressStartRawData, pressStartSize, pressStartDecodeType);
    }
    pressStartAnimData = pressStartRawData;
    anim = HuSprAnimRead(pressStartAnimData);
    sprite = HuSprCreate(anim, WARNING_SPRITE_PRIORITY, WARNING_SPRITE_BANK);
    HuSprGrpMemberSet(group, PRESS_START_SPRITE_MEMBER, sprite);
    HuSprPosSet(group, PRESS_START_SPRITE_MEMBER, 288.0f, 383.0f * scaleY);
    HuSprDispOff(group, PRESS_START_SPRITE_MEMBER);
    HuSprScaleSet(group, PRESS_START_SPRITE_MEMBER, scaleX, scaleY);

    WipeCreate(WIPE_MODE_IN, WIPE_TYPE_NORMAL, 30);
    WipeWait();

    HuPrcSleep(INITIAL_WAIT_FRAMES);
    tpLevelStack[0] = 0.0f;
    phaseSign = 1;
    HuSprDispOn(group, PRESS_START_SPRITE_MEMBER);
    frame = 0;
    while (frame < MAX_INPUT_WAIT_FRAMES &&
           !(HuPadBtnDown[0] | HuPadBtnDown[1] | HuPadBtnDown[2] | HuPadBtnDown[3])) {
        if (frame != 0 && (frame % BLINK_PERIOD_FRAMES) == 0) {
            phaseSign = -phaseSign;
        }

        tpLevel = (float)(frame % BLINK_PERIOD_FRAMES) / 30.0f;
        if (phaseSign < 0) {
            tpLevel = 1.0f - tpLevel;
        }

        HuSprTPLvlSet(group, PRESS_START_SPRITE_MEMBER, tpLevel);
        HuPrcVSleep();
        frame++;
    }

    HuSprDispOff(group, PRESS_START_SPRITE_MEMBER);
    WipeCreate(WIPE_MODE_OUT, WIPE_TYPE_NORMAL, 30);
    WipeWait();

    HuSprGrpKill(group);
    HuPrcSleep(30);
}

OMOBJ *OutViewObj;
OMOBJMAN *BootObjMan;
HuVecF BootCameraRot[2];
HuVecF BootCameraCenter[2];
float BootCameraZoom[2];
s16 BootLightId[3];
s16 TitleMdlId[4];
s16 lbl_1_bss_4;
