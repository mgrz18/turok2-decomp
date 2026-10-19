/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8026E1F8_de_6.c, func_8026EE20_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

void func_0020C0D0(f32 m[4][4], BoxRecAtC *box, BoxRecAtC *out) {
    f32 min[3];
    f32 max[3];
    f32 newMin[3];
    f32 newMax[3];
    f32 a;
    f32 b;
    s32 i;
    s32 j;
    min[0] = box->min[0];
    min[1] = box->min[1];
    min[2] = box->min[2];
    max[0] = box->max[0];
    max[1] = box->max[1];
    max[2] = box->max[2];
    newMin[0] = newMax[0] = m[3][0];
    newMin[1] = newMax[1] = m[3][1];
    newMin[2] = newMax[2] = m[3][2];
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            a = m[j][i] * min[j];
            b = m[j][i] * max[j];
            if (a < b) {
                newMin[i] += a;
                newMax[i] += b;
            } else {
                newMin[i] += b;
                newMax[i] += a;
            }
        }
    }
    out->min[0] = newMin[0];
    out->min[1] = newMin[1];
    out->min[2] = newMin[2];
    out->max[0] = newMax[0];
    out->max[1] = newMax[1];
    out->max[2] = newMax[2];
}
