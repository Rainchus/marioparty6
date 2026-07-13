#ifndef _DOLPHIN_VIFUNCS
#define _DOLPHIN_VIFUNCS

#include <dolphin/types.h>

#include <dolphin/gx/GXStruct.h>
#include <dolphin/vitypes.h>

#ifdef __cplusplus
extern "C" {
#endif

void VIInit(void);
void VIConfigure(const GXRenderModeObj* rm);
void VIConfigurePan(u16 xOrg, u16 yOrg, u16 width, u16 height);
void VIFlush(void);
void VIWaitForRetrace(void);
void VISetNextFrameBuffer(void* fb);
void* VIGetNextFrameBuffer(void);
void* VIGetCurrentFrameBuffer(void);
void VISetNextRightFrameBuffer(void* fb);
void VISetBlack(BOOL black);
void VISet3D(BOOL threeD);
u32 VIGetRetraceCount(void);
u32 VIGetNextField(void);
u32 VIGetCurrentLine(void);
u32 VIGetTvFormat(void);
u32 VIGetScanMode(void);
u32 VIGetDTVStatus(void);
VIRetraceCallback VISetPreRetraceCallback(VIRetraceCallback callback);
VIRetraceCallback VISetPostRetraceCallback(VIRetraceCallback callback);

void __VIInit(VITVMode mode);
void __VISetAdjustingValues(s16 x, s16 y);
void __VIGetAdjustingValues(s16* x, s16* y);
void __VIEnableRawPositionInterrupt(s16 x, s16 y, VIPositionCallback callback);
VIPositionCallback __VIDisableRawPositionInterrupt(void);
void __VIDisplayPositionToXY(u32 hct, u32 vct, s16* x, s16* y);
void __VIGetCurrentPosition(s16* x, s16* y);
void __VISetLatchMode(u32 mode);
int __VIGetLatch0Position(s16* px, s16* py);
int __VIGetLatch1Position(s16* px, s16* py);
int __VIGetLatchPosition(u32 port, s16* px, s16* py);

#ifdef __cplusplus
}
#endif

#endif // _DOLPHIN_VIFUNCS
