#define _MATH_H
#include "dolphin/math.h"

#include "game/charman.h"
#include "game/process.h"
#include "game/object.h"
#include "game/audio.h"
#include "game/sprite.h"

#include "datanum/effect.h"

#define EFFECT_WARN 0
#define EFFECT_DUST 1
#define EFFECT_SMOKE 2
#define EFFECT_STAR 3
#define EFFECT_GLOW 4
#define EFFECT_HIT 5
#define EFFECT_BIRD 6
#define EFFECT_CRY 7
#define EFFECT_LANDDUST 8
#define EFFECT_MAX 9

typedef struct CharWork_s {
    HU3D_MODELID modelId;
    s16 model;
    s16 motNoCurr;
    s16 motNoShiftCurr;
    s16 motNoPrev;
    s16 motNoShiftPrev;
    HU3D_MOTIONID motId[CHAR_MOT_MAX];
    s16 motNoTbl[CHAR_MOT_MAX];
    s16 vol;
    s16 pan;
    u8 voiceFlag[CHAR_MOT_MAX];
    u32 attr;
    s8 stepFx;
    HuVecF pos;
    AMEM_PTR motAMemP;
    HUPROCESS *process;
    s16 timingHookNo;
    s16 motNoPlay[2];
    s16 motPlayTime[2];
} CHARWORK;

typedef struct EffectData_s {
    unsigned int dataNum;
    s16 maxCnt;
    s16 blendMode;
    s32 motCnt;
    unsigned int motDataNum[16];
} EFFECTDATA;

typedef struct EffectParam_s {
    u32 attr;
    GXColor colorBegin;
    GXColor colorEnd;
    HuVecF vel;
    HuVecF velDecay;
    float gravity;
    u32 zero;
    float scaleVel;
    float alphaBase;
    float colorWeight;
} EFFECTPARAM;

static EFFECTDATA effectDataTbl[] = {
    { 0x00210007, 10, HU3D_PARTICLE_BLEND_NORMAL, 2, { 0x00930018, 0x00930019 } },
    { 0x00210002, 150, HU3D_PARTICLE_BLEND_ADDCOL, 0, { 0x00930001, 0x00930002, 0x00930023, 0x00930018, 0x0093001D } },
    { 0x00210003, 70, HU3D_PARTICLE_BLEND_NORMAL, 0, { 0x00930014 } },
    { 0x00210000, 300, HU3D_PARTICLE_BLEND_NORMAL, 0, { 0x0093000A } },
    { 0x00210001, 300, HU3D_PARTICLE_BLEND_ADDCOL, 0, {} },
    { 0x00210005, 200, HU3D_PARTICLE_BLEND_ADDCOL, 0, { 0x00930005 } },
    { 0x00210008, 12, HU3D_PARTICLE_BLEND_NORMAL, 2, { 0x00930015, 0x00930016, 0x00930076 } },
    { 0x00210009, 100, HU3D_PARTICLE_BLEND_ADDCOL, 0, { 0x00930022, 0x00930023 } },
    { 0x00210004, 100, HU3D_PARTICLE_BLEND_NORMAL, 0, { 0x0093000A, 0x00930004 } },
};

char *CharHeadObjNameTbl[15] = {
    "root_head", "root_head", "root_hair1", "ske_head", "eff_head", "hair1", "eff_head",
    "root_head", "root_head", "root_head", "root_head", "root_head", "root_head", "root_head", "root_head",
};

unsigned int CharDataDirTbl[CHARNO_MAX][6] = {
    { 0x008F0000, 0x00900000, 0x00910000, 0x00920000, 0x00930000, 0x008E0000 },
    { 0x00370000, 0x00380000, 0x00390000, 0x003A0000, 0x003B0000, 0x00360000 },
    { 0x00C10000, 0x00C20000, 0x00C30000, 0x00C40000, 0x00C50000, 0x00C00000 },
    { 0x00F40000, 0x00F50000, 0x00F60000, 0x00F70000, 0x00F80000, 0x00F30000 },
    { 0x00ED0000, 0x00EE0000, 0x00EF0000, 0x00F00000, 0x00F10000, 0x00EC0000 },
    { 0x00150000, 0x00160000, 0x00170000, 0x00180000, 0x00190000, 0x00140000 },
    { 0x00E70000, 0x00E80000, 0x00E90000, 0x00EA0000, 0x00EB0000, 0x00E60000 },
    { 0x00310000, 0x00320000, 0x00330000, 0x00340000, 0x00350000, 0x00300000 },
    { 0x00D00000, 0x00D10000, 0x00D20000, 0x00D30000, 0x00D40000, 0x00CF0000 },
    { 0x00B80000, 0x00B90000, 0x00BA0000, 0x00BB0000, 0x00BC0000, 0x00A80000 },
    { 0x002B0000, 0x002C0000, 0x002D0000, 0x002E0000, 0x002F0000, 0x002A0000 },
    { 0x00B40000, 0x00B50000, 0x00B60000, 0x00B70000, 0x00BC0000, 0x00A80000 },
    { 0x00AF0000, 0x00B00000, 0x00B10000, 0x00B20000, 0x00BC0000, 0x00A80000 },
    { 0x00AA0000, 0x00AB0000, 0x00AC0000, 0x00AD0000, 0x00BC0000, 0x00A80000 },
};


static u8 lbl_8026F7B0[0xE8];
static u16 dustFlags[CHAR_NPC_MAX];
static HUPROCESS *hookDustProc[CHAR_MOT_MAX];
static EFFECTPARAM *effParamAll[EFFECT_MAX];
static HU3D_MODELID effModelId[EFFECT_MAX][HU3D_CAM_MAX];
static CHARWORK CharWork[CHARNO_MAX];

static s16 effectLayer;
static BOOL motShiftF;
static AMEM_PTR effectAMemP;



void CharInit(void)
{
    s16 i;
    s16 j;
    for(i=0; i<CHARNO_MAX; i++) {
        CHARWORK *workP = &CharWork[i];
        workP->motAMemP = 0;
        for(j=0; j<CHAR_MOT_MAX; j++) {
            workP->motId[j] = HU3D_MOTIONID_NONE;
        }
        workP->modelId = HU3D_MODELID_NONE;
        workP->process = NULL;
        workP->vol = MSM_VOL_MAX;
        workP->pan = MSM_PAN_CENTER;
    }
    if(!effectAMemP) {
        effectAMemP = HuAR_DVDtoARAM(DATA_effect);
    }
    for(i=0; i<EFFECT_MAX; i++) {
        for(j=0; j<HU3D_CAM_MAX; j++) {
            effModelId[i][j] = HU3D_MODELID_NONE;
        }
        effParamAll[i] = NULL;
    }
    for(i=0; i<CHAR_MOT_MAX; i++) {
        hookDustProc[i] = NULL;
    }
    effectLayer = 0;
}

AMEM_PTR CharMotionAMemPGet(s16 charNo)
{
    return CharWork[charNo].motAMemP;
}

void CharMotionInit(s16 charNo)
{
    if(charNo >= CHARNO_MAX || charNo < 0 || charNo == (u8)CHARNO_NONE) {
        return;
    } else {
        CHARWORK *workP = &CharWork[charNo];
        if(!workP->motAMemP) {
            workP->motAMemP = HuAR_DVDtoARAM(CharDataDirTbl[charNo][4]);
        }
    }
}

void CharMotionInitAsync(s16 charNo)
{
    if(charNo >= CHARNO_MAX || charNo < 0 || charNo == (u8)CHARNO_NONE) {
        return;
    } else {
        CHARWORK *workP = &CharWork[charNo];
        if(!workP->motAMemP) {
            s16 statId = HuDataDirReadAsync(CharDataDirTbl[charNo][4]);
            while(!HuDataGetAsyncStat(statId)) {
                HuPrcVSleep();
            }
            workP->motAMemP = HuAR_MRAMtoARAM(CharDataDirTbl[charNo][4]);
            while(HuARDMACheck()) {
                HuPrcVSleep();
            }
            HuDataDirClose(CharDataDirTbl[charNo][4]);
        }
    }
}

void CharMotionClose(s16 charNo)
{
    CHARWORK *workP = &CharWork[charNo];
    if(workP->motAMemP) {
        if(HuARDirCheck(CharDataDirTbl[charNo][4])) {
            HuARFree(workP->motAMemP);
        }
        workP->motAMemP = 0;
    }
}

void CharDataClose(s16 charNo)
{
    s16 i;
    if(charNo == CHARNO_NONE) {
        for(i=0; i<CHARNO_MAX; i++) {
            CharDataClose(i);
        }
    } else {
        for(i=0; i<5; i++) {
            HuDataDirClose(CharDataDirTbl[charNo][i]);
        }
        CharMotionClose(charNo);
    }
}

void CharMotionLoad(s16 *statList)
{
    CHARWORK *workP;
    CHARWORK *workList[4];
    s16 noList[4];
    s16 sortList[4];
    s16 i;
    s16 j;
    s16 cnt;
    s16 tmp;
    void *data1;
    void *data2;
    HUDATASTAT *statusP;
    OSTime start = OSGetTick();
    for(workP=CharWork, cnt=0, i=cnt; i<CHARNO_MAX; i++, workP++) {
        if(workP->motAMemP) {
            noList[cnt] = i;
            workList[cnt++] = workP;
        }
    }
    for(i=cnt-1; i>=0; i--) {
        for(j=0; j<i; j++) {
            if(workList[j]->motAMemP > workList[j+1]->motAMemP) {
                tmp = noList[j];
                noList[j] = noList[j+1];
                noList[j+1] = tmp;
                workP = workList[j];
                workList[j] = workList[j+1];
                workList[j+1] = workP;
            }
        }
    }
    for(tmp=0, i=tmp; i<cnt; i++) {
        for(j=0; j<cnt; j++) {
            if(statList[j] == noList[i]) {
                tmp++;
                sortList[i] = j;
            }
        }
    }
    if(tmp != cnt) {
        return;
    }
    for(i=cnt-1; i>=0; i--) {
        for(j=0; j<i; j++) {
            if(sortList[j] > sortList[j+1]) {
                data1 = HuAR_ARAMtoMRAM(workList[j]->motAMemP);
                while(HuARDMACheck()) {}
                data2 = HuAR_ARAMtoMRAM(workList[j+1]->motAMemP);
                while(HuARDMACheck()) {}
                HuARFree(workList[j]->motAMemP);
                HuARFree(workList[j+1]->motAMemP);
                workList[j+1]->motAMemP = HuAR_MRAMtoARAM2(data2);
                while(HuARDMACheck()) {}
                workList[j]->motAMemP = HuAR_MRAMtoARAM2(data1);
                while(HuARDMACheck()) {}
                statusP = HuDataGetStatus(data1);
                HuDataDirClose(statusP->dirId << 16);
                statusP = HuDataGetStatus(data2);
                HuDataDirClose(statusP->dirId << 16);
                tmp = sortList[j]; sortList[j] = sortList[j+1]; sortList[j+1] = tmp;
                tmp = noList[j]; noList[j] = noList[j+1]; noList[j+1] = tmp;
                workP = workList[j]; workList[j] = workList[j+1]; workList[j+1] = workP;
            }
        }
    }
    printf("Time %dmsec\n", OSTicksToMilliseconds(OSGetTick()-start));
}

static EFFECTPARAM dustEffParam = { 0, { 128, 128, 128, 255 }, { 64, 32, 0, 255 }, { 0, 2, 1 }, { 0.95f, 0.95f, 0.95f }, 0, 0, 1, -5, 0.02f };
static EFFECTPARAM hitEffParam = { 0, { 255, 64, 64, 128 }, { 255, 64, 64, 128 }, { 0, 0, 0 }, { 0, 0, 0 }, 0, 0, -5, 0, 0 };
static EFFECTPARAM npcHitEffParam = { 0, { 255, 255, 0, 255 }, { 255, 255, 0, 255 }, { 0, 0, 0 }, { 0.95f, 0.95f, 0.95f }, 0, 0, -0.5, -10, 0 };
static EFFECTPARAM warnEffParam = { 0, { 255, 255, 255, 255 }, { 255, 255, 255, 255 }, { 0, 20, 0 }, { 0.95f, 0.85f, 0.95f }, 0, 0, 1, -5, 0 };
static EFFECTPARAM smokeEffParam = { 0, { 128, 32, 32, 255 }, { 0, 0, 0, 255 }, { 0, 10, 0 }, { 1.0f, 0.95f, 1.0f }, 0, 0, 5, -13, 0.1f };

static s8 walkVoiceTimeTbl[56] = {
    10, 32, -1, -1,
    20, 40, -1, -1,
    12, 37, -1, -1,
    4, 25, -1, -1,
    5, 30, -1, -1,
    23, 50, -1, -1,
    19, 39, -1, -1,
    17, 38, -1, -1,
    10, 32, -1, -1,
    21, 44, -1, -1,
    17, 38, -1, -1,
    21, 44, -1, -1,
    21, 44, -1, -1,
    21, 44, -1, -1,
};

static s8 runVoiceTimeTbl[56] = {
    15, 29, -1, -1,
    5, 19, -1, -1,
    5, 22, -1, -1,
    12, 28, -1, -1,
    18, 35, -1, -1,
    0, 16, -1, -1,
    12, 28, -1, -1,
    14, 30, -1, -1,
    10, 32, -1, -1,
    16, 32, -1, -1,
    14, 30, -1, -1,
    16, 32, -1, -1,
    16, 32, -1, -1,
    16, 32, -1, -1,
};

static s8 jumpVoiceTimeTbl[28] = {
    2, 4,
    3, 5,
    2, 3,
    3, -1,
    2, 3,
    4, 5,
    2, 4,
    1, 2,
    4, -1,
    1, 3,
    2, 4,
    1, 3,
    1, 3,
    1, 3,
};

static u8 boardWinAnimLen[CHARNO_MAX] = { 14, 0, 46, 72, 20, 34, 41, 0, 0, 17, 0, 17, 17, 17 };
static u8 winAnimLen[CHARNO_MAX] = { 18, 0, 30, 106, 0, 0, 24, 24, 90, 60, 60, 60, 60, 60 };
static u8 handUpAnimLen[CHARNO_MAX] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
static u8 okAnimLen[CHARNO_MAX] = { 0, 0, 24, 90, 30, 0, 18, 42, 96, 30, 42, 30, 30, 30 };
static u8 winAnimLen2[CHARNO_MAX] = { 18, 0, 30, 106, 0, 0, 24, 24, 90, 60, 24, 60, 60, 60 };
static u8 holdUpAnimLen[CHARNO_MAX] = { 0, 6, 0, 78, 0, 16, 50, 0, 54, 38, 0, 38, 38, 38 };
static u8 winJumpUpAnimLen[CHARNO_MAX] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
static u8 loseAnimLen[CHARNO_MAX] = { 18, 0, 36, 106, 0, 0, 24, 24, 90, 60, 24, 60, 60, 60 };

static s8 jumpAnimLen[CHARNO_MAX] = { 42, 42, 43, 52, 42, 42, -1, 42, 42, 42, 42, 42, 42, 42 };

void CharClose(s16 charNo)
{
    CharModelKill(CHARNO_NONE);
    CharDataClose(CHARNO_NONE);
    HuARFree(effectAMemP);
    effectAMemP = 0;
}

static void CharTimingHook(HU3D_MODELID modelId, HU3D_MOTIONID motId, BOOL lagF);
static void UpdateChar(void);
static s16 GetStepType(void);

HU3D_MODELID CharModelCreate(s16 charNo, s16 model)
{
    s16 sp8 = 0;
    CHARWORK *workP = &CharWork[charNo];
    unsigned int dataNum;
    HU3D_MODELID modelId;
    s16 *property;
    void *dataP;

    if(workP->modelId != HU3D_MODELID_NONE) {
        Hu3DModelKill(workP->modelId);
    }
    if(model & CHAR_MODEL0) {
        dataNum = CharDataDirTbl[charNo][0];
    } else if(model & CHAR_MODEL1) {
        dataNum = CharDataDirTbl[charNo][1];
    } else if(model & CHAR_MODEL2) {
        dataNum = CharDataDirTbl[charNo][2];
    } else {
        dataNum = CharDataDirTbl[charNo][3];
    }
    dataP = HuDataSelHeapReadNum(dataNum, HU_MEMNUM_OVL, HEAP_MODEL);
    workP->modelId = modelId = Hu3DModelCreate(dataP);
    workP->process = HuPrcCreate(UpdateChar, 99, 16384, 0);
    workP->process->property = property = HuMemDirectMalloc(HEAP_HEAP, sizeof(s16));
    workP->model = model;
    workP->attr = 0;
    *property = charNo;
    workP->stepFx = GetStepType();
    Hu3DMotionTimingHookSet(modelId, CharTimingHook);
    workP->timingHookNo = 0;
    workP->motNoPlay[0] = workP->motNoPlay[1] = 32767;
    return modelId;
}

static void CharTimingHook(HU3D_MODELID modelId, HU3D_MOTIONID motId, BOOL lagF)
{
    CHARWORK *workP = &CharWork[0];
    s16 i;
    for(i=0; i<CHARNO_MAX; i++, workP++) {
        if(workP->modelId == modelId) {
            workP->timingHookNo++;
            break;
        }
    }
}

static void UpdateMotPlay(CHARWORK *workP, s16 motNo, s16 motNoShift);
static void UpdateCharAnim(s16 charNo, HU3D_MODELID modelId, s16 motNo, u8 voiceFlag, s16 frameNo, HuVecF *ofs);
static void EyeBmpUpdate(s16 charNo);

static void UpdateChar(void)
{
    s16 *property = HuPrcCurrentGet()->property;
    CHARWORK *workP = &CharWork[*property];
    HU3D_MODEL *modelP = &Hu3DData[workP->modelId];
    s16 updateBmpF = FALSE;
    HuVecF pos;
    s16 timingHookNo;
    pos.y = 100000.0f;
    workP->timingHookNo = timingHookNo = 0;
    
    while(1) {
        s16 motNo;
        s16 motNoShift;
        s16 motId;
        s16 motIdShift;
        s16 i;
        motNo = motNoShift = -1;
        if(modelP->motId != HU3D_MOTIONID_NONE) {
            for(i=0; i<CHAR_MOT_MAX; i++) {
                if(workP->motId[i] == modelP->motId) {
                    motNo = workP->motNoTbl[i];
                    motId = i;
                    break;
                }
            }
        }
        if(modelP->motIdShift != HU3D_MOTIONID_NONE) {
            for(i=0; i<CHAR_MOT_MAX; i++) {
                if(workP->motId[i] == modelP->motIdShift) {
                    motNoShift = workP->motNoTbl[i];
                    motIdShift = i;
                    break;
                }
                
            }
        }
        UpdateMotPlay(workP, motNo, motNoShift);
        motShiftF = FALSE;
        if(motNo != -1) {
            workP->motNoCurr = motNo;
            UpdateCharAnim(*property, workP->modelId, motNo, workP->voiceFlag[motId], modelP->motWork.time, &pos);
            workP->motNoPrev = motNo;
        } else {
            workP->motNoCurr = -1;
        }
        if(modelP->motIdShift != HU3D_MOTIONID_NONE) {
            motShiftF = TRUE;
            updateBmpF = TRUE;
            if(motNoShift != -1) {
                workP->motNoShiftCurr = motNoShift;
                UpdateCharAnim(*property, workP->modelId, motNoShift, workP->voiceFlag[motIdShift], modelP->motShiftWork.time, &pos);
                workP->motNoShiftPrev = motNoShift;
            } else {
                workP->motNoShiftCurr = -1;
            }
        } else {
            if(updateBmpF) {
                EyeBmpUpdate(*property);
                updateBmpF = FALSE;
            }
        }
        workP->pos = modelP->pos;
        if(timingHookNo != workP->timingHookNo) {
            timingHookNo = workP->timingHookNo;
        } else {
            workP->timingHookNo = 0;
        }
        HuPrcVSleep();
    }
}

static void UpdateMotPlay(CHARWORK *workP, s16 motNo, s16 motNoShift)
{
    s16 i;
    if(motNo != -1) {
        if(workP->motNoPlay[0] != motNo && workP->motNoPlay[1] != motNo) {
            for(i=0; i<2; i++) {
                if(workP->motNoPlay[i] != motNoShift) {
                    workP->motNoPlay[i] = motNo;
                    workP->motPlayTime[i] = 0;
                    break;
                }
            }
        } else {
            for(i=0; i<2; i++) {
                if(workP->motNoPlay[i] == motNo) {
                    workP->motPlayTime[i]++;
                    break;
                }
            }
        }
    }
    if(motNoShift != -1) {
        if(workP->motNoPlay[0] != motNoShift && workP->motNoPlay[1] != motNoShift) {
            for(i=0; i<2; i++) {
                if(workP->motNoPlay[i] != motNo) {
                    workP->motNoPlay[i] = motNoShift;
                    workP->motPlayTime[i] = 0;
                    break;
                }
            }
        } else {
            for(i=0; i<2; i++) {
                if(workP->motNoPlay[i] == motNoShift) {
                    workP->motPlayTime[i]++;
                    break;
                }
            }
        }
    }
    if(workP->motNoPlay[0] != motNo && workP->motNoPlay[0] != motNoShift) {
        workP->motNoPlay[0] = 32767;
    }
    if(workP->motNoPlay[1] != motNo && workP->motNoPlay[1] != motNoShift) {
        workP->motNoPlay[1] = 32767;
    }
}

static inline s32 GetMotNoPlayTime(CHARWORK *workP, s16 motNo)
{
    s16 i;
    for(i=0; i<2; i++) {
        if(workP->motNoPlay[i] == motNo) {
            return workP->motPlayTime[i];
        }
    }
    return -1;
}

s16 _CharFXPlay(s16 charNo, s16 seNo, u8 voiceFlag);
static s16 CharWinVoicePlay(s16 charNo, u8 voiceFlag);
static s16 EffectDustCreate(HU3D_MODELID modelId, float posX, float posY, float posZ, float scale, EFFECTPARAM *param);
static s16 EffectHitCreate(HU3D_MODELID modelId, float posX, float posY, float posZ, float scale, EFFECTPARAM *param);
static s16 EffectSmokeCreate(HU3D_MODELID modelId, float posX, float posY, float posZ, float scale, EFFECTPARAM *param);

static inline s16 EffectWarnCreate(HU3D_MODELID modelId, float posX, float posY, float posZ, float scale, float ofsY, EFFECTPARAM *param);
static s16 EffectBirdCreate(HU3D_MODELID modelId, float posX, float posY, float posZ, float scale, s16 charNo, float ofsY, EFFECTPARAM *param);


static s16 PlayStepVoice(s16 charNo, s16 seId, u8 voiceFlag);

static void UpdateCharAnim(s16 charNo, HU3D_MODELID modelId, s16 motNo, u8 voiceFlag, s16 frameNo, HuVecF *ofs)
{
    HU3D_MODEL *modelP = &Hu3DData[modelId];
    CHARWORK *workP = &CharWork[charNo];
    u32 attrOld = 0;
    s16 i;
    s16 dotMax;
    Mtx hitMtx;
    HuVecF pos;
    HuVecF hitPos;
    float warnSize;
    if(!motShiftF && (modelP->motAttr & HU3D_MOTATTR_PAUSE)) {
        return;
    }
    if(voiceFlag & 0x2) {
        return;
    }
    switch(motNo) {
        case CHAR_MOTNO(CHARMOT_HSF_c000m1_315):
        case CHAR_MOTNO(CHARMOT_HSF_c000m1_317):
            if(GetMotNoPlayTime(workP, motNo) == 0 && !(workP->attr & 0x10)) {
                _CharFXPlay(charNo, CHARVOICEID(9), voiceFlag);
            }
            break;

        case CHAR_MOTNO(CHARMOT_HSF_c000m1_301):
        case CHAR_MOTNO(CHARMOT_HSF_c000m1_332):
        case CHAR_MOTNO(CHARMOT_HSF_c000m1_433):
        case CHAR_MOTNO(CHARMOT_HSF_c000m1_442):
        case CHAR_MOTNO(CHARMOT_HSF_c000m1_448):
            if(!motShiftF && modelP->motWork.speed <= 0.5) {
                break;
            }
            if(motShiftF && modelP->motShiftWork.speed <= 0.5) {
                break;
            }
            if((frameNo & 0xF) == 0 && !(workP->attr & 0x10)) {
                dustEffParam.vel.x = -HuSin(modelP->rot.y)*2.0;
                dustEffParam.vel.y = 1+(0.1*frandmod(10));
                dustEffParam.vel.z = -HuCos(modelP->rot.y)*2.0;
                pos.x = modelP->pos.x+(modelP->scale.x*(frandmod(50)-25));
                pos.y = modelP->pos.y;
                pos.z = modelP->pos.z+(modelP->scale.x*(frandmod(50)-25));
                if(charNo == CHARNO_MINIKOOPA) {
                    pos.x += -HuSin(modelP->rot.y)*40.0;
                    pos.z += -HuCos(modelP->rot.y)*40.0;
                }
                EffectDustCreate(modelId, pos.x, pos.y, pos.z, frandmod(10)+30, &dustEffParam);
            }
            for(i=0; i<4; i++) {
                if(frameNo == walkVoiceTimeTbl[(charNo*4)+i]) {
                    PlayStepVoice(charNo, CHARSEID(0), voiceFlag);
                    break;
                }
            }
            break;

        case CHAR_MOTNO(CHARMOT_HSF_c000m1_302):
        case CHAR_MOTNO(CHARMOT_HSF_c000m1_345):
        case CHAR_MOTNO(CHARMOT_HSF_c000m1_434):
        case CHAR_MOTNO(CHARMOT_HSF_c000m1_443):
        case CHAR_MOTNO(CHARMOT_HSF_c000m1_458):
            if(!motShiftF && modelP->motWork.speed <= 0.5) {
                break;
            }
            if(motShiftF && modelP->motShiftWork.speed <= 0.5) {
                break;
            }
            if((frameNo & 0x7) == 0 && !(workP->attr & 0x10)) {
                dustEffParam.vel.x = -HuSin(modelP->rot.y)*4.0;
                dustEffParam.vel.y = 2+(0.1*frandmod(10));
                dustEffParam.vel.z = -HuCos(modelP->rot.y)*4.0;
                pos.x = modelP->pos.x+(modelP->scale.x*(frandmod(50)-25));
                pos.y = modelP->pos.y;
                pos.z = modelP->pos.z+(modelP->scale.x*(frandmod(50)-25));
                if(charNo == CHARNO_MINIKOOPA) {
                    pos.x += -HuSin(modelP->rot.y)*40.0;
                    pos.z += -HuCos(modelP->rot.y)*40.0;
                }
                EffectDustCreate(modelId, pos.x, pos.y, pos.z, frandmod(10)+30, &dustEffParam);
            }
            if(motNo == CHAR_MOTNO(CHARMOT_HSF_c000m1_345) && (frameNo & 0x1)) {
                CharModelCryCreate(charNo, 30, 0.8f);
            }
            for(i=0; i<4; i++) {
                if(frameNo == runVoiceTimeTbl[(charNo*4)+i]) {
                    PlayStepVoice(charNo, CHARSEID(4), voiceFlag);
                    break;
                }
            }
            break;

        case CHAR_MOTNO(CHARMOT_HSF_c000m1_304):
            if(frameNo == 5 && !(workP->attr & 0x10)) {
                CharModelLandDustCreate(charNo, &modelP->pos);
            }
            for(i=0; i<2; i++) {
                if(frameNo == jumpVoiceTimeTbl[(charNo*2)+i]) {
                    PlayStepVoice(charNo, CHARSEID(8), voiceFlag);
                    break;
                }
            }
            break;

        case CHAR_MOTNO(CHARMOT_HSF_c000m1_464):
            if(frameNo == 1 && !(workP->attr & 0x10)) {
                CharModelLandDustCreate(charNo, &modelP->pos);
                PlayStepVoice(charNo, CHARSEID(8), voiceFlag);
            }
            break;

        case CHAR_MOTNO(CHARMOT_HSF_c000m1_305):
            Hu3DModelObjMtxGet(modelId, CharModelItemHookGet(charNo, workP->model, 0), hitMtx);
            if(frameNo == 0) {
                _CharFXPlay(charNo, CHARSEID(22), voiceFlag);
            }
            dotMax = 10;
            if(frameNo <= dotMax && !(workP->attr & 0x10)) {
                hitEffParam.vel.x = 0.0f;
                hitEffParam.vel.y = 0.0f;
                hitEffParam.vel.z = 0.0f;
                hitEffParam.scaleVel = -5.0f;
                pos.x = hitMtx[0][3];
                pos.y = hitMtx[1][3];
                pos.z = hitMtx[2][3];
                if(frameNo != 0 && ofs->y != 100000.0f) {
                    VECSubtract(&pos, ofs, &hitPos);
                    dotMax = 0.2*sqrtf(hitPos.z*hitPos.z+(hitPos.x*hitPos.x+hitPos.y*hitPos.y));
                    if(dotMax > 5) {
                        dotMax = 5;
                    }
                    if(dotMax < 1) {
                        dotMax = 1;
                    }
                    for(i=1; i<=dotMax; i++) {
                        hitPos.x = ofs->x+(pos.x-ofs->x)*((float)i/dotMax);
                        hitPos.y = ofs->y+(pos.y-ofs->y)*((float)i/dotMax);
                        hitPos.z = ofs->z+(pos.z-ofs->z)*((float)i/dotMax);
                        EffectHitCreate(modelId, hitPos.x, hitPos.y, hitPos.z, 50, &hitEffParam);
                    }
                } else {
                    EffectHitCreate(modelId, pos.x, pos.y, pos.z, 40, &hitEffParam);
                }
                *ofs = pos;
            } else {
                ofs->y = 100000.0f;
            }
            break;

        case CHAR_MOTNO(CHARMOT_HSF_c000m1_367):
            if(frameNo == 0) {
                _CharFXPlay(charNo, CHARSEID(23), voiceFlag);
            }
            if(frameNo < 10) {
                Hu3DModelObjMtxGet(modelId, CharModelItemHookGet(charNo, workP->model, 2), hitMtx);
                hitEffParam.vel.x = 0.0f;
                hitEffParam.vel.y = 0.0f;
                hitEffParam.vel.z = 0.0f;
                hitEffParam.scaleVel = -8.0f;

                pos.x = hitMtx[0][3];
                pos.y = hitMtx[1][3];
                pos.z = hitMtx[2][3];
                if(frameNo != 0) {
                    VECSubtract(&pos, ofs, &hitPos);
                    dotMax = 0.2*sqrtf(hitPos.z*hitPos.z+(hitPos.x*hitPos.x+hitPos.y*hitPos.y));
                    if(dotMax > 5) {
                        dotMax = 5;
                    }
                    if(dotMax < 1) {
                        dotMax = 1;
                    }
                    for(i=1; i<=dotMax; i++) {
                        hitPos.x = ofs->x+(pos.x-ofs->x)*((float)i/dotMax);
                        hitPos.y = ofs->y+(pos.y-ofs->y)*((float)i/dotMax);
                        hitPos.z = ofs->z+(pos.z-ofs->z)*((float)i/dotMax);
                        EffectHitCreate(modelId, hitPos.x, hitPos.y, hitPos.z, 80, &hitEffParam);
                    }
                } else {
                    EffectHitCreate(modelId, pos.x, pos.y, pos.z, 80, &hitEffParam);
                }
                *ofs = pos;
            }
            break;

        case CHAR_MOTNO(CHARMOT_HSF_c000m1_310):
            if(frameNo == 0) {
                if(!(workP->attr & 0x10)) {
                    CharEffectHipDropCreate(charNo, &modelP->pos);
                }
                _CharFXPlay(charNo, CHARSEID(18), voiceFlag);
            }
            break;

        case CHAR_MOTNO(CHARMOT_HSF_c000m1_324):
            if(GetMotNoPlayTime(workP, motNo) == 0 && !(workP->attr & 0x10)) {
                _CharFXPlay(charNo, CHARVOICEID(8), voiceFlag);
            }
            if(modelP->motIdShift == HU3D_MOTIONID_NONE || motShiftF) {
                if(!(workP->attr & 0x10)) {
                    if(frameNo == 10) {
                        if(charNo == CHARNO_WALUIGI) {
                            warnSize = 190;
                        } else if(charNo == CHARNO_PEACH || charNo == CHARNO_DAISY) {
                            warnSize = 140;
                        } else {
                            warnSize = 120;
                        }
                        EffectWarnCreate(modelId, modelP->pos.x, 100.0f+modelP->pos.y, modelP->pos.z, 20, warnSize, &warnEffParam);
                    }
                    if(frameNo == 30) {
                        CharModelLandDustCreate(charNo, &modelP->pos);
                        PlayStepVoice(charNo, CHARSEID(8), voiceFlag);
                    }
                }
            }
            break;

        case CHAR_MOTNO(CHARMOT_HSF_c000m1_325):
            if(modelP->motIdShift == HU3D_MOTIONID_NONE || motShiftF) {
                if(!(workP->attr & 0x10)) {
                    if(frameNo == 0) {
                        if(charNo == CHARNO_WALUIGI) {
                            warnSize = 150;
                        } else if(charNo == CHARNO_PEACH || charNo == CHARNO_DAISY) {
                            warnSize = 120;
                        } else {
                            warnSize = 100;
                        }
                        EffectWarnCreate(modelId, modelP->pos.x, 100.0f+modelP->pos.y, modelP->pos.z, 20, warnSize, &warnEffParam);
                    }
                    if(GetMotNoPlayTime(workP, motNo) == 0 && !(workP->attr & 0x10)) {
                        _CharFXPlay(charNo, CHARVOICEID(9), voiceFlag);
                    }
                }
            }
            break;

        case CHAR_MOTNO(CHARMOT_HSF_c000m1_321):
        case CHAR_MOTNO(CHARMOT_HSF_c000m1_322):
        case CHAR_MOTNO(CHARMOT_HSF_c000m1_465):
            if(!(workP->attr & 0x1) && !(workP->attr & 0x10)) {
                _CharFXPlay(charNo, CHARSEID(19), voiceFlag);
                for(i=0; i<3; i++) {
                    EffectBirdCreate(modelId, modelP->pos.x, modelP->pos.y+(100.0f*modelP->scale.x), modelP->pos.z, 1.0f, charNo, i*120, &warnEffParam);
                }
                workP->attr |= 0x1;
            }
            break;

        case CHAR_MOTNO(CHARMOT_HSF_c000m1_320):
            if(!(workP->attr & 0x10)) {
                if(frameNo & 0x1) {
                    Hu3DModelObjMtxGet(modelId, CharModelItemHookGet(charNo, workP->model, 4), hitMtx);
                    pos.x = hitMtx[0][3];
                    pos.y = hitMtx[1][3];
                    pos.z = hitMtx[2][3];
                    EffectSmokeCreate(modelId, pos.x, pos.y, pos.z, 20, &smokeEffParam);
                }
                if(GetMotNoPlayTime(workP, motNo) == 0) {
                    _CharFXPlay(charNo, CHARVOICEID(3), voiceFlag);
                }
            }
            break;

        case CHAR_MOTNO(CHARMOT_HSF_c000m1_303):
            if(frameNo == 0) {
                _CharFXPlay(charNo, CHARSEID(16), voiceFlag);
            }
            break;

        case CHAR_MOTNO(CHARMOT_HSF_c000m1_308):
            if(frameNo == 0) {
                _CharFXPlay(charNo, CHARSEID(17), voiceFlag);
            }
            break;

        case CHAR_MOTNO(CHARMOT_HSF_c000m1_306):
            if(!(workP->attr & 0x12)) {
                if((omcurovl < 0x7B || omcurovl > 0x82) && (omcurovl < 0x72 || omcurovl > 0x74)) {
                    if(frameNo == winAnimLen[charNo]) {
                        CharWinVoicePlay(charNo, voiceFlag);
                        workP->attr |= 0x2;
                        attrOld |= 0x2;
                    }
                }
            }
            break;

        case CHAR_MOTNO(CHARMOT_HSF_c000m1_346):
            if(!(workP->attr & 0x12)) {
                if(frameNo == handUpAnimLen[charNo]) {
                    _CharFXPlay(charNo, CHARVOICEID(6), voiceFlag);
                    workP->attr |= 0x2;
                    attrOld |= 0x2;
                }
            }
            break;

        case CHAR_MOTNO(CHARMOT_HSF_c000m1_349):
            if(!(workP->attr & 0x12)) {
                if(frameNo == okAnimLen[charNo]) {
                    CharWinVoicePlay(charNo, voiceFlag);
                    workP->attr |= 0x2;
                    attrOld |= 0x2;
                }
            }
            break;

        case CHAR_MOTNO(CHARMOT_HSF_c000m1_350):
            if(!(workP->attr & 0x12)) {
                if(frameNo == winAnimLen2[charNo]) {
                    CharWinVoicePlay(charNo, voiceFlag);
                    workP->attr |= 0x2;
                    attrOld |= 0x2;
                }
            }
            break;

        case CHAR_MOTNO(CHARMOT_HSF_c000m1_363):
            if(!(workP->attr & 0x12)) {
                if(frameNo == holdUpAnimLen[charNo]) {
                    CharWinVoicePlay(charNo, voiceFlag);
                    workP->attr |= 0x2;
                    attrOld |= 0x2;
                }
            }
            break;

        case CHAR_MOTNO(CHARMOT_HSF_c000m1_364):
            if(!(workP->attr & 0x12)) {
                if(frameNo == winJumpUpAnimLen[charNo]) {
                    _CharFXPlay(charNo, CHARVOICEID(2), voiceFlag);
                    workP->attr |= 0x2;
                    attrOld |= 0x2;
                }
            }
            break;

        case CHAR_MOTNO(CHARMOT_HSF_c000m1_352):
            if(!(workP->attr & 0x12)) {
                if(frameNo == loseAnimLen[charNo]) {
                    CharWinVoicePlay(charNo, voiceFlag);
                    workP->attr |= 0x2;
                    attrOld |= 0x2;
                }
            }
            break;

        case CHAR_MOTNO(CHARMOT_HSF_c000m1_327):
            if(!(workP->attr & 0x12)) {
                if(GetMotNoPlayTime(workP, motNo) == 0) {
                    CharWinVoicePlay(charNo, voiceFlag);
                }
                workP->attr |= 0x2;
                attrOld |= 0x2;
            }
            break;

        case CHAR_MOTNO(CHARMOT_HSF_c000m1_307):
        case CHAR_MOTNO(CHARMOT_HSF_c000m1_348):
        case CHAR_MOTNO(CHARMOT_HSF_c000m1_351):
        case CHAR_MOTNO(CHARMOT_HSF_c000m1_353):
        case CHAR_MOTNO(CHARMOT_HSF_c000m1_354):
            if(frameNo == 0 && !(workP->attr & 0x14)) {
                workP->attr |= 0x4;
                attrOld |= 0x4;
            }
            break;

        case CHAR_MOTNO(CHARMOT_HSF_c000m1_344):
            if(!(workP->attr & 0x10)) {
                if(frameNo & 0x1) {
                    CharModelCryCreate(charNo, 0, 0);
                }
                if(GetMotNoPlayTime(workP, motNo) == 0) {
                    _CharFXPlay(charNo, CHARVOICEID(3), voiceFlag);
                }
            }
            break;

        case CHAR_MOTNO(CHARMOT_HSF_c000m1_370):
            if(GetMotNoPlayTime(workP, motNo) == 0 && !(workP->attr & 0x10)) {
                _CharFXPlay(charNo, CHARVOICEID(3), voiceFlag);
            }
            if(workP->timingHookNo != 0) {
                CharModelCryCreate(charNo, 30, 0.8f);
            }
            break;

        case CHAR_MOTNO(CHARMOT_HSF_c000m1_333):
            if(GetMotNoPlayTime(workP, motNo) == 0 && !(workP->attr & 0x10)) {
                _CharFXPlay(charNo, CHARVOICEID(11), voiceFlag);
            }
            break;

        case CHAR_MOTNO(CHARMOT_HSF_c000m1_311):
            if(frameNo == jumpAnimLen[charNo] && !(workP->attr & 0x10)) {
                CharModelLandDustCreate(charNo, &modelP->pos);
                for(i=0; i<2; i++) {
                    if(frameNo == jumpVoiceTimeTbl[(charNo*2)+i]) {
                        PlayStepVoice(charNo, CHARSEID(8), voiceFlag);
                        break;
                    }
                }
            }

            break;

        case CHAR_MOTNO(CHARMOT_HSF_c000m1_316):
        case CHAR_MOTNO(CHARMOT_HSF_c000m1_318):
        case CHAR_MOTNO(CHARMOT_HSF_c000m1_382):
            if(GetMotNoPlayTime(workP, motNo) == 0 && !(workP->attr & 0x10)) {
                _CharFXPlay(charNo, CHARVOICEID(9), voiceFlag);
            }
            break;

        case CHAR_MOTNO(CHARMOT_HSF_c000m1_357):
        case CHAR_MOTNO(CHARMOT_HSF_c000m1_381):
            if(GetMotNoPlayTime(workP, motNo) == 0 && !(workP->attr & 0x10)) {
                _CharFXPlay(charNo, CHARVOICEID(15), voiceFlag);
            }
            break;

        case CHAR_MOTNO(CHARMOT_HSF_c000m1_361):
            if(GetMotNoPlayTime(workP, motNo) == 0 && !(workP->attr & 0x10)) {
                _CharFXPlay(charNo, CHARVOICEID(3), voiceFlag);
            }
            break;

        case CHAR_MOTNO(CHARMOT_HSF_c000m1_377):
            if(GetMotNoPlayTime(workP, motNo) == 0 && !(workP->attr & 0x10)) {
                _CharFXPlay(charNo, CHARVOICEID(8), voiceFlag);
            }
            break;
    }
    if(!motShiftF) {
        if(!(attrOld & 0x4)) {
            workP->attr &= ~0x4;
        }
        if(!(attrOld & 0x2)) {
            workP->attr &= ~0x2;
        }
    }
}

static s16 winVoiceTbl[][2] = {
    { 0x1B, 0x243 },
    { 0x1C, 0x243 },
    { 0x2F, 0x243 },
    { 0x4C, 0x243 },
    { 0x4D, 0x243 },
    { 0x4E, 0x243 },
    { 0x4F, 0x243 },
    { 0x50, 0x243 },
    { 0x51, 0x243 },
    { -1, 0 },
};

static float CharYOfsTbl1[CHARNO_MAX] = { 110.0f, 160.0f, 110.0f, 160.0f, 150.0f, 180.0f, 130.0f, 160.0f, 130.0f, 160.0f, 150.0f, 180.0f, 120.0f, 210.0f };
static float CharYOfsTbl2[CHARNO_MAX] = { 110.0f, 160.0f, 110.0f, 160.0f, 130.0f, 160.0f, 110.0f, 160.0f, 130.0f, 160.0f, 130.0f, 160.0f, 130.0f, 160.0f };
static float birdYOfsTbl[CHARNO_MAX] = { 80.0f, 80.0f, 60.0f, 75.0f, 40.0f, 60.0f, 45.0f, 90.0f, 70.0f, 75.0f, 90.0f, 75.0f, 75.0f, 75.0f };

static s16 stepVoiceTbl[] = {
    0x7B, 0,
    0x7C, 0,
    0x7D, 0,
    0x7E, 0,
    0x7F, 0,
    0x80, 0,
    0x81, 0,
    0x82, 0,
    0x72, 0,
    0x73, 0,
    0x74, 0,
    0x5D, 1,
    0x5A, 1,
    0x59, 1,
    0x5B, 1,
    0x5E, 1,
    0x58, 1,
    0x6C, 1,
    0x5C, 1,
    0x63, 0,
    0x62, 0,
    0x60, 0,
    0x65, 0,
    0x61, 2,
    0x64, 0,
    0x66, 0,
    0x67, 0,
    0x68, 0,
    0x69, 0,
    0x6B, 0,
    0x6, 0,
    0x7, 0,
    0x8, 0,
    0x9, 1,
    0xA, 0,
    0xB, 0,
    0xC, 0,
    0xD, 2,
    0xE, 1,
    0xF, 0,
    0x10, 0,
    0x11, 0,
    0x12, 2,
    0x13, 0,
    0x14, 0,
    0x15, 0,
    0x16, 1,
    0x17, 1,
    0x18, 0,
    0x19, 1,
    0x1A, 2,
    0x1B, 0,
    0x1C, 0,
    0x1D, 0,
    0x1E, 1,
    0x1F, 2,
    0x20, 1,
    0x21, 0,
    0x22, 1,
    0x23, 1,
    0x24, 1,
    0x25, 1,
    0x26, 0,
    0x27, 0,
    0x28, 2,
    0x29, 1,
    0x2A, 1,
    0x2B, 0,
    0x2C, 0,
    0x2D, 0,
    0x2E, 0,
    0x2F, 0,
    0x30, 0,
    0x31, 0,
    0x32, 0,
    0x33, 1,
    0x34, 1,
    0x35, 2,
    0x36, 1,
    0x37, 0,
    0x38, 0,
    0x39, 0,
    0x3A, 0,
    0x3B, 2,
    0x3C, 0,
    0x3D, 0,
    0x3E, 1,
    0x3F, 0,
    0x40, 0,
    0x41, 0,
    0x42, 0,
    0x43, 1,
    0x44, 2,
    0x45, 1,
    0x46, 0,
    0x47, 0,
    0x48, 0,
    0x49, 0,
    0x4A, 0,
    0x4B, 0,
    0x4C, 0,
    0x4D, 0,
    0x4E, 0,
    0x4F, 1,
    0x50, 0,
    0x51, 1,
    0x52, 0,
    0x53, 0,
    0x54, 0,
    0x55, 0,
    0x56, 1,
    0x57, 0,
    -1, -1,
};

s16 _CharFXPlay(s16 charNo, s16 seNo, u8 voiceFlag)
{
    CHARWORK *workP = &CharWork[charNo];
    HU3D_MODEL *modelP = &Hu3DData[workP->modelId];
    if(!(voiceFlag & 0x1)) {
        if(workP->attr & 0x8) {
            CharFXPlayPos(charNo, seNo, &modelP->pos);
        } else {
            CharFXPlayVolPan(charNo, seNo, workP->vol, workP->pan);
        }
    }
}

static s16 CharWinVoicePlay(s16 charNo, u8 voiceFlag)
{
    s16 seNo;
    s16 i;
    s16 ret;
    for(i=0; winVoiceTbl[i][0] != DLL_NONE; i++) {
        if(omcurovl == winVoiceTbl[i][0]) {
            break;
        }
    }
    if(winVoiceTbl[i][0] != DLL_NONE) {
        seNo = winVoiceTbl[i][1];
    } else {
        seNo = CHARVOICEID(4);
    }
    _CharFXPlay(charNo, seNo, voiceFlag);
    return ret;
}

static void EffectParticleHook(HU3D_MODEL *modelP, HU3D_PARTICLE *particleP, Mtx mtx);

static void EffectInit(void)
{
    s16 effInitF = FALSE;
    s16 i;
    s16 cameraNo;
    ANIMDATA *anim[EFFECT_MAX];
    
    for(i=0; i<EFFECT_MAX; i++) {
        anim[i] = NULL;
    }
    for(cameraNo=0; cameraNo<HU3D_CAM_MAX; cameraNo++) {
        if(-1.0f == Hu3DCamera[cameraNo].fov) {
            continue;
        }
        for(i=0; i<EFFECT_MAX; i++) {
            if(effModelId[i][cameraNo] != HU3D_MODELID_NONE) {
                continue;
            }
            if(!anim[i]) {
                void *data = HuDataSelHeapReadNum(effectDataTbl[i].dataNum, HU_MEMNUM_OVL, HEAP_MODEL);
                anim[i] = HuSprAnimRead(data);
            }
            effModelId[i][cameraNo] = Hu3DParticleCreate(anim[i], effectDataTbl[i].maxCnt);
            if(i == EFFECT_BIRD) {
                Hu3DParticleAnimModeSet(effModelId[i][cameraNo], 0);
            }
            Hu3DParticleHookSet(effModelId[i][cameraNo], EffectParticleHook);
            Hu3DModelCameraSet(effModelId[i][cameraNo], (1 << cameraNo));
            if(!effParamAll[i]) {
                effParamAll[i] = HuMemDirectMalloc(HEAP_HEAP, effectDataTbl[i].maxCnt*sizeof(EFFECTPARAM));
            }
            Hu3DParticleBlendModeSet(effModelId[i][cameraNo], effectDataTbl[i].blendMode);
            {
                HU3D_PARTICLE *particleP = Hu3DData[effModelId[i][cameraNo]].hookData;
                HU3D_PARTICLE_DATA *particleDataP;
                s16 j;
                particleP->emitCnt = 0;
                particleP->work = effParamAll[i];
                particleP->count = 1;
                for(particleDataP = particleP->data, j=0; j<particleP->maxCnt; j++, particleDataP++) {
                    particleDataP->scale = 0;
                }
                effInitF = TRUE;
            }
        }
    }
    if(effInitF) {
        HuDataDirClose(DATA_effect);
    }
}

static void EffectParticleCreate(u8 type)
{
    ANIMDATA *anim = NULL;
    s16 cameraNo;
    HU3D_PARTICLE *particleP;
    
    for(cameraNo=0; cameraNo<HU3D_CAM_MAX; cameraNo++) {
        if(-1.0f == Hu3DCamera[cameraNo].fov) {
            continue;
        }
        if(effModelId[type][cameraNo] != HU3D_MODELID_NONE) {
            particleP = Hu3DData[effModelId[type][cameraNo]].hookData;
            if(particleP->anim) {
                anim = particleP->anim;
                continue;
            }
        } else {
            if(!anim) {
                void *data = HuDataSelHeapReadNum(effectDataTbl[type].dataNum, HU_MEMNUM_OVL, HEAP_MODEL);
                anim = HuSprAnimRead(data);
            }
            effModelId[type][cameraNo] = Hu3DParticleCreate(anim, effectDataTbl[type].maxCnt);
            Hu3DModelLayerSet(effModelId[type][cameraNo], effectLayer);
            if(type == EFFECT_BIRD) {
                Hu3DParticleAnimModeSet(effModelId[type][cameraNo], 0);
            }
            Hu3DParticleHookSet(effModelId[type][cameraNo], EffectParticleHook);
            Hu3DModelCameraSet(effModelId[type][cameraNo], (1 << cameraNo));
            if(!effParamAll[type]) {
                effParamAll[type] = HuMemDirectMalloc(HEAP_HEAP, effectDataTbl[type].maxCnt*sizeof(EFFECTPARAM));
            }
            Hu3DParticleBlendModeSet(effModelId[type][cameraNo], effectDataTbl[type].blendMode);
            {
                HU3D_PARTICLE_DATA *particleDataP;
                s16 j;
                particleP = Hu3DData[effModelId[type][cameraNo]].hookData;
                particleP->emitCnt = 0;
                particleP->work = effParamAll[type];
                particleP->count = 1;
                for(particleDataP = particleP->data, j=0; j<particleP->maxCnt; j++, particleDataP++) {
                    particleDataP->scale = 0;
                }
            }
        }
    }
}

static s16 EffectCreate(s16 type, s16 cameraBit, float posX, float posY, float posZ, float scale, EFFECTPARAM *param);

static s16 EffectDustCreate(s16 modelId, float posX, float posY, float posZ, float scale, EFFECTPARAM *param)
{
    HU3D_MODEL *modelP = &Hu3DData[modelId];
    if(modelP->scale.x != 1.0) {
        EFFECTPARAM paramNew = *param;
        paramNew.vel.x *= modelP->scale.x;
        paramNew.vel.y *= modelP->scale.x;
        paramNew.vel.z *= modelP->scale.x;
        paramNew.scaleVel *= modelP->scale.x;
        scale *= modelP->scale.x;
        return EffectCreate(EFFECT_DUST, modelP->cameraBit, posX, posY, posZ, scale, &paramNew);
    } else {
        return EffectCreate(EFFECT_DUST, modelP->cameraBit, posX, posY, posZ, scale, param);
    }
}

static s16 EffectSmokeCreate(HU3D_MODELID modelId, float posX, float posY, float posZ, float scale, EFFECTPARAM *param)
{
    HU3D_MODEL *modelP = &Hu3DData[modelId];
    if(modelP->scale.x != 1.0) {
        EFFECTPARAM paramNew = *param;
        paramNew.vel.x *= modelP->scale.x;
        paramNew.vel.y *= modelP->scale.x;
        paramNew.vel.z *= modelP->scale.x;
        paramNew.scaleVel *= modelP->scale.x;
        scale *= modelP->scale.x;
        return EffectCreate(EFFECT_SMOKE, modelP->cameraBit, posX, posY, posZ, scale, &paramNew);
    } else {
        return EffectCreate(EFFECT_SMOKE, modelP->cameraBit, posX, posY, posZ, scale, param);
    }
}

static s16 EffectHitCreate(HU3D_MODELID modelId, float posX, float posY, float posZ, float scale, EFFECTPARAM *param)
{
    HU3D_MODEL *modelP = &Hu3DData[modelId];
    if(modelP->scale.x != 1.0) {
        EFFECTPARAM paramNew = *param;
        paramNew.vel.x *= modelP->scale.x;
        paramNew.vel.y *= modelP->scale.x;
        paramNew.vel.z *= modelP->scale.x;
        paramNew.scaleVel *= modelP->scale.x;
        scale *= modelP->scale.x;
        return EffectCreate(EFFECT_HIT, modelP->cameraBit, posX, posY, posZ, scale, &paramNew);
    } else {
        return EffectCreate(EFFECT_HIT, modelP->cameraBit, posX, posY, posZ, scale, param);
    }
}

static inline s16 EffectStarCreate(HU3D_MODELID modelId, float posX, float posY, float posZ, float scale, EFFECTPARAM *param)
{
    HU3D_MODEL *modelP = &Hu3DData[modelId];
    if(modelP->scale.x != 1.0) {
        EFFECTPARAM paramNew = *param;
        paramNew.vel.x *= modelP->scale.x;
        paramNew.vel.y *= modelP->scale.x;
        paramNew.vel.z *= modelP->scale.x;
        paramNew.scaleVel *= modelP->scale.x;
        scale *= modelP->scale.x;
        return EffectCreate(EFFECT_STAR, modelP->cameraBit, posX, posY, posZ, scale, &paramNew);
    } else {
        return EffectCreate(EFFECT_STAR, modelP->cameraBit, posX, posY, posZ, scale, param);
    }
}

static inline s16 EffectWarnCreate(HU3D_MODELID modelId, float posX, float posY, float posZ, float scale, float ofsY, EFFECTPARAM *param)
{
    HU3D_MODEL *modelP = &Hu3DData[modelId];
    EFFECTPARAM paramNew = *param;
    s16 cameraNo, cameraBit;
    paramNew.vel.x *= modelP->scale.x;
    paramNew.vel.y *= modelP->scale.x;
    paramNew.vel.z *= modelP->scale.x;
    paramNew.scaleVel *= modelP->scale.x;
    scale *= modelP->scale.x;
    for(cameraNo=0, cameraBit=HU3D_CAM0; cameraNo<HU3D_CAM_MAX; cameraNo++, cameraBit <<= 1) {
        HU3D_MODEL *effModelP;
        HU3D_PARTICLE *particleP;
        HU3D_PARTICLE_DATA *particleDataP;
        s16 particleId;
        if(!(cameraBit & modelP->cameraBit)) {
            continue;
        }
        if(-1.0f == Hu3DCamera[cameraNo].fov) {
            continue;
        }
        particleId = EffectCreate(EFFECT_WARN, cameraBit, posX, posY, posZ, scale, &paramNew);
        if(particleId == -1) {
            return;
        } else {
            effModelP = &Hu3DData[effModelId[EFFECT_WARN][cameraNo]];
            particleP = effModelP->hookData;
            particleDataP = &particleP->data[particleId];
            particleDataP->parManId = 0;
            particleDataP->vel.x = modelId;
            particleDataP->vel.y = ofsY;
            particleDataP->accel.x = particleDataP->accel.z = 0;
            particleDataP->accel.y = 100;
        }
    }
}

static s16 EffectBirdCreate(HU3D_MODELID modelId, float posX, float posY, float posZ, float scale, s16 charNo, float ofsY, EFFECTPARAM *param)
{
    HU3D_MODEL *modelP = &Hu3DData[modelId];
    EFFECTPARAM paramNew = *param;
    s16 cameraNo, cameraBit;
    paramNew.vel.x *= modelP->scale.x;
    paramNew.vel.y *= modelP->scale.x;
    paramNew.vel.z *= modelP->scale.x;
    paramNew.scaleVel *= modelP->scale.x;
    scale *= modelP->scale.x;
    for(cameraNo=0, cameraBit=HU3D_CAM0; cameraNo<HU3D_CAM_MAX; cameraNo++, cameraBit <<= 1) {
        HU3D_MODEL *effModelP;
        HU3D_PARTICLE *particleP;
        HU3D_PARTICLE_DATA *particleDataP;
        s16 particleId;
        if(!(cameraBit & modelP->cameraBit)) {
            continue;
        }
        if(-1.0f == Hu3DCamera[cameraNo].fov) {
            continue;
        }
        particleId = EffectCreate(EFFECT_BIRD, cameraBit, posX, posY, posZ, scale, &paramNew);
        if(particleId == -1) {
            return;
        } else {
            effModelP = &Hu3DData[effModelId[EFFECT_BIRD][cameraNo]];
            particleP = effModelP->hookData;
            particleDataP = &particleP->data[particleId];
            particleDataP->parManId = 1;
            particleDataP->time = 0;
            particleDataP->vel.x = charNo;
            particleDataP->vel.y = ofsY;
        }
    }
}

static s16 EffectCreate(s16 type, s16 cameraBit, float posX, float posY, float posZ, float scale, EFFECTPARAM *param)
{
    s16 cameraNo, bit;
    s16 id;
    EffectParticleCreate(type);
    id = -1;
    for(cameraNo=0, bit=HU3D_CAM0; cameraNo<HU3D_CAM_MAX; cameraNo++, bit <<= 1) {
        HU3D_PARTICLE_DATA *particleDataP;
        HU3D_PARTICLE *particleP;
        s16 nextId;
        HU3D_MODEL *modelP;
        EFFECTPARAM *effParam;
        s16 modelId;
        if(!(bit & cameraBit)) {
            continue;
        }
        if(-1.0f == Hu3DCamera[cameraNo].fov) {
            continue;
        }
        modelId = effModelId[type][cameraNo];
        if(modelId == HU3D_MODELID_NONE) {
            return -1;
        }
        modelP = &Hu3DData[modelId];
        particleP = modelP->hookData;
        effParam = particleP->work;
        for(particleDataP=&particleP->data[particleP->emitCnt], nextId=particleP->emitCnt; nextId<particleP->maxCnt; nextId++, particleDataP++) {
            if(!particleDataP->scale) {
                break;
            }
        }
        if(nextId >= particleP->maxCnt) {
            for(particleDataP=&particleP->data[0], nextId=0; nextId<particleP->maxCnt; nextId++, particleDataP++) {
                if(!particleDataP->scale) {
                    break;
                }
            }
        }
        if(nextId != particleP->maxCnt) {
            effParam[nextId] = *param;
            particleDataP->cameraBit = cameraBit;
            particleDataP->pos.x = posX;
            particleDataP->pos.y = posY;
            particleDataP->pos.z = posZ;
            particleDataP->vel = param->vel;
            particleDataP->color = param->colorBegin;
            particleDataP->scaleBase = scale;
            particleDataP->scale = scale;
            particleDataP->time = 0;
            particleDataP->parManId = HU3D_PARMANID_NONE;
            particleP->emitCnt = nextId;
            id = nextId;
        } else {
            id = -1;
        }
    }
    return id;
}

static void UpdateEffect(HU3D_PARTICLE_DATA *particleDataP);
static void UpdateBirdEffect(HU3D_PARTICLE_DATA *particleDataP);
static void UpdateModelEffect(HU3D_PARTICLE_DATA *particleDataP);

static void EffectParticleHook(HU3D_MODEL *modelP, HU3D_PARTICLE *particleP, Mtx mtx)
{
    EFFECTPARAM *param = particleP->work;
    HU3D_PARTICLE_DATA *particleDataP;
    s16 color;
    s16 i;
    if(particleP->count == 0) {
        for(particleDataP=&particleP->data[0], i=0; i<particleP->maxCnt; i++, particleDataP++) {
            particleDataP->scale = 0;
        }
    }
    for(particleDataP=&particleP->data[0], i=0; i<particleP->maxCnt; i++, particleDataP++) {
        if(!particleDataP->scale) {
            continue;
        }
        if(particleDataP->parManId == HU3D_PARMANID_NONE) {
            particleDataP->vel.x *= param[i].velDecay.x;
            particleDataP->vel.y *= param[i].velDecay.y;
            particleDataP->vel.z *= param[i].velDecay.z;
            VECAdd(&particleDataP->vel, &particleDataP->pos, &particleDataP->pos);
            particleDataP->vel.y += param[i].gravity;
            color = particleDataP->color.r+(param[i].colorWeight*(param[i].colorEnd.r-param[i].colorBegin.r));
            if(color < 0) {
                color = 0;
            } else if(color > 255) {
                color = 255;
            }
            particleDataP->color.r = color;
            color = particleDataP->color.g+(param[i].colorWeight*(param[i].colorEnd.g-param[i].colorBegin.g));
            if(color < 0) {
                color = 0;
            } else if(color > 255) {
                color = 255;
            }
            particleDataP->color.g = color;
            color = particleDataP->color.b+(param[i].colorWeight*(param[i].colorEnd.b-param[i].colorBegin.b));
            if(color < 0) {
                color = 0;
            } else if(color > 255) {
                color = 255;
            }
            particleDataP->color.b = color;
            color = particleDataP->color.a+param[i].alphaBase;
            if(color < 1) {
                particleDataP->scale = 0;
            }
            particleDataP->color.a = color;
            if(particleDataP->scale) {
                if(param[i].attr & 0x1) {
                    particleDataP->scale = particleDataP->scaleBase*(((particleDataP->time+i) & 0x1) ? 1.0 : 0.5);
                } else {
                    particleDataP->scale = particleDataP->scaleBase;
                }
                particleDataP->scaleBase += param[i].scaleVel;
                if(particleDataP->scaleBase <= 0.01f) {
                    particleDataP->scale = 0;
                }
            }
            particleDataP->time++;
        } else {
            switch(particleDataP->parManId) {
                case 0:
                    UpdateEffect(particleDataP);
                    break;
                
                case 1:
                    UpdateBirdEffect(particleDataP);
                    break;
                    
                case 2:
                    UpdateModelEffect(particleDataP);
                    break;
                
            }
        }
    }
    DCStoreRangeNoSync(particleP->data, particleP->maxCnt*sizeof(HU3D_PARTICLE_DATA));
}

static void UpdateEffect(HU3D_PARTICLE_DATA *particleDataP)
{
    HU3D_MODEL *modelP = &Hu3DData[(int)particleDataP->vel.x];
    float radius;
    if(particleDataP->time < 8) {
        radius = 0.3+HuSin(40.0f+(10.0f*(particleDataP->time+1)));
        particleDataP->scale = (50.0f*radius)*modelP->scale.x;
        particleDataP->color.a = 255;
        radius = 0.3+HuSin(15.0f*(particleDataP->time+1));
    } else {
        radius = 0.3+HuSin(135.0f);
    }
    radius *= modelP->scale.x;
    particleDataP->pos.x = modelP->pos.x+(particleDataP->accel.x*radius);
    particleDataP->pos.y = (modelP->pos.y+(particleDataP->vel.y*modelP->scale.x))+(particleDataP->accel.y*radius);
    particleDataP->pos.z = modelP->pos.z+(particleDataP->accel.z*radius);
    if(particleDataP->time > 20) {
        particleDataP->color.a -= 32;
        particleDataP->scale -= 8.0f*modelP->scale.x;
        if(particleDataP->scale < 0.0f) {
            particleDataP->scale = 0.0f;
        }
    }
    particleDataP->time++;
}


static void UpdateBirdEffect(HU3D_PARTICLE_DATA *particleDataP)
{
    s16 charNo = (s16)particleDataP->vel.x;
    s16 angle;
    CHARWORK *workP;
    HU3D_MODEL *modelP;
    s16 effectNo;
    BOOL birdMotPlayF;
    
    birdMotPlayF = TRUE;
    workP = &CharWork[charNo];
    modelP = &Hu3DData[workP->modelId];
    
    for(effectNo=0; effectDataTbl[EFFECT_BIRD].motDataNum[effectNo]; effectNo++) {
        if(workP->motNoCurr == CHAR_MOTNO(effectDataTbl[EFFECT_BIRD].motDataNum[effectNo])) {
            break;
        }
    }
    if(effectDataTbl[EFFECT_BIRD].motDataNum[effectNo] == 0) {
        birdMotPlayF = FALSE;
    }
    if(particleDataP->time < 20 && particleDataP->scale < 40.0f*modelP->scale.x) {
        particleDataP->scale += 4.0f*modelP->scale.x;
    }
    particleDataP->color.a = 255;
    angle = (particleDataP->time*5)%360;
    Hu3DModelObjPosGet(workP->modelId, CharHeadObjNameTbl[charNo], &particleDataP->pos);
    particleDataP->pos.x = particleDataP->pos.x+(modelP->scale.x*(40.0*HuSin(particleDataP->vel.y+angle)));
    particleDataP->pos.y = particleDataP->pos.y+(modelP->scale.y*birdYOfsTbl[charNo]);
    particleDataP->pos.z = particleDataP->pos.z+(modelP->scale.z*(40.0*HuCos(particleDataP->vel.y+angle)));
    particleDataP->time++;
    if(particleDataP->time >= 143) {
        particleDataP->time = 72;
    }
    if(!birdMotPlayF && particleDataP->time > 30) {
        particleDataP->scale -= 8.0f*modelP->scale.x;
        if(particleDataP->scale < 0.0f) {
            particleDataP->scale = 0.0f;
            workP->attr &= ~0x1;
            if(particleDataP->vel.y == 0.0) {
                _CharFXPlay(charNo, CHARSEID(24), workP->attr);

            }
        }
    }
}


static s16 GetStepType(void)
{
    s16 i;
    for(i=0; stepVoiceTbl[i] != DLL_NONE; i++) {
        if(omcurovl == stepVoiceTbl[(i*2)+0]) {
            return stepVoiceTbl[(i*2)+1];
        }
    }
    return 0;
}

static inline void MotionParticleInit(unsigned int dataNum)
{
    s16 i;
    s16 j;
    dataNum &= 0xFFFF;
    for(i=0; i<EFFECT_MAX; i++) {
        for(j=0; effectDataTbl[i].motDataNum[j]; j++) {
            if(dataNum == CHAR_MOTNO(effectDataTbl[i].motDataNum[j])) {
                EffectParticleCreate(i);
                break;
            }
        }
    }
}

HU3D_MOTIONID CharMotionCreate(s16 charNo, unsigned int dataNum)
{
    CHARWORK *workP = &CharWork[charNo];
    s16 i;
    s16 motNo;
    void *data;
    unsigned int dir;
    if(workP->modelId == HU3D_MODELID_NONE) {
        return HU3D_MOTIONID_NONE;
    }
    for(motNo=0; motNo<CHAR_MOT_MAX; motNo++) {
        if(workP->motId[motNo] == HU3D_MOTIONID_NONE) {
            break;
        }
    }
    if(motNo == CHAR_MOT_MAX) {
        return HU3D_MOTIONID_NONE;
    }
    dir = dataNum & 0xFFFF0000;
    for(i=0; i<CHARNO_MAX; i++) {
        if(dir == CharDataDirTbl[i][4]) {
            break;
        }
    }
    if(i != CHARNO_MAX || dir == 0) {
        dataNum = FILENUM(dataNum);
        data = HuAR_ARAMtoMRAMFileRead(DATANUM(CharDataDirTbl[charNo][4], dataNum), HU_MEMNUM_OVL, HEAP_MODEL);
        if(!data) {
            data = HuDataSelHeapReadNum(DATANUM(CharDataDirTbl[charNo][4], dataNum), HU_MEMNUM_OVL, HEAP_MODEL);
        }
        workP->motNoTbl[motNo] = dataNum;
    } else {
        for(i=0; i<CHARNO_MAX; i++) {
            if(dir == CharDataDirTbl[i][5]) {
                break;
            }
        }
        if(i != CHARNO_MAX) {
            dataNum = DATANUM(CharDataDirTbl[charNo][5], dataNum & 0xFFFF);
            data = HuDataReadNumHeapShortForce(dataNum, HU_MEMNUM_OVL, HEAP_MODEL);
        } else {
            data = HuDataSelHeapReadNum(dataNum, HU_MEMNUM_OVL, HEAP_MODEL);
        }
        workP->motNoTbl[motNo] = HU3D_MOTIONID_NONE;
    }
    workP->motId[motNo] = Hu3DJointMotion(workP->modelId, data);
    workP->voiceFlag[motNo] = 0;
    MotionParticleInit(dataNum);
    return workP->motId[motNo];
}

void CharMotionNoSet(s16 charNo, HU3D_MOTIONID motId, unsigned int motNo)
{
    CHARWORK *workP = &CharWork[charNo];
    s16 i;
    for(i=0; i<CHAR_MOT_MAX; i++) {
        if(workP->motId[i] == motId) {
            break;
        }
    }
    if(i != CHAR_MOT_MAX) {
        workP->motNoTbl[i] = motNo;
    }
}

void CharMotionKill(s16 charNo, unsigned int motId)
{
    CHARWORK *workP = &CharWork[charNo];
    s16 i;
    for(i=0; i<CHAR_MOT_MAX; i++) {
        if(workP->motId[i] == motId) {
            break;
        }
    }
    workP->motId[i] = HU3D_MOTIONID_NONE;
    Hu3DMotionKill(motId);
    
}

void CharMotionDataClose(s16 charNo)
{
    s16 i;
    if(charNo == CHARNO_NONE) {
        for(i=0; i<CHARNO_MAX; i++) {
            CharMotionDataClose(i);
        }
    } else {
        HuDataDirClose(CharDataDirTbl[charNo][4]);
        HuDataDirClose(DATA_effect);
    }
}

void CharModelDataClose(s16 charNo)
{
    s16 i;
    if(charNo == CHARNO_NONE) {
        for(i=0; i<CHARNO_MAX; i++) {
            CharModelDataClose(i);
        }
    } else {
        for(i=0; i<5; i++) {
            HuDataDirClose(CharDataDirTbl[charNo][i]);
        }
        HuDataDirClose(DATA_effect);
    }
}

void CharModelKill(s16 charNo)
{
    s16 i;
    s16 cameraNo;
    if(charNo == CHARNO_NONE) {
        for(i=0; i<CHARNO_MAX; i++) {
            CharModelKill(i);
        }
        for(i=0; i<CHAR_NPC_NO_BASE; i++) {
            dustFlags[i] = 0;
        }
    } else {
        CHARWORK *workP;
        CharMotionDataClose(charNo);
        workP = &CharWork[charNo];
        for(i=0; i<CHAR_MOT_MAX; i++) {
            if(workP->motId[i] != HU3D_MOTIONID_NONE) {
                Hu3DMotionKill(workP->motId[i]);
            }
            workP->motId[i] = HU3D_MOTIONID_NONE;
            workP->motNoTbl[i] = HU3D_MOTIONID_NONE;
        }
        if(workP->modelId != HU3D_MODELID_NONE) {
            Hu3DModelKill(workP->modelId);
        }
        workP->modelId = HU3D_MODELID_NONE;
        for(i=0; i<CHARNO_MAX; i++) {
            if(CharWork[i].modelId != HU3D_MODELID_NONE) {
                break;
            }                
        }
        if(i == CHARNO_MAX) {
            for(i=0; i<EFFECT_MAX; i++) {
                for(cameraNo=0; cameraNo<HU3D_CAM_MAX; cameraNo++) {
                    if(effModelId[i][cameraNo] != HU3D_MODELID_NONE) {
                        Hu3DModelKill(effModelId[i][cameraNo]);
                    }
                    effModelId[i][cameraNo] = HU3D_MODELID_NONE;
                }
            }
            effectLayer = 0;
            for(i=0; i<EFFECT_MAX; i++) {
                if(effParamAll[i]) {
                    HuMemDirectFree(effParamAll[i]);
                }
                effParamAll[i] = NULL;
            }
        }
        if(workP->process) {
            HuMemDirectFree(workP->process->property);
            HuPrcKill(workP->process);
            for(i=0; i<CHAR_MOT_MAX; i++) {
                if(hookDustProc[i]) {
                    HuPrcKill(hookDustProc[i]);
                }
                hookDustProc[i] = NULL;
            }
            workP->process = NULL;
        }
        workP->vol = MSM_VOL_MAX;
        workP->pan = MSM_PAN_CENTER;
    }
}

void CharMotionSet(s16 charNo, HU3D_MOTIONID motId)
{
    CHARWORK *workP = &CharWork[charNo];
    HU3D_MOTION *motP = &Hu3DMotion[motId];
    EyeBmpUpdate(charNo);
    Hu3DMotionSet(workP->modelId, motId);
}

static void EyeBmpUpdate(s16 charNo)
{
    CHARWORK *workP = &CharWork[charNo];
    HU3D_MODEL *modelP = &Hu3DData[workP->modelId];
    HSF_ATTRIBUTE *attrP = modelP->hsf->attribute;
    s16 i;
    s16 modelBit;
    char **eyeBmp;
    for(i=0, modelBit=1; i<CHAR_MODEL_MAX; i++, modelBit <<= 1) {
        if(modelBit & workP->model) {
            break;
        }
    }
    if(i >= CHAR_MODEL_MAX) {
        return;
    }
    eyeBmp = CharModelEyeBmpGet(charNo, workP->model);
    for(i=0; i<modelP->hsf->attributeNum; i++, attrP++) {
        if(attrP->bitmap->name[0] != eyeBmp[0][0] || strcmp(attrP->bitmap->name, eyeBmp[0])) {
            if(attrP->bitmap->name[0] != eyeBmp[1][0] || strcmp(attrP->bitmap->name, eyeBmp[1])) {
                continue;
            }
        }
        if(attrP->animWorkP) {
            HU3D_ATTR_ANIM *attrAnimP = attrP->animWorkP;
            attrAnimP->trans3D.x = attrAnimP->trans3D.y = attrAnimP->trans3D.z = 0;
            attrAnimP->rot.x = attrAnimP->rot.y = attrAnimP->rot.z = 0;
        }
    }
}

char *CharEyeBmpNameTbl[CHARNO_MAX*CHAR_MODEL_MAX*2] = {
    "s3c000m1_eyes", "s3c000m1_eyes", "s3c000m1_eyes", "s3c000m1_eyes", "s3c000m2_eyes", "s3c000m2_eyes", "mario_eyes", "mario_eyes",
    "S3c001m0_eye", "S3c001m0_eye", "S3c001m1_eye", "S3c001m1_eye", "c001m3_eye", "c001m3_eye", "c001m3_eye", "c001m3_eye",
    "s3c002m0_r_eye", "s3c002m0_l_eye", "s3c002m1_r_eye", "s3c002m1_l_eye", "s3c002m2_r_eye", "s3c002m2_l_eye", "", "",
    "", "", "", "", "", "", "S3c003m3", "S3c003m3",
    "s3c004m0_eye", "s3c004m0_eye", "s3c004m1_eye", "s3c004m1_eye", "s3c004m3_eye", "s3c004m3_eye", "s3c004m3_eye", "s3c004m3_eye",
    "s3c007m0_Eye_L", "s3c007m0_Eye_R", "s3c007m1_Eye_L", "s3c007m1_Eye_R", "s3c006m2_eye", "s3c006m2_eye_R", "", "",
    "s3c007_m0_eye", "s3c007_m0_eye", "s3c007_m1_eye", "s3c007_m1_eye", "s3c007_m2_eye", "s3c007_m2_eye", "s3c007_m3_eye", "s3c007_m3_eye",
    "c008m1_eyes1", "c008m1_eyes1", "c008m1_eyes1", "c008m1_eyes1", "c008m1_eyes1", "c008m1_eyes1", "", "",
    "", "", "", "", "", "", "", "",
    "", "", "", "", "", "", "", "",
    "c011m1_eyes1", "c011m1_eyes1", "c011m1_eyes1", "c011m1_eyes1", "c008m1_eyes1", "c008m1_eyes1", "", "",
    "", "", "", "", "", "", "", "",
    "", "", "", "", "", "", "", "",
    "", "", "", "", "", "", "", "",
};

char **CharModelEyeBmpGet(s16 charNo, s16 model)
{
    s16 i;
    s16 bit;
    for(i=0, bit=CHAR_MODEL0; i<CHAR_MODEL_MAX-1; i++, bit <<= 1) {
        if(bit & model) {
            break;
        }
    }
    return &CharEyeBmpNameTbl[(charNo*(CHAR_MODEL_MAX*2))+(i*2)];
}

static char *CharHookNameTbl[CHARNO_MAX*5] = {
    "a-itemhook-r", "a-itemhook-l", "a-itemhook-fr", "a-itemhook-fl", "a-itemhook-body",
    "a-itemhook-r", "a-itemhook-l", "a-itemhook-fr", "a-itemhook-fl", "a-itemhook-body",
    "a-itemhook-r", "a-itemhook-l", "a-itemhook-fr", "a-itemhook-fl", "a-itemhook-body",
    "a-itemhook-r", "a-itemhook-l", "a-itemhook-fr", "a-itemhook-fl", "a-itemhook-body",
    "a-itemhook-r", "a-itemhook-l", "a-itemhook-fr", "a-itemhook-fl", "a-itemhook-body",
    "a-itemhook-r", "a-itemhook-l", "a-itemhook-fr", "a-itemhook-fl", "a-itemhook-body",
    "a-itemhook-r", "a-itemhook-l", "a-itemhook-fr", "a-itemhook-fl", "a-itemhook-body",
    "a-itemhook-r", "a-itemhook-l", "a-itemhook-fr", "a-itemhook-fl", "a-itemhook-body",
    "a-itemhook-r", "a-itemhook-l", "a-itemhook-fr", "a-itemhook-fl", "a-itemhook-body",
    "a-itemhook-r", "a-itemhook-l", "a-itemhook-fr", "a-itemhook-fl", "a-itemhook-body",
    "a-itemhook-r", "a-itemhook-l", "a-itemhook-fr", "a-itemhook-fl", "a-itemhook-body",
    "a-itemhook-r", "a-itemhook-l", "a-itemhook-fr", "a-itemhook-fl", "a-itemhook-body",
    "a-itemhook-r", "a-itemhook-l", "a-itemhook-fr", "a-itemhook-fl", "a-itemhook-body",
    "a-itemhook-r", "a-itemhook-l", "a-itemhook-fr", "a-itemhook-fl", "a-itemhook-body",
};

char *CharModelItemHookGet(s16 charNo, s16 model, s16 hookNo)
{
    s16 i;
    s16 bit;
    for(i=0, bit=CHAR_MODEL0; i<CHAR_MODEL_MAX-1; i++, bit <<= 1) {
        if(bit & model) {
            break;
        }
    }
    return CharHookNameTbl[(charNo*5)+hookNo];
}

void CharMotionTimeSet(s16 charNo, float time)
{
    CHARWORK *workP = &CharWork[charNo];
    Hu3DMotionTimeSet(workP->modelId, time);
}

float CharMotionTimeGet(s16 charNo)
{
    CHARWORK *workP = &CharWork[charNo];
    return Hu3DMotionTimeGet(workP->modelId);
}

float CharMotionMaxTimeGet(s16 charNo)
{
    CHARWORK *workP = &CharWork[charNo];
    return Hu3DMotionMaxTimeGet(workP->modelId);
}

BOOL CharMotionEndCheck(s16 charNo)
{
    CHARWORK *workP = &CharWork[charNo];
    return Hu3DMotionEndCheck(workP->modelId);
}

HU3D_MOTIONID CharMotionShiftIDGet(s16 charNo)
{
    CHARWORK *workP = &CharWork[charNo];
    return Hu3DMotionShiftIDGet(workP->modelId);
}

void CharMotionShiftSet(s16 charNo, HU3D_MOTIONID motId, float start, float end, u32 attr)
{
    CHARWORK *workP = &CharWork[charNo];
    HU3D_MOTION *motP = &Hu3DMotion[motId];
    Hu3DMotionShiftSet(workP->modelId, motId, start, end, attr);
}

float CharMotionShiftTimeGet(s16 charNo)
{
    CHARWORK *workP = &CharWork[charNo];
    return Hu3DMotionShiftTimeGet(workP->modelId);
}

void CharMotionSpeedSet(s16 charNo, float speed)
{
    CHARWORK *workP = &CharWork[charNo];
    Hu3DMotionSpeedSet(workP->modelId, speed);
}

void CharMotionStartEndSet(s16 charNo, float start, float end)
{
    CHARWORK *workP = &CharWork[charNo];
    Hu3DMotionStartEndSet(workP->modelId, start, end);
}

void CharModelAttrSet(s16 charNo, u32 attr)
{
    CHARWORK *workP = &CharWork[charNo];
    Hu3DModelAttrSet(workP->modelId, attr);
}

void CharModelAttrReset(s16 charNo, u32 attr)
{
    CHARWORK *workP = &CharWork[charNo];
    Hu3DModelAttrReset(workP->modelId, attr);
}

static float modelHeightTbl[CHARNO_MAX] = { 150.0f, 170.0f, 188.0f, 169.0f, 164.0f, 180.0f, 210.0f, 120.0f, 150.0f, 150.0f, 120.0f, 150.0f, 150.0f, 150.0f };

float CharModelHeightGet(s16 charNo)
{
    return modelHeightTbl[charNo];
}

HU3D_MODELID CharModelMotListCreate(s16 charNo, s16 model, unsigned int *motDataNum, HU3D_MOTIONID *motId)
{
    HU3D_MODELID modelId = CharModelCreate(charNo, model);
    s16 no = 0;
    do {
        if(motDataNum[no] == 0) {
            break;
        }
        motId[no] = CharMotionCreate(charNo, motDataNum[no]);
        no++;
    } while(1);
    CharMotionDataClose(charNo);
    return modelId;
}

s32 CharModelTimingHookNoGet(s16 charNo)
{
    CHARWORK *workP = &CharWork[charNo];
    if(workP->timingHookNo == 0) {
        return 0;
    } else if(workP->timingHookNo == 1) {
        return 1;
    } else {
        return 2;
    }
}

typedef struct HookDustWork_s {
    u16 cameraBit;
    HU3D_MODELID modelId;
} HOOKDUSTWORK;

static void CreateHookDust(void);

static inline HUPROCESS *CreateHookDustProc(void)
{
    s16 i;
    for(i=0; i<CHAR_MOT_MAX; i++) {
        if(!hookDustProc[i]) {
            break;
        }
    }
    if(i == CHAR_MOT_MAX) {
        return NULL;
    }
    hookDustProc[i] = HuPrcCreate(CreateHookDust, 100, 8192, 0);
    return hookDustProc[i];
}

static inline void KillHookDustProc(HUPROCESS *process)
{
    s16 i;
    for(i=0; i<CHAR_MOT_MAX; i++) {
        if(hookDustProc[i] == process) {
            HuPrcKill(process);
            hookDustProc[i] = NULL;
        }
    }
}

void CharModelHookDustCreate(s16 charNo, char *objName)
{
    CHARWORK *workP = &CharWork[charNo];
    HU3D_MODEL *modelP = &Hu3DData[workP->modelId];
    HSF_OBJECT *objPtr = Hu3DModelObjPtrGet(workP->modelId, objName);
    HSF_CONSTDATA *constData;
    HOOKDUSTWORK *hookDustWork;
    HU3D_MODELID hookMdlId;
    HUPROCESS *process;
    Mtx hookMtx;
    HuVecF temp;
    
    Hu3DModelObjMtxGet(workP->modelId, objName, hookMtx);
    constData = objPtr->constData;
    hookMdlId = constData->hookMdlId;
    if(hookMdlId == HU3D_MODELID_NONE) {
        return;
    }
    Hu3DModelHookObjReset(workP->modelId, objName);
    process = CreateHookDustProc();
    if(!process) {
        Hu3DModelDispOff(hookMdlId);
        return;
    }
    process->property = hookDustWork = HuMemDirectMallocNum(HEAP_HEAP, sizeof(HOOKDUSTWORK), HU_MEMNUM_OVL);
    modelP = &Hu3DData[hookMdlId];
    Hu3DMtxTransGet(hookMtx, &temp);
    Hu3DModelPosSetV(hookMdlId, &temp);
    Hu3DMtxRotGet(hookMtx, &temp);
    mtxRot(modelP->mtx, temp.x, temp.y, temp.z);
    Hu3DMtxScaleGet(hookMtx, &temp);
    Hu3DModelScaleSetV(hookMdlId, &temp);
    hookDustWork->modelId = hookMdlId;
    hookDustWork->cameraBit = modelP->cameraBit;
}

static void CreateHookDust(void)
{
    HOOKDUSTWORK *hookDustWork = HuPrcCurrentGet()->property;
    HU3D_MODEL *modelP = &Hu3DData[hookDustWork->modelId];
    Mtx rootMtx;
    HuVecF pos;
    s16 i;
    Hu3DModelObjMtxGet(hookDustWork->modelId, "", rootMtx);
    pos.x = PGMinPos.x+((PGMaxPos.x-PGMinPos.x)/2);
    pos.y = PGMinPos.y+((PGMaxPos.y-PGMinPos.y)/2);
    pos.z = PGMinPos.z+((PGMaxPos.z-PGMinPos.z)/2);
    for(i=0; i<40; i++) {
        modelP->pos.y += 4.0f;
        mtxRotCat(modelP->mtx, 24, 0, 0);
        modelP->scale.x *= 0.95f;
        modelP->scale.y *= 0.95f;
        modelP->scale.z *= 0.95f;
        HuPrcVSleep();
    }
    Hu3DModelObjMtxGet(hookDustWork->modelId, "", rootMtx);
    pos.x = PGMinPos.x+((PGMaxPos.x-PGMinPos.x)/2);
    pos.y = PGMinPos.y+((PGMaxPos.y-PGMinPos.y)/2);
    pos.z = PGMinPos.z+((PGMaxPos.z-PGMinPos.z)/2);
    Hu3DModelDispOff(hookDustWork->modelId);
    dustEffParam.vel.x = 0;
    dustEffParam.vel.y = 0;
    dustEffParam.vel.z = 0;
    EffectDustCreate(hookDustWork->modelId, pos.x, pos.y, pos.z, 40.0f, &dustEffParam);
    for(i=0; i<8; i++) {
        dustEffParam.vel.x = frandmod(10)-5;
        dustEffParam.vel.y = frandmod(10)-5;
        dustEffParam.vel.z = frandmod(10)-5;
        EffectDustCreate(hookDustWork->modelId, pos.x, pos.y, pos.z, 20.0f, &dustEffParam);
    }
    MTXIdentity(modelP->mtx);
    KillHookDustProc(HuPrcCurrentGet());
    while(1) {
        HuPrcVSleep();
    }
}

static EFFECTPARAM modelSmokeEffParam = {
    0,
    { 255, 255, 255, 255 },
    { 64, 32, 0, 255 },
    { 0, 2, 1 },
    { 0.95f, 0.95f, 0.95f },
    0,
    0,
    1,
    -5,
    0.02f
};

void CharEffectSmokeCreateScale(s16 cameraBit, HuVecF *pos, float scale)
{
    HU3D_PARTICLE_DATA *particleDataP;
    s16 i;
    s16 effectNo;
    HU3D_PARTICLE *particleP;
    HU3D_MODEL *modelP;
    s16 bit;
    s16 cameraNo;
    for(bit=HU3D_CAM0, cameraNo=0; cameraNo<HU3D_CAM_MAX; cameraNo++, bit <<= 1) {
        if(cameraBit & bit) {
            for(i=0; i<8; i++) {
                effectNo = EffectCreate(EFFECT_SMOKE, bit, pos->x, pos->y, pos->z, 20*scale, &modelSmokeEffParam);
                if(effectNo == -1) {
                    break;
                }
                modelP = &Hu3DData[effModelId[EFFECT_SMOKE][cameraNo]];
                particleP = modelP->hookData;
                particleDataP = &particleP->data[effectNo];
                particleDataP->parManId = 2;
                particleDataP->vel.x = scale*(30*HuSin(i*45));
                particleDataP->vel.y = scale*(30*HuCos(i*45));
                particleDataP->vel.z = 0;
                particleDataP->accel = *pos;
                particleDataP->speedDecay = 1.15f;
                particleDataP->colorIdx = scale*(0.1f*(frandmod(20)-10));
                particleDataP->color.a = 255-(frandmod(3)*16);
            }
            for(i=0; i<8; i++) {
                effectNo = EffectCreate(EFFECT_SMOKE, bit, pos->x, pos->y, pos->z, 10*scale, &modelSmokeEffParam);
                if(effectNo == -1) {
                    break;
                }
                modelP = &Hu3DData[effModelId[EFFECT_SMOKE][cameraNo]];
                particleP = modelP->hookData;
                particleDataP = &particleP->data[effectNo];
                particleDataP->parManId = 2;
                particleDataP->vel.x = scale*(frandmod(100)-50);
                particleDataP->vel.y = scale*(frandmod(100)-50);
                particleDataP->vel.z = scale*(frandmod(100)-50);
                particleDataP->accel = *pos;
                particleDataP->speedDecay = 1.15f;
                particleDataP->colorIdx = scale*(0.1f*(frandmod(20)-10));
                particleDataP->color.a = 255-(frandmod(3)*16);
            }
            effectNo = EffectCreate(EFFECT_SMOKE, bit, pos->x, pos->y, pos->z, 10*scale, &modelSmokeEffParam);
            if(effectNo == -1) {
                break;
            }
            modelP = &Hu3DData[effModelId[EFFECT_SMOKE][cameraNo]];
            particleP = modelP->hookData;
            particleDataP = &particleP->data[effectNo];
            particleDataP->parManId = 2;
            particleDataP->vel.x = 0;
            particleDataP->vel.y = 0;
            particleDataP->vel.z = 0;
            particleDataP->accel = *pos;
            particleDataP->colorIdx = 0;
            particleDataP->speedDecay = 1.15f;
            particleDataP->color.a = 255;
        }
    }
}

void CharEffectSmokeCreate(s16 cameraBit, HuVecF *pos)
{
    CharEffectSmokeCreateScale(cameraBit, pos, 1.0f);
}

static void UpdateModelEffect(HU3D_PARTICLE_DATA *particleDataP)
{
    float speed;
    float angle;
    angle = 20+(3.75f*particleDataP->time);
    if(angle > 90.0f) {
        angle = 90.0f;
    }
    speed = HuSin(angle);
    particleDataP->pos.x = particleDataP->accel.x+(particleDataP->vel.x*speed);
    particleDataP->pos.y = particleDataP->accel.y+(particleDataP->vel.y*speed);
    particleDataP->pos.z = particleDataP->accel.z+(particleDataP->vel.z*speed);
    particleDataP->scale *= particleDataP->speedDecay;
    particleDataP->speedDecay -= 0.01;
    if(particleDataP->speedDecay < 1.0f) {
        particleDataP->speedDecay = 1.0f;
    }
    if(particleDataP->time > 8) {
        s16 alpha = particleDataP->color.a;
        alpha -= 8;
        if(alpha < 0) {
            particleDataP->color.a = 0;
            particleDataP->scale = 0;
        } else {
            particleDataP->color.a = alpha;
        }
    }
    particleDataP->time++;
}

static EFFECTPARAM coinEffParam = {
    1,
    { 255, 255, 0, 255 },
    { 255, 255, 0, 255 },
    { 0, 2, 1 },
    { 0.95f, 1.0f, 0.95f },
    -0.1f,
    0,
    -0.2f,
    -8,
    0
};

void CharEffectCoinGlowCreate(s16 cameraBit, HuVecF *pos)
{
    s16 i;
    for(i=0; i<16; i++) {
        s16 angle = i*(360.0f/16.0f);
        s16 effectNo;
        coinEffParam.vel.x = 5*HuSin(angle);
        coinEffParam.vel.y = 0.1f*(frandmod(100)-50);
        coinEffParam.vel.z = 5*HuCos(angle);
        effectNo = EffectCreate(EFFECT_GLOW, cameraBit, pos->x, pos->y, pos->z, 30, &coinEffParam);
        if(effectNo == -1) {
            break;
        }
    }
}

static EFFECTPARAM modelHitEffParam = {
    0,
    { 255, 255, 0, 255 },
    { 255, 128, 0, 255 },
    { 0, 2, 1 },
    { 0.95f, 0.95f, 0.95f },
    0,
    0,
    -0.2f,
    -16,
    0.05f
};

static EFFECTPARAM hitGlowEffParam = {
    0,
    { 224, 255, 32, 255 },
    { 224, 255, 32, 255 },
    { 0, 2, 1 },
    { 1.0f, 1.0f, 1.0f },
    0,
    0,
    -0.2f,
    -12,
    0.05f
};

void CharModelHitCreate(s16 charNo)
{
    CHARWORK *workP = &CharWork[charNo];
    HU3D_MODEL *modelP = &Hu3DData[workP->modelId];
    s16 motNo;
    Mtx mtx;
    HuVecF pos;
    HuVecF radius;
    
    for(motNo=0; motNo<CHAR_MOT_MAX; motNo++) {
        if(workP->motId[motNo] == modelP->motId) {
            break;
        }
    }
    if(motNo == CHAR_MOT_MAX) {
        return;
    }
    Hu3DModelObjMtxGet(workP->modelId, "test11_tex_we-itemhook-r", mtx);
    pos.x = mtx[0][3];
    pos.y = mtx[1][3];
    pos.z = mtx[2][3];
    mtxRot(mtx, modelP->rot.x, modelP->rot.y, modelP->rot.z);
    radius.x = mtx[0][2];
    radius.y = mtx[1][2];
    radius.z = mtx[2][2]; 
    VECScale(&radius, &radius, 20);
    VECAdd(&pos, &radius, &pos);
    CharEffectHitCreate(modelP->cameraBit, &pos, &modelP->rot);
}

void CharEffectHitCreate(s16 cameraBit, HuVecF *pos, HuVecF *rot)
{
    Mtx mtx;
    HuVecF radius;
    HuVecF dir;
    HuVecF vel;
    s16 i;
    s16 effectNo;
    float angle;
    mtxRot(mtx, rot->x, rot->y, rot->z);
    radius.x = mtx[0][2];
    radius.y = mtx[1][2];
    radius.z = mtx[2][2];
    for(i=0; i<8; i++) {
        angle = i*45;
        dir.x = HuSin(angle);
        dir.y = HuCos(angle);
        dir.z = 0.0f;
        VECNormalize(&dir, &dir);
        MTXMultVec(mtx, &dir, &dir);
        VECScale(&dir, &modelHitEffParam.vel, 10);
        effectNo = EffectCreate(EFFECT_STAR, cameraBit, pos->x, pos->y, pos->z, 30, &modelHitEffParam);
        if(effectNo == -1) {
            break;
        }
        VECScale(&radius, &vel, -3-(0.1*frandmod(20)));
        VECScale(&dir, &dir, 4);
        VECAdd(&dir, &vel, &hitGlowEffParam.vel);
        effectNo = EffectCreate(EFFECT_HIT, cameraBit, pos->x, pos->y, pos->z, 20, &hitGlowEffParam);
        if(effectNo == -1) {
            break;
        }
    }
}

static EFFECTPARAM shoeHitEffParam = {
    0,
    { 32, 32, 255, 255 },
    { 128, 255, 32, 255 },
    { 0, 2, 1 },
    { 0.95f, 0.95f, 0.95f },
    0,
    0,
    -0.2f,
    -16,
    0.06f
};

static EFFECTPARAM shoeHitGlowEffParam = {
    0,
    { 32, 224, 32, 255 },
    { 32, 224, 32, 255 },
    { 0, 2, 1 },
    { 1.0f, 1.0f, 1.0f },
    0,
    0,
    -0.2f,
    -12,
    0.05f
};

void CharModelShoeHitCreate(s16 charNo)
{
    CHARWORK *workP = &CharWork[charNo];
    HU3D_MODEL *modelP = &Hu3DData[workP->modelId];
    Mtx mtx;
    HuVecF pos;
    HuVecF radius;

    Hu3DModelObjMtxGet(workP->modelId, "test11_tex_we-ske_R_shoe1", mtx);
    pos.x = mtx[0][3];
    pos.y = mtx[1][3];
    pos.z = mtx[2][3];
    mtxRot(mtx, modelP->rot.x, modelP->rot.y, modelP->rot.z);
    radius.x = mtx[0][2];
    radius.y = mtx[1][2];
    radius.z = mtx[2][2]; 
    VECScale(&radius, &radius, 30);
    VECAdd(&pos, &radius, &pos);
    CharEffectShoeHitCreate(modelP->cameraBit, &pos, &modelP->rot);
}

void CharEffectShoeHitCreate(s16 cameraBit, HuVecF *pos, HuVecF *rot)
{
    Mtx mtx;
    HuVecF radius;
    HuVecF dir;
    HuVecF vel;
    s16 i;
    s16 effectNo;
    mtxRot(mtx, rot->x, rot->y, rot->z);
    radius.x = mtx[0][2];
    radius.y = mtx[1][2];
    radius.z = mtx[2][2]; 
    for(i=0; i<8; i++) {
        float angle = i*45;
        VECNormalize(&radius, &radius);
        dir.x = ((radius.x*radius.y)*(1-HuCos(angle)))-(radius.z*HuSin(angle));
        dir.y = (radius.y*radius.y)+((1-(radius.y*radius.y))*HuCos(angle));
        dir.z = ((radius.y*radius.z)*(1-HuCos(angle)))+(radius.x*HuSin(angle));
        VECNormalize(&dir, &dir);
        VECScale(&dir, &shoeHitEffParam.vel, 10);
        effectNo = EffectCreate(EFFECT_STAR, cameraBit, pos->x, pos->y, pos->z, 20, &shoeHitEffParam);
        if(effectNo == -1) {
            break;
        }
        VECScale(&radius, &vel, -2-(0.1*frandmod(20)));
        VECScale(&dir, &dir, 2);
        VECAdd(&dir, &vel, &shoeHitGlowEffParam.vel);
        effectNo = EffectCreate(EFFECT_HIT, cameraBit, pos->x, pos->y, pos->z, 20, &shoeHitGlowEffParam);
        if(effectNo == -1) {
            break;
        }
    }
}

void CharEffectLayerSet(s16 layerNo)
{
    s16 i, j;
    for(i=0; i<EFFECT_MAX; i++) {
        for(j=0; j<HU3D_CAM_MAX; j++) {
            if(effModelId[i][j] != HU3D_MODELID_NONE) {
                Hu3DModelLayerSet(effModelId[i][j], layerNo);
            }
        }
    }
    effectLayer = layerNo;
}

void CharMotionVoiceOnSet(s16 charNo, s16 motNo, BOOL voiceOn)
{
    CHARWORK *workP = &CharWork[charNo];
    s16 i;
    if(workP->modelId == HU3D_MODELID_NONE) {
        return;
    }
    motNo = CHAR_MOTNO(motNo);
    for(i=0; i<CHAR_MOT_MAX; i++) {
        if(workP->motNoTbl[i] == motNo) {
            break;
        }
    }
    if(i == CHAR_MOT_MAX) {
        return;
    }
    if(!voiceOn) {
        workP->voiceFlag[i] |= 0x1;
    } else {
        workP->voiceFlag[i] &= ~0x1;
    }
}

void CharModelVoicePanAutoSet(s16 charNo, BOOL voicePanAuto)
{
    CHARWORK *workP = &CharWork[charNo];
    if(voicePanAuto) {
        workP->attr |= 0x8;
    } else {
        workP->attr &= ~0x8;
    }
}

void CharModelVoiceFlagSet(s16 charNo, BOOL fxFlag)
{
    CHARWORK *workP = &CharWork[charNo];
    if(charNo >= CHARNO_MAX) {
        if(!fxFlag) {
            dustFlags[charNo] |= 0x10;
        } else {
            dustFlags[charNo] &= ~0x10;
        }
        return;
    } else {
        if(!fxFlag) {
            workP->attr |= 0x10;
        } else {
            workP->attr &= ~0x10;
        }
    }
}


void CharMotionUpdateSet(s16 charNo, unsigned int dataNum, BOOL updateF)
{
    CHARWORK *workP = &CharWork[charNo];
    s16 i;
    if(charNo >= CHARNO_MAX) {
        return;
    }
    dataNum = FILENUM(dataNum);
    for(i=0; i<CHAR_MOT_MAX; i++) {
        if(workP->motNoTbl[i] == dataNum) {
            break;
        }
    }
    if(i == CHAR_MOT_MAX) {
        return;
    }
    if(updateF) {
        workP->voiceFlag[i] &= ~0x2;
    } else {
        workP->voiceFlag[i] |= 0x2;
    }
}

typedef struct NpcDustWork_s {
    HU3D_MODELID modelId;
    HU3D_MOTIONID motId;
    s16 type;
    s16 npcNo;
} NPCDUSTWORK;

static void UpdateNpcDust(void);

s32 CharNpcDustSet(HU3D_MODELID modelId, HU3D_MOTIONID motId, s16 type, s16 npcNo)
{
    HUPROCESS *parent = HuPrcCurrentGet();
    HUPROCESS *process = HuPrcChildCreate(UpdateNpcDust, 100, 8192, 0, parent);
    if(!process) {
        return;
    } else {
        NPCDUSTWORK *work = HuMemDirectMallocNum(HEAP_HEAP, sizeof(NPCDUSTWORK), HU_MEMNUM_OVL);
        process->property = work;
        work->modelId = modelId;
        work->motId = motId;
        work->type = type;
        work->npcNo = npcNo;
        EffectInit();
    }
}

s32 CharNpcDustVoiceOffSet(HU3D_MODELID modelId, HU3D_MOTIONID motId, s16 type)
{
    s32 ret;
    CharNpcDustSet(modelId, motId, type, CHAR_NPC_NONE);
    return ret;
}

static s8 npcSeTimeTbl[12] = { 7, 32, 1, 30, 1, 30, 1, 20, 19, 32, 2, 19 };

static u16 npcSeTbl[12] = { 177, 177, 177, 177, 177, 177, 288, 2590, 286, 273, 59367, 59367 };

static u16 npcSeTbl2[7] = { 181, 181, 181, 181, 181, 181, 0 };

static s8 npcSeTimeTbl2[8] = { 3, 55, -25, -25, -25, -25, -25, -25 };

static u16 npcSeTbl3[4] = { 177, 177, 177, 177 };

static s8 npcSeTimeTbl3[8] = { 4, 17, -25, -25, -25, -25, -25, -25 };

static u16 npcSeTbl4[4] = { 181, 181, 181, 181 };

static s8 npcSeTimeTbl4[8] = { 1, 23, -25, -25, -25, -25, -25, -25 };

static u16 npcSeTbl5[4] = { 177, 177, 177, 177 };

static void UpdateNpcDust(void)
{
    NPCDUSTWORK *work = HuPrcCurrentGet()->property;
    HU3D_MODELID modelId = work->modelId;
    s16 time = 0;
    HU3D_MODEL *modelP = &Hu3DData[work->modelId];
    s16 npcNo = work->npcNo-CHAR_NPC_NO_BASE;
    Vec pos;
    s16 i;
    while(1) {
        HuPrcVSleep();
        if(Hu3DMotionIDGet(modelId) != work->motId) {
            continue;
        }
        if(modelP->attr & HU3D_ATTR_DISPOFF) {
            continue;
        }
        time = Hu3DMotionTimeGet(modelId);
        switch(work->type) {
            case 0:
                if((time & 0xF) == 0) {
                    if(!(dustFlags[npcNo] & 0x10)) {
                        dustEffParam.vel.x = -HuSin(modelP->rot.y)*2;
                        dustEffParam.vel.y = 1+(0.1*frandmod(10));
                        dustEffParam.vel.z = -HuCos(modelP->rot.y)*2;
                        pos.x = modelP->pos.x+(frandmod(50)-25);
                        pos.y = modelP->pos.y;
                        pos.z = modelP->pos.z+(frandmod(50)-25);
                        EffectDustCreate(modelId, pos.x, pos.y, pos.z, frandmod(10)+30, &dustEffParam);
                    }
                }
                if(work->npcNo != CHAR_NPC_NONE) {
                    for(i=0; i<2; i++) {
                        if(npcSeTimeTbl[(npcNo*2)+i] == (time*2)) {
                            HuAudFXPlay(npcSeTbl[npcNo]);
                            break;
                        }
                    }
                }
                break;
            
            case 1:
                if((time & 0x3) == 0) {
                    if(!(dustFlags[npcNo] & 0x10)) {
                        dustEffParam.vel.x = -HuSin(modelP->rot.y)*4;
                        dustEffParam.vel.y = 2+(0.1*frandmod(10));
                        dustEffParam.vel.z = -HuCos(modelP->rot.y)*4;
                        pos.x = modelP->pos.x+(frandmod(50)-25);
                        pos.y = modelP->pos.y;
                        pos.z = modelP->pos.z+(frandmod(50)-25);
                        EffectDustCreate(modelId, pos.x, pos.y, pos.z, frandmod(10)+30, &dustEffParam);
                    }
                }
                if(work->npcNo != CHAR_NPC_NONE) {
                    for(i=0; i<2; i++) {
                        if(npcSeTimeTbl[(npcNo*2)+i] == (time*2)) {
                            HuAudFXPlay(npcSeTbl2[npcNo]);
                            break;
                        }
                    }
                }
                break;
            
            case 2:
                if(npcNo == CHAR_NPC_NONE) {
                    continue;
                }
                if((time & 0x1F) == 0) {
                    if(!(dustFlags[npcNo] & 0x10)) {
                        dustEffParam.vel.x = -HuSin(modelP->rot.y)*2;
                        dustEffParam.vel.y = 1+(0.1*frandmod(10));
                        dustEffParam.vel.z = -HuCos(modelP->rot.y)*2;
                        pos.x = modelP->pos.x+(frandmod(50)-25);
                        pos.y = modelP->pos.y;
                        pos.z = modelP->pos.z+(frandmod(50)-25);
                        EffectDustCreate(modelId, pos.x, pos.y, pos.z, frandmod(10)+30, &dustEffParam);
                    }
                }
                for(i=0; i<2; i++) {
                    if(npcSeTimeTbl2[(npcNo*2)+i] == (time*2)) {
                        HuAudFXPlay(npcSeTbl3[npcNo]);
                        break;
                    }
                }
                break;
             
            case 3:
                if(npcNo == CHAR_NPC_NONE) {
                    continue;
                }
                if((time & 0x3) == 0) {
                    if(!(dustFlags[npcNo] & 0x10)) {
                        dustEffParam.vel.x = -HuSin(modelP->rot.y)*2;
                        dustEffParam.vel.y = 1+(0.1*frandmod(10));
                        dustEffParam.vel.z = -HuCos(modelP->rot.y)*2;
                        pos.x = modelP->pos.x+(frandmod(50)-25);
                        pos.y = modelP->pos.y;
                        pos.z = modelP->pos.z+(frandmod(50)-25);
                        EffectDustCreate(modelId, pos.x, pos.y, pos.z, frandmod(10)+30, &dustEffParam);
                    }
                }
                for(i=0; i<2; i++) {
                    if(npcSeTimeTbl3[(npcNo*2)+i] == (time*2)) {
                        HuAudFXPlay(npcSeTbl4[npcNo]);
                        break;
                    }
                }
                break;
             
            case 4:
                if(npcNo == CHAR_NPC_NONE) {
                    continue;
                }
                if((time & 0x7) == 0) {
                    if(!(dustFlags[npcNo] & 0x10)) {
                        dustEffParam.vel.x = -HuSin(modelP->rot.y)*2;
                        dustEffParam.vel.y = 1+(0.1*frandmod(10));
                        dustEffParam.vel.z = -HuCos(modelP->rot.y)*2;
                        pos.x = modelP->pos.x+(frandmod(50)-25);
                        pos.y = modelP->pos.y;
                        pos.z = modelP->pos.z+(frandmod(50)-25);
                        EffectDustCreate(modelId, pos.x, pos.y, pos.z, frandmod(10)+30, &dustEffParam);
                    }
                }
                for(i=0; i<2; i++) {
                    if(npcSeTimeTbl4[(npcNo*2)+i] == (time*2)) {
                        HuAudFXPlay(npcSeTbl5[npcNo]);
                        break;
                    }
                }
                break;
             
            case 5:
                if(time != 0) {
                    continue;
                }
                if(!(dustFlags[npcNo] & 0x10)) {
                    for(i=0; i<8; i++){ 
                        npcHitEffParam.vel.x = 10*HuSin(45.0f*i)*modelP->scale.x;
                        npcHitEffParam.vel.y = 0;
                        npcHitEffParam.vel.z = 10*HuCos(45.0f*i)*modelP->scale.x;
                        EffectStarCreate(modelId, modelP->pos.x, modelP->pos.y+(10*modelP->scale.x), modelP->pos.z, 40, &npcHitEffParam);
                    }
                    for(i=0; i<8; i++){ 
                        dustEffParam.vel.x = 4*HuSin((45.0f*i)+22.5)*modelP->scale.x;
                        dustEffParam.vel.y = 0;
                        dustEffParam.vel.z = 4*HuCos((45.0f*i)+22.5)*modelP->scale.x;
                        EffectDustCreate(modelId, modelP->pos.x, modelP->pos.y+(10*modelP->scale.x), modelP->pos.z, 20, &dustEffParam);
                    }
                    if(npcNo != CHAR_NPC_NONE) {
                        HuAudFXPlay(195);
                    }
                }
                break;
        }
    }
}

void CharModelStepSet(s16 charNo, s16 stepFx)
{
    CHARWORK *workP = &CharWork[charNo];
    workP->stepFx = stepFx;
}

static s16 PlayStepVoice(s16 charNo, s16 seId, u8 voiceFlag)
{
    CHARWORK *workP = &CharWork[charNo];
    HU3D_MODEL *modelP = &Hu3DData[workP->modelId];
    s16 ret;
    if(voiceFlag & 0x1) {
        return;
    }
    if(workP->stepFx == 4) {
        seId = 0x8541;
    } else if(workP->stepFx == 5) {
        seId = 0x8549;
    } else if(workP->stepFx == 6) {
        seId = 0xCA;
    } else {
        seId += workP->stepFx;
    }
    if(!(seId & 0x8000)) {
        if(workP->attr & 0x8) {
            return CharFXPlayPos(charNo, seId, &modelP->pos);
        } else {
            _CharFXPlay(charNo, seId, voiceFlag);
            return ret;
        }
    } else {
        seId &= 0x7FFF;
        if(voiceFlag & 0x1) {
            return -1;
        }
        if(workP->attr & 0x8) {
            return HuAudFXEmiterPlay(seId, &modelP->pos);
        } else {
            return HuAudFXPlay(seId);
        }
    }
}

static EFFECTPARAM landEffParam = {
    0,
    { 255, 255, 255, 255 },
    { 255, 255, 255, 255 },
    { 0, 2, 1 },
    { 0.85f, 0.85f, 0.85f },
    0.2f,
    0,
    1,
    -8,
    0.02f
};

static EFFECTPARAM hipDropLandEffParam = {
    0,
    { 255, 255, 255, 255 },
    { 255, 255, 255, 255 },
    { 0, 2, 1 },
    { 0.9f, 0.9f, 0.9f },
    0.2f,
    0,
    1,
    -8,
    0.02f
};

static EFFECTPARAM hipDropStarEffParam = {
    0,
    { 255, 255, 0, 255 },
    { 255, 255, 0, 255 },
    { 0, 0, 0 },
    { 0.9f, 0.9f, 0.9f },
    -0.3f,
    0,
    -0.5f,
    -10,
    0
};

void CharModelLandDustCreate(s16 charNo, HuVecF *pos)
{
    CHARWORK *workP = &CharWork[charNo];
    HU3D_MODEL *modelP = &Hu3DData[workP->modelId];
    s16 i;
    for(i=0; i<12; i++) {
        float speed = (6.0+frandmod(6))*modelP->scale.x;
        Vec effectPos;
        landEffParam.vel.x = speed*HuSin(30.0f*i);
        landEffParam.vel.y = 0;
        landEffParam.vel.z = speed*HuCos(30.0f*i);
        landEffParam.alphaBase = -(frandmod(4)+8);
        speed = (20.0+frandmod(20))*modelP->scale.x;
        effectPos.x = pos->x+(20*HuSin(30.0f*i)*modelP->scale.x);
        effectPos.y = pos->y+(10*modelP->scale.x);
        effectPos.z = pos->z+(20*HuCos(30.0f*i)*modelP->scale.x);
        landEffParam.gravity = frandmod(20)*0.01;
        EffectCreate(EFFECT_LANDDUST, modelP->cameraBit, effectPos.x, effectPos.y, effectPos.z, speed, &landEffParam);
    }
}

void CharModelLandDustCreateStep(s16 charNo, HuVecF *pos)
{
    CHARWORK *workP = &CharWork[charNo];
    HU3D_MODEL *modelP = &Hu3DData[workP->modelId];
    s16 i;
    for(i=0; i<12; i++) {
        float speed = modelP->scale.x*(6.0+frandmod(6));
        Vec effectPos;
        landEffParam.vel.x = speed*HuSin(30.0f*i);
        landEffParam.vel.y = 0;
        landEffParam.vel.z = speed*HuCos(30.0f*i);
        landEffParam.alphaBase = -(frandmod(4)+8);
        speed = modelP->scale.x*(20.0+frandmod(20));
        effectPos.x = pos->x+(modelP->scale.x*(20.0*HuSin(30.0f*i)));
        effectPos.y = pos->y+(10*modelP->scale.x);
        effectPos.z = pos->z+(modelP->scale.x*(20.0*HuCos(30.0f*i)));
        landEffParam.gravity = 0.01*frandmod(20);
        EffectCreate(EFFECT_LANDDUST, modelP->cameraBit, effectPos.x, effectPos.y, effectPos.z, speed, &landEffParam);
    }
    PlayStepVoice(charNo, CHARSEID(8), 0);
}

void CharEffectHipDropCreate(s16 charNo, HuVecF *pos)
{
    CHARWORK *workP = &CharWork[charNo];
    HU3D_MODEL *modelP = &Hu3DData[workP->modelId];
    s16 i;
    for(i=0; i<12; i++) {
        float speed = (6.0+frandmod(6))*modelP->scale.x;
        Vec effectPos;
        hipDropLandEffParam.vel.x = speed*HuSin(30.0f*i);
        hipDropLandEffParam.vel.y = 0;
        hipDropLandEffParam.vel.z = speed*HuCos(30.0f*i);
        hipDropLandEffParam.alphaBase = -(frandmod(4)+8);
        speed = (30.0+frandmod(20))*modelP->scale.x;
        effectPos.x = pos->x+(20*HuSin(30.0f*i)*modelP->scale.x);
        effectPos.y = pos->y+(15*modelP->scale.x);
        effectPos.z = pos->z+(20*HuCos(30.0f*i)*modelP->scale.x);
        hipDropLandEffParam.gravity = frandmod(20)*0.01;
        EffectCreate(EFFECT_LANDDUST, modelP->cameraBit, effectPos.x, effectPos.y, effectPos.z, speed, &hipDropLandEffParam);
    }
    for(i=0; i<8; i++) {
        hipDropStarEffParam.vel.x = (20*HuSin(45.0f*i)*modelP->scale.x);
        hipDropStarEffParam.vel.y = 5;
        hipDropStarEffParam.vel.z = (20*HuCos(45.0f*i)*modelP->scale.x);
        EffectStarCreate(workP->modelId, modelP->pos.x, modelP->pos.y+(10*modelP->scale.x), modelP->pos.z, 40, &hipDropStarEffParam);
    }
}

void CharEffectDustCreate(s16 cameraBit, float scale, HuVecF *pos)
{
    s16 i;
    for(i=0; i<8; i++) {
        dustEffParam.vel.x = scale*(4*HuSin(i*45.0f));
        dustEffParam.vel.y = 0;
        dustEffParam.vel.z = scale*(4*HuCos(i*45.0f));
        EffectCreate(EFFECT_DUST, cameraBit, pos->x, pos->y*scale, pos->z, 20*scale, &dustEffParam);
    }
    for(i=0; i<8; i++) {
        dustEffParam.vel.x = scale*(2*HuSin((i*45.0f)+22.5));
        dustEffParam.vel.y = 0;
        dustEffParam.vel.z = scale*(2*HuCos((i*45.0f)+22.5));
        EffectCreate(EFFECT_DUST, cameraBit, pos->x, pos->y*scale, pos->z, 20*scale, &dustEffParam);
    }
}

static EFFECTPARAM cryEffParam = {
    0,
    { 128, 255, 255, 255 },
    { 255, 255, 255, 255 },
    { 0, 2, 1 },
    { 0.85f, 0.85f, 0.85f },
    -0.5f,
    0,
    -0.1f,
    -16,
    0.02f
};


void CharEffectCryCreate(s16 cameraBit, HuVecF *pos, HuVecF *offset, float scale)
{
    float randZ;
    float randX;
    Mtx rotMtx;
    Vec effectPos;
    HuVecF dir;
    randX = frandmod(180)-90;
    randZ = frandmod(180)-90;
    VECNormalize(offset, &dir);
    mtxRot(rotMtx, frandmod(90)-45, frandmod(90)-45, 0);
    MTXMultVec(rotMtx, &dir, &dir);
    VECNormalize(&dir, &dir);
    cryEffParam.vel.x = 15*dir.x;
    cryEffParam.vel.y = 15*dir.y;
    cryEffParam.vel.z = 15*dir.z;
    effectPos.x = pos->x+(scale*(frandmod(30)-15));
    effectPos.y = pos->y+(scale*(frandmod(20)-10));
    effectPos.z = pos->z+(scale*(frandmod(30)-15));
    EffectCreate(EFFECT_CRY, cameraBit, effectPos.x, effectPos.y, effectPos.z, scale*8, &cryEffParam);
}

void CharModelCryCreate(s16 charNo, float yOfs, float ofsY)
{
    CHARWORK *workP = &CharWork[charNo];
    HU3D_MODEL *modelP = &Hu3DData[workP->modelId];
    Vec pos;
    Vec offset;
    Hu3DModelObjPosGet(workP->modelId, CharHeadObjNameTbl[charNo], &pos);
    if(charNo == CHARNO_WARIO) {
        pos.y = pos.y-(40.0f*modelP->scale.x);
    }
    pos.y += yOfs;
    offset.x = HuSin(modelP->rot.y+180.0f);
    offset.y = ofsY;
    offset.z = HuCos(modelP->rot.y+180.0f);
    CharEffectCryCreate(modelP->cameraBit, &pos, &offset, modelP->scale.x);
}

unsigned int CharModelFileNumGet(s16 charNo, s16 model)
{
    if(model & CHAR_MODEL0) {
        return CharDataDirTbl[charNo][0];
    } else if(model & CHAR_MODEL1) {
        return CharDataDirTbl[charNo][1];
    } else if(model & CHAR_MODEL2) {
        return CharDataDirTbl[charNo][2];
    } else {
        return CharDataDirTbl[charNo][3];
    }
}

unsigned int CharMotionFileNumGet(s16 charNo, u16 motId)
{
    return CharDataDirTbl[charNo][4]|motId;
}

void CharModelVoicePanSet(s16 charNo, s16 pan)
{
    CHARWORK *workP = &CharWork[charNo];
    workP->pan = pan;
}

void CharModelVoiceVolSet(s16 charNo, s16 vol)
{
    CHARWORK *workP = &CharWork[charNo];
    workP->vol = vol;
}


typedef struct WinLoseVoicePlay_s {
    s16 charNo;
    s16 seId;
    unsigned int motId;
} WINLOSEVOICEPLAY;

static void PlayWinLoseVoice(void);

void CharWinLoseVoicePlay(s16 charNo, unsigned int motId, s16 seId)
{
    HUPROCESS *parent = HuPrcCurrentGet();
    HUPROCESS *process = HuPrcChildCreate(PlayWinLoseVoice, 100, 6144, 0, parent);
    if(!process) {
        OSReport("Error: CharWinLoseVoicePlay Failure.\n");
        return;
    } else {
        WINLOSEVOICEPLAY *winLose = HuMemDirectMallocNum(HEAP_HEAP, sizeof(WINLOSEVOICEPLAY), HU_MEMNUM_OVL);
        process->property = winLose;
        winLose->charNo = charNo;
        winLose->seId = seId;
        winLose->motId = motId;
    }
}

static void PlayWinLoseVoice(void)
{
    HUPROCESS *process = HuPrcCurrentGet();
    WINLOSEVOICEPLAY *winLose = process->property;
    CHARWORK *workP = &CharWork[winLose->charNo];
    s16 i;
    s16 motNo;
    
    if(workP->modelId == HU3D_MODELID_NONE) {
        HuMemDirectFree(winLose);
        HuPrcEnd();
    }
    for(i=0; i<CHAR_MOT_MAX; i++) {
        if(workP->motId[i] == winLose->motId) {
            motNo = workP->motNoTbl[i];
            break;
        }
    }
    switch(motNo) {
        case CHAR_MOTNO(CHARMOT_HSF_c000m1_306):
            if(omcurovl >= 0x7B && omcurovl <= 0x7B) {
                HuPrcSleep(boardWinAnimLen[winLose->charNo]);
            } else {
                HuPrcSleep(winAnimLen[winLose->charNo]);
            }
            break;
       
       case CHAR_MOTNO(CHARMOT_HSF_c000m1_346):
            HuPrcSleep(handUpAnimLen[winLose->charNo]);
            break;
       
       case CHAR_MOTNO(CHARMOT_HSF_c000m1_349):
            HuPrcSleep(okAnimLen[winLose->charNo]);
            break;
       
       case CHAR_MOTNO(CHARMOT_HSF_c000m1_350):
            HuPrcSleep(winAnimLen2[winLose->charNo]);
            break;
    }
    _CharFXPlay(winLose->charNo, winLose->seId, 0);
    HuMemDirectFree(process->property);
    HuPrcEnd();
    while(1) {
        HuPrcVSleep();
    }
}

void CharLoseVoicePlay(s16 charNo1, s16 charNo2, s16 charNo3, s16 charNo4)
{
    if(charNo1 != CHARNO_NONE) {
        _CharFXPlay(charNo1, CHARVOICEID(12), 0);
    }
    if(charNo2 != CHARNO_NONE) {
        _CharFXPlay(charNo2, CHARVOICEID(12), 0);
    }
    if(charNo3 != CHARNO_NONE) {
        _CharFXPlay(charNo3, CHARVOICEID(12), 0);
    }
    if(charNo4 != CHARNO_NONE) {
        _CharFXPlay(charNo4, CHARVOICEID(12), 0);
    }
}

s16 CharMotionTotalTimeGet(s16 charNo, int motNo)
{
    switch(motNo) {
        case CHAR_MOTNO(CHARMOT_HSF_c000m1_306):
            return winAnimLen[charNo];
        
        case CHAR_MOTNO(CHARMOT_HSF_c000m1_346):
            return handUpAnimLen[charNo];
        
        case CHAR_MOTNO(CHARMOT_HSF_c000m1_349):
            return okAnimLen[charNo];
        
        case CHAR_MOTNO(CHARMOT_HSF_c000m1_350):
            return winAnimLen2[charNo];
        
        case CHAR_MOTNO(CHARMOT_HSF_c000m1_363):
            return holdUpAnimLen[charNo];
        
        case CHAR_MOTNO(CHARMOT_HSF_c000m1_364):
            return winJumpUpAnimLen[charNo];
        
        case CHAR_MOTNO(CHARMOT_HSF_c000m1_352):
            return loseAnimLen[charNo];
        
        default:
            return 0;
    }
}

s16 CharMotionExtraTimeGet(s16 charNo, int motNo)
{
    return 0;
}

HU3D_LIGHTID CharLightCreateV(HuVecF *pos, HuVecF *dir, GXColor *color)
{
    CHARWORK *workP = CharWork;
    s16 i;
    s16 j;
    HU3D_LIGHTID lightId = HU3D_LIGHTID_NONE;
    for(i=0; i<CHARNO_MAX; i++, workP++) {
        if(workP->modelId == HU3D_MODELID_NONE) {
            continue;
        }
        if(lightId == HU3D_LIGHTID_NONE) {
            lightId = Hu3DLLightCreateV(workP->modelId, pos, dir, color);
            if(lightId == HU3D_LIGHTID_NONE) {
                return -1;
            }
        } else {
            HU3D_MODEL *modelP = &Hu3DData[workP->modelId];
            for(j=0; j<HU3D_MODEL_LLIGHT_MAX; j++) {
                if(modelP->LLightId[j] == HU3D_LIGHTID_NONE) {
                    modelP->LLightId[j] = lightId;
                    modelP->attr |= 0x1000;
                    break;
                }
            }
        }
    }
    return lightId;
}

inline HU3D_LIGHTID CharLightCreateV(HuVecF *pos, HuVecF *dir, GXColor *color);

HU3D_LIGHTID CharLightCreate(float posX, float posY, float posZ, float dirX, float dirY, float dirZ, u8 r, u8 g, u8 b)
{
    HuVecF pos;
    HuVecF dir;
    GXColor color;
    pos.x = posX; pos.y = posY; pos.z = posZ;
    dir.x = dirX; dir.y = dirY; dir.z = dirZ;
    color.r = r; color.g = g; color.b = b; color.a = 255;
    return CharLightCreateV(&pos, &dir, &color);
}

static inline HU3D_LIGHT *GetCharLight(void)
{
    CHARWORK *workP = CharWork;
    s16 i;
    s16 j;
    for(i=0; i<CHARNO_MAX; i++, workP++) {
        if(workP->modelId == HU3D_MODELID_NONE) {
            continue;
        }
        {
            HU3D_MODEL *modelP = &Hu3DData[workP->modelId];
            for(j=0; j<HU3D_MODEL_LLIGHT_MAX; j++) {
                if(modelP->LLightId[j] != HU3D_LIGHTID_NONE) {
                    return &Hu3DLocalLight[modelP->LLightId[j]];
                }
            }
        }
    }
    return NULL;
}

void CharLightSpotSet(s32 func, float cutoff)
{
    HU3D_LIGHT *lightP = GetCharLight();
    if(lightP) {
        lightP->type &= 0xFF00;
        lightP->cutoff = cutoff;
        lightP->func = func;
    }
}

void CharLightInfinitytSet(void)
{
    HU3D_LIGHT *lightP = GetCharLight();
    if(lightP) {
        lightP->type &= 0xFF00;
        lightP->type |= HU3D_LIGHT_TYPE_INFINITYT;
    }
}

void CharLightPointSet(s32 func, float cutoff, float brightness)
{
    HU3D_LIGHT *lightP = GetCharLight();
    if(lightP) {
        lightP->type &= 0xFF00;
        lightP->type |= HU3D_LIGHT_TYPE_POINT;
        lightP->cutoff = cutoff;
        lightP->brightness = brightness;
        lightP->func = func;
    }
}

void CharLightColorSet(u8 r, u8 g, u8 b, u8 a)
{
    HU3D_LIGHT *lightP = GetCharLight();
    if(lightP) {
        lightP->color.r = r;
        lightP->color.g = g;
        lightP->color.b = b;
        lightP->color.a = a;
    }
}

void CharLightPosAimSetV(HuVecF *pos, HuVecF *aim)
{
    HU3D_LIGHT *lightP = GetCharLight();
    if(lightP) {
        lightP->pos = *pos;
        PSVECNormalize(aim, &lightP->dir);
    }
}

void CharLightPosAimSet(float posX, float posY, float posZ, float aimX, float aimY, float aimZ)
{
    HU3D_LIGHT *lightP = GetCharLight();
    if(lightP) {
        lightP->pos.x = posX;
        lightP->pos.y = posY;
        lightP->pos.z = posZ;
        lightP->dir.x = aimX;
        lightP->dir.y = aimY;
        lightP->dir.z = aimZ;
        PSVECNormalize(&lightP->dir, &lightP->dir);
    }
}

void CharLightStaticSet(BOOL staticF)
{
    HU3D_LIGHT *lightP = GetCharLight();
    if(lightP) {
        if(staticF) {
            lightP->type |= HU3D_LIGHT_TYPE_STATIC;
        } else {
            lightP->type &= ~HU3D_LIGHT_TYPE_STATIC;
        }
    }
}

void CharEffectWarnCreate(s16 charNo, float scale)
{
    CHARWORK *workP = &CharWork[charNo];
    HU3D_MODEL *modelP = &Hu3DData[workP->modelId];
    EffectWarnCreate(workP->modelId, modelP->pos.x, 100.0f+modelP->pos.y, modelP->pos.z, 20, scale, &warnEffParam);
}

u32 CharAttrGet(s16 charNo)
{
    CHARWORK *workP = &CharWork[charNo];
    return workP->attr;
}
