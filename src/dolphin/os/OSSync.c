#include "string.h"
#include "dolphin/PPCArch.h"
#include "dolphin/os.h"

/* Defined by the preserved original object fallback. */
void __OSSystemCallVectorStart();
void __OSSystemCallVectorEnd();

void __OSInitSystemCall() {
  void* addr = OSPhysicalToCached(0x00C00);
  memcpy(addr, __OSSystemCallVectorStart, (size_t)__OSSystemCallVectorEnd - (size_t)__OSSystemCallVectorStart);
  DCFlushRangeNoSync(addr, 0x100);
  __sync();
  ICInvalidateRange(addr, 0x100);
}
