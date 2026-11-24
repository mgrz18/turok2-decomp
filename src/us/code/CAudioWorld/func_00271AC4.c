/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80258800_de.c, func_80258F10_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/sound.h"
#include "protos/audio_world.h"

/* Speech at the music volume, at most 1. The ROM passes a fourth argument (0x40) the definition
 * does not take. */
s32 CAudioWorld__PlaySpeech(CAudioWorld *m, s32 group) {
    f32 var_f0 = (1.0f);
    f32 temp_f1 = m->musicVolume;
    if (!(var_f0 < temp_f1)) {
        var_f0 = temp_f1;
    }
    return ((s32 (*)(void *, s32, f32, s32))SpeechPlayer_Play)(&m->fx, group, var_f0, 0x40);
}
