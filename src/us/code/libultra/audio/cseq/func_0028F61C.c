/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802AD270_de_9.c, func_802AE6BC_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

u8 __getTrackByte(ALCSeq *seq, u32 track); /* ABI: returns u8 (the host leaves the upper bits of the return register undefined) */
u32 __readVarLen(ALCSeq *seq, u32 track);
u32 __alCSeqGetTrackEvent(ALCSeq *seq, u32 track, ALEvent *event);

void alCSeqNextEvent(RuntimeState *state, DecodeResult_func_802AE6BC_de *result) {
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
    __alCSeqGetTrackEvent((ALCSeq *)state, selected, (ALEvent *)result);
    result->value = minimum;
    state->previous = minimum;
    state->total += minimum;
    if (result->type != 0x12) {
        u32 delta;
        delta = __readVarLen((ALCSeq *)state, selected);
        state->results[selected] += delta;
    }
    state->first = 1;
}
