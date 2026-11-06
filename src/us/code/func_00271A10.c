/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80258800_de.c, func_80258E5C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

void CAudioWorld__SetFxMix(ObjRecAt2B9C *obj, float value) {
    float v;
    float max = 1.0f;
    if (value > max || !(value < 0.0f)) {
        v = value;
        if (v > max) {
            v = max;
        }
    } else {
        v = 0.0f;
    }
    obj->level = v * 4.0e1f;
}
