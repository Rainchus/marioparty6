#ifndef GSSDK_SMOOTHER_H
#define GSSDK_SMOOTHER_H

#include "gssdk/mtx.h"

typedef struct StaticMtx {
    const f32 *data;
    u16 rows;
    u16 columns;
} StaticMtx;

typedef struct Smoother1600Info {
    StaticMtx spline;
    StaticMtx qrSpline;
    StaticMtx beta;
} Smoother1600Info;

typedef struct Smoother {
    FloatMatrix *spline;
    FloatMatrix *qrSpline;
    FloatMatrix *workingQrSpline;
    FloatMatrix *beta;
    FloatMatrix *input;
    FloatMatrix *coefficients;
    IntMatrix *activeColumns;
    f32 rejectionThreshold;
} Smoother;

extern const Smoother1600Info Smoother1600Info8kHz;
extern const Smoother1600Info Smoother1600Info11kHz;

Smoother *smtConstruct(
    TosContext *context, u32 inputLength, f32 knotSpacing,
    f32 rejectionThreshold, const Smoother1600Info *info);
void smtDestruct(Smoother *smoother, TosContext *context);
u32 Smoothing(Smoother *smoother, f32 *input);

#endif
