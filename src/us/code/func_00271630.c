/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80258800_de.c, func_80258A7C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern f32 D_800A80D0[];
extern s32 func_00270C80(void *, s32, Vec3, s32, s32);
void func_00271630(ContextRecAt2BC0 *arg0, s32 arg1, Vec3 arg2, s32 arg3, s32 arg4, f32 arg5) {
    arg0->alpha = arg5;
    func_00270C80(arg0, arg1, arg2, arg3, arg4);
    arg0->alpha = D_800A80D0[1];
}
extern s32 func_00270C80(void *, s32, Vec3, s32, s32);
