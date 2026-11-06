/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8025A844_de_4.c, func_8025C2EC_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern f32 Random_Float(f32 arg0);
extern f32 Sound_RandomVolume(u8 arg0, u8 arg1);
f32 Sound_RandomVolume(u8 arg0, u8 arg1) {
    extern f32 D_800A8184[];
    f32 first;
    f32 second;
    first = arg0 * D_800A8184[1];
    second = arg1 * D_800A8184[1];
    if (first == 1.0f) {
        return 1.0f;
    }
    if (second == first) {
        return first;
    }
    return first + Random_Float(second - first);
}
