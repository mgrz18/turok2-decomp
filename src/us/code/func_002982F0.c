/* Cut fixed in versions/cuts.us.txt: the real function is 0x2982F0-0x29846C (size 0x17C); splat cut its tail
 * (`return zero.f`: lui/lwc1/jr/nop at 0x29845C) into func_0029845C. */
/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/libultra/sinf.c, sinf, with Turok 2's layouts. */
/* cc1-flags: -quiet -G0 -mips3 -O2 -mgas -meb -mcpu=VR4300 -mno-fix4300 -mhard-float -mfp64 -fforce-addr */

#include "common.h"
#include "globals/libultra.h"

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
            fpoly = (((sSinfP[4].d * xsq + sSinfP[3].d) * xsq + sSinfP[2].d) * xsq + sSinfP[1].d);
            return dx + dx * xsq * fpoly;
        }
        return dx;
    }

    if (xpt < 0x136) {
        dn = dx * sSinfRpi.d;
        if (dn >= 0.0f) {
            n = dn + 0.5f;
        } else {
            n = dn - 0.5f;
        }
        dn = n;
        dx = dx - dn * sSinfPiHi.d;
        dx = dx - dn * sSinfPiLo.d;

        xsq = dx * dx;
        fpoly = (f32)(((sSinfP[4].d * xsq + sSinfP[3].d) * xsq + sSinfP[2].d) * xsq + sSinfP[1].d);
        result = dx + dx * xsq * fpoly;

        if (n & 1) {
            return -result;
        }
        return result;
    }

    return sSinfZero.f;
}
