/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80272FBC_de.c, func_80273648_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern float func_002974C0(float x);
extern float func_002982F0(float x);
void func_00210518(void *arg0, f32 arg1) {
    extern f32 D_800A561C;
    struct Mtx *m = arg0;
    f32 sin_v;
    f32 cos_v;
    f32 zero;
    f32 identity;
    sin_v = func_002982F0(arg1);
    zero = 0;
    identity = D_800A561C;
    m->forward.y = -sin_v;
    m->up.z = sin_v;
    m->right.y = zero;
    m->up.x = zero;
    m->right.z = zero;
    m->forward.x = zero;
    m->m32 = zero;
    m->m31 = zero;
    m->m30 = zero;
    m->m23 = zero;
    m->m13 = zero;
    m->m03 = zero;
    m->m33 = identity;
    m->right.x = identity;
    cos_v = func_002974C0(arg1);
    m->forward.z = cos_v;
    m->up.y = cos_v;
}
