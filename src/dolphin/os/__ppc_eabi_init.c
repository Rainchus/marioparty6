#include <dolphin/PPCArch.h>
#include <dolphin/__ppc_eabi_init.h>

#ifdef __cplusplus
extern "C" {
#endif
void __OSPSInit();
void __OSFPRInit();
void __OSCacheInit();

/* __init_hardware and __flush_cache are defined by the preserved original object fallback. */
void __init_user()
{
    __init_cpp();
}

typedef void (*voidfunctionptr)(void); // pointer to function returning void
__declspec(section ".init") extern voidfunctionptr _ctors[];
__declspec(section ".init") extern voidfunctionptr _dtors[];

void __init_cpp(void)
{
    voidfunctionptr *constructor;

    /*
     *	call static initializers
     */
    for (constructor = _ctors; *constructor; constructor++) {
        (*constructor)();
    }
}

void _ExitProcess(void)
{
    PPCHalt();
}

#ifdef __cplusplus
}
#endif
