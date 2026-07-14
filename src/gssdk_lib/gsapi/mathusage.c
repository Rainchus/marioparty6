#include "types.h"

extern f32 logf(f32 value);

f32 logf_check(f32 value)
{
    u32 negativeInfinity = 0xFF800000;

    if (value == 0.0f) {
        return *(f32 *)&negativeInfinity;
    }
    return logf(value);
}
