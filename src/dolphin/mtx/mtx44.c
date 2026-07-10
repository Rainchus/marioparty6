#include "dolphin/mtx.h"

float tanf(float x);

// Functions match but has issues with float constants
void C_MTXPerspective(Mtx44 m, f32 fovY, f32 aspect, f32 n, f32 f)
{
    (void)1.0f;
    (void)2.0f;
    (void)0.0f;
    (void)-1.0f;
    (void)0.5f;
    (void)0.01745329252f;
    {
        f32 angle = fovY * 0.5f;
        f32 cot;
        f32 tmp;
        angle = MTXDegToRad(angle);
        cot = 1.0f / tanf(angle);
        m[0][0] = cot / aspect;
        m[0][1] = 0.0f;
        m[0][2] = 0.0f;
        m[0][3] = 0.0f;
        m[1][0] = 0.0f;
        m[1][1] = cot;
        m[1][2] = 0.0f;
        m[1][3] = 0.0f;
        m[2][0] = 0.0f;
        m[2][1] = 0.0f;
        tmp = 1.0f / (f - n);
        m[2][2] = -(n)*tmp;
        m[2][3] = -(f * n) * tmp;
        m[3][0] = 0.0f;
        m[3][1] = 0.0f;
        m[3][2] = -1.0f;
        m[3][3] = 0.0f;
    }
}

void C_MTXOrtho(Mtx44 m, f32 t, f32 b, f32 l, f32 r, f32 n, f32 f)
{
    f32 tmp = 1.0f / (r - l);
    m[0][0] = 2.0f * tmp;
    m[0][1] = 0.0f;
    m[0][2] = 0.0f;
    m[0][3] = -(r + l) * tmp;
    tmp = 1.0f / (t - b);
    m[1][0] = 0.0f;
    m[1][1] = 2.0f * tmp;
    m[1][2] = 0.0f;
    m[1][3] = -(t + b) * tmp;
    m[2][0] = 0.0f;
    m[2][1] = 0.0f;
    tmp = 1.0f / (f - n);
    m[2][2] = -(1.0f) * tmp;
    m[2][3] = -(f)*tmp;
    m[3][0] = 0.0f;
    m[3][1] = 0.0f;
    m[3][2] = 0.0f;
    m[3][3] = 1.0f;
}
