#ifndef GSSDK_FFT_H
#define GSSDK_FFT_H

#include "types.h"

void fht(f32 *values, int length);
void realfft(int length, f32 *values);

#endif
