/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802AD270_de_9.c, func_802AE7BC_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern f32 alCSeqTicksToSec(void **arg0, s32 arg1, s32 arg2);
f32 alCSeqTicksToSec(void **arg0, s32 arg1, s32 arg2) {
    f64 angle;
    f32 arg1f;
    f32 scaled;
    s32 temp_v0;
    f64 val;
    angle = (f64)arg2;
    arg1f = (f32)arg1;
    if (arg2 < 0) {
        angle += 4294967296.0;
    }
    temp_v0 = ((ObjRecAt40V1 *)((*arg0)))->unk40;
    scaled = arg1f * (f32)angle;
    val = (f64)temp_v0;
    if (temp_v0 < 0) {
        val += 4294967296.0;
    }
    return scaled / ((f32)val * 1.0e6f);
}
