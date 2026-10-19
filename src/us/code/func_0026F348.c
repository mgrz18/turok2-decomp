/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80295FF4_de_8.c, func_80296878_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

void func_0026F348(void *arg0, void *arg1, void *arg2, void *arg3) {
    extern void func_0020F040(void *out);
    extern void func_0020EFDC(void *out, void *a, void *b);
    extern void func_0020EF2C(void *out, void *a, void *b);
    PlaneEquation *plane = arg0;
    Vec3 *point = arg1;
    u8 sp10[12];
    u8 sp20[12];
    func_0020EF2C(sp10, arg2, arg1);
    func_0020EF2C(sp20, arg3, arg2);
    func_0020EFDC(arg0, sp20, sp10);
    func_0020F040(arg0);
    plane->distance = (plane->normal.x * point->x)
                                  + (plane->normal.y * point->y)
                                  + (plane->normal.z * point->z);
}
