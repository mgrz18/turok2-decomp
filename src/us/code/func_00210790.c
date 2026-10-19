/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80272FBC_de.c, func_802738C0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern float func_002974C0(float x);
extern float func_002982F0(float x);
void func_00210790(void *arg0, f32 arg1) {
    struct Mtx *m = (char *) arg0;
    f32 sin_v;
    f32 cos_v;
    f32 neg_sin;
    f32 a;
    sin_v = func_002982F0(arg1);
    cos_v = func_002974C0(arg1);
    neg_sin = -sin_v;
    a = m->right.y;
    m->right.y = (a * cos_v) + (m->right.z * neg_sin);
    m->right.z = (a * sin_v) + (m->right.z * cos_v);
    a = m->up.y;
    m->up.y = (a * cos_v) + (m->up.z * neg_sin);
    m->up.z = (a * sin_v) + (m->up.z * cos_v);
    a = m->forward.y;
    m->forward.y = (a * cos_v) + (m->forward.z * neg_sin);
    m->forward.z = (a * sin_v) + (m->forward.z * cos_v);
    a = m->m31;
    m->m31 = (a * cos_v) + (m->m32 * neg_sin);
    m->m32 = (a * sin_v) + (m->m32 * cos_v);
}
