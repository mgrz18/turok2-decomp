/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802609AC_de_3.c, func_802615B4_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern f32 func_00218A54(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, f32 arg5);
f32 func_00218A54(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, f32 arg5) {
    f32 bits = *(f32 *)&arg0;
    f32 v1 = *(f32 *)&arg1;
    f32 v2 = *(f32 *)&arg2;
    f32 v3 = *(f32 *)&arg3;
    return bits + (v1 * arg5) + (v2 * arg5 * arg5) + (v3 * arg5 * arg5 * arg5);
}
