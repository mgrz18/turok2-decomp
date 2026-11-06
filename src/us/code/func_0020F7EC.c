/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80271C54_de_2.c, func_80272828_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

void CMtxF__ClampEntries(f32 *arg0) {
    f32 *var_a0;
    f32 *var_v1;
    f32 temp_f0;
    s32 var_a1;
    s32 var_a2;
    f32 maxVal;
    f32 minVal;
    maxVal = 32767.0f;
    minVal = -32767.0f;
    var_a0 = arg0;
    var_a2 = 0;
    do {
        var_a1 = 0;
        var_v1 = var_a0;
        do {
            temp_f0 = *var_v1;
            if (maxVal < temp_f0) {
                *var_v1 = maxVal;
            } else if (temp_f0 < minVal) {
                *var_v1 = minVal;
            }
            var_a1 += 1;
            var_v1 += 1;
        } while (var_a1 < 4);
        var_a2 += 1;
        var_a0 += 4;
    } while (var_a2 < 4);
}
