/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8025D928_de_8.c, func_8025DE54_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern s32 func_00270C80(void *, s32, Vec3, s32, s32);
extern int D_8011ACA0;
s32 func_00275544(s16 arg0, Vec3 arg1, s32 arg4, s32 arg5) {
    extern s32 D_800F2A00;
    if (D_8011ACA0 != 0) {
        return -1;
    }
    return func_00270C80(&D_800F2A00, arg0, arg1, arg4, arg5);
}
