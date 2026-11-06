/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80295FF4_de_8.c, func_80296878_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

void Plane_FromPoints(void *arg0, void *arg1, void *arg2, void *arg3) {
    extern void CVector3__Normalize(void *out);
    extern void CVector3__Cross(void *out, void *a, void *b);
    extern void CVector3__Subtract(void *out, void *a, void *b);
    PlaneEquation *plane = arg0;
    CVector3 *point = arg1;
    u8 sp10[12];
    u8 sp20[12];
    CVector3__Subtract(sp10, arg2, arg1);
    CVector3__Subtract(sp20, arg3, arg2);
    CVector3__Cross(arg0, sp20, sp10);
    CVector3__Normalize(arg0);
    plane->distance = (plane->normal.x * point->x)
                                  + (plane->normal.y * point->y)
                                  + (plane->normal.z * point->z);
}
