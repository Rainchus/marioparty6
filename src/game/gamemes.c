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
#include "game/mgdata.h"

#include "messnum/mg_inst.h"
#include "datanum/mgconst.h"

typedef struct GameMesEntry_s {
    GAMEMES_CREATE create;
    GAMEMES_EXEC exec;
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
static BOOL PauseWaitF;
static BOOL PauseCancelF;
static BOOL PauseEnableF;
static HUPROCESS *PauseProc;
int GameMesLanguageNo;

OMOVL GameMesOvlPrev = DLL_NONE;

GAMEMES_ENTRY GameMesTbl[] = {
    {
        NULL,
        NULL,
        { 292, 240 },
        { 1, 1 },
        60
    },
    {
        GameMesTimerInit,
        GameMesTimerExec,
        { 292, 64 },
        { 1, 1 },
        60
    },
    {
        GameMesMg4PInit,
        GameMesMg4PExec,
        { 292, 240 },
        { 1, 1 },
        180
    },
    {
        GameMesMg2Vs2Init,
        GameMesMg2Vs2Exec,
        { 292, 240 },
        { 1, 1 },
        180
    },
    {
        GameMesMgWinInit,
        GameMesMgWinExec,
        { 292, 240 },
        { 1, 1 },
        180
    },
    {
        GameMesMg1Vs3Init,
        GameMesMg1Vs3Exec,
        { 292, 240 },
        { 1, 1 },
        180
    },
    {
        GameMesMgBattleInit,
        GameMesMgBattleExec,
        { 292, 240 },
        { 1, 1 },
        180
    },
    {
        GameMesMg4PInit,
        GameMesMg4PExec,
        { 292, 240 },
        { 1, 1 },
        180
    },
    {
        GameMesMgKoopaInit,
        GameMesMgKoopaExec,
        { 292, 240 },
        { 1, 1 },
        180
    },
    {
        GameMesMgLastInit,
        GameMesMgLastExec,
        { 292, 240 },
        { 1, 1 },
        180
    },
    {
        GameMesMgKettouInit,
        GameMesMgKettouExec,
        { 292, 240 },
        { 1, 1 },
        180
    },
    {
        GameMesMgDonkeyInit,
        GameMesMgDonkeyExec,
        { 292, 240 },
        { 1, 1 },
        180
    },
    {
        GameMesMg678Init,
        GameMesMg678Exec,
        { 292, 240 },
        { 1, 1 },
        180
    },
    {
        GameMesMgDrawInit,
        GameMesMgDrawExec,
        { 292, 240 },
        { 1, 1 },
        60
    },
    {
        GameMesMgRecordInit,
        GameMesMgRecordExec,
        { 292, 240 },
        { 1, 1 },
        180
    },
    {
        NULL,
        NULL,
        { 292, 240 },
        { 1, 1 },
        60
    },
};

static s16 StartMesTbl[MG_TYPE_MAX] = {
    GAMEMES_TYPE_MG_4P,
    GAMEMES_TYPE_MG_1VS3,
    GAMEMES_TYPE_MG_2VS2,
    GAMEMES_TYPE_MG_BATTLE,
    GAMEMES_TYPE_MG_KOOPA,
    GAMEMES_TYPE_MG_LAST,
    GAMEMES_TYPE_MG_KETTOU,
    GAMEMES_TYPE_MG_DONKEY,
    GAMEMES_TYPE_MG_678
};

static char AsciiTbl[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
static char KanaTbl[] = "ｱｲｳｴｵｶｷｸｹｺｻｼｽｾｿﾀﾁﾂﾃﾄﾅﾆﾇﾈﾉﾊﾋﾌﾍﾎﾏﾐﾑﾒﾓﾔﾕﾖﾗﾘﾙﾚﾛﾜｦﾝｧｨｩｪｫｬｭｮｯｶｷｸｹｺｻｼｽｾｿﾀﾁﾂﾃﾄﾊﾋﾌﾍﾎﾊﾋﾌﾍﾎｰ";
static char NumberTbl[] = "0123456789";
static char PunctTbl[] = "!?.";
static int *FontBufs[5] = {};

static void CreateFontBuf(void);

void GameMesInit(void)
{
    GAMEMES *mes = &GameMesData[0];
    int i;
    for(i=GAMEMES_MAX; i; i--, mes++) {
        mes->mesNo = GAMEMES_TYPE_NULL;
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
                if(mes->mesNo != GAMEMES_TYPE_NULL && NULL != mesTbl->exec) {
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
    for(i=0; i<GAMEMES_SPR_MAX; i++) {
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
    if(mesNo == GAMEMES_TYPE_MG) {
        if(GwSystem.mgNo == GW_MGNO_NONE) {
            mesNo = GAMEMES_TYPE_MG_4P;
        } else {
            mesNo = StartMesTbl[MgDataTbl[GwSystem.mgNo].type];
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
    } else {
        return NULL;
    }
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
    for(i=0; i<GAMEMES_SPR_MAX; i++) {
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
    anim = HuSprAnimRead(GameMesDataRead(GAMEMES_ANM_digit));
    posX = 11;
    for(i=0; i<2; posX -= 22, i++) {
        sprid = HuSprCreate(anim, 5, 0);
        HuSprGrpMemberSet(gid, i, sprid);
        HuSprSpeedSet(gid, i, 0);
        HuSprPosSet(gid, i, posX, 1);
        HuSprColorSet(gid, i, 112, 233, 255);
    }
    anim = HuSprAnimRead(GameMesDataRead(GAMEMES_ANM_timer_new));
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
    if(!FontBufs[0]) {
        FontBufs[0] = HuMemDirectMalloc(HEAP_HEAP, strlen(AsciiTbl)*sizeof(int));
        memset(FontBufs[0], 0, strlen(AsciiTbl)*sizeof(int));
    }
    if(!FontBufs[1]) {
        FontBufs[1] = HuMemDirectMalloc(HEAP_HEAP, strlen(KanaTbl)*sizeof(int));
        memset(FontBufs[1], 0, strlen(KanaTbl)*sizeof(int));
    }
    if(!FontBufs[2]) {
        FontBufs[2] = HuMemDirectMalloc(HEAP_HEAP, strlen(KanaTbl)*sizeof(int));
        memset(FontBufs[2], 0, strlen(KanaTbl)*sizeof(int));
    }
    if(!FontBufs[3]) {
        FontBufs[3] = HuMemDirectMalloc(HEAP_HEAP, strlen(NumberTbl)*sizeof(int));
        memset(FontBufs[3], 0, strlen(NumberTbl)*sizeof(int));
    }
    if(!FontBufs[4]) {
        FontBufs[4] = HuMemDirectMalloc(HEAP_HEAP, strlen(PunctTbl)*sizeof(int));
        memset(FontBufs[4], 0, strlen(PunctTbl)*sizeof(int));
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
    
    for(strId=0; strId<GAMEMES_SPR_MAX; strId++) {
        if(mes->grpId[strId] == HUSPR_GROUP_NONE) {
            break;
        }
    }
    if(strId == GAMEMES_SPR_MAX) {
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
    for(strId=0; strId<GAMEMES_SPR_MAX; strId++) {
        if(mes->grpId[strId] == HUSPR_GROUP_NONE) {
            break;
        }
    }
    if(strId == GAMEMES_SPR_MAX) {
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
    for(i=0,tbl=AsciiTbl; *tbl; i++, tbl++) {
        if(*tbl == c) {
            dataNum = GAMEMES_ANM_upper_a+i;
            return HuSprAnimRead(GameMesDataRead(dataNum));
        }
    }
    for(i=0,tbl=NumberTbl; *tbl; i++, tbl++) {
        if(*tbl == c) {
            dataNum = GAMEMES_ANM_num_0+i;
            return HuSprAnimRead(GameMesDataRead(dataNum));
        }
    }
    for(i=0,tbl=PunctTbl; *tbl; i++, tbl++) {
        if(*tbl == c) {
            dataNum = GAMEMES_ANM_punct_exclamation+i;
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
    PauseProc = HuPrcCreate(PauseExec, 100, 4096, 0);
    HuPrcSetStat(PauseProc, HU_PRC_STAT_PAUSE_ON|HU_PRC_STAT_UPAUSE_ON);
    PauseEnableF = TRUE;
    PauseCancelF = FALSE;
    PauseWaitF = FALSE;
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
    for(i=0; i<GAMEMES_SPR_MAX; i++) {
        mes.sprId[i] = mes.grpId[i] = HUSPR_NONE;
    }
    GameMesStrWinCreate(&mes, MG_INST_SYS_PAUSE);
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
        instMes = &MgDataTbl[mgNo].instMes[MG_INST_TYPE_DECATHLON][0];
    } else {
        if(GWMgTimeGet() == FALSE) {
            instMes = &MgDataTbl[mgNo].instMes[MG_INST_TYPE_DAY][0];
        } else {
            instMes = &MgDataTbl[mgNo].instMes[MG_INST_TYPE_NIGHT][0];
        }
        //Multiplayer instructions for m678
        if(MgDataTbl[mgNo].ovl == DLL_m678dll && GWSubGameNoGet() == 1) {
            instMes = &MgDataTbl[mgNo].instMes[MG_INST_TYPE_NIGHT][0];
        }
    }
    if(instMes[MG_INST_PAGE_CONTROL] == MG_INST_MESS_NULL && instMes[MG_INST_PAGE_CONTROL2] == MG_INST_MESS_NULL) {
        for(i=1; i<=20; i++) {
            time = HuSin(i*(90.0f/20.0f));
            HuSprGrpPosSet(mes.grpId[0], 288, (290*time)-50);
            HuPrcVSleep();
        }
    } else {
        if(MgPauseExitF && !_CheckFlag(FLAG_MG_PRACTICE)) {
            winId[2] = HuWinExCreateFrame(-10000, 400, 412, 42, HUWIN_SPEAKER_NULL, HUWIN_FRAME_DEFAULT);
            HuWinPriSet(winId[2], 5);
            HuWinDispOn(winId[2]);
            HuWinMesSpeedSet(winId[2], 0);
            HuWinAttrSet(winId[2], HUWIN_ATTR_ALIGN_CENTER);
            HuWinMesSet(winId[2], MG_INST_SYS_EXIT);
        }
        if(instMes[MG_INST_PAGE_CONTROL2] != MG_INST_MESS_NULL) {
            s16 insertMesNo;
            winId[0] = HuWinExCreateFrame(-10000, 140, 412, 120, HUWIN_SPEAKER_NULL, HUWIN_FRAME_DEFAULT);
            HuWinPriSet(winId[0], 5);
            HuWinDispOn(winId[0]);
            HuWinMesSpeedSet(winId[0], 0);
            HuWinMesSet(winId[0], instMes[MG_INST_PAGE_CONTROL]);
            winId[1] = HuWinExCreateFrame(-10000, 276, 412, 120, HUWIN_SPEAKER_NULL, HUWIN_FRAME_DEFAULT);
            HuWinPriSet(winId[1], 5);
            HuWinDispOn(winId[1]);
            HuWinMesSpeedSet(winId[1], 0);
            HuWinMesSet(winId[1], instMes[MG_INST_PAGE_CONTROL2]);
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
            winId[0] = HuWinExCreateFrame(-10000, 170, 412, 120, HUWIN_SPEAKER_NULL, HUWIN_FRAME_DEFAULT);
            HuWinPriSet(winId[0], 5);
            HuWinDispOn(winId[0]);
            HuWinMesSpeedSet(winId[0], 0);
            HuWinMesSet(winId[0], instMes[MG_INST_PAGE_CONTROL]);
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
    PauseWaitF = TRUE;
    while(!PauseCancelF) {
        HuPrcVSleep();
    }
    PauseWaitF = FALSE;
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
    PauseProc = FALSE;
    PauseEnableF = FALSE;
    HuPrcEnd();
    while(1) {
        HuPrcVSleep();
    }
}

void GameMesPauseCancel(void)
{
    PauseCancelF = TRUE;
}

void GameMesPauseEnable(BOOL enable)
{
    if(_CheckFlag(FLAG_MG_PAUSE_OFF) || _CheckFlag(FLAG_SYSTEM_5)) {
        return;
    }
    omSysPauseEnable(enable);
}

enum {
    PRACTICE_POS_TOP, //0
    PRACTICE_POS_BOTTOM, //1
    PRACTICE_POS_CENTER, //2
    PRACTICE_POS_BOTTOM_FAR, //3
    PRACTICE_POS_TOP_FAR, //4
    PRACTICE_POS_MAX //5
};

typedef struct Practice_s {
    s16 ovl;
    s16 posNo;
} PRACTICE;

static PRACTICE PracticeTbl[] = {
    DLL_m601dll, PRACTICE_POS_BOTTOM,
    DLL_m602dll, PRACTICE_POS_BOTTOM,
    DLL_m603dll, PRACTICE_POS_CENTER,
    DLL_m604dll, PRACTICE_POS_BOTTOM,
    DLL_m605dll, PRACTICE_POS_TOP,
    DLL_m606dll, PRACTICE_POS_CENTER,
    DLL_m607dll, PRACTICE_POS_BOTTOM,
    DLL_m608dll, PRACTICE_POS_BOTTOM_FAR,
    DLL_m609dll, PRACTICE_POS_BOTTOM,
    DLL_m610dll, PRACTICE_POS_BOTTOM,
    DLL_m611dll, PRACTICE_POS_TOP,
    DLL_m612dll, PRACTICE_POS_CENTER,
    DLL_m613dll, PRACTICE_POS_BOTTOM,
    DLL_m614dll, PRACTICE_POS_CENTER,
    DLL_m615dll, PRACTICE_POS_BOTTOM,
    DLL_m616dll, PRACTICE_POS_BOTTOM,
    DLL_m617dll, PRACTICE_POS_TOP_FAR,
    DLL_m618dll, PRACTICE_POS_CENTER,
    DLL_m619dll, PRACTICE_POS_BOTTOM,
    DLL_m620dll, PRACTICE_POS_BOTTOM,
    DLL_m621dll, PRACTICE_POS_TOP,
    DLL_m622dll, PRACTICE_POS_TOP_FAR,
    DLL_m623dll, PRACTICE_POS_BOTTOM_FAR,
    DLL_m624dll, PRACTICE_POS_TOP_FAR,
    DLL_m625dll, PRACTICE_POS_BOTTOM,
    DLL_m626dll, PRACTICE_POS_BOTTOM,
    DLL_m627dll, PRACTICE_POS_BOTTOM,
    DLL_m628dll, PRACTICE_POS_BOTTOM,
    DLL_m629dll, PRACTICE_POS_BOTTOM,
    DLL_m630dll, PRACTICE_POS_BOTTOM,
    DLL_m631dll, PRACTICE_POS_BOTTOM,
    DLL_m632dll, PRACTICE_POS_BOTTOM,
    DLL_m633dll, PRACTICE_POS_TOP,
    DLL_m634dll, PRACTICE_POS_TOP,
    DLL_m635dll, PRACTICE_POS_BOTTOM,
    DLL_m636dll, PRACTICE_POS_BOTTOM,
    DLL_m637dll, PRACTICE_POS_TOP,
    DLL_m638dll, PRACTICE_POS_BOTTOM,
    DLL_m639dll, PRACTICE_POS_BOTTOM,
    DLL_m640dll, PRACTICE_POS_CENTER,
    DLL_m641dll, PRACTICE_POS_CENTER,
    DLL_m642dll, PRACTICE_POS_BOTTOM,
    DLL_m643dll, PRACTICE_POS_CENTER,
    DLL_m644dll, PRACTICE_POS_BOTTOM,
    DLL_m645dll, PRACTICE_POS_BOTTOM,
    DLL_m646dll, PRACTICE_POS_BOTTOM,
    DLL_m647dll, PRACTICE_POS_TOP,
    DLL_m648dll, PRACTICE_POS_BOTTOM,
    DLL_m649dll, PRACTICE_POS_CENTER,
    DLL_m650dll, PRACTICE_POS_CENTER,
    DLL_m651dll, PRACTICE_POS_BOTTOM,
    DLL_m652dll, PRACTICE_POS_BOTTOM,
    DLL_m653dll, PRACTICE_POS_TOP_FAR,
    DLL_m654dll, PRACTICE_POS_BOTTOM,
    DLL_m655dll, PRACTICE_POS_BOTTOM,
    DLL_m656dll, PRACTICE_POS_CENTER,
    DLL_m657dll, PRACTICE_POS_BOTTOM,
    DLL_m658dll, PRACTICE_POS_BOTTOM,
    DLL_m659dll, PRACTICE_POS_BOTTOM,
    DLL_m660dll, PRACTICE_POS_BOTTOM,
    DLL_m660dll, PRACTICE_POS_BOTTOM,
    DLL_m661dll, PRACTICE_POS_TOP_FAR,
    DLL_m662dll, PRACTICE_POS_BOTTOM,
    DLL_m663dll, PRACTICE_POS_BOTTOM,
    DLL_m664dll, PRACTICE_POS_BOTTOM,
    DLL_m665dll, PRACTICE_POS_TOP,
    DLL_m666dll, PRACTICE_POS_TOP,
    DLL_m667dll, PRACTICE_POS_BOTTOM,
    DLL_m668dll, PRACTICE_POS_BOTTOM,
    DLL_m669dll, PRACTICE_POS_BOTTOM,
    DLL_m670dll, PRACTICE_POS_BOTTOM,
    DLL_m671dll, PRACTICE_POS_TOP,
    DLL_m672dll, PRACTICE_POS_TOP,
    DLL_m673dll, PRACTICE_POS_BOTTOM,
    DLL_m674dll, PRACTICE_POS_BOTTOM,
    DLL_m675dll, PRACTICE_POS_TOP,
    DLL_m676dll, PRACTICE_POS_BOTTOM,
    DLL_m677dll, PRACTICE_POS_TOP,
    DLL_m678dll, PRACTICE_POS_TOP,
    DLL_m679dll, PRACTICE_POS_TOP_FAR,
    DLL_m680dll, PRACTICE_POS_BOTTOM,
    DLL_m681dll, PRACTICE_POS_BOTTOM,
    DLL_m699dll, PRACTICE_POS_TOP_FAR,
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
    for(i=0; PracticeTbl[i].ovl != DLL_NONE; i++) {
        if(omcurovl == PracticeTbl[i].ovl) {
            break;
        }
    }
    if(PracticeTbl[i].ovl != DLL_NONE) {
        HuPrcSetStat(HuPrcChildCreate(PracticeExec, 10, 8192, 0, parent), HU_PRC_STAT_PAUSE_ON|HU_PRC_STAT_UPAUSE_ON);
    }
}

static void PracticeExec(void)
{
    float angle;
    s16 id;
    HUSPR_GROUPID gid;
    MG_DATA *mgP;
    BOOL exitF;
    PRACTICE *practiceP;
    HUSPRID sprid;
    ANIMDATA *anim;
    
    static float posTbl[PRACTICE_POS_MAX] = {
        53,
        424,
        240,
        384,
        102
    };

    angle = 0;
    for(id=0; PracticeTbl[id].ovl != DLL_NONE; id++) {
        if(omcurovl == PracticeTbl[id].ovl) {
            break;
        }
    }
    practiceP = &PracticeTbl[id];
    id = MgNoGet(omcurovl);
    if(id != -1) {
        mgP = &MgDataTbl[id];
    } else {
        mgP = &MgDataTbl[0];
    }
    gid = HuSprGrpCreate(1);
    HuSprGrpPosSet(gid, 0, 0);
    anim = HuSprAnimRead(GameMesDataRead(GAMEMES_ANM_practice));
    sprid = HuSprCreate(anim, 0, 0);
    HuSprGrpMemberSet(gid, 0, sprid);
    HuSprPosSet(gid, 0, 288, posTbl[practiceP->posNo]);
    do {
        if(!wipeFadeInF || WipeCheckIn() || omPauseChk()) {
            HuSprDispOff(gid, 0);
            HuPrcVSleep();
            continue;
        }
        HuSprDispOn(gid, 0);
        exitF = FALSE;
        switch(mgP->type) {
            case MG_TYPE_4P:
            case MG_TYPE_1VS3:
            case MG_TYPE_2VS2:
            case MG_TYPE_BATTLE:
            case MG_TYPE_KOOPA:
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
    HuSprDispOff(gid, 0);
    omSysExitReq = TRUE;
    HuPrcEnd();
    while(1) {
        HuPrcVSleep();
    }
}

void GameMesStartSePlay(void)
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
    if(!omPauseChk() || _CheckFlag(FLAG_MG_PRACTICE) || !PauseWaitF) {
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
    GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
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
    MTXScale(modelview, sp->scale.x, sp->scale.y, 1.0f);
    mtxTransCat(modelview, sp->pos.x, sp->pos.y, 0);
    MTXConcat(*sp->groupMtx, modelview, modelview);
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

static void ScoreBoxBGMake(ANIMDATA *anim, s16 iconNum)
{
    s16 i, j;
    u8 *bmpData;
    s16 blockW;
    s16 w;
    s16 blockH;
    s16 h;

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
                s16 row = i-(h-6);
                if(j == 0) {
                    bmpData[(j&7)+((j>>3)<<5)+(i>>2)*(blockW*4)+(i&3)*8] = row+0xA0;
                } else if(j == 1) {
                    bmpData[(j&7)+((j>>3)<<5)+(i>>2)*(blockW*4)+(i&3)*8] = row+0xB0;
                } else if(j == 2) {
                    bmpData[(j&7)+((j>>3)<<5)+(i>>2)*(blockW*4)+(i&3)*8] = row+0xC0;
                } else if(j == w-3) {
                    bmpData[(j&7)+((j>>3)<<5)+(i>>2)*(blockW*4)+(i&3)*8] = row+0x70;
                } else if(j == w-2) {
                    bmpData[(j&7)+((j>>3)<<5)+(i>>2)*(blockW*4)+(i&3)*8] = row+0x80;
                } else if(j == w-1) {
                    bmpData[(j&7)+((j>>3)<<5)+(i>>2)*(blockW*4)+(i&3)*8] = row+0x90;
                } else {
                    bmpData[(j&7)+((j>>3)<<5)+(i>>2)*(blockW*4)+(i&3)*8] = row+0x60;
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
    if(iconNum) {
        s16 no;
        for(no=0; no<iconNum; no++) {
            for(i=0; i<6; i++) {
                for(j=0; j<6; j++) {
                    bmpData[((j+(no*5))&7)+(((j+(no*5))>>3)<<5)+(i>>2)*(blockW*4)+(i&3)*8] = (j*16)+i;
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

static HUSPR_GROUPID ScoreBoxCreate(s16 sizeX, s16 sizeY, s16 charNum, u8 *charTbl);

HUSPR_GROUPID MgScoreBoxCreate(s16 sizeX, s16 sizeY)
{
    return ScoreBoxCreate(sizeX, sizeY, 0, NULL);
}

HUSPR_GROUPID MgScoreBoxCreateChar(s16 sizeX, s16 sizeY, s16 charNo)
{
    u8 charTbl[1];
    charTbl[0] = charNo;
    return ScoreBoxCreate(sizeX, sizeY, 1, charTbl);
}

HUSPR_GROUPID MgScoreBoxCreateCharMulti(s16 sizeX, s16 sizeY, s16 charNum, u8 *charTbl)
{
    return ScoreBoxCreate(sizeX, sizeY, charNum, charTbl);
}

static HUSPR_GROUPID ScoreBoxCreate(s16 sizeX, s16 sizeY, s16 charNum, u8 *charTbl)
{
    s16 i;
    HUSPR_GROUPID gid;
    HUSPRID sprid;
    ANIMDATA *anim;
    sizeX = (sizeX+7) & 0xFFF8;
    sizeY = (sizeY+7) & 0xFFF8;
    if(sizeX < 48) {
        sizeX = 48;
    }
    if(sizeY < 48) {
        sizeY = 48;
    }
    gid = HuSprGrpCreate(1+charNum);
    if(!ScoreBoxMaskAnim) {
        ScoreBoxMaskAnim = HuSprAnimDataRead(MGCONST_ANM_score_box_mask);
    }
    if(!ScoreBoxAnim) {
        ScoreBoxAnim = HuSprAnimDataRead(MGCONST_ANM_score_box);
    }
    sprid = HuSprFuncCreate(ScoreBoxDraw, 100);
    HuSprGrpMemberSet(gid, 0, sprid);
    HuSprData[sprid].bg = anim = HuSprAnimMake(sizeX/8, sizeY/8, ANIM_BMP_IA4);
    ScoreBoxBGMake(anim, charNum);
    HuSprColorSet(gid, 0, 64, 64, 64);
    HuSprTPLvlSet(gid, 0, 1);
    for(i=0; i<charNum; i++) {
        anim = HuSprAnimDataRead(MGCONST_ANM_icon_mario+charTbl[i]);
        sprid = HuSprCreate(anim, 100, 0);
        HuSprGrpMemberSet(gid, i+1, sprid);
        HuSprPosSet(gid, i+1, -(sizeX/2)+24+(i*40), 0);
        HuSprTPLvlSet(gid, i+1, 1);
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
        HuSprGrpDispOn(gid);
    } else {
        HuSprGrpDispOff(gid);
    }
}

#define FONT_NUMBER(c) (GAMEMES_ANM_num_0+((c)-'0'))
#define FONT_LETTER_UPPER(c) (GAMEMES_ANM_upper_a+((c)-'A'))
#define FONT_LETTER_LOWER(c) (GAMEMES_ANM_lower_a+((c)-'a'))

static unsigned int FontFileTbl[] = {
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
    GAMEMES_ANM_num_0,
    GAMEMES_ANM_num_0,
    GAMEMES_ANM_num_0,
    GAMEMES_ANM_punct_minus,
    GAMEMES_ANM_num_0,
    GAMEMES_ANM_num_0,
    GAMEMES_ANM_num_0,
    FONT_LETTER_UPPER('A'),
    FONT_LETTER_UPPER('B'),
    FONT_LETTER_UPPER('C'),
    FONT_LETTER_UPPER('D'),
    FONT_LETTER_UPPER('E'),
    FONT_LETTER_UPPER('F'),
    FONT_LETTER_UPPER('G'),
    FONT_LETTER_UPPER('H'),
    FONT_LETTER_UPPER('I'),
    FONT_LETTER_UPPER('J'),
    FONT_LETTER_UPPER('K'),
    FONT_LETTER_UPPER('L'),
    FONT_LETTER_UPPER('M'),
    FONT_LETTER_UPPER('N'),
    FONT_LETTER_UPPER('O'),
    FONT_LETTER_UPPER('P'),
    FONT_LETTER_UPPER('Q'),
    FONT_LETTER_UPPER('R'),
    FONT_LETTER_UPPER('S'),
    FONT_LETTER_UPPER('T'),
    FONT_LETTER_UPPER('U'),
    FONT_LETTER_UPPER('V'),
    FONT_LETTER_UPPER('W'),
    FONT_LETTER_UPPER('X'),
    FONT_LETTER_UPPER('Y'),
    FONT_LETTER_UPPER('Z'),
    FONT_LETTER_UPPER('O'),
    GAMEMES_ANM_apos,
    FONT_LETTER_UPPER('O'),
    FONT_LETTER_UPPER('O'),
    FONT_LETTER_UPPER('O'),
    FONT_LETTER_UPPER('O'),
    FONT_LETTER_LOWER('a'),
    FONT_LETTER_LOWER('b'),
    FONT_LETTER_LOWER('c'),
    FONT_LETTER_LOWER('d'),
    FONT_LETTER_LOWER('e'),
    FONT_LETTER_LOWER('f'),
    FONT_LETTER_LOWER('g'),
    FONT_LETTER_LOWER('h'),
    FONT_LETTER_LOWER('i'),
    FONT_LETTER_LOWER('j'),
    FONT_LETTER_LOWER('k'),
    FONT_LETTER_LOWER('l'),
    FONT_LETTER_LOWER('m'),
    FONT_LETTER_LOWER('n'),
    FONT_LETTER_LOWER('o'),
    FONT_LETTER_LOWER('p'),
    FONT_LETTER_LOWER('q'),
    FONT_LETTER_LOWER('r'),
    FONT_LETTER_LOWER('s'),
    FONT_LETTER_LOWER('t'),
    FONT_LETTER_LOWER('u'),
    FONT_LETTER_LOWER('v'),
    FONT_LETTER_LOWER('w'),
    FONT_LETTER_LOWER('x'),
    FONT_LETTER_LOWER('y'),
    FONT_LETTER_LOWER('z'),
    GAMEMES_ANM_num_0,
    GAMEMES_ANM_num_0,
    GAMEMES_ANM_num_0,
    GAMEMES_ANM_punct_empty1,
    GAMEMES_ANM_num_0,
    GAMEMES_ANM_num_0,
    GAMEMES_ANM_num_0,
    GAMEMES_ANM_num_0,
    GAMEMES_ANM_num_0,
    GAMEMES_ANM_punct_minus,
    GAMEMES_ANM_punct_period,
    GAMEMES_ANM_num_0,
    GAMEMES_ANM_num_0,
    GAMEMES_ANM_num_0,
    GAMEMES_ANM_num_0,
    GAMEMES_ANM_num_0,
    GAMEMES_ANM_num_0,
    GAMEMES_ANM_num_0,
    GAMEMES_ANM_num_0,
    GAMEMES_ANM_num_0,
    GAMEMES_ANM_num_0,
    GAMEMES_ANM_num_0,
    GAMEMES_ANM_num_0,
    GAMEMES_ANM_num_0,
    GAMEMES_ANM_num_0,
    GAMEMES_ANM_num_0,
    GAMEMES_ANM_num_0,
    GAMEMES_ANM_num_0,
    GAMEMES_ANM_num_0,
    GAMEMES_ANM_num_0,
    GAMEMES_ANM_num_0,
    GAMEMES_ANM_num_0,
    GAMEMES_ANM_num_0,
    GAMEMES_ANM_num_0,
    GAMEMES_ANM_num_0,
    GAMEMES_ANM_num_0,
    GAMEMES_ANM_num_0,
    GAMEMES_ANM_num_0,
    GAMEMES_ANM_num_0,
    GAMEMES_ANM_num_0,
    GAMEMES_ANM_num_0,
    GAMEMES_ANM_num_0,
    GAMEMES_ANM_num_0,
    GAMEMES_ANM_num_0,
    GAMEMES_ANM_num_0,
    GAMEMES_ANM_num_0,
    GAMEMES_ANM_num_0,
    GAMEMES_ANM_num_0,
    GAMEMES_ANM_num_0,
    GAMEMES_ANM_num_0,
    GAMEMES_ANM_num_0,
    GAMEMES_ANM_num_0,
    GAMEMES_ANM_num_0,
    GAMEMES_ANM_num_0,
    GAMEMES_ANM_num_0,
    GAMEMES_ANM_num_0,
    GAMEMES_ANM_num_0,
    GAMEMES_ANM_num_0,
    GAMEMES_ANM_num_0,
    GAMEMES_ANM_num_0,
    GAMEMES_ANM_num_0,
    GAMEMES_ANM_num_0,
    GAMEMES_ANM_num_0,
    GAMEMES_ANM_num_0,
    GAMEMES_ANM_num_0,
    GAMEMES_ANM_num_0,
    GAMEMES_ANM_num_0,
    GAMEMES_ANM_num_0,
    GAMEMES_ANM_num_0,
    GAMEMES_ANM_num_0,
    GAMEMES_ANM_num_0,
    GAMEMES_ANM_punct_exclamation,
    GAMEMES_ANM_punct_hatena,
    GAMEMES_ANM_num_0,
    GAMEMES_ANM_num_0
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
    for(strId=0; strId<GAMEMES_SPR_MAX; strId++) {
        if(mes->grpId[strId] == HUSPR_GROUP_NONE) {
            break;
        }
    }
    if(strId == GAMEMES_SPR_MAX) {
        return GAMEMES_STR_NONE;
    }
    fileTbl = FontFileTbl;
    charAnimTbl = HuMemDirectMalloc(HEAP_HEAP, STR_CHAR_MAX*sizeof(ANIMDATA *));
    charX = HuMemDirectMalloc(HEAP_HEAP, STR_CHAR_MAX*sizeof(s16));
    charY = HuMemDirectMalloc(HEAP_HEAP, STR_CHAR_MAX*sizeof(s16));
    for(mesPtr=HuWinMesPtrGet(winMessId), x=0, charNum=0; *mesPtr; mesPtr++) {
        if(mesPtr[0] >= '0' || mesPtr[0] == ' ' || mesPtr[0] == 0x10) {
            dataNum = fileTbl[mesPtr[0]];
            charAnimTbl[charNum] = HuSprAnimRead(GameMesDataRead(dataNum & 0x7FFFFFFF));
            charX[charNum] = x;
            if(mesPtr[0] == ' ' || mesPtr[0] == 0x10) {
                charY[charNum] = 0;
                x += 28;
            } else if(mesPtr[0] >= 'a' && mesPtr[0] <= 'z') {
                charY[charNum] = 4;
                x += 42;
            } else if(mesPtr[0] == 194 || mesPtr[0]  == 195) {
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