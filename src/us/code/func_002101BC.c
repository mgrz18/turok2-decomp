/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80272FBC_de.c, func_802732EC_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern f32 func_00298470(f32);
void func_002101BC(void *arg0, f32 *arg1) {
    Matrix *m = arg0;
    Vec3 tmp;
    tmp.x = m->m[0];
    tmp.y = m->m[1];
    tmp.z = m->m[2];
    arg1[0] = func_00298470((tmp.x * tmp.x) + (tmp.y * tmp.y) + (tmp.z * tmp.z));
    tmp.x = m->m[4];
    tmp.y = m->m[5];
    tmp.z = m->m[6];
    arg1[1] = func_00298470((tmp.x * tmp.x) + (tmp.y * tmp.y) + (tmp.z * tmp.z));
    tmp.x = m->m[8];
    tmp.y = m->m[9];
    tmp.z = m->m[10];
    arg1[2] = func_00298470((tmp.x * tmp.x) + (tmp.y * tmp.y) + (tmp.z * tmp.z));
}
