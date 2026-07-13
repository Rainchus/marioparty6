#ifndef _DOLPHIN_OSPRIV
#define _DOLPHIN_OSPRIV

#include "dolphin/os.h"

#ifdef __cplusplus
extern "C" {
#endif

#define OFFSET(addr, align) (((u32)(addr) & ((align)-1)))

__OSExceptionHandler __OSGetExceptionHandler(__OSException exception);
void __OSReschedule(void);
OSTime __OSGetSystemTime();
OSTime __OSTimeToSystemTime(OSTime);

#ifdef __cplusplus
}
#endif

#endif // _DOLPHIN_OSPRIV
