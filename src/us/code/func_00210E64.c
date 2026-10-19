/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80272FBC_de.c, func_80273F94_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern void func_00210EF0(f32 *value);
extern f32 func_00210E64(f32 arg0, f32 arg1);
extern float D_800A5638;
f32 func_00210E64(f32 arg0, f32 arg1) {
    extern f32 D_800A563C[];
    f32 var_f3;
    func_00210EF0(&arg0);
    func_00210EF0(&arg1);
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
void func_00210EF0(f32 *value);
