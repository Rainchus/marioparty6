#include "PowerPC_EABI_Support/Msl/MSL_C/MSL_Common/stdio.h"

extern void abort(void);

void __msl_assertion_failed(const char *condition, const char *filename, int line)
{
    fprintf(stderr, "Assertion (%s) failed in \"%s\" on line %d\n", condition, filename, line);
    abort();
}
