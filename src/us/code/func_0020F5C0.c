/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80271C54_de_2.c, func_802725FC_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern f32 func_0021153C(f32 arg0);
extern f32 func_0020F5C0(f32 *arg0);
extern float D_800A55D4;
extern float D_800A55DC;
extern f32 func_00298470(f32);
extern f32 func_00298470(f32 arg0);
f32 func_0020F5C0(f32 *arg0) {
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
    if (ratio < *(&D_800A55D4 + 1)) {
        return zero;
    }
    if (D_800A55DC < ratio) {
        goto return_zero;
    }
    return func_0021153C(ratio) - *(&D_800A55DC + 1);
return_zero:
    return zero;
}
