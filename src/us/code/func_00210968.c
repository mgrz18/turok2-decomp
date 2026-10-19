/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80272FBC_de.c, func_80273A98_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern float func_002974C0(float x);
extern float func_002982F0(float x);
void func_00210968(void *arg0, f32 arg1) {
    struct Mtx *m = (char *) arg0;
    f32 sin_v;
    f32 cos_v;
    f32 neg_sin;
    f32 a;
    sin_v = func_002982F0(arg1);
    cos_v = func_002974C0(arg1);
    neg_sin = -sin_v;
    a = m->right.x;
    m->right.x = (a * cos_v) + (m->right.z * sin_v);
    m->right.z = (a * neg_sin) + (m->right.z * cos_v);
    a = m->up.x;
    m->up.x = (a * cos_v) + (m->up.z * sin_v);
    m->up.z = (a * neg_sin) + (m->up.z * cos_v);
    a = m->forward.x;
    m->forward.x = (a * cos_v) + (m->forward.z * sin_v);
    m->forward.z = (a * neg_sin) + (m->forward.z * cos_v);
    a = m->m30;
    m->m30 = (a * cos_v) + (m->m32 * sin_v);
    m->m32 = (a * neg_sin) + (m->m32 * cos_v);
}
