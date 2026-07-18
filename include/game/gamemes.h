#ifndef _GAME_GAMEMES_H
#define _GAME_GAMEMES_H

#include "humath.h"
#include "game/sprite.h"
#include "game/object.h"
#include "game/data.h"
#include "datanum/gamemes.h"
#include "messnum/mg_inst_sys.h"

#include "stdarg.h"

#define GAMEMES_ID_NONE -1

#define GAMEMES_MAX 8

#define GAMEMES_SPR_MAX 16

typedef enum {
	GAMEMES_TYPE_NULL = 0, //0
	GAMEMES_TYPE_TIMER, //1
	GAMEMES_TYPE_MG, //2
	GAMEMES_TYPE_MG_2VS2, //3
	GAMEMES_TYPE_MG_WIN, //4
	GAMEMES_TYPE_MG_1VS3, //5
	GAMEMES_TYPE_MG_BATTLE, //6
	GAMEMES_TYPE_MG_4P, //7
	GAMEMES_TYPE_MG_KOOPA, //8
	GAMEMES_TYPE_MG_LAST, //9
	GAMEMES_TYPE_MG_KETTOU, //10
	GAMEMES_TYPE_MG_DONKEY, //11
	GAMEMES_TYPE_MG_678, //12
	GAMEMES_TYPE_MG_DRAW, //13
	GAMEMES_TYPE_MG_RECORD, //14
	GAMEMES_TYPE_MAX, //15
} GAMEMES_TYPE;

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

#define GameMesTimerCreate(time) GameMesCreate(GAMEMES_TYPE_TIMER, (int)(time), -1, -1)
#define GameMesTimerPosCreate(time, x, y) GameMesCreate(GAMEMES_TYPE_TIMER, (int)(time), (int)(x), (int)(y))
#define GameMesTimerValueSet(GameMesId, value) GameMesDispSet(GameMesId, GAMEMES_DISP_UPDATE, (int)(value))
#define GameMesTimerEnd(GameMesId) GameMesDispSet(GameMesId, GAMEMES_DISP_SET, -1)

#define GameMesMgStartCreate() GameMesCreate(GAMEMES_TYPE_MG, GAMEMES_MG_TYPE_START)
#define GameMesMgFinishCreate() GameMesCreate(GAMEMES_TYPE_MG, GAMEMES_MG_TYPE_FINISH)
#define GameMesMgDrawCreate() GameMesCreate(GAMEMES_TYPE_MG, GAMEMES_MG_TYPE_DRAW)
#define GameMesFXPlayCheck(GameMesId) (GameMesStatGet(GameMesId) & GAMEMES_STAT_FXPLAY)
#define GameMesMgStartTypeCreate(type) GameMesCreate(type, GAMEMES_MG_TYPE_START)
#define GameMesMgFinishTypeCreate(type) GameMesCreate(type, GAMEMES_MG_TYPE_FINISH)
#define GameMesMgDrawTypeCreate(type) GameMesCreate(type, GAMEMES_MG_TYPE_DRAW)

#define GameMesMgWinnerCreate(charNo1, charNo2, charNo3, charNo4) GameMesCreate(GAMEMES_TYPE_MG_WIN, GAMEMES_MG_TYPE_WIN, charNo1, charNo2, charNo3, charNo4)
#define GameMesMgWinnerTypeCreate(type, charNo1, charNo2, charNo3, charNo4) GameMesCreate(GAMEMES_TYPE_MG_WIN, type, charNo1, charNo2, charNo3, charNo4)
#define GameMesMgWinnerDrawCreate(type) GameMesCreate(GAMEMES_TYPE_MG_WIN, GAMEMES_MG_TYPE_DRAW)


#define GameMesRecordCreate(val) GameMesCreate(GAMEMES_TYPE_MG_RECORD, val)


typedef s16 GAMEMESID;

typedef struct GameMes_s GAMEMES;

typedef BOOL (*GAMEMES_EXEC)(GAMEMES *mes);
typedef BOOL (*GAMEMES_CREATE)(GAMEMES *mes, va_list args);

typedef struct GameMes_s {
	GAMEMES_EXEC exec; //0x0
	void *buf; //0x4
	HuVec2f pos; //0x8
	HuVec2f scale; //0x10
	float unk18;
	float angle; //0x1C
	float winScale; //0x20
	float unk24;
	s16 time; //0x28
	s16 timeMax; //0x2A
	s16 timerVal; //0x2C
	s16 subMode; //0x2E
	s16 work; //0x30
	s16 charNum; //0x32
	s16 dispMode; //0x34
	s16 dispValue; //0x36
	s16 mesMode; //0x38
	HUSPR_GROUPID grpId[GAMEMES_SPR_MAX]; //0x3A
	HUSPRID sprId[GAMEMES_SPR_MAX]; //0x5A
	u8 mesNo; //0x7A
	u8 stat; //0x7B
	GXColor color; //0x7C
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
//int GameMesStrCopy(GAMEMES *mes, GAMEMESID id);
void GameMesPauseCreate(void);
void GameMesPauseCancel(void);
void GameMesPauseEnable(BOOL enable);
void GameMesPracticeCreate(void);
void GameMesStartSePlay(void);
void GameMesExitCheck(OMOBJ *obj);
GAMEMESID GameMesStrWinCreate(GAMEMES *mes, u32 winMessId);

void MgScoreBoxInit(void);
HUSPR_GROUPID MgScoreBoxCreate(s16 sizeX, s16 sizeY);
HUSPR_GROUPID MgScoreBoxCreateChar(s16 sizeX, s16 sizeY, s16 charNo);
HUSPR_GROUPID MgScoreBoxCreateCharMulti(s16 sizeX, s16 sizeY, s16 charNum, u8 *charTbl);
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
BOOL GameMesMgLastInit(GAMEMES *mes, va_list args);

BOOL GameMesMgLastExec(GAMEMES *mes);
BOOL GameMesMgKettouInit(GAMEMES *mes, va_list args);

BOOL GameMesMgKettouExec(GAMEMES *mes);
BOOL GameMesMgDonkeyInit(GAMEMES *mes, va_list args);
BOOL GameMesMgDonkeyExec(GAMEMES *mes);
BOOL GameMesMg678Init(GAMEMES *mes, va_list args);
BOOL GameMesMg678Exec(GAMEMES *mes);
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
