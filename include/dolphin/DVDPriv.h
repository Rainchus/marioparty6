#ifndef _DOLPHIN_DVDPRIV
#define _DOLPHIN_DVDPRIV

#include <dolphin/dvd.h>
#include <dolphin/hw_regs.h>
#include <dolphin/os.h>
#include <types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct DVDDriveInfo {
  u16 revisionLevel;
  u16 deviceCode;
  u32 releaseDate;
  u8 padding[24];
} DVDDriveInfo;

typedef struct DVDBB1 {
  u32 appLoaderLength;
  void* appLoaderFunc1;
  void* appLoaderFunc2;
  void* appLoaderFunc3;
} DVDBB1;

typedef struct DVDBB2 {
  u32 bootFilePosition;
  u32 FSTPosition;
  u32 FSTLength;
  u32 FSTMaxLength;
  void* FSTAddress;
  u32 userPosition;
  u32 userLength;

  u32 padding0;
} DVDBB2;

typedef void (*DVDOptionalCommandChecker)(DVDCommandBlock* block, void (*cb)(u32 intType));
typedef void (*DVDLowCallback)(u32 intType);

DVDCommandChecker __DVDSetOptionalCommandChecker(DVDCommandChecker func);
void __DVDSetImmCommand(u32 command);
void __DVDSetDmaCommand(u32 command);
void* __DVDGetIssueCommandAddr(void);
void __DVDAudioBufferConfig(DVDCommandBlock* block, u32 enable, u32 size,
                            DVDCBCallback callback);
int __DVDTestAlarm(const OSAlarm* alarm);
void __DVDStoreErrorCode(u32 error);
void __DVDPrintFatalMessage(void);

extern OSThreadQueue __DVDThreadQueue;
extern u32 __DVDLongFileNameFlag;

void __DVDFSInit(void);

DVDDiskID* DVDGetCurrentDiskID(void);
BOOL DVDLowRead(void* addr, u32 length, u32 offset, DVDLowCallback callback);
BOOL DVDLowSeek(u32 offset, DVDLowCallback callback);
BOOL DVDLowWaitCoverClose(DVDLowCallback callback);
BOOL DVDLowReadDiskID(DVDDiskID* diskID, DVDLowCallback callback);
BOOL DVDLowStopMotor(DVDLowCallback callback);
BOOL DVDLowRequestError(DVDLowCallback callback);
BOOL DVDLowInquiry(DVDDriveInfo* info, DVDLowCallback callback);
BOOL DVDLowAudioStream(u32 subcmd, u32 length, u32 offset, DVDLowCallback callback);
BOOL DVDLowRequestAudioStatus(u32 subcmd, DVDLowCallback callback);
BOOL DVDLowAudioBufferConfig(BOOL enable, u32 size, DVDLowCallback callback);
void DVDLowReset(void);
DVDLowCallback DVDLowSetResetCoverCallback(DVDLowCallback callback);
BOOL DVDLowBreak(void);
DVDLowCallback DVDLowClearCallback(void);
u32 DVDLowGetCoverStatus(void);

void __DVDInitWA(void);
void __DVDInterruptHandler(__OSInterrupt interrupt, OSContext* context);
void __DVDLowSetWAType(u32 type, s32 location);
int __DVDLowTestAlarm(const OSAlarm* alarm);

void __DVDClearWaitingQueue(void);
int __DVDPushWaitingQueue(s32 prio, DVDCommandBlock* block);
DVDCommandBlock* __DVDPopWaitingQueue(void);
int __DVDCheckWaitingQueue(void);
int __DVDDequeueWaitingQueue(DVDCommandBlock* block);
int __DVDIsBlockInWaitingQueue(DVDCommandBlock* block);

void __fstLoad(void);
void __DVDPrepareResetAsync(DVDCBCallback callback);

#ifdef __cplusplus
}
#endif // _DOLPHIN_DVDPRIV

#endif __DVDPRIV_H__
