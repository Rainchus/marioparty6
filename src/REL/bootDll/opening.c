/* Block math.h: its "extern inline" sqrtf (compiled as C++ via #pragma cplusplus)
 * emits weak local-static pool data (_half/_three) into every including TU's .rodata.
 * The original opening object's copy was discarded at link in favor of boot's, so the
 * split target opening object must not contain it. opening.c only needs sin/cos/M_PI. */
#define _MATH_H
#define M_PI 3.141592653589793
double sin(double x);
double cos(double x);
#pragma pool_data off

#include "dolphin.h"
#include "humath.h"
#include "game/audio.h"
#include "game/data.h"
#include "game/frand.h"
#include "game/hu3d.h"
#include "game/memory.h"
#include "game/object.h"
#include "game/pad.h"
#include "game/process.h"
#include "game/sprite.h"
#include "game/window.h"
#include "game/wipe.h"

enum {
    OPENING_CAMERA_BIT = 1,

    OPENING_MODEL_BOOK = 1,
    OPENING_MODEL_LIGHT = 3,
    OPENING_MODEL_STAR_CORE = 4,
    OPENING_MODEL_STAR_SIDE_A = 5,
    OPENING_MODEL_STAR_SIDE_B = 6,

    CHAR_EVENT_COUNT = 10,
    CHAR_EVENT_INITIAL_RANDOM_MAX = 30,
    CHAR_EVENT_FRAME_MARGIN = 10,
    CHAR_EVENT_FINAL_WAIT = 11,

    CHAR_FOCUS_RANDOM_FRAME_BASE = 5,
    CHAR_FOCUS_RANDOM_FOCUS_CHAR = 0,
    CHAR_FOCUS_RANDOM_FOCUS_FRAME = 0x4E,
    CHAR_FOCUS_RANDOM_LATE_THRESHOLD = 0xF0,
    CHAR_FOCUS_RANDOM_REARM_BASE = 10,

    GUIDE_FOCUS_SELECTOR_CHAR_MASK = 0xFF,
    GUIDE_FOCUS_SELECTOR_ALT = 0x8000,
    GUIDE_FOCUS_CHAR_A = 10,

    POSTDRAW_COPY_X = 0,
    POSTDRAW_COPY_Y = 0x50,
    POSTDRAW_COPY_W = 0x280,
    POSTDRAW_COPY_H = 0x140,
    POSTDRAW_COPY_MIPMAP = 0,
};

#define CHAR_FOCUS_RANDOM_REARM_SCALE 30.0f
#define GUIDE_FOCUS_VALUE_REST 1.0f
#define GUIDE_FOCUS_VALUE_LIT 1.2f
#define OBJSTAR_X_AMPLITUDE 3.0
#define OBJSTAR_Y_AMPLITUDE 5.0
#define OBJSTAR_PHASE_WRAP_DEG 720.0f
#define LIGHT_SCALE_MAX 0.8999999761581421f
#define LIGHT_SCALE_Z 1.0f
#define SIDE_STAR_SCALE_MAX 0.5f
#define SIDE_STAR_SCALE_Z 0.5f
#define EndScale(maxScale, ratio) ((maxScale) * (1.0 - HuSin(90.0f * (ratio))))
#define BOOK_PHASE_WRAP_DEG 720.0f
#define BOOK_Y_PHASE_SCALE 2.0f
#define BOOK_X_RADIUS_SCALE 5.0
#define BOOK_Y_RADIUS_SCALE 20.0

typedef struct OpeningFocusObject {
    u8 reserved_00[0x34];
    float focus34;
    float focus38;
} OpeningFocusObject;


extern s16 FbCopyMode;
extern s32 OpeningEndF;
extern s32 CharFocusCancelF;
extern s32 CharLightFocusF;
extern float FilterTPLvl;
extern HuVecF StarPos;
extern float OpeningBgRotZ;
extern HuVec2f GuideDist;
extern s16 OpeningBgState;
extern s16 OpeningWinId[4];
extern float BookRadius;
extern HuVecF BookRot;
extern HuVecF BookOfs;
extern HuVecF BookPos;
extern s16 OpeningCharAnimId[12];
extern s16 BookFrameBufAnimId[2];
extern ANIMDATA *OpeningCharAnim[79];
extern ANIMDATA *BookFrameBufAnim[2];
extern void *FilterFrameBuf;
extern void *BookFrameBuf[2];
extern s16 lbl_1_bss_224;
extern s16 OpeningMdlId[8];
extern s16 WinFrameTypeTbl[4];
extern u32 OpeningCharFileTbl[80];
extern char *OpeningCharTexNameTbl[12];
extern char *OpeningCharObjNameTbl[12];
extern HuVecF BookDefPos;
extern HuVecF BookDefRot;
extern char lbl_1_data_8988[0xE];
extern char lbl_1_data_8996[0xF];
extern char lbl_1_data_89A5[0x14];


extern s16 BootLightId[3];

void OpeningBgUpdate(void);
void OpeningDispOff(void);
void OpeningExec(s32 enablePrompt);
void OpeningEventExec(void);
void OpeningKill(void);
static void FilterDraw(float alpha);
static void SetStarDist(s16 modelIndex, float distance);
static void RotateGuide(u32 frameCount);
static void CharWaitEvent(u32 frameCount);
static void BlinkGuide(u32 frameCount);
static void BookOpen(void);
static void CharObjLightStart(u32 frameCount);
static void CharObjStarStart(u32 frameCount);
static void ObjStarStart(u32 frameCount);
static void ObjStarEnd(void);
static void CharHappyEvent(u32 frameCount);
static void MessStart(s16 arg0, u32 message, s16 frameCount);

static void EventCreate(void (*callback)(void), s32 arg0, s32 arg1);
static void PreDrawHook(s16 layerNo);
static void PostDrawHook(s16 layerNo);
static void GuideCharFocus(u32 guideChar, s32 frameCount);
static void SetCharAnimFrame(s16 animIndex, s16 frameIndex);
static void SwapAnimFrame(s16 animIndex, s16 frameIndex);
static void SwapAnimFrameEvent(s32 animIndex, s32 frameIndex);
static void CharObjLightEnd(u32 frameCount);
static void CharObjStarEnd(u32 frameCount);
static void ObjStarExec(void);
static void CharFocusLeft(void);
static void CharFocusRight(void);
static void CharFocusRandom(u32 frameCount);
static void BookOpenEvent(void);
static void BookExec(void);
static void MoveGuide(s32 modelIndex, s32 dist);
static void OpeningCameraSet(HuVecF *rotDeg, HuVecF *target, float distance);

void OpeningCreate(void)
{
    void *temp;
    s16 i;

    OpeningMdlId[0] = Hu3DModelCreate(HuDataSelHeapReadNum(0x00D50016, HU_MEMNUM_OVL, HEAP_MODEL));
    Hu3DModelLayerSet(OpeningMdlId[0], 1);
    Hu3DMotionSpeedSet(OpeningMdlId[0], 0.015f);
    Hu3DModelPosSet(OpeningMdlId[0], 0.0f, 0.0f, -4500.f);

    OpeningMdlId[1] = Hu3DModelCreate(HuDataSelHeapReadNum(0x00D50017, HU_MEMNUM_OVL, HEAP_MODEL));
    Hu3DModelLayerSet(OpeningMdlId[1], 1);
    Hu3DMotionSpeedSet(OpeningMdlId[1], 0.5f);
    Hu3DMotionClusterSpeedSet(OpeningMdlId[1], 0, 0.5f);
    Hu3DModelPosSet(OpeningMdlId[1], 0.0f, 0.0f, -2000.f);
    Hu3DModelRotSet(OpeningMdlId[1], -20.f, -30.f, 30.f);

    OpeningMdlId[2] = Hu3DModelCreate(HuDataSelHeapReadNum(0x00D50018, HU_MEMNUM_OVL, HEAP_MODEL));
    Hu3DModelLayerSet(OpeningMdlId[2], 0);
    Hu3DModelPosSet(OpeningMdlId[2], 0.0f, -330.f, 0.0f);

    OpeningMdlId[3] = Hu3DModelCreate(HuDataSelHeapReadNum(0x00D50019, HU_MEMNUM_OVL, HEAP_MODEL));
    Hu3DModelLayerSet(OpeningMdlId[3], 1);

    OpeningMdlId[4] = Hu3DModelCreate(HuDataSelHeapReadNum(0x00D5001A, HU_MEMNUM_OVL, HEAP_MODEL));
    Hu3DModelLayerSet(OpeningMdlId[4], 1);

    OpeningMdlId[5] = Hu3DModelCreate(HuDataSelHeapReadNum(0x00D5001C, HU_MEMNUM_OVL, HEAP_MODEL));
    Hu3DModelLayerSet(OpeningMdlId[5], 1);

    OpeningMdlId[6] = Hu3DModelCreate(HuDataSelHeapReadNum(0x00D5001D, HU_MEMNUM_OVL, HEAP_MODEL));
    Hu3DModelLayerSet(OpeningMdlId[6], 1);
    Hu3DModelAttrSet(OpeningMdlId[6], 0x40000001);

    OpeningMdlId[7] = Hu3DModelCreate(HuDataSelHeapReadNum(0x00D5001B, HU_MEMNUM_OVL, HEAP_MODEL));
    Hu3DModelLayerSet(OpeningMdlId[7], 1);
    Hu3DMotionSpeedSet(OpeningMdlId[7], 2.0f);
    Hu3DModelScaleSet(OpeningMdlId[7], 2.0f, 2.0f, 2.0f);

    temp = HuMemDirectMallocNum(HEAP_DVD, 0x100000, HU_MEMNUM_OVL);
    BookFrameBuf[0] = HuMemDirectMallocNum(
        HEAP_DVD,
        GXGetTexBufferSize(POSTDRAW_COPY_W, POSTDRAW_COPY_H, GX_TF_RGBA8, POSTDRAW_COPY_MIPMAP, 0),
        HU_MEMNUM_OVL);
    HuMemDirectFree(temp);
    BookFrameBuf[1] = HuMemDirectMallocNum(
        HEAP_HEAP,
        GXGetTexBufferSize(POSTDRAW_COPY_W, POSTDRAW_COPY_H, GX_TF_RGBA8, POSTDRAW_COPY_MIPMAP, 0),
        HU_MEMNUM_OVL);

    BookFrameBufAnim[0] = HuSprAnimMake(POSTDRAW_COPY_W, POSTDRAW_COPY_H, 0);
    BookFrameBufAnim[0]->bmp->data = BookFrameBuf[0];
    BookFrameBufAnim[1] = HuSprAnimMake(POSTDRAW_COPY_W, POSTDRAW_COPY_H, 0);
    BookFrameBufAnim[1]->bmp->data = BookFrameBuf[1];
    BookFrameBufAnimId[0] = Hu3DAnimCreate(BookFrameBufAnim[0], OpeningMdlId[1], lbl_1_data_8988);
    BookFrameBufAnimId[1] = Hu3DAnimCreate(BookFrameBufAnim[1], OpeningMdlId[1], lbl_1_data_8996);

    FilterFrameBuf = HuMemDirectMallocNum(
        HEAP_HEAP,
        GXGetTexBufferSize(POSTDRAW_COPY_W, POSTDRAW_COPY_H, GX_TF_I8, POSTDRAW_COPY_MIPMAP, 0),
        HU_MEMNUM_OVL);

    for (i = 0; i < 79; i++) {
        OpeningCharAnim[i] = HuSprAnimRead(
            HuDataSelHeapReadNum(OpeningCharFileTbl[i], HU_MEMNUM_OVL, HEAP_MODEL));
    }

    for (i = 0; i < 12; i++) {
        OpeningCharAnimId[i] = Hu3DAnimCreate(OpeningCharAnim[0], OpeningMdlId[2], OpeningCharTexNameTbl[i]);
    }

    HuPrcChildCreate(BookExec, 0x100, 0x1000, 0, HuPrcCurrentGet());

    BookOfs.x = BookOfs.y = BookOfs.z = 0.0f;
    BookPos = BookDefPos;
    BookRot = BookDefRot;
    BookRadius = 1.0f;

    OpeningDispOff();
}

void OpeningDispOff(void)
{
    s16 i;

    for (i = 0; i < 8; i++) {
        Hu3DModelAttrSet(OpeningMdlId[i], HU3D_ATTR_DISPOFF);
    }
}

void OpeningExec(s32 enablePrompt)
{
    HUPROCESS *eventProcess;
    HUWINID messageWinId;
    Mtx matrix;
    u32 frame;
    s16 i;
    s16 j;

    Hu3DModelAttrSet(OpeningMdlId[3], HU3D_ATTR_DISPOFF);
    Hu3DModelAttrSet(OpeningMdlId[4], HU3D_ATTR_DISPOFF);
    Hu3DModelAttrSet(OpeningMdlId[5], HU3D_ATTR_DISPOFF);
    Hu3DModelAttrSet(OpeningMdlId[6], HU3D_ATTR_DISPOFF);
    Hu3DModelAttrSet(OpeningMdlId[7], HU3D_ATTR_DISPOFF);
    Hu3DModelAttrReset(OpeningMdlId[0], HU3D_ATTR_DISPOFF);
    Hu3DModelAttrReset(OpeningMdlId[1], HU3D_ATTR_DISPOFF);
    Hu3DModelAttrReset(OpeningMdlId[2], HU3D_ATTR_DISPOFF);

    Hu3DCameraLayerHookSet(OPENING_CAMERA_BIT, 0, PreDrawHook);
    Hu3DCameraLayerHookSet(OPENING_CAMERA_BIT, 8, PostDrawHook);
    Hu3DModelRotSet(OpeningMdlId[5], 0.0f, 0.0f, 0.0f);

    FbCopyMode = 0;
    CharFocusCancelF = 0;
    CharLightFocusF = 0;
    Hu3DAmbColorSet(1.0f, 1.0f, 1.0f);
    Hu3DShineSet(FALSE);

    BookOfs.x = BookOfs.y = BookOfs.z = 0.0f;
    BookPos = BookDefPos;
    BookRot = BookDefRot;
    BookRadius = 1.0f;

    Hu3DMotionTimeSet(OpeningMdlId[1], 0.0f);
    Hu3DModelAttrSet(OpeningMdlId[1], 0x40000002);
    Hu3DMotionClusterTimeSet(OpeningMdlId[1], 0, 0.0f);
    Hu3DModelClusterAttrSet(OpeningMdlId[1], 0, 0xC0000002);

    PSMTXTrans(matrix, 150.f, 0.0f, 0.0f);
    Hu3DModelMtxSet(OpeningMdlId[1], &matrix);

    SetCharAnimFrame(0, 0);
    SetCharAnimFrame(1, 7);
    SetCharAnimFrame(2, 0x0E);
    SetCharAnimFrame(3, 0x15);
    SetCharAnimFrame(4, 0x1C);
    SetCharAnimFrame(5, 0x23);
    SetCharAnimFrame(6, 0x2A);
    SetCharAnimFrame(7, 0x31);
    SetCharAnimFrame(8, 0x38);
    SetCharAnimFrame(9, 0x3F);
    SetCharAnimFrame(10, 0x46);
    SetCharAnimFrame(11, 0x4A);

    for (i = 0; i < 4; i++) {
        OpeningWinId[i] = HuWinExCreateFrame(
            -10000.f,
            364.0f,
            0x220,
            0x44,
            -1,
            WinFrameTypeTbl[i]);
        HuWinAttrSet(OpeningWinId[i], 0x800);
    }

    GuideDist.x = -225.0f;
    GuideDist.y = -135.0f;
    SetStarDist(10, GuideDist.x);
    SetStarDist(11, GuideDist.y);

    messageWinId = HuWinCreate(16.0f, 326.0f, 0x220, 0x2A, 0);
    HuWinAttrSet(messageWinId, 0x800);
    HuWinPriSet(messageWinId, 100);
    HuWinBGTPLvlSet(messageWinId, 0.0f);
    HuWinMesSpeedSet(messageWinId, 0);
    HuWinMesSet(messageWinId, 0x10005);
    HuWinDispOff(messageWinId);

    Hu3DGLightPosSet(BootLightId[0],
                     300.f, 1000.f, 1000.f,
                     -0.3f, -1.0f, -1.0f);
    Hu3DGLightColorSet(BootLightId[0], 255, 255, 255, 255);
    Hu3DGLightColorSet(BootLightId[1], 0, 0, 0, 255);

    HuAudSStreamPlay(0);
    WipeCreate(WIPE_MODE_IN, WIPE_TYPE_NORMAL, 30);
    while (WipeCheck()) {
        HuPrcVSleep();
    }

    OpeningEndF = 0;
    eventProcess = HuPrcChildCreate(OpeningEventExec, 0x100, 0x3000, 0, HuPrcCurrentGet());

    frame = 0;
    while (OpeningEndF == 0) {
        if (enablePrompt != 0) {
            if (frame > 0xFA && omcurovl != 1) {
                HuWinDispOn(messageWinId);
            }

            if (omcurovl == 1 && (HuPadBtnDown[0] & PAD_BUTTON_A)) {
                break;
            }
            if (HuPadBtnDown[0] & PAD_BUTTON_START) {
                break;
            }
        }

        frame++;
        HuPrcVSleep();
    }

    HuWinDispOff(messageWinId);
    HuAudSStreamAllFadeOut(1000);
    WipeCreate(WIPE_MODE_OUT, WIPE_TYPE_NORMAL, 30);
    while (WipeCheck()) {
        HuPrcVSleep();
    }

    HuWinKill(messageWinId);
    HuPrcKill(eventProcess);
    Hu3DCameraLayerHookReset(OPENING_CAMERA_BIT, 0);
    Hu3DCameraLayerHookReset(OPENING_CAMERA_BIT, 8);

    for (i = 0; i < 4; i++) {
        HuWinExKill(OpeningWinId[i]);
    }

    for (j = 0; j < 8; j++) {
        Hu3DModelAttrSet(OpeningMdlId[j], HU3D_ATTR_DISPOFF);
    }
}

void OpeningEventExec(void)
{
    OpeningBgState = 0;
    EventCreate(OpeningBgUpdate, 0, 0);
    EventCreate((void (*)(void))RotateGuide, 0x41A, 0);
    EventCreate((void (*)(void))CharWaitEvent, 0x3DE, 0);
    EventCreate((void (*)(void))BlinkGuide, 0x3F2, 0);
    EventCreate(ObjStarExec, 0, 0);
    BookOpen();

    HuPrcSleep(60);
    HuWinExOpen(OpeningWinId[0]);
    MessStart(0, 0x000B0000, 0xF0);
    MessStart(0, 0x000B0001, 0xF0);
    MessStart(0, 0x000B0002, 0xF0);
    MessStart(0, 0x000B0003, 0x50);
    EventCreate((void (*)(void))GuideCharFocus, 10, 60);
    EventCreate(CharFocusLeft, 0, 0);
    HuPrcSleep(0xA0);

    while (OpeningBgRotZ > -200.f) {
        HuPrcVSleep();
    }

    OpeningBgState = 2;
    EventCreate((void (*)(void))GuideCharFocus, 0x800B, 60);
    EventCreate(CharFocusRight, 0, 0);
    MessStart(0, 0x000B0004, 0x50);
    EventCreate((void (*)(void))GuideCharFocus, 0x800A, 40);
    EventCreate(CharFocusLeft, 0, 0);

    OpeningBgState = 3;
    MessStart(0, 0x000B0005, 70);
    EventCreate((void (*)(void))GuideCharFocus, 0x800B, 40);
    EventCreate(CharFocusRight, 0, 0);

    OpeningBgState = 4;
    MessStart(0, 0x000B0006, 60);
    EventCreate((void (*)(void))GuideCharFocus, 0x800A, 40);
    EventCreate(CharFocusLeft, 0, 0);

    OpeningBgState = 3;
    MessStart(0, 0x000B0007, 60);
    EventCreate((void (*)(void))GuideCharFocus, 0x800B, 40);
    EventCreate(CharFocusRight, 0, 0);

    OpeningBgState = 4;
    MessStart(0, 0x000B0006, 50);
    EventCreate((void (*)(void))GuideCharFocus, 0x800A, 40);
    EventCreate(CharFocusLeft, 0, 0);

    OpeningBgState = 3;
    MessStart(0, 0x000B0007, 50);

    OpeningBgState = 5;
    EventCreate((void (*)(void))GuideCharFocus, 0x800A, 10000);
    EventCreate((void (*)(void))GuideCharFocus, 0x800B, 10000);
    EventCreate((void (*)(void))CharFocusRandom, 600, 0);
    MessStart(0, 0x000B0008, 0xF0);
    MessStart(0, 0x000B0009, 0xF0);
    HuPrcSleep(120);

    HuWinExClose(OpeningWinId[0]);
    CharLightFocusF = 1;
    CharFocusCancelF = 1;
    EventCreate((void (*)(void))CharObjLightStart, 20, 0);
    SwapAnimFrame(0, 4);
    HuWinExOpen(OpeningWinId[0]);
    MessStart(0, 0x000B000A, 0);
    HuPrcSleep(20);

    SwapAnimFrame(1, 0x0B);
    SwapAnimFrame(2, 0x12);
    SwapAnimFrame(3, 0x19);
    SwapAnimFrame(4, 0x20);
    SwapAnimFrame(5, 0x27);
    SwapAnimFrame(6, 0x2E);
    SwapAnimFrame(7, 0x35);
    HuPrcSleep(30);
    SwapAnimFrame(8, 0x39);
    SwapAnimFrame(9, 0x40);
    HuPrcSleep(60);

    EventCreate((void (*)(void))CharObjLightEnd, 20, 0);
    HuPrcSleep(30);
    MessStart(0, 0x000B000B, 10);
    EventCreate((void (*)(void))CharObjStarStart, 30, 0);
    HuPrcSleep(60);

    CharLightFocusF = CharFocusCancelF = 0;
    EventCreate((void (*)(void))GuideCharFocus, 0x800A, 20);
    EventCreate((void (*)(void))GuideCharFocus, 0x800B, 20);
    HuPrcSleep(0xAA);

    MessStart(0, 0x000B000C, 0xF0);
    MessStart(0, 0x000B000D, 0xF0);
    MessStart(0, 0x000B000E, 40);
    EventCreate((void (*)(void))CharHappyEvent, 200, 0);
    HuPrcSleep(200);
    EventCreate((void (*)(void))CharObjStarEnd, 20, 0);

    HuWinExClose(OpeningWinId[0]);
    FbCopyMode = 1;
    Hu3DModelAttrReset(OpeningMdlId[1], 0x40000002);
    Hu3DModelClusterAttrReset(OpeningMdlId[1], 0, 0xC0000002);
    HuPrcSleep(10);
    SetCharAnimFrame(10, 0x48);
    SetCharAnimFrame(11, 0x4C);
    EventCreate((void (*)(void))CharHappyEvent, 0x186, 0);
    HuPrcSleep(0x8C);

    HuWinExOpen(OpeningWinId[0]);
    EventCreate((void (*)(void))ObjStarStart, 30, 0);
    MessStart(0, 0x000B000F, 0xF0);
    FbCopyMode = 2;
    HuWinExClose(OpeningWinId[0]);
    HuPrcSleep(0x82);

    HuWinExOpen(OpeningWinId[0]);
    MessStart(0, 0x000B0010, 0x12C);
    HuWinExClose(OpeningWinId[0]);
    EventCreate(ObjStarEnd, 0, 0);
    HuPrcSleep(0x8C);

    OpeningEndF = 1;
    while (TRUE) {
        HuPrcVSleep();
    }
}

static void PreDrawHook(s16 layerNo)
{
    HuVecF rotDeg;
    HuVecF target;

    Hu3DCameraPerspectiveSet(OPENING_CAMERA_BIT, 45.0f, 20.f, 8000.f, 1.2f);
    Hu3DCameraViewportSet(OPENING_CAMERA_BIT, 0.0f, 0.0f, 640.f, 480.f, 0.0f, 1.0f);
    rotDeg.x = rotDeg.y = rotDeg.z = 0.0f;
    target.x = 0.0f;
    target.y = 0.0f;
    target.z = 0.0f;
    OpeningCameraSet(&rotDeg, &target, 450.f);
}

static void PostDrawHook(s16 layerNo)
{
    HuVecF rotDeg;
    HuVecF target;

    Hu3DCameraPerspectiveSet(OPENING_CAMERA_BIT, 30.f, 20.f, 15000.f, 1.2f);
    Hu3DCameraViewportSet(OPENING_CAMERA_BIT, 0.0f, 0.0f, 640.f, 480.f, 0.0f, 1.0f);
    rotDeg.x = rotDeg.y = rotDeg.z = 0.0f;
    target.x = target.y = target.z = 0.0f;
    OpeningCameraSet(&rotDeg, &target, 500.f);

    switch (FbCopyMode) {
    case 0:
        Hu3DFbCopyExec(
            POSTDRAW_COPY_X,
            POSTDRAW_COPY_Y,
            POSTDRAW_COPY_W,
            POSTDRAW_COPY_H,
            GX_TF_RGBA8,
            POSTDRAW_COPY_MIPMAP,
            BookFrameBuf[0]);
        FilterTPLvl = 0.0f;
        break;
    case 1:
        Hu3DFbCopyExec(
            POSTDRAW_COPY_X,
            POSTDRAW_COPY_Y,
            POSTDRAW_COPY_W,
            POSTDRAW_COPY_H,
            GX_TF_RGBA8,
            POSTDRAW_COPY_MIPMAP,
            BookFrameBuf[1]);
        break;
    case 2:
        FilterTPLvl += 0.01f;
        if (FilterTPLvl > 0.9f) {
            FilterTPLvl = 0.9f;
        }
        Hu3DFbCopyExec(
            POSTDRAW_COPY_X,
            POSTDRAW_COPY_Y,
            POSTDRAW_COPY_W,
            POSTDRAW_COPY_H,
            GX_TF_I8,
            POSTDRAW_COPY_MIPMAP,
            FilterFrameBuf);
        FilterDraw(FilterTPLvl);
        Hu3DFbCopyExec(
            POSTDRAW_COPY_X,
            POSTDRAW_COPY_Y,
            POSTDRAW_COPY_W,
            POSTDRAW_COPY_H,
            GX_TF_RGBA8,
            POSTDRAW_COPY_MIPMAP,
            BookFrameBuf[1]);
        break;
    default:
        break;
    }

    Hu3DZClear();
}

void OpeningKill(void)
{
    Hu3DAnimKill(BookFrameBufAnimId[0]);
    Hu3DAnimKill(BookFrameBufAnimId[1]);
    HuSprAnimKill(BookFrameBufAnim[0]);
    HuSprAnimKill(BookFrameBufAnim[1]);
}

static void OpeningBgUpdate(void)
{
    HSF_OBJECT *bgObj;
    s16 prevState;
    s32 counter;
    s32 frameNo;
    float temp;
    float savedRotZ;

    prevState = -1;
    OpeningBgState = 0;
    bgObj = Hu3DModelObjPtrGet(OpeningMdlId[2], lbl_1_data_89A5);
    counter = 0;
    while (TRUE) {
        if (prevState != OpeningBgState) {
            counter = 0;
        }
        prevState = OpeningBgState;
        switch (OpeningBgState) {
        case 0:
            counter++;
            if (counter > 1000) {
                break;
            }
            temp = (float)sin(3.141592653589793 * (90.f * (counter / 1000.f)) / 180.);
            bgObj->mesh.base.rot.z = -200.f * temp;
            frameNo = (s32)(bgObj->mesh.base.rot.z / 19.0f);
            if (bgObj->mesh.base.rot.z > -120.f) {
                GuideDist.x = -45.0f;
            } else {
                GuideDist.x = -225.0f;
            }
            SetStarDist(10, GuideDist.x);
            frameNo = (s32)(bgObj->mesh.base.rot.z / 19.0f);
            GuideDist.y = -135.0f;
            SetStarDist(11, GuideDist.y);
            OpeningBgRotZ = bgObj->mesh.base.rot.z;
            break;
        case 2:
            counter++;
            if (counter > 30) {
                break;
            }
            temp = counter / 30.f;
            bgObj->mesh.base.rot.z = -200. +
                20. * sin(3.141592653589793 * (90.f * temp) / 180.);
            if (counter == 1) {
                EventCreate((void (*)(void))MoveGuide, 11, -150);
            }
            if (15.0 == counter) {
                EventCreate((void (*)(void))MoveGuide, 10, -210);
            }
            break;
        case 3:
            if (counter == 0) {
                savedRotZ = bgObj->mesh.base.rot.z;
            }
            counter++;
            if (counter > 30) {
                break;
            }
            temp = counter / 30.f;
            bgObj->mesh.base.rot.z = savedRotZ +
                (-200.f - savedRotZ) * sin(3.141592653589793 * (90.f * temp) / 180.);
            break;
        case 4:
            if (counter == 0) {
                savedRotZ = bgObj->mesh.base.rot.z;
            }
            counter++;
            if (counter > 30) {
                break;
            }
            temp = counter / 30.f;
            bgObj->mesh.base.rot.z = savedRotZ +
                (-160.f - savedRotZ) * sin(3.141592653589793 * (90.f * temp) / 180.);
            break;
        case 5:
            if (counter == 0) {
                savedRotZ = bgObj->mesh.base.rot.z;
            }
            counter++;
            if (counter > 30) {
                break;
            }
            temp = counter / 30.f;
            bgObj->mesh.base.rot.z = savedRotZ +
                (-180.f - savedRotZ) * sin(3.141592653589793 * (90.f * temp) / 180.);
            break;
        }
        HuPrcVSleep();
    }
}

static void MoveGuide(s32 modelIndex, s32 dist)
{
    s16 i;
    float startDist;
    float wave;

    startDist = (&GuideDist.x)[modelIndex - 10];
    for (i = 1; i <= 20; i++) {
        wave = (float)sin(3.141592653589793 * (90.f * (i / 20.f)) / 180.);
        SetStarDist(modelIndex, startDist + wave * ((float)dist - startDist));
        HuPrcVSleep();
    }
}

static void BlinkGuide(u32 frameCount)
{
    s16 phase;
    s16 count;
    s16 i;

    SetCharAnimFrame(10, 0x47);
    SetCharAnimFrame(11, 0x4B);
    phase = 1;
    for (i = 1, count = 0; i <= frameCount; i++) {
        if (count++ > 10) {
            phase ^= 1;
            SetCharAnimFrame(10, phase + 0x46);
            SetCharAnimFrame(11, phase + 0x4A);
            count = 0;
        }
        HuPrcVSleep();
    }
    SetCharAnimFrame(10, 0x46);
    SetCharAnimFrame(11, 0x4A);
}

static void GuideCharFocus(u32 guideChar, s32 frameCount)
{
    s16 frame;
    s16 toggleCounter;
    s16 phase;
    BOOL alternate;
    float focusValue;
    OpeningFocusObject *object;

    alternate = guideChar & GUIDE_FOCUS_SELECTOR_ALT;
    CharFocusCancelF = 0;
    guideChar &= GUIDE_FOCUS_SELECTOR_CHAR_MASK;
    object = (OpeningFocusObject *)Hu3DModelObjPtrGet(
        OpeningMdlId[2],
        OpeningCharObjNameTbl[guideChar]);

    if (guideChar == GUIDE_FOCUS_CHAR_A) {
        if (!alternate) {
            SetCharAnimFrame((s16)guideChar, 0x47);
        } else {
            SetCharAnimFrame((s16)guideChar, 0x49);
        }
    } else {
        if (!alternate) {
            SetCharAnimFrame((s16)guideChar, 0x4B);
        } else {
            SetCharAnimFrame((s16)guideChar, 0x4D);
        }
    }

    focusValue = GUIDE_FOCUS_VALUE_LIT;
    phase = 1;
    for (frame = 1, toggleCounter = 0; frame <= frameCount; frame++) {
        object->focus38 = focusValue;
        object->focus34 = focusValue;

        if (toggleCounter++ > 5) {
            phase ^= 1;
            if (CharLightFocusF != 0) {
                focusValue = GUIDE_FOCUS_VALUE_REST;
            } else {
                focusValue = (focusValue == GUIDE_FOCUS_VALUE_REST) ? GUIDE_FOCUS_VALUE_LIT
                                                                    : GUIDE_FOCUS_VALUE_REST;
            }

            if (guideChar == GUIDE_FOCUS_CHAR_A) {
                if (!alternate) {
                    SetCharAnimFrame((s16)guideChar, (s16)(0x46 + phase));
                } else {
                    SetCharAnimFrame((s16)guideChar, (s16)(0x48 + phase));
                }
            } else {
                if (!alternate) {
                    SetCharAnimFrame((s16)guideChar, (s16)(0x4A + phase));
                } else {
                    SetCharAnimFrame((s16)guideChar, (s16)(0x4C + phase));
                }
            }
            toggleCounter = 0;
        }

        if (CharFocusCancelF != 0) {
            break;
        }

        HuPrcVSleep();
    }

    if (guideChar == GUIDE_FOCUS_CHAR_A) {
        if (!alternate) {
            SetCharAnimFrame((s16)guideChar, 0x47);
        } else {
            SetCharAnimFrame((s16)guideChar, 0x49);
        }
    } else {
        if (!alternate) {
            SetCharAnimFrame((s16)guideChar, 0x4B);
        } else {
            SetCharAnimFrame((s16)guideChar, 0x4D);
        }
    }
    object->focus34 = object->focus38 = GUIDE_FOCUS_VALUE_REST;
}

static void RotateGuide(u32 frameCount)
{
    HSF_OBJECT *bgObject;
    HSF_OBJECT *object;
    float phaseDeg;
    float amplitude;
    s16 i;

    phaseDeg = 0.0f;
    amplitude = 10.f;
    bgObject = Hu3DModelObjPtrGet(OpeningMdlId[2], lbl_1_data_89A5);

    for (i = 1; ; i++) {
        object = Hu3DModelObjPtrGet(OpeningMdlId[2], OpeningCharObjNameTbl[10]);
        object->mesh.base.rot.z = -bgObject->mesh.base.rot.z;
        object->mesh.base.rot.z += amplitude * sin(3.141592653589793 * phaseDeg / 180.);

        object = Hu3DModelObjPtrGet(OpeningMdlId[2], OpeningCharObjNameTbl[11]);
        object->mesh.base.rot.z = -bgObject->mesh.base.rot.z;
        object->mesh.base.rot.z += amplitude * cos(3.141592653589793 * phaseDeg / 180.);

        phaseDeg += 5.0f;

        if (i > frameCount - 10) {
            amplitude -= 1.0f;
            if (amplitude < 0.0f) {
                amplitude = 0.0f;
            }
        }

        HuPrcVSleep();
    }
}

static void CharObjLightStart(u32 frameCount)
{
    HuVecF pos;
    s16 i;
    s16 endFrame;
    s16 startFrame;
    float scaleX;
    float scaleY;

    Hu3DModelAttrReset(OpeningMdlId[OPENING_MODEL_LIGHT], HU3D_ATTR_DISPOFF);
    Hu3DModelPosGet(OpeningMdlId[OPENING_MODEL_BOOK], &pos);
    pos.x += 30.f;
    pos.y -= 45.0f;
    pos.z += 20.f;
    Hu3DModelPosSetV(OpeningMdlId[OPENING_MODEL_LIGHT], &pos);
    Hu3DModelScaleSet(OpeningMdlId[OPENING_MODEL_LIGHT], 0.0f, 0.0f, 0.0f);
    HuAudFXPlay(0x4b0);

    for (i = 1; i <= frameCount; i++) {
        if (i <= 0.7 * frameCount) {
            endFrame = 0.7 * frameCount;
            scaleY = 0.8999999761581421 *
                     (sin(3.141592653589793 * (130.f * ((float)i / (float)endFrame)) / 180.) *
                      (1.0 / sin(2.2689280275926285)));
        }
        if (i > 0.3 * frameCount) {
            startFrame = (float)i - 0.3f * frameCount;
            endFrame = 0.7f * frameCount;
            scaleX = 0.8999999761581421 *
                     (sin(3.141592653589793 * (130.f * ((float)startFrame / (float)endFrame)) / 180.) *
                      (1.0 / sin(2.2689280275926285)));
        }
        Hu3DModelScaleSet(OpeningMdlId[OPENING_MODEL_LIGHT], scaleX, scaleY, 1.0f);
        HuPrcVSleep();
    }
}

static void CharObjLightEnd(u32 frameCount)
{
    s16 frame;
    float scale;
    float scaleY;

    for (frame = 1; frame <= frameCount; frame++) {
        scale = EndScale(LIGHT_SCALE_MAX, (float)frame / (float)frameCount);
        scaleY = EndScale(LIGHT_SCALE_MAX, (float)frame / (float)frameCount);
        Hu3DModelScaleSet(OpeningMdlId[OPENING_MODEL_LIGHT], scale, scaleY, LIGHT_SCALE_Z);
        HuPrcVSleep();
    }
}

static void CharObjStarStart(u32 frameCount)
{
    HuVecF pos;
    s16 i;
    s16 halfFrames;
    s16 endFrame;
    s16 startFrame;
    float scaleX;
    float scaleY;

    Hu3DModelAttrReset(OpeningMdlId[OPENING_MODEL_STAR_CORE], HU3D_ATTR_DISPOFF);
    Hu3DModelAttrReset(OpeningMdlId[OPENING_MODEL_STAR_SIDE_A], HU3D_ATTR_DISPOFF);
    Hu3DModelAttrReset(OpeningMdlId[OPENING_MODEL_STAR_SIDE_B], HU3D_ATTR_DISPOFF);
    Hu3DModelPosGet(OpeningMdlId[OPENING_MODEL_BOOK], &pos);
    pos.x += 30.f;
    pos.y -= 45.0f;
    pos.z += 50.f;
    Hu3DModelPosSetV(OpeningMdlId[OPENING_MODEL_STAR_CORE], &pos);
    Hu3DModelScaleSet(OpeningMdlId[OPENING_MODEL_STAR_CORE], 0.0f, 0.0f, 0.0f);
    pos.x = 0.0f;
    pos.y += 90.f;
    pos.z += 50.f;
    StarPos = pos;
    Hu3DModelScaleSet(OpeningMdlId[OPENING_MODEL_STAR_SIDE_A], 0.0f, 0.0f, 0.0f);
    Hu3DModelScaleSet(OpeningMdlId[OPENING_MODEL_STAR_SIDE_B], 0.0f, 0.0f, 0.0f);

    halfFrames = frameCount >> 1;
    for (i = 1; i <= halfFrames; i++) {
        if (i <= 0.7 * halfFrames) {
            endFrame = 0.7 * halfFrames;
            scaleY = 0.8999999761581421 *
                     (sin(3.141592653589793 * (130.f * ((float)i / (float)endFrame)) / 180.) *
                      (1.0 / sin(2.2689280275926285)));
        }
        if (i > 0.3 * halfFrames) {
            startFrame = (float)i - 0.3f * halfFrames;
            endFrame = 0.7f * halfFrames;
            scaleX = 0.8999999761581421 *
                     (sin(3.141592653589793 * (130.f * ((float)startFrame / (float)endFrame)) / 180.) *
                      (1.0 / sin(2.2689280275926285)));
        }
        Hu3DModelScaleSet(OpeningMdlId[OPENING_MODEL_STAR_CORE], scaleX, scaleY, scaleX);
        HuPrcVSleep();
    }

    HuAudFXPlay(0x4b1);

    for (i = 1; i <= halfFrames; i++) {
        scaleX = 0.5 *
                 (sin(3.141592653589793 * (130.f * ((float)i / (float)halfFrames)) / 180.) *
                  (1.0 / sin(2.2689280275926285)));
        Hu3DModelScaleSet(OpeningMdlId[OPENING_MODEL_STAR_SIDE_A], scaleX, scaleX, 0.5f);
        scaleX = 0.5 *
                 sin(3.141592653589793 * (90.f * ((float)i / (float)halfFrames)) / 180.);
        Hu3DModelScaleSet(OpeningMdlId[OPENING_MODEL_STAR_SIDE_B], scaleX, scaleX, 0.5f);
        HuPrcVSleep();
    }
}

static void CharObjStarEnd(u32 frameCount)
{
    s16 frame;
    s16 halfFrames;
    float scale;
    float scaleY;

    halfFrames = frameCount >> 1;

    for (frame = 1; frame <= halfFrames; frame++) {
        scale = EndScale(SIDE_STAR_SCALE_MAX, (float)frame / (float)halfFrames);
        Hu3DModelScaleSet(OpeningMdlId[OPENING_MODEL_STAR_SIDE_A], scale, scale, SIDE_STAR_SCALE_Z);
        Hu3DModelScaleSet(OpeningMdlId[OPENING_MODEL_STAR_SIDE_B], scale, scale, SIDE_STAR_SCALE_Z);
        HuPrcVSleep();
    }

    for (frame = 1; frame <= halfFrames; frame++) {
        scale = EndScale(LIGHT_SCALE_MAX, (float)frame / (float)halfFrames);
        scaleY = EndScale(LIGHT_SCALE_MAX, (float)frame / (float)halfFrames);
        Hu3DModelScaleSet(OpeningMdlId[OPENING_MODEL_STAR_CORE], scale, scaleY, scale);
        HuPrcVSleep();
    }
}

static void ObjStarExec(void)
{
    HuVecF pos;
    float phaseDeg;

    phaseDeg = 0.0f;
    while (TRUE) {
        pos = StarPos;
        pos.x += 3.0 *
                 sin(3.141592653589793 * (phaseDeg / 2.f) / 180.);
        pos.y += 5.0 *
                 sin(3.141592653589793 * phaseDeg / 180.);

        Hu3DModelPosSetV(OpeningMdlId[OPENING_MODEL_STAR_SIDE_A], &pos);
        Hu3DModelPosSetV(OpeningMdlId[OPENING_MODEL_STAR_SIDE_B], &pos);

        phaseDeg += 1.0f;
        if (phaseDeg > 720.f) {
            phaseDeg -= 720.f;
        }

        HuPrcVSleep();
    }
}

static void ObjStarStart(u32 frameCount)
{
    HuVecF pos;
    s16 i;
    float scale;

    Hu3DModelAttrReset(OpeningMdlId[OPENING_MODEL_STAR_SIDE_A], HU3D_ATTR_DISPOFF);
    Hu3DModelAttrReset(OpeningMdlId[OPENING_MODEL_STAR_SIDE_B], HU3D_ATTR_DISPOFF);
    Hu3DModelPosGet(OpeningMdlId[OPENING_MODEL_BOOK], &pos);
    pos.y = 80.f;
    pos.z += 150.f;
    Hu3DModelPosSetV(OpeningMdlId[OPENING_MODEL_STAR_SIDE_A], &pos);
    Hu3DModelScaleSet(OpeningMdlId[OPENING_MODEL_STAR_SIDE_A], 0.0f, 0.0f, 0.0f);
    Hu3DModelPosSetV(OpeningMdlId[OPENING_MODEL_STAR_SIDE_B], &pos);
    Hu3DModelScaleSet(OpeningMdlId[OPENING_MODEL_STAR_SIDE_B], 0.0f, 0.0f, 0.0f);
    HuAudFXPlay(0x4B2);

    for (i = 1; i <= frameCount; i++) {
        scale = 0.5 *
                (sin(3.141592653589793 * (130.f * ((float)i / (float)frameCount)) / 180.) *
                 (1.0 / sin(2.2689280275926285)));
        Hu3DModelScaleSet(OpeningMdlId[OPENING_MODEL_STAR_SIDE_A], scale, scale, 0.5f);
        scale = 0.5 *
                sin(3.141592653589793 * (90.f * ((float)i / (float)frameCount)) / 180.);
        Hu3DModelScaleSet(OpeningMdlId[OPENING_MODEL_STAR_SIDE_B], scale, scale, 0.5f);
        HuPrcVSleep();
    }
}

static void ObjStarEnd(void)
{
    HuVecF vec;
    HuVecF pos;
    HuVecF target = { 0.0f, -40.0f, 500.0f };
    float rot;
    s16 i;

    for (i = 1; i <= 60; i++) {
        rot = 720.f * ((float)i / 60.f);
        Hu3DModelRotSet(
            OpeningMdlId[OPENING_MODEL_STAR_SIDE_A],
            0.0f,
            0.0f,
            30. * sin(3.141592653589793 * rot / 180.));
        HuPrcVSleep();
    }

    for (i = 1; i <= 30; i++) {
        if (i == 5) {
            HuAudFXPlay(0x4B3);
        }
        rot = 720. *
              (1.0 -
               cos(3.141592653589793 * (90.f * ((float)i / 30.f)) / 180.));
        Hu3DModelRotSet(
            OpeningMdlId[OPENING_MODEL_STAR_SIDE_A],
            0.0f,
            rot,
            0.0f);
        HuPrcVSleep();
    }

    HuAudFXPlay(0x4B1);
    Hu3DModelAttrReset(OpeningMdlId[7], HU3D_ATTR_DISPOFF);
    Hu3DMotionTimeSet(OpeningMdlId[7], 0.0f);
    Hu3DModelPosGet(OpeningMdlId[OPENING_MODEL_STAR_SIDE_A], &pos);
    Hu3DModelPosSetV(OpeningMdlId[7], &pos);
    HuPrcSleep(20);
    Hu3DModelPosGet(OpeningMdlId[OPENING_MODEL_STAR_SIDE_A], &pos);

    for (i = 1; i <= 60; i++) {
        if (i == 5) {
            HuAudFXPlay(0x4B4);
        }
        rot = (float)i / 60.f;
        PSVECSubtract(&target, &pos, &vec);
        PSVECScale(
            &vec,
            &vec,
            1.0 -
                cos(3.141592653589793 * (90.f * rot) / 180.));
        PSVECAdd(&pos, &vec, &vec);
        vec.y -= 100. *
                 sin(3.141592653589793 * (180.f * ((float)i / 50.f)) / 180.);
        StarPos = vec;
        Hu3DModelRotSet(
            OpeningMdlId[OPENING_MODEL_STAR_SIDE_A],
            0.0f,
            0.0f,
            20.f * rot);
        HuPrcVSleep();
    }
}

static void CharWaitEvent(u32 frameCount)
{
    s16 i;
    s16 j;
    s16 timer[CHAR_EVENT_COUNT];
    s16 count[CHAR_EVENT_COUNT];

    for (i = 0; i < CHAR_EVENT_COUNT; i++) {
        timer[i] = (s16)frandmod(CHAR_EVENT_INITIAL_RANDOM_MAX);
        count[i] = 1;
    }

    for (j = 0; j < frameCount - CHAR_EVENT_FRAME_MARGIN; j++) {
        for (i = 0; i < CHAR_EVENT_COUNT; i++) {
            if (timer[i] == 0) {
                SwapAnimFrame((s16)i, (s16)((i * 7) + (count[i] & 1)));
                count[i]++;
                timer[i] = (s16)(frandmod(20) + 30);
            }
            timer[i]--;
        }
        HuPrcVSleep();
    }

    HuPrcSleep(CHAR_EVENT_FINAL_WAIT);
}

static void CharFocusLeft(void)
{
    s16 i;

    for (i = 0; i < CHAR_EVENT_COUNT; i++) {
        SwapAnimFrame((s16)i, (s16)((i * 7) + 5));
    }
    HuPrcSleep(10);
}

static void CharFocusRight(void)
{
    s16 i;

    for (i = 0; i < CHAR_EVENT_COUNT; i++) {
        SwapAnimFrame((s16)i, (s16)((i * 7) + 6));
    }
    HuPrcSleep(10);
}

static void CharFocusRandom(u32 frameCount)
{
    s16 charIndex;
    s16 elapsed;
    s32 focusStarted;
    s32 rearmLimit;
    float remainingFraction;
    s16 countdowns[CHAR_EVENT_COUNT];
    s16 phases[CHAR_EVENT_COUNT];

    focusStarted = 0;

    for (charIndex = 0; charIndex < CHAR_EVENT_COUNT; charIndex++) {
        countdowns[charIndex] = (s16)frandmod(CHAR_EVENT_INITIAL_RANDOM_MAX);
        phases[charIndex] = 1;
    }

    for (elapsed = 0; elapsed < frameCount - CHAR_EVENT_FRAME_MARGIN; elapsed++) {
        remainingFraction = 1.0f - ((float)elapsed / (float)frameCount);

        for (charIndex = 0; charIndex < CHAR_EVENT_COUNT; charIndex++) {
            if (charIndex != CHAR_FOCUS_RANDOM_FOCUS_CHAR || focusStarted == 0) {
                if (countdowns[charIndex] == 0) {
                    if (focusStarted == 0 && charIndex == CHAR_FOCUS_RANDOM_FOCUS_CHAR &&
                        elapsed > CHAR_FOCUS_RANDOM_LATE_THRESHOLD) {
                        SetCharAnimFrame(CHAR_FOCUS_RANDOM_FOCUS_CHAR, CHAR_FOCUS_RANDOM_FOCUS_FRAME);
                        focusStarted = 1;
                        continue;
                    }

                    SwapAnimFrame(
                        (s16)charIndex,
                        (s16)((charIndex * 7) + CHAR_FOCUS_RANDOM_FRAME_BASE +
                              (phases[charIndex] & 1)));
                    phases[charIndex]++;
                    rearmLimit = (s32)(40.f * remainingFraction);
                    countdowns[charIndex] = (s16)(
                        frandmod((s16)rearmLimit) +
                        CHAR_FOCUS_RANDOM_REARM_BASE);
                }

                countdowns[charIndex]--;
            }
        }

        HuPrcVSleep();
    }

    HuPrcSleep(CHAR_EVENT_FINAL_WAIT);
}

static void CharHappyEvent(u32 frameCount)
{
    s16 frame;
    s16 i;
    s16 countdowns[CHAR_EVENT_COUNT];
    s16 phases[CHAR_EVENT_COUNT];

    for (i = 0; i < CHAR_EVENT_COUNT; i++) {
        countdowns[i] = (s16)frandmod(CHAR_EVENT_INITIAL_RANDOM_MAX);
        phases[i] = 1;
    }

    for (frame = 0; frame < frameCount - CHAR_EVENT_FRAME_MARGIN; frame++) {
        for (i = 0; i < CHAR_EVENT_COUNT; i++) {
            if (countdowns[i] == 0) {
                SwapAnimFrame((s16)i, (s16)((i * 7) + 2 + (phases[i] & 1)));
                phases[i]++;
                countdowns[i] = (s16)(frandmod(CHAR_EVENT_INITIAL_RANDOM_MAX) + 20);
            }
            countdowns[i]--;
        }
        HuPrcVSleep();
    }

    HuPrcSleep(CHAR_EVENT_FINAL_WAIT);
}

static void SetStarDist(s16 modelIndex, float distance)
{
    HSF_OBJECT *object;

    object = Hu3DModelObjPtrGet(OpeningMdlId[2], OpeningCharObjNameTbl[modelIndex]);
    object->mesh.base.pos.x = 450. * sin(3.141592653589793 * distance / 180.);
    object->mesh.base.pos.y = 450. * cos(3.141592653589793 * distance / 180.);
}

static void SetCharAnimFrame(s16 animIndex, s16 frameIndex)
{
    Hu3DAnimAnimSet(OpeningCharAnimId[animIndex], OpeningCharAnim[frameIndex]);
}

static void SwapAnimFrame(s16 animIndex, s16 frameIndex)
{
    EventCreate((void (*)(void))SwapAnimFrameEvent, animIndex, frameIndex);
}

static void SwapAnimFrameEvent(s32 animIndex, s32 frameIndex)
{
    volatile s16 scratch;

    scratch = 0;
    HuPrcSleep(5);
    Hu3DAnimAnimSet(OpeningCharAnimId[animIndex], OpeningCharAnim[frameIndex]);
}

static void MessStart(s16 arg0, u32 message, s16 frameCount)
{
    s16 i;

    for (i = 0; i < 4; i++) {
        HuWinDispOff(OpeningWinId[i]);
    }

    HuWinDispOn(OpeningWinId[arg0]);
    if (message != 0) {
        HuWinMesSet(OpeningWinId[arg0], message);
    } else {
        HuWinHomeClear(OpeningWinId[arg0]);
    }

    if (frameCount >= 0 && frameCount != 0) {
        HuPrcSleep(frameCount);
    }
}

static void BookOpen(void)
{
    Mtx matrix;
    HuVecF ofs;
    HuVecF endPos = { 0.0f, 40.0f, -300.0f };
    HuVecF endRot = { -10.0f, 360.0f, 360.0f };
    float rate;
    s16 i;

    Hu3DMotionTimeSet(OpeningMdlId[1], 0.0f);
    Hu3DMotionClusterTimeSet(OpeningMdlId[1], 0, 0.0f);
    Hu3DModelAttrReset(OpeningMdlId[1], 0x40000002);
    Hu3DModelClusterAttrReset(OpeningMdlId[1], 0, 0xC0000002);
    EventCreate(BookOpenEvent, 0, 0);

    for (i = 1; i <= 180; i++) {
        rate = i / 180.f;

        PSVECSubtract(&endPos, &BookDefPos, &ofs);
        PSVECScale(&ofs, &ofs,
                   sin(3.141592653589793 * (90.f * rate) / 180.));
        PSVECAdd(&BookDefPos, &ofs, &BookPos);

        PSVECSubtract(&endRot, &BookDefRot, &ofs);
        PSVECScale(&ofs, &ofs,
                   sin(3.141592653589793 * (90.f * rate) / 180.));
        PSVECAdd(&BookDefRot, &ofs, &BookRot);

        PSMTXTrans(matrix,
                   150. * (1.0 - sin(3.141592653589793 * (90.f * rate) / 180.)),
                   0.0f, 0.0f);
        Hu3DModelMtxSet(OpeningMdlId[1], &matrix);

        BookRadius = 0.05000000074505806 + 0.949999988079071 * (1.0 - sin(3.141592653589793 * (90.f * rate) / 180.));

        HuPrcVSleep();
    }
}

static void BookOpenEvent(void)
{
    HuPrcSleep(120);
    Hu3DModelAttrSet(OpeningMdlId[OPENING_MODEL_BOOK], HU3D_MOTATTR_PAUSE);
    Hu3DModelClusterAttrSet(OpeningMdlId[OPENING_MODEL_BOOK], 0, HU3D_CLUSTER_ATTR_PAUSE);
}

static void BookExec(void)
{
    HU3D_MODEL *model;
    float phaseDeg;

    model = &Hu3DData[OpeningMdlId[OPENING_MODEL_BOOK]];
    phaseDeg = 0.0f;

    do {
        do {
            HuPrcVSleep();
        } while ((model->attr & HU3D_ATTR_DISPOFF) != 0);

        PSVECAdd(&BookPos, &BookOfs, &model->pos);

        BookOfs.x = BookRadius * (BOOK_X_RADIUS_SCALE * sin(M_PI * phaseDeg / 180.0));
        BookOfs.y = BookRadius * (BOOK_Y_RADIUS_SCALE * cos(M_PI * (BOOK_Y_PHASE_SCALE * phaseDeg) / 180.0));
        model->rot = BookRot;

        phaseDeg += 1.0f;
        if (phaseDeg > BOOK_PHASE_WRAP_DEG) {
            phaseDeg -= BOOK_PHASE_WRAP_DEG;
        }
    } while (TRUE);
}

static void EventExec(void)
{
    HUPROCESS *process;
    s32 *work;

    process = HuPrcCurrentGet();
    work = process->property;
    ((void (*)(s32, s32))work[0])(work[1], work[2]);
    HuPrcEnd();
}

static void EventDestroy(void)
{
    HUPROCESS *process;

    process = HuPrcCurrentGet();
    HuMemDirectFree(process->property);
}

static void EventCreate(void (*callback)(void), s32 arg0, s32 arg1)
{
    HUPROCESS *process;
    s32 *work;

    process = HuPrcChildCreate(EventExec, 0x100, 0x3000, 0, HuPrcCurrentGet());
    work = HuMemDirectMalloc(HEAP_HEAP, 0xC);
    process->property = work;
    HuPrcDestructorSet2(process, EventDestroy);
    work[0] = (s32)callback;
    work[1] = arg0;
    work[2] = arg1;
}

static void OpeningCameraSet(HuVecF *rotDeg, HuVecF *target, float distance)
{
    HuVecF pos;
    HuVecF tgt;
    HuVecF up;
    float x;
    float y;
    float z;

    x = rotDeg->x;
    y = rotDeg->y;
    z = rotDeg->z;

    pos.x = target->x + (distance * (HuSin(y) * HuCos(x)));
    pos.y = target->y + (distance * -HuSin(x));
    pos.z = target->z + (distance * (HuCos(y) * HuCos(x)));
    tgt.x = target->x;
    tgt.y = target->y;
    tgt.z = target->z;
    up.x = HuSin(y) * HuSin(x);
    up.y = HuCos(x);
    up.z = HuCos(y) * HuSin(x);

    Hu3DCameraPosSet(
        OPENING_CAMERA_BIT,
        pos.x,
        pos.y,
        pos.z,
        up.x,
        up.y,
        up.z,
        tgt.x,
        tgt.y,
        tgt.z);
}
const GXColor lbl_1_rodata_280 = { 0x9a, 0x70, 0x56, 0xff };
const GXColor lbl_1_rodata_284 = { 0xa6, 0xa6, 0xa6, 0xff };

static void FilterDraw(float alpha)
{
    Mtx44 proj;
    Mtx modelview;
    GXTexObj texObj;
    GXColor color1 = lbl_1_rodata_280;
    GXColor color2 = lbl_1_rodata_284;

    C_MTXOrtho(proj, 0.0f, 480.f, 0.0f, 640.f, 0.0f, 8000.f);
    GXSetProjection(proj, GX_ORTHOGRAPHIC);
    PSMTXIdentity(modelview);
    GXLoadPosMtxImm(modelview, GX_PNMTX0);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XY, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    color1.a = 255.0f * alpha;
    GXSetTevColor(GX_TEVREG0, color1);
    GXSetTevColor(GX_TEVREG1, color2);
    GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_TEXC, GX_CC_C0, GX_CC_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_KONST, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0);
    GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_TEXC, GX_CC_CPREV, GX_CC_C1, GX_CC_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_A0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetNumTexGens(1);
    GXSetNumTevStages(2);
    GXInitTexObj(&texObj, FilterFrameBuf, POSTDRAW_COPY_W, POSTDRAW_COPY_H, GX_TF_I8, GX_CLAMP, GX_CLAMP, GX_FALSE);
    GXInitTexObjLOD(&texObj, GX_NEAR, GX_NEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE, GX_ANISO_1);
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition2f32(0.0f, 80.f);
    GXTexCoord2f32(0.0f, 0.0f);
    GXPosition2f32(640.f, 80.f);
    GXTexCoord2f32(1.0f, 0.0f);
    GXPosition2f32(640.f, 400.f);
    GXTexCoord2f32(1.0f, 1.0f);
    GXPosition2f32(0.0f, 400.f);
    GXTexCoord2f32(0.0f, 1.0f);
    GXEnd();
}


s16 OpeningMdlId[8];
s16 lbl_1_bss_224;
void *BookFrameBuf[2];
void *FilterFrameBuf;
ANIMDATA *BookFrameBufAnim[2];
ANIMDATA *OpeningCharAnim[79];
s16 BookFrameBufAnimId[2];
s16 OpeningCharAnimId[12];
HuVecF BookPos;
HuVecF BookOfs;
HuVecF BookRot;
float BookRadius;
s16 OpeningWinId[4];
s16 OpeningBgState;
HuVec2f GuideDist;
float OpeningBgRotZ;
HuVecF StarPos;
float FilterTPLvl;
s32 CharLightFocusF;
s32 CharFocusCancelF;
s32 OpeningEndF;
s16 FbCopyMode;

s16 WinFrameTypeTbl[4] = {
    0, 4, 3, 5,
};

u32 OpeningCharFileTbl[80] = {
    0x00D5001E, 0x00D50020, 0x00D50022, 0x00D50021,
    0x00D50022, 0x00D50023, 0x00D50024, 0x00D50026,
    0x00D50027, 0x00D50028, 0x00D50029, 0x00D5002A,
    0x00D5002B, 0x00D5002C, 0x00D5002D, 0x00D5002E,
    0x00D5002F, 0x00D50030, 0x00D50031, 0x00D50032,
    0x00D50033, 0x00D50034, 0x00D50035, 0x00D50036,
    0x00D50037, 0x00D50038, 0x00D50039, 0x00D5003A,
    0x00D5003B, 0x00D5003C, 0x00D5003D, 0x00D5003E,
    0x00D5003F, 0x00D50040, 0x00D50041, 0x00D50042,
    0x00D50043, 0x00D50044, 0x00D50045, 0x00D50046,
    0x00D50047, 0x00D50048, 0x00D50049, 0x00D5004A,
    0x00D5004B, 0x00D5004C, 0x00D5004D, 0x00D5004E,
    0x00D5004F, 0x00D50050, 0x00D50051, 0x00D50052,
    0x00D50053, 0x00D50054, 0x00D50055, 0x00D50056,
    0x00D50057, 0x00D50058, 0x00D50059, 0x00D5005A,
    0x00D5005B, 0x00D5005C, 0x00D5005D, 0x00D5005E,
    0x00D5005F, 0x00D50060, 0x00D50061, 0x00D50062,
    0x00D50063, 0x00D50064, 0x00D50069, 0x00D5006A,
    0x00D5006B, 0x00D5006C, 0x00D50065, 0x00D50066,
    0x00D50067, 0x00D50068, 0x00D50025, 0xFFFFFFFF,
};

char *OpeningCharTexNameTbl[12] = {
    "Dummy_mario",
    "Dummy_Luigi",
    "Dummy_peach",
    "Dummy_yoshi",
    "Dummy_wario",
    "Dummy_daisy",
    "Dummy_waluigi",
    "Dummy_kinopio",
    "Dummy_teresa",
    "Dummy_minikoopa",
    "guide001",
    "guide002",
};

char *OpeningCharObjNameTbl[12] = {
    "op_stage011-mario",
    "op_stage011-luigi",
    "op_stage011-peach",
    "op_stage011-yoshi",
    "op_stage011-wario",
    "op_stage011-daisy",
    "op_stage011-waluigi",
    "op_stage011-kinopio",
    "op_stage011-teresa",
    "op_stage011-minikoopa",
    "op_stage011-soruru",
    "op_stage011-luluna",
};

HuVecF BookDefPos = { 0.0f, 0.0f, -2000.0f };
HuVecF BookDefRot = { -20.0f, -30.0f, 30.0f };

char lbl_1_data_8988[0xE] = "op_book_dummy";
char lbl_1_data_8996[0xF] = "op_book_dummy2";
char lbl_1_data_89A5[0x14] = "op_stage004-bg_root";
