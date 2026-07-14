#include "types.h"

#include "gssdk/tos.h"

typedef struct TosProfileValueU32 {
    u16 key;
    u16 reserved;
    u32 value;
} TosProfileValueU32;

typedef struct TosProfileValueF32 {
    u16 key;
    u16 reserved;
    f32 value;
} TosProfileValueF32;

typedef struct TosProfile {
    const TosProfileValueF32 *floatValues;
    const TosProfileValueU32 *u32Values;
} TosProfile;

typedef void *(*TosConstructFunction)(TosContext *, u32);

typedef struct TosInitialProc {
    u8 blockID;
    u8 construct;
    u16 reserved;
    TosConstructFunction constructor;
} TosInitialProc;

typedef struct TosQueueWriter {
    u8 queueID;
    u8 construct;
    u8 blockID;
} TosQueueWriter;

typedef struct TosQueueReader {
    u8 queueID;
    u8 blockID;
} TosQueueReader;

extern void *ConstructWindow(TosContext *, u32);
extern void *ConstructFFTMod(TosContext *, u32);
extern void *ConstructMel(TosContext *, u32);
extern void *ConstructVad(TosContext *, u32);
extern void *ConstructLookAhead(TosContext *, u32);
extern void *ConstructSpecSub(TosContext *, u32);
extern void *ConstructStationarity(TosContext *, u32);
extern void *ConstructAcne(TosContext *, u32);
extern void *ConstructDCTLift(TosContext *, u32);
extern void *ConstructGender(TosContext *, u32);
extern void *ConstructCombiner(TosContext *, u32);
extern void *ConstructUndersampler(TosContext *, u32);
extern void *ConstructDelayBlock(TosContext *, u32);
extern void *ConstructPitchWindow(TosContext *, u32);
extern void *ConstructSHS_VUV(TosContext *, u32);
extern void *ConstructPitchDP(TosContext *, u32);
extern void *ConstructGenderFilter(TosContext *, u32);
extern void *ConstructPitchCombiner(TosContext *, u32);
extern void *ConstructStacker(TosContext *, u32);
extern void *ConstructVq(TosContext *, u32);
extern void *ConstructDistCalc(TosContext *, u32);
extern void *ConstructNBestDynProg(TosContext *, u32);
extern void *ConstructDpGenUw(TosContext *, u32);
extern void *ConstructDpScoreUw(TosContext *, u32);
extern void *ConstructIsoWordTokenDp(TosContext *, u32);
extern void *ConstructExtraEventDp(TosContext *, u32);
extern void *ConstructMedian(TosContext *, u32);
extern void *ConstructTriggerLR(TosContext *, u32);

static const TosProfileValueU32 FrameWin_KeyVals[] = {
    { 2, 0, 0x100 },
    { 0, 0, 0 },
};

static const TosProfileValueU32 FFTMod_KeyVals_U32[] = {
    { 6, 0, 0x100 },
    { 4, 0, 0x100 },
    { 5, 0, 0x9C40 },
    { 0, 0, 0 },
};

static const TosProfileValueF32 FFTMod_KeyVals_FLT[] = {
    { 1, 0, 400.0f },
    { 2, 0, 4500.0f },
    { 5, 0, 40000.0f },
    { 0, 0, 0.0f },
};

static const TosProfileValueU32 Mel_KeyVals_U32[] = {
    { 3, 0, 0x81 },
    { 4, 0, 0x15 },
    { 0, 0, 0 },
};

static const TosProfileValueF32 Mel_KeyVals_FLT[] = {
    { 2, 0, 5500.0f },
    { 0, 0, 0.0f },
};

static const TosProfileValueF32 Vad_KeyVals_FLT[] = {
    { 0x24, 0, 15.0f },
    { 0, 0, 0.0f },
};

static const TosProfileValueU32 LookAhead_KeyVals_U32[] = {
    { 1, 0, 5 },
    { 0, 0, 0 },
};

static const TosProfileValueF32 SpecSub_KeyVals_FLT[] = {
    { 2, 0, 0.02f },
    { 7, 0, 0.84f },
    { 3, 0, 1.5f },
    { 0, 0, 0.0f },
};

static const TosProfileValueU32 SpecSub_KeyVals_U32[] = {
    { 6, 0, 3 },
    { 0x1E, 0, 3 },
    { 0x1F, 0, 10 },
    { 0x20, 0, 16 },
    { 0x21, 0, 21 },
    { 0, 0, 0 },
};

static const TosProfileValueF32 Vad2_KeyVals_FLT[] = {
    { 0x24, 0, 15.0f },
    { 0x2B, 0, 26.525f },
    { 0, 0, 0.0f },
};

static const TosProfileValueU32 Vad2_KeyVals_U32[] = {
    { 0x27, 0, 1 },
    { 0x28, 0, 0 },
    { 0x2A, 0, 1 },
    { 0x25, 0, 1 },
    { 0x0D, 0, 2000 },
    { 0x2D, 0, 3 },
    { 0, 0, 0 },
};

static const TosProfileValueF32 Stationarity_KeyVals_FLT[] = {
    { 2, 0, 2.0f },
    { 0, 0, 0.0f },
};

static const TosProfileValueU32 Stationarity_KeyVals_U32[] = {
    { 1, 0, 2 },
    { 0, 0, 0 },
};

static const TosProfileValueF32 Acne_KeyVals_FLT[] = {
    { 3, 0, 6.0f },
    { 4, 0, -15.0f },
    { 5, 0, 50.0f },
    { 0, 0, 0.0f },
};

static const TosProfileValueU32 Acne_KeyVals_U32[] = {
    { 1, 0, 0 },
    { 6, 0, 15 },
    { 2, 0, 50 },
    { 5, 0, 50 },
    { 0, 0, 0 },
};

static const TosProfileValueU32 DCTLift_KeyVals_U32[] = {
    { 1, 0, 21 },
    { 4, 0, 1 },
    { 2, 0, 13 },
    { 0, 0, 0 },
};

static const TosProfileValueU32 Gender_KeyVals_U32[] = {
    { 6, 0, 14 },
    { 5, 0, 2 },
    { 1, 0, 50 },
    { 0, 0, 0 },
};

static const TosProfileValueU32 Undersampler_KeyVals_U32[] = {
    { 1, 0, 110 },
    { 2, 0, 11000 },
    { 3, 0, 2000 },
    { 0, 0, 0 },
};

static const TosProfileValueU32 UndersamplerDelayBlock_KeyVals_U32[] = {
    { 1, 0, 6 },
    { 2, 0, 88 },
    { 0, 0, 0 },
};

static const TosProfileValueF32 PitchWindow_KeyVals_FLT[] = {
    { 2, 0, 0.04f },
    { 0, 0, 0.0f },
};

static const TosProfileValueU32 PitchWindow_KeyVals_U32[] = {
    { 1, 0, 2200 },
    { 3, 0, 110 },
    { 4, 0, 11000 },
    { 0, 0, 0 },
};

static const TosProfileValueU32 PitchFFT_KeyVals_U32[] = {
    { 7, 0, 0 },
    { 4, 0, 128 },
    { 6, 0, 88 },
    { 0, 0, 0 },
};

static const TosProfileValueU32 SHS_VUV_KeyVals_U32[] = {
    { 2, 0, 2200 },
    { 3, 0, 65 },
    { 0, 0, 0 },
};

static const TosProfileValueF32 PitchDP_KeyVals_FLT[] = {
    { 9, 0, 0.01f },
    { 0, 0, 0.0f },
};

static const TosProfileValueU32 PitchDP_KeyVals_U32[] = {
    { 2, 0, 500 },
    { 3, 0, 65 },
    { 4, 0, 15 },
    { 0, 0, 0 },
};

static const TosProfileValueU32 MelNormGenderFilter_KeyVals_U32[] = {
    { 2, 0, 88 },
    { 0, 0, 0 },
};

static const TosProfileValueU32 Stacker_Keyvals_U32[] = {
    { 3, 0, 2 },
    { 1, 0, 152 },
    { 2, 0, 20 },
    { 0, 0, 0 },
};

static const TosProfileValueU32 Vq_KeyVals_U32[] = {
    { 2, 0, 5 },
    { 1, 0, 43 },
    { 0, 0, 0 },
};

static const TosProfileValueU32 DistCalc_KeyVals_U32[] = {
    { 0x20, 0, 5 },
    { 0x1E, 0, 0 },
    { 0x1F, 0, 0 },
    { 0x13, 0, 32 },
    { 0x14, 0, 0 },
    { 0, 0, 0 },
};

static const TosProfileValueF32 NBestDynProg_KeyVals_FLT[] = {
    { 0x0A, 0, 0.2f },
    { 0, 0, 0.0f },
};

static const TosProfileValueU32 NBestDynProg_KeyVals_U32[] = {
    { 6, 0, 60 },
    { 0x0B, 0, 5000 },
    { 9, 0, 300 },
    { 0x0E, 0, 512 },
    { 7, 0, 10 },
    { 0x0F, 0, 5 },
    { 0, 0, 0 },
};

static const TosProfileValueU32 DpGenUwID_KeyVals_U32[] = {
    { 0x13, 0, 1 },
    { 0x14, 0, 120 },
    { 0x23, 0, 900 },
    { 0, 0, 0 },
};

static const TosProfileValueU32 DpScoreUw_KeyVals_U32[] = {
    { 0x13, 0, 1 },
    { 0, 0, 0 },
};

static const TosProfileValueU32 IsoWordTokenDp_KeyVals_U32[] = {
    { 0x0F, 0, 5 },
    { 0, 0, 0 },
};

static const TosProfileValueU32 ExtraEventDp_KeyVals_U32[] = {
    { 0x0F, 0, 5 },
    { 0, 0, 0 },
};

static const TosProfileValueU32 LogEMedian_KeyVals_U32[] = {
    { 1, 0, 9 },
    { 0, 0, 0 },
};

static const TosProfileValueU32 Trigger_KeyVals_U32[] = {
    { 0x11, 0, 30 },
    { 3, 0, 10 },
    { 4, 0, 45 },
    { 0x0E, 0, 100 },
    { 6, 0, 10 },
    { 0, 0, 0 },
};

static const TosProfileValueF32 Trigger_KeyVals_FLT[] = {
    { 9, 0, 3.0f },
    { 8, 0, 1.0f },
    { 0x0C, 0, 10.5f },
    { 0x0D, 0, 31.0f },
    { 0, 0, 0.0f },
};

static const TosProfileValueU32 MelSpecSubQueue_KeyVals[] = {
    { 4, 0, 45 },
    { 0, 0, 0 },
};

static const TosProfileValueU32 NoiseSpecQueue_KeyVals[] = {
    { 4, 0, 45 },
    { 0, 0, 0 },
};

static const TosProfileValueU32 SilDetectQueue_KeyVals[] = {
    { 4, 0, 45 },
    { 0, 0, 0 },
};

static const TosProfileValueU32 Vad2Queue_KeyVals[] = {
    { 4, 0, 45 },
    { 0, 0, 0 },
};

static const TosProfileValueU32 LevelsQueue_KeyVals[] = {
    { 4, 0, 45 },
    { 0, 0, 0 },
};

static const TosProfileValueU32 LabelQueue_KeyVals[] = {
    { 2, 0, 5 },
    { 0, 0, 0 },
};

static const TosProfileValueU32 UndersamplerDelayBlockQueue_KeyVals[] = {
    { 4, 0, 45 },
    { 0, 0, 0 },
};

const TosProfile IniFile[87] = {
    { NULL, NULL },                              /* 0x00 */
    { NULL, FrameWin_KeyVals },                  /* 0x01 */
    { FFTMod_KeyVals_FLT, FFTMod_KeyVals_U32 },  /* 0x02 */
    { Mel_KeyVals_FLT, Mel_KeyVals_U32 },        /* 0x03 */
    { Mel_KeyVals_FLT, Mel_KeyVals_U32 },        /* 0x04 */
    { Mel_KeyVals_FLT, Mel_KeyVals_U32 },        /* 0x05 */
    { Vad_KeyVals_FLT, NULL },                   /* 0x06 */
    { NULL, LookAhead_KeyVals_U32 },             /* 0x07 */
    { SpecSub_KeyVals_FLT, SpecSub_KeyVals_U32 },/* 0x08 */
    { SpecSub_KeyVals_FLT, SpecSub_KeyVals_U32 },/* 0x09 */
    { SpecSub_KeyVals_FLT, SpecSub_KeyVals_U32 },/* 0x0A */
    { Vad2_KeyVals_FLT, Vad2_KeyVals_U32 },      /* 0x0B */
    { Stationarity_KeyVals_FLT,
      Stationarity_KeyVals_U32 },                /* 0x0C */
    { Acne_KeyVals_FLT, Acne_KeyVals_U32 },      /* 0x0D */
    { Acne_KeyVals_FLT, Acne_KeyVals_U32 },      /* 0x0E */
    { Acne_KeyVals_FLT, Acne_KeyVals_U32 },      /* 0x0F */
    { NULL, DCTLift_KeyVals_U32 },               /* 0x10 */
    { NULL, DCTLift_KeyVals_U32 },               /* 0x11 */
    { NULL, DCTLift_KeyVals_U32 },               /* 0x12 */
    { NULL, Gender_KeyVals_U32 },                /* 0x13 */
    { NULL, NULL },                              /* 0x14 */
    { NULL, Undersampler_KeyVals_U32 },          /* 0x15 */
    { NULL, UndersamplerDelayBlock_KeyVals_U32 },/* 0x16 */
    { PitchWindow_KeyVals_FLT,
      PitchWindow_KeyVals_U32 },                 /* 0x17 */
    { NULL, PitchFFT_KeyVals_U32 },              /* 0x18 */
    { NULL, SHS_VUV_KeyVals_U32 },               /* 0x19 */
    { PitchDP_KeyVals_FLT, PitchDP_KeyVals_U32 },/* 0x1A */
    { NULL, MelNormGenderFilter_KeyVals_U32 },   /* 0x1B */
    { NULL, NULL },                              /* 0x1C */
    { NULL, Stacker_Keyvals_U32 },               /* 0x1D */
    { NULL, Vq_KeyVals_U32 },                    /* 0x1E */
    { NULL, DistCalc_KeyVals_U32 },              /* 0x1F */
    { NBestDynProg_KeyVals_FLT,
      NBestDynProg_KeyVals_U32 },                /* 0x20 */
    { NULL, DpGenUwID_KeyVals_U32 },             /* 0x21 */
    { NULL, DpScoreUw_KeyVals_U32 },             /* 0x22 */
    { NULL, IsoWordTokenDp_KeyVals_U32 },        /* 0x23 */
    { NULL, ExtraEventDp_KeyVals_U32 },          /* 0x24 */
    { NULL, NULL },                              /* 0x25 */
    { NULL, LogEMedian_KeyVals_U32 },            /* 0x26 */
    { Trigger_KeyVals_FLT, Trigger_KeyVals_U32 },/* 0x27 */
    { NULL, NULL },                              /* 0x28 */
    { NULL, NULL },                              /* 0x29 */
    { NULL, NULL },                              /* 0x2A */
    { NULL, NULL },                              /* 0x2B */
    { NULL, NULL },                              /* 0x2C */
    { NULL, NULL },                              /* 0x2D */
    { NULL, NULL },                              /* 0x2E */
    { NULL, NULL },                              /* 0x2F */
    { NULL, SilDetectQueue_KeyVals },            /* 0x30 */
    { NULL, NULL },                              /* 0x31 */
    { NULL, NULL },                              /* 0x32 */
    { NULL, NULL },                              /* 0x33 */
    { NULL, MelSpecSubQueue_KeyVals },           /* 0x34 */
    { NULL, MelSpecSubQueue_KeyVals },           /* 0x35 */
    { NULL, MelSpecSubQueue_KeyVals },           /* 0x36 */
    { NULL, NoiseSpecQueue_KeyVals },            /* 0x37 */
    { NULL, NoiseSpecQueue_KeyVals },            /* 0x38 */
    { NULL, NoiseSpecQueue_KeyVals },            /* 0x39 */
    { NULL, Vad2Queue_KeyVals },                 /* 0x3A */
    { NULL, LevelsQueue_KeyVals },               /* 0x3B */
    { NULL, NULL },                              /* 0x3C */
    { NULL, NULL },                              /* 0x3D */
    { NULL, NULL },                              /* 0x3E */
    { NULL, NULL },                              /* 0x3F */
    { NULL, NULL },                              /* 0x40 */
    { NULL, NULL },                              /* 0x41 */
    { NULL, NULL },                              /* 0x42 */
    { NULL, NULL },                              /* 0x43 */
    { NULL, NULL },                              /* 0x44 */
    { NULL, LabelQueue_KeyVals },                /* 0x45 */
    { NULL, NULL },                              /* 0x46 */
    { NULL, NULL },                              /* 0x47 */
    { NULL, NULL },                              /* 0x48 */
    { NULL, UndersamplerDelayBlockQueue_KeyVals },/* 0x49 */
    { NULL, NULL },                              /* 0x4A */
    { NULL, NULL },                              /* 0x4B */
    { NULL, NULL },                              /* 0x4C */
    { NULL, NULL },                              /* 0x4D */
    { NULL, NULL },                              /* 0x4E */
    { NULL, NULL },                              /* 0x4F */
    { NULL, NULL },                              /* 0x50 */
    { NULL, NULL },                              /* 0x51 */
    { NULL, NULL },                              /* 0x52 */
    { NULL, NULL },                              /* 0x53 */
    { NULL, NULL },                              /* 0x54 */
    { NULL, NULL },                              /* 0x55 */
    { NULL, NULL },                              /* 0x56 */
};

const TosInitialProc InitialProcs[] = {
    { 0x00, 1, 0, NULL },
    { 0x01, 1, 0, ConstructWindow },
    { 0x02, 1, 0, ConstructFFTMod },
    { 0x03, 0, 0, ConstructMel },
    { 0x04, 0, 0, ConstructMel },
    { 0x05, 0, 0, ConstructMel },
    { 0x06, 1, 0, ConstructVad },
    { 0x07, 1, 0, ConstructLookAhead },
    { 0x08, 0, 0, ConstructSpecSub },
    { 0x09, 0, 0, ConstructSpecSub },
    { 0x0A, 0, 0, ConstructSpecSub },
    { 0x0B, 1, 0, ConstructVad },
    { 0x0C, 1, 0, ConstructStationarity },
    { 0x0D, 0, 0, ConstructAcne },
    { 0x0E, 0, 0, ConstructAcne },
    { 0x0F, 0, 0, ConstructAcne },
    { 0x10, 0, 0, ConstructDCTLift },
    { 0x11, 0, 0, ConstructDCTLift },
    { 0x12, 0, 0, ConstructDCTLift },
    { 0x13, 1, 0, ConstructGender },
    { 0x14, 1, 0, ConstructCombiner },
    { 0x15, 0, 0, ConstructUndersampler },
    { 0x16, 0, 0, ConstructDelayBlock },
    { 0x17, 0, 0, ConstructPitchWindow },
    { 0x18, 0, 0, ConstructFFTMod },
    { 0x19, 0, 0, ConstructSHS_VUV },
    { 0x1A, 0, 0, ConstructPitchDP },
    { 0x1B, 0, 0, ConstructGenderFilter },
    { 0x1C, 0, 0, ConstructPitchCombiner },
    { 0x1D, 1, 0, ConstructStacker },
    { 0x1E, 1, 0, ConstructVq },
    { 0x1F, 1, 0, ConstructDistCalc },
    { 0x20, 1, 0, ConstructNBestDynProg },
    { 0x21, 0, 0, ConstructDpGenUw },
    { 0x22, 0, 0, ConstructDpScoreUw },
    { 0x23, 1, 0, ConstructIsoWordTokenDp },
    { 0x24, 1, 0, ConstructExtraEventDp },
    { 0x25, 1, 0, ConstructGenderFilter },
    { 0x26, 1, 0, ConstructMedian },
    { 0x27, 1, 0, ConstructTriggerLR },
    { 0xFF, 1, 0, NULL },
};

const TosQueueWriter QueueWriters[] = {
    { 0x28, 1, 0x00 },
    { 0x29, 1, 0x01 },
    { 0x2A, 1, 0x02 },
    { 0x2B, 1, 0x02 },
    { 0x2C, 0, 0x03 },
    { 0x2D, 0, 0x04 },
    { 0x2E, 0, 0x05 },
    { 0x2F, 1, 0x06 },
    { 0x30, 1, 0x07 },
    { 0x30, 1, 0x00 },
    { 0x31, 0, 0x08 },
    { 0x32, 0, 0x09 },
    { 0x33, 0, 0x0A },
    { 0x34, 0, 0x08 },
    { 0x35, 0, 0x09 },
    { 0x36, 0, 0x0A },
    { 0x37, 0, 0x08 },
    { 0x38, 0, 0x09 },
    { 0x39, 0, 0x0A },
    { 0x3A, 1, 0x0B },
    { 0x3B, 1, 0x0B },
    { 0x3C, 1, 0x0C },
    { 0x3D, 0, 0x0D },
    { 0x3E, 0, 0x0E },
    { 0x3F, 0, 0x0F },
    { 0x40, 1, 0x10 },
    { 0x41, 1, 0x11 },
    { 0x42, 1, 0x12 },
    { 0x43, 1, 0x13 },
    { 0x43, 1, 0x00 },
    { 0x44, 1, 0x14 },
    { 0x45, 1, 0x1E },
    { 0x45, 1, 0x00 },
    { 0x46, 0, 0x1F },
    { 0x47, 0, 0x21 },
    { 0x47, 0, 0x22 },
    { 0x48, 0, 0x15 },
    { 0x49, 0, 0x16 },
    { 0x4A, 0, 0x17 },
    { 0x4B, 0, 0x18 },
    { 0x4C, 0, 0x19 },
    { 0x4D, 0, 0x1A },
    { 0x4E, 0, 0x19 },
    { 0x4F, 0, 0x1C },
    { 0x50, 1, 0x1D },
    { 0x51, 0, 0x08 },
    { 0x52, 0, 0x09 },
    { 0x53, 0, 0x0A },
    { 0x54, 1, 0x25 },
    { 0x55, 1, 0x26 },
    { 0x56, 0, 0x1B },
    { 0xFF, 1, 0xFF },
};

const TosQueueReader QueueReaders[] = {
    { 0x28, 0x01 },
    { 0x28, 0x15 },
    { 0x28, 0x27 },
    { 0x29, 0x02 },
    { 0x2A, 0x06 },
    { 0x2B, 0x03 },
    { 0x2B, 0x04 },
    { 0x2B, 0x05 },
    { 0x2C, 0x08 },
    { 0x2D, 0x09 },
    { 0x2E, 0x0A },
    { 0x2F, 0x07 },
    { 0x30, 0x08 },
    { 0x30, 0x09 },
    { 0x30, 0x0A },
    { 0x30, 0x1F },
    { 0x30, 0x20 },
    { 0x30, 0x23 },
    { 0x31, 0x0B },
    { 0x32, 0x0B },
    { 0x33, 0x0B },
    { 0x34, 0x0D },
    { 0x35, 0x0E },
    { 0x36, 0x0F },
    { 0x37, 0x0D },
    { 0x38, 0x0E },
    { 0x39, 0x0F },
    { 0x3B, 0x0C },
    { 0x3A, 0x0C },
    { 0x3C, 0x0D },
    { 0x3C, 0x0E },
    { 0x3C, 0x0F },
    { 0x3B, 0x0D },
    { 0x3B, 0x0E },
    { 0x3B, 0x0F },
    { 0x3B, 0x19 },
    { 0x3B, 0x27 },
    { 0x3B, 0x26 },
    { 0x3D, 0x10 },
    { 0x3D, 0x1B },
    { 0x3E, 0x11 },
    { 0x3E, 0x1B },
    { 0x3F, 0x12 },
    { 0x3F, 0x1B },
    { 0x3C, 0x13 },
    { 0x40, 0x13 },
    { 0x41, 0x13 },
    { 0x42, 0x13 },
    { 0x43, 0x14 },
    { 0x44, 0x1D },
    { 0x48, 0x16 },
    { 0x49, 0x17 },
    { 0x4A, 0x18 },
    { 0x4B, 0x19 },
    { 0x4C, 0x1A },
    { 0x4D, 0x1C },
    { 0x4E, 0x1C },
    { 0x4F, 0x1D },
    { 0x50, 0x1E },
    { 0x45, 0x20 },
    { 0x45, 0x1F },
    { 0x45, 0x00 },
    { 0x45, 0x23 },
    { 0x45, 0x24 },
    { 0x46, 0x21 },
    { 0x46, 0x22 },
    { 0x47, 0x1F },
    { 0x51, 0x25 },
    { 0x52, 0x25 },
    { 0x53, 0x25 },
    { 0x54, 0x27 },
    { 0x55, 0x27 },
    { 0x56, 0x1C },
    { 0xFF, 0xFF },
};
