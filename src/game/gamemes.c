/* Block math.h's extern inline sqrtf: its weak _half/_three statics would
 * prepend 16 bytes to .sdata2 that the original gamemes.o does not have. */
#define _MATH_H
#define M_PI 3.141592653589793
double sin(double);
double cos(double);
extern inline double fabs(double x)
{
   return __fabs(x);
}

#include "game/gamemes.h"
#include "game/process.h"
#include "game/data.h"
#include "game/gamework.h"
#include "game/armem.h"
#include "game/main.h"
#include "game/hu3d.h"
#include "game/audio.h"
#include "game/window.h"
#include "game/flag.h"
#include "game/wipe.h"
#include "game/pad.h"
#include "game/objdll.h"
#include "game/mgdata.h"

#define MSM_SE_CMN_04 3
#define MSM_SE_CMN_06 5
#define MSM_SE_CMN_07 6

typedef struct GameMesEntry_s {
    GAMEMESCREATE create;
    GAMEMESEXEC exec;
    HuVec2f pos;
    HuVec2f scale;
    int timeMax;
} GAMEMES_ENTRY;

GAMEMES GameMesData[GAMEMES_MAX];
s16 GameMesTime;
u8 GameMesCloseF;
static u8 GameMesDebugF;
unsigned int GameMesVWait;
static ANIMDATA *ScoreBoxMaskAnim;
static ANIMDATA *ScoreBoxAnim;
static BOOL pauseWaitF;
static BOOL pauseCancelF;
static BOOL pauseEnableF;
static HUPROCESS *pauseProc;
int GameMesLanguageNo;

OMOVL GameMesOvlPrev = DLL_NONE;

GAMEMES_ENTRY GameMesTbl[] = {
    { NULL, NULL, { 292, 240 }, { 1, 1 }, 60 },
    { GameMesTimerInit, GameMesTimerExec, { 292, 64 }, { 1, 1 }, 60 },
    { GameMesMg4PInit, GameMesMg4PExec, { 292, 240 }, { 1, 1 }, 180 },
    { GameMesMg2Vs2Init, GameMesMg2Vs2Exec, { 292, 240 }, { 1, 1 }, 180 },
    { GameMesMgWinInit, GameMesMgWinExec, { 292, 240 }, { 1, 1 }, 180 },
    { GameMesMg1Vs3Init, GameMesMg1Vs3Exec, { 292, 240 }, { 1, 1 }, 180 },
    { GameMesMgBattleInit, GameMesMgBattleExec, { 292, 240 }, { 1, 1 }, 180 },
    { GameMesMg4PInit, GameMesMg4PExec, { 292, 240 }, { 1, 1 }, 180 },
    { GameMesMgKoopaInit, GameMesMgKoopaExec, { 292, 240 }, { 1, 1 }, 180 },
    { GameMesMgRareInit, GameMesMgRareExec, { 292, 240 }, { 1, 1 }, 180 },
    { GameMesMgKettouInit, GameMesMgKettouExec, { 292, 240 }, { 1, 1 }, 180 },
    { GameMesMgDonkeyInit, GameMesMgDonkeyExec, { 292, 240 }, { 1, 1 }, 180 },
    { GameMesMgSdInit, GameMesMgSdExec, { 292, 240 }, { 1, 1 }, 180 },
    { GameMesMgDrawInit, GameMesMgDrawExec, { 292, 240 }, { 1, 1 }, 60 },
    { GameMesMgRecordInit, GameMesMgRecordExec, { 292, 240 }, { 1, 1 }, 180 },
    { NULL, NULL, { 292, 240 }, { 1, 1 }, 60 },
};

static s16 startMesTbl[MG_TYPE_MAX] = {
    GAMEMES_MES_MG_4P,
    GAMEMES_MES_MG_1VS3,
    GAMEMES_MES_MG_2VS2,
    GAMEMES_MES_MG_BATTLE,
    GAMEMES_MES_MG_KUPA,
    GAMEMES_MES_MG_LAST,
    GAMEMES_MES_MG_KETTOU,
    GAMEMES_MES_MG_DONKEY,
    GAMEMES_MES_MG_SD
};

static char asciiTbl[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
static char kanaTbl[] = "ｱｲｳｴｵｶｷｸｹｺｻｼｽｾｿﾀﾁﾂﾃﾄﾅﾆﾇﾈﾉﾊﾋﾌﾍﾎﾏﾐﾑﾒﾓﾔﾕﾖﾗﾘﾙﾚﾛﾜｦﾝｧｨｩｪｫｬｭｮｯｶｷｸｹｺｻｼｽｾｿﾀﾁﾂﾃﾄﾊﾋﾌﾍﾎﾊﾋﾌﾍﾎｰ";
static char numberTbl[] = "0123456789";
static char punctTbl[] = "!?.";
static int *fontBufs[5] = {};

static void CreateFontBuf(void);

void GameMesInit(void)
{
    GAMEMES *mes = &GameMesData[0];
    int i;
    for(i=GAMEMES_MAX; i; i--, mes++) {
        mes->mesNo = GAMEMES_MES_NULL;
        mes->buf = NULL;
    }
    GameMesCloseF = FALSE;
    GameMesDebugF = FALSE;
    GameMesTime = 30;
    HuAR_DVDtoARAM(DATA_gamemes);
    HuAR_DVDtoARAM(DATA_mgconst);
    while(HuARDMACheck());
    CreateFontBuf();
    GameMesOvlPrev = DLL_NONE;
    GameMesLanguageNo = GWLanguageGet();
    MgScoreBoxInit();
}

void *GameMesDataRead(unsigned int dataNum)
{
    return HuAR_ARAMtoMRAMFileRead(dataNum, HU_MEMNUM_OVL, HEAP_MODEL);
}

void GameMesExec(void)
{
    GAMEMES *mes;
    GAMEMES_ENTRY *mesTbl;
    int j;
    u8 stat;
    int i;
    BOOL result;
    GameMesVWait = HuSysVWaitGet(GameMesVWait);
    if(!Hu3DPauseF) {
        result = FALSE;
        stat = GAMEMES_STAT_NONE;
        mes = &GameMesData[0];
        for(i=0; i<GAMEMES_MAX; i++, mes++) {
            if(mes->stat == GAMEMES_STAT_NONE) {
                continue;
            }
            if(mes->exec) {
                result = mes->exec(mes);
            } else {
                mesTbl = &GameMesTbl[mes->mesNo];
                if(mes->mesNo != GAMEMES_MES_NULL && NULL != mesTbl->exec) {
                    result = mesTbl->exec(mes);
                }
            }
            if(result == FALSE) {
                mes->stat = GAMEMES_STAT_NONE;
                if(!GameMesDebugF) {
                    for(j=0; j<GAMEMES_MAX; j++) {
                        if(GameMesData[j].stat != GAMEMES_STAT_NONE) {
                            break;
                        }
                    }
                }
                if(!mes->buf) {
                    HuMemDirectFree(mes->buf);
                    mes->buf = NULL;
                }
            }
            stat |= mes->stat;
        }
        if(stat == GAMEMES_STAT_NONE || (stat & GAMEMES_STAT_TIMEEND)) {
            if(GameMesTime > 0) {
                GameMesTime -= GameMesVWait;
            }
        }
    }
}

static GAMEMESID CreateGameMes(s16 mesNo, va_list args)
{
    GAMEMES *mes = &GameMesData[0];
    GAMEMES_ENTRY *mesTbl = &GameMesTbl[(u8)mesNo];
    int i;
    int mesId;

    for(mesId=0; mesId<GAMEMES_MAX; mesId++, mes++) {
        if(mes->stat == GAMEMES_STAT_NONE) {
            break;
        }
    }
    if(mesId >= GAMEMES_MAX) {
        return GAMEMES_ID_NONE;
    }
    mes->stat |= GAMEMES_STAT_EXIST;
    if(mes->buf) {
        HuMemDirectFree(mes->buf);
    }
    mes->buf = NULL;
    mes->mesNo = mesNo;
    mes->time = 0;
    mes->pos.x = mesTbl->pos.x;
    mes->pos.y = mesTbl->pos.y;
    mes->scale.x = mesTbl->scale.x;
    mes->scale.y = mesTbl->scale.y;
    mes->unk18 = 0;
    mes->color.g = 255;
    mes->timeMax = mesTbl->timeMax;
    mes->timerVal = mes->subMode = mes->work = mes->charNum = 0;
    mes->angle = mes->winScale = 0;
    mes->dispMode = mes->dispValue = 0;
    for(i=0; i<GAMEMES_SPRMAX; i++) {
        mes->sprId[i] = mes->grpId[i] = HUSPR_NONE;
    }
    if(NULL != mesTbl->create) {
        i = mesTbl->create(mes, args);
        if(i == FALSE) {
            mes->stat = GAMEMES_STAT_NONE;
            return GAMEMES_ID_NONE;
        }
    }
    GameMesTime = 30;
    return mesId;
}

GAMEMESID GameMesCreate(s16 mesNo, ...)
{
    GAMEMESID id;
    va_list args;
    va_start(args, mesNo);
    GameMesLanguageNo = GWLanguageGet();
    if(mesNo == GAMEMES_MES_MG) {
        if(GwSystem.mgNo == 0xffff) {
            mesNo = GAMEMES_MES_MG_4P;
        } else {
            mesNo = startMesTbl[MgDataTbl[GwSystem.mgNo].type];
        }
    }
    id = CreateGameMes(mesNo, args);
    va_end(args);
    return id;
}

GAMEMES *GameMesGet(GAMEMESID id)
{
    if(id >= 0 && id < GAMEMES_MAX) {
        return &GameMesData[id];
    }
    return NULL;
}

u8 GameMesStatGet(GAMEMESID id)
{
    GAMEMES *mes;
    u8 stat;
    stat = GAMEMES_STAT_NONE;
    if(id < 0) {
        int i;
        mes = &GameMesData[0];
        for(i=GAMEMES_MAX; i; i--, mes++) {
            stat |= mes->stat;
        }
    } else {
        if(id < GAMEMES_MAX) {
            stat = GameMesData[id].stat;
        }
    }
    return stat;
}

void GameMesPosSet(GAMEMESID id, float posX, float posY)
{
    if(id < 0) {
        return;
    }
    if(id >= GAMEMES_MAX) {
        return;
    }
    GameMesData[id].pos.x = posX;
    GameMesData[id].pos.y = posY;
}

void GameMesDispSet(GAMEMESID id, s16 mode, s16 value)
{
    if(id < 0) {
        return;
    }
    if(id >= GAMEMES_MAX) {
        return;
    }
    GameMesData[id].dispMode = mode;
    GameMesData[id].dispValue = value;
}

void GameMesKill(GAMEMESID id)
{
    if(id < 0) {
        return;
    }
    if(id >= GAMEMES_MAX) {
        return;
    }
    if(GameMesData[id].stat == GAMEMES_STAT_NONE) {
        return;
    }
    GameMesData[id].stat = GAMEMES_STAT_KILL;
}

void GameMesClose(void)
{
    GameMesCloseF = TRUE;
    GameMesExec();
    GameMesCloseF = FALSE;
    GameMesStub();
    GameMesDebugF = FALSE;
}

BOOL GameMesKillCheck(void)
{
    u8 stat = GameMesStatGet(GAMEMES_ID_NONE);
    if(stat == GAMEMES_STAT_NONE || (stat & (GAMEMES_STAT_TIMEEND|GAMEMES_STAT_KILL))) {
        if(GameMesTime <= 0 || (stat & GAMEMES_STAT_KILL)) {
            return TRUE;
        }
    }
    return FALSE;
}

void GameMesStub(void)
{

}

void GameMesSprKill(GAMEMES *mes)
{
    int i;
    for(i=0; i<GAMEMES_SPRMAX; i++) {
        if(mes->grpId[i] >= 0) {
            HuSprGrpKill(mes->grpId[i]);
        }
        if(mes->sprId[i] >= 0) {
            HuSprKill(mes->sprId[i]);
        }
    }
}

#define TIMER_SUBMODE_OFF 1
#define TIMER_SUBMODE_SHOW 2
#define TIMER_SUBMODE_DIGITUP 3

BOOL GameMesTimerInit(GAMEMES *mes, va_list args)
{
    HUSPR_GROUPID gid;
    ANIMDATA *anim;
    HUSPRID sprid;

    int maxTime, posX, posY;
    s16 i;
    maxTime = va_arg(args, int);
    posX = va_arg(args, int);
    posY = va_arg(args, int);
    if(maxTime <= 0 && maxTime > 99) {
        maxTime = 99;
    }
    mes->timerVal = maxTime;
    if(posX >= 0) {
        mes->pos.x = posX;
    }
    if(posY >= 0) {
        mes->pos.y = posY;
    }
    mes->work = 0;
    mes->subMode = TIMER_SUBMODE_SHOW;
    mes->angle = 0;
    gid = HuSprGrpCreate(3);
    mes->grpId[0] = gid;
    HuSprGrpScaleSet(gid, mes->scale.x, mes->scale.y);
    anim = HuSprAnimRead(GameMesDataRead(GAMEMES_ANM_timerDigit));
    posX = 11;
    for(i=0; i<2; posX -= 22, i++) {
        sprid = HuSprCreate(anim, 5, 0);
        HuSprGrpMemberSet(gid, i, sprid);
        HuSprSpeedSet(gid, i, 0);
        HuSprPosSet(gid, i, posX, 1);
        HuSprColorSet(gid, i, 112, 233, 255);
    }
    anim = HuSprAnimRead(GameMesDataRead(GAMEMES_ANM_timerBack));
    sprid = HuSprCreate(anim, 7, 0);
    HuSprGrpMemberSet(gid, 2, sprid);
    HuSprPosSet(gid, 2, 0, 0);
    HuSprTPLvlSet(gid, 2, 1.0f);
    HuSprGrpPosSet(gid, -100, -100);
    return TRUE;
}

BOOL GameMesTimerExec(GAMEMES *mes)
{
    HUSPR_GROUPID gid = mes->grpId[0];
    s16 i;
    u8 digit[2];
    float scale, tpLvl;
    if(mes->dispMode != 0 && mes->subMode != GAMEMES_SUBMODE_NULL) {
        switch(mes->dispMode) {
            case GAMEMES_DISP_SET:
                switch(mes->dispValue) {
                    case -1:
                        mes->stat |= GAMEMES_STAT_TIMEEND;
                        mes->subMode = GAMEMES_SUBMODE_NULL;
                        mes->angle = 0;
                        break;

                    case 0:
                        mes->subMode = TIMER_SUBMODE_SHOW;
                        mes->angle = 0;
                        break;

                    case 1:
                        mes->subMode = TIMER_SUBMODE_DIGITUP;
                        mes->angle = 0;
                        break;
                }
                mes->dispMode = GAMEMES_DISP_NONE;
                break;

            case GAMEMES_DISP_UPDATE:
                if(mes->dispValue < 0 && !(mes->stat & GAMEMES_STAT_TIMEEND)) {
                    mes->stat |= GAMEMES_STAT_TIMEEND;
                    mes->subMode = GAMEMES_SUBMODE_NULL;
                    mes->angle = 0;
                } else {
                    if(mes->dispValue > 99) {
                        mes->timerVal = 99;
                    } else {
                        if(mes->timerVal != mes->dispValue) {
                            mes->timerVal = mes->dispValue;
                            if(mes->dispValue <= 5) {
                                HuAudFXPlay(MSM_SE_CMN_07);
                                mes->subMode = TIMER_SUBMODE_DIGITUP;
                                mes->angle = 0;
                                HuSprColorSet(gid, 0, 255, 112, 160);
                                HuSprColorSet(gid, 1, 255, 112, 160);
                            } else {
                                HuSprColorSet(gid, 0, 112, 233, 255);
                                HuSprColorSet(gid, 1, 112, 233, 255);
                            }
                        }
                    }
                }
                mes->dispMode = GAMEMES_DISP_NONE;
                break;

            case GAMEMES_DISP_HIDE:
                for(i=0; i<2; i++) {
                    HuSprDispOn(gid, i);
                }
                mes->subMode = TIMER_SUBMODE_DIGITUP;
                mes->dispMode = GAMEMES_DISP_NONE;
                break;

            default:
                mes->dispMode = GAMEMES_DISP_NONE;
                break;
        }
    }
    if(mes->subMode == TIMER_SUBMODE_OFF) {
        return TRUE;
    }
    if(mes->timerVal > 99) {
        digit[0] = digit[1] = 9;
    } else {
        int value = mes->timerVal/10;
        digit[1] = value;
        digit[0] = mes->timerVal-(value*10);
    }
    HuSprGrpPosSet(gid, mes->pos.x, mes->pos.y);
    HuSprGrpScaleSet(gid, mes->scale.x, mes->scale.y);
    for(i=0; i<2; i++) {
        HuSprBankSet(gid, i, digit[i]);
    }
    if(mes->subMode != GAMEMES_SUBMODE_NONE) {
        switch(mes->subMode) {
            case TIMER_SUBMODE_SHOW:
            {
                float scaleX, scaleY;
                scale = fabs(((5*HuSin(mes->angle))+1)-(HuSin(130)*5));
                scaleX = mes->scale.x*scale;
                scaleY = mes->scale.y*scale;
                mes->angle += 5.0f*GameMesVWait;
                if(mes->angle > 130) {
                    mes->subMode = GAMEMES_SUBMODE_NONE;
                } else {
                    HuSprGrpScaleSet(gid, scaleX, scaleY);
                }
            }
                break;

            case TIMER_SUBMODE_DIGITUP:
                scale = 1+HuSin(mes->angle);
                tpLvl = 1-(0.5*HuSin(mes->angle));
                mes->angle += 18.0f*GameMesVWait;
                if(mes->angle > 180) {
                    mes->subMode = GAMEMES_SUBMODE_NONE;
                    scale = 1;
                    tpLvl = 1;
                }
                for(i=0; i<2; i++) {
                    HuSprScaleSet(gid, i, scale, scale);
                    HuSprTPLvlSet(gid, i, tpLvl);
                }
                break;

            case GAMEMES_SUBMODE_NULL:
                HuSprGrpScaleSet(gid, mes->scale.x, mes->scale.y);
                for(i=0; i<2; i++) {
                    HuSprScaleSet(gid, i, 1, 1);
                    HuSprTPLvlSet(gid, i, 1);
                }
                mes->angle++;
                if(mes->angle < 60) {
                    break;
                }
                tpLvl = 1.0-((mes->angle-60)/20);
                if(tpLvl <= 0) {
                    tpLvl = 0;
                    mes->subMode = GAMEMES_SUBMODE_NONE;
                    mes->stat |= GAMEMES_STAT_KILL;
                }
                for(i=0; i<3; i++) {
                    HuSprTPLvlSet(gid, i, tpLvl);
                }
                break;
        }
    }
    if(GameMesCloseF || (mes->stat & GAMEMES_STAT_KILL)) {
        GameMesSprKill(mes);
        return FALSE;
    }
    return TRUE;
}

#undef TIMER_SUBMODE_OFF
#undef TIMER_SUBMODE_SHOW
#undef TIMER_SUBMODE_DIGITUP


static void CreateFontBuf(void)
{
    if(!fontBufs[0]) {
        fontBufs[0] = HuMemDirectMalloc(HEAP_HEAP, strlen(asciiTbl)*sizeof(int));
        memset(fontBufs[0], 0, strlen(asciiTbl)*sizeof(int));
    }
    if(!fontBufs[1]) {
        fontBufs[1] = HuMemDirectMalloc(HEAP_HEAP, strlen(kanaTbl)*sizeof(int));
        memset(fontBufs[1], 0, strlen(kanaTbl)*sizeof(int));
    }
    if(!fontBufs[2]) {
        fontBufs[2] = HuMemDirectMalloc(HEAP_HEAP, strlen(kanaTbl)*sizeof(int));
        memset(fontBufs[2], 0, strlen(kanaTbl)*sizeof(int));
    }
    if(!fontBufs[3]) {
        fontBufs[3] = HuMemDirectMalloc(HEAP_HEAP, strlen(numberTbl)*sizeof(int));
        memset(fontBufs[3], 0, strlen(numberTbl)*sizeof(int));
    }
    if(!fontBufs[4]) {
        fontBufs[4] = HuMemDirectMalloc(HEAP_HEAP, strlen(punctTbl)*sizeof(int));
        memset(fontBufs[4], 0, strlen(punctTbl)*sizeof(int));
    }
}

static ANIMDATA *CreateFontChar(char *str, s16 flag);

#define STR_CHAR_MAX 100

int GameMesStrCreate(GAMEMES *mes, char *str, s16 flag)
{
    s16 charNum;
    s16 x;
    s16 i;
    s16 strId;
    char *s;
    ANIMDATA **charAnimTbl;
    s16 *charX;
    s16 spaceNum;
    HUSPR_GROUPID gid;

    for(strId=0; strId<GAMEMES_SPRMAX; strId++) {
        if(mes->grpId[strId] == HUSPR_GROUP_NONE) {
            break;
        }
    }
    if(strId == GAMEMES_SPRMAX) {
        return GAMEMES_STR_NONE;
    }
    charAnimTbl = HuMemDirectMalloc(HEAP_HEAP, STR_CHAR_MAX*sizeof(ANIMDATA *));
    charX = HuMemDirectMalloc(HEAP_HEAP, STR_CHAR_MAX*sizeof(s16));

    for(s=str, x=0, charNum=spaceNum=0; *s; s++) {
        if(*s == ' ') {
            x += 56;
            spaceNum++;
        } else {
            charAnimTbl[charNum] = CreateFontChar(s, flag);
            if(charAnimTbl[charNum]) {
                charX[charNum] = x;
                x += 56;
                charNum++;
            }
        }
    }
    gid = HuSprGrpCreate(charNum+spaceNum);
    mes->grpId[strId] = gid;
    x = (x/2)-28;
    for(i=0; i<charNum; i++) {
        HUSPRID sprid = HuSprCreate(charAnimTbl[i], 5, 0);
        HuSprGrpMemberSet(gid, i, sprid);
        HuSprPosSet(gid, i, charX[i]-x, 0);
    }
    mes->charNum = charNum;
    HuMemDirectFree(charAnimTbl);
    HuMemDirectFree(charX);
    return strId;
}

int GameMesStrCopy(GAMEMES *mes, s16 id)
{
    s16 strId;
    for(strId=0; strId<GAMEMES_SPRMAX; strId++) {
        if(mes->grpId[strId] == HUSPR_GROUP_NONE) {
            break;
        }
    }
    if(strId == GAMEMES_SPRMAX) {
        return GAMEMES_STR_NONE;
    }
    mes->grpId[strId] = HuSprGrpCopy(mes->grpId[id]);
    return strId;
}

static ANIMDATA *CreateFontChar(char *str, s16 flag)
{
    char *tbl;
    s16 i;
    unsigned int dataNum;
    char c;
    c = *str;
    if(c == 222 || c == 223) {
        return NULL;
    }
    for(i=0,tbl=asciiTbl; *tbl; i++, tbl++) {
        if(*tbl == c) {
            dataNum = GAMEMES_ANM_letterAUpper+i;
            return HuSprAnimRead(GameMesDataRead(dataNum));
        }
    }
    for(i=0,tbl=numberTbl; *tbl; i++, tbl++) {
        if(*tbl == c) {
            dataNum = GAMEMES_ANM_number0+i;
            return HuSprAnimRead(GameMesDataRead(dataNum));
        }
    }
    for(i=0,tbl=punctTbl; *tbl; i++, tbl++) {
        if(*tbl == c) {
            dataNum = GAMEMES_ANM_punctExcl+i;
            return HuSprAnimRead(GameMesDataRead(dataNum));
        }
    }
    return NULL;
}

static void PauseExec(void);

void GameMesPauseCreate(void)
{
    GameMesPauseEnable(FALSE);
    HuWinInit(1);
    pauseProc = HuPrcCreate(PauseExec, 100, 4096, 0);
    HuPrcSetStat(pauseProc, HU_PRC_STAT_PAUSE_ON|HU_PRC_STAT_UPAUSE_ON);
    pauseEnableF = TRUE;
    pauseCancelF = FALSE;
    pauseWaitF = FALSE;
}

static void PauseExec(void)
{
    GAMEMES mes;
    s16 charNo[GW_PLAYER_MAX][GW_PLAYER_MAX];
    s16 charNum[GW_PLAYER_MAX];
    HUWINID winId[3] = { HUWIN_NONE, HUWIN_NONE, HUWIN_NONE };

    s16 i;
    u32 *instMes;
    s16 j;
    s16 mgNo;
    float time;

    HuAudFXPlay(MSM_SE_CMN_06);
    for(i=0; i<GAMEMES_SPRMAX; i++) {
        mes.sprId[i] = mes.grpId[i] = HUSPR_NONE;
    }
    GameMesStrWinCreate(&mes, 0x420010);
    for(i=0; i<mes.charNum; i++) {
        HuSprPriSet(mes.grpId[0], i, 0);
    }
    for(i=0; i<GW_PLAYER_MAX; i++) {
        charNum[i] = 0;
    }
    for(i=0; i<GW_PLAYER_MAX; i++) {
        charNo[GwPlayerConf[i].grpNo][charNum[GwPlayerConf[i].grpNo]] = GwPlayerConf[i].charNo;
        charNum[GwPlayerConf[i].grpNo]++;
    }
    mgNo = MgNoGet(omcurovl);
    if(_CheckFlag(FLAG_INST_DECA)) {
        instMes = MgDataTbl[mgNo].instMes[2];
    } else {
        s16 night = GwMgNightF;
        if(night == 0) {
            instMes = MgDataTbl[mgNo].instMes[0];
        } else {
            instMes = MgDataTbl[mgNo].instMes[1];
        }
        if(MgDataTbl[mgNo].ovl == 0x53) {
            int subGame = GwSystem.subGameNo;
            if(subGame == 1) {
                instMes = MgDataTbl[mgNo].instMes[1];
            }
        }
    }
    if(!instMes[1] && !instMes[2]) {
        for(i=1; i<=20; i++) {
            time = HuSin(i*(90.0f/20.0f));
            HuSprGrpPosSet(mes.grpId[0], 288, (290*time)-50);
            HuPrcVSleep();
        }
    } else {
        if(MgPauseExitF && !_CheckFlag(FLAG_MG_PRACTICE)) {
            winId[2] = HuWinExCreateFrame(-10000, 400, 412, 42, HUWIN_SPEAKER_NULL, 0);
            HuWinPriSet(winId[2], 5);
            HuWinDispOn(winId[2]);
            HuWinMesSpeedSet(winId[2], 0);
            HuWinAttrSet(winId[2], HUWIN_ATTR_ALIGN_CENTER);
            HuWinMesSet(winId[2], 0x420009);
        }
        if(instMes[2]) {
            s16 insertMesNo;
            winId[0] = HuWinExCreateFrame(-10000, 140, 412, 120, HUWIN_SPEAKER_NULL, 0);
            HuWinPriSet(winId[0], 5);
            HuWinDispOn(winId[0]);
            HuWinMesSpeedSet(winId[0], 0);
            HuWinMesSet(winId[0], instMes[1]);
            winId[1] = HuWinExCreateFrame(-10000, 276, 412, 120, HUWIN_SPEAKER_NULL, 0);
            HuWinPriSet(winId[1], 5);
            HuWinDispOn(winId[1]);
            HuWinMesSpeedSet(winId[1], 0);
            HuWinMesSet(winId[1], instMes[2]);
            for(i=insertMesNo=0; i<GW_PLAYER_MAX; i++) {
                for(j=0; j<charNum[i]; j++) {
                    HuWinInsertMesSet(winId[0], charNo[i][j], (int)insertMesNo);
                    HuWinInsertMesSet(winId[1], charNo[i][j], (int)insertMesNo);
                    insertMesNo++;
                }
            }
            for(i=1; i<=20; i++) {
                time = HuSin(i*(90.0f/20.0f));
                HuSprGrpPosSet(mes.grpId[0], 288, (150*time)-50);
                HuWinPosSet(winId[0], (482*time)-400, 140);
                HuWinPosSet(winId[1], (-318*time)+400, 272);
                if(winId[2] != HUWIN_NONE) {
                    HuWinPosSet(winId[2], 82, (100*(1.0-time))+404);
                }
                HuPrcVSleep();
            }
        } else {
            winId[0] = HuWinExCreateFrame(-10000, 170, 412, 120, HUWIN_SPEAKER_NULL, 0);
            HuWinPriSet(winId[0], 5);
            HuWinDispOn(winId[0]);
            HuWinMesSpeedSet(winId[0], 0);
            HuWinMesSet(winId[0], instMes[1]);
            for(i=1; i<=20; i++) {
                time = HuSin(i*(90.0f/20.0f));
                HuSprGrpPosSet(mes.grpId[0], 288, (150*time)-50);
                HuWinPosSet(winId[0], (482*time)-400, 170);
                if(winId[2] != HUWIN_NONE) {
                    HuWinPosSet(winId[2], (-318*time)+400, 404);
                }
                HuPrcVSleep();
            }
        }
    }
    GameMesPauseEnable(TRUE);
    pauseWaitF = TRUE;
    while(!pauseCancelF) {
        HuPrcVSleep();
    }
    pauseWaitF = FALSE;
    if(winId[0] == HUWIN_NONE && winId[1] == HUWIN_NONE) {
        for(i=1; i<=10; i++) {
            time = HuCos(i*(90.0f/10.0f));
            HuSprGrpPosSet(mes.grpId[0], 288, (290*time)-50);
            HuPrcVSleep();
        }
    } else if(winId[1] != HUWIN_NONE) {
        for(i=1; i<=10; i++) {
            time = HuCos(i*(90.0f/10.0f));
            HuSprGrpPosSet(mes.grpId[0], 288, (150*time)-50);
            HuWinPosSet(winId[0], (482*time)-400, 140);
            HuWinPosSet(winId[1], (-318*time)+400, 272);
            if(winId[2] != HUWIN_NONE) {
                HuWinPosSet(winId[2], 82, (100*(1.0-time))+404);
            }
            HuPrcVSleep();
        }
    } else {
        for(i=1; i<=10; i++) {
            time = HuCos(i*(90.0f/10.0f));
            HuSprGrpPosSet(mes.grpId[0], 288, (150*time)-50);
            HuWinPosSet(winId[0], (482*time)-400, 170);
            if(winId[2] != HUWIN_NONE) {
                HuWinPosSet(winId[2], (-318*time)+400, 404);
            }
            HuPrcVSleep();
        }
    }
    omSysPauseCtrl(FALSE);
    if(winId[0] != HUWIN_NONE) {
        HuWinKill(winId[0]);
    }
    if(winId[1] != HUWIN_NONE) {
        HuWinKill(winId[1]);
    }
    if(winId[2] != HUWIN_NONE) {
        HuWinKill(winId[2]);
    }
    HuSprGrpKill(mes.grpId[0]);
    pauseProc = FALSE;
    pauseEnableF = FALSE;
    HuPrcEnd();
    while(1) {
        HuPrcVSleep();
    }
}

void GameMesPauseCancel(void)
{
    pauseCancelF = TRUE;
}

void GameMesPauseEnable(BOOL enable)
{
    if(_CheckFlag(FLAG_MG_PAUSE_OFF) || _CheckFlag(FLAGNUM(FLAG_GROUP_SYSTEM, 5))) {
        return;
    }
    omSysPauseEnable(enable);
}

#define PRACTICE_POS_TOP 0
#define PRACTICE_POS_BOTTOM 1
#define PRACTICE_POS_CENTER 2
#define PRACTICE_POS_NUM 5

typedef struct Practice_s {
    s16 ovl;
    s16 posNo;
} PRACTICE;

static PRACTICE practiceTbl[] = {
    6, 1, 7, 1, 8, 2, 9, 1, 10, 0, 11, 2, 12, 1, 13, 3, 14, 1, 15, 1,
    16, 0, 17, 2, 18, 1, 19, 2, 20, 1, 21, 1, 22, 4, 23, 2, 24, 1, 25, 1,
    26, 0, 27, 4, 28, 3, 29, 4, 30, 1, 31, 1, 32, 1, 33, 1, 34, 1, 35, 1,
    36, 1, 37, 1, 38, 0, 39, 0, 40, 1, 41, 1, 42, 0, 43, 1, 44, 1, 45, 2,
    46, 2, 47, 1, 48, 2, 49, 1, 50, 1, 51, 1, 52, 0, 53, 1, 54, 2, 55, 2,
    56, 1, 57, 1, 58, 4, 59, 1, 60, 1, 61, 2, 62, 1, 63, 1, 64, 1, 65, 1,
    65, 1, 66, 4, 67, 1, 68, 1, 69, 1, 70, 0, 71, 0, 72, 1, 73, 1, 74, 1,
    75, 1, 76, 0, 77, 0, 78, 1, 79, 1, 80, 0, 81, 1, 82, 0, 83, 0, 84, 4,
    85, 1, 86, 1, 87, 4,
    DLL_NONE, 0
};

static void PracticeExec(void);

void GameMesPracticeCreate(void)
{
    HUPROCESS *parent = HuPrcCurrentGet();
    s16 i;
    if(!_CheckFlag(FLAG_MG_PRACTICE)) {
        return;
    }
    wipeFadeInF = FALSE;
    for(i=0; practiceTbl[i].ovl != DLL_NONE; i++) {
        if(omcurovl == practiceTbl[i].ovl) {
            break;
        }
    }
    if(practiceTbl[i].ovl != DLL_NONE) {
        HuPrcSetStat(HuPrcChildCreate(PracticeExec, 10, 8192, 0, parent), HU_PRC_STAT_PAUSE_ON|HU_PRC_STAT_UPAUSE_ON);
    }
}

static void PracticeExec(void)
{
    float angle;
    s16 id;
    HUSPR_GROUPID gid;
    MGDATA *mgP;
    BOOL exitF;
    PRACTICE *practiceP;
    HUSPRID sprid;
    ANIMDATA *anim;

    static float posTbl[PRACTICE_POS_NUM] = {
        53,
        424,
        240,
        384,
        102
    };

    angle = 0;
    for(id=0; practiceTbl[id].ovl != DLL_NONE; id++) {
        if(omcurovl == practiceTbl[id].ovl) {
            break;
        }
    }
    practiceP = &practiceTbl[id];
    id = MgNoGet(omcurovl);
    if(id != -1) {
        mgP = &MgDataTbl[id];
    } else {
        mgP = &MgDataTbl[0];
    }
    gid = HuSprGrpCreate(1);
    HuSprGrpPosSet(gid, 0, 0);
    anim = HuSprAnimRead(GameMesDataRead(GAMEMES_ANM_practiceExit));
    sprid = HuSprCreate(anim, 0, 0);
    HuSprGrpMemberSet(gid, 0, sprid);
    HuSprPosSet(gid, 0, 288, posTbl[practiceP->posNo]);
    do {
        if(!wipeFadeInF || WipeCheckIn() || omPauseChk()) {
            HuSprAttrSet(gid, 0, 4);
            HuPrcVSleep();
            continue;
        }
        HuSprAttrReset(gid, 0, 4);
        exitF = FALSE;
        switch(mgP->type) {
            case MG_TYPE_4P:
            case MG_TYPE_1VS3:
            case MG_TYPE_2VS2:
            case MG_TYPE_BATTLE:
            case MG_TYPE_KUPA:
            case MG_TYPE_LAST:
            case MG_TYPE_DONKEY:
                for(id=0; id<GW_PLAYER_MAX; id++) {
                    if((HuPadBtnDown[GwPlayerConf[id].padNo] & PAD_TRIGGER_Z) && GwPlayerConf[id].type == GW_PLAYER_TYPE_MAN) {
                        break;
                    }
                }
                if(id != GW_PLAYER_MAX) {
                    exitF = TRUE;
                }
                break;

            case MG_TYPE_KETTOU:
                for(id=0; id<GW_PLAYER_MAX; id++) {
                    if((HuPadBtnDown[GwPlayerConf[id].padNo] & PAD_TRIGGER_Z) && GwPlayerConf[id].type == GW_PLAYER_TYPE_MAN && GwPlayerConf[id].grpNo < 2) {
                        break;
                    }
                }
                if(id != GW_PLAYER_MAX) {
                    exitF = TRUE;
                }
                break;

            default:
                break;
        }
        if(exitF) {
            break;
        }
        HuSprTPLvlSet(gid, 0, 0.7f);
        angle += 2;
        HuPrcVSleep();
    } while(1);
    HuSprAttrSet(gid, 0, 4);
    omSysExitReq = TRUE;
    HuPrcEnd();
    while(1) {
        HuPrcVSleep();
    }
}

void GameMesEnd(void)
{
}

void GameMesExitCheck(OMOBJ *obj)
{
    MgExitReq = FALSE;
    if(obj->work[0] == 0) {
        if(MgNoGet(omcurovl) == -1) {
            omDelObjEx(HuPrcCurrentGet(), obj);
            return;
        } else {
            obj->work[0]++;
        }
    }
    if(!omPauseChk() || _CheckFlag(FLAG_MG_PRACTICE) || !pauseWaitF) {
        return;
    }
    if(HuPadBtnDown[omDBGSysKeyObj->work[1]] & PAD_TRIGGER_Z) {
        HuAudFXPlay(MSM_SE_CMN_04);
        GameMesPauseCancel();
        omSysPauseCtrl(FALSE);
        omSysExitReq = TRUE;
        MgExitReq = TRUE;
        omDelObjEx(HuPrcCurrentGet(), obj);
    }
}

static void ScoreBoxBGMake(ANIMDATA *anim, s16 num);

static void ScoreBoxDraw(HUSPRITE *sp)
{
    ANIMBMP *bmp = sp->bg->bmp;
    float sizeX = bmp->sizeX*16;
    float sizeY = bmp->sizeY*16;
    Mtx modelview;
    HuVec2f vtx[4];
    GXColor color;
    GXSetScissor(sp->scissorX, sp->scissorY, sp->scissorW, sp->scissorH);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    color.r = sp->r;
    color.g = sp->g;
    color.b = sp->b;
    color.a = sp->a;
    GXSetTevColor(GX_TEVREG0, color);
    GXSetNumTevStages(3);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_C0, GX_CC_TEXC, GX_CC_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD0, GX_TEXMAP2, GX_COLOR0A0);
    GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_CPREV, GX_CC_TEXC, GX_CC_TEXA, GX_CC_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_APREV, GX_CA_TEXA, GX_CA_TEXA, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevOrder(GX_TEVSTAGE2, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevColorIn(GX_TEVSTAGE2, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_CPREV);
    GXSetTevColorOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE2, GX_CA_ZERO, GX_CA_A0, GX_CA_APREV, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT0, GX_DF_CLAMP, GX_AF_SPOT);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
    GXSetAlphaCompare(GX_GEQUAL, 1, GX_AOP_AND, GX_GEQUAL, 1);
    GXSetZCompLoc(GX_TRUE);
    PSMTXScale(modelview, sp->scale.x, sp->scale.y, 1.0f);
    mtxTransCat(modelview, sp->pos.x, sp->pos.y, 0);
    PSMTXConcat(*sp->groupMtx, modelview, modelview);
    GXLoadPosMtxImm(modelview, GX_PNMTX0);
    HuSprTexLoad(ScoreBoxMaskAnim, 0, GX_TEXMAP0, GX_CLAMP, GX_CLAMP, GX_NEAR);
    HuSprTexLoad(sp->bg, 0, GX_TEXMAP1, GX_CLAMP, GX_CLAMP, GX_NEAR);
    HuSprTexLoad(ScoreBoxAnim, 0, GX_TEXMAP2, GX_CLAMP, GX_CLAMP, GX_NEAR);
    GXSetNumIndStages(2);
    GXSetTexCoordScaleManually(GX_TEXCOORD0, GX_TRUE, sizeX, sizeY);
    GXSetIndTexOrder(GX_INDTEXSTAGE0, GX_TEXCOORD0, GX_TEXMAP1);
    GXSetIndTexCoordScale(GX_INDTEXSTAGE0, GX_ITS_16, GX_ITS_16);
    GXSetTevIndTile(GX_TEVSTAGE0, GX_INDTEXSTAGE0, 16, 16, 16, 16, GX_ITF_4, GX_ITM_0, GX_ITB_NONE, GX_ITBA_OFF);
    GXSetIndTexOrder(GX_INDTEXSTAGE1, GX_TEXCOORD0, GX_TEXMAP1);
    GXSetIndTexCoordScale(GX_INDTEXSTAGE1, GX_ITS_16, GX_ITS_16);
    GXSetTevIndTile(GX_TEVSTAGE1, GX_INDTEXSTAGE1, 16, 16, 16, 16, GX_ITF_4, GX_ITM_0, GX_ITB_NONE, GX_ITBA_OFF);
    vtx[0].x = (-sizeX/2)/2;
    vtx[0].y = (-sizeY/2)/2;
    vtx[1].x = (sizeX/2)/2;
    vtx[1].y = (-sizeY/2)/2;
    vtx[2].x = (sizeX/2)/2;
    vtx[2].y = (sizeY/2)/2;
    vtx[3].x = (-sizeX/2)/2;
    vtx[3].y = (sizeY/2)/2;
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3f32(vtx[0].x, vtx[0].y, 0);
    GXTexCoord2f32(0, 0);
    GXPosition3f32(vtx[1].x, vtx[1].y, 0);
    GXTexCoord2f32(1, 0);
    GXPosition3f32(vtx[2].x, vtx[2].y, 0);
    GXTexCoord2f32(1, 1);
    GXPosition3f32(vtx[3].x, vtx[3].y, 0);
    GXTexCoord2f32(0, 1);
    GXEnd();
    GXSetNumIndStages(0);
    GXSetTevDirect(GX_TEVSTAGE0);
    GXSetTexCoordScaleManually(GX_TEXCOORD0, GX_FALSE, 0, 0);
    GXSetTevDirect(GX_TEVSTAGE1);
    GXSetTexCoordScaleManually(GX_TEXCOORD1, GX_FALSE, 0, 0);
}

static void ScoreBoxBGMake(ANIMDATA *anim, s16 num)
{
    s16 i, j;
    u8 *bmpData;
    s16 blockW;
    s16 w;
    s16 blockH;
    s16 h;
    s16 k;

    w = anim->bmp->sizeX;
    h = anim->bmp->sizeY;
    blockW = (w+7) & 0xF8;
    blockH = (h+3) & 0xFC;
    bmpData = anim->bmp->data = HuMemDirectMallocNum(HEAP_HEAP, blockW*blockH, HU_MEMNUM_OVL);
    for(i=0; i<h; i++) {
        if(i < 3) {
            for(j=0; j<w; j++) {
                if(j == 0) {
                    bmpData[(j&7)+((j>>3)<<5)+(i>>2)*(blockW*4)+(i&3)*8] = i+0xA0;
                } else if(j == 1) {
                    bmpData[(j&7)+((j>>3)<<5)+(i>>2)*(blockW*4)+(i&3)*8] = i+0xB0;
                } else if(j == 2) {
                    bmpData[(j&7)+((j>>3)<<5)+(i>>2)*(blockW*4)+(i&3)*8] = i+0xC0;
                } else if(j == w-3) {
                    bmpData[(j&7)+((j>>3)<<5)+(i>>2)*(blockW*4)+(i&3)*8] = i+0x70;
                } else if(j == w-2) {
                    bmpData[(j&7)+((j>>3)<<5)+(i>>2)*(blockW*4)+(i&3)*8] = i+0x80;
                } else if(j == w-1) {
                    bmpData[(j&7)+((j>>3)<<5)+(i>>2)*(blockW*4)+(i&3)*8] = i+0x90;
                } else {
                    bmpData[(j&7)+((j>>3)<<5)+(i>>2)*(blockW*4)+(i&3)*8] = i+0x60;
                }
            }
        } else if(i >= h-3) {
            for(j=0; j<w; j++) {
                s16 v = i-(h-6);
                if(j == 0) {
                    bmpData[(j&7)+((j>>3)<<5)+(i>>2)*(blockW*4)+(i&3)*8] = v+0xA0;
                } else if(j == 1) {
                    bmpData[(j&7)+((j>>3)<<5)+(i>>2)*(blockW*4)+(i&3)*8] = v+0xB0;
                } else if(j == 2) {
                    bmpData[(j&7)+((j>>3)<<5)+(i>>2)*(blockW*4)+(i&3)*8] = v+0xC0;
                } else if(j == w-3) {
                    bmpData[(j&7)+((j>>3)<<5)+(i>>2)*(blockW*4)+(i&3)*8] = v+0x70;
                } else if(j == w-2) {
                    bmpData[(j&7)+((j>>3)<<5)+(i>>2)*(blockW*4)+(i&3)*8] = v+0x80;
                } else if(j == w-1) {
                    bmpData[(j&7)+((j>>3)<<5)+(i>>2)*(blockW*4)+(i&3)*8] = v+0x90;
                } else {
                    bmpData[(j&7)+((j>>3)<<5)+(i>>2)*(blockW*4)+(i&3)*8] = v+0x60;
                }
            }
        } else {
            for(j=0; j<w; j++) {
                if(j == 0) {
                    bmpData[(j&7)+((j>>3)<<5)+(i>>2)*(blockW*4)+(i&3)*8] = 0xD0;
                } else if(j == w-1) {
                    bmpData[(j&7)+((j>>3)<<5)+(i>>2)*(blockW*4)+(i&3)*8] = 0xF1;
                } else {
                    bmpData[(j&7)+((j>>3)<<5)+(i>>2)*(blockW*4)+(i&3)*8] = 0xD2;
                }
            }
        }
    }
    if(num) {
        for(k=0; k<num; k++) {
            for(i=0; i<6; i++) {
                for(j=0; j<6; j++) {
                    bmpData[((j+k*5)&7)+(((j+k*5)>>3)<<5)+(i>>2)*(blockW*4)+(i&3)*8] = i+(j*16);
                }
            }
        }
    }
    DCStoreRangeNoSync(anim->bmp->data, blockW*blockH);
}

void MgScoreBoxInit(void)
{
    ScoreBoxMaskAnim = ScoreBoxAnim = NULL;
}

static HUSPR_GROUPID ScoreBoxCreate(s16 sizeX, s16 sizeY, s16 num, u8 *chars);

HUSPR_GROUPID MgScoreBoxCreate(s16 sizeX, s16 sizeY)
{
    return ScoreBoxCreate(sizeX, sizeY, 0, NULL);
}

HUSPR_GROUPID MgScoreBoxCreateChar(s16 sizeX, s16 sizeY, s16 charId)
{
    u8 chars[1];
    chars[0] = charId;
    return ScoreBoxCreate(sizeX, sizeY, 1, chars);
}

HUSPR_GROUPID MgScoreBoxCreateCharMulti(s16 sizeX, s16 sizeY, s16 num, u8 *chars)
{
    return ScoreBoxCreate(sizeX, sizeY, num, chars);
}

static HUSPR_GROUPID ScoreBoxCreate(s16 sizeX, s16 sizeY, s16 num, u8 *chars)
{
    HUSPR_GROUPID gid;
    HUSPRID sprid;
    ANIMDATA *anim;
    s16 i;
    sizeX = (sizeX+7) & 0xFFF8;
    sizeY = (sizeY+7) & 0xFFF8;
    if(sizeX < 0x30) {
        sizeX = 0x30;
    }
    if(sizeY < 0x30) {
        sizeY = 0x30;
    }
    gid = HuSprGrpCreate(num+1);
    if(!ScoreBoxMaskAnim) {
        ScoreBoxMaskAnim = HuSprAnimDataRead(DATANUM(DATA_mgconst, 0x2f));
    }
    if(!ScoreBoxAnim) {
        ScoreBoxAnim = HuSprAnimDataRead(DATANUM(DATA_mgconst, 0x2e));
    }
    sprid = HuSprFuncCreate(ScoreBoxDraw, 100);
    HuSprGrpMemberSet(gid, 0, sprid);
    HuSprData[sprid].bg = anim = HuSprAnimMake(sizeX/8, sizeY/8, ANIM_BMP_IA4);
    ScoreBoxBGMake(anim, num);
    HuSprColorSet(gid, 0, 64, 64, 64);
    HuSprTPLvlSet(gid, 0, 1.0f);
    for(i=0; i<num; i++) {
        anim = HuSprAnimRead(HuDataSelHeapReadNum(DATA_mgconst+chars[i], HU_MEMNUM_OVL, HEAP_MODEL));
        sprid = HuSprCreate(anim, 100, 0);
        HuSprGrpMemberSet(gid, i+1, sprid);
        HuSprPosSet(gid, i+1, -(sizeX/2)+24+(i*40), 0);
        HuSprTPLvlSet(gid, i+1, 1.0f);
    }
    return gid;
}

void MgScoreBoxKill(HUSPR_GROUPID gid)
{
    HuSprGrpKill(gid);
}

void MgScoreBoxColorSet(HUSPR_GROUPID gid, u8 r, u8 g, u8 b)
{
    HuSprColorSet(gid, 0, r, g, b);
}

void MgScoreBoxTPLvlSet(HUSPR_GROUPID gid, float tpLvl)
{
    HuSprGrpTPLvlSet(gid, tpLvl);
}

void MgScoreBoxPosSet(HUSPR_GROUPID gid, float posX, float posY)
{
    HuSprGrpPosSet(gid, posX, posY);
}

void MgScoreBoxDispSet(HUSPR_GROUPID gid, BOOL disp)
{
    if(disp) {
        HuSprGrpAttrReset(gid, 4);
    } else {
        HuSprGrpAttrSet(gid, 4);
    }
}

#define FONT_NUMBER(c) (GAMEMES_ANM_number0+((c)-'0'))
#define FONT_LETTERUPPER(c) (GAMEMES_ANM_letterAUpper+((c)-'A'))
#define FONT_LETTERLOWER(c) (GAMEMES_ANM_letterALower+((c)-'a'))

static unsigned int fontFileTbl[] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    FONT_NUMBER('0'),
    FONT_NUMBER('1'),
    FONT_NUMBER('2'),
    FONT_NUMBER('3'),
    FONT_NUMBER('4'),
    FONT_NUMBER('5'),
    FONT_NUMBER('6'),
    FONT_NUMBER('7'),
    FONT_NUMBER('8'),
    FONT_NUMBER('9'),
    GAMEMES_ANM_number0,
    GAMEMES_ANM_number0,
    GAMEMES_ANM_number0,
    GAMEMES_ANM_fontDash,
    GAMEMES_ANM_number0,
    GAMEMES_ANM_number0,
    GAMEMES_ANM_number0,
    FONT_LETTERUPPER('A'),
    FONT_LETTERUPPER('B'),
    FONT_LETTERUPPER('C'),
    FONT_LETTERUPPER('D'),
    FONT_LETTERUPPER('E'),
    FONT_LETTERUPPER('F'),
    FONT_LETTERUPPER('G'),
    FONT_LETTERUPPER('H'),
    FONT_LETTERUPPER('I'),
    FONT_LETTERUPPER('J'),
    FONT_LETTERUPPER('K'),
    FONT_LETTERUPPER('L'),
    FONT_LETTERUPPER('M'),
    FONT_LETTERUPPER('N'),
    FONT_LETTERUPPER('O'),
    FONT_LETTERUPPER('P'),
    FONT_LETTERUPPER('Q'),
    FONT_LETTERUPPER('R'),
    FONT_LETTERUPPER('S'),
    FONT_LETTERUPPER('T'),
    FONT_LETTERUPPER('U'),
    FONT_LETTERUPPER('V'),
    FONT_LETTERUPPER('W'),
    FONT_LETTERUPPER('X'),
    FONT_LETTERUPPER('Y'),
    FONT_LETTERUPPER('Z'),
    FONT_LETTERUPPER('O'),
    GAMEMES_ANM_fontApos,
    FONT_LETTERUPPER('O'),
    FONT_LETTERUPPER('O'),
    FONT_LETTERUPPER('O'),
    FONT_LETTERUPPER('O'),
    FONT_LETTERLOWER('a'),
    FONT_LETTERLOWER('b'),
    FONT_LETTERLOWER('c'),
    FONT_LETTERLOWER('d'),
    FONT_LETTERLOWER('e'),
    FONT_LETTERLOWER('f'),
    FONT_LETTERLOWER('g'),
    FONT_LETTERLOWER('h'),
    FONT_LETTERLOWER('i'),
    FONT_LETTERLOWER('j'),
    FONT_LETTERLOWER('k'),
    FONT_LETTERLOWER('l'),
    FONT_LETTERLOWER('m'),
    FONT_LETTERLOWER('n'),
    FONT_LETTERLOWER('o'),
    FONT_LETTERLOWER('p'),
    FONT_LETTERLOWER('q'),
    FONT_LETTERLOWER('r'),
    FONT_LETTERLOWER('s'),
    FONT_LETTERLOWER('t'),
    FONT_LETTERLOWER('u'),
    FONT_LETTERLOWER('v'),
    FONT_LETTERLOWER('w'),
    FONT_LETTERLOWER('x'),
    FONT_LETTERLOWER('y'),
    FONT_LETTERLOWER('z'),
    GAMEMES_ANM_number0,
    GAMEMES_ANM_number0,
    GAMEMES_ANM_number0,
    GAMEMES_ANM_fontTilde,
    GAMEMES_ANM_number0,
    GAMEMES_ANM_number0,
    GAMEMES_ANM_number0,
    GAMEMES_ANM_number0,
    GAMEMES_ANM_number0,
    GAMEMES_ANM_fontDash,
    GAMEMES_ANM_punctDot,
    GAMEMES_ANM_number0,
    GAMEMES_ANM_number0,
    GAMEMES_ANM_number0,
    GAMEMES_ANM_number0,
    GAMEMES_ANM_number0,
    GAMEMES_ANM_number0,
    GAMEMES_ANM_number0,
    GAMEMES_ANM_number0,
    GAMEMES_ANM_number0,
    GAMEMES_ANM_number0,
    GAMEMES_ANM_number0,
    GAMEMES_ANM_number0,
    GAMEMES_ANM_number0,
    GAMEMES_ANM_number0,
    GAMEMES_ANM_number0,
    GAMEMES_ANM_number0,
    GAMEMES_ANM_number0,
    GAMEMES_ANM_number0,
    GAMEMES_ANM_number0,
    GAMEMES_ANM_number0,
    GAMEMES_ANM_number0,
    GAMEMES_ANM_number0,
    GAMEMES_ANM_number0,
    GAMEMES_ANM_number0,
    GAMEMES_ANM_number0,
    GAMEMES_ANM_number0,
    GAMEMES_ANM_number0,
    GAMEMES_ANM_number0,
    GAMEMES_ANM_number0,
    GAMEMES_ANM_number0,
    GAMEMES_ANM_number0,
    GAMEMES_ANM_number0,
    GAMEMES_ANM_number0,
    GAMEMES_ANM_number0,
    GAMEMES_ANM_number0,
    GAMEMES_ANM_number0,
    GAMEMES_ANM_number0,
    GAMEMES_ANM_number0,
    GAMEMES_ANM_number0,
    GAMEMES_ANM_number0,
    GAMEMES_ANM_number0,
    GAMEMES_ANM_number0,
    GAMEMES_ANM_number0,
    GAMEMES_ANM_number0,
    GAMEMES_ANM_number0,
    GAMEMES_ANM_number0,
    GAMEMES_ANM_number0,
    GAMEMES_ANM_number0,
    GAMEMES_ANM_number0,
    GAMEMES_ANM_number0,
    GAMEMES_ANM_number0,
    GAMEMES_ANM_number0,
    GAMEMES_ANM_number0,
    GAMEMES_ANM_number0,
    GAMEMES_ANM_number0,
    GAMEMES_ANM_number0,
    GAMEMES_ANM_number0,
    GAMEMES_ANM_number0,
    GAMEMES_ANM_number0,
    GAMEMES_ANM_number0,
    GAMEMES_ANM_punctExcl,
    GAMEMES_ANM_punctHatena,
    GAMEMES_ANM_number0,
    GAMEMES_ANM_number0
};

GAMEMESID GameMesStrWinCreate(GAMEMES *mes, u32 winMessId)
{
    char *mesPtr;
    s16 x;
    s16 *charY;
    unsigned int *fileTbl;
    s16 charNum;
    s16 i;
    GAMEMESID strId;
    unsigned int dataNum;
    s16 *charX;
    ANIMDATA **charAnimTbl;
    HUSPR_GROUPID gid;
    HUSPRID sprid;
    HuWinInit(0);
    for(strId=0; strId<GAMEMES_SPRMAX; strId++) {
        if(mes->grpId[strId] == HUSPR_GROUP_NONE) {
            break;
        }
    }
    if(strId == GAMEMES_SPRMAX) {
        return GAMEMES_STR_NONE;
    }
    fileTbl = fontFileTbl;
    charAnimTbl = HuMemDirectMalloc(HEAP_HEAP, STR_CHAR_MAX*sizeof(ANIMDATA *));
    charX = HuMemDirectMalloc(HEAP_HEAP, STR_CHAR_MAX*sizeof(s16));
    charY = HuMemDirectMalloc(HEAP_HEAP, STR_CHAR_MAX*sizeof(s16));
    for(mesPtr=HuWinMesPtrGet(winMessId), x=0, charNum=0; *mesPtr; mesPtr++) {
        if(mesPtr[0] < 48 && mesPtr[0] != ' ' && mesPtr[0] != 16) {
            continue;
        }
        dataNum = fileTbl[mesPtr[0]];
        charAnimTbl[charNum] = HuSprAnimRead(GameMesDataRead(dataNum & 0x7fffffff));
        charX[charNum] = x;
        if(mesPtr[0] == ' ' || mesPtr[0] == 16) {
            charY[charNum] = 0;
            x += 28;
        } else if(mesPtr[0] >= 'a' && mesPtr[0] <= 'z') {
            charY[charNum] = 4;
            x += 42;
        } else if(mesPtr[0] == 194 || mesPtr[0] == 195) {
            charY[charNum] = 0;
            x += 56;
        } else if(mesPtr[0] == '\\') {
            charY[charNum] = 0;
            x += 56;
        } else if(dataNum & 0x80000000) {
            charY[charNum] = -2;
            x += 56;
        } else {
            charY[charNum] = 0;
            x += 56;
        }
        charNum++;
    }
    gid = HuSprGrpCreate(charNum);
    mes->grpId[strId] = gid;
    x = (x/2)-28;
    for(i=0; i<charNum; i++) {
        sprid = HuSprCreate(charAnimTbl[i], 0, 0);
        HuSprGrpMemberSet(gid, i, sprid);
        HuSprPosSet(gid, i, charX[i]-x, charY[i]);
    }
    mes->charNum = charNum;
    HuMemDirectFree(charAnimTbl);
    HuMemDirectFree(charX);
    HuMemDirectFree(charY);
    return strId;
}
