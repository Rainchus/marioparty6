#ifndef _DOLPHIN_DVD
#define _DOLPHIN_DVD

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DVD_ASSERTMSGLINE(line, cond, msg) \
  if (!(cond))                             \
  OSPanic(__FILE__, line, msg)

#define DVD_ASSERTMSG1LINE(line, cond, msg, arg1) \
  if (!(cond))                                    \
  OSPanic(__FILE__, line, msg, arg1)

#define DVD_ASSERTMSG2LINE(line, cond, msg, arg1, arg2) \
  if (!(cond))                                          \
  OSPanic(__FILE__, line, msg, arg1, arg2)

#define DVD_MIN_TRANSFER_SIZE 32

#define DVD_STATE_FATAL_ERROR -1
#define DVD_STATE_END 0
#define DVD_STATE_BUSY 1
#define DVD_STATE_WAITING 2
#define DVD_STATE_COVER_CLOSED 3
#define DVD_STATE_NO_DISK 4
#define DVD_STATE_COVER_OPEN 5
#define DVD_STATE_WRONG_DISK 6
#define DVD_STATE_MOTOR_STOPPED 7
#define DVD_STATE_PAUSING 8
#define DVD_STATE_IGNORED 9
#define DVD_STATE_CANCELED 10
#define DVD_STATE_RETRY 11

#define DVD_FILEINFO_READY 0
#define DVD_FILEINFO_BUSY 1

#define DVD_RESULT_GOOD 0
#define DVD_RESULT_FATAL_ERROR -1
#define DVD_RESULT_IGNORED -2
#define DVD_RESULT_CANCELED -3

#define DVD_AIS_SUCCESS 0x0

#define DVD_INTTYPE_TC 1
#define DVD_INTTYPE_DE 2
#define DVD_INTTYPE_CVR 4

#define DVD_COMMAND_NONE 0
#define DVD_COMMAND_READ 1
#define DVD_COMMAND_SEEK 2
#define DVD_COMMAND_CHANGE_DISK 3
#define DVD_COMMAND_BSREAD 4
#define DVD_COMMAND_READID 5
#define DVD_COMMAND_INITSTREAM 6
#define DVD_COMMAND_CANCELSTREAM 7
#define DVD_COMMAND_STOP_STREAM_AT_END 8
#define DVD_COMMAND_REQUEST_AUDIO_ERROR 9
#define DVD_COMMAND_REQUEST_PLAY_ADDR 10
#define DVD_COMMAND_REQUEST_START_ADDR 11
#define DVD_COMMAND_REQUEST_LENGTH 12
#define DVD_COMMAND_AUDIO_BUFFER_CONFIG 13
#define DVD_COMMAND_INQUIRY 14
#define DVD_COMMAND_BS_CHANGE_DISK 15
#define DVD_COMMAND_UNK_16 16

typedef struct DVDDiskID {
  char gameName[4];
  char company[2];
  u8 diskNumber;
  u8 gameVersion;
  u8 streaming;
  u8 streamingBufSize; // 0 = default
  u8 padding[22];      // 0's are stored
} DVDDiskID;

BOOL DVDCompareDiskID(const DVDDiskID* id1, const DVDDiskID* id2);

typedef struct DVDCommandBlock DVDCommandBlock;

typedef void (*DVDCBCallback)(s32 result, DVDCommandBlock* block);
typedef void (*DVDCommandCheckerCallback)(u32 intType);
typedef void (*DVDCommandChecker)(DVDCommandBlock* block, DVDCommandCheckerCallback callback);

struct DVDCommandBlock {
  DVDCommandBlock* next;
  DVDCommandBlock* prev;
  u32 command;
  s32 state;
  u32 offset;
  u32 length;
  void* addr;
  u32 currTransferSize;
  u32 transferredSize;
  DVDDiskID* id;
  DVDCBCallback callback;
  void* userData;
};

typedef struct DVDFileInfo DVDFileInfo;

typedef void (*DVDCallback)(s32 result, DVDFileInfo* fileInfo);

struct DVDFileInfo {
  DVDCommandBlock cb;
  u32 startAddr;
  u32 length;
  DVDCallback callback;
};

typedef struct {
  u32 entryNum;
  u32 location;
  u32 next;
} DVDDir;

typedef struct {
  u32 entryNum;
  BOOL isDir;
  char* name;
} DVDDirEntry;

void DVDInit();
int DVDReadAbsAsyncPrio(DVDCommandBlock* block, void* addr, s32 length, s32 offset,
                        DVDCBCallback callback, s32 prio);
BOOL DVDSetAutoInvalidation(BOOL autoInvalidation);
void DVDResume(void);
DVDDiskID* DVDGetCurrentDiskID(void);
BOOL DVDCheckDisk(void);
BOOL DVDClose(DVDFileInfo* f);
BOOL DVDSetAutoFatalMessaging(BOOL);
void DVDReset();
s32 DVDCancel(volatile DVDCommandBlock* block);
BOOL DVDOpen(char* fileName, DVDFileInfo* fileInfo);
BOOL DVDFastOpen(s32 entrynum, DVDFileInfo* fileInfo);
s32 DVDGetCommandBlockStatus(const DVDCommandBlock* block);
BOOL DVDCancelAsync(DVDCommandBlock* block, DVDCBCallback callback);
s32 DVDCancel(volatile DVDCommandBlock* block);
BOOL DVDCancelAllAsync(DVDCBCallback callback);
s32 DVDCancelAll(void);
BOOL DVDPrepareStreamAsync(DVDFileInfo* fInfo, u32 length, u32 offset, DVDCallback callback);
s32 DVDPrepareStream(DVDFileInfo* fInfo, u32 length, u32 offset);

BOOL DVDCancelStreamAsync(DVDCommandBlock* block, DVDCBCallback callback);
s32 DVDCancelStream(DVDCommandBlock* block);

BOOL DVDStopStreamAtEndAsync(DVDCommandBlock* block, DVDCBCallback callback);
s32 DVDStopStreamAtEnd(DVDCommandBlock* block);

BOOL DVDGetStreamErrorStatusAsync(DVDCommandBlock* block, DVDCBCallback callback);
s32 DVDGetStreamErrorStatus(DVDCommandBlock* block);

BOOL DVDGetStreamPlayAddrAsync(DVDCommandBlock* block, DVDCBCallback callback);
s32 DVDGetStreamPlayAddr(DVDCommandBlock* block);

s32 DVDGetDriveStatus();

s32 DVDConvertPathToEntrynum(char* pathPtr);

BOOL DVDReadAsyncPrio(DVDFileInfo* fileInfo, void* addr, s32 length, s32 offset,
                      DVDCallback callback, s32 prio);
               
BOOL DVDReadPrio(DVDFileInfo* fileInfo, void* addr, s32 length, s32 offset, s32 prio);

			   
#define DVDReadAsync(fileInfo, addr, length, offset, callback)                                     \
  DVDReadAsyncPrio((fileInfo), (addr), (length), (offset), (callback), 2)
#define DVDRead(fileInfo, addr, length, offset)                 \
                         DVDReadPrio((fileInfo), (addr), (length), (offset), 2)
                         
#define DVDSeekAsync(fileInfo, offset, callback)                                                   \
  DVDSeekAsyncPrio((fileInfo), (offset), (callback), 2)

#ifdef __cplusplus
}
#endif

#endif // _DOLPHIN_DVD
