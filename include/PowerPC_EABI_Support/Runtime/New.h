#include "PowerPC_EABI_Support/Runtime/exception.h"

namespace std {
class bad_alloc : public exception {
public:
    bad_alloc() throw() {}
    bad_alloc(const bad_alloc&) throw() {}

    bad_alloc& operator=(const bad_alloc&) throw() {
        return *this;
    }

    virtual ~bad_alloc() throw() {}
    virtual const char* what() const throw() { return "bad_alloc"; }
};

typedef void (*new_handler)();

extern char __throws_bad_alloc;
extern new_handler __new_handler;
void __throw_bad_alloc();
} // namespace std

void operator delete(void* arg0) throw();
void operator delete[](void* arg0) throw();
