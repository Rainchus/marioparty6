#include <dolphin/gx.h>

f32 cosf(f32 angle);
f32 sinf(f32 angle);

static GXVtxDescList vcd[27];
static GXVtxAttrFmtList vat[27];

static inline void GetVertState(void)
{
    GXGetVtxDescv(vcd);
    GXGetVtxAttrFmtv(GX_VTXFMT3, vat);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_NRM, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT3, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT3, GX_VA_NRM, GX_NRM_XYZ, GX_F32, 0);
}

static inline void RestoreVertState(void)
{
    GXSetVtxDescv(vcd);
    GXSetVtxAttrFmtv(GX_VTXFMT3, vat);
}

void GXDrawCylinder(u8 numEdges)
{
    s32 i;
    f32 top;
    f32 bottom;
    f32 x[100];
    f32 y[100];
    f32 angle;

    top = 1.0f;
    bottom = -top;
    GetVertState();

    for (i = 0; i <= numEdges; i++) {
        angle = (3.1415927f * (2.0f * i)) / numEdges;
        x[i] = cosf(angle);
        y[i] = sinf(angle);
    }

    GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT3, (numEdges + 1) * 2);
    for (i = 0; i <= numEdges; i++) {
        GXPosition3f32(x[i], y[i], bottom);
        GXNormal3f32(x[i], y[i], 0.0f);
        GXPosition3f32(x[i], y[i], top);
        GXNormal3f32(x[i], y[i], 0.0f);
    }
    GXEnd();

    GXBegin(GX_TRIANGLEFAN, GX_VTXFMT3, numEdges + 2);
    GXPosition3f32(0.0f, 0.0f, top);
    GXNormal3f32(0.0f, 0.0f, 1.0f);
    for (i = 0; i <= numEdges; i++) {
        GXPosition3f32(x[i], -y[i], top);
        GXNormal3f32(0.0f, 0.0f, 1.0f);
    }
    GXEnd();

    GXBegin(GX_TRIANGLEFAN, GX_VTXFMT3, numEdges + 2);
    GXPosition3f32(0.0f, 0.0f, bottom);
    GXNormal3f32(0.0f, 0.0f, -1.0f);
    for (i = 0; i <= numEdges; i++) {
        GXPosition3f32(x[i], y[i], bottom);
        GXNormal3f32(0.0f, 0.0f, -1.0f);
    }
    GXEnd();

    RestoreVertState();
}

void GXDrawSphere(u8 numMajor, u8 numMinor)
{
    GXAttrType texType;
    f32 radius;
    f32 majorStep;
    f32 minorStep;
    s32 i;
    s32 j;
    f32 a;
    f32 b;
    f32 r0;
    f32 r1;
    f32 z0;
    f32 z1;
    f32 c;
    f32 x;
    f32 y;

    radius = 1.0f;
    majorStep = 3.1415927f / numMajor;
    minorStep = 6.2831855f / numMinor;
    GXGetVtxDesc(GX_VA_TEX0, &texType);
    GetVertState();

    if (texType != GX_NONE) {
        GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
        GXSetVtxAttrFmt(GX_VTXFMT3, GX_VA_TEX0, GX_TEX_ST, GX_RGBA6, 0);
    }

    for (i = 0; i < numMajor; i++) {
        a = i * majorStep;
        b = a + majorStep;
        r0 = radius * sinf(a);
        r1 = radius * sinf(b);
        z0 = radius * cosf(a);
        z1 = radius * cosf(b);
        GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT3, (numMinor + 1) * 2);
        for (j = 0; j <= numMinor; j++) {
            c = j * minorStep;
            x = cosf(c);
            y = sinf(c);
            GXPosition3f32(x * r1, y * r1, z1);
            GXNormal3f32((x * r1) / radius, (y * r1) / radius, z1 / radius);
            if (texType != GX_NONE) {
                GXTexCoord2f32((f32)j / (f32)numMinor,
                               (f32)(i + 1) / (f32)numMajor);
            }
            GXPosition3f32(x * r0, y * r0, z0);
            GXNormal3f32((x * r0) / radius, (y * r0) / radius, z0 / radius);
            if (texType != GX_NONE) {
                GXTexCoord2f32((f32)j / (f32)numMinor,
                               (f32)i / (f32)numMajor);
            }
        }
        GXEnd();
    }

    RestoreVertState();
}
