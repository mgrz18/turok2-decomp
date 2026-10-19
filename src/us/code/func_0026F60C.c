/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80295FF4_de_8.c, func_80296B74_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

s32 func_0026F60C(f32 (*arg0)[4], f32 *arg1) {
    s32 i;
    s32 inside;
    f32 *plane;
    f32 temp_f0;
    f32 temp_f0_2;
    f32 temp_f0_3;
    f32 temp_f0_4;
    f32 temp_f1;
    f32 temp_f1_2;
    f32 x;
    f32 y;
    f32 z;
    f32 sum;
    for (i = 0; i < 6; i++) {
        plane = arg0[i];
        temp_f0 = plane[0];
        temp_f0_2 = arg1[3] * temp_f0;
        x = arg1[0] * temp_f0;
        if (!(x <= temp_f0_2)) {
            x = temp_f0_2;
        }
        temp_f0_3 = plane[1];
        y = arg1[1] * temp_f0_3;
        temp_f1 = arg1[4] * temp_f0_3;
        sum = x;
        if (!(y <= temp_f1)) {
            y = temp_f1;
        }
        sum += y;
        temp_f0_4 = plane[2];
        z = arg1[2] * temp_f0_4;
        temp_f1_2 = arg1[5] * temp_f0_4;
        if (!(z <= temp_f1_2)) {
            z = temp_f1_2;
        }
        sum += z;
        inside = sum <= plane[3];
        if (!inside) {
            return 0;
        }
    }
    return 1;
}
