#ifndef _GAME_MGDATA_H
#define _GAME_MGDATA_H

#include "dolphin.h"

#define MG_TYPE_4P 0
#define MG_TYPE_1VS3 1
#define MG_TYPE_2VS2 2
#define MG_TYPE_BATTLE 3
#define MG_TYPE_KUPA 4
#define MG_TYPE_LAST 5
#define MG_TYPE_KETTOU 6
#define MG_TYPE_DONKEY 7
#define MG_TYPE_SD 8
#define MG_TYPE_NONE 9
#define MG_TYPE_MAX MG_TYPE_NONE

typedef struct MgData_s {
    u16 ovl;                 // 0x00
    u8 type;                 // 0x02
    u8 unk03;                // 0x03
    u16 flag;                // 0x04
    s8 recordNo;             // 0x06
    u8 unk07;                // 0x07
    u32 nameMes;             // 0x08
    unsigned int dataDir;    // 0x0C
    unsigned int instPic[3]; // 0x10
    unsigned int instPicN[3];// 0x1C
    char *movie[3];          // 0x28
    char *movieN[3];         // 0x34
    u32 instMes[3][5];       // 0x40
} MGDATA;                    // size 0x7C

extern MGDATA MgDataTbl[];
extern BOOL MgPauseExitF;
extern BOOL MgExitReq;
extern BOOL MgInstExitF;
extern BOOL MgBoard2Force;
extern int MgSubMode;
extern int MiracleBookEvtNo;
extern int MgBattleOrder;
extern int MgModeSubMode;
extern s16 MgModeScore[4];
extern s16 MgGameNo;
extern u8 MgModePlayNum;
extern u32 MgModeWork[64];
extern int MgModeWorkInt[16];
extern float MgModeWorkFloat[16];

s32 MgNoGet(s16 ovlNo);
int MgSubModeGet(void);
int MgDecaScoreCalc(int gameNo, int mgScore);

#endif
