#include "dolphin.h"

void fn_80146DB8(void);

u8 lbl_80245FC8[64] = {
    0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48,
    0x4B, 0x4C, 0x4D, 0x4E, 0x50, 0x51, 0x52, 0x53,
    0x54, 0x55, 0x56, 0x57, 0x59, 0x5A, 0x91, 0x92,
    0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A,
    0x9B, 0x9C, 0x9D, 0x9E, 0x9F, 0xA0, 0xA1, 0xA2,
    0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA,
    0xAB, 0xAC, 0xAE, 0xAF, 0xB0, 0xB1, 0xB2, 0xB3,
    0xB4, 0xB5, 0xB6, 0xB7, 0xB9, 0xBA, 0xBC, 0xBD
};

s32 lbl_80246008[24] = {
    9, 12, 19, 14, 16, 21, 13, 2, 18, 6, 15, 7,
    17, 0, 1, 3, 10, 4, 20, 5, 11, 22, 8, 23
};

u8 lbl_802B6A20[0xE0];

void fn_80146BA0(s32 arg0, u8 *arg1)
{
    s32 value;
    u32 bits;
    s32 sum;
    u32 work;
    s32 i;

    value = arg0 + 0x18EF;
    work = value;
    sum = 0;
    for (i = 0; i < 7; i++) {
        sum += work & 3;
        work >>= 2;
    }
    sum &= 3;
    work = value + (sum << 14);
    sum = ((work & 0xFF) + ((work >> 8) & 0xFF)) & 0xFF;
    work += sum << 16;
    bits = 0;
    for (i = 0; i < 24; i++) {
        bits |= (work & 1) << lbl_80246008[i];
        work >>= 1;
    }
    for (i = 0; i < 4; i++) {
        arg1[i] = lbl_80245FC8[bits & 0x3F];
        bits >>= 6;
    }
}

s32 fn_80146C88(u8 *arg0)
{
    s32 i;
    u32 work;
    u32 bits;
    u32 chk;
    u32 c;

    fn_80146DB8();
    bits = 0;
    for (i = 0; i < 4; i++) {
        bits <<= 6;
        c = lbl_802B6A20[*(arg0 + 3 - i) - 0x20];
        if (c > 0x40) {
            return -1;
        }
        bits |= c;
    }
    work = 0;
    for (i = 23; i >= 0; i--) {
        work <<= 1;
        work |= (bits >> lbl_80246008[i]) & 1;
    }
    chk = ((work & 0xFF) + ((work >> 8) & 0xFF)) & 0xFF;
    if (chk != ((work >> 16) & 0xFF)) {
        return -1;
    }
    work &= 0xFFFF;
    bits = work;
    chk = 0;
    for (i = 0; i < 7; i++) {
        chk += work & 3;
        work >>= 2;
    }
    chk &= 3;
    if (chk != ((bits >> 14) & 3)) {
        return -1;
    }
    return (bits & 0x3FFF) - 0x18EF;
}

void fn_80146DB8(void)
{
    s32 i;

    for (i = 0; i < 0xE0; i++) {
        lbl_802B6A20[i] = 0xFF;
    }
    for (i = 0; i < 0x40; i++) {
        lbl_802B6A20[lbl_80245FC8[i] - 0x20] = i;
    }
}
