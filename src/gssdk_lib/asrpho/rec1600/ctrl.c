#include "gssdk/tos.h"

extern u32 cbAskCurrentGain(TosContext *, u8, u8, void *, u32);
extern u32 cbCancelUserWordTraining(TosContext *, u8, u8, void *, u32);
extern u32 cbDoSecondPassIfEnabled(TosContext *, u8, u8, void *, u32);
extern u32 cbDoUserWordTraining(TosContext *, u8, u8, void *, u32);
extern u32 cbGainRequest(TosContext *, u8, u8, void *, u32);
extern u32 cbGenderDisable(TosContext *, u8, u8, void *, u32);
extern u32 cbLastUtteranceAccepted(TosContext *, u8, u8, void *, u32);
extern u32 cbNewUserWordUtterance(TosContext *, u8, u8, void *, u32);
extern u32 cbNothingRecognised(TosContext *, u8, u8, void *, u32);
extern u32 cbScoreUserWordResultAvailable(TosContext *, u8, u8, void *, u32);
extern u32 cbSpellNothingRecognised(TosContext *, u8, u8, void *, u32);
extern u32 cbSpellRecognised(TosContext *, u8, u8, void *, u32);
extern u32 cbStartDetected(TosContext *, u8, u8, void *, u32);
extern u32 cbTOSAbnormalCond(TosContext *, u8, u8, void *, u32);
extern u32 cbTOSRecognised(TosContext *, u8, u8, void *, u32);
extern u32 cbTSAfterStartDetected(TosContext *, u8, u8, void *, u32);
extern u32 cbTSDetected(TosContext *, u8, u8, void *, u32);
extern u32 cbUserWordResultAvailable(TosContext *, u8, u8, void *, u32);
extern u32 cbUtteranceAccepted(TosContext *, u8, u8, void *, u32);
extern u32 cbUtteranceNotAccepted(TosContext *, u8, u8, void *, u32);

static const struct {
    TosControlCommand command;
    TosControlTarget targets[1];
} iARCHI_FLUSH_IGNORE = {
    { 0x03, 0x00, 0xFFFF, 0 },
    {
        { 0x00, 0x00 },
    },
};

static const struct {
    TosControlCommand command;
    TosControlTarget targets[6];
} iARCHI_FLUSH_SLEEP_TORECOVER = {
    { 0x03, 0x28, 0x0005, 0 },
    {
        { 0x01, 0x02 },
        { 0x07, 0x01 },
        { 0x0C, 0x01 },
        { 0x16, 0x65 },
        { 0x20, 0x04 },
        { 0x00, 0x00 },
    },
};

static const struct {
    TosControlCommand command;
    TosControlTarget targets[6];
} iARCHI_FLUSH_SLEEP_TOSPELLRECOVER = {
    { 0x03, 0x28, 0x0009, 0 },
    {
        { 0x01, 0x02 },
        { 0x07, 0x01 },
        { 0x0C, 0x01 },
        { 0x16, 0x65 },
        { 0x20, 0x04 },
        { 0x00, 0x00 },
    },
};

static const struct {
    TosControlCommand command;
    TosControlTarget targets[6];
} iARCHI_FLUSH_SLEEP_TOISOWORDRECOVER = {
    { 0x03, 0x28, 0x000D, 0 },
    {
        { 0x01, 0x02 },
        { 0x07, 0x01 },
        { 0x0C, 0x01 },
        { 0x16, 0x65 },
        { 0x23, 0x04 },
        { 0x00, 0x00 },
    },
};

static const struct {
    TosControlCommand command;
    TosControlTarget targets[6];
} iARCHI_FLUSH_RECORDSLEEP = {
    { 0x03, 0x28, 0x0012, 0 },
    {
        { 0x01, 0x02 },
        { 0x07, 0x01 },
        { 0x0C, 0x01 },
        { 0x16, 0x65 },
        { 0x21, 0x04 },
        { 0x00, 0x00 },
    },
};

static const struct {
    TosControlCommand command;
    TosControlTarget targets[16];
} iARCHI_FLUSH = {
    { 0x03, 0x28, 0xFFFE, 0 },
    {
        { 0x01, 0x02 },
        { 0x07, 0x01 },
        { 0x0C, 0x01 },
        { 0x0D, 0x02 },
        { 0x0E, 0x02 },
        { 0x0F, 0x02 },
        { 0x13, 0x03 },
        { 0x14, 0x02 },
        { 0x16, 0x65 },
        { 0x1A, 0xC8 },
        { 0x1C, 0xC9 },
        { 0x20, 0x04 },
        { 0x21, 0x04 },
        { 0x23, 0x04 },
        { 0x24, 0x03 },
        { 0x00, 0x00 },
    },
};

static const struct {
    TosControlCommand command;
    TosControlTarget targets[1];
} iARCHI_GOTOIDLE = {
    { 0x07, 0x00, 0x0002, 0 },
    {
        { 0x00, 0x00 },
    },
};

static const struct {
    TosControlCommand command;
    TosControlTarget targets[11];
} iARCHI_GOTOSLEEP = {
    { 0x01, 0x00, 0xFFFE, 0 },
    {
        { 0x20, 0x03 },
        { 0x23, 0x03 },
        { 0x24, 0x02 },
        { 0x01, 0x01 },
        { 0x0B, 0x07 },
        { 0x14, 0x01 },
        { 0x16, 0x64 },
        { 0x17, 0xC8 },
        { 0x1A, 0xC9 },
        { 0x1C, 0xC8 },
        { 0x00, 0x00 },
    },
};

static const struct {
    TosControlCommand command;
    TosControlTarget targets[10];
} iARCHI_GOTOSPELLSLEEP = {
    { 0x06, 0x00, 0x0007, 0 },
    {
        { 0x20, 0x03 },
        { 0x24, 0x02 },
        { 0x01, 0x01 },
        { 0x0B, 0x07 },
        { 0x14, 0x01 },
        { 0x16, 0x64 },
        { 0x17, 0xC8 },
        { 0x1A, 0xC9 },
        { 0x1C, 0xC8 },
        { 0x00, 0x00 },
    },
};

static const struct {
    TosControlCommand command;
    TosControlTarget targets[10];
} iARCHI_GOTOISOWORDSLEEP = {
    { 0x08, 0x00, 0x000B, 0 },
    {
        { 0x23, 0x03 },
        { 0x24, 0x02 },
        { 0x01, 0x01 },
        { 0x0B, 0x07 },
        { 0x14, 0x01 },
        { 0x16, 0x64 },
        { 0x17, 0xC8 },
        { 0x1A, 0xC9 },
        { 0x1C, 0xC8 },
        { 0x00, 0x00 },
    },
};

static const struct {
    TosControlCommand command;
    TosControlTarget targets[11];
} iARCHI_START_DETECTION = {
    { 0x33, 0x00, 0xFFFF, 0 },
    {
        { 0x27, 0x64 },
        { 0x0C, 0xFA },
        { 0x0D, 0xFA },
        { 0x0E, 0xFA },
        { 0x0F, 0xFA },
        { 0x17, 0xFA },
        { 0x19, 0xFA },
        { 0x1F, 0x18 },
        { 0x20, 0x26 },
        { 0x23, 0x26 },
        { 0x00, 0x00 },
    },
};

static const struct {
    TosControlCommand command;
    TosControlTarget targets[11];
} iARCHI_NO_START_DETECTION = {
    { 0x34, 0x00, 0xFFFF, 0 },
    {
        { 0x27, 0x64 },
        { 0x0C, 0xF9 },
        { 0x0D, 0xF9 },
        { 0x0E, 0xF9 },
        { 0x0F, 0xF9 },
        { 0x17, 0xF9 },
        { 0x19, 0xF9 },
        { 0x1F, 0x17 },
        { 0x20, 0x25 },
        { 0x23, 0x25 },
        { 0x00, 0x00 },
    },
};

static const struct {
    TosControlCommand command;
    TosControlTarget targets[11];
} iARCHI_DISABLE_M = {
    { 0x3D, 0x00, 0xFFFF, 0 },
    {
        { 0x03, 0xFF },
        { 0x08, 0xFF },
        { 0x0D, 0xFF },
        { 0x10, 0xFF },
        { 0x2C, 0xFF },
        { 0x34, 0xFF },
        { 0x37, 0xFF },
        { 0x3D, 0xFF },
        { 0x31, 0xFF },
        { 0x51, 0xFF },
        { 0x00, 0x00 },
    },
};

static const struct {
    TosControlCommand command;
    TosControlTarget targets[11];
} iARCHI_DISABLE_F = {
    { 0x3E, 0x00, 0xFFFF, 0 },
    {
        { 0x04, 0xFF },
        { 0x09, 0xFF },
        { 0x0E, 0xFF },
        { 0x11, 0xFF },
        { 0x2D, 0xFF },
        { 0x35, 0xFF },
        { 0x38, 0xFF },
        { 0x3E, 0xFF },
        { 0x32, 0xFF },
        { 0x52, 0xFF },
        { 0x00, 0x00 },
    },
};

static const struct {
    TosControlCommand command;
    TosControlTarget targets[11];
} iARCHI_DISABLE_C = {
    { 0x3F, 0x00, 0xFFFF, 0 },
    {
        { 0x05, 0xFF },
        { 0x0A, 0xFF },
        { 0x0F, 0xFF },
        { 0x12, 0xFF },
        { 0x2E, 0xFF },
        { 0x36, 0xFF },
        { 0x39, 0xFF },
        { 0x3F, 0xFF },
        { 0x33, 0xFF },
        { 0x53, 0xFF },
        { 0x00, 0x00 },
    },
};

static const struct {
    TosControlCommand command;
    TosControlTarget targets[3];
} iARCHI_DISABLE_INPUT_M = {
    { 0x6C, 0x00, 0xFFFF, 0 },
    {
        { 0x40, 0x09 },
        { 0x13, 0x0A },
        { 0x00, 0x00 },
    },
};

static const struct {
    TosControlCommand command;
    TosControlTarget targets[3];
} iARCHI_DISABLE_INPUT_F = {
    { 0x6D, 0x00, 0xFFFF, 0 },
    {
        { 0x41, 0x09 },
        { 0x13, 0x0A },
        { 0x00, 0x00 },
    },
};

static const struct {
    TosControlCommand command;
    TosControlTarget targets[3];
} iARCHI_DISABLE_INPUT_C = {
    { 0x6E, 0x00, 0xFFFF, 0 },
    {
        { 0x42, 0x09 },
        { 0x13, 0x0A },
        { 0x00, 0x00 },
    },
};

static const struct {
    TosControlCommand command;
    TosControlTarget targets[14];
} iARCHI_ENABLE_M = {
    { 0x40, 0x00, 0xFFFF, 0 },
    {
        { 0x03, 0xF8 },
        { 0x08, 0xF8 },
        { 0x0D, 0xF8 },
        { 0x10, 0xF8 },
        { 0x2C, 0xF8 },
        { 0x34, 0xF8 },
        { 0x37, 0xF8 },
        { 0x3D, 0xF8 },
        { 0x40, 0x0B },
        { 0x31, 0xF8 },
        { 0x51, 0xF8 },
        { 0x13, 0x02 },
        { 0x03, 0x01 },
        { 0x00, 0x00 },
    },
};

static const struct {
    TosControlCommand command;
    TosControlTarget targets[14];
} iARCHI_ENABLE_F = {
    { 0x41, 0x00, 0xFFFF, 0 },
    {
        { 0x04, 0xF8 },
        { 0x09, 0xF8 },
        { 0x0E, 0xF8 },
        { 0x11, 0xF8 },
        { 0x2D, 0xF8 },
        { 0x35, 0xF8 },
        { 0x38, 0xF8 },
        { 0x3E, 0xF8 },
        { 0x41, 0x0B },
        { 0x32, 0xF8 },
        { 0x52, 0xF8 },
        { 0x13, 0x02 },
        { 0x04, 0x01 },
        { 0x00, 0x00 },
    },
};

static const struct {
    TosControlCommand command;
    TosControlTarget targets[14];
} iARCHI_ENABLE_C = {
    { 0x42, 0x00, 0xFFFF, 0 },
    {
        { 0x05, 0xF8 },
        { 0x0A, 0xF8 },
        { 0x0F, 0xF8 },
        { 0x12, 0xF8 },
        { 0x2E, 0xF8 },
        { 0x36, 0xF8 },
        { 0x39, 0xF8 },
        { 0x3F, 0xF8 },
        { 0x42, 0x0B },
        { 0x33, 0xF8 },
        { 0x53, 0xF8 },
        { 0x13, 0x02 },
        { 0x05, 0x01 },
        { 0x00, 0x00 },
    },
};

static const struct {
    TosControlCommand command;
    TosControlTarget targets[4];
} iARCHI_STOP_SPECSUB_SIL_QUEUE = {
    { 0x6A, 0x00, 0xFFFF, 0 },
    {
        { 0x08, 0x04 },
        { 0x09, 0x04 },
        { 0x0A, 0x04 },
        { 0x00, 0x00 },
    },
};

static const struct {
    TosControlCommand command;
    TosControlTarget targets[4];
} iARCHI_START_SPECSUB_SIL_QUEUE = {
    { 0x6B, 0x00, 0xFFFF, 0 },
    {
        { 0x08, 0x03 },
        { 0x09, 0x03 },
        { 0x0A, 0x03 },
        { 0x00, 0x00 },
    },
};

static const struct {
    TosControlCommand command;
    TosControlTarget targets[1];
} iARCHI_GOTONEXTSTATE = {
    { 0x02, 0x00, 0xFFFE, 0 },
    {
        { 0x00, 0x00 },
    },
};

static const struct {
    TosControlCommand command;
    TosControlTarget targets[1];
} iARCHI_START_ARCHI_CONTROL = {
    { 0x22, 0x00, 0xFFFF, 0 },
    {
        { 0x00, 0x00 },
    },
};

static const struct {
    TosControlCommand command;
    TosControlTarget targets[1];
} iARCHI_STOP_ARCHI_CONTROL = {
    { 0x23, 0x00, 0xFFFF, 0 },
    {
        { 0x00, 0x00 },
    },
};

static const struct {
    TosControlCommand command;
    TosControlTarget targets[13];
} iARCHI_PUT_SESSION = {
    { 0x26, 0x00, 0xFFFF, 0 },
    {
        { 0x06, 0x05 },
        { 0x0B, 0x05 },
        { 0x08, 0x02 },
        { 0x09, 0x02 },
        { 0x0A, 0x02 },
        { 0x0D, 0x04 },
        { 0x0E, 0x04 },
        { 0x0F, 0x04 },
        { 0x13, 0x06 },
        { 0x27, 0x69 },
        { 0x1F, 0xFB },
        { 0x1F, 0x0A },
        { 0x00, 0x00 },
    },
};

static const struct {
    TosControlCommand command;
    TosControlTarget targets[13];
} iARCHI_GET_SESSION = {
    { 0x27, 0x00, 0xFFFF, 0 },
    {
        { 0x06, 0x06 },
        { 0x0B, 0x06 },
        { 0x08, 0x01 },
        { 0x09, 0x01 },
        { 0x0A, 0x01 },
        { 0x0D, 0x03 },
        { 0x0E, 0x03 },
        { 0x0F, 0x03 },
        { 0x13, 0x07 },
        { 0x27, 0x6A },
        { 0x1F, 0xFB },
        { 0x1F, 0x0B },
        { 0x00, 0x00 },
    },
};

static const struct {
    TosControlCommand command;
    TosControlTarget targets[2];
} iARCHI_GET_DISTCALCFUNCS = {
    { 0x55, 0x00, 0xFFFF, 0 },
    {
        { 0x1F, 0x1B },
        { 0x00, 0x00 },
    },
};

static const struct {
    TosControlCommand command;
    TosControlTarget targets[4];
} iARCHI_SET_DISTCALCFUNCS = {
    { 0x56, 0x00, 0xFFFF, 0 },
    {
        { 0x20, 0x24 },
        { 0x23, 0x24 },
        { 0x24, 0x06 },
        { 0x00, 0x00 },
    },
};

static const struct {
    TosControlCommand command;
    TosControlTarget targets[2];
} iARCHI_GET_PSIGNALLEVEL = {
    { 0x04, 0x00, 0xFFFF, 0 },
    {
        { 0x02, 0x01 },
        { 0x00, 0x00 },
    },
};

static const struct {
    TosControlCommand command;
    TosControlTarget targets[2];
} iARCHI_GET_SNR = {
    { 0x05, 0x00, 0xFFFF, 0 },
    {
        { 0x0B, 0x01 },
        { 0x00, 0x00 },
    },
};

static const struct {
    TosControlCommand command;
    TosControlTarget targets[2];
} iARCHI_GET_PBEGINTIME = {
    { 0x2A, 0x00, 0xFFFF, 0 },
    {
        { 0x27, 0x65 },
        { 0x00, 0x00 },
    },
};

static const struct {
    TosControlCommand command;
    TosControlTarget targets[2];
} iARCHI_GET_EXTRAEVENTSCORE = {
    { 0x63, 0x00, 0xFFFF, 0 },
    {
        { 0x24, 0x01 },
        { 0x00, 0x00 },
    },
};

static const struct {
    TosControlCommand command;
    TosControlTarget targets[2];
} iARCHI_VAD_SET_PARAM = {
    { 0x50, 0x00, 0xFFFF, 0 },
    {
        { 0x0B, 0x03 },
        { 0x00, 0x00 },
    },
};

static const struct {
    TosControlCommand command;
    TosControlTarget targets[2];
} iARCHI_TRIGGER_SET_PARAM = {
    { 0x51, 0x00, 0xFFFF, 0 },
    {
        { 0x27, 0x67 },
        { 0x00, 0x00 },
    },
};

static const struct {
    TosControlCommand command;
    TosControlTarget targets[2];
} iARCHI_GENDER_SET_PARAM = {
    { 0x52, 0x00, 0xFFFF, 0 },
    {
        { 0x13, 0x08 },
        { 0x00, 0x00 },
    },
};

static const struct {
    TosControlCommand command;
    TosControlTarget targets[3];
} iARCHI_DISTCALC_SET_PARAM = {
    { 0x53, 0x00, 0xFFFF, 0 },
    {
        { 0x1F, 0xFB },
        { 0x1F, 0x0F },
        { 0x00, 0x00 },
    },
};

static const struct {
    TosControlCommand command;
    TosControlTarget targets[3];
} iARCHI_NBESTDYNPROG_SET_PARAM = {
    { 0x54, 0x00, 0xFFFF, 0 },
    {
        { 0x20, 0x1A },
        { 0x23, 0x1A },
        { 0x00, 0x00 },
    },
};

static const struct {
    TosControlCommand command;
    TosControlTarget targets[2];
} iARCHI_VAD_GET_PARAM = {
    { 0x46, 0x00, 0xFFFF, 0 },
    {
        { 0x0B, 0x04 },
        { 0x00, 0x00 },
    },
};

static const struct {
    TosControlCommand command;
    TosControlTarget targets[2];
} iARCHI_TRIGGER_GET_PARAM = {
    { 0x47, 0x00, 0xFFFF, 0 },
    {
        { 0x27, 0x66 },
        { 0x00, 0x00 },
    },
};

static const struct {
    TosControlCommand command;
    TosControlTarget targets[2];
} iARCHI_GENDER_GET_PARAM = {
    { 0x48, 0x00, 0xFFFF, 0 },
    {
        { 0x13, 0x09 },
        { 0x00, 0x00 },
    },
};

static const struct {
    TosControlCommand command;
    TosControlTarget targets[3];
} iARCHI_DISTCALC_GET_PARAM = {
    { 0x49, 0x00, 0xFFFF, 0 },
    {
        { 0x1F, 0xFB },
        { 0x1F, 0x0D },
        { 0x00, 0x00 },
    },
};

static const struct {
    TosControlCommand command;
    TosControlTarget targets[2];
} iARCHI_NBESTDYNPROG_GET_PARAM = {
    { 0x4A, 0x00, 0xFFFF, 0 },
    {
        { 0x20, 0x1B },
        { 0x00, 0x00 },
    },
};

static const struct {
    TosControlCommand command;
    TosControlTarget targets[15];
} iARCHI_LOADLANGUAGE = {
    { 0x25, 0x00, 0x0001, 0 },
    {
        { 0x13, 0x01 },
        { 0x1E, 0x01 },
        { 0x20, 0xFB },
        { 0x21, 0xFB },
        { 0x22, 0xFB },
        { 0x23, 0xFB },
        { 0x24, 0xFB },
        { 0x1F, 0xFB },
        { 0x1F, 0x10 },
        { 0x20, 0x22 },
        { 0x21, 0x22 },
        { 0x22, 0x07 },
        { 0x23, 0x22 },
        { 0x24, 0x07 },
        { 0x00, 0x00 },
    },
};

static const struct {
    TosControlCommand command;
    TosControlTarget targets[2];
} iARCHI_UNLOADLANGUAGE = {
    { 0x5F, 0x00, 0x0000, 0 },
    {
        { 0x1F, 0x1E },
        { 0x00, 0x00 },
    },
};

static const struct {
    TosControlCommand command;
    TosControlTarget targets[3];
} iARCHI_CHANGECTX_TOIDLE = {
    { 0x24, 0x00, 0x0002, 0 },
    {
        { 0x20, 0x23 },
        { 0x23, 0x23 },
        { 0x00, 0x00 },
    },
};

static const struct {
    TosControlCommand command;
    TosControlTarget targets[3];
} iARCHI_CHANGECTX = {
    { 0x24, 0x00, 0xFFFF, 0 },
    {
        { 0x20, 0x23 },
        { 0x23, 0x23 },
        { 0x00, 0x00 },
    },
};

static const struct {
    TosControlCommand command;
    TosControlTarget targets[2];
} iARCHI_SETEXEVCTX = {
    { 0x66, 0x00, 0xFFFF, 0 },
    {
        { 0x24, 0x08 },
        { 0x00, 0x00 },
    },
};

static const struct {
    TosControlCommand command;
    TosControlTarget targets[3];
} iARCHI_CTXDELETED = {
    { 0x29, 0x00, 0xFFFF, 0 },
    {
        { 0x20, 0x27 },
        { 0x23, 0x27 },
        { 0x00, 0x00 },
    },
};

static const struct {
    TosControlCommand command;
    TosControlTarget targets[2];
} iARCHI_SETACTWORDS = {
    { 0x19, 0x00, 0xFFFF, 0 },
    {
        { 0x20, 0x1C },
        { 0x00, 0x00 },
    },
};

static const struct {
    TosControlCommand command;
    TosControlTarget targets[2];
} iARCHI_GETACTWORDS = {
    { 0x1A, 0x00, 0xFFFF, 0 },
    {
        { 0x20, 0x1D },
        { 0x00, 0x00 },
    },
};

static const struct {
    TosControlCommand command;
    TosControlTarget targets[2];
} iARCHI_MIKE_SET = {
    { 0x35, 0x00, 0xFFFF, 0 },
    {
        { 0x0B, 0x02 },
        { 0x00, 0x00 },
    },
};

static const struct {
    TosControlCommand command;
    TosControlTarget targets[6];
} iARCHI_USERWORDTRAININGSTART = {
    { 0x0E, 0x00, 0x000F, 0 },
    {
        { 0x22, 0xF8 },
        { 0x21, 0xF8 },
        { 0x46, 0xF8 },
        { 0x47, 0xF8 },
        { 0x24, 0xFF },
        { 0x00, 0x00 },
    },
};

static const struct {
    TosControlCommand command;
    TosControlTarget targets[5];
} iARCHI_USERWORDTRAININGSTOP = {
    { 0x10, 0x00, 0xFFFF, 0 },
    {
        { 0x22, 0xFF },
        { 0x21, 0xFF },
        { 0x46, 0xFF },
        { 0x47, 0xFF },
        { 0x00, 0x00 },
    },
};

static const struct {
    TosControlCommand command;
    TosControlTarget targets[2];
} iARCHI_INITUSERWORD = {
    { 0x0F, 0x00, 0xFFFF, 0 },
    {
        { 0x21, 0x17 },
        { 0x00, 0x00 },
    },
};

static const struct {
    TosControlCommand command;
    TosControlTarget targets[9];
} iARCHI_USERTRAIN_OK_START = {
    { 0x0A, 0x00, 0xFFFE, cbNewUserWordUtterance },
    {
        { 0x14, 0x01 },
        { 0x01, 0x01 },
        { 0x0B, 0x07 },
        { 0x16, 0x64 },
        { 0x17, 0xC8 },
        { 0x1A, 0xC9 },
        { 0x1C, 0xC8 },
        { 0x21, 0x03 },
        { 0x00, 0x00 },
    },
};

static const struct {
    TosControlCommand command;
    TosControlTarget targets[9];
} iARCHI_USERTRAIN_OK_LASTUTT_START = {
    { 0x16, 0x00, 0xFFFE, cbNewUserWordUtterance },
    {
        { 0x01, 0x01 },
        { 0x0B, 0x07 },
        { 0x14, 0x01 },
        { 0x16, 0x64 },
        { 0x17, 0xC8 },
        { 0x1A, 0xC9 },
        { 0x1C, 0xC8 },
        { 0x21, 0x03 },
        { 0x00, 0x00 },
    },
};

static const struct {
    TosControlCommand command;
    TosControlTarget targets[1];
} iARCHI_USERTRAIN_OK_ACCEPT = {
    { 0x0A, 0x00, 0x000F, cbUtteranceAccepted },
    {
        { 0x00, 0x00 },
    },
};

static const struct {
    TosControlCommand command;
    TosControlTarget targets[1];
} iARCHI_USERTRAIN_OK_LASTUTT_ACCEPT = {
    { 0x16, 0x00, 0xFFFE, cbLastUtteranceAccepted },
    {
        { 0x00, 0x00 },
    },
};

static const struct {
    TosControlCommand command;
    TosControlTarget targets[1];
} iARCHI_USERTRAIN_OK_LASTUTT_CONFIRM = {
    { 0x16, 0x00, 0x0014, cbDoUserWordTraining },
    {
        { 0x00, 0x00 },
    },
};

static const struct {
    TosControlCommand command;
    TosControlTarget targets[1];
} iARCHI_USERTRAIN_NOK_ONACCEPT = {
    { 0x0C, 0x00, 0x000F, cbUtteranceNotAccepted },
    {
        { 0x00, 0x00 },
    },
};

static const struct {
    TosControlCommand command;
    TosControlTarget targets[1];
} iARCHI_USERTRAIN_NOK_ONCONFIRMORSTART = {
    { 0x0C, 0x00, 0x0001, cbCancelUserWordTraining },
    {
        { 0x00, 0x00 },
    },
};

static const struct {
    TosControlCommand command;
    TosControlTarget targets[1];
} iARCHI_USERTRAIN_CANCEL = {
    { 0x0B, 0x00, 0x0001, cbCancelUserWordTraining },
    {
        { 0x00, 0x00 },
    },
};

static const struct {
    TosControlCommand command;
    TosControlTarget targets[2];
} iARCHI_INITSCOREUSERWORD = {
    { 0x13, 0x00, 0xFFFF, 0 },
    {
        { 0x22, 0x04 },
        { 0x00, 0x00 },
    },
};

static const struct {
    TosControlCommand command;
    TosControlTarget targets[2];
} iARCHI_SETPARAMSCOREUSERWORD = {
    { 0x17, 0x00, 0xFFFF, 0 },
    {
        { 0x22, 0x05 },
        { 0x00, 0x00 },
    },
};

static const struct {
    TosControlCommand command;
    TosControlTarget targets[2];
} iARCHI_GETPARAMSCOREUSERWORD = {
    { 0x17, 0x00, 0xFFFF, 0 },
    {
        { 0x22, 0x06 },
        { 0x00, 0x00 },
    },
};

static const struct {
    TosControlCommand command;
    TosControlTarget targets[5];
} iARCHI_SILADATPTON = {
    { 0x15, 0x00, 0xFFFF, 0 },
    {
        { 0x1F, 0x02 },
        { 0x1F, 0x17 },
        { 0x20, 0x25 },
        { 0x23, 0x25 },
        { 0x00, 0x00 },
    },
};

static const struct {
    TosControlCommand command;
    TosControlTarget targets[5];
} iARCHI_SILADAPTOFF = {
    { 0x11, 0x00, 0xFFFF, 0 },
    {
        { 0x1F, 0x03 },
        { 0x1F, 0x18 },
        { 0x20, 0x26 },
        { 0x23, 0x26 },
        { 0x00, 0x00 },
    },
};

static const struct {
    TosControlCommand command;
    TosControlTarget targets[3];
} iARCHI_LABEL_START_SECONDPASS = {
    { 0x1F, 0x00, 0xFFFF, 0 },
    {
        { 0x20, 0x03 },
        { 0x24, 0xFC },
        { 0x00, 0x00 },
    },
};

static const struct {
    TosControlCommand command;
    TosControlTarget targets[3];
} iARCHI_LABEL_STOP_SECONDPASS = {
    { 0x20, 0x00, 0xFFFF, 0 },
    {
        { 0x24, 0xFB },
        { 0x20, 0x04 },
        { 0x00, 0x00 },
    },
};

static const struct {
    TosControlCommand command;
    TosControlTarget targets[2];
} iARCHI_LABEL_START_UW = {
    { 0x1F, 0x00, 0xFFFF, 0 },
    {
        { 0x22, 0x01 },
        { 0x00, 0x00 },
    },
};

static const struct {
    TosControlCommand command;
    TosControlTarget targets[2];
} iARCHI_LABEL_STOP_UW = {
    { 0x20, 0x00, 0xFFFF, 0 },
    {
        { 0x22, 0x02 },
        { 0x00, 0x00 },
    },
};

static const struct {
    TosControlCommand command;
    TosControlTarget targets[10];
} iARCHI_NO_PITCH_PROCESSING = {
    { 0x5D, 0x00, 0xFFFF, 0 },
    {
        { 0x15, 0xFC },
        { 0x16, 0xFC },
        { 0x17, 0xFC },
        { 0x18, 0xFC },
        { 0x19, 0xFC },
        { 0x1A, 0xFC },
        { 0x1B, 0xFC },
        { 0x1C, 0xFC },
        { 0x1D, 0x65 },
        { 0x00, 0x00 },
    },
};

static const struct {
    TosControlCommand command;
    TosControlTarget targets[27];
} iARCHI_DO_PITCH_PROCESSING = {
    { 0x5E, 0x00, 0xFFFF, 0 },
    {
        { 0x15, 0xF8 },
        { 0x16, 0xF8 },
        { 0x17, 0xF8 },
        { 0x18, 0xF8 },
        { 0x19, 0xF8 },
        { 0x1A, 0xF8 },
        { 0x1B, 0xF8 },
        { 0x1C, 0xF8 },
        { 0x48, 0xF8 },
        { 0x49, 0xF8 },
        { 0x4A, 0xF8 },
        { 0x4B, 0xF8 },
        { 0x4C, 0xF8 },
        { 0x4D, 0xF8 },
        { 0x4E, 0xF8 },
        { 0x4F, 0xF8 },
        { 0x56, 0xF8 },
        { 0x15, 0xFB },
        { 0x16, 0xFB },
        { 0x17, 0xFB },
        { 0x18, 0xFB },
        { 0x19, 0xFB },
        { 0x1A, 0xFB },
        { 0x1B, 0xFB },
        { 0x1C, 0xFB },
        { 0x1D, 0x97 },
        { 0x00, 0x00 },
    },
};

static const struct {
    TosControlCommand command;
    TosControlTarget targets[2];
} iARCHI_ENABLE_EXTRAEVENTDETECTION = {
    { 0x64, 0x00, 0xFFFF, 0 },
    {
        { 0x24, 0xF8 },
        { 0x00, 0x00 },
    },
};

static const struct {
    TosControlCommand command;
    TosControlTarget targets[2];
} iARCHI_DISABLE_EXTRAEVENTDETECTION = {
    { 0x65, 0x00, 0xFFFF, 0 },
    {
        { 0x24, 0xFF },
        { 0x00, 0x00 },
    },
};

static const struct {
    TosControlCommand command;
    TosControlTarget targets[13];
} iGENDER_DISABLE_M = {
    { 0x0D, 0x00, 0xFFFF, cbGenderDisable },
    {
        { 0x03, 0xFF },
        { 0x08, 0xFF },
        { 0x0D, 0xFF },
        { 0x10, 0xFF },
        { 0x2C, 0xFF },
        { 0x34, 0xFF },
        { 0x37, 0xFF },
        { 0x3D, 0xFF },
        { 0x31, 0xFF },
        { 0x51, 0xFF },
        { 0x40, 0xFF },
        { 0x13, 0x0A },
        { 0x00, 0x00 },
    },
};

static const struct {
    TosControlCommand command;
    TosControlTarget targets[13];
} iGENDER_DISABLE_F = {
    { 0x0E, 0x00, 0xFFFF, cbGenderDisable },
    {
        { 0x04, 0xFF },
        { 0x09, 0xFF },
        { 0x0E, 0xFF },
        { 0x11, 0xFF },
        { 0x2D, 0xFF },
        { 0x35, 0xFF },
        { 0x38, 0xFF },
        { 0x3E, 0xFF },
        { 0x32, 0xFF },
        { 0x52, 0xFF },
        { 0x41, 0xFF },
        { 0x13, 0x0A },
        { 0x00, 0x00 },
    },
};

static const struct {
    TosControlCommand command;
    TosControlTarget targets[13];
} iGENDER_DISABLE_C = {
    { 0x0F, 0x00, 0xFFFF, cbGenderDisable },
    {
        { 0x05, 0xFF },
        { 0x0A, 0xFF },
        { 0x0F, 0xFF },
        { 0x12, 0xFF },
        { 0x2E, 0xFF },
        { 0x36, 0xFF },
        { 0x39, 0xFF },
        { 0x3F, 0xFF },
        { 0x33, 0xFF },
        { 0x53, 0xFF },
        { 0x42, 0xFF },
        { 0x13, 0x0A },
        { 0x00, 0x00 },
    },
};

static const struct {
    TosControlCommand command;
    TosControlTarget targets[1];
} iTRIGGER_START_DETECTED_IGNORE = {
    { 0xC8, 0x00, 0xFFFF, 0 },
    {
        { 0x00, 0x00 },
    },
};

static const struct {
    TosControlCommand command;
    TosControlTarget targets[10];
} iTRIGGER_START_DETECTED_TO_NEXTSTATE = {
    { 0xC8, 0x00, 0xFFFE, cbStartDetected },
    {
        { 0x0C, 0xF9 },
        { 0x0D, 0xF9 },
        { 0x0E, 0xF9 },
        { 0x0F, 0xF9 },
        { 0x17, 0xF9 },
        { 0x19, 0xF9 },
        { 0x1F, 0x17 },
        { 0x20, 0x25 },
        { 0x23, 0x25 },
        { 0x00, 0x00 },
    },
};

static const struct {
    TosControlCommand command;
    TosControlTarget targets[1];
} iTRIGGER_TS_DETECTED_IGNORE = {
    { 0xC9, 0x00, 0xFFFF, 0 },
    {
        { 0x00, 0x00 },
    },
};

static const struct {
    TosControlCommand command;
    TosControlTarget targets[16];
} iTRIGGER_TS_DETECTED_TO_NEXTSTATE = {
    { 0xC9, 0x28, 0xFFFE, cbTSDetected },
    {
        { 0x01, 0x02 },
        { 0x07, 0x01 },
        { 0x0C, 0x01 },
        { 0x0D, 0x02 },
        { 0x0E, 0x02 },
        { 0x0F, 0x02 },
        { 0x13, 0x03 },
        { 0x14, 0x02 },
        { 0x16, 0x65 },
        { 0x1A, 0xC8 },
        { 0x1C, 0xC9 },
        { 0x21, 0x04 },
        { 0x20, 0x04 },
        { 0x23, 0x04 },
        { 0x24, 0x03 },
        { 0x00, 0x00 },
    },
};

static const struct {
    TosControlCommand command;
    TosControlTarget targets[1];
} iTRIGGER_GARBLED_SOUND_DETECTED = {
    { 0xCA, 0x00, 0xFFFF, cbTOSAbnormalCond },
    {
        { 0x00, 0x00 },
    },
};

static const struct {
    TosControlCommand command;
    TosControlTarget targets[1];
} iVAD_ASK_CURRENT_GAIN = {
    { 0x0A, 0x00, 0xFFFF, cbAskCurrentGain },
    {
        { 0x00, 0x00 },
    },
};

static const struct {
    TosControlCommand command;
    TosControlTarget targets[1];
} iVAD_CHANGE_MIKE = {
    { 0x0B, 0x00, 0xFFFF, cbGainRequest },
    {
        { 0x00, 0x00 },
    },
};

static const struct {
    TosControlCommand command;
    TosControlTarget targets[1];
} iVAD_EVENTRECEIVED = {
    { 0x0C, 0x00, 0xFFFF, cbTOSAbnormalCond },
    {
        { 0x00, 0x00 },
    },
};

static const struct {
    TosControlCommand command;
    TosControlTarget targets[1];
} iNBESTDYNPROG_TSDETECTED_IGNORE = {
    { 0x08, 0x00, 0xFFFF, 0 },
    {
        { 0x00, 0x00 },
    },
};

static const struct {
    TosControlCommand command;
    TosControlTarget targets[15];
} iNBESTDYNPROG_TSDETECTED_TONEXTSTATE = {
    { 0x08, 0x28, 0xFFFE, cbTSAfterStartDetected },
    {
        { 0x01, 0x02 },
        { 0x07, 0x01 },
        { 0x0C, 0x01 },
        { 0x0D, 0x02 },
        { 0x0E, 0x02 },
        { 0x0F, 0x02 },
        { 0x13, 0x03 },
        { 0x14, 0x02 },
        { 0x16, 0x65 },
        { 0x1A, 0xC8 },
        { 0x1C, 0xC9 },
        { 0x20, 0x04 },
        { 0x23, 0x04 },
        { 0x24, 0x03 },
        { 0x00, 0x00 },
    },
};

static const struct {
    TosControlCommand command;
    TosControlTarget targets[1];
} iNBESTDYNPROG_WORDRECOGNISED = {
    { 0x09, 0x00, 0xFFFF, cbDoSecondPassIfEnabled },
    {
        { 0x00, 0x00 },
    },
};

static const struct {
    TosControlCommand command;
    TosControlTarget targets[1];
} iISOWORDTOKENDP_WORDRECOGNISED = {
    { 0x09, 0x00, 0xFFFF, cbDoSecondPassIfEnabled },
    {
        { 0x00, 0x00 },
    },
};

static const struct {
    TosControlCommand command;
    TosControlTarget targets[1];
} iNBESTDYNPROG_NOWORDRECOGNISED = {
    { 0x0A, 0x00, 0x0002, cbNothingRecognised },
    {
        { 0x00, 0x00 },
    },
};

static const struct {
    TosControlCommand command;
    TosControlTarget targets[1];
} iNBESTDYNPROG_WORDRECOGNISED_SECONDPASS = {
    { 0x09, 0x00, 0x0002, cbTOSRecognised },
    {
        { 0x00, 0x00 },
    },
};

static const struct {
    TosControlCommand command;
    TosControlTarget targets[1];
} iNBESTDYNPROG_NOWORDRECOGNISED_SECONDPASS = {
    { 0x0A, 0x00, 0x0002, cbNothingRecognised },
    {
        { 0x00, 0x00 },
    },
};

static const struct {
    TosControlCommand command;
    TosControlTarget targets[1];
} iNBESTDYNPROG_WORDRECOGNISED_SPELLSECONDPASS = {
    { 0x09, 0x00, 0xFFFF, cbSpellRecognised },
    {
        { 0x00, 0x00 },
    },
};

static const struct {
    TosControlCommand command;
    TosControlTarget targets[1];
} iNBESTDYNPROG_NOWORDRECOGNISED_SPELLSECONDPASS = {
    { 0x0A, 0x00, 0xFFFF, cbSpellNothingRecognised },
    {
        { 0x00, 0x00 },
    },
};

static const struct {
    TosControlCommand command;
    TosControlTarget targets[1];
} iDPGENUW_RESULTREADY = {
    { 0x0C, 0x00, 0xFFFF, cbUserWordResultAvailable },
    {
        { 0x00, 0x00 },
    },
};

static const struct {
    TosControlCommand command;
    TosControlTarget targets[13];
} iDPGENUW_TSDETECTED_TONEXTSTATE = {
    { 0x08, 0x28, 0xFFFE, cbTSAfterStartDetected },
    {
        { 0x01, 0x02 },
        { 0x07, 0x01 },
        { 0x0C, 0x01 },
        { 0x0D, 0x02 },
        { 0x0E, 0x02 },
        { 0x0F, 0x02 },
        { 0x13, 0x03 },
        { 0x14, 0x02 },
        { 0x16, 0x65 },
        { 0x1A, 0xC8 },
        { 0x1C, 0xC9 },
        { 0x21, 0x04 },
        { 0x00, 0x00 },
    },
};

static const struct {
    TosControlCommand command;
    TosControlTarget targets[1];
} iDPSCOREUW_RESULTREADY = {
    { 0x0C, 0x00, 0xFFFF, cbScoreUserWordResultAvailable },
    {
        { 0x00, 0x00 },
    },
};

static const struct {
    TosControlCommand command;
    TosControlTarget targets[6];
} iARCHI_SWITCH_TO_NBESTDP = {
    { 0x57, 0x00, 0xFFFF, 0 },
    {
        { 0x20, 0xFB },
        { 0x21, 0xFC },
        { 0x22, 0xFC },
        { 0x1F, 0xFC },
        { 0x23, 0xFC },
        { 0x00, 0x00 },
    },
};

static const struct {
    TosControlCommand command;
    TosControlTarget targets[6];
} iARCHI_SWITCH_TO_TREEDP = {
    { 0x5A, 0x00, 0xFFFF, 0 },
    {
        { 0x23, 0xFB },
        { 0x20, 0xFC },
        { 0x21, 0xFC },
        { 0x22, 0xFC },
        { 0x1F, 0xFC },
        { 0x00, 0x00 },
    },
};

static const struct {
    TosControlCommand command;
    TosControlTarget targets[7];
} iARCHI_SWITCH_TO_GENUWDP = {
    { 0x58, 0x00, 0xFFFF, 0 },
    {
        { 0x20, 0xFC },
        { 0x21, 0xFB },
        { 0x22, 0xFC },
        { 0x1F, 0xFB },
        { 0x23, 0xFC },
        { 0x21, 0x22 },
        { 0x00, 0x00 },
    },
};

static const struct {
    TosControlCommand command;
    TosControlTarget targets[7];
} iARCHI_SWITCH_TO_SCOREUWDP = {
    { 0x59, 0x00, 0xFFFF, 0 },
    {
        { 0x20, 0xFC },
        { 0x21, 0xFC },
        { 0x22, 0xFB },
        { 0x1F, 0xFB },
        { 0x23, 0xFC },
        { 0x22, 0x07 },
        { 0x00, 0x00 },
    },
};


static const struct {
    TosControlCommand command;
    TosControlTarget targets[2];
} iARCHI_RESIZEDISTANCEQUEUES = {
    { 0x60, 0x00, 0xFFFF, 0 },
    {
        { 0x1F, 0x1F },
        { 0x00, 0x00 },
    },
};

static const struct {
    TosControlCommand command;
    TosControlTarget targets[2];
} iARCHI_DESTRUCTISODP = {
    { 0x61, 0x00, 0xFFFF, 0 },
    {
        { 0x23, 0xFF },
        { 0x00, 0x00 },
    },
};

static const struct {
    TosControlCommand command;
    TosControlTarget targets[2];
} iARCHI_DESTRUCTEXTRAEVENTDP = {
    { 0x67, 0x00, 0xFFFF, 0 },
    {
        { 0x24, 0xFF },
        { 0x00, 0x00 },
    },
};

static const struct {
    TosControlCommand command;
    TosControlTarget targets[3];
} iARCHI_CONSTRUCTISODPANDLOADLANG = {
    { 0x62, 0x00, 0xFFFF, 0 },
    {
        { 0x23, 0xF8 },
        { 0x23, 0x22 },
        { 0x00, 0x00 },
    },
};

static const struct {
    TosControlCommand command;
    TosControlTarget targets[6];
} iARCHI_GOTO_PROMPT = {
    { 0x69, 0x00, 0x000F, 0 },
    {
        { 0x20, 0xFC },
        { 0x21, 0xFB },
        { 0x22, 0xFC },
        { 0x1F, 0xFB },
        { 0x23, 0xFC },
        { 0x00, 0x00 },
    },
};

static const TosControlCommand *const VAD_BOOT_DATAREADY_IDLE[] = {
    &iVAD_ASK_CURRENT_GAIN.command,
    0,
};

static const TosControlCommand *const NBESTDYNPROG_RUN[] = {
    &iNBESTDYNPROG_TSDETECTED_TONEXTSTATE.command,
    0,
};

static const TosControlCommand *const DPGENUW_RECORD[] = {
    &iDPGENUW_TSDETECTED_TONEXTSTATE.command,
    0,
};

static const TosControlCommand *const DPGENUW_ACCEPT[] = {
    &iDPGENUW_RESULTREADY.command,
    0,
};

static const TosControlCommand *const DPSCOREUW_DATAREADY[] = {
    &iDPSCOREUW_RESULTREADY.command,
    0,
};

static const TosControlCommand *const ARCHI_BOOT[] = {
    &iARCHI_LOADLANGUAGE.command,
    &iARCHI_GET_PSIGNALLEVEL.command,
    &iARCHI_GET_PBEGINTIME.command,
    &iARCHI_MIKE_SET.command,
    &iARCHI_VAD_SET_PARAM.command,
    &iARCHI_TRIGGER_SET_PARAM.command,
    &iARCHI_GENDER_SET_PARAM.command,
    &iARCHI_DISTCALC_SET_PARAM.command,
    &iARCHI_NBESTDYNPROG_SET_PARAM.command,
    &iARCHI_VAD_GET_PARAM.command,
    &iARCHI_GENDER_GET_PARAM.command,
    &iARCHI_TRIGGER_GET_PARAM.command,
    &iARCHI_DISTCALC_GET_PARAM.command,
    &iARCHI_NBESTDYNPROG_GET_PARAM.command,
    &iARCHI_SILADAPTOFF.command,
    &iARCHI_CTXDELETED.command,
    &iARCHI_ENABLE_EXTRAEVENTDETECTION.command,
    &iARCHI_DISABLE_EXTRAEVENTDETECTION.command,
    0,
};

static const TosControlCommand *const ARCHI_DATAREADY[] = {
    &iARCHI_PUT_SESSION.command,
    &iARCHI_GET_SESSION.command,
    &iARCHI_NO_PITCH_PROCESSING.command,
    &iARCHI_DO_PITCH_PROCESSING.command,
    &iARCHI_LOADLANGUAGE.command,
    &iARCHI_UNLOADLANGUAGE.command,
    &iARCHI_SWITCH_TO_NBESTDP.command,
    &iARCHI_SWITCH_TO_TREEDP.command,
    &iARCHI_CHANGECTX_TOIDLE.command,
    &iARCHI_CTXDELETED.command,
    &iARCHI_MIKE_SET.command,
    &iARCHI_SILADAPTOFF.command,
    &iARCHI_START_ARCHI_CONTROL.command,
    &iARCHI_STOP_ARCHI_CONTROL.command,
    &iARCHI_SILADATPTON.command,
    &iARCHI_VAD_SET_PARAM.command,
    &iARCHI_TRIGGER_SET_PARAM.command,
    &iARCHI_GENDER_SET_PARAM.command,
    &iARCHI_DISTCALC_SET_PARAM.command,
    &iARCHI_NBESTDYNPROG_SET_PARAM.command,
    &iARCHI_USERWORDTRAININGSTART.command,
    &iARCHI_USERWORDTRAININGSTOP.command,
    &iARCHI_INITSCOREUSERWORD.command,
    &iARCHI_SETPARAMSCOREUSERWORD.command,
    &iARCHI_GETPARAMSCOREUSERWORD.command,
    &iARCHI_SWITCH_TO_SCOREUWDP.command,
    &iARCHI_LABEL_START_UW.command,
    &iARCHI_LABEL_STOP_UW.command,
    &iARCHI_DISABLE_M.command,
    &iARCHI_DISABLE_F.command,
    &iARCHI_DISABLE_C.command,
    &iARCHI_DISABLE_INPUT_M.command,
    &iARCHI_DISABLE_INPUT_F.command,
    &iARCHI_DISABLE_INPUT_C.command,
    &iARCHI_ENABLE_M.command,
    &iARCHI_ENABLE_F.command,
    &iARCHI_ENABLE_C.command,
    &iARCHI_GET_DISTCALCFUNCS.command,
    &iARCHI_SET_DISTCALCFUNCS.command,
    &iARCHI_CONSTRUCTISODPANDLOADLANG.command,
    &iARCHI_DESTRUCTISODP.command,
    0,
};

static const TosControlCommand *const ARCHI_IDLE[] = {
    &iARCHI_FLUSH_IGNORE.command,
    &iARCHI_SILADATPTON.command,
    &iARCHI_LOADLANGUAGE.command,
    &iARCHI_PUT_SESSION.command,
    &iARCHI_GET_SESSION.command,
    &iARCHI_MIKE_SET.command,
    &iARCHI_USERWORDTRAININGSTART.command,
    &iARCHI_CHANGECTX.command,
    &iARCHI_SETEXEVCTX.command,
    &iARCHI_CTXDELETED.command,
    &iARCHI_GOTOSLEEP.command,
    &iARCHI_GOTOSPELLSLEEP.command,
    &iARCHI_GOTOISOWORDSLEEP.command,
    &iARCHI_SETACTWORDS.command,
    &iARCHI_GETACTWORDS.command,
    &iARCHI_VAD_SET_PARAM.command,
    &iARCHI_TRIGGER_SET_PARAM.command,
    &iARCHI_GENDER_SET_PARAM.command,
    &iARCHI_DISTCALC_SET_PARAM.command,
    &iARCHI_NBESTDYNPROG_SET_PARAM.command,
    &iARCHI_GET_SNR.command,
    &iARCHI_SWITCH_TO_NBESTDP.command,
    &iARCHI_SWITCH_TO_TREEDP.command,
    &iARCHI_GET_EXTRAEVENTSCORE.command,
    &iARCHI_ENABLE_EXTRAEVENTDETECTION.command,
    &iARCHI_DISABLE_EXTRAEVENTDETECTION.command,
    &iARCHI_DISABLE_M.command,
    &iARCHI_DISABLE_F.command,
    &iARCHI_DISABLE_C.command,
    &iARCHI_DISABLE_INPUT_M.command,
    &iARCHI_DISABLE_INPUT_F.command,
    &iARCHI_DISABLE_INPUT_C.command,
    &iARCHI_ENABLE_M.command,
    &iARCHI_ENABLE_F.command,
    &iARCHI_ENABLE_C.command,
    &iARCHI_GET_DISTCALCFUNCS.command,
    &iARCHI_SET_DISTCALCFUNCS.command,
    &iARCHI_CONSTRUCTISODPANDLOADLANG.command,
    &iARCHI_DESTRUCTISODP.command,
    &iARCHI_DESTRUCTEXTRAEVENTDP.command,
    0,
};

static const TosControlCommand *const ARCHI_SLEEP[] = {
    &iARCHI_GOTONEXTSTATE.command,
    &iARCHI_FLUSH_SLEEP_TORECOVER.command,
    &iARCHI_MIKE_SET.command,
    &iARCHI_GET_SNR.command,
    &iARCHI_START_DETECTION.command,
    &iARCHI_GETACTWORDS.command,
    &iARCHI_STOP_SPECSUB_SIL_QUEUE.command,
    &iARCHI_START_SPECSUB_SIL_QUEUE.command,
    0,
};

static const TosControlCommand *const ARCHI_SPELLSLEEP[] = {
    &iARCHI_GOTONEXTSTATE.command,
    &iARCHI_FLUSH_SLEEP_TOSPELLRECOVER.command,
    &iARCHI_MIKE_SET.command,
    &iARCHI_GET_SNR.command,
    &iARCHI_START_DETECTION.command,
    &iARCHI_GETACTWORDS.command,
    &iARCHI_STOP_SPECSUB_SIL_QUEUE.command,
    &iARCHI_START_SPECSUB_SIL_QUEUE.command,
    0,
};

static const TosControlCommand *const ARCHI_ISOWORDSLEEP[] = {
    &iARCHI_GOTONEXTSTATE.command,
    &iARCHI_FLUSH_SLEEP_TOISOWORDRECOVER.command,
    &iARCHI_MIKE_SET.command,
    &iARCHI_GET_SNR.command,
    &iARCHI_START_DETECTION.command,
    &iARCHI_GETACTWORDS.command,
    &iARCHI_STOP_SPECSUB_SIL_QUEUE.command,
    &iARCHI_START_SPECSUB_SIL_QUEUE.command,
    0,
};

static const TosControlCommand *const ARCHI_RUN[] = {
    &iARCHI_FLUSH.command,
    &iARCHI_MIKE_SET.command,
    &iARCHI_GET_SNR.command,
    &iARCHI_NO_START_DETECTION.command,
    &iARCHI_GETACTWORDS.command,
    &iARCHI_STOP_SPECSUB_SIL_QUEUE.command,
    &iARCHI_START_SPECSUB_SIL_QUEUE.command,
    0,
};

static const TosControlCommand *const ARCHI_RECOVER[] = {
    &iARCHI_GOTONEXTSTATE.command,
    &iARCHI_GOTOIDLE.command,
    &iARCHI_FLUSH_IGNORE.command,
    &iARCHI_GET_SNR.command,
    0,
};

static const TosControlCommand *const ARCHI_SECONDPASS[] = {
    &iARCHI_LABEL_START_SECONDPASS.command,
    &iARCHI_LABEL_STOP_SECONDPASS.command,
    &iARCHI_SILADAPTOFF.command,
    &iARCHI_FLUSH_IGNORE.command,
    &iARCHI_NBESTDYNPROG_SET_PARAM.command,
    &iARCHI_TRIGGER_SET_PARAM.command,
    &iARCHI_GET_SNR.command,
    &iARCHI_CHANGECTX.command,
    &iARCHI_SWITCH_TO_NBESTDP.command,
    &iARCHI_SWITCH_TO_TREEDP.command,
    0,
};

static const TosControlCommand *const ARCHI_SPELLSECONDPASS[] = {
    &iARCHI_LABEL_START_SECONDPASS.command,
    &iARCHI_LABEL_STOP_SECONDPASS.command,
    &iARCHI_SILADAPTOFF.command,
    &iARCHI_FLUSH_IGNORE.command,
    &iARCHI_NBESTDYNPROG_SET_PARAM.command,
    &iARCHI_TRIGGER_SET_PARAM.command,
    &iARCHI_CHANGECTX.command,
    &iARCHI_GET_EXTRAEVENTSCORE.command,
    &iARCHI_GOTOIDLE.command,
    &iARCHI_GET_SNR.command,
    0,
};

static const TosControlCommand *const ARCHI_PROMPT[] = {
    &iARCHI_INITUSERWORD.command,
    &iARCHI_USERTRAIN_OK_START.command,
    &iARCHI_USERTRAIN_OK_LASTUTT_START.command,
    &iARCHI_USERTRAIN_CANCEL.command,
    &iARCHI_USERTRAIN_NOK_ONCONFIRMORSTART.command,
    &iARCHI_SWITCH_TO_GENUWDP.command,
    &iARCHI_GET_SNR.command,
    &iARCHI_MIKE_SET.command,
    &iARCHI_RESIZEDISTANCEQUEUES.command,
    0,
};

static const TosControlCommand *const ARCHI_RECORDSLEEP[] = {
    &iARCHI_FLUSH_RECORDSLEEP.command,
    &iARCHI_GOTONEXTSTATE.command,
    &iARCHI_START_DETECTION.command,
    &iARCHI_GET_SNR.command,
    &iARCHI_MIKE_SET.command,
    &iARCHI_STOP_SPECSUB_SIL_QUEUE.command,
    &iARCHI_START_SPECSUB_SIL_QUEUE.command,
    0,
};

static const TosControlCommand *const ARCHI_RECORD[] = {
    &iARCHI_NO_START_DETECTION.command,
    &iARCHI_FLUSH.command,
    &iARCHI_GET_SNR.command,
    &iARCHI_MIKE_SET.command,
    &iARCHI_STOP_SPECSUB_SIL_QUEUE.command,
    &iARCHI_START_SPECSUB_SIL_QUEUE.command,
    0,
};

static const TosControlCommand *const ARCHI_ACCEPT[] = {
    &iARCHI_GET_SESSION.command,
    &iARCHI_USERTRAIN_OK_ACCEPT.command,
    &iARCHI_USERTRAIN_OK_LASTUTT_ACCEPT.command,
    &iARCHI_USERTRAIN_NOK_ONACCEPT.command,
    &iARCHI_USERTRAIN_CANCEL.command,
    &iARCHI_GET_SNR.command,
    &iARCHI_MIKE_SET.command,
    0,
};

static const TosControlCommand *const ARCHI_CONFIRM[] = {
    &iARCHI_USERTRAIN_OK_LASTUTT_CONFIRM.command,
    &iARCHI_USERTRAIN_NOK_ONCONFIRMORSTART.command,
    &iARCHI_USERTRAIN_CANCEL.command,
    &iARCHI_GET_SNR.command,
    &iARCHI_MIKE_SET.command,
    &iARCHI_GOTO_PROMPT.command,
    0,
};

static const TosControlCommand *const ARCHI_UWTRAINED[] = {
    &iARCHI_PUT_SESSION.command,
    &iARCHI_GET_SESSION.command,
    &iARCHI_NO_PITCH_PROCESSING.command,
    &iARCHI_DO_PITCH_PROCESSING.command,
    &iARCHI_LOADLANGUAGE.command,
    &iARCHI_UNLOADLANGUAGE.command,
    &iARCHI_SWITCH_TO_NBESTDP.command,
    &iARCHI_SWITCH_TO_TREEDP.command,
    &iARCHI_CHANGECTX_TOIDLE.command,
    &iARCHI_CTXDELETED.command,
    &iARCHI_MIKE_SET.command,
    &iARCHI_SILADAPTOFF.command,
    &iARCHI_START_ARCHI_CONTROL.command,
    &iARCHI_STOP_ARCHI_CONTROL.command,
    &iARCHI_SILADATPTON.command,
    &iARCHI_VAD_SET_PARAM.command,
    &iARCHI_TRIGGER_SET_PARAM.command,
    &iARCHI_GENDER_SET_PARAM.command,
    &iARCHI_DISTCALC_SET_PARAM.command,
    &iARCHI_NBESTDYNPROG_SET_PARAM.command,
    &iARCHI_USERWORDTRAININGSTART.command,
    &iARCHI_USERWORDTRAININGSTOP.command,
    &iARCHI_INITSCOREUSERWORD.command,
    &iARCHI_SETPARAMSCOREUSERWORD.command,
    &iARCHI_GETPARAMSCOREUSERWORD.command,
    &iARCHI_SWITCH_TO_SCOREUWDP.command,
    &iARCHI_LABEL_START_UW.command,
    &iARCHI_LABEL_STOP_UW.command,
    &iARCHI_DISABLE_M.command,
    &iARCHI_DISABLE_F.command,
    &iARCHI_DISABLE_C.command,
    &iARCHI_ENABLE_M.command,
    &iARCHI_ENABLE_F.command,
    &iARCHI_ENABLE_C.command,
    &iARCHI_GET_DISTCALCFUNCS.command,
    &iARCHI_SET_DISTCALCFUNCS.command,
    &iARCHI_GOTO_PROMPT.command,
    &iARCHI_CONSTRUCTISODPANDLOADLANG.command,
    &iARCHI_DESTRUCTISODP.command,
    0,
};

static const TosControlCommand *const GENDER_FROZEN[] = {
    &iGENDER_DISABLE_M.command,
    &iGENDER_DISABLE_F.command,
    &iGENDER_DISABLE_C.command,
    0,
};

static const TosControlCommand *const TRIGGER_IDLE[] = {
    &iTRIGGER_GARBLED_SOUND_DETECTED.command,
    &iTRIGGER_TS_DETECTED_IGNORE.command,
    0,
};

static const TosControlCommand *const TRIGGER_SLEEP[] = {
    &iTRIGGER_START_DETECTED_TO_NEXTSTATE.command,
    &iTRIGGER_GARBLED_SOUND_DETECTED.command,
    0,
};

static const TosControlCommand *const TRIGGER_RUN[] = {
    &iTRIGGER_TS_DETECTED_TO_NEXTSTATE.command,
    &iTRIGGER_GARBLED_SOUND_DETECTED.command,
    0,
};

static const TosControlCommand *const TRIGGER_RECOVER[] = {
    &iTRIGGER_START_DETECTED_IGNORE.command,
    &iTRIGGER_TS_DETECTED_IGNORE.command,
    &iTRIGGER_GARBLED_SOUND_DETECTED.command,
    0,
};

static const TosControlCommand *const TRIGGER_RECORD[] = {
    &iTRIGGER_TS_DETECTED_TO_NEXTSTATE.command,
    &iTRIGGER_GARBLED_SOUND_DETECTED.command,
    0,
};

static const TosControlCommand *const TRIGGER_RECORDSLEEP[] = {
    &iTRIGGER_START_DETECTED_TO_NEXTSTATE.command,
    &iTRIGGER_GARBLED_SOUND_DETECTED.command,
    0,
};

static const TosControlCommand *const TRIGGER_ACCEPT[] = {
    &iTRIGGER_START_DETECTED_IGNORE.command,
    &iTRIGGER_TS_DETECTED_IGNORE.command,
    &iTRIGGER_GARBLED_SOUND_DETECTED.command,
    0,
};

static const TosControlCommand *const VAD_OTHER_STATES[] = {
    &iVAD_CHANGE_MIKE.command,
    &iVAD_EVENTRECEIVED.command,
    0,
};

static const TosControlCommand *const NBESTDYNPROG_RECOVER[] = {
    &iNBESTDYNPROG_TSDETECTED_IGNORE.command,
    &iNBESTDYNPROG_WORDRECOGNISED.command,
    &iNBESTDYNPROG_NOWORDRECOGNISED.command,
    0,
};

static const TosControlCommand *const ISOWORDTOKENDP_RECOVER[] = {
    &iNBESTDYNPROG_TSDETECTED_IGNORE.command,
    &iISOWORDTOKENDP_WORDRECOGNISED.command,
    &iNBESTDYNPROG_NOWORDRECOGNISED.command,
    0,
};

static const TosControlCommand *const NBESTDYNPROG_SECONDPASS[] = {
    &iNBESTDYNPROG_WORDRECOGNISED_SECONDPASS.command,
    &iNBESTDYNPROG_NOWORDRECOGNISED_SECONDPASS.command,
    0,
};

static const TosControlCommand *const NBESTDYNPROG_SPELLSECONDPASS[] = {
    &iNBESTDYNPROG_WORDRECOGNISED_SPELLSECONDPASS.command,
    &iNBESTDYNPROG_NOWORDRECOGNISED_SPELLSECONDPASS.command,
    0,
};

static const TosControlBlock STD_BOOT[] = {
    { 0x00, { 0, 0, 0 }, ARCHI_BOOT },
    { 0x0B, { 0, 0, 0 }, VAD_BOOT_DATAREADY_IDLE },
    { 0, { 0, 0, 0 }, 0 },
};

static const TosControlBlock STD_DATAREADY[] = {
    { 0x00, { 0, 0, 0 }, ARCHI_DATAREADY },
    { 0x0B, { 0, 0, 0 }, VAD_BOOT_DATAREADY_IDLE },
    { 0x22, { 0, 0, 0 }, DPSCOREUW_DATAREADY },
    { 0x13, { 0, 0, 0 }, GENDER_FROZEN },
    { 0, { 0, 0, 0 }, 0 },
};

static const TosControlBlock STD_IDLE[] = {
    { 0x00, { 0, 0, 0 }, ARCHI_IDLE },
    { 0x27, { 0, 0, 0 }, TRIGGER_IDLE },
    { 0x0B, { 0, 0, 0 }, VAD_BOOT_DATAREADY_IDLE },
    { 0x13, { 0, 0, 0 }, GENDER_FROZEN },
    { 0, { 0, 0, 0 }, 0 },
};

static const TosControlBlock STD_SLEEP[] = {
    { 0x00, { 0, 0, 0 }, ARCHI_SLEEP },
    { 0x27, { 0, 0, 0 }, TRIGGER_SLEEP },
    { 0x0B, { 0, 0, 0 }, VAD_OTHER_STATES },
    { 0x13, { 0, 0, 0 }, GENDER_FROZEN },
    { 0, { 0, 0, 0 }, 0 },
};

static const TosControlBlock STD_RUN[] = {
    { 0x00, { 0, 0, 0 }, ARCHI_RUN },
    { 0x27, { 0, 0, 0 }, TRIGGER_RUN },
    { 0x0B, { 0, 0, 0 }, VAD_OTHER_STATES },
    { 0x20, { 0, 0, 0 }, NBESTDYNPROG_RUN },
    { 0x13, { 0, 0, 0 }, GENDER_FROZEN },
    { 0, { 0, 0, 0 }, 0 },
};

static const TosControlBlock STD_RECOVER[] = {
    { 0x00, { 0, 0, 0 }, ARCHI_RECOVER },
    { 0x27, { 0, 0, 0 }, TRIGGER_RECOVER },
    { 0x0B, { 0, 0, 0 }, VAD_OTHER_STATES },
    { 0x20, { 0, 0, 0 }, NBESTDYNPROG_RECOVER },
    { 0x13, { 0, 0, 0 }, GENDER_FROZEN },
    { 0, { 0, 0, 0 }, 0 },
};

static const TosControlBlock STD_SECONDPASS[] = {
    { 0x00, { 0, 0, 0 }, ARCHI_SECONDPASS },
    { 0x20, { 0, 0, 0 }, NBESTDYNPROG_SECONDPASS },
    { 0x27, { 0, 0, 0 }, TRIGGER_RECOVER },
    { 0, { 0, 0, 0 }, 0 },
};

static const TosControlBlock STD_SPELLSLEEP[] = {
    { 0x00, { 0, 0, 0 }, ARCHI_SPELLSLEEP },
    { 0x27, { 0, 0, 0 }, TRIGGER_SLEEP },
    { 0x0B, { 0, 0, 0 }, VAD_OTHER_STATES },
    { 0, { 0, 0, 0 }, 0 },
};

static const TosControlBlock STD_SPELLRUN[] = {
    { 0x00, { 0, 0, 0 }, ARCHI_RUN },
    { 0x27, { 0, 0, 0 }, TRIGGER_RUN },
    { 0x0B, { 0, 0, 0 }, VAD_OTHER_STATES },
    { 0x20, { 0, 0, 0 }, NBESTDYNPROG_RUN },
    { 0x13, { 0, 0, 0 }, GENDER_FROZEN },
    { 0, { 0, 0, 0 }, 0 },
};

static const TosControlBlock STD_SPELLRECOVER[] = {
    { 0x00, { 0, 0, 0 }, ARCHI_RECOVER },
    { 0x27, { 0, 0, 0 }, TRIGGER_RECOVER },
    { 0x20, { 0, 0, 0 }, NBESTDYNPROG_RECOVER },
    { 0x0B, { 0, 0, 0 }, VAD_OTHER_STATES },
    { 0x13, { 0, 0, 0 }, GENDER_FROZEN },
    { 0, { 0, 0, 0 }, 0 },
};

static const TosControlBlock STD_SPELLSECONDPASS[] = {
    { 0x00, { 0, 0, 0 }, ARCHI_SPELLSECONDPASS },
    { 0x20, { 0, 0, 0 }, NBESTDYNPROG_SPELLSECONDPASS },
    { 0x27, { 0, 0, 0 }, TRIGGER_RECOVER },
    { 0, { 0, 0, 0 }, 0 },
};

static const TosControlBlock STD_ISOWORDSLEEP[] = {
    { 0x00, { 0, 0, 0 }, ARCHI_ISOWORDSLEEP },
    { 0x27, { 0, 0, 0 }, TRIGGER_SLEEP },
    { 0x0B, { 0, 0, 0 }, VAD_OTHER_STATES },
    { 0, { 0, 0, 0 }, 0 },
};

static const TosControlBlock STD_ISOWORDRUN[] = {
    { 0x00, { 0, 0, 0 }, ARCHI_RUN },
    { 0x27, { 0, 0, 0 }, TRIGGER_RUN },
    { 0x0B, { 0, 0, 0 }, VAD_OTHER_STATES },
    { 0x23, { 0, 0, 0 }, NBESTDYNPROG_RUN },
    { 0x13, { 0, 0, 0 }, GENDER_FROZEN },
    { 0, { 0, 0, 0 }, 0 },
};

static const TosControlBlock STD_ISOWORDRECOVER[] = {
    { 0x00, { 0, 0, 0 }, ARCHI_RECOVER },
    { 0x27, { 0, 0, 0 }, TRIGGER_RECOVER },
    { 0x0B, { 0, 0, 0 }, VAD_OTHER_STATES },
    { 0x23, { 0, 0, 0 }, ISOWORDTOKENDP_RECOVER },
    { 0x13, { 0, 0, 0 }, GENDER_FROZEN },
    { 0, { 0, 0, 0 }, 0 },
};

static const TosControlBlock STD_PROMPT[] = {
    { 0x00, { 0, 0, 0 }, ARCHI_PROMPT },
    { 0, { 0, 0, 0 }, 0 },
};

static const TosControlBlock STD_RECORDSLEEP[] = {
    { 0x00, { 0, 0, 0 }, ARCHI_RECORDSLEEP },
    { 0x27, { 0, 0, 0 }, TRIGGER_RECORDSLEEP },
    { 0x0B, { 0, 0, 0 }, VAD_OTHER_STATES },
    { 0x13, { 0, 0, 0 }, GENDER_FROZEN },
    { 0, { 0, 0, 0 }, 0 },
};

static const TosControlBlock STD_RECORD[] = {
    { 0x00, { 0, 0, 0 }, ARCHI_RECORD },
    { 0x27, { 0, 0, 0 }, TRIGGER_RECORD },
    { 0x0B, { 0, 0, 0 }, VAD_OTHER_STATES },
    { 0x21, { 0, 0, 0 }, DPGENUW_RECORD },
    { 0x13, { 0, 0, 0 }, GENDER_FROZEN },
    { 0, { 0, 0, 0 }, 0 },
};

static const TosControlBlock STD_ACCEPT[] = {
    { 0x00, { 0, 0, 0 }, ARCHI_ACCEPT },
    { 0x27, { 0, 0, 0 }, TRIGGER_ACCEPT },
    { 0x0B, { 0, 0, 0 }, VAD_OTHER_STATES },
    { 0x21, { 0, 0, 0 }, DPGENUW_ACCEPT },
    { 0x13, { 0, 0, 0 }, GENDER_FROZEN },
    { 0, { 0, 0, 0 }, 0 },
};

static const TosControlBlock STD_CONFIRM[] = {
    { 0x00, { 0, 0, 0 }, ARCHI_CONFIRM },
    { 0x0B, { 0, 0, 0 }, VAD_OTHER_STATES },
    { 0, { 0, 0, 0 }, 0 },
};

static const TosControlBlock STD_UWTRAINED[] = {
    { 0x00, { 0, 0, 0 }, ARCHI_UWTRAINED },
    { 0x0B, { 0, 0, 0 }, VAD_BOOT_DATAREADY_IDLE },
    { 0x22, { 0, 0, 0 }, DPSCOREUW_DATAREADY },
    { 0, { 0, 0, 0 }, 0 },
};

static const TosControlBlock *const STD_[] = {
    STD_BOOT,
    STD_DATAREADY,
    STD_IDLE,
    STD_SLEEP,
    STD_RUN,
    STD_RECOVER,
    STD_SECONDPASS,
    STD_SPELLSLEEP,
    STD_SPELLRUN,
    STD_SPELLRECOVER,
    STD_SPELLSECONDPASS,
    STD_ISOWORDSLEEP,
    STD_ISOWORDRUN,
    STD_ISOWORDRECOVER,
    STD_SECONDPASS,
    STD_PROMPT,
    STD_RECORDSLEEP,
    STD_RECORD,
    STD_ACCEPT,
    STD_CONFIRM,
    STD_UWTRAINED,
    0,
};

const TosControlBlock *const *const ControlTable[] = {
    STD_,
    0,
    0,
};
