/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80272FBC_de.c, func_802734F8_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern void func_002103C8(float * arg0, float * arg1, float sx, float sy, float sz);
extern f32 D_800A5618;
void func_002103C8(f32 *arg0, f32 *arg1, f32 sx, f32 sy, f32 sz) {
    arg0[0] = arg1[0] * sx;
    arg0[1] = arg1[1] * sx;
    arg0[2] = arg1[2] * sx;
    arg0[4] = arg1[4] * sy;
    arg0[5] = arg1[5] * sy;
    arg0[6] = arg1[6] * sy;
    arg0[8] = arg1[8] * sz;
    arg0[9] = arg1[9] * sz;
    arg0[10] = arg1[10] * sz;
    arg0[12] = arg1[12];
    arg0[13] = arg1[13];
    arg0[14] = arg1[14];
    arg0[3] = arg0[7] = arg0[11] = 0.0f;
    arg0[15] = D_800A5618;
}
