#ifndef _GAME_MGDATA_H
#define _GAME_MGDATA_H

#include "dolphin.h"
#include "game/gamework.h"

enum {
	MG_TYPE_4P, //0
	MG_TYPE_1VS3, //1
	MG_TYPE_2VS2, //2
	MG_TYPE_BATTLE, //3
	MG_TYPE_KOOPA, //4
	MG_TYPE_LAST, //5
	MG_TYPE_KETTOU, //6
	MG_TYPE_DONKEY, //7
	MG_TYPE_678, //8
	MG_TYPE_MAX //9
};

enum {
	MG_TIME_DAY, //0
	MG_TIME_NIGHT, //1
	MG_TIME_MAX //2
};

enum {
	MG_INST_TYPE_DAY, //0
	MG_INST_TYPE_NIGHT, //1
	MG_INST_TYPE_DECATHLON, //2
	MG_INST_TYPE_MAX //3
};

enum { 
	MG_INST_PAGE_INST, //0
	MG_INST_PAGE_CONTROL, //1
	MG_INST_PAGE_CONTROL2, //2
	MG_INST_PAGE_HELP, //3
	MG_INST_PAGE_HELP2, //4
	MG_INST_PAGE_MAX //5
};

#define MG_SUBGAME_MAX 3

#define MG_INST_MESS_NULL 0

#define MG_FLAG_PACK_ACTION (1 << 0)
#define MG_FLAG_PACK_SKILL (1 << 1)
#define MG_FLAG_PACK_WEIRD (1 << 2)
#define MG_FLAG_PACK_EASY (1 << 3)
#define MG_FLAG_4P (1 << 4)
#define MG_FLAG_TEAM_ORDER (1 << 5)
#define MG_FLAG_COIN (1 << 6)
#define MG_FLAG_RARE (1 << 7)
#define MG_FLAG_HAS_NIGHT (1 << 8)
#define MG_FLAG_MIC (1 << 9)
#define MG_FLAG_DISABLE_NIGHT (1 << 10)
#define MG_FLAG_DISABLE_DAY (1 << 11)

#define MG_FLAG_PACK_ALL (MG_FLAG_PACK_EASY|MG_FLAG_PACK_WEIRD|MG_FLAG_PACK_SKILL|MG_FLAG_PACK_ACTION)

#define MgModeWorkGet() ((void *)MgModeWork)
#define MgModeWorkGetAs(type) ((type *)MgModeWork)

typedef struct MgData_s {
	u16 ovl; //0x00
	u8 type; //0x02
	u16 flag; //0x04
	s8 recordNo; //0x06
	u32 nameMes; //0x08
	unsigned int dataDir; //0x0C
	unsigned int instPic[MG_TIME_MAX][MG_SUBGAME_MAX]; //0x10
	char *movie[MG_TIME_MAX][MG_SUBGAME_MAX]; //0x28
	u32 instMes[MG_INST_TYPE_MAX][MG_INST_PAGE_MAX]; //0x40
} MG_DATA;

extern float MgModeWorkFloat[16];
extern int MgModeWorkInt[16];
extern MG_DATA MgDataTbl[];
extern GW_COMMON GwCommonBackup;
extern u8 MgModeWork[256];

extern int MgModeSubMode;
extern s16 MgModeScore[GW_PLAYER_MAX];
extern s16 MgGameNo;
extern u8 MgModePlayNum;
extern u8 MgBattleOrder[GW_PLAYER_MAX];
extern int lbl_802C0438;
extern int MgSubMode;
extern int MBookEvtNo;
extern BOOL MgExitReq;
extern BOOL MgPauseExitF;
extern BOOL MgInstExitF;
extern BOOL MgBoardE3F;

int MgNoGet(s16 ovlNo);
int MgSubModeGet(void);
int MgDecaScoreCalc(int gameNo, int mgScore);

#endif