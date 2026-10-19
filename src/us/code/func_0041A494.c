/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8043E830_de_13.c, func_8043EE24_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern struct Field_f32_10 D_8011ACB0;
extern f32 D_800B6D28;
s32 func_0041A494(void) {
    struct Field_f32_10 *state = &D_8011ACB0;
    f32 timer = state->value - D_800B6D28;
    state->value = timer;
    if (timer < 0.0f) {
        state->value = 0.0f;
    }
    return 0;
}
