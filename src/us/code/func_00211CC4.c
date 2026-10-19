/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802744AC_de.c, func_80274D10_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern s32 func_00211CC4(f32 *arg0, f32 *arg1, f32 *arg2);
s32 func_00211CC4(f32 *arg0, f32 *arg1, f32 *arg2) {
    Vec3 delta;
    f32 temp_f3;
    s32 result;
    delta.x = arg1[0] - arg0[0];
    delta.z = arg1[2] - arg0[2];
    temp_f3 = (arg1[6] * arg0[12]) + (arg1[8] * arg0[14]);
    result = 0;
    if (temp_f3 != 0.0f) {
        result = 1;
        *arg2 = ((delta.x * arg0[12]) + (delta.z * arg0[14])) / -temp_f3;
    }
    return result;
}
