/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80267198_de_3.c, func_80267E48_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern float D_800A65D0;
extern MenuMatchGlobals D_801198B0;
extern s32 func_0028135C(void *arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5, Triple arg6);
void func_002389EC(s32 arg0, s32 arg1, s32 arg2, Triple t, Vec4s v) {
    func_0028135C(&D_801198B0, (f32) v.x, (f32) v.y, (f32) v.z, (f32) v.w * D_800A65D0, 0, t);
}
