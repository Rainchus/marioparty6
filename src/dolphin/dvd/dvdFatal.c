#include <dolphin/dvd.h>

static void (*FatalFunc)();

void __DVDPrintFatalMessage(void)
{
    if (FatalFunc) {
        FatalFunc();
    }
}
