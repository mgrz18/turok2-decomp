/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80231F6C_de_13.c, func_802337D0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern f32 func_0021170C(f32 arg0, f32 arg1);
extern float D_800A97D0;
extern float D_800A97D4;
extern float D_800A97D8;
extern float D_800A97DC;
extern f32 D_800B6D28;
u32 func_0027BD30(StateRecAt10 *state) {
    f32 scale;
    f32 step;
    if (state->value > state->limit) {
        state->limit = state->value;
    }
    if (state->rate == 0.0f) {
        state->accumulator = D_800A97D0;
    }
    if (state->rate < state->limit + state->limit) {
        state->rate = state->limit + state->limit;
    }
    step = D_800B6D28;
    scale = D_800A97D4;
    state->value = func_0021170C(state->value, step * scale);
    state->limit = func_0021170C(state->limit, D_800B6D28 * scale);
    switch (state->mode) {
    case 1:
        if (state->rate > 0.0f) {
            state->accumulator += (state->rate + state->rate) * (D_800B6D28 * scale);
            state->rate = func_0021170C(state->rate, D_800A97D8);
        }
        break;
    case 0:
        if (state->rate > 0.0f) {
            state->rate = func_0021170C(state->rate, D_800A97DC);
        }
        break;
    }
    return state->rate == 0.0f;
}
