#ifndef _BOARD_MASU_H
#define _BOARD_MASU_H

#include "humath.h"

#define MASU_LINK_MAX 5

#define MASU_LAYER_DEFAULT 0

#define MASU_FLAG_BATTAN 0x4000

typedef struct Masu_s {
    u8 _unk00[0x10];
    u16 flag;
    u8 _unk12[2];
    u32 mAttr;
    u8 _unk18[0x64 - 0x18];
    u16 linkNum;
    u16 linkTbl[MASU_LINK_MAX];
} MASU;

MASU *mbMasuGet(s16 id);
u16 mbMasuAttrGet(s16 id);
int mbMasuRawNumGet(void);
BOOL mbMasuPosGet(s16 id, HuVecF *pos);
int mbMasuLinkTblGet(s16 id, s16 *linkTbl);
s16 mbMasuLinkParentGet(s16 id, s16 *linkTbl);
int mbMasuFind_IdStepGet(s16 id, s16 targetId);
int mbMasuFind_TypeStepGet2(s16 id, int type, BOOL arg2, BOOL arg3);

#endif
