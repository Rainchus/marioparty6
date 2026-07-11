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
    u8 flag;                 // 0x03
    s8 recordNo;             // 0x04
    u8 pad05[3];
    u32 nameMes;             // 0x08
    unsigned int dataDir;    // 0x0C
    unsigned int instPic[3]; // 0x10
    char *movie[3];          // 0x1C
    u32 pad28[6];            // 0x28
    u32 instMes[3][5];       // 0x40
} MGDATA;                    // size 0x7C

extern MGDATA MgDataTbl[];
extern BOOL MgPauseExitF;
extern BOOL MgExitReq;

s32 MgNoGet(s16 ovlNo);

#endif
