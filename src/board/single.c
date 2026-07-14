#include "game/gamework.h"

#include <string.h>

static u32 singleMgUnlock[4];

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
