#include "game/board/masu.h"
#include "game/gamework.h"
#include "game/flag.h"

#include <string.h>

static u32 singleMgUnlock[4];
static int singleTeamChar = -1;
static int miniKoopaType;
static u8 masuTypeNum;
static u8 masuType[5];

void mbSingleSaveInit(int teamChar, int mgPack, int storyComDif)
{
    int i;

    GWPartySet(FALSE);
    GwSystem.tagF = FALSE;
    GwSystem.storyComDif = storyComDif;
    GWBonusStarSet(FALSE);
    GwSystem.mgPack = mgPack;
    for (i = 0; i < GW_PLAYER_MAX; i++) {
        GwPlayer[i].handicap = 0;
    }
    GwSystem.turnMax = 50;
    memset(&GwPlayer[0], 0, GW_PLAYER_MAX * sizeof(GW_PLAYER));
    singleTeamChar = teamChar;
    _ClearFlag(0);
    _ClearFlag(1);
    _ClearFlag(2);
    _SetFlag(FLAG_BOARD_INIT);
    _ClearFlag(FLAG_BOARD_TUTORIAL);
    _SetFlag(5);
    _ClearFlag(FLAG_INST_DECA);
    _SetFlag(FLAGNUM(FLAG_GROUP_COMMON, 13));
}

void mbSingleMgUnlockInit(void)
{
    memset(singleMgUnlock, 0, sizeof(singleMgUnlock));
}

void mbSingleMgUnlockWrite(void)
{
    int word;
    int bit;

    for (word = 0; word < 4; word++) {
        for (bit = 0; bit < 32; bit++) {
            if (singleMgUnlock[word] & (1 << bit)) {
                GWMgUnlockSet((word << 5) + bit + GW_MGNO_BASE);
            }
        }
    }
}

void mbSingleMgUnlockSet(int mgNo)
{
    int word;
    int bit;

    mgNo -= GW_MGNO_BASE;
    word = mgNo >> 5;
    bit = mgNo % 32;
    singleMgUnlock[word] |= (1 << bit);
}

void mbSingleMgUnlockReset(int mgNo)
{
    int word;
    int bit;

    mgNo -= GW_MGNO_BASE;
    word = mgNo >> 5;
    bit = mgNo % 32;
    singleMgUnlock[word] &= ~(1 << bit);
}

BOOL mbSingleMgUnlockGet(int mgNo)
{
    int word;
    int bit;

    mgNo -= GW_MGNO_BASE;
    word = mgNo >> 5;
    bit = mgNo % 32;
    if (singleMgUnlock[word] & (1 << bit)) {
        return TRUE;
    } else {
        return FALSE;
    }
}

BOOL mbSingleMgUnlockCheckAny(void)
{
    int word;

    for (word = 0; word < 4; word++) {
        if (singleMgUnlock[word]) {
            return TRUE;
        }
    }
    return FALSE;
}

int mbSingleMgUnlockNumGet(void)
{
    int num = 0;
    int word;
    int bit;

    for (word = 0; word < 4; word++) {
        for (bit = 0; bit < 32; bit++) {
            if (singleMgUnlock[word] & (1 << bit)) {
                num++;
            }
        }
    }
    return num;
}

static void SingleMasuTypeReset(void)
{
    masuTypeNum = 0;
    memset(masuType, 0, sizeof(masuType));
}

void mbSinglePrizeFlagReset(int flag)
{
    if (flag <= 63) {
        GwSinglePrizeFlag[flag >> 5] &= ~(1 << (flag & 0x1F));
    }
}

int mbSingleStepGet(void)
{
    s16 masuId = GwPlayer[GwSystem.turnPlayerNo].masuId;
    return mbMasuFind_TypeStepGet(masuId, 7);
}

int mbSingleOppCharGet(void)
{
    return miniKoopaType + 11;
}

void mbSingleTeamCharSet(int character)
{
    singleTeamChar = character;
}

int mbSingleTeamCharGet(void)
{
    return singleTeamChar;
}
