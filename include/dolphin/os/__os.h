#ifndef _DOLPHIN_OS_INTERNAL_H_
#define _DOLPHIN_OS_INTERNAL_H_

#include <dolphin/os.h>

#ifdef __cplusplus
extern "C" {
#endif

void __OSBootDol(u32 doloffset, u32 restartCode, const char** argv);

#ifdef __cplusplus
}
#endif

#endif
