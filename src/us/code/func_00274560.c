/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8025A844_de_4.c, func_8025C2EC_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern f32 func_0021149C(f32 arg0);
extern f32 func_00274560(u8 arg0, u8 arg1);
f32 func_00274560(u8 arg0, u8 arg1) {
    extern f32 D_800A818C;
    extern f32 D_800A8184[];
    f32 first;
    f32 second;
    first = arg0 * D_800A8184[1];
    second = arg1 * D_800A8184[1];
    if (first == D_800A818C) {
        return D_800A818C;
    }
    if (second == first) {
        return first;
    }
    return first + func_0021149C(second - first);
}
