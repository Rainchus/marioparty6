#include "game/pad.h"

u16 mbPadDStkRepGetAll(void)
{
    u16 btn = 0;
    int i;
    for (i = 0; i < 4; i++) {
        if (!HuPadStatGet(i)) {
            btn |= HuPadDStkRep[i];
        }
    }
    return btn;
}

u16 mbPadBtnDownGetAll(void)
{
    u16 btn = 0;
    int i;
    for (i = 0; i < 4; i++) {
        if (!HuPadStatGet(i)) {
            btn |= HuPadBtnDown[i];
        }
    }
    return btn;
}
