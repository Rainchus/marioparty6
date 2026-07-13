#ifndef _DOLPHIN_OSPRIV
#define _DOLPHIN_OSPRIV

#include "dolphin/os.h"

#ifdef __cplusplus
extern "C" {
#endif

#define OFFSET(addr, align) (((u32)(addr) & ((align)-1)))

__OSExceptionHandler __OSGetExceptionHandler(__OSException exception);
void __OSReschedule(void);
void __OSThreadInit(void);
s32 __OSGetEffectivePriority(OSThread* thread);
void __OSPromoteThread(OSThread* thread, s32 priority);
void __OSUnlockAllMutex(OSThread* thread);
BOOL __OSCheckDeadLock(OSThread* thread);
BOOL __OSCheckMutexes(OSThread* thread);
OSTime __OSGetSystemTime();
OSTime __OSTimeToSystemTime(OSTime);

#ifdef __cplusplus
}
#endif

#endif // _DOLPHIN_OSPRIV
