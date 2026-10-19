/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80254D44_de_9.c, func_80255540_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern s32 func_00202AC0(s32 arg0);
s32 func_00202AC0(s32 arg0) {
    s32 var_a1;
    s32 var_a2;
    s32 var_v1;
    var_a1 = 0;
    var_a2 = 0;
    var_v1 = 0;
    do {
        if (arg0 & (u32)(1 << var_v1)) {
            var_a1 += 1;
            var_a2 = var_v1;
        }
        var_v1 += 1;
    } while (var_v1 < 0x20);
    if (var_a1 != 1) {
        var_a2 += 1;
    }
    return 1 << var_a2;
}
