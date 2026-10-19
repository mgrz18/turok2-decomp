/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802744AC_de.c, func_80274D8C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern s32 func_00211D40(void *arg0, void *arg1);
s32 func_00211D40(void *arg0, void *arg1) {
    GeometryEdge *edge = arg0;
    Vec3 *point = arg1;
    Vec3 normal;
    Vec3 delta;
    s32 result;
    normal = edge->normal;
    delta.x = point->x - edge->start.x;
    delta.z = point->z - edge->start.z;
    result = 0;
    if (!(((normal.x * delta.x) + (normal.z * delta.z)) <= 0.0f)) {
        result = 1;
    }
    return result;
}
