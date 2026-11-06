/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80271C54_de_2.c, func_802725FC_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern f32 Math_AcosLookup(f32 arg0);
extern f32 CVector3__GetPitch(f32 *arg0);
extern f32 func_00298470(f32);
extern f32 func_00298470(f32 arg0);
f32 CVector3__GetPitch(f32 *arg0) {
    f32 y;
    f32 zero;
    f32 x;
    f32 z;
    f32 magnitude_squared;
    f32 ratio;
    y = arg0[1];
    zero = 0.0f;
    x = arg0[0];
    z = arg0[2];
    if (y == zero) {
        return zero;
    }
    magnitude_squared = (x * x) + (y * y) + (z * z);
    if (magnitude_squared == zero) {
        return zero;
    }
    ratio = y / func_00298470(magnitude_squared);
    if (ratio < -1.0f) {
        return zero;
    }
    if (1.0f < ratio) {
        goto return_zero;
    }
    return Math_AcosLookup(ratio) - 1.5707964f;
return_zero:
    return zero;
}
