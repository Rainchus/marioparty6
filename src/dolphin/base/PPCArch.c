#include "types.h"

union FpscrUnion {
    f64 f;
    struct {
        u32 fpscr_pad;
        u32 fpscr;
    } u;
};

#define HID0_SPD 0x00000200

u32 PPCMfhid0(void);
void PPCMthid0(u32 newHID0);

u32 PPCMffpscr(void)
{
    union FpscrUnion m;

    m.f = __mffs();
    return m.u.fpscr;
}

void PPCDisableSpeculation(void)
{
    PPCMthid0(PPCMfhid0() | HID0_SPD);
}
