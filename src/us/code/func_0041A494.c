/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8043E830_de_13.c, func_8043EE24_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "structs/game.h"

extern f32 gFrameIncrement;
s32 GameOverMenu_TickCountdown(void) {
    GameMatch *state = &D_8011ACB0;
    f32 timer = state->countdown - gFrameIncrement;
    state->countdown = timer;
    if (timer < 0.0f) {
        state->countdown = 0.0f;
    }
    return 0;
}
