/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802A7AA4_de_7.c, func_802AAB7C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern void COnScreen__DrawText(u8 *text, f32 x, f32 y, f32 width, f32 height, s32 shadow, s32 alpha, s32 language);
void func_0027B274(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5, s32 arg6, s32 arg7) {
    if (arg5 != 0) {
        COnScreen__DrawText(arg0, arg1, arg2, arg3, arg4, 1, arg6, arg7);
    }
    COnScreen__DrawText(arg0, arg1, arg2, arg3, arg4, 0, arg6, arg7);
}
