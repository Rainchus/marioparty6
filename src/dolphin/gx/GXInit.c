#include <dolphin/PPCArch.h>
#include <dolphin/gx.h>
#include <dolphin/os.h>
#include <dolphin/vi.h>

#include <dolphin/gx/GXPriv.h>

const char* __GXVersion =
    "<< Dolphin SDK - GX\trelease build: Apr  5 2004 04:14:28 (0x2301) >>";

static GXFifoObj FifoObj;

static GXData gxData;
GXData* const __GXData = &gxData;

u32* __piReg = NULL;
u16* __cpReg = NULL;
u16* __peReg = NULL;
u16* __memReg = NULL;

static u16 DefaultTexData[] __attribute__((aligned(32))) = {
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
};

static GXVtxAttrFmtList GXDefaultVATList[] = {
    {GX_VA_POS, GX_POS_XYZ, GX_F32, 0},
    {GX_VA_NRM, GX_NRM_XYZ, GX_F32, 0},
    {GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0},
    {GX_VA_CLR1, GX_CLR_RGBA, GX_RGBA8, 0},
    {GX_VA_TEX0, GX_TEX_ST, GX_F32, 0},
    {GX_VA_TEX1, GX_TEX_ST, GX_F32, 0},
    {GX_VA_TEX2, GX_TEX_ST, GX_F32, 0},
    {GX_VA_TEX3, GX_TEX_ST, GX_F32, 0},
    {GX_VA_TEX4, GX_TEX_ST, GX_F32, 0},
    {GX_VA_TEX5, GX_TEX_ST, GX_F32, 0},
    {GX_VA_TEX6, GX_TEX_ST, GX_F32, 0},
    {GX_VA_TEX7, GX_TEX_ST, GX_F32, 0},
    {GX_VA_NULL, 0, 0, 0},
};

static f32 GXDefaultProjData[] = {1.0f, 0.0f, 1.0f, 0.0f, -1.0f, -2.0f, 0.0f};

static u32 GXTexRegionAddrTable[] = {
    0x00000, 0x10000, 0x20000, 0x30000, 0x40000, 0x50000, 0x60000, 0x70000,
    0x08000, 0x18000, 0x28000, 0x38000, 0x48000, 0x58000, 0x68000, 0x78000,
    0x00000, 0x90000, 0x20000, 0xB0000, 0x40000, 0x98000, 0x60000, 0xB8000,
    0x80000, 0x10000, 0xA0000, 0x30000, 0x88000, 0x50000, 0xA8000, 0x70000,
    0x00000, 0x90000, 0x20000, 0xB0000, 0x40000, 0x90000, 0x60000, 0xB0000,
    0x80000, 0x10000, 0xA0000, 0x30000, 0x80000, 0x50000, 0xA0000, 0x70000,
};

static BOOL __GXShutdown(BOOL final);

static OSResetFunctionInfo GXResetFuncInfo = {
    __GXShutdown,
    OS_RESET_PRIO_GX,
    NULL,
    NULL,
};

static void EnableWriteGatherPipe(void)
{
    u32 hid2 = PPCMfhid2();

    PPCMtwpar(OSUncachedToPhysical((void*)GXFIFO_ADDR));
    hid2 |= 0x40000000;
    PPCMthid2(hid2);
}

static GXTexRegion* __GXDefaultTexRegionCallback(const GXTexObj* t_obj, GXTexMapID id)
{
    GXTexFmt fmt;
    u8 mm;

    fmt = GXGetTexObjFmt(t_obj);
    mm = GXGetTexObjMipMap(t_obj);
    id = (GXTexMapID)(id % GX_MAX_TEXMAP);

    switch (fmt) {
        case GX_TF_RGBA8:
            if (mm) {
                return &gx->TexRegions2[id];
            }
            return &gx->TexRegions1[id];
        case GX_TF_C4:
        case GX_TF_C8:
        case GX_TF_C14X2:
            return &gx->TexRegions0[id];
        default:
            if (mm) {
                return &gx->TexRegions1[id];
            }
            return &gx->TexRegions0[id];
    }
}

static GXTlutRegion* __GXDefaultTlutRegionCallback(u32 idx)
{
    if (idx >= 20) {
        return NULL;
    }
    return &gx->TlutRegions[idx];
}

static BOOL __GXShutdown(BOOL final)
{
    static u32 peCount;
    static OSTime time;
    static u32 calledOnce;
    u32 reg;
    u32 peCountNew;
    OSTime timeNew;

    if (!final) {
        if (!calledOnce) {
            peCount = __GXReadMEMCounterU32(0x28, 0x27);
            time = OSGetTime();
            calledOnce = 1;
            return FALSE;
        }

        timeNew = OSGetTime();
        peCountNew = __GXReadMEMCounterU32(0x28, 0x27);

        if (timeNew - time < 10) {
            return FALSE;
        }

        if (peCountNew != peCount) {
            peCount = peCountNew;
            time = timeNew;
            return FALSE;
        }
    } else {
        GXSetBreakPtCallback(NULL);
        GXSetDrawSyncCallback(NULL);
        GXSetDrawDoneCallback(NULL);

        GX_WRITE_U32(0);
        GX_WRITE_U32(0);
        GX_WRITE_U32(0);
        GX_WRITE_U32(0);
        GX_WRITE_U32(0);
        GX_WRITE_U32(0);
        GX_WRITE_U32(0);
        GX_WRITE_U32(0);

        PPCSync();

        reg = 0;
        GX_SET_CP_REG(1, reg);

        reg = 3;
        GX_SET_CP_REG(2, reg);

        gx->abtWaitPECopy = TRUE;
        __GXAbort();
    }

    return TRUE;
}

void __GXInitRevisionBits(void)
{
    u32 i;

    for (i = 0; i < GX_MAX_VTXFMT; i++) {
        SET_REG_FIELD(0, gx->vatA[i], 1, 30, 1);
        SET_REG_FIELD(0, gx->vatB[i], 1, 31, 1);

        GX_WRITE_U8(0x8);
        GX_WRITE_U8(i | 0x80);
        GX_WRITE_U32(gx->vatB[i]);
    }

    {
        u32 reg1 = 0;
        u32 reg2 = 0;

        SET_REG_FIELD(0, reg1, 1, 0, 1);
        SET_REG_FIELD(0, reg1, 1, 1, 1);
        SET_REG_FIELD(0, reg1, 1, 2, 1);
        SET_REG_FIELD(0, reg1, 1, 3, 1);
        SET_REG_FIELD(0, reg1, 1, 4, 1);
        SET_REG_FIELD(0, reg1, 1, 5, 1);
        GX_WRITE_XF_REG(0, reg1);

        SET_REG_FIELD(0, reg2, 1, 0, 1);
        GX_WRITE_XF_REG(0x12, reg2);
    }

    {
        u32 reg = 0;

        SET_REG_FIELD(0, reg, 1, 0, 1);
        SET_REG_FIELD(0, reg, 1, 1, 1);
        SET_REG_FIELD(0, reg, 1, 2, 1);
        SET_REG_FIELD(0, reg, 1, 3, 1);
        SET_REG_FIELD(0, reg, 8, 24, 0x58);
        GX_WRITE_RAS_REG(reg);
    }
}

GXFifoObj* GXInit(void* base, u32 size)
{
    static u32 resetFuncRegistered;
    u32 i;
    u32 reg;
    u32 freqBase;

    OSRegisterVersion(__GXVersion);

    gx->inDispList = FALSE;
    gx->dlSaveContext = TRUE;
    gx->abtWaitPECopy = TRUE;
    gx->tcsManEnab = FALSE;
    gx->tevTcEnab = FALSE;

    GXSetMisc(GX_MT_XF_FLUSH, 0);

    __piReg = OSPhysicalToUncached(0xC003000);
    __cpReg = OSPhysicalToUncached(0xC000000);
    __peReg = OSPhysicalToUncached(0xC001000);
    __memReg = OSPhysicalToUncached(0xC004000);
    __GXFifoInit();
    GXInitFifoBase(&FifoObj, base, size);
    GXSetCPUFifo(&FifoObj);
    GXSetGPFifo(&FifoObj);

    if (!resetFuncRegistered) {
        OSRegisterResetFunction(&GXResetFuncInfo);
        resetFuncRegistered = 1;
    }

    __GXPEInit();
    EnableWriteGatherPipe();

    gx->genMode = 0;
    SET_REG_FIELD(0, gx->genMode, 8, 24, 0);
    gx->bpMask = 255;
    SET_REG_FIELD(0, gx->bpMask, 8, 24, 0x0F);
    gx->lpSize = 0;
    SET_REG_FIELD(0, gx->lpSize, 8, 24, 0x22);

    for (i = 0; i < GX_MAX_TEVSTAGE; ++i) {
        gx->tevc[i] = 0;
        gx->teva[i] = 0;
        gx->tref[i / 2] = 0;
        gx->texmapId[i] = GX_TEXMAP_NULL;
        SET_REG_FIELD(0, gx->tevc[i], 8, 24, 0xC0 + i * 2);
        SET_REG_FIELD(0, gx->teva[i], 8, 24, 0xC1 + i * 2);
        SET_REG_FIELD(0, gx->tevKsel[i / 2], 8, 24, 0xF6 + i / 2);
        SET_REG_FIELD(0, gx->tref[i / 2], 8, 24, 0x28 + i / 2);
    }

    gx->iref = 0;
    SET_REG_FIELD(0, gx->iref, 8, 24, 0x27);

    for (i = 0; i < GX_MAX_TEXCOORD; ++i) {
        gx->suTs0[i] = 0;
        gx->suTs1[i] = 0;
        SET_REG_FIELD(0, gx->suTs0[i], 8, 24, 0x30 + i * 2);
        SET_REG_FIELD(0, gx->suTs1[i], 8, 24, 0x31 + i * 2);
    }

    SET_REG_FIELD(0, gx->suScis0, 8, 24, 0x20);
    SET_REG_FIELD(0, gx->suScis1, 8, 24, 0x21);
    SET_REG_FIELD(0, gx->cmode0, 8, 24, 0x41);
    SET_REG_FIELD(0, gx->cmode1, 8, 24, 0x42);
    SET_REG_FIELD(0, gx->zmode, 8, 24, 0x40);
    SET_REG_FIELD(0, gx->peCtrl, 8, 24, 0x43);
    SET_REG_FIELD(0, gx->cpTex, 2, 7, 0);

    gx->zScale = 16777216.0f;
    gx->zOffset = 0.0f;
    gx->dirtyState = 0;
    gx->dirtyVAT = FALSE;

    freqBase = __OSBusClock / 500;
    __GXFlushTextureState();
    reg = (freqBase >> 11) | 0x400 | 0x69000000;
    GX_WRITE_RAS_REG(reg);

    __GXFlushTextureState();
    reg = (freqBase / 0x1080) | 0x200 | 0x46000000;
    GX_WRITE_RAS_REG(reg);

    __GXInitRevisionBits();

    for (i = 0; i < GX_MAX_TEXMAP; i++) {
        GXInitTexCacheRegion(&gx->TexRegions0[i], GX_FALSE, GXTexRegionAddrTable[i],
                             GX_TEXCACHE_32K, GXTexRegionAddrTable[i + 8], GX_TEXCACHE_32K);
        GXInitTexCacheRegion(&gx->TexRegions1[i], GX_FALSE, GXTexRegionAddrTable[i + 16],
                             GX_TEXCACHE_32K, GXTexRegionAddrTable[i + 24], GX_TEXCACHE_32K);
        GXInitTexCacheRegion(&gx->TexRegions2[i], GX_TRUE, GXTexRegionAddrTable[i + 32],
                             GX_TEXCACHE_32K, GXTexRegionAddrTable[i + 40], GX_TEXCACHE_32K);
    }

    for (i = 0; i < 16; i++) {
        GXInitTlutRegion(&gx->TlutRegions[i], 0xC0000 + 0x2000 * i, GX_TLUT_256);
    }

    for (i = 0; i < 4; i++) {
        GXInitTlutRegion(&gx->TlutRegions[i + 16], 0xE0000 + 0x8000 * i, GX_TLUT_1K);
    }

    {
        u32 cpReg = 0;

        GX_SET_CP_REG(3, cpReg);

        SET_REG_FIELD(0, gx->perfSel, 4, 4, 0);
        GX_WRITE_U8(0x8);
        GX_WRITE_U8(0x20);
        GX_WRITE_U32(gx->perfSel);

        GX_WRITE_XF_REG(6, 0);
        GX_WRITE_RAS_REG(0x23000000);
        GX_WRITE_RAS_REG(0x24000000);
        GX_WRITE_RAS_REG(0x67000000);
    }

    __GXSetIndirectMask(0);
    __GXSetTmemConfig(2);
    __GXInitGX();

    return &FifoObj;
}

void __GXInitGX(void)
{
    GXRenderModeObj* rmode;
    GXTexObj texObj;
    f32 identityMtx[3][4];
    GXColor clear = {64, 64, 64, 255};
    GXColor black = {0, 0, 0, 0};
    GXColor white = {255, 255, 255, 255};
    u32 i;

    switch (VIGetTvFormat()) {
        case VI_NTSC:
            rmode = &GXNtsc480IntDf;
            break;
        case VI_PAL:
            rmode = &GXPal528IntDf;
            break;
        case VI_EURGB60:
            rmode = &GXEurgb60Hz480IntDf;
            break;
        case VI_MPAL:
            rmode = &GXMpal480IntDf;
            break;
        default:
            rmode = &GXNtsc480IntDf;
            break;
    }

    GXSetCopyClear(clear, 0xFFFFFF);
    GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
    GXSetTexCoordGen(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX1, GX_IDENTITY);
    GXSetTexCoordGen(GX_TEXCOORD2, GX_TG_MTX2x4, GX_TG_TEX2, GX_IDENTITY);
    GXSetTexCoordGen(GX_TEXCOORD3, GX_TG_MTX2x4, GX_TG_TEX3, GX_IDENTITY);
    GXSetTexCoordGen(GX_TEXCOORD4, GX_TG_MTX2x4, GX_TG_TEX4, GX_IDENTITY);
    GXSetTexCoordGen(GX_TEXCOORD5, GX_TG_MTX2x4, GX_TG_TEX5, GX_IDENTITY);
    GXSetTexCoordGen(GX_TEXCOORD6, GX_TG_MTX2x4, GX_TG_TEX6, GX_IDENTITY);
    GXSetTexCoordGen(GX_TEXCOORD7, GX_TG_MTX2x4, GX_TG_TEX7, GX_IDENTITY);
    GXSetNumTexGens(1);
    GXClearVtxDesc();
    GXInvalidateVtxCache();

    for (i = GX_VA_POS; i <= GX_LIGHT_ARRAY; i++) {
        GXSetArray(i, gx, 0);
    }

    for (i = GX_VTXFMT0; i < GX_MAX_VTXFMT; i++) {
        GXSetVtxAttrFmtv(i, GXDefaultVATList);
    }

    GXSetLineWidth(6, GX_TO_ZERO);
    GXSetPointSize(6, GX_TO_ZERO);
    GXEnableTexOffsets(GX_TEXCOORD0, GX_FALSE, GX_FALSE);
    GXEnableTexOffsets(GX_TEXCOORD1, GX_FALSE, GX_FALSE);
    GXEnableTexOffsets(GX_TEXCOORD2, GX_FALSE, GX_FALSE);
    GXEnableTexOffsets(GX_TEXCOORD3, GX_FALSE, GX_FALSE);
    GXEnableTexOffsets(GX_TEXCOORD4, GX_FALSE, GX_FALSE);
    GXEnableTexOffsets(GX_TEXCOORD5, GX_FALSE, GX_FALSE);
    GXEnableTexOffsets(GX_TEXCOORD6, GX_FALSE, GX_FALSE);
    GXEnableTexOffsets(GX_TEXCOORD7, GX_FALSE, GX_FALSE);

    identityMtx[0][0] = 1.0f;
    identityMtx[0][1] = 0.0f;
    identityMtx[0][2] = 0.0f;
    identityMtx[0][3] = 0.0f;
    identityMtx[1][0] = 0.0f;
    identityMtx[1][1] = 1.0f;
    identityMtx[1][2] = 0.0f;
    identityMtx[1][3] = 0.0f;
    identityMtx[2][0] = 0.0f;
    identityMtx[2][1] = 0.0f;
    identityMtx[2][2] = 1.0f;
    identityMtx[2][3] = 0.0f;

    GXLoadPosMtxImm(identityMtx, GX_PNMTX0);
    GXLoadNrmMtxImm(identityMtx, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
    GXLoadTexMtxImm(identityMtx, GX_IDENTITY, GX_MTX3x4);
    GXLoadTexMtxImm(identityMtx, GX_PTIDENTITY, GX_MTX3x4);
    GXSetViewport(0.0f, 0.0f, rmode->fbWidth, rmode->xfbHeight, 0.0f, 1.0f);
    GXSetProjectionv(GXDefaultProjData);
    GXSetCoPlanar(GX_DISABLE);
    GXSetCullMode(GX_CULL_BACK);
    GXSetClipMode(GX_CLIP_ENABLE);
    GXSetScissor(0, 0, rmode->fbWidth, rmode->efbHeight);
    GXSetScissorBoxOffset(0, 0);
    GXSetNumChans(0);
    GXSetChanCtrl(GX_COLOR0A0, GX_DISABLE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE,
                  GX_AF_NONE);
    GXSetChanAmbColor(GX_COLOR0A0, black);
    GXSetChanMatColor(GX_COLOR0A0, white);
    GXSetChanCtrl(GX_COLOR1A1, GX_DISABLE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE,
                  GX_AF_NONE);
    GXSetChanAmbColor(GX_COLOR1A1, black);
    GXSetChanMatColor(GX_COLOR1A1, white);
    GXInvalidateTexAll();
    GXSetTexRegionCallback((GXTexRegionCallback)__GXDefaultTexRegionCallback);
    GXSetTlutRegionCallback(__GXDefaultTlutRegionCallback);

    GXInitTexObj(&texObj, DefaultTexData, 4, 4, GX_TF_IA8, GX_CLAMP, GX_CLAMP, GX_FALSE);
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    GXLoadTexObj(&texObj, GX_TEXMAP1);
    GXLoadTexObj(&texObj, GX_TEXMAP2);
    GXLoadTexObj(&texObj, GX_TEXMAP3);
    GXLoadTexObj(&texObj, GX_TEXMAP4);
    GXLoadTexObj(&texObj, GX_TEXMAP5);
    GXLoadTexObj(&texObj, GX_TEXMAP6);
    GXLoadTexObj(&texObj, GX_TEXMAP7);

    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR0A0);
    GXSetTevOrder(GX_TEVSTAGE2, GX_TEXCOORD2, GX_TEXMAP2, GX_COLOR0A0);
    GXSetTevOrder(GX_TEVSTAGE3, GX_TEXCOORD3, GX_TEXMAP3, GX_COLOR0A0);
    GXSetTevOrder(GX_TEVSTAGE4, GX_TEXCOORD4, GX_TEXMAP4, GX_COLOR0A0);
    GXSetTevOrder(GX_TEVSTAGE5, GX_TEXCOORD5, GX_TEXMAP5, GX_COLOR0A0);
    GXSetTevOrder(GX_TEVSTAGE6, GX_TEXCOORD6, GX_TEXMAP6, GX_COLOR0A0);
    GXSetTevOrder(GX_TEVSTAGE7, GX_TEXCOORD7, GX_TEXMAP7, GX_COLOR0A0);
    GXSetTevOrder(GX_TEVSTAGE8, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
    GXSetTevOrder(GX_TEVSTAGE9, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
    GXSetTevOrder(GX_TEVSTAGE10, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
    GXSetTevOrder(GX_TEVSTAGE11, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
    GXSetTevOrder(GX_TEVSTAGE12, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
    GXSetTevOrder(GX_TEVSTAGE13, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
    GXSetTevOrder(GX_TEVSTAGE14, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
    GXSetTevOrder(GX_TEVSTAGE15, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);

    GXSetNumTevStages(1);
    GXSetTevOp(GX_TEVSTAGE0, GX_REPLACE);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetZTexture(GX_ZT_DISABLE, GX_TF_Z8, 0);

    for (i = GX_TEVSTAGE0; i < GX_MAX_TEVSTAGE; i++) {
        GXSetTevKColorSel(i, GX_TEV_KCSEL_1_4);
        GXSetTevKAlphaSel(i, GX_TEV_KASEL_1);
        GXSetTevSwapMode(i, GX_TEV_SWAP0, GX_TEV_SWAP0);
    }

    GXSetTevSwapModeTable(GX_TEV_SWAP0, GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE, GX_CH_ALPHA);
    GXSetTevSwapModeTable(GX_TEV_SWAP1, GX_CH_RED, GX_CH_RED, GX_CH_RED, GX_CH_ALPHA);
    GXSetTevSwapModeTable(GX_TEV_SWAP2, GX_CH_GREEN, GX_CH_GREEN, GX_CH_GREEN, GX_CH_ALPHA);
    GXSetTevSwapModeTable(GX_TEV_SWAP3, GX_CH_BLUE, GX_CH_BLUE, GX_CH_BLUE, GX_CH_ALPHA);

    for (i = GX_TEVSTAGE0; i < GX_MAX_TEVSTAGE; i++) {
        GXSetTevDirect(i);
    }

    GXSetNumIndStages(0);
    GXSetIndTexCoordScale(GX_INDTEXSTAGE0, GX_ITS_1, GX_ITS_1);
    GXSetIndTexCoordScale(GX_INDTEXSTAGE1, GX_ITS_1, GX_ITS_1);
    GXSetIndTexCoordScale(GX_INDTEXSTAGE2, GX_ITS_1, GX_ITS_1);
    GXSetIndTexCoordScale(GX_INDTEXSTAGE3, GX_ITS_1, GX_ITS_1);

    GXSetFog(GX_FOG_NONE, 0.0f, 1.0f, 0.1f, 1.0f, black);
    GXSetFogRangeAdj(GX_DISABLE, 0, NULL);
    GXSetBlendMode(GX_BM_NONE, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
    GXSetColorUpdate(GX_ENABLE);
    GXSetAlphaUpdate(GX_ENABLE);
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
    GXSetZCompLoc(GX_TRUE);
    GXSetDither(GX_ENABLE);
    GXSetDstAlpha(GX_DISABLE, 0);
    GXSetPixelFmt(GX_PF_RGB8_Z24, GX_ZC_LINEAR);
    GXSetFieldMask(GX_ENABLE, GX_ENABLE);
    GXSetFieldMode(rmode->field_rendering,
                   rmode->viHeight == 2 * rmode->xfbHeight ? GX_ENABLE : GX_DISABLE);

    GXSetDispCopySrc(0, 0, rmode->fbWidth, rmode->efbHeight);
    GXSetDispCopyDst(rmode->fbWidth, rmode->efbHeight);
    GXSetDispCopyYScale((f32)rmode->xfbHeight / (f32)rmode->efbHeight);
    GXSetCopyClamp(GX_CLAMP_TOP | GX_CLAMP_BOTTOM);
    GXSetCopyFilter(rmode->aa, rmode->sample_pattern, GX_TRUE, rmode->vfilter);
    GXSetDispCopyGamma(GX_GM_1_0);
    GXSetDispCopyFrame2Field(GX_COPY_PROGRESSIVE);
    GXClearBoundingBox();

    GXPokeColorUpdate(GX_TRUE);
    GXPokeAlphaUpdate(GX_TRUE);
    GXPokeDither(GX_FALSE);
    GXPokeBlendMode(GX_BM_NONE, GX_BL_ZERO, GX_BL_ONE, GX_LO_SET);
    GXPokeAlphaMode(GX_ALWAYS, 0);
    GXPokeAlphaRead(GX_READ_FF);
    GXPokeDstAlpha(GX_DISABLE, 0);
    GXPokeZMode(GX_TRUE, GX_ALWAYS, GX_TRUE);

    GXSetGPMetric(GX_PERF0_NONE, GX_PERF1_NONE);
    GXClearGPMetric();
}
