#ifndef _DOLPHIN_OSFASTCAST
#define _DOLPHIN_OSFASTCAST

#include <dolphin/types.h>

#ifdef __cplusplus
extern "C" {
#endif

void OSInitFastCast(void);

static s16 __OSf32tos16(f32 inF)
{
    if (inF != inF || inF >= 32767.0f) {
        return 32767;
    }
    if (inF <= -32768.0f) {
        return -32768;
    }
    return (s16)inF;
}

static void OSf32tos16(f32* in, s16* out)
{
    *out = __OSf32tos16(*in);
}

static u8 __OSf32tou8(f32 inF)
{
    if (inF != inF || inF >= 255.0f) {
        return 255;
    }
    if (inF <= 0.0f) {
        return 0;
    }
    return (u8)inF;
}

static void OSf32tou8(f32* in, u8* out)
{
    *out = __OSf32tou8(*in);
}

static s8 __OSf32tos8(f32 inF)
{
    if (inF != inF || inF >= 127.0f) {
        return 127;
    }
    if (inF <= -128.0f) {
        return -128;
    }
    return (s8)inF;
}

static void OSf32tos8(f32* in, s8* out)
{
    *out = __OSf32tos8(*in);
}

static u16 __OSf32tou16(f32 inF)
{
    if (inF != inF || inF >= 65535.0f) {
        return 65535;
    }
    if (inF <= 0.0f) {
        return 0;
    }
    return (u16)inF;
}

static void OSf32tou16(f32* in, u16* out)
{
    *out = __OSf32tou16(*in);
}

static f32 __OSs8tof32(const s8* in)
{
    return (f32)*in;
}

static void OSs8tof32(const s8* in, f32* out)
{
    *out = __OSs8tof32(in);
}

static f32 __OSs16tof32(const s16* in)
{
    return (f32)*in;
}

static void OSs16tof32(const s16* in, f32* out)
{
    *out = __OSs16tof32(in);
}

static f32 __OSu8tof32(const u8* in)
{
    return (f32)*in;
}

static void OSu8tof32(const u8* in, f32* out)
{
    *out = __OSu8tof32(in);
}

static f32 __OSu16tof32(const u16* in)
{
    return (f32)*in;
}

static void OSu16tof32(const u16* in, f32* out)
{
    *out = __OSu16tof32(in);
}

#ifdef __cplusplus
}
#endif

#endif
