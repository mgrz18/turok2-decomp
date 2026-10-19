/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8024E140_de_3.c, func_8024E59C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern void func_002688C8(void *arg0, TripleRecAt8 t, void *arg4, s32 *arg5, s32 arg6, s32 arg7);
extern f32 func_0026BE60(struct Node75 *node, f32 x, f32 z);
void func_00268560(void *arg0, void *arg1, void *arg2) {
    Vec3 *source = arg1;
    Vec3 *output = arg2;
    TripleRecAt8 t;
    s32 sp30;
    t.a = source->x;
    t.b = 0;
    t.c = source->z;
    func_002688C8(arg0, t, arg2, &sp30, 0, 0);
    output->y = func_0026BE60((struct Node75 *)sp30, output->x, output->z);
}
