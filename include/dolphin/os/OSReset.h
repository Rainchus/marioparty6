#ifndef _DOLPHIN_OSRESET
#define _DOLPHIN_OSRESET

#include <types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define OS_RESETCODE_RESTART 0x80000000
#define OS_RESETCODE_SYSTEM 0x40000000

#define OS_RESETCODE_EXEC 0xC0000000
#define OS_RESETCODE_NETCONFIG 0xC0010000

#define OS_RESET_TIMEOUT OSMillisecondsToTicks(1000)

#define OS_RESET_RESTART 0
#define OS_RESET_HOTRESET 1
#define OS_RESET_SHUTDOWN 2

#define OS_RESET_PRIO_SO 110
#define OS_RESET_PRIO_IP 111
#define OS_RESET_PRIO_CARD 127
#define OS_RESET_PRIO_PAD 127
#define OS_RESET_PRIO_GX 127
#define OS_RESET_PRIO_ALARM 4294967295

typedef BOOL (*OSResetFunction)(BOOL final);
typedef struct OSResetFunctionInfo OSResetFunctionInfo;

typedef struct OSResetFunctionQueue {
  OSResetFunctionInfo* head;
  OSResetFunctionInfo* tail;
} OSResetFunctionQueue;

struct OSResetFunctionInfo {
  // public
  OSResetFunction func;
  u32 priority;

  // private
  OSResetFunctionInfo* next;
  OSResetFunctionInfo* prev;
};

void OSRegisterResetFunction(OSResetFunctionInfo* info);
BOOL __OSCallResetFunctions(BOOL final);
void __OSDoHotReset(u32 resetCode);
void OSResetSystem(s32 reset, u32 resetCode, BOOL forceMenu);
u32 OSGetResetCode(void);

#ifdef __cplusplus
}
#endif

#endif // _DOLPHIN_OSRESET
