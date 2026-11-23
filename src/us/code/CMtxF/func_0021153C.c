#include "common.h"
#include "globals/math.h"

/* acos lookup: 1024 samples of acos over [0, 1]. */

f32 Math_AcosLookup(f32 x) {
    s32 neg;
    f32 pos;
    f32 lo;
    f32 r;
    s32 i;

    if (x < 0.0f) {
        neg = 1;
        pos = x * -1023.0f;
    } else {
        pos = x * 1023.0f;
        neg = 0;
    }
#ifdef TARGET_PORT
    /* MIPS trunc.w.s gives 0x7FFFFFFF for a NaN or an out-of-range value (the lookup then takes
     * its end branch); x86 cvttss2si gives INT_MIN, a negative table index (a NaN reaches here
     * from CVector3__GetYaw of a zero-length path step in the Mother boss level). */
    i = (pos == pos && pos < 2147483648.0f) ? (s32) pos : 0x7FFFFFFF;
#else
    i = pos;
#endif
    if ((f32) i < 1023.0f) {
        lo = gAcosTable[i];
        r = lo + (pos - (f32) i) * (*(&gAcosTable[i] + 1) - lo);
        if (neg) {
            return 3.1415927f - r;
        }
        return r;
    }
    if (neg) {
        return 3.1415927f - gAcosTable[1023];
    }
    return gAcosTable[1023];
}
