/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0025DCA8.c, func_0025DCA8.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "structs/sound.h"

extern f32 D_800A8200;
void alCSPSetVol(void *csp, s16 vol);
void SndSeq_UpdateVolume(SndSeq *s) {
    f32 f;
    if (s->sliding != 0) {
        f = (f32)s->unk24;
        f *= s->owner->volume2BA4 * 0.01f;
        f *= s->current;
    } else {
        f = (f32)s->unk24 * (s->owner->volume2BA4 * 0.01f);
        if (s->owner->muted != 0) {
            f *= 0.7f;
        }
        if (f == s->vol) {
            return;
        }
    }
    alCSPSetVol((void *)s->csp, f * D_800A8200);
    s->vol = f;
}
