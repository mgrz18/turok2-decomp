/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80275DD4_de_2.c, func_80276510_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

void func_0026CDAC(f32 *arg0, s32 arg1, Vec3 *arg2)
{
    f32 value;
    f32 bound;
    s32 i;
    i = 0;
    if (arg1 > 0) {
        do {
            value = arg2[i].x;
            bound = arg0[0];
            if (!(value <= bound)) {
                value = bound;
            }
            arg0[0] = value;
            value = arg2[i].z;
            bound = arg0[1];
            if (!(value <= bound)) {
                value = bound;
            }
            arg0[1] = value;
            value = arg2[i].x;
            bound = arg0[2];
            if (!(bound <= value)) {
                value = bound;
            }
            arg0[2] = value;
            value = arg2[i].z;
            bound = arg0[3];
            if (!(bound <= value)) {
                value = bound;
            }
            arg0[3] = value;
            i += 1;
        } while (i < arg1);
    }
}
