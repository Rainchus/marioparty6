#include "game/mgdata.h"

#include "game/object.h"
#include "messnum/mg_name.h"
#include "messnum/mg_inst.h"

#include "datanum/instpic.h"

#include "mgdata.inc"

#define DECA_SCORE_TIME 0
#define DECA_SCORE_POINT 1
#define DECA_SCORE_NUM 11


int MgModeSubMode;
s16 MgModeScore[GW_PLAYER_MAX];
s16 MgGameNo;
u8 MgModePlayNum;
u8 MgBattleOrder[GW_PLAYER_MAX];
int lbl_802C0438;
int MgSubMode;
int MBookEvtNo;
BOOL MgExitReq;
BOOL MgPauseExitF;
BOOL MgInstExitF;
BOOL MgBoardE3F;

u8 ATTRIBUTE_ALIGN(4) MgModeWork[256];
GW_COMMON GwCommonBackup;
int MgModeWorkInt[16];
float MgModeWorkFloat[16];

int MgNoGet(s16 ovlNo)
{
    MG_DATA *mgData;
    s16 i;
    for(mgData=&MgDataTbl[0], i=0; mgData->ovl != (u16)DLL_NONE; mgData++, i++) {
        if(mgData->ovl == ovlNo) {
            return i;
        }
    }
    return -1;
}

int MgSubModeGet(void)
{
    return MgSubMode;
}

typedef struct MgDecaScore_s {
    s32 mgNo;
    s16 type;
    s16 pointNum;
    HuVec2f points[10];
} MGDECASCORE;

static MGDECASCORE MgDecaScoreTbl[DECA_SCORE_NUM] = {
    {
        502,
        DECA_SCORE_TIME,
        5,
        {
            { 0, 90 },
            { 100, 66 },
            { 550, 42 },
            { 800, 31 },
            { 1000, 27 }
        }
    },
    {
        504,
        DECA_SCORE_TIME,
        5,
        {
            { 0, 90 },
            { 100, 72 },
            { 450, 42 },
            { 800, 23 },
            { 1000, 15 }
        }
    },
    {
        563,
        DECA_SCORE_POINT,
        7,
        {
            { 0, 0 },
            { 100, 8 },
            { 350, 18 },
            { 700, 28 },
            { 800, 31 },
            { 950, 39 },
            { 1000, 44 }
        }
    },
    {
        506,
        DECA_SCORE_POINT,
        6,
        {
            { 0, 0 },
            { 100, 60 },
            { 400, 100 },
            { 650, 123 },
            { 850, 140 },
            { 1000, 150 },
        }
    },
    {
        507,
        DECA_SCORE_TIME,
        5,
        {
            { 0, 30 },
            { 150, 20 },
            { 400, 10 },
            { 700, 3 },
            { 1000, 0.1 },
        }
    },
    {
        510,
        DECA_SCORE_POINT,
        7,
        {
            { 0, 0 },
            { 50, 5 },
            { 150, 10 },
            { 300, 15 },
            { 500, 20 },
            { 700, 25 },
            { 1000, 30 },
        }
    },
    {
        511,
        DECA_SCORE_TIME,
        7,
        {
            { 0, 90 },
            { 100, 75 },
            { 250, 65 },
            { 500, 55 },
            { 700, 50 },
            { 850, 45 },
            { 1000, 37 },
        }
    },
    {
        513,
        DECA_SCORE_TIME,
        7,
        {
            { 0, 60 },
            { 50, 25 },
            { 300, 17 },
            { 550, 13 },
            { 750, 11 },
            { 950, 8 },
            { 1000, 6 },
        }
    },
    {
        512,
        DECA_SCORE_TIME,
        7,
        {
            { 0, 90 },
            { 200, 48 },
            { 400, 38 },
            { 500, 33 },
            { 650, 30 },
            { 900, 28 },
            { 1000, 27 },
        }
    },
    {
        514,
        DECA_SCORE_POINT,
        7,
        {
            { 0, 0 },
            { 0, 12.32 },
            { 150, 20 },
            { 600, 30 },
            { 900, 40 },
            { 980, 45 },
            { 1000, 48 },
        }
    },
    {
        5141,
        DECA_SCORE_POINT,
        7,
        {
            { 0, 0 },
            { 0, 36.97109 },
            { 150, 60.018 },
            { 600, 90.027 },
            { 900, 120.036 },
            { 980, 135.0405 },
            { 1000, 144.0432 },
        }
    },
};

int MgDecaScoreCalc(int gameNo, int mgScore)
{
    MGDECASCORE *scoreP;
    HuVec2f *point;
    HuVec2f *pointNext;
    int pointNo;
    s32 mgNo;
    float value;
    int result;
    result = 1000;
    OSReport("%d\n", mgScore);
    mgNo = -1;
    switch(gameNo) {
        case 0:
            if(mgScore == 0) {
                result = 0;
            } else {
                value = mgScore/50.0f;
                mgNo = 502;
            }
            break;
        
        case 1:
            if(mgScore == 0) {
                result = 0;
            } else {
                value = mgScore/50.0f;
                mgNo = 504;
            }
            break;
       
       case 2:
            value = mgScore;
            mgNo = 563;
            break;
       
       case 3:
            value = mgScore;
            mgNo = 506;
            break;
       
       case 4:
            if(mgScore == 0) {
                result = 0;
            } else {
                value = mgScore/1000.0f;
                mgNo = 507;
            }
            break;
       
       case 5:
            value = mgScore;
            mgNo = 510;
            break;
       
       case 6:
            if(mgScore == 0) {
                result = 0;
            } else {
                value = mgScore/50.0f;
                mgNo = 511;
            }
            break;
       
       case 8:
            if(mgScore == 0) {
                result = 0;
            } else {
                value = mgScore/50.0f;
                mgNo = 512;
            }
            break;
       
       case 7:
            if(mgScore == 0) {
                result = 0;
            } else {
                value = mgScore/50.0f;
                mgNo = 513;
            }
            break;
       
       case 9:
            value = mgScore/100.0f;
            mgNo = 514;
            break;
    }
    if(mgNo > 0) {
        
        int i;
        for(scoreP=&MgDecaScoreTbl[0], i=DECA_SCORE_NUM; i--; scoreP++) {
            if(scoreP->mgNo == mgNo) {
                break;
            }
        }
        point = NULL;
        pointNext = &scoreP->points[0];
        result = 1000;
        if(scoreP->type != DECA_SCORE_TIME) {
            for(pointNo=0; pointNo<scoreP->pointNum; point=pointNext, pointNext++, pointNo++) {
                if(value <= pointNext->y) {
                    if(!point) {
                        result = 0;
                        break;
                    } else {
                        result = point->x;
                        result += (pointNext->x-point->x)*((value-point->y)/(pointNext->y-point->y));
                        break;
                    }
                }
                
            }
        } else {
            for(pointNo=0; pointNo<scoreP->pointNum; point=pointNext, pointNext++, pointNo++) {
                if(value >= pointNext->y) {
                    if(!point) {
                        result = 0;
                        break;
                    } else {
                        result = point->x;
                        result += (pointNext->x-point->x)*((point->y-value)/(point->y-pointNext->y));
                        break;
                    }
                }
            }
        }
    }
    if(result < 0) {
        result = 0;
    } else if(result > 1000) {
        result = 1000;
    }
    return result;
}

int lbl_802BF860 = 0xFFD815FF;
int lbl_802BF864 = 0xFFFFFFFF;
int lbl_802BF868 = 0x42FF7AFF;