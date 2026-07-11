#ifndef _GAME_GAMEMES_H
#define _GAME_GAMEMES_H

#include "dolphin.h"
#include "humath.h"
#include "game/sprite.h"
#include "game/object.h"
#include "datanum/gamemes.h"

#include "stdarg.h"

#define GAMEMES_ID_NONE -1

#define GAMEMES_MAX 8

#define GAMEMES_SPRMAX 16

#define GAMEMES_MES_NULL 0
#define GAMEMES_MES_TIMER 1
#define GAMEMES_MES_MG 2
#define GAMEMES_MES_MG_2VS2 3
#define GAMEMES_MES_MG_WINNER 4
#define GAMEMES_MES_MG_1VS3 5
#define GAMEMES_MES_MG_BATTLE 6
#define GAMEMES_MES_MG_4P 7
#define GAMEMES_MES_MG_KUPA 8
#define GAMEMES_MES_MG_LAST 9
#define GAMEMES_MES_MG_KETTOU 10
#define GAMEMES_MES_MG_DONKEY 11
#define GAMEMES_MES_MG_SD 12
#define GAMEMES_MES_MG_DRAW 13
#define GAMEMES_MES_MG_RECORD 14

#define GAMEMES_STAT_NONE 0
#define GAMEMES_STAT_EXIST (1 << 0)
#define GAMEMES_STAT_TIMEEND (1 << 2)
#define GAMEMES_STAT_KILL (1 << 3)
#define GAMEMES_STAT_FXPLAY (1 << 4)

#define GAMEMES_DISP_NONE 0
#define GAMEMES_DISP_UPDATE 1
#define GAMEMES_DISP_SET 2
#define GAMEMES_DISP_HIDE 3

#define GAMEMES_SUBMODE_NULL -1
#define GAMEMES_SUBMODE_NONE 0

#define GAMEMES_STR_NONE -1

#define GAMEMES_MG_TYPE_START 0
#define GAMEMES_MG_TYPE_FINISH 1
#define GAMEMES_MG_TYPE_DRAW 2
#define GAMEMES_MG_TYPE_WIN 3
#define GAMEMES_MG_TYPE_LOSE 4
#define GAMEMES_MG_TYPE_CHAMPION 5

#define GAMEMES_MG_WINNER_NONE -1

#define GameMesTimerCreate(time) GameMesCreate(GAMEMES_MES_TIMER, (int)(time), -1, -1)
#define GameMesTimerPosCreate(time, x, y) GameMesCreate(GAMEMES_MES_TIMER, (int)(time), (int)(x), (int)(y))
#define GameMesTimerValueSet(GameMesId, value) GameMesDispSet(GameMesId, GAMEMES_DISP_UPDATE, (int)(value))
#define GameMesTimerEnd(GameMesId) GameMesDispSet(GameMesId, GAMEMES_DISP_SET, -1)

#define GameMesMgStartCreate() GameMesCreate(GAMEMES_MES_MG, GAMEMES_MG_TYPE_START)
#define GameMesMgFinishCreate() GameMesCreate(GAMEMES_MES_MG, GAMEMES_MG_TYPE_FINISH)
#define GameMesMgDrawCreate() GameMesCreate(GAMEMES_MES_MG, GAMEMES_MG_TYPE_DRAW)
#define GameMesFXPlayCheck(GameMesId) (GameMesStatGet(GameMesId) & GAMEMES_STAT_FXPLAY)

typedef s16 GAMEMESID;

typedef struct GameMes_s GAMEMES;

typedef BOOL (*GAMEMESEXEC)(GAMEMES *mes);
typedef BOOL (*GAMEMESCREATE)(GAMEMES *mes, va_list args);

typedef struct GameMes_s {
    GAMEMESEXEC exec;
    void *buf;
    HuVec2f pos;
    HuVec2f scale;
    float unk18;
    float angle;
    float winScale;
    float unk24;
    s16 time;
    s16 timeMax;
    s16 timerVal;
    s16 subMode;
    s16 work;
    s16 charNum;
    s16 dispMode;
    s16 dispValue;
    s16 mesMode;
    HUSPR_GROUPID grpId[GAMEMES_SPRMAX];
    HUSPRID sprId[GAMEMES_SPRMAX];
    u8 mesNo;
    u8 stat;
    GXColor color;
} GAMEMES;

extern GAMEMES GameMesData[GAMEMES_MAX];

void GameMesInit(void);
void *GameMesDataRead(unsigned int dataNum);
void GameMesExec(void);

GAMEMESID GameMesCreate(s16 mesNo, ...);
GAMEMES *GameMesGet(GAMEMESID id);
u8 GameMesStatGet(GAMEMESID id);
void GameMesPosSet(GAMEMESID id, float posX, float posY);
void GameMesDispSet(GAMEMESID id, s16 mode, s16 value);
void GameMesKill(GAMEMESID id);
void GameMesClose(void);
BOOL GameMesKillCheck(void);
void GameMesStub(void);
void GameMesSprKill(GAMEMES *mes);
int GameMesStrCreate(GAMEMES *mes, char *str, s16 flag);
int GameMesStrCopy(GAMEMES *mes, s16 id);
void GameMesPauseCreate(void);
void GameMesPauseCancel(void);
void GameMesPauseEnable(BOOL enable);
void GameMesPracticeCreate(void);
void GameMesEnd(void);
void GameMesExitCheck(OMOBJ *obj);
GAMEMESID GameMesStrWinCreate(GAMEMES *mes, u32 winMessId);

void MgScoreBoxInit(void);
HUSPR_GROUPID MgScoreBoxCreate(s16 sizeX, s16 sizeY);
HUSPR_GROUPID MgScoreBoxCreateChar(s16 sizeX, s16 sizeY, s16 charId);
HUSPR_GROUPID MgScoreBoxCreateCharMulti(s16 sizeX, s16 sizeY, s16 num, u8 *chars);
void MgScoreBoxKill(HUSPR_GROUPID gid);
void MgScoreBoxColorSet(HUSPR_GROUPID gid, u8 r, u8 g, u8 b);
void MgScoreBoxTPLvlSet(HUSPR_GROUPID gid, float tpLvl);
void MgScoreBoxPosSet(HUSPR_GROUPID gid, float posX, float posY);
void MgScoreBoxDispSet(HUSPR_GROUPID gid, BOOL disp);

BOOL GameMesTimerInit(GAMEMES *mes, va_list args);
BOOL GameMesTimerExec(GAMEMES *mes);
BOOL GameMesMg4PInit(GAMEMES *mes, va_list args);
BOOL GameMesMg4PExec(GAMEMES *mes);
BOOL GameMesMg2Vs2Init(GAMEMES *mes, va_list args);
BOOL GameMesMg2Vs2Exec(GAMEMES *mes);
BOOL GameMesMgWinInit(GAMEMES *mes, va_list args);
BOOL GameMesMgWinExec(GAMEMES *mes);
BOOL GameMesMg1Vs3Init(GAMEMES *mes, va_list args);
BOOL GameMesMg1Vs3Exec(GAMEMES *mes);
BOOL GameMesMgBattleInit(GAMEMES *mes, va_list args);
BOOL GameMesMgBattleExec(GAMEMES *mes);
BOOL GameMesMgKoopaInit(GAMEMES *mes, va_list args);
BOOL GameMesMgKoopaExec(GAMEMES *mes);
BOOL GameMesMgRareInit(GAMEMES *mes, va_list args);
BOOL GameMesMgRareExec(GAMEMES *mes);
BOOL GameMesMgKettouInit(GAMEMES *mes, va_list args);
BOOL GameMesMgKettouExec(GAMEMES *mes);
BOOL GameMesMgDonkeyInit(GAMEMES *mes, va_list args);
BOOL GameMesMgDonkeyExec(GAMEMES *mes);
BOOL GameMesMgSdInit(GAMEMES *mes, va_list args);
BOOL GameMesMgSdExec(GAMEMES *mes);
BOOL GameMesMgDrawInit(GAMEMES *mes, va_list args);
BOOL GameMesMgDrawExec(GAMEMES *mes);
BOOL GameMesMgRecordInit(GAMEMES *mes, va_list args);
BOOL GameMesMgRecordExec(GAMEMES *mes);

extern GAMEMES GameMesData[GAMEMES_MAX];
extern s16 GameMesTime;
extern u8 GameMesCloseF;
extern unsigned int GameMesVWait;
extern int GameMesLanguageNo;
extern OMOVL GameMesOvlPrev;

#endif
