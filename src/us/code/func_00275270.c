/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/fuzzy/func_8025DB80_de.c, func_8025DB80_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern s32 BinarySearch(void *, s32, s32);
extern void *CISet__GetBlock(void *, s32);
extern f32 HalfToFloat(s32);
void *SndSeq_GetSongData(SequenceState *state, s32 index) {
    s32 selection;
    s32 section;
    void *result;
    SequenceCommand *command;
    f32 duration;
    selection = BinarySearch(state->resource->lookup, state->resource->count, index);
    if (selection != -1) {
        section = selection * 2;
        result = CISet__GetBlock(state->resource->sections, section | 1);
        command = CISet__GetBlock(state->resource->sections, section);
        state->command = command->command;
        duration = HalfToFloat(command->duration);
        state->duration = duration;
        if (duration <= 0.0f) state->duration = 0.1f;
        return result;
    }
    return ((void *)0);
}
