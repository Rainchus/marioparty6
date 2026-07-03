#include "game/object.h"
#include "game/gamework.h"
#include "game/hu3d.h"
#include "game/pad.h"

//Fix when mgdata.h is finished
extern BOOL MgPauseExitF;
//Fix when gamemes.h is finished
extern void GameMesExitCheck(OMOBJ *obj);

void omGameSysInit(OMOBJMAN *objman)
{
    int i;
    OMOBJ *obj;

    omSystemKeyCheckSetup(objman);
    Hu3DCameraScissorSet(1, 0, 0, 640, 480);
    omSysPauseEnable(0);

    for(i=0; i<GW_PLAYER_MAX; i++) {
        GWMgCoinBonusSet(i, 0);
        GWMgCoinSet(i, 0);
        GWMgScoreSet(i, 0);
    }

    if(MgPauseExitF) {
        obj = omAddObjEx(objman, 0x7FDC, 0, 0, -1, GameMesExitCheck);
        omSetStatBit(obj, OM_STAT_NOPAUSE|OM_STAT_SPRPAUSE);
    }
    HuWinInit(0);
}

void omVibrate(s16 playerNo, s16 duration, s16 off, s16 on)
{
    if (!WipeCheckIn() && GWVibrateGet() != FALSE && GwPlayerConf[playerNo].type == GW_PLAYER_TYPE_MAN) {
        HuPadRumbleSet(GwPlayerConf[playerNo].padNo, duration, off, on);
    }
}

s16 omPadErrChk(s16 padNo)
{
    s16 stat = HuPadStatGet(padNo);
    if(stat == -4) {
        return 0;
    } else {
        return stat;
    }
}