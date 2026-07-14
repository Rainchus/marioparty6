/* @(#)e_exp.c 1.6 04/04/22 */
/*
 * ====================================================
 * Copyright (C) 2004 by Sun Microsystems, Inc. All rights reserved.
 *
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice
 * is preserved.
 * ====================================================
 */

#include "PowerPC_EABI_Support/Msl/MSL_C/MSL_Common_Embedded/Math/fdlibm.h"

#ifdef __STDC__
static const double
#else
static double
#endif
one = 1.0,
halF[2] = {0.5, -0.5},
huge = 1.0e+300,
twom1000 = 9.33263618503218878990e-302,
o_threshold = 7.09782712893383973096e+02,
u_threshold = -7.45133219101941108420e+02,
ln2HI[2] = {
    6.93147180369123816490e-01,
    -6.93147180369123816490e-01,
},
ln2LO[2] = {
    1.90821492927058770002e-10,
    -1.90821492927058770002e-10,
},
invln2 = 1.44269504088896338700e+00,
P1 = 1.66666666666666019037e-01,
P2 = -2.77777777770155933842e-03,
P3 = 6.61375632143793436117e-05,
P4 = -1.65339022054652515390e-06,
P5 = 4.13813679705723846039e-08;

#ifdef __STDC__
double __ieee754_exp(double x)
#else
double __ieee754_exp(x)
double x;
#endif
{
    double y, hi, lo, c, t;
    int k, xsb;
    unsigned int hx;

    hx = __HI(x);
    xsb = (hx >> 31) & 1;
    hx &= 0x7fffffff;

    if (hx >= 0x40862E42) {
        if (hx >= 0x7ff00000) {
            if (((hx & 0xfffff) | __LO(x)) != 0) {
                return x + x;
            } else {
                return (xsb == 0) ? x : 0.0;
            }
        }
        if (x > o_threshold) {
            return huge * huge;
        }
        if (x < u_threshold) {
            return twom1000 * twom1000;
        }
    }

    if (hx > 0x3fd62e42) {
        if (hx < 0x3FF0A2B2) {
            hi = x - ln2HI[xsb];
            lo = ln2LO[xsb];
            k = 1 - xsb - xsb;
        } else {
            k = (int)(invln2 * x + halF[xsb]);
            t = k;
            hi = x - t * ln2HI[0];
            lo = t * ln2LO[0];
        }
        x = hi - lo;
    } else if (hx < 0x3e300000) {
        if (huge + x > one) {
            return one + x;
        }
    } else {
        k = 0;
    }

    t = x * x;
    c = x - t * (P1 + t * (P2 + t * (P3 + t * (P4 + t * P5))));
    if (k == 0) {
        return one - ((x * c) / (c - 2.0) - x);
    } else {
        y = one - ((lo - (x * c) / (2.0 - c)) - hi);
    }

    if (k >= -1021) {
        __HI(y) += k << 20;
        return y;
    } else {
        __HI(y) += (k + 1000) << 20;
        return y * twom1000;
    }
}
