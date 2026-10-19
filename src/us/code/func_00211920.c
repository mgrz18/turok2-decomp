/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802744AC_de.c, func_802749B4_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern float D_800A56A8;
extern float D_800A56A0;
extern f32 func_00211920(f32 arg0, f32 arg1);
extern float D_800A56B8;
extern float D_800A56B0;
f32 func_00211920(f32 arg0, f32 arg1) {
    f32 var_f0;
    f32 var_f14;
    var_f14 = arg1;
    if (var_f14 < D_800A56A0) {
        do {
            var_f14 += *(&D_800A56A0 + 1);
        } while (var_f14 < D_800A56A0);
    }
    if (D_800A56A8 < var_f14) {
        do {
            var_f14 -= *(&D_800A56A8 + 1);
        } while (D_800A56A8 < var_f14);
    }
    var_f0 = arg0 - var_f14;
    if (var_f0 < D_800A56B0) {
        do {
            var_f0 += *(&D_800A56B0 + 1);
        } while (var_f0 < D_800A56B0);
    }
    if (D_800A56B8 < var_f0) {
        do {
            var_f0 -= *(&D_800A56B8 + 1);
        } while (D_800A56B8 < var_f0);
    }
    return -var_f0;
}
