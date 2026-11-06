/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8025C524_de_8.c, func_8025D158_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "globals/audio.h"

extern s16 alSndpAllocate(void *, void *);
void SpeechPlayer_Construct(struct Shape_typemap_110 *record, s32 resource) {
    extern void *CAudioWorld__GetSndPlayer(s32);
    record->field_0 = resource;
    record->field_4 = -1;
    record->field_C = -1;
    record->field_8 = 0;
    record->field_14 = 0;
    record->field_18 = 0x40;
    record->field_1C = alSndpAllocate(CAudioWorld__GetSndPlayer(record->field_0), &sSpeechSound);
}
