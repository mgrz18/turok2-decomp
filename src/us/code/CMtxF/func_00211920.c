/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802744AC_de.c, func_802749B4_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern f32 Angle_Delta(f32 arg0, f32 arg1);
f32 Angle_Delta(f32 arg0, f32 arg1) {
    f32 var_f0;
    f32 var_f14;
    var_f14 = arg1;
    if (var_f14 < -3.1415927f) {
        do {
            var_f14 += 6.2831855f;
        } while (var_f14 < -3.1415927f);
    }
    if (3.1415927f < var_f14) {
        do {
            var_f14 -= 6.2831855f;
        } while (3.1415927f < var_f14);
    }
    var_f0 = arg0 - var_f14;
    if (var_f0 < -3.1415927f) {
        do {
            var_f0 += 6.2831855f;
        } while (var_f0 < -3.1415927f);
    }
    if (3.1415927f < var_f0) {
        do {
            var_f0 -= 6.2831855f;
        } while (3.1415927f < var_f0);
    }
    return -var_f0;
}
