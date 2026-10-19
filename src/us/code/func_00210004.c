/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80272FBC_de.c, func_8027310C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern float D_800A5604;
void func_00210004(void *arg0, void *arg1, f32 arg2) {
    Vec3 *direction = arg1;
    Matrix *o = arg0;
    f32 zero = 0.0f;
    f32 temp_f0;
    f32 temp_f1;
    f32 temp_f2;
    f32 temp_f3;
    temp_f0 = D_800A5604;
    o->m[15] = temp_f0;
    o->m[10] = temp_f0;
    o->m[0] = temp_f0;
    o->m[14] = zero;
    o->m[13] = zero;
    o->m[12] = zero;
    o->m[11] = zero;
    o->m[9] = zero;
    o->m[8] = zero;
    o->m[7] = zero;
    o->m[3] = zero;
    o->m[2] = zero;
    o->m[1] = zero;
    temp_f3 = temp_f0 / direction->y;
    temp_f2 = direction->x * temp_f3;
    temp_f1 = direction->z * temp_f3;
    o->m[13] = arg2;
    o->m[5] = zero;
    o->m[4] = -temp_f2;
    o->m[6] = -temp_f1;
    o->m[12] = arg2 * temp_f2;
    o->m[14] = arg2 * temp_f1;
}
