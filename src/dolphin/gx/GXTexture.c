#include <dolphin/gx.h>
#include <dolphin/os.h>

#include <dolphin/gx/GXPriv.h>

// GXTexObj internal data
typedef struct __GXTexObjInt_struct {
    u32 mode0;
    u32 mode1;
    u32 image0;
    u32 image3;
    void *userData;
    GXTexFmt fmt;
    u32 tlutName;
    u16 loadCnt;
    u8 loadFmt;
    u8 flags;
} __GXTexObjInt;

// GXTexRegion internal data
typedef struct __GXTexRegionInt_struct {
    u32 image1;
    u32 image2;
    u16 sizeEven;
    u16 sizeOdd;
    u8 is32bMipmap;
    u8 isCached;
} __GXTexRegionInt;

// GXTlutObj internal data
typedef struct __GXTlutObjInt_struct {
    u32 tlut;
    u32 loadTlut0;
    u16 numEntries;
} __GXTlutObjInt;

// GXTlutRegion internal data
typedef struct __GXTlutRegionInt_struct {
    u32 loadTlut1;
    __GXTlutObjInt tlutObj;
} __GXTlutRegionInt;

u8 GXTexMode0Ids[8] = { 0x80, 0x81, 0x82, 0x83, 0xA0, 0xA1, 0xA2, 0xA3 };
u8 GXTexMode1Ids[8] = { 0x84, 0x85, 0x86, 0x87, 0xA4, 0xA5, 0xA6, 0xA7 };
u8 GXTexImage0Ids[8] = { 0x88, 0x89, 0x8A, 0x8B, 0xA8, 0xA9, 0xAA, 0xAB };
u8 GXTexImage1Ids[8] = { 0x8C, 0x8D, 0x8E, 0x8F, 0xAC, 0xAD, 0xAE, 0xAF };
u8 GXTexImage2Ids[8] = { 0x90, 0x91, 0x92, 0x93, 0xB0, 0xB1, 0xB2, 0xB3 };
u8 GXTexImage3Ids[8] = { 0x94, 0x95, 0x96, 0x97, 0xB4, 0xB5, 0xB6, 0xB7 };
u8 GXTexTlutIds[8] = { 0x98, 0x99, 0x9A, 0x9B, 0xB8, 0xB9, 0xBA, 0xBB };
static u8 GX2HWFiltConv[6] = { 0x00, 0x04, 0x01, 0x05, 0x02, 0x06 };

static inline void __GXGetTexTileShift(GXTexFmt fmt, u32 *rowTileS, u32 *colTileS)
{
    switch (fmt) {
        case GX_TF_I4:
        case 0x8:
        case GX_TF_CMPR:
        case GX_CTF_R4:
        case GX_CTF_Z4:
            *rowTileS = 3;
            *colTileS = 3;
            break;
        case GX_TF_I8:
        case GX_TF_IA4:
        case 0x9:
        case GX_TF_Z8:
        case GX_CTF_RA4:
        case GX_TF_A8:
        case GX_CTF_R8:
        case GX_CTF_G8:
        case GX_CTF_B8:
        case GX_CTF_Z8M:
        case GX_CTF_Z8L:
            *rowTileS = 3;
            *colTileS = 2;
            break;
        case GX_TF_IA8:
        case GX_TF_RGB565:
        case GX_TF_RGB5A3:
        case GX_TF_RGBA8:
        case 0xA:
        case GX_TF_Z16:
        case GX_TF_Z24X8:
        case GX_CTF_RA8:
        case GX_CTF_RG8:
        case GX_CTF_GB8:
        case GX_CTF_Z16L:
            *rowTileS = 2;
            *colTileS = 2;
            break;
        default:
            *rowTileS = *colTileS = 0;
            ASSERTMSGLINEV(0x1BC, 0, "%s: invalid texture format", "GX");
            break;
    }
}

u32 GXGetTexBufferSize(u16 width, u16 height, u32 format, u8 mipmap, u8 max_lod)
{
    u32 tileShiftX;
    u32 tileShiftY;
    u32 tileBytes;
    u32 bufferSize;
    u32 nx;
    u32 ny;
    u32 level;

    ASSERTMSGLINEV(0x1CC, width <= 1024, "%s: width too large", "GXGetTexBufferSize");
    ASSERTMSGLINEV(0x1CD, height <= 1024, "%s: height too large", "GXGetTexBufferSize");

    __GXGetTexTileShift(format, &tileShiftX, &tileShiftY);
    if (format == GX_TF_RGBA8 || format == GX_TF_Z24X8) {
        tileBytes = 64;
    }
    else {
        tileBytes = 32;
    }
    if (mipmap == 1) {
        nx = 1 << (31 - __cntlzw(width));
        ASSERTMSGLINEV(0x1DF, width == nx, "%s: width must be a power of 2", "GXGetTexBufferSize");
        ny = 1 << (31 - __cntlzw(height));
        ASSERTMSGLINEV(0x1E2, height == ny, "%s: height must be a power of 2", "GXGetTexBufferSize");

        bufferSize = 0;
        for (level = 0; level < max_lod; level++) {
            nx = (width + (1 << tileShiftX) - 1) >> tileShiftX;
            ny = (height + (1 << tileShiftY) - 1) >> tileShiftY;
            bufferSize += tileBytes * (nx * ny);
            if (width == 1 && height == 1) {
                break;
            }
            width = (width > 1) ? width >> 1 : 1;
            height = (height > 1) ? height >> 1 : 1;
        }
    }
    else {
        nx = (width + (1 << tileShiftX) - 1) >> tileShiftX;
        ny = (height + (1 << tileShiftY) - 1) >> tileShiftY;
        bufferSize = nx * ny * tileBytes;
    }
    return bufferSize;
}

void __GetImageTileCount(GXTexFmt fmt, u16 wd, u16 ht, u32 *rowTiles, u32 *colTiles, u32 *cmpTiles)
{
    u32 texRowShift;
    u32 texColShift;

    __GXGetTexTileShift(fmt, &texRowShift, &texColShift);
    if (wd == 0) {
        wd = 1;
    }
    if (ht == 0) {
        ht = 1;
    }
    *rowTiles = (wd + (1 << texRowShift) - 1) >> texRowShift;
    *colTiles = (ht + (1 << texColShift) - 1) >> texColShift;
    *cmpTiles = (fmt == GX_TF_RGBA8 || fmt == GX_TF_Z24X8) ? 2 : 1;
}

#define SOME_SET_REG_MACRO(reg, size, shift, val)                                                                                                   \
    do {                                                                                                                                             \
        (reg) = (u32)__rlwimi((u32)(reg), (val), (shift), (32 - (shift) - (size)), (31 - (shift)));                                                 \
    } while (0);

void GXInitTexObj(GXTexObj *obj, void *image_ptr, u16 width, u16 height, GXTexFmt format, GXTexWrapMode wrap_s, GXTexWrapMode wrap_t, u8 mipmap)
{
    u32 imageBase;
    u32 maxLOD;
    u16 rowT;
    u16 colT;
    u32 rowC;
    u32 colC;
    __GXTexObjInt *t = (__GXTexObjInt *)obj;

    ASSERTMSGLINE(0x235, obj, "Texture Object Pointer is null");
    CHECK_GXBEGIN(0x237, "GXInitTexObj");
    ASSERTMSGLINEV(0x238, width <= 1024, "%s: width too large", "GXInitTexObj");
    ASSERTMSGLINEV(0x239, height <= 1024, "%s: height too large", "GXInitTexObj");
    ASSERTMSGLINEV(0x23B, !(format & 0x20), "%s: invalid texture format", "GXInitTexObj");
    memset(t, 0, 0x20);
    SET_REG_FIELD(0x258, t->mode0, 2, 0, wrap_s);
    SET_REG_FIELD(0x259, t->mode0, 2, 2, wrap_t);
    SET_REG_FIELD(0x25A, t->mode0, 1, 4, 1);
    if (mipmap != 0) {
        u8 lmax;
        t->flags |= 1;

        if (format == 8 || format == 9 || format == 10) {
            SOME_SET_REG_MACRO(t->mode0, 3, 5, 5);
        }
        else {
            SOME_SET_REG_MACRO(t->mode0, 3, 5, 6);
        }

        if (width > height) {
            maxLOD = 31 - __cntlzw(width);
        }
        else {
            maxLOD = 31 - __cntlzw(height);
        }
        lmax = 16.0f * maxLOD;
        SET_REG_FIELD(0x278, t->mode1, 8, 8, lmax);
    }
    else {
        SOME_SET_REG_MACRO(t->mode0, 3, 5, 4);
    }
    t->fmt = format;
    SET_REG_FIELD(0x286, t->image0, 10, 0, width - 1);
    SET_REG_FIELD(0x287, t->image0, 10, 10, height - 1);
    SET_REG_FIELD(0x288, t->image0, 4, 20, format & 0xF);
    ASSERTMSGLINEV(0x28E, ((u32)image_ptr & 0x1F) == 0, "%s: %s pointer not aligned to 32B", "GXInitTexObj", "image");
    imageBase = (u32)((u32)image_ptr >> 5) & 0x01FFFFFF;
    SET_REG_FIELD(0x290, t->image3, 21, 0, imageBase);
    switch (format & 0xF) {
        case 0:
        case 8:
            t->loadFmt = 1;
            rowT = 3;
            colT = 3;
            break;
        case 1:
        case 2:
        case 9:
            t->loadFmt = 2;
            rowT = 3;
            colT = 2;
            break;
        case 3:
        case 4:
        case 5:
        case 10:
            t->loadFmt = 2;
            rowT = 2;
            colT = 2;
            break;
        case 6:
            t->loadFmt = 3;
            rowT = 2;
            colT = 2;
            break;
        case 14:
            t->loadFmt = 0;
            rowT = 3;
            colT = 3;
            break;
        default:
            ASSERTMSGLINEV(0x2BB, 0, "%s: invalid texture format", "GXPreLoadEntireTexture");
            t->loadFmt = 2;
            rowT = 2;
            colT = 2;
            break;
    }
    rowC = (width + (1 << rowT) - 1) >> rowT;
    colC = (height + (1 << colT) - 1) >> colT;
    t->loadCnt = (rowC * colC) & 0x7FFF;
    t->flags |= 2;
}

void GXInitTexObjCI(
    GXTexObj *obj, void *image_ptr, u16 width, u16 height, GXCITexFmt format, GXTexWrapMode wrap_s, GXTexWrapMode wrap_t, u8 mipmap, u32 tlut_name)
{
    __GXTexObjInt *t = (__GXTexObjInt *)obj;

    ASSERTMSGLINE(0x2E1, obj, "Texture Object Pointer is null");
    CHECK_GXBEGIN(0x2E3, "GXInitTexObjCI");
    GXInitTexObj(obj, image_ptr, width, height, format, wrap_s, wrap_t, mipmap);
    t->flags &= 0xFFFFFFFD;
    t->tlutName = tlut_name;
}

void GXInitTexObjLOD(GXTexObj *obj, GXTexFilter min_filt, GXTexFilter mag_filt, f32 min_lod, f32 max_lod, f32 lod_bias, u8 bias_clamp, u8 do_edge_lod,
    GXAnisotropy max_aniso)
{
    u8 lbias;
    u8 lmin;
    u8 lmax;
    __GXTexObjInt *t = (__GXTexObjInt *)obj;

    ASSERTMSGLINE(0x308, obj, "Texture Object Pointer is null");
    CHECK_GXBEGIN(0x30A, "GXInitTexObjLOD");

    if (lod_bias < -4.0f) {
        lod_bias = -4.0f;
    }
    else if (lod_bias >= 4.0f) {
        lod_bias = 3.99f;
    }
    lbias = 32.0f * lod_bias;
    SET_REG_FIELD(0x314, t->mode0, 8, 9, lbias);
    ASSERTMSG1LINE(0x317, (u32)mag_filt <= 1, "%s: invalid mag_filt value", "GXInitTexObjLOD");
    SET_REG_FIELD(0x318, t->mode0, 1, 4, (mag_filt == GX_LINEAR) ? 1 : 0);
    ASSERTMSG1LINE(0x31B, (u32)min_filt <= 5, "%s: invalid min_filt value", "GXInitTexObjLOD");
    SET_REG_FIELD(0x31C, t->mode0, 3, 5, GX2HWFiltConv[min_filt]);
    SET_REG_FIELD(0x31E, t->mode0, 1, 8, do_edge_lod ? 0 : 1);
    SET_REG_FIELD(0x321, t->mode0, 1, 17, 0);
    SET_REG_FIELD(0x321, t->mode0, 1, 18, 0);
    SET_REG_FIELD(0x321, t->mode0, 2, 19, max_aniso);
    SET_REG_FIELD(0x322, t->mode0, 1, 21, bias_clamp);

    if (min_lod < 0.0f) {
        min_lod = 0.0f;
    }
    else if (min_lod > 10.0f) {
        min_lod = 10.0f;
    }
    lmin = 16.0f * min_lod;
    if (max_lod < 0.0f) {
        max_lod = 0.0f;
    }
    else if (max_lod > 10.0f) {
        max_lod = 10.0f;
    }
    lmax = 16.0f * max_lod;
    SET_REG_FIELD(0x330, t->mode1, 8, 0, lmin);
    SET_REG_FIELD(0x331, t->mode1, 8, 8, lmax);
}

GXTexFmt GXGetTexObjFmt(const GXTexObj *to)
{
    const __GXTexObjInt *t = (const __GXTexObjInt *)to;

    ASSERTMSGLINE(0x466, to, "Texture Object Pointer is null");
    return t->fmt;
}

GXBool GXGetTexObjMipMap(const GXTexObj *to)
{
    const __GXTexObjInt *t = (const __GXTexObjInt *)to;

    ASSERTMSGLINE(0x478, to, "Texture Object Pointer is null");
    return (t->flags & 1) == 1;
}

void GXLoadTexObjPreLoaded(GXTexObj *obj, GXTexRegion *region, GXTexMapID id)
{
    __GXTlutRegionInt *tlr;
    u32 m0;
    u32 m1;
    u32 img0;
    u32 img1;
    u32 img2;
    u32 img3;
    __GXTexObjInt *t = (__GXTexObjInt *)obj;
    __GXTexRegionInt *r = (__GXTexRegionInt *)region;

    ASSERTMSGLINE(0x4E9, obj, "Texture Object Pointer is null");
    ASSERTMSGLINE(0x4E9, region, "TexRegion Object Pointer is null");
    CHECK_GXBEGIN(0x4EB, "GXLoadTexObjPreLoaded");
    ASSERTMSGLINEV(0x4EC, id < 8, "%s: invalid texture map ID", "GXLoadTexObj");

    m0 = t->mode0;
    m1 = t->mode1;
    img0 = t->image0;
    img1 = r->image1;
    img2 = r->image2;
    img3 = t->image3;

    SET_REG_FIELD(0x4F7, t->mode0, 8, 24, GXTexMode0Ids[id]);
    SET_REG_FIELD(0x4F8, t->mode1, 8, 24, GXTexMode1Ids[id]);
    SET_REG_FIELD(0x4F9, t->image0, 8, 24, GXTexImage0Ids[id]);
    SET_REG_FIELD(0x4FA, r->image1, 8, 24, GXTexImage1Ids[id]);
    SET_REG_FIELD(0x4FB, r->image2, 8, 24, GXTexImage2Ids[id]);
    SET_REG_FIELD(0x4FC, t->image3, 8, 24, GXTexImage3Ids[id]);

    GX_WRITE_RAS_REG(t->mode0);
    GX_WRITE_RAS_REG(t->mode1);
    GX_WRITE_RAS_REG(t->image0);
    GX_WRITE_RAS_REG(r->image1);
    GX_WRITE_RAS_REG(r->image2);
    GX_WRITE_RAS_REG(t->image3);

    if (!(t->flags & 2)) {
        ASSERTMSGLINEV(0x507, gx->tlutRegionCallback, "%s: Tex/Tlut Region Callback not set", "GXLoadTexObj/PreLoaded");
        tlr = (__GXTlutRegionInt *)gx->tlutRegionCallback(t->tlutName);
        ASSERTMSGLINEV(0x509, tlr, "%s: Tex/Tlut Region Callback returns NULL", "GXLoadTexObj/PreLoaded");

        SET_REG_FIELD(0x50B, tlr->tlutObj.tlut, 8, 24, GXTexTlutIds[id]);
        GX_WRITE_RAS_REG(tlr->tlutObj.tlut);
    }
    gx->tImage0[id] = t->image0;
    gx->tMode0[id] = t->mode0;
    gx->dirtyState |= 1;
    gx->bpSentNot = 0;
}

void GXLoadTexObj(GXTexObj *obj, GXTexMapID id)
{
    GXTexRegion *r;

    CHECK_GXBEGIN(0x526, "GXLoadTexObj");
    ASSERTMSGLINEV(0x527, id < 8, "%s: invalid texture map ID", "GXLoadTexObj");
    ASSERTMSGLINEV(0x52C, gx->texRegionCallback, "%s: Tex/Tlut Region Callback not set", "GXLoadTexObj");
    r = gx->texRegionCallback(obj, id);
    ASSERTMSGLINEV(0x52E, r, "%s: Tex/Tlut Region Callback returns NULL", "GXLoadTexObj");
    GXLoadTexObjPreLoaded(obj, r, id);
}

void GXInitTlutObj(GXTlutObj *tlut_obj, void *lut, GXTlutFmt fmt, u16 n_entries)
{
    __GXTlutObjInt *t = (__GXTlutObjInt *)tlut_obj;

    ASSERTMSGLINE(0x546, tlut_obj, "TLut Object Pointer is null");
    CHECK_GXBEGIN(0x547, "GXInitTlutObj");
    ASSERTMSGLINEV(0x54A, n_entries <= 0x4000, "%s: number of entries exceeds maximum", "GXInitTlutObj");
    ASSERTMSGLINEV(0x54C, ((u32)lut & 0x1F) == 0, "%s: %s pointer not aligned to 32B", "GXInitTlutObj", "Tlut");
    t->tlut = 0;
    SET_REG_FIELD(0x54F, t->tlut, 2, 10, fmt);
    SET_REG_FIELD(0x550, t->loadTlut0, 21, 0, ((u32)lut & 0x3FFFFFFF) >> 5);
    SET_REG_FIELD(0x551, t->loadTlut0, 8, 24, 0x64);
    t->numEntries = n_entries;
}

void GXLoadTlut(GXTlutObj *tlut_obj, u32 tlut_name)
{
    __GXTlutRegionInt *r;
    u32 tlut_offset;
    __GXTlutObjInt *t = (__GXTlutObjInt *)tlut_obj;

    ASSERTMSGLINE(0x598, tlut_obj, "TLut Object Pointer is null");
    CHECK_GXBEGIN(0x59A, "GXLoadTlut");
    ASSERTMSGLINEV(0x59B, gx->tlutRegionCallback, "%s: Tex/Tlut Region Callback not set", "GXLoadTlut");
    r = (__GXTlutRegionInt *)gx->tlutRegionCallback(tlut_name);
    ASSERTMSGLINEV(0x59D, r, "%s: Tex/Tlut Region Callback returns NULL", "GXLoadTlut");
    __GXFlushTextureState();
    GX_WRITE_RAS_REG(t->loadTlut0);
    GX_WRITE_RAS_REG(r->loadTlut1);
    __GXFlushTextureState();
    tlut_offset = r->loadTlut1 & 0x3FF;
    SET_REG_FIELD(0x5AD, t->tlut, 10, 0, tlut_offset);
    r->tlutObj = *t;
}

void GXInitTexCacheRegion(GXTexRegion *region, u8 is_32b_mipmap, u32 tmem_even, GXTexCacheSize size_even, u32 tmem_odd, GXTexCacheSize size_odd)
{
    u32 WidthExp2;
    __GXTexRegionInt *t = (__GXTexRegionInt *)region;

    ASSERTMSGLINE(0x5CC, region, "TexRegion Object Pointer is null");
    CHECK_GXBEGIN(0x5CE, "GXInitTexCacheRegion");
    ASSERTMSGLINEV(0x5D0, (tmem_even & 0x1F) == 0, "%s: %s pointer not aligned to 32B", "GXInitTexCacheRegion", "tmem even");
    ASSERTMSGLINEV(0x5D2, (tmem_odd & 0x1F) == 0, "%s: %s pointer not aligned to 32B", "GXInitTexCacheRegion", "tmem odd");
    switch (size_even) {
        case GX_TEXCACHE_32K:
            WidthExp2 = 3;
            break;
        case GX_TEXCACHE_128K:
            WidthExp2 = 4;
            break;
        case GX_TEXCACHE_512K:
            WidthExp2 = 5;
            break;
        default:
            ASSERTMSGLINEV(0x5DA, 0, "%s: Invalid %s size", "GXInitTexCacheRegion", "tmem even");
            break;
    }
    t->image1 = 0;
    SET_REG_FIELD(0x5DF, t->image1, 15, 0, tmem_even >> 5);
    SET_REG_FIELD(0x5E0, t->image1, 3, 15, WidthExp2);
    SET_REG_FIELD(0x5E1, t->image1, 3, 18, WidthExp2);
    SET_REG_FIELD(0x5E2, t->image1, 1, 21, 0);

    switch (size_odd) {
        case GX_TEXCACHE_32K:
            WidthExp2 = 3;
            break;
        case GX_TEXCACHE_128K:
            WidthExp2 = 4;
            break;
        case GX_TEXCACHE_512K:
            WidthExp2 = 5;
            break;
        case GX_TEXCACHE_NONE:
            WidthExp2 = 0;
            break;
        default:
            ASSERTMSGLINEV(0x5EA, 0, "%s: Invalid %s size", "GXInitTexCacheRegion", "tmem odd");
            break;
    }
    t->image2 = 0;
    SET_REG_FIELD(0x5EF, t->image2, 15, 0, tmem_odd >> 5);
    SET_REG_FIELD(0x5F0, t->image2, 3, 15, WidthExp2);
    SET_REG_FIELD(0x5F1, t->image2, 3, 18, WidthExp2);
    t->is32bMipmap = is_32b_mipmap;
    t->isCached = 1;
}

void GXInitTlutRegion(GXTlutRegion *region, u32 tmem_addr, GXTlutSize tlut_size)
{
    __GXTlutRegionInt *t = (__GXTlutRegionInt *)region;

    ASSERTMSGLINE(0x674, region, "TLutRegion Object Pointer is null");
    CHECK_GXBEGIN(0x676, "GXInitTlutRegion");
    ASSERTMSGLINEV(0x677, (tmem_addr & 0x1FF) == 0, "%s: tmem pointer is not aligned to 512B", "GXInitTlutRegion");
    ASSERTMSGLINEV(0x678, tlut_size <= 0x400, "%s: tlut size exceeds 16K", "GXInitTlutRegion");
    t->loadTlut1 = 0;
    tmem_addr -= 0x80000;
    SET_REG_FIELD(0x67C, t->loadTlut1, 10, 0, tmem_addr >> 9);
    SET_REG_FIELD(0x67D, t->loadTlut1, 11, 10, tlut_size);
    SET_REG_FIELD(0x67E, t->loadTlut1, 8, 24, 0x65);
}

void GXInvalidateTexAll(void)
{
    u32 reg0;
    u32 reg1;

    CHECK_GXBEGIN(0x6DB, "GXInvalidateTexAll");
    reg0 = 0x66001000;
    reg1 = 0x66001100;
    __GXFlushTextureState();
    GX_WRITE_RAS_REG(reg0);
    GX_WRITE_RAS_REG(reg1);
    __GXFlushTextureState();
}

GXTexRegionCallback GXSetTexRegionCallback(GXTexRegionCallback f)
{
    GXTexRegionCallback oldcb = gx->texRegionCallback;

    gx->texRegionCallback = f;
    return oldcb;
}

GXTlutRegionCallback GXSetTlutRegionCallback(GXTlutRegionCallback f)
{
    GXTlutRegionCallback oldcb = gx->tlutRegionCallback;

    gx->tlutRegionCallback = f;
    return oldcb;
}

void GXSetTexCoordScaleManually(GXTexCoordID coord, u8 enable, u16 ss, u16 ts)
{
    CHECK_GXBEGIN(0x7C5, "GXSetTexCoordScaleManually");
    ASSERTMSGLINEV(0x7C7, coord < 8, "%s: bad texcoord specified", "GXSetTexCoordScaleManually");
    gx->tcsManEnab = (gx->tcsManEnab & ~(1 << coord)) | (enable << coord);
    if (enable != 0) {
        SET_REG_FIELD(0x7CD, gx->suTs0[coord], 16, 0, (u16)(ss - 1));
        SET_REG_FIELD(0x7CE, gx->suTs1[coord], 16, 0, (u16)(ts - 1));
        GX_WRITE_RAS_REG(gx->suTs0[coord]);
        GX_WRITE_RAS_REG(gx->suTs1[coord]);
        gx->bpSentNot = 0;
    }
}

static void __SetSURegs(u32 tmap, u32 tcoord)
{
    u32 w;
    u32 h;
    u8 s_bias;
    u8 t_bias;

    w = GET_REG_FIELD(gx->tImage0[tmap], 10, 0);
    h = GET_REG_FIELD(gx->tImage0[tmap], 10, 10);
    SET_REG_FIELD(0x829, gx->suTs0[tcoord], 16, 0, w);
    SET_REG_FIELD(0x82A, gx->suTs1[tcoord], 16, 0, h);
    s_bias = GET_REG_FIELD(gx->tMode0[tmap], 2, 0) == 1;
    t_bias = GET_REG_FIELD(gx->tMode0[tmap], 2, 2) == 1;
    SET_REG_FIELD(0x830, gx->suTs0[tcoord], 1, 16, s_bias);
    SET_REG_FIELD(0x831, gx->suTs1[tcoord], 1, 16, t_bias);
    GX_WRITE_RAS_REG(gx->suTs0[tcoord]);
    GX_WRITE_RAS_REG(gx->suTs1[tcoord]);
    gx->bpSentNot = 0;
}

void __GXSetSUTexRegs(void)
{
    u32 nStages;
    u32 nIndStages;
    u32 i;
    u32 map;
    u32 tmap;
    u32 coord;
    u32 *ptref;

    if (gx->tcsManEnab != 0xFF) {
        nStages = GET_REG_FIELD(gx->genMode, 4, 10) + 1;
        nIndStages = GET_REG_FIELD(gx->genMode, 3, 16);
        for (i = 0; i < nIndStages; i++) {
            switch (i) {
                case 0:
                    tmap = GET_REG_FIELD(gx->iref, 3, 0);
                    coord = GET_REG_FIELD(gx->iref, 3, 3);
                    break;
                case 1:
                    tmap = GET_REG_FIELD(gx->iref, 3, 6);
                    coord = GET_REG_FIELD(gx->iref, 3, 9);
                    break;
                case 2:
                    tmap = GET_REG_FIELD(gx->iref, 3, 12);
                    coord = GET_REG_FIELD(gx->iref, 3, 15);
                    break;
                case 3:
                    tmap = GET_REG_FIELD(gx->iref, 3, 18);
                    coord = GET_REG_FIELD(gx->iref, 3, 21);
                    break;
            }
            if (!(gx->tcsManEnab & (1 << coord))) {
                __SetSURegs(tmap, coord);
            }
        }
        i = 0;
        for (i = 0; i < nStages; i++) {
            ptref = &gx->tref[i / 2];
            map = gx->texmapId[i];
            tmap = map & 0xFFFFFEFF;
            if (i & 1) {
                coord = GET_REG_FIELD(*ptref, 3, 15);
            }
            else {
                coord = GET_REG_FIELD(*ptref, 3, 3);
            }
            if ((tmap != 0xFF) && !(gx->tcsManEnab & (1 << coord)) && (gx->tevTcEnab & (1 << i))) {
                __SetSURegs(tmap, coord);
            }
        }
    }
}

void __GXSetTmemConfig(u32 config)
{
    switch (config) {
        case 2:
            GX_WRITE_RAS_REG(0x8c0d8000);
            GX_WRITE_RAS_REG(0x900dc000);

            GX_WRITE_RAS_REG(0x8d0d8800);
            GX_WRITE_RAS_REG(0x910dc800);

            GX_WRITE_RAS_REG(0x8e0d9000);
            GX_WRITE_RAS_REG(0x920dd000);

            GX_WRITE_RAS_REG(0x8f0d9800);
            GX_WRITE_RAS_REG(0x930dd800);

            GX_WRITE_RAS_REG(0xac0da000);
            GX_WRITE_RAS_REG(0xb00dc400);

            GX_WRITE_RAS_REG(0xad0da800);
            GX_WRITE_RAS_REG(0xb10dcc00);

            GX_WRITE_RAS_REG(0xae0db000);
            GX_WRITE_RAS_REG(0xb20dd400);

            GX_WRITE_RAS_REG(0xaf0db800);
            GX_WRITE_RAS_REG(0xb30ddc00);

            break;
        case 1:
            GX_WRITE_RAS_REG(0x8c0d8000);
            GX_WRITE_RAS_REG(0x900dc000);

            GX_WRITE_RAS_REG(0x8d0d8800);
            GX_WRITE_RAS_REG(0x910dc800);

            GX_WRITE_RAS_REG(0x8e0d9000);
            GX_WRITE_RAS_REG(0x920dd000);

            GX_WRITE_RAS_REG(0x8f0d9800);
            GX_WRITE_RAS_REG(0x930dd800);

            GX_WRITE_RAS_REG(0xac0da000);
            GX_WRITE_RAS_REG(0xb00de000);

            GX_WRITE_RAS_REG(0xad0da800);
            GX_WRITE_RAS_REG(0xb10de800);

            GX_WRITE_RAS_REG(0xae0db000);
            GX_WRITE_RAS_REG(0xb20df000);

            GX_WRITE_RAS_REG(0xaf0db800);
            GX_WRITE_RAS_REG(0xb30df800);

            break;
        case 0:
        default:
            GX_WRITE_RAS_REG(0x8c0d8000);
            GX_WRITE_RAS_REG(0x900dc000);

            GX_WRITE_RAS_REG(0x8d0d8400);
            GX_WRITE_RAS_REG(0x910dc400);

            GX_WRITE_RAS_REG(0x8e0d8800);
            GX_WRITE_RAS_REG(0x920dc800);

            GX_WRITE_RAS_REG(0x8f0d8c00);
            GX_WRITE_RAS_REG(0x930dcc00);

            GX_WRITE_RAS_REG(0xac0d9000);
            GX_WRITE_RAS_REG(0xb00dd000);

            GX_WRITE_RAS_REG(0xad0d9400);
            GX_WRITE_RAS_REG(0xb10dd400);

            GX_WRITE_RAS_REG(0xae0d9800);
            GX_WRITE_RAS_REG(0xb20dd800);

            GX_WRITE_RAS_REG(0xaf0d9c00);
            GX_WRITE_RAS_REG(0xb30ddc00);

            break;
    }
}
