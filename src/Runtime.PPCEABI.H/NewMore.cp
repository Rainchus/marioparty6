#include "PowerPC_EABI_Support/Runtime/New.h"

namespace std {
    char __throws_bad_alloc = 1;

    void __throw_bad_alloc() {
        if (__throws_bad_alloc) {
            throw bad_alloc();
        }
    }
}
