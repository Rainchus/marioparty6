#include <dolphin.h>
#include <dolphin/demo.h>
#include <dolphin/gx.h>
#include <dolphin/pad.h>
#include <dolphin/vi.h>
#include "game/init.h"

#include <dolphin/demoPriv.h>

// .sbss
static struct _GXRenderModeObj *rmode; // size: 0x4, address: 0x0

struct _GXRenderModeObj *DEMOGetRenderModeObj()
{
    return rmode;
}
