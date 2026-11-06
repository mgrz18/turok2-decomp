/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8043E830_de_13.c, func_8043EE24_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "structs/game.h"
#include "globals/game.h"
#include "globals/engine.h"

s32 GameOverMenu_TickCountdown(void) {
    GameMatch *state = &gEngineApp_m_Settings.match;
    f32 timer = state->countdown - gFrameIncrement;
    state->countdown = timer;
    if (timer < 0.0f) {
        state->countdown = 0.0f;
    }
    return 0;
}
