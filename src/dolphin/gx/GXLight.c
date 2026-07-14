#include <dolphin/gx.h>
#include <dolphin/os.h>

#include <dolphin/gx/GXPriv.h>

f32 cosf(f32 x);

extern inline f32 sqrtf(f32 x)
{
    const f64 half = 0.5;
    const f64 three = 3.0;
    volatile f32 result;

    if (x > 0.0f) {
        f64 guess = __frsqrte((f64)x);
        guess = half * guess * (three - guess * guess * x);
        guess = half * guess * (three - guess * guess * x);
        guess = half * guess * (three - guess * guess * x);
        result = (f32)(x * guess);
        return result;
    }
    return x;
}

#define GXCOLOR_AS_U32(color) (*(u32*)&(color))

void GXInitLightAttn(GXLightObj* lt_obj, f32 a0, f32 a1, f32 a2, f32 k0, f32 k1, f32 k2)
{
    GXLightObjInt* obj = (GXLightObjInt*)lt_obj;

    obj->a[0] = a0;
    obj->a[1] = a1;
    obj->a[2] = a2;
    obj->k[0] = k0;
    obj->k[1] = k1;
    obj->k[2] = k2;
}

void GXInitLightAttnA(GXLightObj* lt_obj, f32 a0, f32 a1, f32 a2)
{
    GXLightObjInt* obj = (GXLightObjInt*)lt_obj;

    obj->a[0] = a0;
    obj->a[1] = a1;
    obj->a[2] = a2;
}

void GXInitLightSpot(GXLightObj* lt_obj, f32 cutoff, GXSpotFn spot_func)
{
    f32 a0, a1, a2;
    f32 r;
    f32 d;
    f32 cr;
    GXLightObjInt* obj = (GXLightObjInt*)lt_obj;

    if (cutoff <= 0.0f || cutoff > 90.0f) {
        spot_func = GX_SP_OFF;
    }

    r = (3.1415927f * cutoff) / 180.0f;
    cr = cosf(r);

    switch (spot_func) {
        case GX_SP_FLAT:
            a0 = -1000.0f * cr;
            a1 = 1000.0f;
            a2 = 0.0f;
            break;
        case GX_SP_COS:
            a1 = 1.0f / (1.0f - cr);
            a0 = -cr * a1;
            a2 = 0.0f;
            break;
        case GX_SP_COS2:
            a2 = 1.0f / (1.0f - cr);
            a0 = 0.0f;
            a1 = -cr * a2;
            break;
        case GX_SP_SHARP:
            d = 1.0f / ((1.0f - cr) * (1.0f - cr));
            a0 = (cr * (cr - 2.0f)) * d;
            a1 = 2.0f * d;
            a2 = -d;
            break;
        case GX_SP_RING1:
            d = 1.0f / ((1.0f - cr) * (1.0f - cr));
            a2 = -4.0f * d;
            a0 = a2 * cr;
            a1 = (4.0f * (1.0f + cr)) * d;
            break;
        case GX_SP_RING2:
            d = 1.0f / ((1.0f - cr) * (1.0f - cr));
            a0 = 1.0f - ((2.0f * cr * cr) * d);
            a1 = (4.0f * cr) * d;
            a2 = -2.0f * d;
            break;
        case GX_SP_OFF:
        default:
            a0 = 1.0f;
            a1 = 0.0f;
            a2 = 0.0f;
            break;
    }

    obj->a[0] = a0;
    obj->a[1] = a1;
    obj->a[2] = a2;
}

void GXInitLightDistAttn(GXLightObj* lt_obj, f32 ref_dist, f32 ref_br, GXDistAttnFn dist_func)
{
    f32 k0, k1, k2;
    GXLightObjInt* obj = (GXLightObjInt*)lt_obj;

    if (ref_dist < 0.0f) {
        dist_func = GX_DA_OFF;
    }
    if (ref_br <= 0.0f || ref_br >= 1.0f) {
        dist_func = GX_DA_OFF;
    }

    switch (dist_func) {
        case GX_DA_GENTLE:
            k0 = 1.0f;
            k1 = (1.0f - ref_br) / (ref_br * ref_dist);
            k2 = 0.0f;
            break;
        case GX_DA_MEDIUM:
            k0 = 1.0f;
            k1 = (0.5f * (1.0f - ref_br)) / (ref_br * ref_dist);
            k2 = (0.5f * (1.0f - ref_br)) / (ref_br * ref_dist * ref_dist);
            break;
        case GX_DA_STEEP:
            k0 = 1.0f;
            k1 = 0.0f;
            k2 = (1.0f - ref_br) / (ref_br * ref_dist * ref_dist);
            break;
        case GX_DA_OFF:
        default:
            k0 = 1.0f;
            k1 = 0.0f;
            k2 = 0.0f;
            break;
    }

    obj->k[0] = k0;
    obj->k[1] = k1;
    obj->k[2] = k2;
}

void GXInitLightPos(GXLightObj* lt_obj, f32 x, f32 y, f32 z)
{
    GXLightObjInt* obj = (GXLightObjInt*)lt_obj;

    obj->lpos[0] = x;
    obj->lpos[1] = y;
    obj->lpos[2] = z;
}

void GXInitLightDir(GXLightObj* lt_obj, f32 nx, f32 ny, f32 nz)
{
    GXLightObjInt* obj = (GXLightObjInt*)lt_obj;

    obj->ldir[0] = -nx;
    obj->ldir[1] = -ny;
    obj->ldir[2] = -nz;
}

void GXInitSpecularDir(GXLightObj* lt_obj, f32 nx, f32 ny, f32 nz)
{
    f32 mag;
    f32 vx;
    f32 vy;
    f32 vz;
    GXLightObjInt* obj = (GXLightObjInt*)lt_obj;

    vx = -nx;
    vy = -ny;
    vz = -nz + 1.0f;

    mag = (vx * vx) + (vy * vy) + (vz * vz);
    if (mag != 0.0f) {
        mag = 1.0f / sqrtf(mag);
    }

    obj->ldir[0] = vx * mag;
    obj->ldir[1] = vy * mag;
    obj->ldir[2] = vz * mag;
    obj->lpos[0] = nx * -1000000000000000000.0f;
    obj->lpos[1] = ny * -1000000000000000000.0f;
    obj->lpos[2] = nz * -1000000000000000000.0f;
}

void GXInitLightColor(GXLightObj* lt_obj, GXColor color)
{
    GXLightObjInt* obj = (GXLightObjInt*)lt_obj;

    *(u32*)&obj->color = *(u32*)&color;
}

static inline void PushLight(const register GXLightObjInt* lt_obj, register void* dest)
{
	register u32 zero, color;
	register f32 a0_a1, a2_k0, k1_k2;
	register f32 px_py, pz_dx, dy_dz;
#ifdef __MWERKS__ // clang-format off
	asm {
		lwz     color, 12(lt_obj)
		xor     zero, zero, zero
		psq_l   a0_a1, 16(lt_obj), 0, 0
		psq_l   a2_k0, 24(lt_obj), 0, 0
		psq_l   k1_k2, 32(lt_obj), 0, 0
		psq_l   px_py, 40(lt_obj), 0, 0
		psq_l   pz_dx, 48(lt_obj), 0, 0
		psq_l   dy_dz, 56(lt_obj), 0, 0

		stw     zero,  0(dest)
		stw     zero,  0(dest)
		stw     zero,  0(dest)
		stw     color, 0(dest)
		psq_st  a0_a1, 0(dest), 0, 0
		psq_st  a2_k0, 0(dest), 0, 0
		psq_st  k1_k2, 0(dest), 0, 0
		psq_st  px_py, 0(dest), 0, 0
		psq_st  pz_dx, 0(dest), 0, 0
		psq_st  dy_dz, 0(dest), 0, 0
	}
#endif // clang-format on
}

void GXLoadLightObjImm(const GXLightObj* lt_obj, GXLightID light)
{
    u32 addr;
    u32 idx;
    const GXLightObjInt* obj = (const GXLightObjInt*)lt_obj;

    idx = 31 - __cntlzw(light);
    idx &= 7;
    addr = idx * XF_LIGHT_SIZE + XF_LIGHT_BASE;
    GX_WRITE_U8(0x10);
    GX_WRITE_U32(addr | 0xF0000);
    PushLight(obj, (void*)GX_FIFO_ADDR);
    gx->bpSentNot = 1;
}

void GXSetChanAmbColor(GXChannelID chan, GXColor amb_color)
{
    u32 reg;
    u32 rgb;
    u32 colIdx;

    switch (chan) {
        case GX_COLOR0:
            reg = gx->ambColor[GX_COLOR0];
            rgb = GXCOLOR_AS_U32(amb_color) >> 8;
            SET_REG_FIELD(675, reg, 24, 8, rgb);
            colIdx = 0;
            break;
        case GX_COLOR1:
            reg = gx->ambColor[GX_COLOR1];
            rgb = GXCOLOR_AS_U32(amb_color) >> 8;
            SET_REG_FIELD(690, reg, 24, 8, rgb);
            colIdx = 1;
            break;
        case GX_ALPHA0:
            reg = gx->ambColor[GX_COLOR0];
            SET_REG_FIELD(696, reg, 8, 0, amb_color.a);
            colIdx = 0;
            break;
        case GX_ALPHA1:
            reg = gx->ambColor[GX_COLOR1];
            SET_REG_FIELD(702, reg, 8, 0, amb_color.a);
            colIdx = 1;
            break;
        case GX_COLOR0A0:
            reg = GXCOLOR_AS_U32(amb_color);
            colIdx = 0;
            break;
        case GX_COLOR1A1:
            reg = GXCOLOR_AS_U32(amb_color);
            colIdx = 1;
            break;
        default:
            return;
    }

    GX_WRITE_XF_REG(colIdx + 10, reg);
    gx->bpSentNot = 1;
    gx->ambColor[colIdx] = reg;
}

void GXSetChanMatColor(GXChannelID chan, GXColor mat_color)
{
    u32 reg;
    u32 rgb;
    u32 colIdx;

    switch (chan) {
        case GX_COLOR0:
            reg = gx->matColor[GX_COLOR0];
            rgb = GXCOLOR_AS_U32(mat_color) >> 8;
            SET_REG_FIELD(776, reg, 24, 8, rgb);
            colIdx = 0;
            break;
        case GX_COLOR1:
            reg = gx->matColor[GX_COLOR1];
            rgb = GXCOLOR_AS_U32(mat_color) >> 8;
            SET_REG_FIELD(791, reg, 24, 8, rgb);
            colIdx = 1;
            break;
        case GX_ALPHA0:
            reg = gx->matColor[GX_COLOR0];
            SET_REG_FIELD(797, reg, 8, 0, mat_color.a);
            colIdx = 0;
            break;
        case GX_ALPHA1:
            reg = gx->matColor[GX_COLOR1];
            SET_REG_FIELD(803, reg, 8, 0, mat_color.a);
            colIdx = 1;
            break;
        case GX_COLOR0A0:
            reg = GXCOLOR_AS_U32(mat_color);
            colIdx = 0;
            break;
        case GX_COLOR1A1:
            reg = GXCOLOR_AS_U32(mat_color);
            colIdx = 1;
            break;
        default:
            return;
    }

    GX_WRITE_XF_REG(colIdx + 12, reg);
    gx->bpSentNot = 1;
    gx->matColor[colIdx] = reg;
}

void GXSetNumChans(u8 nChans)
{
    SET_REG_FIELD(860, gx->genMode, 3, 4, nChans);
    GX_WRITE_XF_REG(9, nChans);
    gx->dirtyState |= 4;
}

void GXSetChanCtrl(GXChannelID chan, GXBool enable, GXColorSrc amb_src, GXColorSrc mat_src,
                   u32 light_mask, GXDiffuseFn diff_fn, GXAttnFn attn_fn)
{
    u32 reg;
    u32 idx;

    idx = chan & 3;
    reg = 0;
    SET_REG_FIELD(907, reg, 1, 1, enable);
    SET_REG_FIELD(908, reg, 1, 0, mat_src);
    SET_REG_FIELD(909, reg, 1, 6, amb_src);
    SET_REG_FIELD(911, reg, 2, 7, (attn_fn == GX_AF_SPEC) ? 0 : diff_fn);
    SET_REG_FIELD(912, reg, 1, 9, attn_fn != GX_AF_NONE);
    SET_REG_FIELD(913, reg, 1, 10, attn_fn != GX_AF_SPEC);
    SET_REG_FIELD(925, reg, 4, 2, light_mask & 0xF);
    SET_REG_FIELD(926, reg, 4, 11, (light_mask >> 4) & 0xF);

    GX_WRITE_XF_REG(idx + 14, reg);
    if (chan == GX_COLOR0A0) {
        GX_WRITE_XF_REG(16, reg);
    } else if (chan == GX_COLOR1A1) {
        GX_WRITE_XF_REG(17, reg);
    }
    gx->bpSentNot = 1;
}
