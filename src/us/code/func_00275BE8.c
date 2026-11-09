/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8025C524_de_2.c, func_8025C8D8_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "protos/list.h"

void LoopSounds_Construct(void *arg0, SoundSourceNode *arg1, s32 arg2) {
    s32 i;
    CList__Construct(arg0, 0, sizeof(void *));
    CList__Construct(&((SoundVoicePool *)arg0)->playing, 0, sizeof(void *));
    i = 0;
    if (arg2 > 0) {
        do {
            CList__AddTail(arg0, arg1);
            i += 1;
            arg1 = arg1 + 1;
        } while (i < arg2);
    }
    ((SoundVoicePool *)arg0)->state = 0;
}
