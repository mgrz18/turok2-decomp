/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80272FBC_de.c, func_80273F94_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern void Angle_Wrap(f32 *value);
extern f32 Angle_Difference(f32 arg0, f32 arg1);
extern float D_800A5638;
f32 Angle_Difference(f32 arg0, f32 arg1) {
    extern f32 D_800A563C[];
    f32 var_f3;
    Angle_Wrap(&arg0);
    Angle_Wrap(&arg1);
    if (arg0 > arg1) {
        var_f3 = arg1 + D_800A5638;
        if ((var_f3 - arg0) < (arg0 - arg1)) {
            arg1 = var_f3;
        }
    } else {
        var_f3 = arg1 - D_800A563C[0];
        if ((arg0 - var_f3) < (arg1 - arg0)) {
            arg1 = var_f3;
        }
    }
    return arg0 - arg1;
}
void Angle_Wrap(f32 *value);
