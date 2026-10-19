/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802AD270_de_9.c, func_802AE6BC_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

void func_0028F61C(RuntimeState *state, DecodeResult_func_802AE6BC_de *result) {
    extern u32 func_0028F98C(RuntimeState *state, u32 index);
    extern s32 __alCSeqGetTrackEvent(RuntimeState *state, u32 index, DecodeResult *result);
    u32 minimum;
    u32 index;
    u32 selected;
    u32 decrement;
    minimum = -1;
    index = 0;
    decrement = state->previous;
    do {
        if ((state->active >> index) & 1) {
            if (state->first != 0) {
                state->results[index] -= decrement;
            }
            if (state->results[index] < minimum) {
                minimum = state->results[index];
                selected = index;
            }
        }
        index++;
    } while (index < 16);
    __alCSeqGetTrackEvent(state, selected, result);
    result->value = minimum;
    state->previous = minimum;
    state->total += minimum;
    if (result->type != 0x12) {
        u32 delta;
        delta = func_0028F98C(state, selected);
        state->results[selected] += delta;
    }
    state->first = 1;
}
