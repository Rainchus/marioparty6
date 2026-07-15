#include "gssdk/smoother.h"

extern void *heap_Calloc(void *heap, u32 count, u32 size);
extern void heap_Free(void *heap, void *ptr);

Smoother *smtConstruct(
    TosContext *context, u32 inputLength, f32 knotSpacing,
    f32 rejectionThreshold, const Smoother1600Info *info)
{
    Smoother *smoother;
    u32 coefficientCount;

    smoother = heap_Calloc(context->heap, 1, sizeof(Smoother));
    if (smoother == NULL) {
        return NULL;
    }

    smoother->spline = mtxConstruct(context);
    if (smoother->spline == NULL) {
        heap_Free(context->heap, smoother);
        return NULL;
    }
    mtxAssociateAll(
        smoother->spline, info->spline.rows, info->spline.columns,
        (f32 *)info->spline.data);

    smoother->qrSpline = mtxConstruct(context);
    if (smoother->qrSpline == NULL) {
        heap_Free(context->heap, smoother);
        return NULL;
    }
    mtxAssociateAll(
        smoother->qrSpline, info->qrSpline.rows, info->qrSpline.columns,
        (f32 *)info->qrSpline.data);

    smoother->beta = mtxConstruct(context);
    if (smoother->beta == NULL) {
        heap_Free(context->heap, smoother);
        return NULL;
    }
    mtxAssociateAll(
        smoother->beta, info->beta.rows, info->beta.columns,
        (f32 *)info->beta.data);

    coefficientCount =
        (u32)(0.5 + (f32)(inputLength - 1) / knotSpacing);

    smoother->input = mtxConstruct(context);
    if (smoother->input == NULL) {
        heap_Free(context->heap, smoother);
        return NULL;
    }
    mtxAssociateAll(smoother->input, inputLength, 1, NULL);

    smoother->coefficients = mtxConstruct(context);
    if (smoother->coefficients == NULL) {
        heap_Free(context->heap, smoother);
        return NULL;
    }
    if (mtxInit(
            smoother->coefficients, coefficientCount + 2, 1, context) ==
        NULL) {
        heap_Free(context->heap, smoother);
        return NULL;
    }

    smoother->activeColumns = imtxConstruct(context);
    if (smoother->activeColumns == NULL) {
        heap_Free(context->heap, smoother);
        return NULL;
    }
    if (imtxInit(
            smoother->activeColumns, 1, coefficientCount + 2, context) ==
        NULL) {
        heap_Free(context->heap, smoother);
        return NULL;
    }

    smoother->workingQrSpline = mtxConstruct(context);
    if (smoother->workingQrSpline == NULL) {
        heap_Free(context->heap, smoother);
        return NULL;
    }
    if (mtxInitCopy(
            smoother->workingQrSpline, smoother->qrSpline, context) == NULL) {
        heap_Free(context->heap, smoother);
        return NULL;
    }

    smoother->rejectionThreshold = rejectionThreshold;
    return smoother;
}

void smtDestruct(Smoother *smoother, TosContext *context)
{
    mtxDestruct(smoother->spline, context);
    mtxDestruct(smoother->qrSpline, context);
    mtxDestruct(smoother->beta, context);
    mtxDestruct(smoother->input, context);
    mtxDestruct(smoother->coefficients, context);
    imtxDestruct(smoother->activeColumns, context);
    mtxDestruct(smoother->workingQrSpline, context);
    heap_Free(context->heap, smoother);
}

u32 Smoothing(Smoother *smoother, f32 *input)
{
    FloatMatrix *workingQrSpline = smoother->workingQrSpline;
    FloatMatrix *spline = smoother->spline;
    FloatMatrix *inputVector = smoother->input;
    FloatMatrix *coefficients = smoother->coefficients;
    IntMatrix *activeColumns = smoother->activeColumns;
    u32 column;
    u32 i;

    mtxFillCopy(workingQrSpline, smoother->qrSpline);
    inputVector->values = input;
    inputVector->valuesEnd = input + inputVector->elementCount;
    mtxCompress(
        inputVector,
        mtxMax(inputVector) - smoother->rejectionThreshold);

    if (QrPreMult(workingQrSpline, smoother->beta, inputVector) != 0) {
        return 1;
    }

    for (i = activeColumns->columns; i != 0; i--) {
        activeColumns->values[i - 1] = i - 1;
    }

    column = 3;
    while (column > 2) {
        f32 *coefficient;
        f32 *inputValue;

        column = workingQrSpline->columns;
        coefficient = coefficients->values + coefficients->rows;
        inputValue = inputVector->values + column;

        for (; column != 0; column--) {
            f32 *diagonal;
            f32 *matrixValue;
            f32 *nextCoefficient;

            coefficient--;
            inputValue--;
            *coefficient = *inputValue;

            diagonal =
                workingQrSpline->values +
                (column - 1) * workingQrSpline->rows + (column - 1);
            matrixValue = diagonal + workingQrSpline->rows;
            nextCoefficient = coefficient + 1;
            while (matrixValue <
                   workingQrSpline->values +
                       workingQrSpline->rows * workingQrSpline->columns) {
                *coefficient -= *matrixValue * *nextCoefficient++;
                matrixValue += workingQrSpline->rows;
            }
            *coefficient /= *diagonal;

            if (*coefficient > 0.0f && column > 2) {
                break;
            }
        }

        if (column != 0) {
            if (QrDeleteCol(
                    workingQrSpline, inputVector, column - 1) != 0) {
                return 1;
            }
            coefficients->rows--;
            imtxDeleteCol(activeColumns, column - 1);
        }
    }

    mtxFillX(inputVector, 0.0f);
    {
        f32 *coefficient = coefficients->values;
        f32 *coefficientEnd = coefficient + coefficients->rows;
        s32 *activeColumn = activeColumns->values;

        while (coefficient < coefficientEnd) {
            f32 *output = inputVector->values;
            f32 *splineValue =
                spline->values + spline->rows * *activeColumn++;

            for (i = 0; i < inputVector->elementCount; i++) {
                *output++ += *splineValue++ * *coefficient;
            }
            coefficient++;
        }
    }

    workingQrSpline->columns = spline->columns;
    coefficients->rows = spline->columns;
    activeColumns->columns = spline->columns;
    return 0;
}
