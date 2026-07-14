#include "gssdk/mtx.h"

#include <string.h>

u32 QrPreMult(FloatMatrix *matrix, FloatMatrix *multipliers, FloatMatrix *vector)
{
    u32 column;
    u32 row;
    u32 offset = 0;
    f32 *matrixValue = matrix->values;
    f32 *vectorValue = vector->values;
    f32 *multiplier = multipliers->values;
    f32 *matrixCursor;
    f32 *vectorCursor;

    for (column = 0; column < matrix->columns; column++, offset++) {
        f32 sum;

        matrixValue += offset;
        sum = *vectorValue++;
        matrixCursor = matrixValue;
        vectorCursor = vectorValue;
        for (row = column; row < matrix->rows; row++) {
            sum += *vectorCursor++ * *matrixCursor++;
        }

        sum *= *multiplier++;
        vectorValue[-1] -= sum;
        vectorCursor = vectorValue;
        for (row = column; row < matrix->rows; row++) {
            *vectorCursor++ -= sum * *matrixValue++;
        }
    }

    return 0;
}

void mtxFillCopy(FloatMatrix *destination, const FloatMatrix *source)
{
    memcpy(
        destination->values, source->values,
        destination->elementCount * sizeof(f32));
}

void mtxFillX(FloatMatrix *matrix, f32 value)
{
    u32 elementCount = matrix->elementCount;
    f32 *cursor = matrix->values;
    u32 i;

    for (i = 0; i < elementCount; i++) {
        *cursor++ = value;
    }
}
