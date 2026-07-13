#define _MATH_H
#include "dolphin.h"
#include "game/armem.h"
#include "game/data.h"
#include "game/gamework.h"
#include "game/hu3d.h"
#include "game/main.h"
#include "game/memory.h"
#include "game/object.h"
#include "game/pad.h"
#include "game/process.h"
#include "game/sprite.h"
#include "game/window.h"

typedef u8 undefined1;
typedef u16 undefined2;
typedef u32 undefined4;
typedef u64 undefined8;
typedef u32 uint;
typedef u8 byte;
typedef u16 ushort;
typedef u64 ulonglong;
typedef s64 longlong;
typedef void (*MCResponseCallback)(u16 *response);

extern s32 currentHeapHandle;

void *memcpy(void *, const void *, u32);
void *memset(void *, int, u32);
char *strcpy(char *, const char *);
char *strcat(char *, const char *);
int strcmp(const char *, const char *);
u32 strlen(const char *);
double log10(double);
void *HuMemDirectRealloc(HEAPID, void *, s32);
s32 WipeCheckIn(void);
void MICInit(void);
void M2SInit(void);
void M2SOpen(void);
void M2SClose(void);

typedef struct MicResultNode_s MicResultNode_s;

static void InitMCSelWin(void);
static void MCExtHandler(void);
static void MCSelWinFunc(void);
static void MCSelWinContextProc(void);
static void MCAnswerMain(void);
static void MCDeviceMesExec(void);
static void MCListenerFunc(void);
static void *MCThreadFunc(void *);
static void MCThreadWakeup(OSAlarm *, OSContext *);
static void MicNotifyCallBack(undefined4, s32, undefined4, uint);
static void MicResultCallBack(undefined4, undefined4, undefined4);
static void MicResultExec(undefined4, undefined4, MicResultNode_s *);
static s32 MicResultGet(s32);
static s32 MicWriteResponseBuf(undefined2, undefined2 *);
static void *MCDVDRead(const char *);
static char *MakeMCFilename(char *, char *);
static s32 ActivateContext(s16);
void HuMCMicSet(s32);
inline s32 HuMCMicSaveGet(void);
int HuMCProbe(undefined4);
s32 HuMCMount(s32);
void HuMCMicSprKill(void);
void HuMCSelWinKill(void);
BOOL HuMCSelWinCheck(void);
void HuMCSelModeSet(s16);
void HuMCSelWinContextKill(void);
void HuMCSessionExportReset(void);
static int MicWriteResponse(s16, s32, void *, s16, s32);

#define s_Silence_Error_8023abff "Silence Error!!!!!!!!!!\n"
#define lbl_8023ABFF s_Silence_Error_8023abff
#define s_Mount_OK__8023ac18 "Mount OK!\n"
#define s_MIC_Error__Engine__x___8023ac23 "MIC Error: Engine(%x).\n"
#define s_threshold__d_not_reached_8023ace6 (lbl_8023A988 + 0x35e)
#define s_Abnormal_condition___d__s_at_sam_8023ad00 (lbl_8023A988 + 0x378)
#define s_SPEECH_DETECTED_8023ad28 (lbl_8023A988 + 0x3a0)
#define s_LISTENING_8023ad39 (lbl_8023A988 + 0x3b1)
#define s_STOP_LISTEN_8023ad44 (lbl_8023A988 + 0x3bc)
#define s_SILENCE_DETECTED_8023ad51 (lbl_8023A988 + 0x3c9)

typedef struct MCVolData_s {
    s16 *sample;
    s32 sampleNo;
    u32 index;
} MCVolData_s;

typedef struct MCLanguageData_s {
    char japanese[0x27];
    char english[0x29];
    char *file[6];
} MCLanguageData_s;

typedef struct MCLngFileTbl_s {
    char *file[6];
} MCLngFileTbl_s;

typedef struct MCResponseEntry_s {
    u32 value0;
    u32 value1;
} MCResponseEntry_s;

typedef struct MCResponseData_s {
    u32 data[0x18];
} MCResponseData_s;

typedef struct MCContextData_s {
    char path[0x40];
    void *ctxData;
    void *gcdData;
    void *wrdData;
    void *binData;
    void *context;
} MCContextData_s;

typedef struct MCUnkResponseEntry_s {
    u16 score;
    u16 unk;
    s16 *word;
} MCUnkResponseEntry_s;

typedef struct MCUnkResponse_s {
    s16 status;
    u8 pad[0xe];
    MCUnkResponseEntry_s entry[10];
} MCUnkResponse_s;

struct MicResultNode_s {
    u32 unk0;
    u16 count;
    u16 score;
    s32 *result;
    struct MicResultNode_s *next;
};

typedef struct MicResponseEntry_s {
    u16 score;
    s16 count;
    s16 *result;
} MicResponseEntry_s;

typedef struct MCSelWinWork_s {
    s16 winId;
    s16 choice;
    HUPROCESS *proc;
    f32 x;
    f32 y;
    u8 *item;
    u8 *order;
} MCSelWinWork_s;

/* .bss emission order = first-reference order (per compiled function), with
 * never-referenced statics sinking to the end — the declarations below only
 * document the layout. PlayerSession carries ATTRIBUTE_ALIGN(32): it is the
 * first .bss object, and the section's 32-byte alignment is what produces the
 * 0x18 link-time gap at 0x802870A8..0x802870C0 that dtk renders as a leading
 * pad object (pad_08_802870A8_bss) — there is no such object in this TU. */
static u8 lbl_80287554[0x108];
static MCVolData_s MCVolData;
static char MCFileName[0x40];
static OSAlarm MCThreadAlarm;
static u8 MCUnkResponseData[0x60];
static MCSelWinWork_s MCSelWinWork;
static u8 MCCurResponse[0x60];
static u32 PlayerSession[4] ATTRIBUTE_ALIGN(32);
static OSMessageQueue MCMessageQueue;
static OSThread MCThread;

static u32 gap_10_802C0564_sbss;
static u8 *MCThreadStack;
static void *LngData;
static void *M2SBuffer;
static void *MicBuffer;
static s16 *MCResultData;
static u16 gap_10_802C054E_sbss;
static s16 MCResultNum;
static s32 MCResponseLastNo;
static s32 MCResponseNo;
static MCContextData_s *MCContext;
static u16 gap_10_802C053E_sbss;
static s16 ContextCur;
static MCContextData_s *MCContextP;
static s32 MCStat;
static HUPROCESS *MCAnswerProc;
static s32 MCListenF;
static MCResponseCallback MCContextCallback;
static u16 gap_10_802C0526_sbss;
static s16 MCSprGrpId;
static OSMessage MCMessageArray[2];
static OSThreadQueue MCThreadQueue;
static s32 MC_gsapiEngineError;
static s32 MicOpenF;
static void *MCSessionP;
static s8 MCSessionTimer[4];
static MCResponseCallback MCCallback;
static s32 MCSelWinMaxTime;
static s32 ValidResultF;
static s32 LanguageNo;
static s32 MCWrongDeviceF;
static HUPROCESS *MCListenerProc;
static s32 MCInitF;
static u32 MCButtonDown;
static u32 MCButton;
static u8 *MCResponseBuf;
static void *MC_gsapiEngine;
static u32 pad_10_802C04D4_sbss;

static s32 MCSprStat = 3;
static s16 MCSessionCur = -1;
static s16 MCSessionPrev = -1;
static s32 HeapNum = 0x30000000;
static s16 MCYesNoCtxId = -1;
static s32 MCMicValue = -1;
static s32 MCThreshold = 4000;
static s32 M2SShift = -1;

static char lbl_802BF984[] = "US";
static char lbl_802BF987[] = ".ctx";
static char lbl_802BF98C[] = ".gcd";
static char lbl_802BF991[] = ".wrd";
static char lbl_802BF996[] = ".bin";
static char lbl_802BF99B[] = "Ouch!\n";
static char lbl_802BF9A2[] = "BAD SNR";
static char gap_09_802BF9AA_sdata[6] = {0, 0, 0, 0, 0, 0};

static char lbl_8023A988[] = "/mic/lng/asr16v220_jpj200_float_4b.lng";
static char lbl_8023A9AF[] = "/mic/lng/asr16v220_enu300_float_4b.lng";
static char *LngFileTbl[] = {
    lbl_8023A988, lbl_8023A9AF, lbl_8023A9AF,
    lbl_8023A9AF, lbl_8023A9AF, lbl_8023A9AF
};


static const f32 lbl_802C1E48 = 0.0f;
static const f32 lbl_802C1E4C = -100.0f;
static const f64 lbl_802C1E50 = -10000.0;
static const f32 lbl_802C1E58 = 16.0f;
static const f64 lbl_802C1E60 = -10001.0;
static const f32 lbl_802C1E68 = 560.0f;
static const f32 lbl_802C1E6C = 0.8f;
static const f64 lbl_802C1E70 = -10002.0;
static const f32 lbl_802C1E78 = 40.0f;
static const f64 lbl_802C1E80 = -10003.0;
static const f32 lbl_802C1E88 = 440.0f;
static const f32 lbl_802C1E8C = 60.0f;
static const f64 lbl_802C1E90 = 4503601774854144.0;
static const f32 lbl_802C1E98 = -10000.0f;
static const f32 lbl_802C1E9C = 200.0f;
static const f32 lbl_802C1EA0 = 0.9f;
static const f32 lbl_802C1EA4 = 288.0f;
static const f32 lbl_802C1EA8 = 240.0f;
static const f32 lbl_802C1EAC = 20.0f;
static const f64 lbl_802C1EB0 = 10.0;
static const f32 lbl_802C1EB8 = 1099511600000.0f;
static const f64 lbl_802C1EC0 = 4503599627370496.0;

/* 80091828 HuMCSysInit */

void HuMCSysInit(void)

{
  short sVar2;

  MCResponseBuf = (u8 *)(MCContext = 0);
  MCInitF = 0;
  MicOpenF = 1;
  MCAnswerProc = 0;
  MCThreadStack = 0;
  ContextCur = 0xffff;
  HeapNum = 0x30000000;
  InitMCSelWin();
  OSSetErrorHandler(0x10,0);
  __OSFpscrEnableBits = __OSFpscrEnableBits & 0xffffffef;
  if (HuMCProbe(1) == 0) {
    HuMCMicSet(1);
  }
  else {
    HuMCMicSet(2);
  }
  MCSessionCur = MCSessionPrev = -1;
  for (sVar2 = 0; sVar2 < 4; sVar2++) {
    PlayerSession[sVar2] = 0;
    *(undefined1 *)((int)&MCSessionTimer + (int)sVar2) = 0;
  }
  return;
}


/* 80091918 HuMCInit */

s32 HuMCInit(s16 mountResult)
{
  s32 apiResult;
  s16 i;
  s32 result;
  s32 rawLanguage;
  s32 heapCheck;

  mountResult;
  (void)mountResult;
  (void)rawLanguage;
  result = 0;
  if (MCResponseBuf != 0) {
    return;
  }
  MCWrongDeviceF = 0;
  if (MCMicValue != -1) {
    HuMCMicSet(MCMicValue);
  }
  MCResponseBuf = HuMemDirectMallocNum(0,0x6000,0x30000000);
  MCResponseNo = MCResponseLastNo = 0;
  MCResultData = HuMemDirectMallocNum(0,0x80,0x30000000);
  MCResultNum = 0;
  MCContext = HuMemDirectMallocNum(0,sizeof(MCContextData_s) * 8,0x30000000);
  for (i = 0; i < 8; i++) {
    MCContext[i].context = 0;
  }
  MCSprGrpId = -1;
  ContextCur = -1;
  MCYesNoCtxId = -1;
  if (HuMCMicSaveGet() != 1) {
    return 0;
  }

  M2SShift = -1;
  gsapi_SetUserData(1000);
  if (MC_gsapiEngine != 0) {
    gsapi_EngineClose(MC_gsapiEngine);
  }
  MCThreshold = 4000;
  MICInit();
  M2SInit();
  if ((M2SBuffer = HuMemDirectMallocNum(0,0x100,0x30000000)) == 0) {
    OSReport("Mic Error: Not enough Memory\n");
    HuMemDirectFreeNum(0,0x30000000);
    return -128;
  }
  if (M2SSetBuffer(M2SBuffer) == 0) {
    OSReport("Mic Error: M2SSetBuffer()\n");
    HuMemDirectFreeNum(0,0x30000000);
    return -128;
  }
  if ((MicBuffer = HuMemDirectMallocNum(0,0x3000,0x30000000)) == 0) {
    OSReport("Mic Error: Not enough Memory\n");
    HuMemDirectFreeNum(0,0x30000000);
    return -128;
  }

  result = HuMCMount(1);
  M2SSetPrerecordSamples(100);
  M2SSetMode(3);
  MICStart(1);
  if ((apiResult = gsapi_Init(MicResultCallBack,0)) != 0) {
    OSReport("Mic Error: gsapi_Init() %x\n",apiResult);
    HuMemDirectFreeNum(0,0x30000000);
    return -128;
  }
  rawLanguage = GwCommon.languageNo;
  mountResult = rawLanguage;
  LanguageNo = mountResult;
  if ((LngData = MCDVDRead(LngFileTbl[LanguageNo])) == 0) {
    OSReport("Mic Error: Read Language Failue\n");
    gsapi_Close();
    HuMemDirectFreeNum(0,0x30000000);
    return -128;
  }
  if ((apiResult = gsapi_LanguageLoadBuffer(LngData,0)) != 0) {
    OSReport("Mic Error: gsapi_LanguageLoadBuffer().%x\n",apiResult);
    gsapi_Close();
    HuMemDirectFreeNum(0,0x30000000);
    return -128;
  }
  if ((apiResult = gsapi_EngineOpen(0,&MC_gsapiEngine)) != 0) {
    OSReport("Mic Error: gsapi_EngineOpen().%x\n",apiResult);
    gsapi_Close();
    HuMemDirectFreeNum(0,0x30000000);
    return -128;
  }
  MCSessionP = 0;
  if ((apiResult = gsapi_NotifySetCallback(MicNotifyCallBack)) != 0) {
    OSReport("Mic Error: gsapi_NotifySetCallback().%x\n",apiResult);
    gsapi_Close();
    HuMemDirectFreeNum(0,0x30000000);
    return -128;
  }
  if ((apiResult = gsapi_EngineSetMode(MC_gsapiEngine,1)) != 0) {
    OSReport("Mic Error: gsapi_EngineSetMode().%x\n",apiResult);
    gsapi_Close();
    HuMemDirectFreeNum(0,0x30000000);
    return -128;
  }

  gsapi_EngineSetParam(MC_gsapiEngine,9,MCThreshold);
  MCStat = 0;
  MCListenF = 0;
  MCContextCallback = 0;
  OSInitMessageQueue(&MCMessageQueue,MCMessageArray,2);
  MCThreadStack = HuMemDirectMallocNum(0,0x4000,0x30000000);
  OSCreateThread(&MCThread,MCThreadFunc,0,MCThreadStack + 0x4000,
                 0x4000,0x1f,1);
  OSResumeThread(&MCThread);
  OSInitThreadQueue(&MCThreadQueue);
  MCInitF = 1;
  OSReport("HEAP HEAP Malloc Size %x\n",HuMemUsedMallocSizeGet(0));
  heapCheck = OSCheckHeap(currentHeapHandle);
  OSReport("OSAlloc Size Left %dkb(%x)\n",OSCheckHeap(currentHeapHandle) / 1024,heapCheck);
  MCAnswerProc = HuPrcCreate(MCAnswerMain,65000,0x4000,0);
  HuPrcSetStat(MCAnswerProc,0xc);
  MCMicValue = -1;
  return result;
}


/* 80091de0 HuMCClose */

void HuMCClose(void)

{
  if (MCResponseBuf != 0) {
    MCInitF = 0;
    if ((MCThreadStack != 0) && (HuMCMicSaveGet() == 1)) {
      OSSendMessage(&MCMessageQueue,(OSMessage)2,1);
      OSSleepThread(&MCThreadQueue);
    }
    HuMCMicSprKill();
    if (HuMCMicSaveGet() == 1) {
      MICStop(0);
      MICStop(1);
    }
    HuMCSessionExportReset();
    if (MCAnswerProc != 0) {
      HuPrcKill(MCAnswerProc);
    }
    MCAnswerProc = 0;
    if (MCListenerProc != 0) {
      HuPrcKill(MCListenerProc);
    }
    MCListenerProc = 0;
    if (MCThreadStack != 0) {
      OSCancelThread(&MCThread);
    }
    MCThreadStack = 0;
    HuMCSelWinKill();
    if (MC_gsapiEngine != 0) {
      gsapi_EngineClose(MC_gsapiEngine);
      gsapi_LanguageUnLoad();
      gsapi_EngineClose(MC_gsapiEngine);
      gsapi_Close();
      MC_gsapiEngine = 0;
    }
    HuMemDirectFreeNum(0,0x30000000);
    HuMemDirectFreeNum(2,0x30000000);
    MCResponseBuf = (u8 *)(MCContext = 0);
    MICUnmount(0);
    MICUnmount(1);
    if (MCMicValue != -1) {
      HuMCMicSet(MCMicValue);
    }
    MCMicValue = -1;
  }
  return;
}


/* 80091f40 HuMCContextCreate */

s16 HuMCContextCreate(char *path)

{
  char *temp;
  char *scan;
  char *filename;
  MCContextData_s *context;
  s16 index;
  s32 error;

  if (HuMCMicSaveGet() != 1) {
    return -1;
  }
  temp = HuMemDirectMalloc(0,0x100);
  strcpy(temp,path);
  if (LanguageNo != 0) {
    scan = temp;
    while ((*scan != '\0') && (*scan != '.')) {
      scan++;
    }
    switch (LanguageNo) {
      case 1:
        strcat(temp,lbl_802BF984);
        break;
      default:
        strcat(temp,lbl_802BF984);
        break;
    }
  }
  for (index = 0; index < 8; index++) {
    if (MCContext[index].context == 0) {
      break;
    }
    if (strcmp(path,MCContext[index].path) == 0) {
      HuMemDirectFree(temp);
      return index;
    }
  }
  if (index == 8) {
    HuMemDirectFree(temp);
    return -1;
  }
  context = &MCContext[index];
  strcpy(context->path,temp);
  filename = MakeMCFilename(temp,lbl_802BF987);
  context->ctxData = MCDVDRead(filename);
  filename = MakeMCFilename(temp,lbl_802BF98C);
  context->gcdData = MCDVDRead(filename);
  filename = MakeMCFilename(temp,lbl_802BF991);
  context->wrdData = MCDVDRead(filename);
  filename = MakeMCFilename(temp,lbl_802BF996);
  context->binData = MCDVDRead(filename);
  error = gsapi_ContextSetCtxData(context->ctxData,&context->context);
  if (error != 0) {
    OSReport("Error CTX %x\n",error);
  }
  error = gsapi_ContextSetGcdData(context->context,context->gcdData);
  if (error != 0) {
    OSReport("Error GCD %x\n",error);
  }
  error = gsapi_ContextSetWrdData(context->context,context->wrdData);
  if (error != 0) {
    OSReport("Error WRD %x\n",error);
  }
  gsapi_ContextSetParam(context->context,9,MCThreshold);
  if ((omcurovl == 0x4a) || (omcurovl == 0x4b) || (omcurovl == 0x46) ||
      (omcurovl == 0x66) || (omcurovl == 0x67) || (omcurovl == 0x69) ||
      (omcurovl == 0x68)) {
    gsapi_ContextSetParam(context->context,10,200);
  }
  else {
    gsapi_ContextSetParam(context->context,10,100);
  }
  HuMemDirectFree(temp);
  return index;
}


/* 8009221c HuMCContextKill */

void HuMCContextKill(short param_1)

{
  MCContextData_s *context;

  if (param_1 >= 0) {
    context = &MCContext[param_1];
    if (context->context) {
      gsapi_ContextDeActivate(context->context);
      HuMemDirectFree(context->ctxData);
      HuMemDirectFree(context->gcdData);
      HuMemDirectFree(context->wrdData);
      context->context = NULL;
    }
  }
}


/* 8009229c HuMCThresholdSet */

void HuMCThresholdSet(undefined4 param_1)

{
  MCThreshold = param_1;
  return;
}


/* 800922a4 HuMCContextSet */

static char lbl_8023AB60[] = "MIC Error: gsapi_ContextActivate() %x\n";

s32 HuMCContextSet(s16 param_1)

{
  s32 result;
  if (HuMCSelWinCheck()) {
    MCResponseNo = MCResponseLastNo = 0;
    HuMCSelModeSet(1);
    return 0;
  }
  if (HuMCMicSaveGet() != 1) {
    return 0;
  }
  if ((MCStat != 0) || (MicOpenF != 0) || (param_1 < 0) || (MC_gsapiEngine == 0)) {
    return 0;
  }
  if ((result = ActivateContext(param_1)) != 0) {
    OSReport(lbl_8023AB60,result);
    return 0;
  }
  MCContextP = &MCContext[param_1];
  MCResponseNo = MCResponseLastNo = 0;
  gsapi_EngineSetMode(MC_gsapiEngine,1);
  gsapi_EngineSetParam(MC_gsapiEngine,1,0);
  gsapi_EngineSetParam(MC_gsapiEngine,0,0);
  OSSendMessage(&MCMessageQueue,0,1);
  MCStat = 1;
  MCListenF = 0;
  MCContextCallback = 0;
  return 1;
}


/* 800923f4 HuMCStatGet */

static char lbl_8023AB87[] = "Error!!!!!!!!!!!!!!!!!!\n";

s32 HuMCStatGet(void)

{
  if (HuMCSelWinCheck()) {
    while (MCResponseNo == 0) {
      HuPrcVSleep();
    }
    return HuMCResponseGet2();
  }
  if (HuMCMicSaveGet() != 1) {
    return -1;
  }
  if ((MCStat != 1) || (MicOpenF != 0)) {
    return -2;
  }
  while (MCResponseNo == 0) {
    if (MCListenF != 0) {
      OSReport(lbl_8023AB87);
      OSSendMessage(&MCMessageQueue,(OSMessage)2,1);
      OSSleepThread(&MCThreadQueue);
      OSSendMessage(&MCMessageQueue,0,1);
      MCListenF = 0;
    }
    HuPrcVSleep();
  }
  return HuMCResponseGet2();
}


/* 800924d4 HuMCResponseGet2 */

int HuMCResponseGet2(void)

{
  s16 response;

  if (HuMCSelWinCheck()) {
    HuMCSelModeSet(2);
    if (*(short *)(MCResponseBuf + MCResponseLastNo * 0x60 + 4)) {
      goto sel_response_valid;
    }
    return -1;
sel_response_valid:
    return *(s32 *)(MCResponseBuf + MCResponseLastNo * 0x60 + 0xc);
  }
  if (HuMCMicSaveGet() != 1) {
    return -1;
  }
  if ((MCStat != 1) || (MicOpenF != 0)) {
    return -2;
  }
  OSSendMessage(&MCMessageQueue,(OSMessage)2,1);
  OSSleepThread(&MCThreadQueue);
  MCStat = 0;
  if (MCResponseNo == 0) {
    return -1;
  }
  response = *(s16 *)(MCResponseBuf + MCResponseLastNo * 0x60);
  if (response != 0) {
    return response;
  }
  if (MCResponseNo == MCResponseLastNo) {
    return -1;
  }
  if (*(s16 *)(MCResponseBuf + MCResponseLastNo * 0x60 + 4)) {
    goto response_valid;
  }
  return -1;
response_valid:
  return *(s32 *)(MCResponseBuf + MCResponseLastNo * 0x60 + 0xc);
}

/* 80092628 HuMCContextCallbackSet */

s32 HuMCContextCallbackSet(s16 param_1,MCResponseCallback param_2)

{
  s32 result;

  if (HuMCSelWinCheck()) {
    if (param_2 != 0) {
      MCContextCallback = param_2;
    }
    MCResponseNo = MCResponseLastNo = 0;
    HuMCSelModeSet(1);
    return 0;
  }
  if (HuMCMicSaveGet() != 1) {
    return 0;
  }
  if ((MCStat != 0) || (MicOpenF != 0) || (param_1 < 0) || (MC_gsapiEngine == 0)) {
    return 0;
  }
  if ((result = ActivateContext(param_1)) != 0) {
    OSReport(lbl_8023AB60,result);
    return 0;
  }
  ContextCur = param_1;
  MCContextP = &MCContext[param_1];
  MCResponseLastNo = MCResponseNo = 0;
  gsapi_EngineSetMode(MC_gsapiEngine,2);
  gsapi_EngineSetParam(MC_gsapiEngine,1,1);
  gsapi_EngineSetParam(MC_gsapiEngine,0,1);
  OSSendMessage(&MCMessageQueue,0,1);
  if (param_2 != 0) {
    MCContextCallback = param_2;
  }
  else {
    MCContextCallback = 0;
  }
  MCStat = 2;
  return 1;
}


/* 8009279c HuMCResponseGet */

int HuMCResponseGet(void)

{
  s16 response;

  if (HuMCSelWinCheck()) {
    HuMCSelModeSet(2);
    if (*(short *)(MCResponseBuf + MCResponseLastNo * 0x60 + 4)) {
      goto response_valid;
    }
    return -1;
response_valid:
    return *(s32 *)(MCResponseBuf + MCResponseLastNo * 0x60 + 0xc);
  }
  if (HuMCMicSaveGet() != 1) {
    return -1;
  }
  if ((MCStat != 2) || (MicOpenF != 0)) {
    return -1;
  }
  MCContextCallback = 0;
  OSSendMessage(&MCMessageQueue,(OSMessage)2,1);
  OSSleepThread(&MCThreadQueue);
  response = *(s16 *)(MCResponseBuf + MCResponseLastNo * 0x60);
  MCStat = 0;
  if (response != 0) {
    return response;
  }
  if (MCResponseNo == MCResponseLastNo) {
    return -1;
  }
  return *(s32 *)(MCResponseBuf + MCResponseLastNo * 0x60 + 0xc);
}


/* 800928c0 HuMCCurResponseGet */

u16 *HuMCCurResponseGet(void)
{
    if (MCResponseNo > MCResponseLastNo) {
        *(MCResponseData_s *)MCCurResponse =
            *(MCResponseData_s *)(MCResponseBuf + (MCResponseLastNo * 0x60));
        MCResponseLastNo++;
        if (MCResponseNo <= MCResponseLastNo) {
            MCResponseLastNo = MCResponseNo;
        }
    } else {
        *(s16 *)&MCCurResponse[0] = -1;
        *(u16 *)&MCCurResponse[2] = 0xFFFF;
        *(s16 *)&MCCurResponse[4] = 0;
        *(s32 *)&MCCurResponse[8] = 0;
        *(s32 *)&MCCurResponse[12] = -1;
        MCResponseLastNo = MCResponseNo;
    }
    return (u16 *)MCCurResponse;
}


/* 8009299c HuMCButtonGet */

s32 HuMCButtonGet(void)

{
  if ((HuMCMicSaveGet() != 1) || (MCResponseBuf == 0)) {
    return 0;
  }
  return MCButton;
}


/* 800929dc HuMCButtonDownGet */

s32 HuMCButtonDownGet(void)

{
  if ((HuMCMicSaveGet() != 1) || (MCResponseBuf == 0)) {
    return 0;
  }
  return MCButtonDown;
}


/* 80092a1c HuMCProbe */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

int HuMCProbe(undefined4 param_1)
{
  int result;
  OSTick start;

  start = OSGetTick();
  while (OSTicksToMilliseconds(OSGetTick() - start) < 500) {
    result = MICProbeEx(param_1);
    if (result != -1) {
      break;
    }
  }
  return result;
}


/* 80092aa0 HuMCMount */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

static char lbl_8023ABA0[] = "MIC Error: M2SSetActiveChannel() %d\n";
static char lbl_8023ABC5[] = "M2SSetShifts Error\n";

inline s32 HuMCMount(s32 chan)
{
  s32 result;
  s32 idResult;
  s32 activeResult;
  s32 probeResult2;
  OSTick start;
  s32 probeResult;
  s32 deviceId;

  M2SClose();
  MicOpenF = 1;
  MCWrongDeviceF = 0;
  start = OSGetTick();
  while (OSTicksToMilliseconds(OSGetTick() - start) < 500) {
    if ((probeResult = MICProbeEx(chan)) != -1) {
      break;
    }
  }
  probeResult2 = probeResult;
  if ((result = probeResult2) != 0) {
    return result;
  }
  if ((result = MICMount(chan, MicBuffer, 0x3000, MCExtHandler)) != 0) {
    if (result != -4) {
      return result;
    }
    result = 0;
  }
  idResult = MICGetDeviceID(chan, &deviceId);
  if (idResult != 0) {
    return idResult;
  }
  if (deviceId != 0) {
    MCWrongDeviceF = 1;
    return -2;
  }
  activeResult = M2SSetActiveChannel(chan);
  if (activeResult == 0) {
    OSReport(lbl_8023ABA0, activeResult);
    return -0x80;
  }
  MICSetGain(chan, 0);
  if (M2SSetShifts(M2SShift) == 0) {
    OSReport(lbl_8023ABC5);
  }
  MicOpenF = 0;
  MICStart(1);
  M2SOpen();
  return result;
}


static inline s32 MCProbeSub(s32 chan)
{
  s32 result;
  OSTick start;

  start = OSGetTick();
  while (OSTicksToMilliseconds(OSGetTick() - start) < 500) {
    if ((result = MICProbeEx(chan)) != -1) {
      break;
    }
  }
  return result;
}

static inline s32 MCMountSub(s32 chan)
{
  s32 result;
  s32 idResult;
  s32 deviceId;
  s32 activeResult;

  M2SClose();
  MicOpenF = 1;
  MCWrongDeviceF = 0;
  if ((result = MCProbeSub(chan)) != 0) {
    return result;
  }
  if ((result = MICMount(chan, MicBuffer, 0x3000, MCExtHandler)) != 0) {
    if (result != -4) {
      return result;
    }
    result = 0;
  }
  idResult = MICGetDeviceID(chan, &deviceId);
  if (idResult != 0) {
    return idResult;
  }
  if (deviceId != 0) {
    MCWrongDeviceF = 1;
    return -2;
  }
  if ((activeResult = M2SSetActiveChannel(chan)) == 0) {
    OSReport(lbl_8023ABA0, activeResult);
    return -0x80;
  }
  MICSetGain(chan, 0);
  if (M2SSetShifts(M2SShift) == 0) {
    OSReport(lbl_8023ABC5);
  }
  MicOpenF = 0;
  MICStart(1);
  M2SOpen();
  return result;
}



/* 80092c34 MCExtHandler */

void MCExtHandler(void)

{
  MicOpenF = 1;
  M2SStop();
  OSReport(lbl_802BF99B);
  return;
}


/* 80092c68 HuMCShiftsSet */

void HuMCShiftsSet(undefined4 param_1)

{
  M2SShift = param_1;
  M2SSetShifts(param_1);
  return;
}


/* 80092c9c HuMCMicSprCreate */

void HuMCMicSprCreate(f32 param_1,f32 param_2)

{
  void *data;
  ANIMDATA *anim;
  HUSPRID sprId;
  
  if (HuMCMicSaveGet() == 1) {
    data = HuAR_ARAMtoMRAMFileRead(0xf20032,0x30000000,2);
    anim = HuSprAnimRead(data);
    MCSprGrpId = HuSprGrpCreate(1);
    sprId = HuSprCreate(anim,1,0);
    HuSprGrpMemberSet(MCSprGrpId,0,sprId);
    HuSprGrpPosSet(MCSprGrpId,lbl_802C1E48,lbl_802C1E48);
    HuSprPosSet(MCSprGrpId,0,param_1,param_2);
    HuSprAttrSet(MCSprGrpId,0,4);
  }
  return;
}


/* 80092d6c HuMCMicSprKill */

void HuMCMicSprKill(void)

{
  if (MCSprGrpId != -1) {
    HuSprGrpKill(MCSprGrpId);
    MCSprGrpId = -1;
  }
  return;
}


/* 80092da4 HuMCMicGet */

s32 HuMCMicGet(void)

{
  if (MCMicValue != -1) {
    return MCMicValue;
  }
  return GwCommon.mic;
}


/* 80092dcc HuMCMicSaveGet */

inline s32 HuMCMicSaveGet(void)

{
  return GwCommon.mic;
}


/* 80092de0 HuMCMicSet */

void HuMCMicSet(s32 param_1)

{
  if (MCResponseBuf != 0) {
    MCMicValue = param_1;
  }
  else {
    GwCommon.mic = (u8)param_1;
  }
}


/* 80092e10 InitMCSelWin */

void InitMCSelWin(void)

{
  MCSelWinWork.winId = 0xffff;
  MCSelWinWork.proc = 0;
  return;
}


/* 80092e34 HuMCSelWinCreate */

void HuMCSelWinCreate(f32 param_1,f32 param_2)

{
  s32 mic;
  MCSelWinWork.winId = 0xffff;
  mic = GwCommon.mic;
  if (mic == 2) {
    MCSelWinWork.winId = HuWinCreate((double)lbl_802C1E4C,(double)lbl_802C1E4C,0x20,0x20,0);
    MCSelWinWork.x = param_1;
    MCSelWinWork.y = param_2;
    MCSelWinWork.item = (undefined1 *)HuMemDirectMallocNum(0,0x100,0x30000000);
    *MCSelWinWork.item = 0;
    MCSelWinWork.order = HuMemDirectMallocNum(0,0x40,0x30000000);
    MCSelWinWork.choice = 0;
    MCSelWinWork.proc = HuPrcCreate(MCSelWinFunc,65000,0x2000,0);
    HuPrcSetStat(MCSelWinWork.proc,0xc);
  }
  return;
}


/* 80092f64 HuMCSelWinKill */

void HuMCSelWinKill(void)

{
  if (MCSelWinWork.winId != -1) {
    HuWinKill(MCSelWinWork.winId);
  }
  MCSelWinWork.winId = 0xffff;
  if (MCSelWinWork.proc != 0) {
    HuPrcKill(MCSelWinWork.proc);
  }
  MCSelWinWork.proc = 0;
  if (MCSelWinWork.item != 0) {
    HuMemDirectFree(MCSelWinWork.item);
  }
  MCSelWinWork.item = 0;
  if (MCSelWinWork.order != 0) {
    HuMemDirectFree(MCSelWinWork.order);
  }
  MCSelWinWork.order = 0;
  return;
}


/* 80093050 HuMCSelWinItemRandSet */

/* WARNING: Removing unreachable block (ram,0x800935ec) */
/* WARNING: Removing unreachable block (ram,0x800935f4) */

static char lbl_8023ABD9[] = "Error: HuMCSelWinItemRandSet() %d<%d\n";

s32 HuMCSelWinItemRandSet(s32 messageBase,s16 itemCount,s16 fixedItem,
                          s16 shownCount,s16 padNo)
{
  s32 temp;
  s16 randomItem;
  s16 pass;
  u8 *item;
  s16 i;
  HuVec2f size;
  f32 x;
  f32 y;

  item = MCSelWinWork.item;
  if (HuMCMicSaveGet() != 2) {
    return -1;
  }
  if (MCSelWinWork.winId != -1) {
    HuWinKill(MCSelWinWork.winId);
  }
  if (itemCount < shownCount) {
    OSReport(lbl_8023ABD9,itemCount,shownCount);
    return -1;
  }

  for (i = 0; i < itemCount; i++) {
    MCSelWinWork.order[i] = (u8)i;
  }
  temp = MCSelWinWork.order[0];
  MCSelWinWork.order[0] = MCSelWinWork.order[fixedItem];
  MCSelWinWork.order[fixedItem] = temp;
  for (pass = 0; pass < 10; pass++) {
    for (i = 1; i < itemCount; i++) {
      randomItem = frandmod(itemCount - 1) + 1;
      temp = MCSelWinWork.order[i];
      MCSelWinWork.order[i] = MCSelWinWork.order[randomItem];
      MCSelWinWork.order[randomItem] = temp;
    }
  }
  for (pass = 0; pass < shownCount - 1; pass++) {
    for (i = 0; i < shownCount - pass - 1; i++) {
      if (MCSelWinWork.order[i] > MCSelWinWork.order[i + 1]) {
        temp = MCSelWinWork.order[i];
        MCSelWinWork.order[i] = MCSelWinWork.order[i + 1];
        MCSelWinWork.order[i + 1] = temp;
      }
    }
  }

  *item++ = 0xb;
  for (i = 0; i < shownCount; i++) {
    *item++ = 0x10;
    *item++ = 0xf;
    *item++ = 0x1f;
    *item++ = (u8)(i + 1);
    if (i + 1 < shownCount) {
      *item++ = 10;
    }
    HuWinInsertMesSizeGet(messageBase + MCSelWinWork.order[i],(s16)i);
  }
  *item++ = 0;
  HuWinMesMaxSizeGet(1,&size,MCSelWinWork.item);

  if (lbl_802C1E50 == MCSelWinWork.x) {
    x = lbl_802C1E58;
  }
  else if (lbl_802C1E60 == MCSelWinWork.x) {
    x = lbl_802C1E68 - lbl_802C1E6C * size.x;
  }
  else {
    x = MCSelWinWork.x;
  }
  if (lbl_802C1E70 == MCSelWinWork.y) {
    y = lbl_802C1E78;
  }
  else if (lbl_802C1E80 == MCSelWinWork.y) {
    y = lbl_802C1E88 - lbl_802C1E6C * size.y;
  }
  else {
    y = MCSelWinWork.y;
  }

  MCSelWinWork.winId = HuWinCreate(x,y,(s16)size.x,(s16)size.y,0);
  HuWinScaleSet(MCSelWinWork.winId,lbl_802C1E6C,lbl_802C1E6C);
  for (i = 0; i < itemCount; i++) {
    HuWinInsertMesSet(MCSelWinWork.winId,messageBase + MCSelWinWork.order[i],(s16)i);
  }
  HuWinAttrSet(MCSelWinWork.winId,0x10);
  HuWinAttrSet(MCSelWinWork.winId,0x4000);
  winData[MCSelWinWork.winId].padMask = 1 << padNo;
  HuWinDispOff(MCSelWinWork.winId);
  if (MCSelWinWork.proc != 0) {
    HuPrcKill(MCSelWinWork.proc);
  }
  MCSelWinWork.proc = HuPrcCreate(MCSelWinFunc,65000,0x2000,0);
  HuPrcSetStat(MCSelWinWork.proc,0xc);
  return MCSelWinWork.winId;
}


/* 80093614 HuMCSelWinItemSet */

void HuMCSelWinItemSet(s32 param_1,s16 param_2,s16 param_3)

{
  HuMCSelWinItemRandSet(param_1,param_2,0,param_2,param_3);
  return;
}


/* 8009365c HuMCSelWinCheck */

inline BOOL HuMCSelWinCheck(void)

{
  if (MCSelWinWork.winId == -1) {
    return FALSE;
  }
  return TRUE;
}


/* 80093680 HuMCSelModeGet */

int HuMCSelModeGet(void)

{
  if (HuMCSelWinCheck() == FALSE) {
    return -1;
  }
  return MCSelWinWork.choice;
}


/* 800936d0 HuMCSelModeSet */

void HuMCSelModeSet(short param_1)

{
  HUWIN *win;
  
  if (MCSelWinWork.winId != -1) {
    if (param_1 == 2) {
      win = &winData[MCSelWinWork.winId];
      if (win->stat == 3) {
        win->stat = 0;
        win->choice = -1;
      }
    }
    MCSelWinWork.choice = param_1;
  }
}


/* 80093748 MCSelWinFunc */

static void MCSelWinFunc(void)
{
  HUWIN *win;
  MCResponseCallback callback;
  s16 choice;
  s16 timer;
  s16 response;

  for (;;) {
    switch (MCSelWinWork.choice) {
      case 1:
        if (*MCSelWinWork.item == 0) {
          MCSelWinWork.choice = 0;
          break;
        }
        HuWinDispOn(MCSelWinWork.winId);
        HuWinMesSet(MCSelWinWork.winId,(u32)MCSelWinWork.item);
        win = &winData[MCSelWinWork.winId];
        while (win->stat != 0) {
          if (omUPauseFlag + omPauseChk() != 0) {
            HuWinDispOff(MCSelWinWork.winId);
          } else {
            HuWinDispOn(MCSelWinWork.winId);
          }
          HuPrcVSleep();
        }
        HuWinChoiceSet(MCSelWinWork.winId,0);
        while (win->stat != 0) {
          if (omUPauseFlag + omPauseChk() != 0) {
            HuWinDispOff(MCSelWinWork.winId);
          } else {
            HuWinDispOn(MCSelWinWork.winId);
          }
          HuPrcVSleep();
        }
        choice = win->choice;
        if (choice >= 0) {
          response = MCSelWinWork.order[choice];
          for (timer = 0; timer < 10; timer++) {
            if (omUPauseFlag + omPauseChk() != 0) {
              HuWinDispOff(MCSelWinWork.winId);
            } else {
              HuWinDispOn(MCSelWinWork.winId);
            }
            HuPrcVSleep();
          }
          MicWriteResponse(0,1,&response,10000,response + 1);
        } else if (choice == -1) {
          MicWriteResponse(-2,0,NULL,0,-1);
        }
        if (MCContextCallback) {
          callback = MCContextCallback;
          callback(HuMCCurResponseGet());
        }
        MCSelWinWork.choice = 2;
        break;

      case 2:
        HuWinDispOff(MCSelWinWork.winId);
        MCSelWinWork.choice = 0;
        break;
    }
    HuPrcVSleep();
  }
}


/* 80093aa0 HuMCSelWinContextSet */

void HuMCSelWinContextSet(s16 param_1,MCResponseCallback param_2,u8 param_3)

{
  if (HuMCMicSaveGet() == 0) {
    return;
  }
  if (MCListenerProc != 0) {
    HuMCSelWinContextKill();
  }
  MCCallback = param_2;
  MCListenerProc = HuPrcCreate(MCSelWinContextProc,65000,0x2000,0);
  MCListenerProc->property = (void *)(((u32)param_3 << 16) | (u16)param_1);
  MCSelWinMaxTime = 0;
  return;
}


/* 80093b44 HuMCSelWinContextKill */

void HuMCSelWinContextKill(void)
{
  s16 response;

  MicWriteResponse(-1,0,NULL,0,-1);
  if (HuMCSelWinCheck()) {
    HuMCSelModeSet(2);
    (void)(MCResponseBuf + MCResponseLastNo * 0x60 + 4);
  }
  else {
    if ((HuMCMicSaveGet() == 1) && (MCStat == 1) && (MicOpenF == 0)) {
      OSSendMessage(&MCMessageQueue,(OSMessage)2,1);
      OSSleepThread(&MCThreadQueue);
      MCStat = 0;
      if (MCResponseNo != 0) {
        response = *(s16 *)(MCResponseBuf + MCResponseLastNo * 0x60);
        if ((response == 0) && (MCResponseNo != MCResponseLastNo)) {
          (void)(MCResponseBuf + MCResponseLastNo * 0x60 + 4);
        }
      }
    }
  }
  if (MCListenerProc) {
    HuPrcKill(MCListenerProc);
  }
  MCListenerProc = 0;
}


/* 80093ce8 HuMCSelWinMaxTimeSet */

void HuMCSelWinMaxTimeSet(f32 param_1)

{
  MCSelWinMaxTime = *(volatile const f32 *)&lbl_802C1E8C * param_1;
  return;
}


/* 80093d0c HuMCSelWinMaxTimeGet */

f32 HuMCSelWinMaxTimeGet(void)

{
  return (f32)MCSelWinMaxTime;
}


/* 80093d38 HuMCSelWinChoiceGet */

int HuMCSelWinChoiceGet(void)

{
  s16 *response;
  s16 zero;
  s16 index;

  MCResponseNo = MCResponseLastNo = zero = 0;
  for (;;) {
    if (zero != MCResponseNo) {
      break;
    }
    HuPrcVSleep();
  }
  index = MCResponseNo - 1;
  while (index >= 0) {
    response = (s16 *)(MCResponseBuf + index * 0x60);
    if (*response == 0) {
      break;
    }
    index--;
  }
  if (index == -1) {
    return -1;
  }
  return **(s16 **)(response + 4);
}


/* 80093dec MCSelWinContextProc */

static void MCSelWinContextProc(void)
{
  HUPROCESS *proc;
  u32 property;
  u8 padNo;
  s32 context;
  s32 button;
  s16 responseNo;
  s16 timer;
  s16 *response;

  proc = HuPrcCurrentGet();
  property = (u32)proc->property;
  padNo = property >> 16;
  context = (u16)property;
  button = HuMCButtonGet();
  for (;;) {
    if (((HuMCMicSaveGet() == 1) && (HuMCButtonDownGet() || button)) ||
        (HuMCSelWinCheck() && (HuPadBtnDown[padNo] & 0x20))) {
      button = 0;
      HuMCContextSet(context);
      MCContextCallback = MCCallback;
      if (HuMCMicSaveGet() == 1) {
        responseNo = MCResponseNo;
        while (HuMCButtonGet()) {
          if (responseNo != MCResponseNo) {
            response = (s16 *)(MCResponseBuf + (MCResponseNo - 1) * 0x60);
            if ((*response == 0) || (*response == -2) || (*response == -4)) {
              break;
            }
            responseNo = MCResponseNo;
            HuMCResponseGet2();
            HuMCContextSet(context);
            MCContextCallback = MCCallback;
          }
          if (MCSelWinMaxTime != 0) {
            MCSelWinMaxTime--;
            if (MCSelWinMaxTime == 0) {
              MicWriteResponse(-3,0,NULL,0,-1);
              break;
            }
          }
          if (MCListenF != 0) {
            OSReport(lbl_8023ABFF);
            OSSendMessage(&MCMessageQueue,(OSMessage)2,1);
            OSSleepThread(&MCThreadQueue);
            OSSendMessage(&MCMessageQueue,0,1);
            MCListenF = 0;
          }
          HuPrcVSleep();
        }
        if (responseNo == MCResponseNo) {
          timer = 0;
          while (timer < 60) {
            if (responseNo != MCResponseNo) {
              break;
            }
            HuPrcVSleep();
            timer++;
          }
          if (timer == 60) {
            MicWriteResponse(-1,0,NULL,0,-1);
          }
        }
        HuMCResponseGet2();
        break;
      }
      else {
        while (MCResponseNo == 0) {
          if (MCSelWinMaxTime != 0) {
            MCSelWinMaxTime--;
            if (MCSelWinMaxTime == 0) {
              MicWriteResponse(-3,0,NULL,0,-1);
              break;
            }
          }
          HuPrcVSleep();
        }
        HuMCResponseGet2();
        break;
      }
    }
    if (MCSelWinMaxTime != 0) {
      MCSelWinMaxTime--;
      if (MCSelWinMaxTime == 0) {
        MicWriteResponse(-3,0,NULL,0,-1);
        break;
      }
    }
    HuPrcVSleep();
  }
  MCListenerProc = NULL;
  HuPrcEnd();
  for (;;) {
    HuPrcVSleep();
  }
}


/* 8009482c HuMCListenerCreate */

void HuMCListenerCreate(s16 param_1,MCResponseCallback param_2,u8 param_3)

{
  s32 mic;

  mic = GwCommon.mic;
  if (mic != 0) {
    MCCallback = param_2;
    if (MCListenerProc != 0) {
      HuPrcKill(MCListenerProc);
    }
    MCListenerProc = HuPrcCreate(MCListenerFunc,65000,0x2000,0);
    MCListenerProc->property = (void *)(((u32)param_3 << 16) | (u16)param_1);
  }
  return;
}


/* 800948cc HuMCListenerKill */

void HuMCListenerKill(void)

{
  s16 response;
  s32 mic;

  MicWriteResponse(-1,0,0,0,-1);
  if (MCSelWinWork.winId == -1 ? FALSE : TRUE) {
    HuMCSelModeSet(2);
    (void)(MCResponseBuf + MCResponseLastNo * 0x60 + 4);
  }
  else {
    mic = GwCommon.mic;
    if (((mic == 1) && (MCStat == 2)) && (MicOpenF == 0)) {
      MCContextCallback = 0;
      OSSendMessage(&MCMessageQueue,(OSMessage)2,1);
      OSSleepThread(&MCThreadQueue);
      response = *(s16 *)(MCResponseBuf + MCResponseLastNo * 0x60);
      MCStat = 0;
      if (response == 0) {
        (void)(MCResponseNo == MCResponseLastNo);
      }
    }
  }
  if (HuMCSelWinCheck()) {
    HuMCSelWinKill();
  }
  if (MCListenerProc != 0) {
    HuPrcKill(MCListenerProc);
  }
  MCListenerProc = 0;
  return;
}


/* 80094b48 MCListenerFunc */

static void MCListenerFunc(void)
{
  HUPROCESS *process;
  u32 property;
  u8 padNo;
  u16 context;
  s16 responseNo;
  s16 timer;

  process = HuPrcCurrentGet();
  property = (u32)process->property;
  padNo = property >> 16;
  context = property;
  responseNo = 100;
  for (;;) {
    if (((HuMCMicSaveGet() == 1) && HuMCButtonGet()) ||
        (HuMCSelWinCheck() && (HuPadBtnDown[padNo] & 0x20))) {
      MCResponseCallback contextCallback;

      contextCallback = MCCallback;
      HuMCContextCallbackSet(context,contextCallback);
      responseNo = MCResponseNo;
      if (HuMCMicSaveGet() == 1) {
        while (HuMCButtonGet()) {
          HuPrcVSleep();
        }
        if (responseNo == MCResponseNo) {
          for (timer = 0; timer < 60; timer++) {
            if (responseNo != MCResponseNo) {
              break;
            }
            HuPrcVSleep();
          }
          HuPrcVSleep();
        }
      } else {
        HuPrcVSleep();
      }
      HuMCResponseGet();
    }
    if ((ValidResultF != 0) && MCCallback) {
      MCResponseCallback resultCallback;

      resultCallback = MCCallback;
      (*resultCallback)(HuMCCurResponseGet());
      ValidResultF = 0;
    }
    HuPrcVSleep();
  }
}


/* 80095034 MCAnswerMain */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

static void MCAnswerMain(void)
{
  u32 oldButton;
  s16 oldResponseNo;
  MCResponseCallback callback;

  (void)oldResponseNo;
  ValidResultF = 0;
  for (;;) {
    do {
      HuPrcVSleep();
    } while (HuMCSelWinCheck() || (HuMCMicSaveGet() != 1));

    if ((MicOpenF != 0) && (HuMCMicSaveGet() == 1)) {
      if ((omcurovl != 0x5d) && (omcurovl != 0x72) &&
          (omcurovl != 0x73) && (omcurovl != 0x74)) {
        s32 mountResult;

        mountResult = -3;
        while ((WipeCheckIn() != 0) || (Hu3DPauseF != 0)) {
          mountResult = MCMountSub(1);
          if (mountResult == 0) {
            break;
          }
          HuPrcVSleep();
        }
        if (mountResult != 0) {
          MCDeviceMesExec();
        }
        OSReport(s_Mount_OK__8023ac18);
      } else {
        while (MCMountSub(1) != 0) {
          HuPrcVSleep();
        }
      }
    }

    MICGetButton(1,&MCButton);
    MCButton &= 0x10;
    MCButtonDown = MCButton & (oldButton ^ MCButton);
    oldButton = MCButton;
    if (MCContextCallback && (oldResponseNo != MCResponseNo)) {
      if (oldResponseNo < MCResponseNo) {
        callback = MCContextCallback;
        (*callback)(HuMCCurResponseGet());
        if (ValidResultF != 0) {
          ValidResultF = 0;
        }
      }
      oldResponseNo = MCResponseNo;
    }
    if (MCSprStat == 4) {
      OSReport(s_MIC_Error__Engine__x___8023ac23,MC_gsapiEngineError);
    }
    if (MCSprGrpId != -1) {
      if (MCSprStat == 1) {
        HuSprAttrReset(MCSprGrpId,0,4);
      } else {
        HuSprAttrSet(MCSprGrpId,0,4);
      }
    }
  }
}


/* 800955e0 HuMCSessionSet */

void HuMCSessionSet(short param_1)

{
  s32 mic;

  mic = GwCommon.mic;
  if ((mic == 1) && (param_1 < 4)) {
    MCSessionCur = param_1;
  }
  return;
}


/* 8009561c HuMCSessionClose */

void HuMCSessionClose(void)

{
  short sVar1;
  
  for (sVar1 = 0; sVar1 < 4; sVar1++) {
    if (PlayerSession[sVar1] != 0) {
      gsapi_EngineSessionDataFree(PlayerSession[sVar1]);
    }
  }
  MCSessionCur = MCSessionPrev = -1;
  for (sVar1 = 0; sVar1 < 4; sVar1++) {
    PlayerSession[sVar1] = 0;
  }
  HuMemDirectFreeNum(0,0x30000001);
  return;
}


/* 800956e4 HuMCSessionKill */

void HuMCSessionKill(short param_1)

{
  s32 mic;

  mic = GwCommon.mic;
  if ((mic != 1) || (MC_gsapiEngine == 0)) {
    return;
  }
  if (PlayerSession[param_1] != 0) {
    gsapi_EngineSessionDataImport(MC_gsapiEngine,MCSessionP,0);
    gsapi_EngineSessionDataFree(PlayerSession[param_1]);
    PlayerSession[param_1] = 0;
  }
  return;
}


/* 800957a4 HuMCUnkResponseCheck */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

s32 HuMCUnkResponseCheck(void)
{
  MCUnkResponse_s *response;
  s32 i;
  s16 word[3];
  s16 score[3];
  s16 count;
  s32 j;

  *(s16 *)(MCUnkResponseData + 0x10) =
      *(s16 *)(MCUnkResponseData + 0x18) =
      *(s16 *)(MCUnkResponseData + 0x20) = 0;
  for (i = MCResponseNo - 1; i >= 0; i--) {
    response = (MCUnkResponse_s *)(MCResponseBuf + i * 0x60);
    if (response->status == 0) {
      break;
    }
  }
  if (i == -1) {
    return 0;
  }
  if (response->entry[0].score == 0) {
    return 0;
  }

  i = count = 0;
  for (; i < 3; i++) {
    if (response->entry[i].score != 0) {
      for (j = 0; j < count; j++) {
        if (*response->entry[i].word == word[j]) {
          break;
        }
      }
      if (j == count) {
        word[count] = *response->entry[i].word;
        score[count] = response->entry[i].score;
        count++;
      }
    }
  }
  if (count <= 1) {
    return 0;
  }
  if ((score[0] - score[1]) <= 1000) {
    *(MCUnkResponse_s *)MCUnkResponseData = *response;
    return 1;
  }
  return 0;
}


/* 80095968 HuMCNewResponseGet */

static char lbl_8023AC3B[] =
    "/mic/ctx/mic_yesno\0"
    "Reset Session Export >>>%x\n\0"
    "Session Export >>>%x:%x\n\0"
    "Session Import >>>%x:%x\n\0"
    "Session Import >>>%x\n";

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

s32 HuMCNewResponseGet(s32 messageBase)
{
  MCUnkResponse_s *response;
  HuVec2f size;
  s16 word[3];
  s16 score[3];
  s16 count;
  s32 i;
  s32 j;
  s32 choice;
  s32 window;

  response = (MCUnkResponse_s *)MCUnkResponseData;
  if (response->entry[0].score == 0) {
    return -1;
  }
  if (MCYesNoCtxId == -1) {
    MCYesNoCtxId = HuMCContextCreate(lbl_8023AC3B);
    if (MCYesNoCtxId == -1) {
      return *response->entry[0].word;
    }
  }

  i = count = 0;
  for (; i < 3; i++) {
    if (response->entry[i].score != 0) {
      for (j = 0; j < count; j++) {
        if (*response->entry[i].word == word[j]) {
          break;
        }
      }
      if (j == count) {
        word[count] = *response->entry[i].word;
        score[count] = response->entry[i].score;
        count++;
      }
    }
  }
  if (count <= 1) {
    return -1;
  }

  for (i = 0; i < count; i++) {
    HuWinInsertMesSizeGet(messageBase + word[i],0);
    HuWinMesMaxSizeGet(1,&size,0x1c0039);
    window = HuWinExCreateFrame(lbl_802C1E98,lbl_802C1E9C,
                               (s16)size.x,(s16)size.y,-1,0);
    HuWinExOpen(window);
    HuWinMesSpeedSet(window,0);
    HuWinAttrSet(window,0x800);
    HuWinInsertMesSet(window,messageBase + word[i],0);
    HuWinMesSet(window,0x1c0039);
    do {
      s32 context = MCYesNoCtxId;
      HuMCSelWinContextSet(context,NULL,0);
    } while (((choice = HuMCSelWinChoiceGet()) != 0) && (choice != 1));
    HuWinExClose(window);
    HuWinKill(window);
    if (choice == 0) {
      break;
    }
  }
  if (i == count) {
    return -1;
  }
  return word[i];
}


/* 80095e20 HuMCSessionExportReset */

void HuMCSessionExportReset(void)

{
  int iVar1;
  s32 mic;
  char *strings;
  
  strings = lbl_8023A988;
  mic = GwCommon.mic;
  if ((mic != 1) || (MCSessionCur == -1)) {
    return;
  }
  {
    if ((MCSessionP == 0) &&
       (iVar1 = gsapi_EngineSessionDataExport(MC_gsapiEngine,&MCSessionP), iVar1 != 0)) {
      OSReport(strings + 0x2c6,iVar1);
    }
    if (MCSessionPrev >= 0) {
      gsapi_EngineSessionDataFree(PlayerSession[MCSessionPrev]);
      HeapNum = 0x30000001;
      iVar1 = gsapi_EngineSessionDataExport(MC_gsapiEngine,PlayerSession + MCSessionPrev);
      if (iVar1 != 0) {
        OSReport(strings + 0x2e2,iVar1,PlayerSession[MCSessionPrev]);
      }
      HeapNum = 0x30000000;
    }
    MCSessionPrev = MCSessionCur;
    if (MCSessionCur == -2) {
      iVar1 = gsapi_EngineSessionDataImport(MC_gsapiEngine,MCSessionP,0);
      if (iVar1 != 0) {
        OSReport(strings + 0x2fb,iVar1,MCSessionP);
      }
    }
    else if ((PlayerSession[MCSessionCur] != 0) &&
            (iVar1 = gsapi_EngineSessionDataImport(MC_gsapiEngine,PlayerSession[MCSessionCur],0),
            iVar1 != 0)) {
      OSReport(strings + 0x314,iVar1);
    }
    if (MCSessionCur >= 0) {
      *(undefined1 *)((int)&MCSessionTimer + (int)MCSessionCur) = 0;
    }
  }
  return;
}


/* 80096008 MCDeviceMesExec */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

static void MCDeviceMesExec(void)
{
  char paused;
  u8 pauseEnable;
  s32 pad;
  ANIMDATA *anim;
  void *fileData;
  HUSPR_GROUPID group;
  HUSPRID sprite;
  HUWINID window;
  s32 result;

  pauseEnable = omSysPauseEnableFlag;
  paused = omPauseChk();

  omUPauseFlag = 1;
  omSysPauseEnable(0);
  omObjManPause(1);
  HuPrcAllPause(1);
  Hu3DPauseSet(1);
  HuSprPauseSet(1);
  HuPadRumbleAllStop();

  fileData = HuAR_ARAMtoMRAMFileRead(0xf20033,0x30000000,2);
  anim = HuSprAnimRead(fileData);
  group = HuSprGrpCreate(1);
  sprite = HuSprCreate(anim,0,0);
  HuSprGrpMemberSet(group,0,sprite);
  HuSprTPLvlSet(group,0,lbl_802C1EA0);
  HuSprGrpPosSet(group,lbl_802C1EA4,lbl_802C1EA8);
  HuSprScaleSet(group,0,lbl_802C1EAC,lbl_802C1EAC);
  window = HuWinWarningCreate(lbl_802C1E98,lbl_802C1E98,0x19c,0x78);
  HuWinPriSet(window,0);
  HuWinMesSpeedSet(window,0);
  HuWinWarningOpen(window);
  MCButton = MCButtonDown = 0;
  if (MCWrongDeviceF != 0) {
    result = -2;
  }
  else {
    result = -3;
  }

  while (result != 0) {
    if (result == -2) {
      HuWinMesSet(window,0x4a0015);
    }
    else {
      HuWinMesSet(window,0x4a0014);
    }
    HuWinMesWait(window);
    for (;;) {
      for (pad = 0; pad < 4; pad++) {
        if (HuPadBtnDown[pad] & 0x100) {
          break;
        }
      }
      if (pad != 4) {
        HuPrcVSleep();
        break;
      }
      HuPrcVSleep();
    }
    result = MCMountSub(1);
  }

  HuWinWarningClose(window);
  HuWinWarningKill(window);
  HuSprGrpKill(group);
  MICStart(1);
  if ((MCStat == 2) && (ContextCur >= 0)) {
    MCStat = 0;
    HuMCContextCallbackSet(ContextCur,MCContextCallback);
  }
  omUPauseFlag = 0;
  omSysPauseEnable(pauseEnable);
  omObjManPause(0);
  if (!paused) {
    HuPrcAllPause(0);
    Hu3DPauseSet(0);
    HuSprPauseSet(0);
  }
}


/* 80096534 MCThreadFunc */

static void *MCThreadFunc(void *arg)

{
  OSMessage message;
  s32 mic;

  for (;;) {
    if (OSReceiveMessage(&MCMessageQueue,&message,0)) {
      MCSprStat = (s32)message;
    }
    if (HuMCSelWinCheck() || (mic = GwCommon.mic, mic != 1)) {
      OSSuspendThread(&MCThread);
      continue;
    }
    if ((MicOpenF != 0) && (MCSprStat != 2)) {
      if (MCSprStat == 1) {
        MCSprStat = 2;
      }
      else {
        MCSprStat = 3;
      }
    }
    switch (MCSprStat) {
      case 0:
        MC_gsapiEngineError = gsapi_EngineStart(MC_gsapiEngine);
        if (MC_gsapiEngineError < 0) {
          MCSprStat = 4;
          OSSuspendThread(&MCThread);
        }
        else {
          MCSprStat = 1;
        }
        continue;
      case 1:
        gsapi_EngineRestart(0);
        OSSetThreadPriority(&MCThread,0x1f);
        continue;
      case 2:
        MC_gsapiEngineError = gsapi_EngineStop(MC_gsapiEngine);
        if (MC_gsapiEngineError < 0) {
          MCSprStat = 4;
          OSSuspendThread(&MCThread);
        }
        else {
          MCSprStat = 3;
        }
        OSWakeupThread(&MCThreadQueue);
        continue;
      case 3:
        OSSetThreadPriority(&MCThread,0x1f);
        continue;
    }
  }
}


/* 800966bc MCThreadWakeup */

static void MCThreadWakeup(OSAlarm *alarm, OSContext *context)

{
  OSSetThreadPriority(&MCThread,0x1f);
  return;
}


/* 800966e8 HuMCPeriodicProc */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void HuMCPeriodicProc(void)

{
  s32 mic;

  if ((MCInitF == 0) || (MCResponseBuf == 0) ||
      (mic = GwCommon.mic, mic != 1)) {
    return;
  }
  OSCreateAlarm(&MCThreadAlarm);
  OSSetAlarm(&MCThreadAlarm,(OSTime)OSMillisecondsToTicks(5),MCThreadWakeup);
  OSSetThreadPriority(&MCThread,0);
  return;
}


/* 80096794 MicNotifyCallBack */

static char lbl_8023ACB2[] = "OVERLOAD";
static char lbl_8023ACBB[] = "TOO QUIET";
static char lbl_8023ACC5[] = "NO SIGNAL";
static char lbl_8023ACCF[] = "GARBLED SOUND";
static char lbl_8023ACDD[] =
    "POOR MIC\0"
    "threshold %d not reached\n\0"
    "Abnormal condition #%d %s at sample %d\n\0"
    "SPEECH DETECTED\n\0"
    "LISTENING\n\0"
    "STOP LISTEN\n\0"
    "SILENCE DETECTED\n\0";
static char * const MCErrorTbl[] = {
    lbl_802BF9A2, lbl_8023ACB2, lbl_8023ACBB,
    lbl_8023ACC5, lbl_8023ACCF, lbl_8023ACDD
};

static void MicNotifyCallBack(undefined4 param_1,s32 event,undefined4 sample,uint value)

{
  short session1;
  short session2;
  s32 mic1;
  s32 mic2;
  char *strings;

  strings = lbl_8023A988;
  switch(event) {
  case 0:
    if (((value < 4000) && (MCSessionCur >= 0)) &&
       (MCSessionTimer[MCSessionCur]++ > 2)) {
      session1 = MCSessionCur;
      mic1 = GwCommon.mic;
      if (((mic1 == 1) && (MC_gsapiEngine != 0)) &&
         (PlayerSession[session1] != 0)) {
        gsapi_EngineSessionDataImport(MC_gsapiEngine,MCSessionP,0);
        gsapi_EngineSessionDataFree(PlayerSession[session1]);
        PlayerSession[session1] = 0;
      }
      *(undefined1 *)((int)&MCSessionTimer + (int)MCSessionCur) = 0;
    }
  case 4:
    if (event == 0) {
      OSReport(strings + 0x35e,value);
      MicWriteResponse(0xfffffffe,0,0,0,0xffffffff);
    }
    else {
      if ((MCSessionCur >= 0) && (MCSessionTimer[MCSessionCur]++ > 2)) {
        session2 = MCSessionCur;
        mic2 = GwCommon.mic;
        if ((mic2 == 1) &&
           ((MC_gsapiEngine != 0 && (PlayerSession[session2] != 0)))) {
          gsapi_EngineSessionDataImport(MC_gsapiEngine,MCSessionP,0);
          gsapi_EngineSessionDataFree(PlayerSession[session2]);
          PlayerSession[session2] = 0;
        }
        *(undefined1 *)((int)&MCSessionTimer + (int)MCSessionCur) = 0;
      }
      if (value == 1) {
        MicWriteResponse(0xfffffffc,0,0,0,0xffffffff);
      }
      OSReport(strings + 0x378,value,
               MCErrorTbl[value],sample);
    }
    break;
  case 7:
    OSReport(strings + 0x3a0);
    break;
  case 2:
    OSReport(strings + 0x3b1);
    MCListenF = 0;
    break;
  case 3:
    OSReport(strings + 0x3bc);
    MCListenF = 1;
    break;
  case 8:
    OSReport(strings + 0x3c9);
  }
}


/* 80096a60 MicResultCallBack */

void MicResultCallBack(undefined4 param_1,undefined4 param_2,undefined4 param_3)

{
  MicResultExec(param_1,param_2,(MicResultNode_s *)param_3);
  return;
}


/* 80096a98 MicResultExec */

static char lbl_8023AD88[] =
    "%d:%d(%d),%s\n\0"
    "NBEST Error %x\n\0\0";

static void MicResultExec(undefined4 param_1,undefined4 param_2,MicResultNode_s *root)

{
  MicResultNode_s *node;
  s16 *resultP;
  s16 i;
  s16 j;
  char *name;
  MicResponseEntry_s entry[10];

  node = root;
  if (root->count != 0) {
    for (node = root, i = 0; i < 10; i++) {
      if (!node || (node->count == 0)) {
        entry[i].score = 0;
        entry[i].count = 0;
        }
        else {
        if (((node->count + (-2)) + MCResultNum) >= 0x40) {
          MCResultNum = 0;
        }
        resultP = &MCResultData[MCResultNum];
        for (j = 1; j < (root->count - 1); j++) {
          MCResultData[MCResultNum++] = MicResultGet(node->result[j]);
          gsapi_EngineGetParam(MC_gsapiEngine,node->result[j],&name);
          OSReport(lbl_8023AD88,i,MicResultGet(node->result[j]),node->score,name);
        }
        entry[i].score = node->score;
        entry[i].count = node->count - 2;
        entry[i].result = resultP;
        node = node->next;
      }
    }
    MicWriteResponseBuf(0,(undefined2 *)entry);
    if (MCSessionCur >= 0) {
      MCSessionTimer[MCSessionCur] = 0;
    }
    ValidResultF = 1;
  }
  else {
    MicWriteResponse(-2,0,NULL,0,-1);
  }
}


/* 80096ca4 MicResultGet */

static s32 MicResultGet(s32 param_1)

{
  s32 count;
  s32 total;
  s32 i;
  u16 *ptr;
  
  ptr = MCContextP->binData;
  count = *ptr++;
  i = total = 0;
  for (; i < count; ptr++, i++) {
    total += *ptr;
    if (total > param_1) {
      return i;
    }
  }
  return -1;
}


/* 80096d1c MicWriteResponse */

static int MicWriteResponse(s16 param_1,s32 param_2,void *param_3,s16 param_4,
                    s32 param_5)

{
  short sVar1;
  undefined2 *puVar2;
  
  puVar2 = (undefined2 *)(MCResponseBuf + MCResponseNo * 0x60);
  *(s16 *)((u8 *)puVar2 + 0) = param_1;
  *(s16 *)((u8 *)puVar2 + 2) = param_4;
  *(s16 *)((u8 *)puVar2 + 4) = param_2;
  *(void **)(puVar2 + 4) = param_3;
  *(undefined4 *)(puVar2 + 6) = param_5;
  *(s16 *)((u8 *)puVar2 + 0x10) = param_4;
  *(s16 *)((u8 *)puVar2 + 0x12) = param_2;
  *(void **)(puVar2 + 10) = param_3;
  for (sVar1 = 1; sVar1 < 10; sVar1++) {
    *(s16 *)((u8 *)puVar2 + sVar1 * 8 + 0x10) = 0;
  }
  MCResponseNo = MCResponseNo + 1;
  if (MCResponseNo >= 0x100) {
    MCResponseNo = 0xff;
  }
  return MCResponseNo;
}


/* 80096dc0 MicWriteResponseBuf */

static s32 MicWriteResponseBuf(undefined2 param_1,undefined2 *param_2)

{
  short sVar2;
  undefined2 *puVar3;
  
  puVar3 = (undefined2 *)(MCResponseBuf + MCResponseNo * 0x60);
  *puVar3 = param_1;
  puVar3[1] = *param_2;
  *(s16 *)((u8 *)puVar3 + 4) = *(s16 *)((u8 *)param_2 + 2);
  *(undefined4 *)(puVar3 + 4) = *(undefined4 *)(param_2 + 2);
  *(int *)(puVar3 + 6) = (int)**(short **)(param_2 + 2);
  for (sVar2 = 0; sVar2 < 10; sVar2++) {
    *(MCResponseEntry_s *)((u8 *)puVar3 + sVar2 * 8 + 0x10) =
        ((MCResponseEntry_s *)param_2)[sVar2];
  }
  MCResponseNo = MCResponseNo + 1;
  if (MCResponseNo >= 0x100) {
    MCResponseNo = 0xff;
  }
  return MCResponseNo;
}


/* 80096e7c MCDVDRead */

static void *MCDVDRead(const char *param_1)

{
  void *__src;
  void *__dest;
  
  __src = (void *)HuDvdDataRead(param_1);
  __dest = (void *)HuMemDirectMallocNum(0,DirDataSize,0x30000000);
  DCFlushRange(__dest,DirDataSize + 0x1f & 0xffffffe0);
  memcpy(__dest,__src,DirDataSize);
  HuMemDirectFree(__src);
  return __dest;
}


/* 80096efc MakeMCFilename */

static char *MakeMCFilename(char *param_1,char *param_2)

{
  char *src;
  char *dst;
  
  src = param_1;
  dst = MCFileName;
  while ((*src != '\0') && (*src != '.')) {
    *dst++ = *src++;
  }
  src = param_2;
  while (*src != '\0') {
    *dst++ = *src++;
  }
  *dst = '\0';
  return MCFileName;
}


/* 80096f84 ActivateContext */

static s32 ActivateContext(s16 context)

{
  int iVar1;
  s32 result;
  s32 mic;
  char *strings;
  
  strings = lbl_8023A988;
  result = gsapi_ContextSetParam(MCContext[context].context,5,10);
  if (result != 0) {
    OSReport(strings + 0x40e,result);
  }
  result = gsapi_ContextActivate(MC_gsapiEngine,MCContext[context].context);
  mic = GwCommon.mic;
  if ((mic == 1) && (MCSessionCur != -1)) {
    if ((MCSessionP == 0) &&
       (iVar1 = gsapi_EngineSessionDataExport(MC_gsapiEngine,&MCSessionP), iVar1 != 0)) {
      OSReport(strings + 0x2c6,iVar1);
    }
    if (MCSessionPrev >= 0) {
      gsapi_EngineSessionDataFree(PlayerSession[MCSessionPrev]);
      HeapNum = 0x30000001;
      iVar1 = gsapi_EngineSessionDataExport(MC_gsapiEngine,PlayerSession + MCSessionPrev);
      if (iVar1 != 0) {
        OSReport(strings + 0x2e2,iVar1,PlayerSession[MCSessionPrev]);
      }
      HeapNum = 0x30000000;
    }
    MCSessionPrev = MCSessionCur;
    if (MCSessionCur == -2) {
      iVar1 = gsapi_EngineSessionDataImport(MC_gsapiEngine,MCSessionP,0);
      if (iVar1 != 0) {
        OSReport(strings + 0x2fb,iVar1,MCSessionP);
      }
    }
    else if ((PlayerSession[MCSessionCur] != 0) &&
            (iVar1 = gsapi_EngineSessionDataImport(MC_gsapiEngine,PlayerSession[MCSessionCur],0),
            iVar1 != 0)) {
      OSReport(strings + 0x314,iVar1);
    }
    if (MCSessionCur >= 0) {
      *(undefined1 *)((int)&MCSessionTimer + (int)MCSessionCur) = 0;
    }
  }
  return result;
}


/* 800971c4 heap_Open */

undefined4 heap_Open(undefined4 *param_1)

{
  *param_1 = 0;
  return 0;
}


/* 800971d4 heap_Close */

undefined4 heap_Close(void)

{
  HuMemDirectFreeNum(0,HeapNum);
  return 0;
}


/* 80097200 heap_Alloc */

void *heap_Alloc(void *heap, u32 size)

{
  void *result;
  result = HuMemDirectTailMallocNum(0,size,HeapNum);
  return result;
}


/* 80097240 heap_Calloc */

void *heap_Calloc(void *heap, u32 count, u32 size)

{
  void *alloc;
  void *result1;
  void *result2;
  void *result;

  alloc = HuMemDirectTailMallocNum(0,count * size,HeapNum);
  result1 = alloc;
  result2 = result1;
  result = result2;
  if (!result) {
    return 0;
  }
  memset(result,0,count * size);
  return result;
}


/* 800972b8 heap_Realloc */

void *heap_Realloc(void *heap, void *ptr, u32 size)

{
  void *alloc;
  void *result1;
  void *result2;
  void *result;

  if (ptr == 0) {
    alloc = HuMemDirectTailMallocNum(0,size,HeapNum);
    result1 = alloc;
    result2 = result1;
    result = result2;
  }
  else {
    result = HuMemDirectRealloc(0,ptr,size);
  }
  return result;
}


/* 80097330 heap_Free */

void heap_Free(void *heap, void *ptr)

{
  HuMemDirectFree(ptr);
  return;
}


/* 80097358 HuMCVolSampleCreate */

void HuMCVolSampleCreate(void)

{
  MCVolData.sample = (void *)HuMemDirectMallocNum(0,0x15888,0x30000000);
  memset(MCVolData.sample,0,0x15888);
  MCVolData.sampleNo = 0;
  MCVolData.index = 0;
  return;
}


/* 800973d0 HuMCVolGet */

/* WARNING: Removing unreachable block (ram,0x80097624) */
/* WARNING: Removing unreachable block (ram,0x8009762c) */

s32 HuMCVolGet(s32 minDb,u32 scale)

{
  BOOL interrupts;
  s16 *sample;
  s32 sampleNo;
  s32 oldSampleNo;
  s32 remaining;
  s32 got;
  s32 result;
  u32 index;
  s32 i;
  s32 value;
  f32 power;
  f32 db;

  interrupts = OSDisableInterrupts();
  if (!MICIsActive(1)) {
    memset(MCVolData.sample,0,0x15888);
    MCVolData.sampleNo = 0;
    MCVolData.index = 0;
    OSRestoreInterrupts(interrupts);
    return -1;
  }
  sample = MCVolData.sample;
  sampleNo = MCVolData.sampleNo;
  index = MCVolData.index;
  oldSampleNo = sampleNo;
  remaining = 0xac44 - sampleNo;
  got = MICGetSamples(1,sample + sampleNo,index,remaining);
  sampleNo += got;
  index = MICUpdateIndex(1,index,got);
  if (sampleNo >= 0xac44) {
    got = MICGetSamples(1,sample,index,oldSampleNo);
    sampleNo = got;
    index = MICUpdateIndex(1,index,got);
  }
  MCVolData.sampleNo = sampleNo;
  MCVolData.index = index;
  OSRestoreInterrupts(interrupts);

  sampleNo--;
  if (sampleNo < 0) {
    sampleNo += 0xac44;
  }
  power = lbl_802C1E48;
  for (i = 0; i < 0x400; i++) {
    value = sample[sampleNo];
    power += (f32)(value * value);
    sampleNo--;
    if (sampleNo < 0) {
      sampleNo += 0xac44;
    }
  }
  if (power > lbl_802C1E48) {
    db = (f32)(lbl_802C1EB0 * log10((double)(power / lbl_802C1EB8)));
  }
  else {
    db = lbl_802C1E4C;
  }
  db = (db * (f32)scale) / (f32)-minDb;
  result = (s32)db;
  result += scale;
  if (result < 0) {
    result = 0;
  }
  return result;
}
