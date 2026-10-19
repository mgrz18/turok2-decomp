/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80272FBC_de.c, func_802737F0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern float func_002974C0(float x);
extern float func_002982F0(float x);
void func_002106C0(void *arg0, f32 arg1) {
    struct Mtx *m = (char *) arg0;
    f32 sin_v;
    f32 cos_v;
    f32 neg_sin;
    f32 a;
    sin_v = func_002982F0(arg1);
    cos_v = func_002974C0(arg1);
    neg_sin = -sin_v;
    a = m->up.x;
    m->up.x = (cos_v * a) + (sin_v * m->forward.x);
    m->forward.x = (neg_sin * a) + (cos_v * m->forward.x);
    a = m->up.y;
    m->up.y = (cos_v * a) + (sin_v * m->forward.y);
    m->forward.y = (neg_sin * a) + (cos_v * m->forward.y);
    a = m->up.z;
    m->up.z = (cos_v * a) + (sin_v * m->forward.z);
    m->forward.z = (neg_sin * a) + (cos_v * m->forward.z);
}
