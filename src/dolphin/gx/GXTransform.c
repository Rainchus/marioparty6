#include <dolphin/gx.h>
#include <dolphin/mtx.h>

#include <dolphin/gx/GXPriv.h>

static inline void Copy6Floats(const f32 src[6], f32 dest[6])
{
    dest[0] = src[0];
    dest[1] = src[1];
    dest[2] = src[2];
    dest[3] = src[3];
    dest[4] = src[4];
    dest[5] = src[5];
}

static inline void WriteProjection(const f32 proj[6])
{
    GX_WRITE_U8(GX_LOAD_XF_REG);
    GX_WRITE_U32(0x61020);
    GX_WRITE_F32(proj[0]);
    GX_WRITE_F32(proj[1]);
    GX_WRITE_F32(proj[2]);
    GX_WRITE_F32(proj[3]);
    GX_WRITE_F32(proj[4]);
    GX_WRITE_F32(proj[5]);
    GX_WRITE_U32(gx->projType);
}

static inline void WriteMtx4x3(const f32 mtx[3][4])
{
    GX_WRITE_F32(mtx[0][0]);
    GX_WRITE_F32(mtx[0][1]);
    GX_WRITE_F32(mtx[0][2]);
    GX_WRITE_F32(mtx[0][3]);
    GX_WRITE_F32(mtx[1][0]);
    GX_WRITE_F32(mtx[1][1]);
    GX_WRITE_F32(mtx[1][2]);
    GX_WRITE_F32(mtx[1][3]);
    GX_WRITE_F32(mtx[2][0]);
    GX_WRITE_F32(mtx[2][1]);
    GX_WRITE_F32(mtx[2][2]);
    GX_WRITE_F32(mtx[2][3]);
}

static inline void WriteNrmMtx3x3(const f32 mtx[3][4])
{
    GX_WRITE_F32(mtx[0][0]);
    GX_WRITE_F32(mtx[0][1]);
    GX_WRITE_F32(mtx[0][2]);
    GX_WRITE_F32(mtx[1][0]);
    GX_WRITE_F32(mtx[1][1]);
    GX_WRITE_F32(mtx[1][2]);
    GX_WRITE_F32(mtx[2][0]);
    GX_WRITE_F32(mtx[2][1]);
    GX_WRITE_F32(mtx[2][2]);
}

static inline void WriteMtx4x2(const f32 mtx[2][4])
{
    GX_WRITE_F32(mtx[0][0]);
    GX_WRITE_F32(mtx[0][1]);
    GX_WRITE_F32(mtx[0][2]);
    GX_WRITE_F32(mtx[0][3]);
    GX_WRITE_F32(mtx[1][0]);
    GX_WRITE_F32(mtx[1][1]);
    GX_WRITE_F32(mtx[1][2]);
    GX_WRITE_F32(mtx[1][3]);
}

void GXProject(f32 x, f32 y, f32 z, const Mtx mtx, const f32* pm, const f32* vp,
               f32* sx, f32* sy, f32* sz)
{
    Vec peye;
    f32 xc;
    f32 yc;
    f32 zc;
    f32 wc;

    peye.x = mtx[0][3] + ((mtx[0][2] * z) + ((mtx[0][0] * x) + (mtx[0][1] * y)));
    peye.y = mtx[1][3] + ((mtx[1][2] * z) + ((mtx[1][0] * x) + (mtx[1][1] * y)));
    peye.z = mtx[2][3] + ((mtx[2][2] * z) + ((mtx[2][0] * x) + (mtx[2][1] * y)));
    if (pm[0] == 0.0f) {
        xc = (peye.x * pm[1]) + (peye.z * pm[2]);
        yc = (peye.y * pm[3]) + (peye.z * pm[4]);
        zc = pm[6] + (peye.z * pm[5]);
        wc = 1.0f / -peye.z;
    } else {
        xc = pm[2] + (peye.x * pm[1]);
        yc = pm[4] + (peye.y * pm[3]);
        zc = pm[6] + (peye.z * pm[5]);
        wc = 1.0f;
    }
    *sx = (vp[2] / 2.0f) + (vp[0] + (wc * (xc * vp[2] / 2.0f)));
    *sy = (vp[3] / 2.0f) + (vp[1] + (wc * (-yc * vp[3] / 2.0f)));
    *sz = vp[5] + (wc * (zc * (vp[5] - vp[4])));
}

void GXSetProjection(const Mtx44 mtx, GXProjectionType type)
{
    gx->projType = type;
    gx->projMtx[0] = mtx[0][0];
    gx->projMtx[2] = mtx[1][1];
    gx->projMtx[4] = mtx[2][2];
    gx->projMtx[5] = mtx[2][3];
    if (type == GX_ORTHOGRAPHIC) {
        gx->projMtx[1] = mtx[0][3];
        gx->projMtx[3] = mtx[1][3];
    } else {
        gx->projMtx[1] = mtx[0][2];
        gx->projMtx[3] = mtx[1][2];
    }
    WriteProjection(gx->projMtx);
    gx->bpSentNot = 1;
}

void GXSetProjectionv(const f32* ptr)
{
    gx->projType = ptr[0] == 0.0f ? GX_PERSPECTIVE : GX_ORTHOGRAPHIC;
    Copy6Floats(&ptr[1], gx->projMtx);
    WriteProjection(gx->projMtx);
    gx->bpSentNot = 1;
}

void GXGetProjectionv(f32* ptr)
{
    ptr[0] = (u32)gx->projType != GX_PERSPECTIVE ? 1.0f : 0.0f;
    Copy6Floats(gx->projMtx, &ptr[1]);
}

void GXLoadPosMtxImm(const Mtx mtx, u32 id)
{
    u32 addr = id * 4;
    GX_WRITE_U8(GX_LOAD_XF_REG);
    GX_WRITE_U32(addr | 0xB0000);
    WriteMtx4x3(mtx);
}

void GXLoadNrmMtxImm(const Mtx mtx, u32 id)
{
    u32 addr = id * 3 + 0x400;
    GX_WRITE_U8(GX_LOAD_XF_REG);
    GX_WRITE_U32(addr | 0x80000);
    WriteNrmMtx3x3(mtx);
}

void GXSetCurrentMtx(u32 id)
{
    SET_REG_FIELD(712, gx->matIdxA, 6, 0, id);
    __GXSetMatrixIndex(GX_VA_PNMTXIDX);
}

void GXLoadTexMtxImm(const f32 mtx[][4], u32 id, GXTexMtxType type)
{
    u32 addr;
    u32 count;

    if (id >= GX_PTTEXMTX0) {
        addr = (id - GX_PTTEXMTX0) * 4 + 0x500;
    } else {
        addr = id * 4;
    }
    count = type == GX_MTX2x4 ? 8 : 12;
    GX_WRITE_U8(GX_LOAD_XF_REG);
    GX_WRITE_U32(addr | ((count - 1) << 16));
    if (type == GX_MTX3x4) {
        WriteMtx4x3(mtx);
    } else {
        WriteMtx4x2(mtx);
    }
}

void __GXSetViewport(void)
{
    f32 sx;
    f32 sy;
    f32 sz;
    f32 ox;
    f32 oy;
    f32 oz;
    f32 zmin;
    f32 zmax;

    sx = gx->vpWd / 2.0f;
    sy = -gx->vpHt / 2.0f;
    ox = 342.0f + (gx->vpLeft + (gx->vpWd / 2.0f));
    oy = 342.0f + (gx->vpTop + (gx->vpHt / 2.0f));
    zmin = gx->vpNearz * gx->zScale;
    zmax = gx->vpFarz * gx->zScale;
    sz = zmax - zmin;
    oz = zmax + gx->zOffset;

    GX_WRITE_U8(GX_LOAD_XF_REG);
    GX_WRITE_U32(0x5101A);
    GX_WRITE_F32(sx);
    GX_WRITE_F32(sy);
    GX_WRITE_F32(sz);
    GX_WRITE_F32(ox);
    GX_WRITE_F32(oy);
    GX_WRITE_F32(oz);
}

void GXSetViewportJitter(f32 left, f32 top, f32 wd, f32 ht, f32 nearz, f32 farz,
                         u32 field)
{
    if (field == 0) {
        top -= 0.5f;
    }
    gx->vpLeft = left;
    gx->vpTop = top;
    gx->vpWd = wd;
    gx->vpHt = ht;
    gx->vpNearz = nearz;
    gx->vpFarz = farz;
    __GXSetViewport();
    gx->bpSentNot = 1;
}

void GXSetViewport(f32 left, f32 top, f32 wd, f32 ht, f32 nearz, f32 farz)
{
    GXSetViewportJitter(left, top, wd, ht, nearz, farz, 1);
}

void GXGetViewportv(f32* vp)
{
    Copy6Floats(&gx->vpLeft, vp);
}

void GXSetScissor(u32 left, u32 top, u32 wd, u32 ht)
{
    u32 tp = top + 342;
    u32 lf = left + 342;
    u32 bm = tp + ht - 1;
    u32 rt = lf + wd - 1;

    SET_REG_FIELD(1059, gx->suScis0, 11, 0, tp);
    SET_REG_FIELD(1060, gx->suScis0, 11, 12, lf);
    SET_REG_FIELD(1062, gx->suScis1, 11, 0, bm);
    SET_REG_FIELD(1063, gx->suScis1, 11, 12, rt);
    GX_WRITE_RAS_REG(gx->suScis0);
    GX_WRITE_RAS_REG(gx->suScis1);
    gx->bpSentNot = 0;
}

void GXSetScissorBoxOffset(s32 x_off, s32 y_off)
{
    u32 reg = 0;
    u32 hx = (u32)(x_off + 342) >> 1;
    u32 hy = (u32)(y_off + 342) >> 1;

    SET_REG_FIELD(1129, reg, 10, 0, hx);
    SET_REG_FIELD(1130, reg, 10, 10, hy);
    SET_REG_FIELD(1131, reg, 8, 24, 0x59);
    GX_WRITE_RAS_REG(reg);
    gx->bpSentNot = 0;
}

void GXSetClipMode(GXClipMode mode)
{
    GX_WRITE_XF_REG(5, mode);
    gx->bpSentNot = 1;
}

void __GXSetMatrixIndex(GXAttr matIdxAttr)
{
    if (matIdxAttr < GX_VA_TEX4MTXIDX) {
        GX_WRITE_SOME_REG4(8, 0x30, gx->matIdxA, -12);
        GX_WRITE_XF_REG(24, gx->matIdxA);
    } else {
        GX_WRITE_SOME_REG4(8, 0x40, gx->matIdxB, -12);
        GX_WRITE_XF_REG(25, gx->matIdxB);
    }
    gx->bpSentNot = 1;
}
