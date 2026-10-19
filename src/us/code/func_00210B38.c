/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80272FBC_de.c, func_80273C68_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern float func_002974C0(float x);
extern float func_002982F0(float x);
void func_00210B38(void *arg0, f32 arg1) {
    struct Mtx *m = (char *) arg0;
    f32 sin_v;
    f32 cos_v;
    f32 neg_sin;
    f32 a;
    sin_v = func_002982F0(arg1);
    cos_v = func_002974C0(arg1);
    neg_sin = -sin_v;
    a = m->right.x;
    m->right.x = (a * cos_v) + (m->right.y * neg_sin);
    m->right.y = (a * sin_v) + (m->right.y * cos_v);
    a = m->up.x;
    m->up.x = (a * cos_v) + (m->up.y * neg_sin);
    m->up.y = (a * sin_v) + (m->up.y * cos_v);
    a = m->forward.x;
    m->forward.x = (a * cos_v) + (m->forward.y * neg_sin);
    m->forward.y = (a * sin_v) + (m->forward.y * cos_v);
    a = m->m30;
    m->m30 = (a * cos_v) + (m->m31 * neg_sin);
    m->m31 = (a * sin_v) + (m->m31 * cos_v);
}
