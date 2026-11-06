/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/libultra/seqplayer.c, __lookupVoice.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
#include "common.h"
#include "structs/libaudio.h"

ALVoiceState *__lookupVoice(ALSeqPlayer *seqp, u8 key, u8 chan);
ALVoiceState *__lookupVoice(ALSeqPlayer *seqp, u8 key, u8 chan) {
    ALVoiceState *vs;
    for (vs = seqp->vAllocHead; vs != 0; vs = vs->next) {
        if (vs->key == key && vs->channel == chan && vs->phase != 3 && vs->phase != 4) {
            return vs;
        }
    }
    return 0;
}
