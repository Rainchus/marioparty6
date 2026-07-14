#include "PowerPC_EABI_Support/Msl/MSL_C/MSL_Common/alloc.h"
#include "PowerPC_EABI_Support/Runtime/New.h"

std::new_handler std::__new_handler = 0;

void operator delete[](void* arg0) throw() {
    if (arg0 != 0) {
        free(arg0);
    }
}

void operator delete(void* arg0) throw() {
    if (arg0 != 0) {
        free(arg0);
    }
}
