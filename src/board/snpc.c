#include "game/board/main.h"

#define SNPC_MAGIC 0x534E5043

typedef struct MBSNPCSAVEWORK {
    u8 flags;
    u8 masuId;
    u8 effectMissCount;
} MBSNPCSAVEWORK;

typedef struct MBSNPCWORK MBSNPCWORK;

static u32 snpcMagic;
static MBSNPCSAVEWORK *snpcSaveWork;
static MBSNPCWORK *snpcWork;

static void SNpcStarFunc(void);

void mbSNpcInit(void)
{
    snpcMagic = 0;
    snpcSaveWork = NULL;
    snpcWork = NULL;
}

int mbSNpcMasuGet(void)
{
    if (snpcMagic != SNPC_MAGIC) {
        return 0;
    }
    return snpcSaveWork->masuId;
}

static void SNpcStarFunc(void)
{
}
