#ifndef METROTRK_PORTABLE_MEM_TRK_H
#define METROTRK_PORTABLE_MEM_TRK_H

#include "stddef.h"

void* TRK_memset(void* dst, int val, size_t n);
void* TRK_memcpy(void* dst, const void* src, size_t n);

#endif
