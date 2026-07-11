/* Block math.h's extern inline sqrtf: its weak _half/_three statics would
 * prepend 16 bytes to .sdata2 that the original mggamemes.o does not have. */
#define _MATH_H
#define M_PI 3.141592653589793
double sin(double);
double cos(double);

#include "game/gamemes.h"
#include "game/audio.h"
#include "game/gamework.h"
#include "game/sprite.h"

#define MSM_SE_CMN_26 0x19
#define MSM_SE_CMN_27 0x1A
#define MSM_SE_CMN_28 0x1B
#define MSM_SE_CMN_29 0x1C

#define sind(x) sin((M_PI * (x)) / 180.0)
#define cosd(x) cos((M_PI * (x)) / 180.0)

static float WinnerMesNameY[4];
static float WinnerMesNameX[4];
static int WinnerMesCharNo[4];
static int WinnerMesSprNum[5];
static s16 WinnerMesSprX[6];
static float DrawMesY[15][6];
static int DrawMesTime[15][6];
static float SdMesTPLvl[6][15];
static float SdMesSprPosY[15];
static float SdMesSprPosX[15];
static int MesUnk1[15];
static int SdMesScaleTime[15];
static float SdMesScale[15];
static float SprRareZRot[15];
static float SprOrbitAngle[15][2];
static float SprMainAngle[15][2];
static float SprPosY[15][2];
static float SprPosX[15][2];

static int WinnerMesXOfs;
static float GameMesScale;
static float GameMesTPLvl;
static int MgMesTime;
static int GameMesLetterNum;
static int SdMesWork[2];
static int RecordMesValue;
static int RecordMesLanguageNo;
static int WinnerMesLanguageNo;
static int WinnerMesCharNum;

static char *WinnerMesNameTbl[16*6] = {
    "\xCF\xD8\xB5", "MARIO", "MARIO", "MARIO", "MARIO", "MARIO",
    "\xD9\xB2\xB0\xBC\xDE", "LUIGI", "LUIGI", "LUIGI", "LUIGI", "LUIGI",
    "\xCB\xDF\xB0\xC1", "PEACH", "PEACH", "PEACH", "PEACH", "PEACH",
    "\xD6\xAF\xBC\xB0", "YOSHI", "YOSHI", "YOSHI", "YOSHI", "YOSHI",
    "\xDC\xD8\xB5", "WARIO", "WARIO", "WARIO", "WARIO", "WARIO",
    "\xC3\xDE\xB2\xBC\xDE\xB0", "DAISY", "DAISY", "DAISY", "DAISY", "DAISY",
    "\xDC\xD9\xB2\xB0\xBC\xDE", "WALUIGI", "WALUIGI", "WALUIGI", "WALUIGI", "WALUIGI",
    "\xB7\xC9\xCB\xDF\xB5", "TOAD", "TOAD", "TOAD", "TOAD", "TOAD",
    "\xC3\xDA\xBB", "BOO", "BUU HUU", "BOO", "BOO", "BOO",
    "\xD0\xC6\xB8\xAF\xCA\xDF", "KOOPA KID", "MINI BOWSER", "MINI BOWSER", "MINI BOWSER", "MINI BOWSER",
    "\xB7\xC9\xCB\xDF\xBA", "TOADETTE", "TOADETTE", "TOADETTE", "TOADETTE", "TOADETTE",
    "\xD0\xC6\xB8\xAF\xCA\xDFR", "RED K.KID", "M.BOWSER R", "MINI B.ROUGE", "MINI B.ROSSO", "MINI B.ROJO",
    "\xD0\xC6\xB8\xAF\xCA\xDFG", "GREEN K.KID", "M.BOWSER G", "MINI B.VERT", "MINI B.VERDE", "MINI B.VERDE",
    "\xD0\xC6\xB8\xAF\xCA\xDF" "B", "BLUE K.KID", "M.BOWSER B", "MINI B.BLEU", "MINI B.BLU", "MINI B.AZUL",
    "\xC4\xDE\xDD\xB7\xB0", "DK", "DONKEY KONG", "DK", "DK", "DK",
    "\xB8\xAF\xCA\xDF", "BOWSER\0", "BOWSER\0", "BOWSER\0", "BOWSER\0", "BOWSER\0"
};

static int WinnerMesGrpInfo[96] = {
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0x30, 0xFF, 0xFF, 0xFF,
    0xFF, 0x50, 0x40, 0x40, 0x40, 0x40,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0x30, 0x80, 0x40, 0x40, 0x40,
    0xFF, 0x50, 0x80, 0x40, 0x40, 0x40,
    0xFF, 0x40, 0x80, 0x40, 0x40, 0x40,
    0xFF, 0xFF, 0x60, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};

static s16 WinnerMesOfsTbl[2][4][5][2] = {
    {
        {{0, -35}, {0, 35}},
        {{0, -35}, {-144, 35}, {144, 35}},
        {{0, -105}, {0, 105}, {-144, 0}, {144, 0}},
        {{0, -105}, {-144, 0}, {144, 0}, {-144, 105}, {144, 105}}
    },
    {
        {{144, 0}, {-144, 0}},
        {{0, 35}, {-144, -35}, {144, -35}},
        {{0, 105}, {0, -105}, {-144, 0}, {144, 0}},
        {{0, 105}, {-144, -105}, {144, -105}, {-144, 0}, {144, 0}}
    }
};

static void FixWinnerNameOfs(int grpId, int charNo, float scale, int sprNum);
static float GetGameMesWidth(GAMEMES *mes, int type, int no, float scale);

BOOL GameMesMg4PInit(GAMEMES *mes, va_list args)
{
    s16 j;
    u32 mesId;
    GAMEMESID strMes;
    float x;
    float y;

    mes->subMode = va_arg(args, int);
    if(mes->subMode == GAMEMES_MG_TYPE_DRAW) {
        GameMesMgDrawInit(mes, args);
        return TRUE;
    }
    mes->mesMode = 1;
    mes->angle = 0;
    mesId = (mes->subMode == GAMEMES_MG_TYPE_START) ? 0x42000A : 0x42000B;
    strMes = GameMesStrWinCreate(mes, mesId);
    for(j=0; j<mes->charNum; j++) {
        x = GetGameMesWidth(mes, 0, j, 1.0f);
        y = mes->pos.y;
        SprPosX[j][0] = SprPosX[j][1] = 56.0f;
        SprPosY[j][0] = SprPosY[j][1] = 56.0f + mes->pos.y;
        if(mes->subMode == GAMEMES_MG_TYPE_START) {
            x -= SprPosX[j][0];
            y -= SprPosY[j][0];
            HuSprScaleSet(mes->grpId[0], j, mes->scale.x, mes->scale.y);
        } else {
            HuSprScaleSet(mes->grpId[0], j, 0.0f, 0.0f);
        }
        HuSprPosSet(mes->grpId[0], j, x, y);
        HuSprPriSet(mes->grpId[0], j, 5);
        SprMainAngle[j][0] = SprMainAngle[j][1] = 0.0f;
    }
    GameMesLetterNum = 1;
    MgMesTime = 0;
    GameMesPauseEnable(FALSE);
    return TRUE;
}

BOOL GameMesMg4PExec(GAMEMES *mes)
{
    s16 j;
    float y;
    float x;

    mes->time += GameMesVWait;
    if(mes->time >= mes->timeMax && mes->mesMode == 0) {
        mes->stat |= GAMEMES_STAT_TIMEEND;
        mes->mesMode = -1;
        mes->angle = 0;
    }
    switch(mes->mesMode) {
        case 1:
            if(mes->subMode == GAMEMES_MG_TYPE_START) {
                if(mes->time <= 60) {
                    if(mes->time > 0 && mes->time % 8 == 0) {
                        if(++GameMesLetterNum > mes->charNum) {
                            GameMesLetterNum = mes->charNum;
                        }
                    }
                    for(j=0; j<GameMesLetterNum; j++) {
                        x = GetGameMesWidth(mes, 0, j, 1.0f);
                        y = mes->pos.y;
                        if(mes->time <= 10.0f + (j * 8)) {
                            SprMainAngle[j][0] += 9.0f;
                            if(SprMainAngle[j][0] >= 90.0f) {
                                SprMainAngle[j][0] = 90.0f;
                            }
                            x -= SprPosX[j][0];
                            SprPosY[j][0] = (168.0f + SprPosY[j][1]) * sind(90.0f - SprMainAngle[j][0]);
                            y += 168.0f;
                            y -= SprPosY[j][0];
                        } else {
                            SprMainAngle[j][1] += 9.0f;
                            if(SprMainAngle[j][1] >= 90.0f) {
                                SprMainAngle[j][1] = 90.0f;
                            }
                            SprPosX[j][0] = SprPosX[j][1] * sind(90.0f - SprMainAngle[j][1]);
                            x -= SprPosX[j][0];
                            SprPosY[j][0] = 168.0 * (1.0 - sind(SprMainAngle[j][1]));
                            y += SprPosY[j][0];
                        }
                        HuSprPosSet(mes->grpId[0], j, x, y);
                    }
                }
                if(mes->time >= 60) {
                    GameMesScale = (sind((mes->time-60)*9.0f)*0.5)+1.0;
                    for(j=0; j<mes->charNum; j++) {
                        x = GetGameMesWidth(mes, 0, j, GameMesScale);
                        y = mes->pos.y;
                        HuSprPosSet(mes->grpId[0], j, x, y);
                        HuSprScaleSet(mes->grpId[0], j, mes->scale.x*GameMesScale, mes->scale.y*GameMesScale);
                    }
                }
                if(mes->time == 60) {
                    HuAudFXPlay(MSM_SE_CMN_26);
                    mes->stat |= GAMEMES_STAT_FXPLAY;
                }
                if(mes->time == 80) {
                    mes->stat |= GAMEMES_STAT_TIMEEND;
                    GameMesEnd();
                }
                if(mes->time >= 80) {
                    mes->mesMode = 0;
                    mes->timeMax = 110;
                }
            } else {
                if(mes->time <= 20) {
                    GameMesScale = sind(mes->time*4.5f);
                    for(j=0; j<mes->charNum; j++) {
                        x = GetGameMesWidth(mes, 2, j, GameMesScale);
                        y = mes->pos.y;
                        HuSprPosSet(mes->grpId[0], j, x, y);
                        HuSprScaleSet(mes->grpId[0], j, mes->scale.x*GameMesScale, mes->scale.y*GameMesScale);
                    }
                }
                if(mes->time == 20) {
                    if(mes->subMode == GAMEMES_MG_TYPE_FINISH) {
                        HuAudFXPlay(MSM_SE_CMN_27);
                    } else {
                        HuAudFXPlay(MSM_SE_CMN_29);
                    }
                    mes->stat |= GAMEMES_STAT_FXPLAY;
                }
                if(mes->time >= 50) {
                    if(MgMesTime > 0 && MgMesTime % 8 == 0) {
                        if(++GameMesLetterNum > mes->charNum) {
                            GameMesLetterNum = mes->charNum;
                        }
                    }
                    for(j=0; j<GameMesLetterNum; j++) {
                        x = GetGameMesWidth(mes, 2, j, 1.0f);
                        y = mes->pos.y;
                        if(MgMesTime <= 10.0f + (j * 8)) {
                            SprMainAngle[j][0] += 9.0f;
                            if(SprMainAngle[j][0] >= 90.0f) {
                                SprMainAngle[j][0] = 90.0f;
                            }
                            SprPosX[j][0] = SprPosX[j][1] * sind(SprMainAngle[j][0]);
                            x -= SprPosX[j][0];
                            SprPosY[j][0] = 168.0 * sind(90.0f + SprMainAngle[j][0]);
                            y += 168.0f;
                            y -= SprPosY[j][0];
                        } else {
                            x -= SprPosX[j][0];
                            SprMainAngle[j][1] += 9.0f;
                            if(SprMainAngle[j][1] >= 90.0f) {
                                SprMainAngle[j][1] = 90.0f;
                            }
                            SprPosY[j][0] = (168.0f + SprPosY[j][1]) * sind(SprMainAngle[j][1]);
                            y += 168.0f;
                            y -= SprPosY[j][0];
                        }
                        HuSprPosSet(mes->grpId[0], j, x, y);
                    }
                    MgMesTime++;
                }
                if(mes->time >= 120) {
                    mes->mesMode = 0;
                    mes->stat |= GAMEMES_STAT_KILL;
                }
            }
            break;

        case -1:
            mes->angle += 0.1f*GameMesVWait;
            GameMesTPLvl = 1.0f-mes->angle;
            if(GameMesTPLvl <= 0) {
                GameMesTPLvl = 0;
                mes->mesMode = 0;
                mes->stat |= GAMEMES_STAT_KILL;
            }
            for(j=0; j<mes->charNum; j++) {
                HuSprTPLvlSet(mes->grpId[0], j, GameMesTPLvl);
            }
            break;

        default:
            break;
    }
    if(GameMesCloseF || (mes->stat & GAMEMES_STAT_KILL)) {
        GameMesSprKill(mes);
        if(mes->subMode == GAMEMES_MG_TYPE_START) {
            GameMesPauseEnable(TRUE);
        }
        return FALSE;
    }
    return TRUE;
}

BOOL GameMesMg1Vs3Init(GAMEMES *mes, va_list args)
{
    s16 j;
    u32 mesId;
    GAMEMESID strMes;
    float x;
    float y;

    mes->subMode = va_arg(args, int);
    if(mes->subMode == GAMEMES_MG_TYPE_DRAW) {
        GameMesMgDrawInit(mes, args);
        return TRUE;
    }
    mes->mesMode = 1;
    mes->angle = 0;
    mesId = (mes->subMode == GAMEMES_MG_TYPE_START) ? 0x42000A : 0x42000B;
    strMes = GameMesStrWinCreate(mes, mesId);
    for(j=0; j<mes->charNum; j++) {
        x = GetGameMesWidth(mes, 0, j, 1.0f);
        y = mes->pos.y;
        if(j == 0) {
            SprPosX[j][0] = SprPosX[j][1] = 56.0f + GetGameMesWidth(mes, 0, 0, 1.0f);
        } else {
            SprPosX[j][0] = SprPosX[j][1] = 56.0f + (2.0f*mes->pos.x - GetGameMesWidth(mes, 0, 1, 1.0f));
        }
        SprPosY[j][0] = SprPosY[j][1] = 56.0f + mes->pos.y;
        if(mes->subMode == GAMEMES_MG_TYPE_START) {
            if(j == 0) {
                x -= SprPosX[j][0];
                y += SprPosY[j][0];
            } else {
                x += SprPosX[j][0];
                y -= SprPosY[j][0];
            }
            HuSprScaleSet(mes->grpId[0], j, mes->scale.x, mes->scale.y);
        } else {
            HuSprScaleSet(mes->grpId[0], j, 0.0f, 0.0f);
        }
        HuSprPosSet(mes->grpId[0], j, x, y);
        HuSprPriSet(mes->grpId[0], j, 5);
        SprMainAngle[j][0] = SprMainAngle[j][1] = 0.0f;
    }
    GameMesPauseEnable(FALSE);
    return TRUE;
}

BOOL GameMesMg1Vs3Exec(GAMEMES *mes)
{
    s16 j;
    float x;
    float y;

    mes->time += GameMesVWait;
    if(mes->time >= mes->timeMax && mes->mesMode == 0) {
        mes->stat |= GAMEMES_STAT_TIMEEND;
        mes->mesMode = -1;
        mes->angle = 0;
    }
    switch(mes->mesMode) {
        case 1:
            if(mes->subMode == GAMEMES_MG_TYPE_START) {
                if(mes->time <= 60) {
                    for(j=0; j<mes->charNum; j++) {
                        SprMainAngle[j][0] += 2.25f;
                        if(SprMainAngle[j][0] >= 90.0f) {
                            SprMainAngle[j][0] = 90.0f;
                        }
                        x = GetGameMesWidth(mes, 0, j, 1.0f);
                        y = mes->pos.y;
                        if(j == 0) {
                            SprPosX[j][0] = SprPosX[j][1] * (1.0 - sind(SprMainAngle[j][0]));
                            x -= SprPosX[j][0];
                            SprPosY[j][0] = SprPosY[j][1] * sind(90.0f - SprMainAngle[j][0]);
                            y += SprPosY[j][0];
                        } else {
                            SprPosX[j][0] = SprPosX[j][1] * (1.0 - sind(SprMainAngle[j][0]));
                            x += SprPosX[j][0];
                            SprPosY[j][0] = SprPosY[j][1] * sind(90.0f - SprMainAngle[j][0]);
                            y -= SprPosY[j][0];
                        }
                        HuSprPosSet(mes->grpId[0], j, x, y);
                    }
                }
                if(mes->time >= 60) {
                    GameMesScale = (sind((mes->time-60)*9.0f)*0.5)+1.0;
                    for(j=0; j<mes->charNum; j++) {
                        x = GetGameMesWidth(mes, 0, j, GameMesScale);
                        y = mes->pos.y;
                        HuSprPosSet(mes->grpId[0], j, x, y);
                        HuSprScaleSet(mes->grpId[0], j, mes->scale.x*GameMesScale, mes->scale.y*GameMesScale);
                    }
                }
                if(mes->time == 60) {
                    HuAudFXPlay(MSM_SE_CMN_26);
                    mes->stat |= GAMEMES_STAT_FXPLAY;
                }
                if(mes->time == 80) {
                    mes->stat |= GAMEMES_STAT_TIMEEND;
                    GameMesEnd();
                }
                if(mes->time >= 80) {
                    mes->mesMode = 0;
                    mes->timeMax = 110;
                }
            } else {
                if(mes->time <= 20) {
                    GameMesScale = sind(mes->time*4.5f);
                    for(j=0; j<mes->charNum; j++) {
                        x = GetGameMesWidth(mes, 2, j, GameMesScale);
                        y = mes->pos.y;
                        HuSprPosSet(mes->grpId[0], j, x, y);
                        HuSprScaleSet(mes->grpId[0], j, mes->scale.x*GameMesScale, mes->scale.y*GameMesScale);
                    }
                }
                if(mes->time == 20) {
                    if(mes->subMode == GAMEMES_MG_TYPE_FINISH) {
                        HuAudFXPlay(MSM_SE_CMN_27);
                    } else {
                        HuAudFXPlay(MSM_SE_CMN_29);
                    }
                    mes->stat |= GAMEMES_STAT_FXPLAY;
                }
                if(mes->time >= 50) {
                    for(j=0; j<mes->charNum; j++) {
                        SprMainAngle[j][0] += 2.25f;
                        if(SprMainAngle[j][0] >= 90.0f) {
                            SprMainAngle[j][0] = 90.0f;
                        }
                        x = GetGameMesWidth(mes, 2, j, 1.0f);
                        y = mes->pos.y;
                        if(j == 0) {
                            SprPosX[j][0] = SprPosX[j][1] * sind(90.0f - SprMainAngle[j][0]);
                            x -= SprPosX[j][1];
                            x += SprPosX[j][0];
                            SprPosY[j][0] = SprPosY[j][1] * (1.0 - sind(SprMainAngle[j][0]));
                            y += SprPosY[j][1];
                            y -= SprPosY[j][0];
                        } else {
                            SprPosX[j][0] = SprPosX[j][1] * sind(90.0f - SprMainAngle[j][0]);
                            x += SprPosX[j][1];
                            x -= SprPosX[j][0];
                            SprPosY[j][0] = SprPosY[j][1] * (1.0 - sind(SprMainAngle[j][0]));
                            y -= SprPosY[j][1];
                            y += SprPosY[j][0];
                        }
                        HuSprPosSet(mes->grpId[0], j, x, y);
                    }
                }
                if(mes->time >= 120) {
                    mes->mesMode = 0;
                    mes->stat |= GAMEMES_STAT_KILL;
                }
            }
            break;

        case -1:
            mes->angle += 0.1f*GameMesVWait;
            GameMesTPLvl = 1.0f-mes->angle;
            if(GameMesTPLvl <= 0) {
                GameMesTPLvl = 0;
                mes->mesMode = 0;
                mes->stat |= GAMEMES_STAT_KILL;
            }
            for(j=0; j<mes->charNum; j++) {
                HuSprTPLvlSet(mes->grpId[0], j, GameMesTPLvl);
            }
            break;

        default:
            break;
    }
    if(GameMesCloseF || (mes->stat & GAMEMES_STAT_KILL)) {
        GameMesSprKill(mes);
        if(mes->subMode == GAMEMES_MG_TYPE_START) {
            GameMesPauseEnable(TRUE);
        }
        return FALSE;
    }
    return TRUE;
}

BOOL GameMesMg2Vs2Init(GAMEMES *mes, va_list args)
{
    s16 j;
    s16 center;
    u32 mesId;
    GAMEMESID strMes;
    float x;
    float y;

    mes->subMode = va_arg(args, int);
    if(mes->subMode == GAMEMES_MG_TYPE_DRAW) {
        GameMesMgDrawInit(mes, args);
        return TRUE;
    }
    mes->mesMode = 1;
    mes->angle = 0;
    mesId = (mes->subMode == GAMEMES_MG_TYPE_START) ? 0x42000A : 0x42000B;
    strMes = GameMesStrWinCreate(mes, mesId);
    for(j=0; j<mes->charNum; j++) {
        x = GetGameMesWidth(mes, 0, j, 1.0f);
        y = mes->pos.y;
        center = mes->charNum / 2;
        if(j < center) {
            SprPosX[j][0] = SprPosX[j][1] = 56.0f + GetGameMesWidth(mes, 0, center-1, 1.0f);
        } else if(j > center) {
            SprPosX[j][0] = SprPosX[j][1] = 56.0f + (2.0f*mes->pos.x - GetGameMesWidth(mes, 0, center+1, 1.0f));
        }
        SprPosY[j][0] = SprPosY[j][1] = 56.0f + mes->pos.y;
        if(mes->subMode == GAMEMES_MG_TYPE_START) {
            if(j == center) {
                GameMesScale = 0.0f;
            } else {
                if(j < center) {
                    x += SprPosX[j][0];
                    y -= SprPosY[j][0];
                } else {
                    x -= SprPosX[j][0];
                    y += SprPosY[j][0];
                }
                GameMesScale = 1.0f;
            }
            HuSprScaleSet(mes->grpId[0], j, mes->scale.x, mes->scale.y);
        } else {
            GameMesScale = 0.0f;
            HuSprScaleSet(mes->grpId[0], j, GameMesScale, GameMesScale);
        }
        HuSprPosSet(mes->grpId[0], j, x, y);
        HuSprPriSet(mes->grpId[0], j, 5);
        SprMainAngle[j][0] = SprMainAngle[j][1] = 0.0f;
    }
    MgMesTime = 0;
    GameMesPauseEnable(FALSE);
    return TRUE;
}

BOOL GameMesMg2Vs2Exec(GAMEMES *mes)
{
    s16 j;
    s16 center;
    float x;
    float y;

    mes->time += GameMesVWait;
    if(mes->time >= mes->timeMax && mes->mesMode == 0) {
        mes->stat |= GAMEMES_STAT_TIMEEND;
        mes->mesMode = -1;
        mes->angle = 0;
    }
    switch(mes->mesMode) {
        case 1:
            if(mes->subMode == GAMEMES_MG_TYPE_START) {
                if(mes->time <= 60) {
                    for(j=0; j<mes->charNum; j++) {
                        SprMainAngle[j][0] += 2.25f;
                        if(SprMainAngle[j][0] >= 90.0f) {
                            SprMainAngle[j][0] = 90.0f;
                        }
                        x = GetGameMesWidth(mes, 0, j, 1.0f);
                        y = mes->pos.y;
                        center = mes->charNum / 2;
                        if(j == center) {
                            if(mes->time < 20) {
                                GameMesScale = 0.0f;
                            } else {
                                GameMesScale = sind(MgMesTime*4.5f);
                                if(MgMesTime > 20) {
                                    GameMesScale = 1.0f;
                                }
                            }
                            HuSprScaleSet(mes->grpId[0], j, mes->scale.x*GameMesScale, mes->scale.y*GameMesScale);
                        } else if(j < center) {
                            SprPosX[j][0] = SprPosX[j][1] * (1.0 - sind(SprMainAngle[j][0]));
                            x -= SprPosX[j][0];
                            SprPosY[j][0] = SprPosY[j][1] * sind(90.0f-SprMainAngle[j][0]);
                            y -= SprPosY[j][0];
                        } else {
                            SprPosX[j][0] = SprPosX[j][1] * (1.0 - sind(SprMainAngle[j][0]));
                            x += SprPosX[j][0];
                            SprPosY[j][0] = SprPosY[j][1] * sind(90.0f-SprMainAngle[j][0]);
                            y += SprPosY[j][0];
                        }
                        HuSprPosSet(mes->grpId[0], j, x, y);
                    }
                    if(mes->time >= 20) {
                        MgMesTime++;
                    }
                }
                if(mes->time >= 60) {
                    GameMesScale = (sind((mes->time-60)*9.0f)*0.5)+1.0;
                    for(j=0; j<mes->charNum; j++) {
                        x = GetGameMesWidth(mes, 0, j, GameMesScale);
                        y = mes->pos.y;
                        HuSprPosSet(mes->grpId[0], j, x, y);
                        HuSprScaleSet(mes->grpId[0], j, mes->scale.x*GameMesScale, mes->scale.y*GameMesScale);
                    }
                }
                if(mes->time == 60) {
                    HuAudFXPlay(MSM_SE_CMN_26);
                    mes->stat |= GAMEMES_STAT_FXPLAY;
                }
                if(mes->time == 80) {
                    mes->stat |= GAMEMES_STAT_TIMEEND;
                    GameMesEnd();
                }
                if(mes->time >= 80) {
                    mes->mesMode = 0;
                    mes->timeMax = 110;
                }
            } else {
                if(mes->time <= 20) {
                    GameMesScale = sind(mes->time*4.5f);
                    for(j=0; j<mes->charNum; j++) {
                        x = GetGameMesWidth(mes, 2, j, GameMesScale);
                        y = mes->pos.y;
                        HuSprPosSet(mes->grpId[0], j, x, y);
                        HuSprScaleSet(mes->grpId[0], j, mes->scale.x*GameMesScale, mes->scale.y*GameMesScale);
                    }
                }
                if(mes->time == 20) {
                    if(mes->subMode == GAMEMES_MG_TYPE_FINISH) {
                        HuAudFXPlay(MSM_SE_CMN_27);
                    } else {
                        HuAudFXPlay(MSM_SE_CMN_29);
                    }
                    mes->stat |= GAMEMES_STAT_FXPLAY;
                }
                if(mes->time >= 50) {
                    for(j=0; j<mes->charNum; j++) {
                        SprMainAngle[j][0] += 2.25f;
                        if(SprMainAngle[j][0] >= 90.0f) {
                            SprMainAngle[j][0] = 90.0f;
                        }
                        x = GetGameMesWidth(mes, 2, j, 1.0f);
                        y = mes->pos.y;
                        center = mes->charNum / 2;
                        if(j == center) {
                            GameMesScale = (sind(MgMesTime*4.5f)*3.0)+1.0;
                            if(MgMesTime > 20) {
                                GameMesScale = 1.0f;
                            }
                            HuSprScaleSet(mes->grpId[0], j, mes->scale.x*GameMesScale, mes->scale.y*GameMesScale);
                            GameMesTPLvl = sind(MgMesTime*4.5f);
                            if(MgMesTime > 20) {
                                GameMesTPLvl = 1.0f;
                            }
                            HuSprTPLvlSet(mes->grpId[0], j, 1.0f-GameMesTPLvl);
                        } else if(j < center) {
                            SprPosX[j][0] = SprPosX[j][1] * sind(90.0f-SprMainAngle[j][0]);
                            x -= SprPosX[j][1];
                            x += SprPosX[j][0];
                            SprPosY[j][0] = SprPosY[j][1] * (1.0-sind(SprMainAngle[j][0]));
                            y -= SprPosY[j][1];
                            y += SprPosY[j][0];
                        } else {
                            SprPosX[j][0] = SprPosX[j][1] * sind(90.0f-SprMainAngle[j][0]);
                            x += SprPosX[j][1];
                            x -= SprPosX[j][0];
                            SprPosY[j][0] = SprPosY[j][1] * (1.0-sind(SprMainAngle[j][0]));
                            y += SprPosY[j][1];
                            y -= SprPosY[j][0];
                        }
                        HuSprPosSet(mes->grpId[0], j, x, y);
                    }
                    MgMesTime++;
                }
                if(mes->time >= 120) {
                    mes->mesMode = 0;
                    mes->stat |= GAMEMES_STAT_KILL;
                }
            }
            break;

        case -1:
            mes->angle += 0.1f*GameMesVWait;
            GameMesTPLvl = 1.0f-mes->angle;
            if(GameMesTPLvl <= 0) {
                GameMesTPLvl = 0;
                mes->mesMode = 0;
                mes->stat |= GAMEMES_STAT_KILL;
            }
            for(j=0; j<mes->charNum; j++) {
                HuSprTPLvlSet(mes->grpId[0], j, GameMesTPLvl);
            }
            break;

        default:
            break;
    }
    if(GameMesCloseF || (mes->stat & GAMEMES_STAT_KILL)) {
        GameMesSprKill(mes);
        if(mes->subMode == GAMEMES_MG_TYPE_START) {
            GameMesPauseEnable(TRUE);
        }
        return FALSE;
    }
    return TRUE;
}

BOOL GameMesMgBattleInit(GAMEMES *mes, va_list args)
{
    s16 j;
    u32 mesId;
    GAMEMESID strMes;
    float x;
    float y;

    mes->subMode = va_arg(args, int);
    if(mes->subMode == GAMEMES_MG_TYPE_DRAW) {
        GameMesMgDrawInit(mes, args);
        return TRUE;
    }
    mes->mesMode = 1;
    mes->angle = 0;
    mesId = (mes->subMode == GAMEMES_MG_TYPE_START) ? 0x42000A : 0x42000B;
    strMes = GameMesStrWinCreate(mes, mesId);
    for(j=0; j<mes->charNum; j++) {
        if(mes->subMode == GAMEMES_MG_TYPE_START) {
            x = mes->pos.x;
            y = -112.0f;
            HuSprPosSet(mes->grpId[0], j, x, y);
            HuSprPriSet(mes->grpId[0], j, 5);
            HuSprScaleSet(mes->grpId[0], j, mes->scale.x, mes->scale.y);
        } else {
            x = GetGameMesWidth(mes, 0, j, 1.0f);
            y = mes->pos.y;
            GameMesScale = 0.0f;
            HuSprScaleSet(mes->grpId[0], j, GameMesScale, GameMesScale);
        }
        SprOrbitAngle[j][0] = SprOrbitAngle[j][1] = 0.0f;
        SprMainAngle[j][0] = SprMainAngle[j][1] = 0.0f;
    }
    MgMesTime = 0;
    GameMesPauseEnable(FALSE);
    return TRUE;
}

BOOL GameMesMgBattleExec(GAMEMES *mes)
{
    s16 j;
    s16 center;
    float x;
    float y;
    float angle;
    float radius;

    mes->time += GameMesVWait;
    if(mes->time >= mes->timeMax && mes->mesMode == 0) {
        mes->stat |= GAMEMES_STAT_TIMEEND;
        mes->mesMode = -1;
        mes->angle = 0;
    }
    switch(mes->mesMode) {
        case 1:
            if(mes->subMode == GAMEMES_MG_TYPE_START) {
                if(mes->time <= 60) {
                    for(j=0; j<mes->charNum; j++) {
                        if(mes->time < 30) {
                            x = mes->pos.x;
                            y = mes->pos.y;
                            SprMainAngle[j][0] += 3.6f;
                            if(SprMainAngle[j][0] >= 90.0f) {
                                SprMainAngle[j][0] = 90.0f;
                            }
                            radius = (112.0f+mes->pos.x)*(1.0f-sind(SprMainAngle[j][0]));
                            angle = (360.0f/mes->charNum)*(j-2);
                            SprOrbitAngle[j][0] += 5.0f;
                            angle += SprOrbitAngle[j][0];
                            x += radius*cosd(angle);
                            y += radius*sind(angle);
                        } else if(mes->time < 55) {
                            x = mes->pos.x;
                            y = mes->pos.y;
                            center = mes->charNum / 2;
                            SprOrbitAngle[j][1] += (GetGameMesWidth(mes, 0, center, 1.0f)-GetGameMesWidth(mes, 0, 0, 1.0f))/15.0f;
                            if(j < center) {
                                x -= SprOrbitAngle[j][1];
                                if(x < GetGameMesWidth(mes, 0, j, 1.0f)) {
                                    x = GetGameMesWidth(mes, 0, j, 1.0f);
                                }
                            } else {
                                x += SprOrbitAngle[j][1];
                                if(x > GetGameMesWidth(mes, 0, j, 1.0f)) {
                                    x = GetGameMesWidth(mes, 0, j, 1.0f);
                                }
                            }
                        } else {
                            x = GetGameMesWidth(mes, 0, j, 1.0f);
                            y = mes->pos.y;
                        }
                        HuSprPosSet(mes->grpId[0], j, x, y);
                    }
                }
                if(mes->time >= 60) {
                    GameMesScale = (sind((mes->time-60)*9.0f)*0.5)+1.0;
                    for(j=0; j<mes->charNum; j++) {
                        x = GetGameMesWidth(mes, 0, j, GameMesScale);
                        y = mes->pos.y;
                        HuSprPosSet(mes->grpId[0], j, x, y);
                        HuSprScaleSet(mes->grpId[0], j, mes->scale.x*GameMesScale, mes->scale.y*GameMesScale);
                    }
                }
                if(mes->time == 60) {
                    HuAudFXPlay(MSM_SE_CMN_26);
                    mes->stat |= GAMEMES_STAT_FXPLAY;
                }
                if(mes->time == 80) {
                    mes->stat |= GAMEMES_STAT_TIMEEND;
                    GameMesEnd();
                }
                if(mes->time >= 80) {
                    mes->mesMode = 0;
                    mes->timeMax = 110;
                }
            } else {
                if(mes->time <= 20) {
                    GameMesScale = sind(mes->time*4.5f);
                    for(j=0; j<mes->charNum; j++) {
                        x = GetGameMesWidth(mes, 2, j, GameMesScale);
                        y = mes->pos.y;
                        HuSprPosSet(mes->grpId[0], j, x, y);
                        HuSprScaleSet(mes->grpId[0], j, mes->scale.x*GameMesScale, mes->scale.y*GameMesScale);
                    }
                }
                if(mes->time == 20) {
                    if(mes->subMode == GAMEMES_MG_TYPE_FINISH) {
                        HuAudFXPlay(MSM_SE_CMN_27);
                    } else {
                        HuAudFXPlay(MSM_SE_CMN_29);
                    }
                    mes->stat |= GAMEMES_STAT_FXPLAY;
                }
                if(mes->time >= 50) {
                    for(j=0; j<mes->charNum; j++) {
                        if(MgMesTime < 20) {
                            x = GetGameMesWidth(mes, 2, j, 1.0f);
                            y = mes->pos.y;
                            center = mes->charNum / 2;
                            SprOrbitAngle[j][1] += (GetGameMesWidth(mes, 2, center, 1.0f)-GetGameMesWidth(mes, 2, 0, 1.0f))/15.0f;
                            if(j < center) {
                                x += SprOrbitAngle[j][1];
                                if(x > mes->pos.x) {
                                    x = mes->pos.x;
                                }
                            } else {
                                x -= SprOrbitAngle[j][1];
                                if(x < mes->pos.x) {
                                    x = mes->pos.x;
                                }
                            }
                        } else {
                            x = mes->pos.x;
                            y = mes->pos.y;
                            SprMainAngle[j][0] += 4.5f;
                            if(SprMainAngle[j][0] >= 90.0f) {
                                SprMainAngle[j][0] = 90.0f;
                            }
                            radius = (112.0f+mes->pos.x)*(1.0f-sind(90.0f-SprMainAngle[j][0]));
                            angle = (360.0f/mes->charNum)*(j-2);
                            SprOrbitAngle[j][0] -= 5.0f;
                            angle += SprOrbitAngle[j][0];
                            x += radius*cosd(angle);
                            y += radius*sind(angle);
                        }
                        HuSprPosSet(mes->grpId[0], j, x, y);
                    }
                    MgMesTime++;
                }
                if(mes->time >= 120) {
                    mes->mesMode = 0;
                    mes->stat |= GAMEMES_STAT_KILL;
                }
            }
            break;

        case -1:
            mes->angle += 0.1f*GameMesVWait;
            GameMesTPLvl = 1.0f-mes->angle;
            if(GameMesTPLvl <= 0) {
                GameMesTPLvl = 0;
                mes->mesMode = 0;
                mes->stat |= GAMEMES_STAT_KILL;
            }
            for(j=0; j<mes->charNum; j++) {
                HuSprTPLvlSet(mes->grpId[0], j, GameMesTPLvl);
            }
            break;

        default:
            break;
    }
    if(GameMesCloseF || (mes->stat & GAMEMES_STAT_KILL)) {
        GameMesSprKill(mes);
        if(mes->subMode == GAMEMES_MG_TYPE_START) {
            GameMesPauseEnable(TRUE);
        }
        return FALSE;
    }
    return TRUE;
}

BOOL GameMesMgKoopaInit(GAMEMES *mes, va_list args)
{
    s16 j;
    u32 mesId;
    GAMEMESID strMes;
    float x;
    float y;

    mes->subMode = va_arg(args, int);
    if(mes->subMode == GAMEMES_MG_TYPE_DRAW) {
        GameMesMgDrawInit(mes, args);
        return TRUE;
    }
    mes->mesMode = 1;
    mes->angle = 0;
    mesId = (mes->subMode == GAMEMES_MG_TYPE_START) ? 0x42000A : 0x42000B;
    strMes = GameMesStrWinCreate(mes, mesId);
    for(j=0; j<mes->charNum; j++) {
        x = GetGameMesWidth(mes, 0, j, 1.0f);
        y = mes->pos.y;
        if(mes->subMode == GAMEMES_MG_TYPE_START) {
            SprOrbitAngle[j][0] = SprOrbitAngle[j][1] = 56.0f+(2.0f*mes->pos.x)-GetGameMesWidth(mes, 0, 0, 1.0f);
            x += SprOrbitAngle[j][0];
            HuSprScaleSet(mes->grpId[0], j, mes->scale.x, mes->scale.y);
        } else {
            SprOrbitAngle[j][0] = 0.0f;
            SprOrbitAngle[j][1] = 2.0f*mes->pos.x-GetGameMesWidth(mes, 0, 0, 1.0f);
            GameMesScale = 0.0f;
            HuSprScaleSet(mes->grpId[0], j, GameMesScale, GameMesScale);
        }
        HuSprPosSet(mes->grpId[0], j, x, y);
        HuSprPriSet(mes->grpId[0], j, 5);
    }
    GameMesPauseEnable(FALSE);
    return TRUE;
}

BOOL GameMesMgKoopaExec(GAMEMES *mes)
{
    s16 j;
    float x;
    float y;

    mes->time += GameMesVWait;
    if(mes->time >= mes->timeMax && mes->mesMode == 0) {
        mes->stat |= GAMEMES_STAT_TIMEEND;
        mes->mesMode = -1;
        mes->angle = 0;
    }
    switch(mes->mesMode) {
        case 1:
            if(mes->subMode == GAMEMES_MG_TYPE_START) {
                if(mes->time <= 60) {
                    for(j=0; j<mes->charNum; j++) {
                        if(j == mes->charNum-1) {
                            if(mes->time >= 35) {
                                SprOrbitAngle[j][0] -= SprOrbitAngle[j][1]/20.0f;
                            }
                            x = GetGameMesWidth(mes, 0, j, 1.0f)+SprOrbitAngle[j][0];
                            if(x < GetGameMesWidth(mes, 0, j, 1.0f)) {
                                x = GetGameMesWidth(mes, 0, j, 1.0f);
                            }
                        } else {
                            if(mes->time >= 15) {
                                SprOrbitAngle[j][0] -= SprOrbitAngle[j][1]/20.0f;
                            }
                            x = GetGameMesWidth(mes, 0, j, 1.0f)+SprOrbitAngle[j][0];
                            if(x < GetGameMesWidth(mes, 0, j, 1.0f)) {
                                x = GetGameMesWidth(mes, 0, j, 1.0f);
                            }
                        }
                        y = mes->pos.y;
                        HuSprPosSet(mes->grpId[0], j, x, y);
                    }
                }
                if(mes->time >= 60) {
                    GameMesScale = (sind((mes->time-60)*9.0f)*0.5)+1.0;
                    for(j=0; j<mes->charNum; j++) {
                        x = GetGameMesWidth(mes, 0, j, GameMesScale);
                        y = mes->pos.y;
                        HuSprPosSet(mes->grpId[0], j, x, y);
                        HuSprScaleSet(mes->grpId[0], j, mes->scale.x*GameMesScale, mes->scale.y*GameMesScale);
                    }
                }
                if(mes->time == 60) {
                    HuAudFXPlay(MSM_SE_CMN_26);
                    mes->stat |= GAMEMES_STAT_FXPLAY;
                }
                if(mes->time == 80) {
                    mes->stat |= GAMEMES_STAT_TIMEEND;
                    GameMesEnd();
                }
                if(mes->time >= 80) {
                    mes->mesMode = 0;
                    mes->timeMax = 110;
                }
            } else {
                if(mes->time <= 20) {
                    GameMesScale = sind(mes->time*4.5f);
                    for(j=0; j<mes->charNum; j++) {
                        x = GetGameMesWidth(mes, 2, j, GameMesScale);
                        y = mes->pos.y;
                        HuSprPosSet(mes->grpId[0], j, x, y);
                        HuSprScaleSet(mes->grpId[0], j, mes->scale.x*GameMesScale, mes->scale.y*GameMesScale);
                    }
                }
                if(mes->time == 20) {
                    if(mes->subMode == GAMEMES_MG_TYPE_FINISH) {
                        HuAudFXPlay(MSM_SE_CMN_27);
                    } else {
                        HuAudFXPlay(MSM_SE_CMN_29);
                    }
                    mes->stat |= GAMEMES_STAT_FXPLAY;
                }
                if(mes->time >= 50) {
                    for(j=0; j<mes->charNum; j++) {
                        if(j == mes->charNum-1) {
                            if(mes->time >= 70) {
                                SprOrbitAngle[j][0] += SprOrbitAngle[j][1]/20.0f;
                            }
                        } else {
                            SprOrbitAngle[j][0] += SprOrbitAngle[j][1]/20.0f;
                        }
                        x = GetGameMesWidth(mes, 2, j, 1.0f)-SprOrbitAngle[j][0];
                        y = mes->pos.y;
                        HuSprPosSet(mes->grpId[0], j, x, y);
                    }
                }
                if(mes->time >= 120) {
                    mes->mesMode = 0;
                    mes->stat |= GAMEMES_STAT_KILL;
                }
            }
            break;

        case -1:
            mes->angle += 0.1f*GameMesVWait;
            GameMesTPLvl = 1.0f-mes->angle;
            if(GameMesTPLvl <= 0) {
                GameMesTPLvl = 0;
                mes->mesMode = 0;
                mes->stat |= GAMEMES_STAT_KILL;
            }
            for(j=0; j<mes->charNum; j++) {
                HuSprTPLvlSet(mes->grpId[0], j, GameMesTPLvl);
            }
            break;

        default:
            break;
    }
    if(GameMesCloseF || (mes->stat & GAMEMES_STAT_KILL)) {
        GameMesSprKill(mes);
        if(mes->subMode == GAMEMES_MG_TYPE_START) {
            GameMesPauseEnable(TRUE);
        }
        return FALSE;
    }
    return TRUE;
}

BOOL GameMesMgRareInit(GAMEMES *mes, va_list args)
{
    s16 j;
    u32 mesId;
    GAMEMESID strMes;
    float x;
    float y;

    mes->subMode = va_arg(args, int);
    if(mes->subMode == GAMEMES_MG_TYPE_DRAW) {
        GameMesMgDrawInit(mes, args);
        return TRUE;
    }
    mes->mesMode = 1;
    mes->angle = 0;
    mesId = (mes->subMode == GAMEMES_MG_TYPE_START) ? 0x42000A : 0x42000B;
    strMes = GameMesStrWinCreate(mes, mesId);
    for(j=0; j<mes->charNum; j++) {
        x = GetGameMesWidth(mes, 0, j, 1.0f);
        y = mes->pos.y;
        if(mes->subMode == GAMEMES_MG_TYPE_START) {
            SprOrbitAngle[j][0] = SprOrbitAngle[j][1] = 56.0f+(2.0f*mes->pos.x)-GetGameMesWidth(mes, 0, 0, 1.0f);
            x += SprOrbitAngle[j][0];
            HuSprScaleSet(mes->grpId[0], j, mes->scale.x, mes->scale.y);
            SprRareZRot[j] = 600.0f;
            HuSprZRotSet(mes->grpId[0], j, SprRareZRot[j]);
        } else {
            SprOrbitAngle[j][0] = 0.0f;
            SprOrbitAngle[j][1] = 2.0f*mes->pos.x-GetGameMesWidth(mes, 0, 0, 1.0f);
            GameMesScale = 0.0f;
            HuSprScaleSet(mes->grpId[0], j, GameMesScale, GameMesScale);
            SprRareZRot[j] = 0.0f;
        }
        HuSprPosSet(mes->grpId[0], j, x, y);
        HuSprPriSet(mes->grpId[0], j, 5);
    }
    GameMesPauseEnable(FALSE);
    return TRUE;
}

BOOL GameMesMgRareExec(GAMEMES *mes)
{
    s16 j;
    float x;
    float y;

    mes->time += GameMesVWait;
    if(mes->time >= mes->timeMax && mes->mesMode == 0) {
        mes->stat |= GAMEMES_STAT_TIMEEND;
        mes->mesMode = -1;
        mes->angle = 0;
    }
    switch(mes->mesMode) {
        case 1:
            if(mes->subMode == GAMEMES_MG_TYPE_START) {
                if(mes->time <= 60 && mes->time >= 20) {
                    for(j=0; j<mes->charNum; j++) {
                        SprOrbitAngle[j][0] -= SprOrbitAngle[j][1]/20.0f;
                        x = GetGameMesWidth(mes, 0, j, 1.0f)+SprOrbitAngle[j][0];
                        if(x < GetGameMesWidth(mes, 0, j, 1.0f)) {
                            x = GetGameMesWidth(mes, 0, j, 1.0f);
                        }
                        y = mes->pos.y;
                        HuSprPosSet(mes->grpId[0], j, x, y);
                        if(mes->time < 40) {
                            SprRareZRot[j] -= 30.0f;
                            HuSprZRotSet(mes->grpId[0], j, SprRareZRot[j]);
                        }
                    }
                }
                if(mes->time >= 60) {
                    GameMesScale = (sind((mes->time-60)*9.0f)*0.5)+1.0;
                    for(j=0; j<mes->charNum; j++) {
                        x = GetGameMesWidth(mes, 0, j, GameMesScale);
                        y = mes->pos.y;
                        HuSprPosSet(mes->grpId[0], j, x, y);
                        HuSprScaleSet(mes->grpId[0], j, mes->scale.x*GameMesScale, mes->scale.y*GameMesScale);
                    }
                }
                if(mes->time == 60) {
                    HuAudFXPlay(MSM_SE_CMN_26);
                    mes->stat |= GAMEMES_STAT_FXPLAY;
                }
                if(mes->time == 80) {
                    mes->stat |= GAMEMES_STAT_TIMEEND;
                    GameMesEnd();
                }
                if(mes->time >= 80) {
                    mes->mesMode = 0;
                    mes->timeMax = 110;
                }
            } else {
                if(mes->time <= 20) {
                    GameMesScale = sind(mes->time*4.5f);
                    for(j=0; j<mes->charNum; j++) {
                        x = GetGameMesWidth(mes, 2, j, GameMesScale);
                        y = mes->pos.y;
                        HuSprPosSet(mes->grpId[0], j, x, y);
                        HuSprScaleSet(mes->grpId[0], j, mes->scale.x*GameMesScale, mes->scale.y*GameMesScale);
                    }
                }
                if(mes->time == 20) {
                    if(mes->subMode == GAMEMES_MG_TYPE_FINISH) {
                        HuAudFXPlay(MSM_SE_CMN_27);
                    } else {
                        HuAudFXPlay(MSM_SE_CMN_29);
                    }
                    mes->stat |= GAMEMES_STAT_FXPLAY;
                }
                if(mes->time >= 50) {
                    for(j=0; j<mes->charNum; j++) {
                        SprOrbitAngle[j][0] += SprOrbitAngle[j][1]/20.0f;
                        x = GetGameMesWidth(mes, 2, j, 1.0f)-SprOrbitAngle[j][0];
                        y = mes->pos.y;
                        HuSprPosSet(mes->grpId[0], j, x, y);
                        SprRareZRot[j] -= 30.0f;
                        HuSprZRotSet(mes->grpId[0], j, SprRareZRot[j]);
                    }
                }
                if(mes->time >= 120) {
                    mes->mesMode = 0;
                    mes->stat |= GAMEMES_STAT_KILL;
                }
            }
            break;

        case -1:
            mes->angle += 0.1f*GameMesVWait;
            GameMesTPLvl = 1.0f-mes->angle;
            if(GameMesTPLvl <= 0) {
                GameMesTPLvl = 0;
                mes->mesMode = 0;
                mes->stat |= GAMEMES_STAT_KILL;
            }
            for(j=0; j<mes->charNum; j++) {
                HuSprTPLvlSet(mes->grpId[0], j, GameMesTPLvl);
            }
            break;

        default:
            break;
    }
    if(GameMesCloseF || (mes->stat & GAMEMES_STAT_KILL)) {
        GameMesSprKill(mes);
        if(mes->subMode == GAMEMES_MG_TYPE_START) {
            GameMesPauseEnable(TRUE);
        }
        return FALSE;
    }
    return TRUE;
}

BOOL GameMesMgKettouInit(GAMEMES *mes, va_list args)
{
    s16 j;
    s16 center;
    u32 mesId;
    GAMEMESID strMes;
    float y;
    float x;

    mes->subMode = va_arg(args, int);
    if(mes->subMode == GAMEMES_MG_TYPE_DRAW) {
        GameMesMgDrawInit(mes, args);
        return TRUE;
    }
    mes->mesMode = 1;
    mes->angle = 0;
    mesId = (mes->subMode == GAMEMES_MG_TYPE_START) ? 0x42000A : 0x42000B;
    strMes = GameMesStrWinCreate(mes, mesId);
    for(j=0; j<mes->charNum; j++) {
        x = GetGameMesWidth(mes, 0, j, 1.0f);
        y = mes->pos.y;
        if(mes->subMode == GAMEMES_MG_TYPE_START) {
            center = mes->charNum / 2;
            if(j == center) {
                GameMesScale = 0.0f;
            } else {
                SprOrbitAngle[j][0] = SprOrbitAngle[j][1] = 56.0f+mes->pos.y;
                if(j < center) {
                    y -= SprOrbitAngle[j][0];
                } else {
                    y += SprOrbitAngle[j][0];
                }
                GameMesScale = 1.0f;
            }
            HuSprScaleSet(mes->grpId[0], j, GameMesScale, GameMesScale);
        } else {
            SprOrbitAngle[j][0] = 0.0f;
            SprOrbitAngle[j][1] = 56.0f+mes->pos.y;
            GameMesScale = 0.0f;
            HuSprScaleSet(mes->grpId[0], j, GameMesScale, GameMesScale);
        }
        HuSprPosSet(mes->grpId[0], j, x, y);
        HuSprPriSet(mes->grpId[0], j, 5);
    }
    MgMesTime = 0;
    GameMesPauseEnable(FALSE);
    return TRUE;
}

BOOL GameMesMgKettouExec(GAMEMES *mes)
{
    s16 j;
    s16 center;
    float y;
    float x;

    mes->time += GameMesVWait;
    if(mes->time >= mes->timeMax && mes->mesMode == 0) {
        mes->stat |= GAMEMES_STAT_TIMEEND;
        mes->mesMode = -1;
        mes->angle = 0;
    }
    switch(mes->mesMode) {
        case 1:
            if(mes->subMode == GAMEMES_MG_TYPE_START) {
                if(mes->time <= 60 && mes->time >= 20) {
                    for(j=0; j<mes->charNum; j++) {
                        x = GetGameMesWidth(mes, 0, j, 1.0f);
                        SprOrbitAngle[j][0] -= SprOrbitAngle[j][1]/20.0f;
                        center = mes->charNum / 2;
                        if(j == center) {
                            y = mes->pos.y;
                            GameMesScale = sind(MgMesTime*4.5f);
                            if(MgMesTime > 20) {
                                GameMesScale = 1.0f;
                            }
                            HuSprScaleSet(mes->grpId[0], j, mes->scale.x*GameMesScale, mes->scale.y*GameMesScale);
                        } else if(j < center) {
                            y = mes->pos.y-SprOrbitAngle[j][0];
                            if(y > mes->pos.y) {
                                y = mes->pos.y;
                            }
                        } else {
                            y = mes->pos.y+SprOrbitAngle[j][0];
                            if(y < mes->pos.y) {
                                y = mes->pos.y;
                            }
                        }
                        HuSprPosSet(mes->grpId[0], j, x, y);
                    }
                    MgMesTime++;
                }
                if(mes->time >= 60) {
                    GameMesScale = (sind((mes->time-60)*9.0f)*0.5)+1.0;
                    for(j=0; j<mes->charNum; j++) {
                        x = GetGameMesWidth(mes, 0, j, GameMesScale);
                        y = mes->pos.y;
                        HuSprPosSet(mes->grpId[0], j, x, y);
                        HuSprScaleSet(mes->grpId[0], j, mes->scale.x*GameMesScale, mes->scale.y*GameMesScale);
                    }
                }
                if(mes->time == 60) {
                    HuAudFXPlay(MSM_SE_CMN_26);
                    mes->stat |= GAMEMES_STAT_FXPLAY;
                }
                if(mes->time == 80) {
                    mes->stat |= GAMEMES_STAT_TIMEEND;
                    GameMesEnd();
                }
                if(mes->time >= 80) {
                    mes->mesMode = 0;
                    mes->timeMax = 110;
                }
            } else {
                if(mes->time <= 20) {
                    GameMesScale = sind(mes->time*4.5f);
                    for(j=0; j<mes->charNum; j++) {
                        x = GetGameMesWidth(mes, 2, j, GameMesScale);
                        y = mes->pos.y;
                        HuSprPosSet(mes->grpId[0], j, x, y);
                        HuSprScaleSet(mes->grpId[0], j, mes->scale.x*GameMesScale, mes->scale.y*GameMesScale);
                    }
                }
                if(mes->time == 20) {
                    if(mes->subMode == GAMEMES_MG_TYPE_FINISH) {
                        HuAudFXPlay(MSM_SE_CMN_27);
                    } else {
                        HuAudFXPlay(MSM_SE_CMN_29);
                    }
                    mes->stat |= GAMEMES_STAT_FXPLAY;
                }
                if(mes->time >= 50) {
                    for(j=0; j<mes->charNum; j++) {
                        x = GetGameMesWidth(mes, 2, j, 1.0f);
                        SprOrbitAngle[j][0] += SprOrbitAngle[j][1]/20.0f;
                        center = mes->charNum / 2;
                        if(j == center) {
                            y = mes->pos.y;
                            GameMesScale = (sind(MgMesTime*4.5f)*3.0)+1.0;
                            if(MgMesTime > 20) {
                                GameMesScale = 1.0f;
                            }
                            HuSprScaleSet(mes->grpId[0], j, mes->scale.x*GameMesScale, mes->scale.y*GameMesScale);
                            GameMesTPLvl = sind(MgMesTime*4.5f);
                            if(MgMesTime > 20) {
                                GameMesTPLvl = 1.0f;
                            }
                            HuSprTPLvlSet(mes->grpId[0], j, 1.0f-GameMesTPLvl);
                        } else if(j < center) {
                            y = mes->pos.y-SprOrbitAngle[j][0];
                        } else {
                            y = mes->pos.y+SprOrbitAngle[j][0];
                        }
                        HuSprPosSet(mes->grpId[0], j, x, y);
                    }
                    MgMesTime++;
                }
                if(mes->time >= 120) {
                    mes->mesMode = 0;
                    mes->stat |= GAMEMES_STAT_KILL;
                }
            }
            break;

        case -1:
            mes->angle += 0.1f*GameMesVWait;
            GameMesTPLvl = 1.0f-mes->angle;
            if(GameMesTPLvl <= 0) {
                GameMesTPLvl = 0;
                mes->mesMode = 0;
                mes->stat |= GAMEMES_STAT_KILL;
            }
            for(j=0; j<mes->charNum; j++) {
                HuSprTPLvlSet(mes->grpId[0], j, GameMesTPLvl);
            }
            break;

        default:
            break;
    }
    if(GameMesCloseF || (mes->stat & GAMEMES_STAT_KILL)) {
        GameMesSprKill(mes);
        if(mes->subMode == GAMEMES_MG_TYPE_START) {
            GameMesPauseEnable(TRUE);
        }
        return FALSE;
    }
    return TRUE;
}

BOOL GameMesMgDonkeyInit(GAMEMES *mes, va_list args)
{
    s16 j;
    s16 center;
    u32 mesId;
    GAMEMESID strMes;
    float x;
    float y;

    mes->subMode = va_arg(args, int);
    if(mes->subMode == GAMEMES_MG_TYPE_DRAW) {
        GameMesMgDrawInit(mes, args);
        return TRUE;
    }
    mes->mesMode = 1;
    mes->angle = 0;
    mesId = (mes->subMode == GAMEMES_MG_TYPE_START) ? 0x42000A : 0x42000B;
    strMes = GameMesStrWinCreate(mes, mesId);
    for(j=0; j<mes->charNum; j++) {
        x = GetGameMesWidth(mes, 0, j, 1.0f);
        y = mes->pos.y;
        if(mes->subMode == GAMEMES_MG_TYPE_START) {
            center = mes->charNum / 2;
            if(j == center) {
                GameMesScale = 0.0f;
            } else {
                SprOrbitAngle[j][0] = SprOrbitAngle[j][1] = 56.0f+mes->pos.y;
                if(j < center) {
                    x -= SprOrbitAngle[j][0];
                    y -= SprOrbitAngle[j][0];
                } else {
                    x += SprOrbitAngle[j][0];
                    y -= SprOrbitAngle[j][0];
                }
                GameMesScale = 1.0f;
            }
            HuSprScaleSet(mes->grpId[0], j, GameMesScale, GameMesScale);
        } else {
            SprOrbitAngle[j][0] = 0.0f;
            SprOrbitAngle[j][1] = 56.0f+mes->pos.y;
            GameMesScale = 0.0f;
            HuSprScaleSet(mes->grpId[0], j, GameMesScale, GameMesScale);
        }
        HuSprPosSet(mes->grpId[0], j, x, y);
        HuSprPriSet(mes->grpId[0], j, 5);
    }
    MgMesTime = 0;
    GameMesPauseEnable(FALSE);
    return TRUE;
}

BOOL GameMesMgDonkeyExec(GAMEMES *mes)
{
    s16 j;
    s16 center;
    float x;
    float y;

    mes->time += GameMesVWait;
    if(mes->time >= mes->timeMax && mes->mesMode == 0) {
        mes->stat |= GAMEMES_STAT_TIMEEND;
        mes->mesMode = -1;
        mes->angle = 0;
    }
    switch(mes->mesMode) {
        case 1:
            if(mes->subMode == GAMEMES_MG_TYPE_START) {
                if(mes->time <= 60 && mes->time >= 20) {
                    for(j=0; j<mes->charNum; j++) {
                        x = GetGameMesWidth(mes, 0, j, 1.0f);
                        SprOrbitAngle[j][0] -= SprOrbitAngle[j][1]/20.0f;
                        center = mes->charNum / 2;
                        if(j == center) {
                            y = mes->pos.y;
                            GameMesScale = sind(MgMesTime*4.5f);
                            if(MgMesTime > 20) {
                                GameMesScale = 1.0f;
                            }
                            HuSprScaleSet(mes->grpId[0], j, mes->scale.x*GameMesScale, mes->scale.y*GameMesScale);
                        } else if(j < center) {
                            x = GetGameMesWidth(mes, 0, j, 1.0f)-SprOrbitAngle[j][0];
                            if(x > GetGameMesWidth(mes, 0, j, 1.0f)) {
                                x = GetGameMesWidth(mes, 0, j, 1.0f);
                            }
                            y = mes->pos.y-SprOrbitAngle[j][0];
                            if(y > mes->pos.y) {
                                y = mes->pos.y;
                            }
                        } else {
                            x = GetGameMesWidth(mes, 0, j, 1.0f)+SprOrbitAngle[j][0];
                            if(x < GetGameMesWidth(mes, 0, j, 1.0f)) {
                                x = GetGameMesWidth(mes, 0, j, 1.0f);
                            }
                            y = mes->pos.y-SprOrbitAngle[j][0];
                            if(y > mes->pos.y) {
                                y = mes->pos.y;
                            }
                        }
                        HuSprPosSet(mes->grpId[0], j, x, y);
                    }
                    MgMesTime++;
                }
                if(mes->time >= 60) {
                    GameMesScale = (sind((mes->time-60)*9.0f)*0.5)+1.0;
                    for(j=0; j<mes->charNum; j++) {
                        x = GetGameMesWidth(mes, 0, j, GameMesScale);
                        y = mes->pos.y;
                        HuSprPosSet(mes->grpId[0], j, x, y);
                        HuSprScaleSet(mes->grpId[0], j, mes->scale.x*GameMesScale, mes->scale.y*GameMesScale);
                    }
                }
                if(mes->time == 60) {
                    HuAudFXPlay(MSM_SE_CMN_26);
                    mes->stat |= GAMEMES_STAT_FXPLAY;
                }
                if(mes->time == 80) {
                    mes->stat |= GAMEMES_STAT_TIMEEND;
                    GameMesEnd();
                }
                if(mes->time >= 80) {
                    mes->mesMode = 0;
                    mes->timeMax = 110;
                }
            } else {
                if(mes->time <= 20) {
                    GameMesScale = sind(mes->time*4.5f);
                    for(j=0; j<mes->charNum; j++) {
                        x = GetGameMesWidth(mes, 2, j, GameMesScale);
                        y = mes->pos.y;
                        HuSprPosSet(mes->grpId[0], j, x, y);
                        HuSprScaleSet(mes->grpId[0], j, mes->scale.x*GameMesScale, mes->scale.y*GameMesScale);
                    }
                }
                if(mes->time == 20) {
                    if(mes->subMode == GAMEMES_MG_TYPE_FINISH) {
                        HuAudFXPlay(MSM_SE_CMN_27);
                    } else {
                        HuAudFXPlay(MSM_SE_CMN_29);
                    }
                    mes->stat |= GAMEMES_STAT_FXPLAY;
                }
                if(mes->time >= 50) {
                    for(j=0; j<mes->charNum; j++) {
                        x = GetGameMesWidth(mes, 2, j, 1.0f);
                        SprOrbitAngle[j][0] += SprOrbitAngle[j][1]/20.0f;
                        center = mes->charNum / 2;
                        if(j == center) {
                            y = mes->pos.y;
                            GameMesScale = (sind(MgMesTime*4.5f)*3.0)+1.0;
                            if(MgMesTime > 20) {
                                GameMesScale = 1.0f;
                            }
                            HuSprScaleSet(mes->grpId[0], j, mes->scale.x*GameMesScale, mes->scale.y*GameMesScale);
                            GameMesTPLvl = sind(MgMesTime*4.5f);
                            if(MgMesTime > 20) {
                                GameMesTPLvl = 1.0f;
                            }
                            HuSprTPLvlSet(mes->grpId[0], j, 1.0f-GameMesTPLvl);
                        } else if(j < center) {
                            x = GetGameMesWidth(mes, 2, j, 1.0f)-SprOrbitAngle[j][0];
                            y = mes->pos.y-SprOrbitAngle[j][0];
                        } else {
                            x = GetGameMesWidth(mes, 2, j, 1.0f)+SprOrbitAngle[j][0];
                            y = mes->pos.y-SprOrbitAngle[j][0];
                        }
                        HuSprPosSet(mes->grpId[0], j, x, y);
                    }
                    MgMesTime++;
                }
                if(mes->time >= 120) {
                    mes->mesMode = 0;
                    mes->stat |= GAMEMES_STAT_KILL;
                }
            }
            break;

        case -1:
            mes->angle += 0.1f*GameMesVWait;
            GameMesTPLvl = 1.0f-mes->angle;
            if(GameMesTPLvl <= 0) {
                GameMesTPLvl = 0;
                mes->mesMode = 0;
                mes->stat |= GAMEMES_STAT_KILL;
            }
            for(j=0; j<mes->charNum; j++) {
                HuSprTPLvlSet(mes->grpId[0], j, GameMesTPLvl);
            }
            break;

        default:
            break;
    }
    if(GameMesCloseF || (mes->stat & GAMEMES_STAT_KILL)) {
        GameMesSprKill(mes);
        if(mes->subMode == GAMEMES_MG_TYPE_START) {
            GameMesPauseEnable(TRUE);
        }
        return FALSE;
    }
    return TRUE;
}

BOOL GameMesMgSdInit(GAMEMES *mes, va_list args)
{
    s16 i;
    s16 j;
    GAMEMESID strMes;
    float x;
    float y;

    mes->subMode = va_arg(args, int);
    if(mes->subMode == GAMEMES_MG_TYPE_DRAW) {
        GameMesMgDrawInit(mes, args);
        return TRUE;
    }
    mes->mesMode = 1;
    mes->angle = 0;
    if(mes->subMode == GAMEMES_MG_TYPE_START) {
        for(j=0; j<15; j++) {
            SdMesScale[j] = 20.0f;
            SdMesScaleTime[j] = 0;
        }
        GameMesScale = 0.0f;
        SdMesWork[0] = SdMesWork[1] = 0;
    } else {
        for(j=0; j<15; j++) {
            MesUnk1[j] = 0;
            SdMesScale[j] = 1.0f;
        }
        GameMesScale = 0.0f;
        SdMesWork[0] = 1;
        SdMesWork[1] = 0;
    }
    for(i=0; i<6; i++) {
        if(i == 0) {
            u32 mesId = (mes->subMode == GAMEMES_MG_TYPE_START) ? 0x42000A : 0x42000B;
            strMes = GameMesStrWinCreate(mes, mesId);
        } else {
            strMes = GameMesStrCopy(mes, (s16)(int)strMes);
        }
        for(j=0; j<mes->charNum; j++) {
            if(mes->subMode == GAMEMES_MG_TYPE_START) {
                x = mes->pos.x+(mes->scale.x*(((-mes->charNum*56)/2)+(j*56)+28));
                y = mes->pos.y;
                if(i == 0) {
                    HuSprScaleSet(mes->grpId[i], j, mes->scale.x*SdMesScale[j], mes->scale.y*SdMesScale[j]);
                } else {
                    HuSprScaleSet(mes->grpId[i], j, mes->scale.x, mes->scale.y);
                }
            } else {
                x = mes->pos.x+(mes->scale.x*(((-mes->charNum*56)/2)+(j*56)+28));
                y = mes->pos.y;
                if(i == 0) {
                    SdMesSprPosX[j] = x;
                    SdMesSprPosY[j] = y;
                    HuSprScaleSet(mes->grpId[i], j, 0.0f, 0.0f);
                } else {
                    HuSprScaleSet(mes->grpId[i], j, mes->scale.x, mes->scale.y);
                }
            }
            HuSprPosSet(mes->grpId[i], j, x, y);
            HuSprPriSet(mes->grpId[i], j, (mes->subMode == GAMEMES_MG_TYPE_START) ? (i+5) : (j+(i+5)));
            if(i == 0) {
                HuSprTPLvlSet(mes->grpId[i], j, (mes->subMode == GAMEMES_MG_TYPE_START) ? 0.0f : 1.0f);
                SdMesTPLvl[i][j] = (mes->subMode == GAMEMES_MG_TYPE_START) ? 0.0f : 1.0f;
            } else {
                HuSprTPLvlSet(mes->grpId[i], j, 1.0f/(i+1));
                SdMesTPLvl[i][j] = 1.0f/(i+1);
            }
            if(mes->subMode == GAMEMES_MG_TYPE_START) {
                HuSprDispOff(mes->grpId[i], j);
            }
            if(i != 0 && mes->subMode == GAMEMES_MG_TYPE_FINISH) {
                HuSprDispOff(mes->grpId[i], j);
            }
        }
    }
    GameMesPauseEnable(FALSE);
    return TRUE;
}

BOOL GameMesMgSdExec(GAMEMES *mes)
{
    s16 j;
    s16 i;
    int timeStart;
    int timeEnd;
    float x;
    float y;

    mes->time += GameMesVWait;
    if(mes->time >= mes->timeMax && mes->mesMode == 0) {
        mes->stat |= GAMEMES_STAT_TIMEEND;
        mes->mesMode = -1;
        mes->angle = 0;
    }
    switch(mes->mesMode) {
        case 1:
            if(mes->subMode == GAMEMES_MG_TYPE_START) {
                if(GameMesLanguageNo == 0) {
                    timeStart = 110;
                    timeEnd = 130;
                } else {
                    timeStart = 123;
                    timeEnd = 143;
                }
                if(mes->time <= timeStart) {
                    if(SdMesWork[0] < mes->charNum) {
                        if(mes->time > 0) {
                            SdMesWork[0] = 1;
                        }
                        if(mes->time > 8) {
                            SdMesWork[0]++;
                        }
                        for(j=2; j<mes->charNum; j++) {
                            if(mes->time > ((j-2)*7)+45) {
                                SdMesWork[0]++;
                            }
                        }
                        if(SdMesWork[0] > mes->charNum) {
                            SdMesWork[0] = mes->charNum;
                        }
                    }
                    for(i=0; i<6; i++) {
                        for(j=0; j<SdMesWork[0]; j++) {
                            if(i == 0) {
                                SdMesScale[j] -= 20.0f/(timeStart/5);
                                if(SdMesScale[j] < 1.0f) {
                                    SdMesScale[j] = 1.0f;
                                }
                                HuSprDispOn(mes->grpId[i], j);
                            }
                            HuSprScaleSet(mes->grpId[i], j, mes->scale.x*SdMesScale[j], mes->scale.y*SdMesScale[j]);
                            if(i == 0) {
                                SdMesTPLvl[0][j] += 0.017f;
                                if(SdMesTPLvl[0][j] > 1.0f) {
                                    SdMesTPLvl[0][j] = 1.0f;
                                }
                                HuSprTPLvlSet(mes->grpId[i], j, SdMesTPLvl[0][j]);
                                x = mes->pos.x+(mes->scale.x*(((-mes->charNum*56)/2)+(j*56)+28));
                            } else {
                                x = mes->pos.x+(mes->scale.x*(((-mes->charNum*56)/2)+(j*56)+28));
                            }
                            y = mes->pos.y;
                            HuSprPosSet(mes->grpId[i], j, x, y);
                            if(i == 0) {
                                if(SdMesScale[j] == 1.0f) {
                                    SdMesScaleTime[j]++;
                                }
                            }
                            if(SdMesScaleTime[j] != 0) {
                                HuSprDispOn(mes->grpId[i], j);
                                HuSprScaleSet(mes->grpId[i], j,
                                    mes->scale.x*(1.0f+(0.04f*((SdMesScaleTime[j]/2)*i))),
                                    mes->scale.y*(1.0f+(0.04f*((SdMesScaleTime[j]/2)*i))));
                                if(i != 0) {
                                    SdMesTPLvl[i][j] -= 0.02f;
                                    if(SdMesTPLvl[i][j] < 0.01f) {
                                        SdMesTPLvl[i][j] = 0.0f;
                                        HuSprDispOff(mes->grpId[i], j);
                                    } else {
                                        HuSprTPLvlSet(mes->grpId[i], j, SdMesTPLvl[i][j]);
                                        x += frandmod(7)*(0.1f*(frandmod(2) != 0 ? 1 : -1));
                                        y += frandmod(7)*(0.1f*(frandmod(2) != 0 ? 1 : -1));
                                        HuSprPosSet(mes->grpId[i], j, x, y);
                                        if(i == 1) {
                                            HuSprPosSet(mes->grpId[0], j, x, y);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                if(mes->time >= timeStart) {
                    GameMesScale = (sind((mes->time-timeStart)*9.0f)*0.5)+1.0;
                    for(j=0; j<mes->charNum; j++) {
                        x = mes->pos.x+(mes->scale.x*(((-mes->charNum*56)/2)+(j*56)+28)*GameMesScale);
                        y = mes->pos.y;
                        HuSprPosSet(mes->grpId[0], j, x, y);
                        HuSprScaleSet(mes->grpId[0], j, mes->scale.x*GameMesScale, mes->scale.y*GameMesScale);
                    }
                }
                if(mes->time == timeStart) {
                    HuAudFXPlay(MSM_SE_CMN_26);
                    mes->stat |= GAMEMES_STAT_FXPLAY;
                }
                if(mes->time == timeEnd) {
                    mes->stat |= GAMEMES_STAT_TIMEEND;
                    GameMesEnd();
                }
                if(mes->time >= timeEnd) {
                    mes->mesMode = 0;
                    mes->timeMax = timeEnd+30;
                }
            } else {
                if(mes->time <= 20) {
                    GameMesScale = sind(mes->time*4.5f);
                    for(j=0; j<mes->charNum; j++) {
                        x = mes->pos.x+(mes->scale.x*(((-mes->charNum*56)/2)+(j*56)+28)*GameMesScale);
                        y = mes->pos.y;
                        HuSprPosSet(mes->grpId[0], j, x, y);
                        HuSprScaleSet(mes->grpId[0], j, mes->scale.x*GameMesScale, mes->scale.y*GameMesScale);
                    }
                }
                if(mes->time == 20) {
                    if(mes->subMode == GAMEMES_MG_TYPE_FINISH) {
                        HuAudFXPlay(MSM_SE_CMN_27);
                    } else {
                        HuAudFXPlay(MSM_SE_CMN_29);
                    }
                    mes->stat |= GAMEMES_STAT_FXPLAY;
                }
                if(mes->time >= 50) {
                    for(i=0; i<1; i++) {
                        for(j=0; j<SdMesWork[0]; j++) {
                            x = mes->pos.x+(mes->scale.x*(((-mes->charNum*56)/2)+(j*56)+28));
                            y = mes->pos.y;
                            HuSprPosSet(mes->grpId[i], j, x, y);
                            if(SdMesScale[j] == 1.0f) {
                                if(SdMesWork[1] < 3) {
                                    HuSprScaleSet(mes->grpId[i], j,
                                        mes->scale.x*(1.0f-(0.2f*(SdMesWork[1]+1))),
                                        mes->scale.y*(1.0f-(0.2f*(SdMesWork[1]+1))));
                                } else if(SdMesWork[1] < 5) {
                                    HuSprScaleSet(mes->grpId[i], j,
                                        mes->scale.x*(1.0f-(0.3f*(4-SdMesWork[1]))),
                                        mes->scale.y*(1.0f-(0.3f*(4-SdMesWork[1]))));
                                }
                            }
                            if(SdMesWork[1] >= 5 || SdMesScale[j] > 1.0f) {
                                if(i == 0) {
                                    SdMesScale[j] += 1.5f;
                                    if(SdMesScale[j] < 0.0f) {
                                        SdMesScale[j] = 0.0f;
                                    }
                                }
                                HuSprScaleSet(mes->grpId[i], j, mes->scale.x*SdMesScale[j], mes->scale.y*SdMesScale[j]);
                                SdMesTPLvl[i][j] -= 0.07f;
                                if(SdMesTPLvl[i][j] < 0.0f) {
                                    SdMesTPLvl[i][j] = 0.0f;
                                    HuSprDispOff(mes->grpId[i], j);
                                } else {
                                    HuSprTPLvlSet(mes->grpId[i], j, SdMesTPLvl[i][j]);
                                }
                            }
                        }
                    }
                    if(SdMesWork[0] <= mes->charNum && ++SdMesWork[1] == 8) {
                        SdMesWork[0]++;
                        if(SdMesWork[0] > mes->charNum) {
                            SdMesWork[0] = mes->charNum;
                        }
                        SdMesWork[1] = 0;
                    }
                }
                if(mes->time >= 180) {
                    mes->mesMode = 0;
                    mes->stat |= GAMEMES_STAT_KILL;
                }
            }
            break;

        case -1:
            mes->angle += GameMesVWait*0.1f;
            GameMesTPLvl = 1.0f-mes->angle;
            if(GameMesTPLvl <= 0) {
                GameMesTPLvl = 0;
                mes->mesMode = 0;
                mes->stat |= GAMEMES_STAT_KILL;
            }
            for(j=0; j<mes->charNum; j++) {
                HuSprTPLvlSet(mes->grpId[0], j, GameMesTPLvl);
            }
            break;

        default:
            break;
    }
    if(GameMesCloseF || (mes->stat & GAMEMES_STAT_KILL)) {
        GameMesSprKill(mes);
        if(mes->subMode == GAMEMES_MG_TYPE_START) {
            GameMesPauseEnable(TRUE);
        }
        return FALSE;
    }
    return TRUE;
}

BOOL GameMesMgDrawInit(GAMEMES *mes, va_list args)
{
    s16 i;
    s16 j;
    float x;
    float y;
    float scale;
    GAMEMESID strMes;

    mes->subMode = GAMEMES_MG_TYPE_DRAW;
    mes->mesNo = GAMEMES_MES_MG_DRAW;
    mes->mesMode = 1;
    mes->angle = 0;
    for(j=0; j<15; j++) {
        MesUnk1[j] = 0;
        for(i=0; i<6; i++) {
            DrawMesTime[j][i] = 0;
            DrawMesY[j][i] = mes->pos.y;
        }
    }
    GameMesScale = 0;
    SdMesWork[0] = 1;
    SdMesWork[1] = 0;
    for(i=0; i<6; i++) {
        if(i == 0) {
            strMes = GameMesStrWinCreate(mes, 0x42000D);
        } else {
            strMes = GameMesStrCopy(mes, (s16)(int)strMes);
        }
        for(j=0; j<mes->charNum; j++) {
            x = GetGameMesWidth(mes, 1, j, 1.0f);
            y = mes->pos.y;
            if(i == 0) {
                SdMesSprPosX[j] = x;
                SdMesSprPosY[j] = y;
            }
            HuSprPosSet(mes->grpId[i], j, x, y);
            HuSprPriSet(mes->grpId[i], j, 5+i);
            HuSprTPLvlSet(mes->grpId[i], j, 1.0f/(i+1));
            if(mes->charNum <= 6) {
                scale = 1.0f;
            } else {
                scale = 6.0f/mes->charNum;
            }
            mes->scale.x = scale;
            if(i == 0) {
                HuSprScaleSet(mes->grpId[i], j, 0, 0);
            } else {
                HuSprScaleSet(mes->grpId[i], j, mes->scale.x, mes->scale.y);
                HuSprDispOff(mes->grpId[i], j);
            }
        }
    }
    return TRUE;
}

BOOL GameMesMgDrawExec(GAMEMES *mes)
{
    s16 i;
    s16 j;
    s16 k;
    float x;
    float y;
    float t;

    mes->time += GameMesVWait;
    switch(mes->mesMode) {
        case 1:
            if(mes->time <= 20) {
                GameMesScale = sind(mes->time*4.5f);
                for(j=0; j<mes->charNum; j++) {
                    x = GetGameMesWidth(mes, 1, j, GameMesScale);
                    y = mes->pos.y;
                    HuSprPosSet(mes->grpId[0], j, x, y);
                    HuSprScaleSet(mes->grpId[0], j, mes->scale.x*GameMesScale, mes->scale.y*GameMesScale);
                }
            }
            if(mes->time == 20) {
                HuAudFXPlay(MSM_SE_CMN_29);
                mes->stat |= GAMEMES_STAT_FXPLAY;
            }
            if(mes->time >= 140) {
                for(i=0; i<6; i++) {
                    for(j=0; j<SdMesWork[0]; j++) {
                        DrawMesTime[j][i]++;
                        x = GetGameMesWidth(mes, 1, j, 1.0f);
                        if(i == 0) {
                            t = DrawMesTime[j][i];
                            y = mes->pos.y-((36.0f*t)-(1.8f*t*t));
                            for(k=5; k>0; k--) {
                                DrawMesY[j][k] = DrawMesY[j][k-1];
                            }
                            DrawMesY[j][0] = y;
                        } else {
                            y = DrawMesY[j][i];
                        }
                        HuSprPosSet(mes->grpId[i], j, x, y);
                        HuSprDispOn(mes->grpId[i], j);
                        DrawMesY[j][i] = y;
                    }
                }
                if(SdMesWork[0] < mes->charNum && ++SdMesWork[1] == 10) {
                    SdMesWork[0]++;
                    SdMesWork[1] = 0;
                }
            }
            if(mes->time >= 210) {
                mes->mesMode = 0;
                mes->stat |= GAMEMES_STAT_KILL;
            }
            break;
    }
    if(GameMesCloseF || (mes->stat & GAMEMES_STAT_KILL)) {
        GameMesSprKill(mes);
        return FALSE;
    }
    return TRUE;
}

BOOL GameMesMgRecordInit(GAMEMES *mes, va_list args)
{
    s16 no;
    int num;
    s16 i;
    ANIMDATA *animP;
    HUSPRID sprId;
    s16 x;
    s16 separatorNo[2];
    s16 recordNo;

    RecordMesValue = va_arg(args, int);
    mes->mesMode = 1;
    mes->angle = 0;
    switch(omcurovl) {
        case 0x53:
            RecordMesLanguageNo = 0;
            num = 0;
            break;

        case 0x0D:
            RecordMesLanguageNo = 1;
            num = 1;
            break;

        case 0x0B:
        case 0x17:
        case 0x2B:
        case 0x35:
        case 0x37:
        case 0x39:
            RecordMesLanguageNo = 2;
            num = 0;
            break;

        case 0x6B:
            recordNo = GwSystem.subGameNo;
            RecordMesValue = GWMikeActRecordGet((s16)(int)recordNo);
            RecordMesLanguageNo = 2;
            num = 0;
            break;

        default:
            RecordMesLanguageNo = -1;
            num = 0;
            break;
    }
    mes->grpId[0] = HuSprGrpCreate(num+10);
    num = (GameMesLanguageNo == 0) ? DATANUM(DATA_gamemes, 9)
        : (GameMesLanguageNo == 1) ? DATANUM(DATA_gamemes, 10)
        : (GameMesLanguageNo == 2) ? DATANUM(DATA_gamemes, 11)
        : (GameMesLanguageNo == 3) ? DATANUM(DATA_gamemes, 12)
        : (GameMesLanguageNo == 4) ? DATANUM(DATA_gamemes, 13)
        : (GameMesLanguageNo == 5) ? DATANUM(DATA_gamemes, 14)
        : DATANUM(DATA_gamemes, 10);
    animP = HuSprAnimRead(GameMesDataRead(num));
    sprId = HuSprCreate(animP, 0, 0);
    HuSprGrpMemberSet(mes->grpId[0], 0, sprId);
    HuSprPosSet(mes->grpId[0], 0, 0, -32);
    HuSprPriSet(mes->grpId[0], 0, 2);
    animP = HuSprAnimRead(GameMesDataRead(DATANUM(DATA_gamemes, 8)));
    sprId = HuSprCreate(animP, 0, 0);
    HuSprGrpMemberSet(mes->grpId[0], 1, sprId);
    HuSprPriSet(mes->grpId[0], 1, 2);
    animP = HuSprAnimRead(GameMesDataRead(DATANUM(DATA_gamemes, 7)));

    for(i=0; i<8; i++) {
        sprId = HuSprCreate(animP, 0, 0);
        HuSprGrpMemberSet(mes->grpId[0], i+2, sprId);
        HuSprPriSet(mes->grpId[0], i+2, 2);
    }
    if(RecordMesLanguageNo == 1) {
        animP = HuSprAnimRead(GameMesDataRead(DATANUM(DATA_gamemes, 15)));
        sprId = HuSprCreate(animP, 0, 0);
        HuSprGrpMemberSet(mes->grpId[0], 10, sprId);
        HuSprPriSet(mes->grpId[0], 10, 2);
    }
    if(RecordMesLanguageNo < 2) {
        no = 0;
        if(RecordMesValue > 999999) {
            RecordMesValue = 999999;
        }
        num = RecordMesValue/100000;
        if(num != 0) {
            HuSprBankSet(mes->grpId[0], no+2, num);
            RecordMesValue -= num*100000;
            no++;
        }
        num = RecordMesValue/10000;
        if(num != 0 || no != 0) {
            HuSprBankSet(mes->grpId[0], no+2, num);
            RecordMesValue -= num*10000;
            no++;
        }
        num = RecordMesValue/1000;
        if(num != 0 || no != 0) {
            HuSprBankSet(mes->grpId[0], no+2, num);
            RecordMesValue -= num*1000;
            no++;
        }
        num = RecordMesValue/100;
        if(num != 0 || no != 0) {
            HuSprBankSet(mes->grpId[0], no+2, num);
            RecordMesValue -= num*100;
            no++;
        }
        num = RecordMesValue/10;
        if(num != 0 || no != 0) {
            HuSprBankSet(mes->grpId[0], no+2, num);
            RecordMesValue -= num*10;
            no++;
        }
        HuSprBankSet(mes->grpId[0], no+2, RecordMesValue);
        no++;
        for(i=no; i<8; i++) {
            HuSprDispOff(mes->grpId[0], i+2);
        }
        if(RecordMesLanguageNo == 1) {
            num = 16;
            HuSprPosSet(mes->grpId[0], 10, 48+(-((no*44)+48)/2)+(no*44)+num, 32);
        } else {
            num = 0;
        }
        HuSprPosSet(mes->grpId[0], 1, 24+(-((no*44)+48)/2)-num, 32);
        for(i=0; i<no; i++) {
            HuSprPosSet(mes->grpId[0], i+2, 70+(-((no*44)+48)/2)+(i*44)-num, 32);
            if(RecordMesLanguageNo == -1) {
                HuSprDispOff(mes->grpId[0], i+2);
            }
        }
    } else if(RecordMesLanguageNo == 2) {
        no = 0;
        num = RecordMesValue/36000;
        if(num != 0) {
            HuSprBankSet(mes->grpId[0], no+2, num);
            RecordMesValue -= num*36000;
            no++;
        }
        num = RecordMesValue/3600;
        HuSprBankSet(mes->grpId[0], no+2, num);
        RecordMesValue -= num*3600;
        no++;

        HuSprBankSet(mes->grpId[0], no+2, 11);
        separatorNo[0] = no;
        no++;

        num = RecordMesValue/600;
        HuSprBankSet(mes->grpId[0], no+2, num);
        RecordMesValue -= num*600;
        no++;

        num = RecordMesValue/60;
        HuSprBankSet(mes->grpId[0], no+2, num);
        RecordMesValue -= num*60;
        no++;

        HuSprBankSet(mes->grpId[0], no+2, 10);
        separatorNo[1] = no;
        no++;

        RecordMesValue = (100.0/60.0)*(float)RecordMesValue;

        num = RecordMesValue/10;
        HuSprBankSet(mes->grpId[0], no+2, num);
        RecordMesValue -= num*10;
        no++;

        HuSprBankSet(mes->grpId[0], no+2, RecordMesValue);
        no++;
        for(i=no; i<8; i++) {
            HuSprDispOff(mes->grpId[0], i+2);
        }
        HuSprPosSet(mes->grpId[0], 1, 24+(-((no*48)+20)/2), 32);
        for(x=72+(-((no*48)+20)/2), i=0; i<no; i++) {
            HuSprPosSet(mes->grpId[0], i+2, x, 32);
            if(separatorNo[0] == i) {
                x += 28;
            } else if(separatorNo[1] == i) {
                x += 48;
            } else {
                x += 48;
            }
        }
    }
    HuSprGrpScaleSet(mes->grpId[0], 0, 0);
    return TRUE;
}

BOOL GameMesMgRecordExec(GAMEMES *mes)
{
    if(!_CheckFlag(FLAG_MG_PRACTICE)) {
        mes->time += GameMesVWait;
        if(mes->time >= mes->timeMax && mes->mesMode == 0) {
            mes->stat |= GAMEMES_STAT_TIMEEND;
            mes->mesMode = -1;
            mes->angle = 0;
        }
        switch(mes->mesMode) {
            case 1:
                if(mes->time <= 20) {
                    GameMesScale = sind(mes->time*4.5f);
                    HuSprGrpPosSet(mes->grpId[0], 288, 240);
                    HuSprGrpScaleSet(mes->grpId[0], mes->scale.x*GameMesScale, mes->scale.y*GameMesScale);
                }
                if(mes->time == 20) {
                    HuAudFXPlay(MSM_SE_CMN_28);
                    mes->stat |= GAMEMES_STAT_FXPLAY;
                }
                if(mes->time >= 100) {
                    mes->mesMode = 0;
                    mes->timeMax = 140;
                }
                break;

            case -1:
                mes->angle += 0.1f*GameMesVWait;
                GameMesTPLvl = 1.0f-mes->angle;
                if(GameMesTPLvl <= 0.0f) {
                    GameMesTPLvl = 0;
                }
                HuSprGrpTPLvlSet(mes->grpId[0], GameMesTPLvl);
                GameMesScale = 1.0f-mes->angle;
                if(GameMesScale <= 0.0f) {
                    GameMesScale = 0;
                    mes->stat |= GAMEMES_STAT_KILL;
                }
                HuSprGrpScaleSet(mes->grpId[0], mes->scale.x*GameMesScale, mes->scale.y*GameMesScale);
                break;
        }
    } else {
        mes->stat |= GAMEMES_STAT_TIMEEND;
        mes->stat |= GAMEMES_STAT_KILL;
    }
    if(GameMesCloseF || (mes->stat & GAMEMES_STAT_KILL)) {
        GameMesSprKill(mes);
        return FALSE;
    }
    return TRUE;
}

BOOL GameMesMgWinInit(GAMEMES *mes, va_list args)
{
    int i;
    int charNo;
    int type;
    int nameLen;
    GAMEMESID strMes;
    char *nameMes;
    s16 nameFlag;
    u32 mesId;
    int chars[4];
    int charCount;
    int temp;
    float x;
    float y;
    float ofs;
    float scale;

    type = va_arg(args, int);
    if(type < 0 && type >= 6) {
        return FALSE;
    }
    mesId = 0x42000C;
    strMes = GameMesStrWinCreate(mes, mesId);
    WinnerMesSprX[0] = mes->charNum*56;
    WinnerMesSprNum[0] = mes->charNum;
    if(mes->charNum <= 6) {
        scale = 1.0f;
    } else {
        scale = 6.0f/mes->charNum;
    }
    mes->scale.x = scale;
    charCount = 0;
    chars[0] = chars[1] = chars[2] = chars[3] = -1;
    for(i=0; i<4; i++) {
        charNo = va_arg(args, int);
        if(charNo >= 0) {
            chars[charCount] = charNo;
            charCount++;
        }
    }
    if((GameMesLanguageNo == 3 || GameMesLanguageNo == 4) && charCount == 3) {
        temp = chars[0];
        chars[0] = chars[2];
        chars[2] = chars[1];
        chars[1] = temp;
    }
    WinnerMesCharNum = 0;
    for(i=0; i<4; i++) {
        charNo = chars[i];
        if(charNo < 0) {
            continue;
        }
        WinnerMesLanguageNo = (GameMesLanguageNo == 0) ? 0
            : (GameMesLanguageNo == 1) ? 1
            : (GameMesLanguageNo == 2) ? 2
            : (GameMesLanguageNo == 3) ? 3
            : (GameMesLanguageNo == 4) ? 4
            : 5;
        nameMes = WinnerMesNameTbl[(charNo*6)+WinnerMesLanguageNo];
        nameFlag = 0;
        strMes = GameMesStrCreate(mes, nameMes, nameFlag);
        if(GameMesLanguageNo == 1) {
            if(charNo == 9 || charNo == 11 || charNo == 12 || charNo == 13) {
                mes->charNum++;
            }
        } else if(GameMesLanguageNo == 2) {
            if(charNo == 8 || charNo == 9 || charNo == 11 || charNo == 12 || charNo == 13 || charNo == 14) {
                mes->charNum++;
            }
        } else if(GameMesLanguageNo == 3 || GameMesLanguageNo == 4 || GameMesLanguageNo == 5) {
            if(charNo == 9 || charNo == 11 || charNo == 12 || charNo == 13) {
                mes->charNum++;
            }
        }
        WinnerMesCharNo[WinnerMesCharNum] = charNo;
        WinnerMesCharNum++;
        WinnerMesSprX[WinnerMesCharNum] = mes->charNum*56;
        WinnerMesSprNum[WinnerMesCharNum] = mes->charNum;
    }
    if(WinnerMesCharNum == 0) {
        return FALSE;
    }
    mes->charNum = WinnerMesCharNum+1;
    mes->mesMode = 1;
    mes->angle = 0;
    WinnerMesXOfs = 600;
    for(y=0, i=0; i<mes->charNum; i++) {
        if(WinnerMesCharNum == 1) {
            nameLen = WinnerMesSprX[1]/56;
            if(nameLen <= 5) {
                mes->winScale = 1;
                ofs = WinnerMesSprX[1];
            } else {
                mes->winScale = 5.0f/nameLen;
                ofs = 280;
            }
            if(i != 0) {
                FixWinnerNameOfs(mes->grpId[i], WinnerMesCharNo[i-1], mes->winScale, WinnerMesSprX[i]/56);
            }
            x = ofs+WinnerMesSprX[0]+32.0f;
            if(i == 0) {
                x = ((576.0f-x)/2)+(x-(WinnerMesSprX[0]/2));
            } else {
                x = ((576.0f-x)/2)+(ofs/2);
            }
        } else if(i == 0) {
            x = 288.0f;
        } else if(WinnerMesCharNum == 2) {
            if(WinnerMesSprX[i]/56 <= 4) {
                mes->winScale = 1;
            } else {
                mes->winScale = 4.0f/(WinnerMesSprX[i]/56);
            }
            if(i != 0) {
                FixWinnerNameOfs(mes->grpId[i], WinnerMesCharNo[i-1], mes->winScale, WinnerMesSprX[i]/56);
            }
            if(i == 1) {
                x = 272.0f-((WinnerMesSprX[1]*mes->winScale)/2);
                if(WinnerMesSprX[i]/56 < 4) {
                    x -= ((4-(WinnerMesSprX[i]/56))*56)/2.0f;
                }
            } else {
                x = 304.0f+((WinnerMesSprX[2]*mes->winScale)/2);
                if(WinnerMesSprX[i]/56 < 4) {
                    x += ((4-(WinnerMesSprX[i]/56))*56)/2.0f;
                }
            }
        } else if(WinnerMesCharNum == 3) {
            if(WinnerMesSprX[i]/56 <= 4) {
                mes->winScale = 1;
            } else {
                mes->winScale = 4.0f/(WinnerMesSprX[i]/56);
            }
            if(i != 0) {
                FixWinnerNameOfs(mes->grpId[i], WinnerMesCharNo[i-1], mes->winScale, WinnerMesSprX[i]/56);
            }
            if(i == 1) {
                x = 288;
            } else if(i == 2) {
                x = 272.0f-((WinnerMesSprX[2]*mes->winScale)/2);
                if(WinnerMesSprX[i]/56 < 4) {
                    x -= ((4-(WinnerMesSprX[i]/56))*56)/2.0f;
                }
            } else {
                x = 304.0f+((WinnerMesSprX[3]*mes->winScale)/2);
                if(WinnerMesSprX[i]/56 < 4) {
                    x += ((4-(WinnerMesSprX[i]/56))*56)/2.0f;
                }
            }
        } else if(WinnerMesCharNum == 4) {
            if(WinnerMesSprX[i]/56 <= 4) {
                mes->winScale = 1;
            } else {
                mes->winScale = 4.0f/(WinnerMesSprX[i]/56);
            }
            if(i != 0) {
                FixWinnerNameOfs(mes->grpId[i], WinnerMesCharNo[i-1], mes->winScale, WinnerMesSprX[i]/56);
            }
            if((i%2) != 0) {
                x = 272.0f-((WinnerMesSprX[i]*mes->winScale)/2);
                if(WinnerMesSprX[i]/56 < 4) {
                    x -= ((4-(WinnerMesSprX[i]/56))*56)/2.0f;
                }
            } else {
                x = 304.0f+((WinnerMesSprX[i]*mes->winScale)/2);
                if(WinnerMesSprX[i]/56 < 4) {
                    x += ((4-(WinnerMesSprX[i]/56))*56)/2.0f;
                }
            }
        }
        WinnerMesNameX[i] = x;
        if(WinnerMesCharNum == 1) {
            switch(omcurovl) {
                case 0x08: case 0x0B: case 0x13: case 0x14: case 0x16:
                case 0x18: case 0x1A: case 0x1D: case 0x1F: case 0x22:
                case 0x24: case 0x25: case 0x35: case 0x38: case 0x39:
                case 0x41: case 0x42: case 0x44: case 0x46: case 0x48:
                case 0x4B:
                    WinnerMesNameY[i] = 123.0f;
                    break;

                case 0x0A: case 0x0C: case 0x0D: case 0x0E: case 0x0F:
                case 0x11: case 0x15: case 0x17: case 0x1E: case 0x20:
                case 0x26: case 0x27: case 0x37: case 0x3A: case 0x3B:
                case 0x3C: case 0x3D: case 0x3E: case 0x3F: case 0x40:
                case 0x43:
                    WinnerMesNameY[i] = 357.0f;
                    break;

                case 0x06: case 0x12:
                    WinnerMesNameY[i] = 357.0f;
                    WinnerMesNameY[i] += 50.0f;
                    break;

                case 0x09: {
                    s16 night = GwMgNightF;
                    if(night == 0) {
                        WinnerMesNameY[i] = 357.0f;
                    } else {
                        WinnerMesNameY[i] = 240.0f+WinnerMesOfsTbl[(GameMesLanguageNo == 3 || GameMesLanguageNo == 4) ? 0 : 1][WinnerMesCharNum-1][i][1];
                    }
                    break;
                }

                default:
                    WinnerMesNameY[i] = 240.0f+WinnerMesOfsTbl[(GameMesLanguageNo == 3 || GameMesLanguageNo == 4) ? 0 : 1][WinnerMesCharNum-1][i][1];
                    break;
            }
        } else if(WinnerMesCharNum == 2) {
            switch(omcurovl) {
                case 0x08: case 0x12: case 0x13: case 0x18: case 0x1A:
                case 0x28: case 0x31: case 0x32: case 0x49:
                    WinnerMesNameY[i] = 73.0f;
                    if(i == 0) {
                        WinnerMesNameY[0] += 70.0f;
                    }
                    break;

                case 0x06: case 0x0A: case 0x0C: case 0x0D: case 0x0E:
                case 0x15: case 0x2A: case 0x2B: case 0x2D: case 0x30:
                    WinnerMesNameY[i] = 407.0f;
                    if(i != 0) {
                        WinnerMesNameY[i] -= 70.0f;
                    }
                    break;

                case 0x09: {
                    s16 night = GwMgNightF;
                    if(night == 0) {
                        WinnerMesNameY[i] = 407.0f;
                        if(i != 0) {
                            WinnerMesNameY[i] -= 70.0f;
                        }
                    } else {
                        WinnerMesNameY[i] = 240.0f+WinnerMesOfsTbl[(GameMesLanguageNo == 3 || GameMesLanguageNo == 4) ? 0 : 1][WinnerMesCharNum-1][i][1];
                    }
                    break;
                }

                case 0x14: case 0x29:
                    WinnerMesNameY[i] = 73.0f;
                    if(i == 0) {
                        WinnerMesNameY[0] += 70.0f;
                    }
                    WinnerMesNameY[i] += 50.0f;
                    break;

                default:
                    WinnerMesNameY[i] = 240.0f+WinnerMesOfsTbl[(GameMesLanguageNo == 3 || GameMesLanguageNo == 4) ? 0 : 1][WinnerMesCharNum-1][i][1];
                    break;
            }
        } else {
            WinnerMesNameY[i] = 240.0f+WinnerMesOfsTbl[(GameMesLanguageNo == 3 || GameMesLanguageNo == 4) ? 0 : 1][WinnerMesCharNum-1][i][1];
        }
        x = WinnerMesNameX[i];
        y = WinnerMesNameY[i];
        if(WinnerMesCharNum == 1) {
            if(GameMesLanguageNo == 3 || GameMesLanguageNo == 4) {
                if(i == 1) {
                    y += WinnerMesXOfs;
                }
            } else {
                if(i == 1) {
                    x -= WinnerMesXOfs;
                }
            }
        } else if(WinnerMesCharNum == 2) {
            if(i == 1) {
                x -= WinnerMesXOfs;
            } else if(i == 2) {
                x += WinnerMesXOfs;
            }
        } else if(WinnerMesCharNum == 3) {
            if(GameMesLanguageNo == 3 || GameMesLanguageNo == 4) {
                if(i == 1) {
                    y += WinnerMesXOfs;
                } else if(i == 2) {
                    x -= WinnerMesXOfs;
                } else if(i == 3) {
                    x += WinnerMesXOfs;
                }
            } else {
                if(i == 1) {
                    y -= WinnerMesXOfs;
                } else if(i == 2) {
                    x -= WinnerMesXOfs;
                } else if(i == 3) {
                    x += WinnerMesXOfs;
                }
            }
        } else if(WinnerMesCharNum == 4) {
            if(i == 1) {
                x -= WinnerMesXOfs;
            } else if(i == 2) {
                x += WinnerMesXOfs;
            } else if(i == 3) {
                x -= WinnerMesXOfs;
            } else if(i == 4) {
                x += WinnerMesXOfs;
            }
        }
        HuSprGrpPosSet(mes->grpId[i], x, y);
        if(i == 0) {
            HuSprGrpScaleSet(mes->grpId[0], 0, 0);
        } else {
            HuSprGrpScaleSet(mes->grpId[i], mes->winScale, 1.0f);
        }
        for(charNo=0; charNo<WinnerMesSprNum[i]; charNo++) {
            HuSprPriSet(mes->grpId[i], charNo, 2);
        }
    }
    return TRUE;
}

BOOL GameMesMgWinExec(GAMEMES *mes)
{
    int i;
    float x;
    float y;
    float t;
    float scale;
    float zRot;

    mes->time += GameMesVWait;
    if(mes->time >= mes->timeMax && mes->mesMode == 0) {
        mes->stat |= GAMEMES_STAT_TIMEEND;
        mes->mesMode = -1;
        mes->angle = 0;
    }
    switch(mes->mesMode) {
        case 1:
            if(mes->time <= 20) {
                t = mes->time;
                zRot = (t/20.0f)*360.0f;
                HuSprGrpZRotSet(mes->grpId[0], zRot);
                scale = sind((t/20.0f)*90.0f);
                HuSprGrpScaleSet(mes->grpId[0], mes->scale.x*scale, mes->scale.y*scale);
                WinnerMesXOfs -= 30;
                for(i=1; i<mes->charNum; i++) {
                    x = WinnerMesNameX[i];
                    y = WinnerMesNameY[i];
                    if(WinnerMesCharNum == 1) {
                        if(GameMesLanguageNo == 3 || GameMesLanguageNo == 4) {
                            if(i == 1) {
                                y += WinnerMesXOfs;
                            }
                        } else {
                            if(i == 1) {
                                x -= WinnerMesXOfs;
                            }
                        }
                    } else if(WinnerMesCharNum == 2) {
                        if(i == 1) {
                            x -= WinnerMesXOfs;
                        } else if(i == 2) {
                            x += WinnerMesXOfs;
                        }
                    } else if(WinnerMesCharNum == 3) {
                        if(GameMesLanguageNo == 3 || GameMesLanguageNo == 4) {
                            if(i == 1) {
                                y += WinnerMesXOfs;
                            } else if(i == 2) {
                                x -= WinnerMesXOfs;
                            } else if(i == 3) {
                                x += WinnerMesXOfs;
                            }
                        } else {
                            if(i == 1) {
                                y -= WinnerMesXOfs;
                            } else if(i == 2) {
                                x -= WinnerMesXOfs;
                            } else if(i == 3) {
                                x += WinnerMesXOfs;
                            }
                        }
                    } else if(WinnerMesCharNum == 4) {
                        if(i == 1) {
                            x -= WinnerMesXOfs;
                        } else if(i == 2) {
                            x += WinnerMesXOfs;
                        } else if(i == 3) {
                            x -= WinnerMesXOfs;
                        } else if(i == 4) {
                            x += WinnerMesXOfs;
                        }
                    }
                    HuSprGrpPosSet(mes->grpId[i], x, y);
                }
            }
            if(mes->time == 140) {
                mes->mesMode = 0;
                mes->timeMax = 170;
            }
            break;

        case -1:
            mes->angle += GameMesVWait*0.1f;
            GameMesTPLvl = 1.0f-mes->angle;
            if(GameMesTPLvl <= 0) {
                GameMesTPLvl = 0;
                mes->stat |= GAMEMES_STAT_KILL;
            }
            for(i=0; i<mes->charNum; i++) {
                HuSprGrpTPLvlSet(mes->grpId[i], GameMesTPLvl);
            }
            break;

        default:
            break;
    }
    if(GameMesCloseF || (mes->stat & GAMEMES_STAT_KILL)) {
        GameMesSprKill(mes);
        return FALSE;
    }
    return TRUE;
}

static void FixWinnerNameOfs(int grpId, int charNo, float scale, int sprNum)
{
    int i;
    HUSPRITE *sp;
    BOOL flag;
    int half;

    flag = FALSE;
    if(GameMesLanguageNo == 1) {
        if(charNo == 9) {
            flag = TRUE;
        }
    } else if(GameMesLanguageNo == 2) {
        if(charNo == 8 || charNo == 9 || charNo == 14) {
            flag = TRUE;
        }
    } else if(GameMesLanguageNo == 3 || GameMesLanguageNo == 4 || GameMesLanguageNo == 5) {
        if(charNo == 9) {
            flag = TRUE;
        }
    }
    if(flag != FALSE) {
        half = (WinnerMesGrpInfo[WinnerMesLanguageNo + charNo * 6] & 0xF0) >> 4;
        for(i=0; i<half; i++) {
            sp = &HuSprData[HuSprGrpData[grpId].sprId[i]];
            sp->pos.x += (56.0f * scale) / 2.0f;
        }
        for(i=half; i<sprNum-1; i++) {
            sp = &HuSprData[HuSprGrpData[grpId].sprId[i]];
            sp->pos.x -= (56.0f * scale) / 2.0f;
        }
    }
}

static float GetGameMesWidth(GAMEMES *mes, int type, int no, float scale)
{
    float x;
    int width;

    width = 16;
    x = mes->pos.x + scale * (mes->scale.x * (((-mes->charNum * 56) / 2) + (no * 56) + 28));
    if(GameMesLanguageNo == 1) {
        switch(type) {
            case 0:
                if(no < 2) {
                    x += scale * ((width / 2) * mes->scale.x);
                } else {
                    x -= scale * ((width / 2) * mes->scale.x);
                }
                break;

            case 1:
                if(no < 2) {
                    x += scale * (width * mes->scale.x);
                }
                break;

            case 2:
                if(no == 0) {
                    x += scale * (mes->scale.x * ((width / 2) + (width * 2)));
                } else if(no == 1) {
                    x += scale * (mes->scale.x * ((width / 2) + (width * 2)));
                } else if(no == 2) {
                    x += scale * (mes->scale.x * (width + (width / 2)));
                } else if(no == 3) {
                    x += scale * ((width / 2) * mes->scale.x);
                } else if(no == 4) {
                    x -= scale * ((width / 2) * mes->scale.x);
                } else if(no == 5) {
                    x -= scale * (mes->scale.x * (width + (width / 2)));
                } else if(no == 6) {
                    x -= scale * (mes->scale.x * ((width / 2) + (width * 2)));
                }
                break;
        }
    }
    return x;
}
