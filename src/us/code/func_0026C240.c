/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80275DD4_de.c, func_802760BC_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern f32 func_0021153C(f32 arg0);
extern f32 func_00298470(f32);
f32 func_0026C240(void *arg0, s32 arg1) {
    Triangle *polygon = arg0;
    Vec3 *v0;
    Vec3 *v1;
    f32 dx, dz;
    f32 mag;
    f32 result;
    v1 = polygon->vertices[arg1];
    v0 = polygon->vertices[(arg1 + 1) % 3];
    dx = v0->x - v1->x;
    dz = v0->z - v1->z;
    mag = func_00298470((dx * dx) + (dz * dz));
    if (mag == 0.0f) {
        return 0.0f;
    }
    result = func_0021153C(dx / mag);
    if (!(dz < 0.0f)) {
        result = -result;
    }
    return result;
}
