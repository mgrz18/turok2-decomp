/* Cut fixed in versions/cuts.us.txt: the real function is 0x2982F0-0x29846C (size 0x17C); splat cut its tail
 * (`return zero.f`: lui/lwc1/jr/nop at 0x29845C) into func_0029845C. */
/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/libultra/sinf.c, sinf, with Turok 2's layouts. */
/* cc1-flags: -quiet -G0 -mips3 -O2 -mgas -meb -mcpu=VR4300 -mno-fix4300 -mhard-float -mfp64 -fforce-addr */

#include "common.h"

typedef union {
    struct {
        u32 hi;
        u32 lo;
    } w;
    f64 d;
} du;

typedef union {
    u32 w;
    f32 f;
} fu;

extern const du D_800AA830[]; /* P: polynomial coefficients */
extern const du D_800AA858;   /* 1/pi */
extern const du D_800AA860;   /* pi, high part */
extern const du D_800AA868;   /* pi, low part */
extern const fu D_800AA870;   /* 0.0f */

f32 func_002982F0(f32 x) {
    f32 dx;
    f32 dn;
    f32 xsq;
    f32 result;
    f32 fpoly;
    s32 n;
    s32 ix;
    s32 xpt;

    ix = *(s32 *)&x;
    xpt = ix >> 22;
    xpt &= 0x1FF;
    dx = x;

    if (xpt < 0xFF) {
        if (xpt >= 0xE6) {
            xsq = dx * dx;
            fpoly = (((D_800AA830[4].d * xsq + D_800AA830[3].d) * xsq + D_800AA830[2].d) * xsq + D_800AA830[1].d);
            return dx + dx * xsq * fpoly;
        }
        return dx;
    }

    if (xpt < 0x136) {
        dn = dx * D_800AA858.d;
        if (dn >= 0.0f) {
            n = dn + 0.5f;
        } else {
            n = dn - 0.5f;
        }
        dn = n;
        dx = dx - dn * D_800AA860.d;
        dx = dx - dn * D_800AA868.d;

        xsq = dx * dx;
        fpoly = (f32)(((D_800AA830[4].d * xsq + D_800AA830[3].d) * xsq + D_800AA830[2].d) * xsq + D_800AA830[1].d);
        result = dx + dx * xsq * fpoly;

        if (n & 1) {
            return -result;
        }
        return result;
    }

    return D_800AA870.f;
}
