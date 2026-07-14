#include "types.h"

extern f64 __ieee754_log10();

f64 log10(f64 __x)
{
	return (f64)__ieee754_log10();
}
