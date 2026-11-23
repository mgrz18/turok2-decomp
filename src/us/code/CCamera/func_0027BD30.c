/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80231F6C_de_13.c, func_802337D0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "globals/engine.h"

extern f32 Math_DecayToZero(f32 arg0, f32 arg1);
u32 CShakeAxis__Advance(StateRecAt10 *state) {
    f32 scale;
    f32 step;
    if (state->value > state->limit) {
        state->limit = state->value;
    }
    if (state->rate == 0.0f) {
        state->accumulator = 9.0e1f;
    }
    if (state->rate < state->limit + state->limit) {
        state->rate = state->limit + state->limit;
    }
    step = gFrameIncrement;
    scale = 3.0f;
    state->value = Math_DecayToZero(state->value, step * scale);
    state->limit = Math_DecayToZero(state->limit, gFrameIncrement * scale);
    switch (state->mode) {
    case 1:
        if (state->rate > 0.0f) {
            state->accumulator += (state->rate + state->rate) * (gFrameIncrement * scale);
            state->rate = Math_DecayToZero(state->rate, 6.0f);
        }
        break;
    case 0:
        if (state->rate > 0.0f) {
            state->rate = Math_DecayToZero(state->rate, 6.0f);
        }
        break;
    }
    return state->rate == 0.0f;
}
