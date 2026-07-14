#include "game/flag.h"
#include "game/wipe.h"

extern BOOL mbTutorialExitReqGet(void);

void mbWipeCreate(s16 mode, s16 type, s16 time)
{
    if (!_CheckFlag(FLAG_BOARD_TUTORIAL) || !mbTutorialExitReqGet()) {
        WipeCreate(mode, type, time);
    }
}

void mbWipeWait(void)
{
    while (WipeCheck()) {
        HuPrcVSleep();
    }
}

void mbWipeFadeOut(void)
{
    BOOL wipeF;

    if (_CheckFlag(FLAG_BOARD_TUTORIAL) && mbTutorialExitReqGet()) {
        wipeF = FALSE;
    } else {
        WipeCreate(WIPE_MODE_OUT, WIPE_TYPE_NORMAL, 21);
        wipeF = TRUE;
    }
    if (wipeF) {
        WipeColorSet(0, 0, 0);
        while (WipeCheck()) {
            HuPrcVSleep();
        }
    }
}

void mbWipeFadeIn(void)
{
    BOOL wipeF;

    if (_CheckFlag(FLAG_BOARD_TUTORIAL) && mbTutorialExitReqGet()) {
        wipeF = FALSE;
    } else {
        WipeCreate(WIPE_MODE_IN, WIPE_TYPE_NORMAL, 21);
        wipeF = TRUE;
    }
    if (wipeF) {
        while (WipeCheck()) {
            HuPrcVSleep();
        }
    }
}

void mbWipeWhiteFadeOut(void)
{
    BOOL wipeF;

    if (_CheckFlag(FLAG_BOARD_TUTORIAL) && mbTutorialExitReqGet()) {
        wipeF = FALSE;
    } else {
        WipeCreate(WIPE_MODE_OUT, WIPE_TYPE_WHITE, 21);
        wipeF = TRUE;
    }
    if (wipeF) {
        while (WipeCheck()) {
            HuPrcVSleep();
        }
    }
}

void mbWipeWhiteFadeIn(void)
{
    BOOL wipeF;

    if (_CheckFlag(FLAG_BOARD_TUTORIAL) && mbTutorialExitReqGet()) {
        wipeF = FALSE;
    } else {
        WipeCreate(WIPE_MODE_IN, WIPE_TYPE_WHITE, 21);
        wipeF = TRUE;
    }
    if (wipeF) {
        while (WipeCheck()) {
            HuPrcVSleep();
        }
    }
}

void mbWipeDissolveFadeOut(void)
{
    BOOL wipeF;

    if (_CheckFlag(FLAG_BOARD_TUTORIAL) && mbTutorialExitReqGet()) {
        wipeF = FALSE;
    } else {
        WipeCreate(WIPE_MODE_OUT, WIPE_TYPE_CROSS_COPY, 1);
        wipeF = TRUE;
    }
    if (wipeF) {
        while (WipeCheck()) {
            HuPrcVSleep();
        }
    }
}

void mbWipeDissolveFadeIn(void)
{
    BOOL wipeF;

    if (_CheckFlag(FLAG_BOARD_TUTORIAL) && mbTutorialExitReqGet()) {
        wipeF = FALSE;
    } else {
        WipeCreate(WIPE_MODE_IN, WIPE_TYPE_CROSS_COPY, 30);
        wipeF = TRUE;
    }
    if (wipeF) {
        while (WipeCheck()) {
            HuPrcVSleep();
        }
    }
}

void mbWipeFadeOutTime(int time)
{
    BOOL wipeF;

    if (_CheckFlag(FLAG_BOARD_TUTORIAL) && mbTutorialExitReqGet()) {
        wipeF = FALSE;
    } else {
        WipeCreate(WIPE_MODE_OUT, WIPE_TYPE_NORMAL, time);
        wipeF = TRUE;
    }
    if (wipeF) {
        WipeColorSet(0, 0, 0);
        while (WipeCheck()) {
            HuPrcVSleep();
        }
    }
}

void mbWipeFadeInTime(int time)
{
    BOOL wipeF;

    if (_CheckFlag(FLAG_BOARD_TUTORIAL) && mbTutorialExitReqGet()) {
        wipeF = FALSE;
    } else {
        WipeCreate(WIPE_MODE_IN, WIPE_TYPE_NORMAL, time);
        wipeF = TRUE;
    }
    if (wipeF) {
        while (WipeCheck()) {
            HuPrcVSleep();
        }
    }
}

void mbWipeWhiteFadeOutTime(int time)
{
    BOOL wipeF;

    if (_CheckFlag(FLAG_BOARD_TUTORIAL) && mbTutorialExitReqGet()) {
        wipeF = FALSE;
    } else {
        WipeCreate(WIPE_MODE_OUT, WIPE_TYPE_WHITE, time);
        wipeF = TRUE;
    }
    if (wipeF) {
        while (WipeCheck()) {
            HuPrcVSleep();
        }
    }
}

void mbWipeWhiteFadeInTime(int time)
{
    BOOL wipeF;

    if (_CheckFlag(FLAG_BOARD_TUTORIAL) && mbTutorialExitReqGet()) {
        wipeF = FALSE;
    } else {
        WipeCreate(WIPE_MODE_IN, WIPE_TYPE_WHITE, time);
        wipeF = TRUE;
    }
    if (wipeF) {
        while (WipeCheck()) {
            HuPrcVSleep();
        }
    }
}

void mbWipeDissolveFadeOutTime(int time)
{
    BOOL wipeF;

    if (_CheckFlag(FLAG_BOARD_TUTORIAL) && mbTutorialExitReqGet()) {
        wipeF = FALSE;
    } else {
        WipeCreate(WIPE_MODE_OUT, WIPE_TYPE_CROSS_COPY, time);
        wipeF = TRUE;
    }
    if (wipeF) {
        while (WipeCheck()) {
            HuPrcVSleep();
        }
    }
}

void mbWipeDissolveFadeInTime(int time)
{
    BOOL wipeF;

    if (_CheckFlag(FLAG_BOARD_TUTORIAL) && mbTutorialExitReqGet()) {
        wipeF = FALSE;
    } else {
        WipeCreate(WIPE_MODE_IN, WIPE_TYPE_CROSS_COPY, time);
        wipeF = TRUE;
    }
    if (wipeF) {
        while (WipeCheck()) {
            HuPrcVSleep();
        }
    }
}
