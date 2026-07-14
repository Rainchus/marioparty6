#include "gssdk/mtx.h"

#include <math.h>
#include <string.h>

extern void *heap_Alloc(void *heap, u32 size);
extern void *heap_Calloc(void *heap, u32 count, u32 size);
extern void heap_Free(void *heap, void *ptr);
extern f32 expf(f32 value);
extern f32 logf_check(f32 value);

FloatMatrix *mtxConstruct(TosContext *context)
{
    FloatMatrix *matrix = heap_Calloc(context->heap, 1, sizeof(FloatMatrix));

    if (matrix == NULL) {
        return NULL;
    }
    return matrix;
}

void mtxDestruct(FloatMatrix *matrix, TosContext *context)
{
    if (matrix->ownsValues != 0 && matrix->values != NULL) {
        heap_Free(context->heap, matrix->values);
    }
    heap_Free(context->heap, matrix);
}

IntMatrix *imtxConstruct(TosContext *context)
{
    IntMatrix *matrix = heap_Calloc(context->heap, 1, sizeof(IntMatrix));

    if (matrix == NULL) {
        return NULL;
    }
    return matrix;
}

void imtxDestruct(IntMatrix *matrix, TosContext *context)
{
    if (matrix->ownsValues != 0 && matrix->values != NULL) {
        heap_Free(context->heap, matrix->values);
    }
    heap_Free(context->heap, matrix);
}

FloatMatrix *mtxInit(
    FloatMatrix *matrix, u32 rows, u32 columns, TosContext *context)
{
    u32 elementCount = rows * columns;
    u32 byteCount;

    byteCount = elementCount * sizeof(f32);
    matrix->rows = rows;
    matrix->columns = columns;
    matrix->elementCount = elementCount;
    matrix->values = heap_Alloc(context->heap, byteCount);
    if (matrix->values == NULL) {
        heap_Free(context->heap, matrix);
        return NULL;
    }
    matrix->ownsValues = 1;
    matrix->valuesEnd = (f32 *)((u8 *)matrix->values + byteCount);
    memset(matrix->values, 0, byteCount);
    return matrix;
}

IntMatrix *imtxInit(
    IntMatrix *matrix, u32 rows, u32 columns, TosContext *context)
{
    u32 elementCount = rows * columns;

    matrix->rows = rows;
    matrix->columns = columns;
    matrix->elementCount = elementCount;
    matrix->values = heap_Calloc(context->heap, elementCount, sizeof(s32));
    if (matrix->values == NULL) {
        heap_Free(context->heap, matrix);
        return NULL;
    }
    matrix->ownsValues = 1;
    matrix->valuesEnd = matrix->values + elementCount;
    return matrix;
}

FloatMatrix *mtxInitCopy(
    FloatMatrix *matrix, const FloatMatrix *source, TosContext *context)
{
    matrix->rows = source->rows;
    matrix->columns = source->columns;
    matrix->elementCount = source->rows * source->columns;
    matrix->values = heap_Alloc(
        context->heap, matrix->rows * matrix->columns * sizeof(f32));
    if (matrix->values == NULL) {
        heap_Free(context->heap, matrix);
        return NULL;
    }
    matrix->ownsValues = 1;
    matrix->valuesEnd = matrix->values + matrix->rows * matrix->columns;
    mtxFillCopy(matrix, source);
    return matrix;
}

void imtxDeleteCol(IntMatrix *matrix, u32 column)
{
    memcpy(
        matrix->values + matrix->rows * column,
        matrix->values + matrix->rows * column + matrix->rows,
        (matrix->rows * matrix->columns - matrix->rows -
         matrix->rows * column) *
            sizeof(s32));
    matrix->columns--;
    matrix->elementCount--;
    matrix->valuesEnd -= matrix->rows;
}

void mtxAssociateAll(
    FloatMatrix *matrix, u32 rows, u32 columns, f32 *values)
{
    matrix->rows = rows;
    matrix->columns = columns;
    matrix->elementCount = rows * columns;
    matrix->values = values;
    matrix->valuesEnd = values + matrix->elementCount;
}

f32 mtxMax(const FloatMatrix *matrix)
{
    f32 *value = matrix->values;
    f32 maximum = *value++;
    u32 i;

    for (i = 1; i < matrix->elementCount; i++) {
        if (*value > maximum) {
            maximum = *value;
        }
        value++;
    }
    return maximum;
}

void mtxCompress(FloatMatrix *matrix, f32 center)
{
    u32 elementCount = matrix->rows * matrix->columns;
    u32 i = 0;
    f32 *value = matrix->values + i;

    for (; i < elementCount; i++, value++) {
        f32 reduction =
            0.5 * logf_check(1.0f + expf((f32)(2.0 * (*value - center))));

        *value = *value - reduction;
    }
}

u32 QrDeleteCol(FloatMatrix *matrix, FloatMatrix *vector, u32 column)
{
    f32 *matrixEnd = matrix->values + matrix->rows * matrix->columns;
    f32 *vectorEnd = vector->values + vector->elementCount;
    u32 currentColumn;

    for (currentColumn = column + 1; currentColumn < matrix->columns;
         currentColumn++) {
        f32 *source =
            matrix->values + matrix->rows * currentColumn;
        f32 *diagonal = source + currentColumn;
        f32 upper;
        f32 lower;
        f32 absoluteUpper;
        f32 absoluteLower;
        f32 ratio;
        f32 cosine;
        f32 sine;
        f32 *destination;
        f32 *value;
        u32 row;

        lower = *diagonal;
        diagonal--;
        upper = *diagonal;
        absoluteUpper = upper > 0.0f ? upper : -upper;
        absoluteLower = lower > 0.0f ? lower : -lower;

        if (absoluteLower > absoluteUpper) {
            ratio = -upper / lower;
            sine = 1.0 / sqrtf(1.0f + ratio * ratio);
            cosine = sine * ratio;
        } else {
            ratio = -lower / upper;
            cosine = 1.0 / sqrtf(1.0f + ratio * ratio);
            sine = cosine * ratio;
        }

        destination = source - matrix->rows;
        for (row = 1; row < currentColumn; row++) {
            *destination++ = *source++;
        }
        *destination = cosine * upper - sine * lower;

        value = diagonal + matrix->rows;
        while (value < matrixEnd) {
            f32 first = value[0];
            f32 second = value[1];

            value[0] = cosine * first - sine * second;
            value[1] = sine * first + cosine * second;
            value += matrix->rows;
        }

        value = vector->values + currentColumn;
        while (value < vectorEnd) {
            f32 first = value[-1];
            f32 second = value[0];

            value[-1] = cosine * first - sine * second;
            value[0] = sine * first + cosine * second;
            value += vector->rows;
        }
    }
    matrix->columns--;
    return 0;
}
