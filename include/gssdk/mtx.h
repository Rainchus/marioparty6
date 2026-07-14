#ifndef GSSDK_MTX_H
#define GSSDK_MTX_H

#include "gssdk/tos.h"

typedef struct FloatMatrix {
    u32 rows;
    u32 columns;
    u32 elementCount;
    f32 *values;
    f32 *valuesEnd;
    u8 ownsValues;
} FloatMatrix;

typedef struct IntMatrix {
    u32 rows;
    u32 columns;
    u32 elementCount;
    s32 *values;
    s32 *valuesEnd;
    u8 ownsValues;
} IntMatrix;

FloatMatrix *mtxConstruct(TosContext *context);
void mtxDestruct(FloatMatrix *matrix, TosContext *context);
IntMatrix *imtxConstruct(TosContext *context);
void imtxDestruct(IntMatrix *matrix, TosContext *context);
FloatMatrix *mtxInit(
    FloatMatrix *matrix, u32 rows, u32 columns, TosContext *context);
IntMatrix *imtxInit(
    IntMatrix *matrix, u32 rows, u32 columns, TosContext *context);
FloatMatrix *mtxInitCopy(
    FloatMatrix *matrix, const FloatMatrix *source, TosContext *context);
void imtxDeleteCol(IntMatrix *matrix, u32 column);
void mtxAssociateAll(
    FloatMatrix *matrix, u32 rows, u32 columns, f32 *values);
f32 mtxMax(const FloatMatrix *matrix);
void mtxCompress(FloatMatrix *matrix, f32 center);
u32 QrDeleteCol(FloatMatrix *matrix, FloatMatrix *vector, u32 column);

u32 QrPreMult(
    FloatMatrix *matrix, FloatMatrix *multipliers, FloatMatrix *vector);
void mtxFillCopy(FloatMatrix *destination, const FloatMatrix *source);
void mtxFillX(FloatMatrix *matrix, f32 value);

#endif
