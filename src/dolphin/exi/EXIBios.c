#include <dolphin.h>
#include <dolphin/exi.h>
#include <dolphin/hw_regs.h>
#include <string.h>

#define REG_MAX 5
#define REG(chan, index) (__EXIRegs[((chan) * REG_MAX) + (index)])

#define STATE_DMA 1
#define STATE_IMM 2
#define STATE_BUSY 3
#define STATE_SELECTED 4
#define STATE_ATTACHED 8
#define STATE_LOCKED 16

#define MAX_CHAN 3
#define MAX_DEV 3

#define EXI_0LENGTH_EXILENGTH_MASK 0x03FFFFE0
#define EXI_0CR(tstart, dma, rw, tlen) \
  ((((u32)(tstart)) << 0) | (((u32)(dma)) << 1) | (((u32)(rw)) << 2) | \
   (((u32)(tlen)) << 4))

typedef struct EXIControl {
  EXICallback exiCallback;
  EXICallback tcCallback;
  EXICallback extCallback;
  volatile u32 state;
  int immLen;
  u8* immBuf;
  u32 dev;
  u32 id;
  s32 idTime;
  int items;
  struct {
    u32 dev;
    EXICallback callback;
  } queue[MAX_DEV];
} EXIControl;

const char* __EXIVersion =
    "<< Dolphin SDK - EXI\trelease build: Apr  5 2004 04:14:14 (0x2301) >>";

static EXIControl Ecb[MAX_CHAN];
static u32 IDSerialPort1;
s32 __EXIProbeStartTime[2] AT_ADDRESS(OS_BASE_CACHED | 0x30C0);

extern void __OSEnableBarnacle(s32 chan, u32 dev);

static BOOL __EXIProbe(s32 chan);
static BOOL __EXIAttach(s32 chan, EXICallback extCallback);

#pragma scheduling off

static void SetExiInterruptMask(s32 chan, EXIControl* exi) {
  EXIControl* exi2;

  exi2 = &Ecb[2];
  switch (chan) {
    case 0:
      if ((exi->exiCallback == NULL && exi2->exiCallback == NULL) ||
          (exi->state & STATE_LOCKED)) {
        __OSMaskInterrupts(OS_INTERRUPTMASK_EXI_0_EXI | OS_INTERRUPTMASK_EXI_2_EXI);
      } else {
        __OSUnmaskInterrupts(OS_INTERRUPTMASK_EXI_0_EXI | OS_INTERRUPTMASK_EXI_2_EXI);
      }
      break;
    case 1:
      if (exi->exiCallback == NULL || (exi->state & STATE_LOCKED)) {
        __OSMaskInterrupts(OS_INTERRUPTMASK_EXI_1_EXI);
      } else {
        __OSUnmaskInterrupts(OS_INTERRUPTMASK_EXI_1_EXI);
      }
      break;
    case 2:
      if (__OSGetInterruptHandler(__OS_INTERRUPT_PI_DEBUG) == NULL ||
          (exi->state & STATE_LOCKED)) {
        __OSMaskInterrupts(OS_INTERRUPTMASK_PI_DEBUG);
      } else {
        __OSUnmaskInterrupts(OS_INTERRUPTMASK_PI_DEBUG);
      }
      break;
  }
}

static void CompleteTransfer(s32 chan) {
  EXIControl* exi = &Ecb[chan];
  u8* buf;
  u32 data;
  int i;
  int len;

  if (exi->state & STATE_BUSY) {
    if ((exi->state & STATE_IMM) && (len = exi->immLen)) {
      buf = exi->immBuf;
      data = REG(chan, 4);
      for (i = 0; i < len; i++) {
        *buf++ = (u8)((data >> ((3 - i) * 8)) & 0xFF);
      }
    }
    exi->state &= ~STATE_BUSY;
  }
}

BOOL EXIImm(s32 chan, void* buf, s32 len, u32 type, EXICallback callback) {
  EXIControl* exi = &Ecb[chan];
  BOOL enabled;

  enabled = OSDisableInterrupts();
  if ((exi->state & STATE_BUSY) || !(exi->state & STATE_SELECTED)) {
    OSRestoreInterrupts(enabled);
    return FALSE;
  }

  exi->tcCallback = callback;
  if (exi->tcCallback != NULL) {
    EXIClearInterrupts(chan, FALSE, TRUE, FALSE);
    __OSUnmaskInterrupts(OS_INTERRUPTMASK_EXI_0_TC >> (chan * 3));
  }

  exi->state |= STATE_IMM;
  if (type != EXI_READ) {
    u32 data;
    int i;

    data = 0;
    for (i = 0; i < len; i++) {
      data |= ((u8*)buf)[i] << ((3 - i) * 8);
    }
    REG(chan, 4) = data;
  }

  exi->immBuf = buf;
  exi->immLen = type != EXI_WRITE ? len : 0;
  REG(chan, 3) = EXI_0CR(1, 0, type, len - 1);
  OSRestoreInterrupts(enabled);
  return TRUE;
}

BOOL EXIImmEx(s32 chan, void* buf, s32 len, u32 mode) {
  s32 xLen;

  while (len) {
    xLen = (len < 4) ? len : 4;
    if (!EXIImm(chan, buf, xLen, mode, NULL)) {
      return FALSE;
    }
    if (!EXISync(chan)) {
      return FALSE;
    }
    (u8*)buf += xLen;
    len -= xLen;
  }

  return TRUE;
}

BOOL EXIDma(s32 chan, void* buf, s32 len, u32 type, EXICallback callback) {
  EXIControl* exi = &Ecb[chan];
  BOOL enabled;

  enabled = OSDisableInterrupts();
  if ((exi->state & STATE_BUSY) || !(exi->state & STATE_SELECTED)) {
    OSRestoreInterrupts(enabled);
    return FALSE;
  }

  exi->tcCallback = callback;
  if (exi->tcCallback != NULL) {
    EXIClearInterrupts(chan, FALSE, TRUE, FALSE);
    __OSUnmaskInterrupts(OS_INTERRUPTMASK_EXI_0_TC >> (chan * 3));
  }

  exi->state |= STATE_DMA;
  REG(chan, 1) = (u32)buf & EXI_0LENGTH_EXILENGTH_MASK;
  REG(chan, 2) = len;
  REG(chan, 3) = EXI_0CR(1, 1, type, 0);
  OSRestoreInterrupts(enabled);
  return TRUE;
}

BOOL EXISync(s32 chan) {
  EXIControl* exi = &Ecb[chan];
  BOOL result = FALSE;
  BOOL enabled;

  while (exi->state & STATE_SELECTED) {
    if (((REG(chan, 3) & 1) >> 0) == 0) {
      enabled = OSDisableInterrupts();
      if (exi->state & STATE_SELECTED) {
        CompleteTransfer(chan);
        if (__OSGetDIConfig() != 0xFF ||
            (OSGetConsoleType() & 0xF0000000) == 0x20000000 || exi->immLen != 4 ||
            (REG(chan, 0) & 0x70) ||
            (REG(chan, 4) != EXI_USB_ADAPTER && REG(chan, 4) != EXI_IS_VIEWER &&
             REG(chan, 4) != EXI_ETHER_VIEWER) ||
            __OSDeviceCode == 0x8200) {
          result = TRUE;
        }
      }
      OSRestoreInterrupts(enabled);
      break;
    }
  }

  return result;
}

u32 EXIClearInterrupts(s32 chan, BOOL exiInterrupt, BOOL transferInterrupt,
                       BOOL externalInterrupt) {
  u32 current;
  u32 next;

  current = next = REG(chan, 0);
  next &= 0x7F5;
  if (exiInterrupt) {
    next |= 2;
  }
  if (transferInterrupt) {
    next |= 8;
  }
  if (externalInterrupt) {
    next |= 0x800;
  }
  REG(chan, 0) = next;
  return current;
}

EXICallback EXISetExiCallback(s32 chan, EXICallback callback) {
  EXIControl* exi = &Ecb[chan];
  EXICallback previous;
  BOOL enabled;

  enabled = OSDisableInterrupts();
  previous = exi->exiCallback;
  exi->exiCallback = callback;
  if (chan != 2) {
    SetExiInterruptMask(chan, exi);
  } else {
    SetExiInterruptMask(0, &Ecb[0]);
  }
  OSRestoreInterrupts(enabled);
  return previous;
}

void EXIProbeReset(void) {
  __EXIProbeStartTime[0] = __EXIProbeStartTime[1] = 0;
  Ecb[0].idTime = Ecb[1].idTime = 0;
  __EXIProbe(0);
  __EXIProbe(1);
}

static BOOL __EXIProbe(s32 chan) {
  EXIControl* exi = &Ecb[chan];
  BOOL enabled;
  BOOL result;
  u32 cpr;
  s32 time;

  if (chan == 2) {
    return TRUE;
  }

  result = TRUE;
  enabled = OSDisableInterrupts();
  cpr = REG(chan, 0);
  if (!(exi->state & STATE_ATTACHED)) {
    if (cpr & 0x800) {
      EXIClearInterrupts(chan, FALSE, FALSE, TRUE);
      __EXIProbeStartTime[chan] = exi->idTime = 0;
    }

    if (cpr & 0x1000) {
      time = (s32)(OSTicksToMilliseconds(OSGetTime()) / 100) + 1;
      if (__EXIProbeStartTime[chan] == 0) {
        __EXIProbeStartTime[chan] = time;
      }
      if (time - __EXIProbeStartTime[chan] < 3) {
        result = FALSE;
      }
    } else {
      __EXIProbeStartTime[chan] = exi->idTime = 0;
      result = FALSE;
    }
  } else if (!(cpr & 0x1000) || (cpr & 0x800)) {
    __EXIProbeStartTime[chan] = exi->idTime = 0;
    result = FALSE;
  }
  OSRestoreInterrupts(enabled);
  return result;
}

BOOL EXIProbe(s32 chan) {
  EXIControl* exi = &Ecb[chan];
  BOOL result;
  u32 id;

  result = __EXIProbe(chan);
  if (result && exi->idTime == 0) {
    result = EXIGetID(chan, 0, &id) ? TRUE : FALSE;
  }
  return result;
}

s32 EXIProbeEx(s32 chan) {
  if (EXIProbe(chan)) {
    return 1;
  } else if (__EXIProbeStartTime[chan] != 0) {
    return 0;
  } else {
    return -1;
  }
}

static BOOL __EXIAttach(s32 chan, EXICallback callback) {
  EXIControl* exi = &Ecb[chan];
  BOOL enabled;

  enabled = OSDisableInterrupts();
  if ((exi->state & STATE_ATTACHED) || !__EXIProbe(chan)) {
    OSRestoreInterrupts(enabled);
    return FALSE;
  }

  EXIClearInterrupts(chan, TRUE, FALSE, FALSE);
  exi->extCallback = callback;
  __OSUnmaskInterrupts(OS_INTERRUPTMASK_EXI_0_EXT >> (chan * 3));
  exi->state |= STATE_ATTACHED;
  OSRestoreInterrupts(enabled);
  return TRUE;
}

BOOL EXIAttach(s32 chan, EXICallback callback) {
  EXIControl* exi = &Ecb[chan];
  BOOL enabled;
  BOOL result;

  EXIProbe(chan);
  enabled = OSDisableInterrupts();
  if (exi->idTime == 0) {
    OSRestoreInterrupts(enabled);
    return FALSE;
  }
  result = __EXIAttach(chan, callback);
  OSRestoreInterrupts(enabled);
  return result;
}

BOOL EXIDetach(s32 chan) {
  EXIControl* exi = &Ecb[chan];
  BOOL enabled;

  enabled = OSDisableInterrupts();
  if (!(exi->state & STATE_ATTACHED)) {
    OSRestoreInterrupts(enabled);
    return TRUE;
  }
  if ((exi->state & STATE_LOCKED) && exi->dev == 0) {
    OSRestoreInterrupts(enabled);
    return FALSE;
  }
  exi->state &= ~STATE_ATTACHED;
  __OSMaskInterrupts((OS_INTERRUPTMASK_EXI_0_EXT | OS_INTERRUPTMASK_EXI_0_EXI) >> (chan * 3));
  OSRestoreInterrupts(enabled);
  return TRUE;
}

BOOL EXISelectSD(s32 chan, u32 dev, u32 frequency) {
  EXIControl* exi = &Ecb[chan];
  u32 cpr;
  BOOL enabled;

  enabled = OSDisableInterrupts();
  if ((exi->state & STATE_SELECTED) ||
      (chan != 2 && ((dev == 0 && !(exi->state & STATE_ATTACHED) && !EXIProbe(chan)) ||
                     !(exi->state & STATE_LOCKED) || exi->dev != dev))) {
    OSRestoreInterrupts(enabled);
    return FALSE;
  }

  exi->state |= STATE_SELECTED;
  cpr = REG(chan, 0);
  cpr &= 0x405;
  cpr |= frequency * 0x10;
  REG(chan, 0) = cpr;

  if (exi->state & STATE_ATTACHED) {
    switch (chan) {
      case 0:
        __OSMaskInterrupts(OS_INTERRUPTMASK_EXI_0_EXT);
        break;
      case 1:
        __OSMaskInterrupts(OS_INTERRUPTMASK_EXI_1_EXT);
        break;
    }
  }
  OSRestoreInterrupts(enabled);
  return TRUE;
}

BOOL EXISelect(s32 chan, u32 dev, u32 frequency) {
  EXIControl* exi = &Ecb[chan];
  u32 cpr;
  BOOL enabled;

  enabled = OSDisableInterrupts();
  if ((exi->state & STATE_SELECTED) ||
      (chan != 2 && ((dev == 0 && !(exi->state & STATE_ATTACHED) && !__EXIProbe(chan)) ||
                     !(exi->state & STATE_LOCKED) || exi->dev != dev))) {
    OSRestoreInterrupts(enabled);
    return FALSE;
  }

  exi->state |= STATE_SELECTED;
  cpr = REG(chan, 0);
  cpr &= 0x405;
  cpr |= ((1 << dev) << 7) | (frequency * 0x10);
  REG(chan, 0) = cpr;

  if (exi->state & STATE_ATTACHED) {
    switch (chan) {
      case 0:
        __OSMaskInterrupts(OS_INTERRUPTMASK_EXI_0_EXT);
        break;
      case 1:
        __OSMaskInterrupts(OS_INTERRUPTMASK_EXI_1_EXT);
        break;
    }
  }
  OSRestoreInterrupts(enabled);
  return TRUE;
}

BOOL EXIDeselect(s32 chan) {
  EXIControl* exi = &Ecb[chan];
  u32 cpr;
  BOOL enabled;

  enabled = OSDisableInterrupts();
  if (!(exi->state & STATE_SELECTED)) {
    OSRestoreInterrupts(enabled);
    return FALSE;
  }
  exi->state &= ~STATE_SELECTED;
  cpr = REG(chan, 0);
  REG(chan, 0) = cpr & 0x405;

  if (exi->state & STATE_ATTACHED) {
    switch (chan) {
      case 0:
        __OSUnmaskInterrupts(OS_INTERRUPTMASK_EXI_0_EXT);
        break;
      case 1:
        __OSUnmaskInterrupts(OS_INTERRUPTMASK_EXI_1_EXT);
        break;
    }
  }
  OSRestoreInterrupts(enabled);

  if (chan != 2 && (cpr & 0x80)) {
    return __EXIProbe(chan) ? TRUE : FALSE;
  }
  return TRUE;
}

static void EXIIntrruptHandler(__OSInterrupt interrupt, OSContext* context) {
  s32 chan;
  EXIControl* exi;
  EXICallback callback;

  chan = (interrupt - __OS_INTERRUPT_EXI_0_EXI) / 3;
  exi = &Ecb[chan];
  EXIClearInterrupts(chan, TRUE, FALSE, FALSE);
  callback = exi->exiCallback;
  if (callback != NULL) {
    OSContext exceptionContext;

    OSClearContext(&exceptionContext);
    OSSetCurrentContext(&exceptionContext);
    callback(chan, context);
    OSClearContext(&exceptionContext);
    OSSetCurrentContext(context);
  }
}

static void TCIntrruptHandler(__OSInterrupt interrupt, OSContext* context) {
  s32 chan;
  EXIControl* exi;
  EXICallback callback;

  chan = (interrupt - __OS_INTERRUPT_EXI_0_TC) / 3;
  exi = &Ecb[chan];
  __OSMaskInterrupts(OS_INTERRUPTMASK(interrupt));
  EXIClearInterrupts(chan, FALSE, TRUE, FALSE);
  callback = exi->tcCallback;
  if (callback != NULL) {
    OSContext exceptionContext;

    exi->tcCallback = NULL;
    CompleteTransfer(chan);
    OSClearContext(&exceptionContext);
    OSSetCurrentContext(&exceptionContext);
    callback(chan, context);
    OSClearContext(&exceptionContext);
    OSSetCurrentContext(context);
  }
}

static void EXTIntrruptHandler(__OSInterrupt interrupt, OSContext* context) {
  s32 chan;
  EXIControl* exi;
  EXICallback callback;

  chan = (interrupt - __OS_INTERRUPT_EXI_0_EXT) / 3;
  __OSMaskInterrupts((OS_INTERRUPTMASK_EXI_0_EXT | OS_INTERRUPTMASK_EXI_0_EXI) >>
                     (chan * 3));
  exi = &Ecb[chan];
  callback = exi->extCallback;
  exi->state &= ~STATE_ATTACHED;
  if (callback != NULL) {
    OSContext exceptionContext;

    OSClearContext(&exceptionContext);
    OSSetCurrentContext(&exceptionContext);
    exi->extCallback = NULL;
    callback(chan, context);
    OSClearContext(&exceptionContext);
    OSSetCurrentContext(context);
  }
}

void EXIInit(void) {
  u32 id;

  while (((REG(0, 3) & 1) == 1) || ((REG(1, 3) & 1) == 1) ||
         ((REG(2, 3) & 1) == 1)) {
  }

  __OSMaskInterrupts(OS_INTERRUPTMASK_EXI);
  REG(0, 0) = 0;
  REG(1, 0) = 0;
  REG(2, 0) = 0;
  REG(0, 0) = 0x2000;
  __OSSetInterruptHandler(__OS_INTERRUPT_EXI_0_EXI, EXIIntrruptHandler);
  __OSSetInterruptHandler(__OS_INTERRUPT_EXI_0_TC, TCIntrruptHandler);
  __OSSetInterruptHandler(__OS_INTERRUPT_EXI_0_EXT, EXTIntrruptHandler);
  __OSSetInterruptHandler(__OS_INTERRUPT_EXI_1_EXI, EXIIntrruptHandler);
  __OSSetInterruptHandler(__OS_INTERRUPT_EXI_1_TC, TCIntrruptHandler);
  __OSSetInterruptHandler(__OS_INTERRUPT_EXI_1_EXT, EXTIntrruptHandler);
  __OSSetInterruptHandler(__OS_INTERRUPT_EXI_2_EXI, EXIIntrruptHandler);
  __OSSetInterruptHandler(__OS_INTERRUPT_EXI_2_TC, TCIntrruptHandler);

  EXIGetID(0, 2, &IDSerialPort1);
  if (__OSInIPL) {
    EXIProbeReset();
  } else if (EXIGetID(0, 0, &id) && id == 0x07010000) {
    __OSEnableBarnacle(1, 0);
  } else if (EXIGetID(1, 0, &id) && id == 0x07010000) {
    __OSEnableBarnacle(0, 2);
  }
  OSRegisterVersion(__EXIVersion);
}

BOOL EXILock(s32 chan, u32 dev, EXICallback callback) {
  EXIControl* exi = &Ecb[chan];
  BOOL enabled;
  int i;

  enabled = OSDisableInterrupts();
  if (exi->state & STATE_LOCKED) {
    if (callback != NULL) {
      for (i = 0; i < exi->items; i++) {
        if (exi->queue[i].dev == dev) {
          OSRestoreInterrupts(enabled);
          return FALSE;
        }
      }
      exi->queue[exi->items].callback = callback;
      exi->queue[exi->items].dev = dev;
      exi->items++;
    }
    OSRestoreInterrupts(enabled);
    return FALSE;
  }

  exi->state |= STATE_LOCKED;
  exi->dev = dev;
  SetExiInterruptMask(chan, exi);
  OSRestoreInterrupts(enabled);
  return TRUE;
}

BOOL EXIUnlock(s32 chan) {
  EXIControl* exi = &Ecb[chan];
  BOOL enabled;
  EXICallback callback;

  enabled = OSDisableInterrupts();
  if (!(exi->state & STATE_LOCKED)) {
    OSRestoreInterrupts(enabled);
    return FALSE;
  }

  exi->state &= ~STATE_LOCKED;
  SetExiInterruptMask(chan, exi);
  if (exi->items > 0) {
    callback = exi->queue[0].callback;
    if (--exi->items > 0) {
      memmove(&exi->queue[0], &exi->queue[1], exi->items * sizeof(exi->queue[0]));
    }
    callback(chan, NULL);
  }
  OSRestoreInterrupts(enabled);
  return TRUE;
}

u32 EXIGetState(s32 chan) {
  EXIControl* exi = &Ecb[chan];

  return exi->state;
}

static void UnlockedHandler(s32 chan, OSContext* context) {
  u32 id;

  EXIGetID(chan, 0, &id);
}

s32 EXIGetID(s32 chan, u32 dev, u32* id) {
  EXIControl* exi = &Ecb[chan];
  BOOL error;
  u32 command;
  s32 startTime;
  BOOL enabled;

  if (chan == 0 && dev == 2 && IDSerialPort1 != 0) {
    *id = IDSerialPort1;
    return 1;
  }

  if (chan < 2 && dev == 0) {
    if (!__EXIProbe(chan)) {
      return 0;
    }
    if (exi->idTime == __EXIProbeStartTime[chan]) {
      *id = exi->id;
      return exi->idTime;
    }
    if (!__EXIAttach(chan, NULL)) {
      return 0;
    }
    startTime = __EXIProbeStartTime[chan];
  }

  enabled = OSDisableInterrupts();
  error = !EXILock(chan, dev, chan < 2 && dev == 0 ? UnlockedHandler : NULL);
  if (!error) {
    error = !EXISelect(chan, dev, EXI_FREQ_1M);
    if (!error) {
      command = 0;
      error |= !EXIImm(chan, &command, 2, EXI_WRITE, NULL);
      error |= !EXISync(chan);
      error |= !EXIImm(chan, id, 4, EXI_READ, NULL);
      error |= !EXISync(chan);
      error |= !EXIDeselect(chan);
    }
    EXIUnlock(chan);
  }
  OSRestoreInterrupts(enabled);

  if (chan < 2 && dev == 0) {
    EXIDetach(chan);
    enabled = OSDisableInterrupts();
    error |= __EXIProbeStartTime[chan] != startTime;
    if (!error) {
      exi->id = *id;
      exi->idTime = startTime;
    }
    OSRestoreInterrupts(enabled);
    if (error) {
      return 0;
    }
    return exi->idTime;
  }

  return error ? 0 : 1;
}

s32 EXIGetType(s32 chan, u32 dev, u32* type) {
  u32 deviceType;
  s32 probe;

  probe = EXIGetID(chan, dev, &deviceType);
  if (!probe) {
    return probe;
  }

  switch (deviceType & ~0xFF) {
    case 0x04020300:
    case EXI_ETHER:
    case 0x04020100:
    case EXI_MIC:
      *type = deviceType & ~0xFF;
      return probe;
    default:
      switch (deviceType & ~0xFFFF) {
        case 0:
          if (!(deviceType & 0x3803)) {
            switch (deviceType & 0xFC) {
              case EXI_MEMORY_CARD_59:
              case EXI_MEMORY_CARD_123:
              case EXI_MEMORY_CARD_251:
              case EXI_MEMORY_CARD_507:
              case EXI_MEMORY_CARD_1019:
              case EXI_MEMORY_CARD_2043:
                *type = deviceType & 0xFC;
                return probe;
            }
          }
          break;
        case EXI_IS_VIEWER:
          *type = EXI_IS_VIEWER;
          return probe;
      }
      *type = deviceType;
      return probe;
  }
}

char* EXIGetTypeString(u32 type) {
  switch (type) {
    case EXI_MEMORY_CARD_59:
      return "Memory Card 59";
    case EXI_MEMORY_CARD_123:
      return "Memory Card 123";
    case EXI_MEMORY_CARD_251:
      return "Memory Card 251";
    case EXI_MEMORY_CARD_507:
      return "Memory Card 507";
    case EXI_MEMORY_CARD_1019:
      return "Memory Card 1019";
    case EXI_MEMORY_CARD_2043:
      return "Memory Card 2043";
    case EXI_USB_ADAPTER:
      return "USB Adapter";
    case EXI_NPDP_GDEV:
      return "GDEV";
    case EXI_MODEM:
      return "Modem";
    case EXI_MARLIN:
      return "Marlin";
    case EXI_AD16:
      return "AD16";
    case EXI_RS232C:
      return "RS232C";
    case 0x80000020:
    case 0x80000080:
    case 0x80000040:
    case 0x80000008:
    case 0x80000010:
    case 0x80000004:
      return "Net Card";
    case EXI_ETHER_VIEWER:
      return "Artist Ether";
    case 0x04020100:
    case 0x04020300:
    case EXI_ETHER:
    case 0x04220000:
      return "Broadband Adapter";
    case EXI_MIC:
      return "Mic";
    case EXI_STREAM_HANGER:
      return "Stream Hanger";
    case EXI_IS_VIEWER:
      return "IS-DOL-VIEWER";
    default:
      return "Unknown";
  }
}

#pragma scheduling reset
