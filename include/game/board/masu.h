#ifndef _BOARD_MASU_H
#define _BOARD_MASU_H

#include "humath.h"

#define MASU_LINK_MAX 5

#define MASU_LAYER_DEFAULT 0

#define MASU_FLAG_BATTAN 0x4000

typedef struct Masu_s {
    u8 _pad[0x10];
    u16 flag;
} MASU;

MASU *mbMasuGet(s16 id);
u16 mbMasuAttrGet(s16 id);
int mbMasuRawNumGet(void);
BOOL mbMasuPosGet(s16 id, HuVecF *pos);
int mbMasuLinkTblGet(s16 id, s16 *linkTbl);

#endif
