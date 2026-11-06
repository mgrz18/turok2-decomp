/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0025DAEC.c, func_0025DAEC.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "structs/sound.h"

void SndSeq_FadeOut(SndSeq *s) {
    f32 f;
    s->flags = 2;
    s->unk18 = 0x20;
    s->volume.w = (s32)(s->vol * 32767.0f);
    f = s->unk30;
    if (!(0.1f <= f)) {
        f = 0.1f;
    }
    s->unk30 = f;
    s->track = 0;
    s->fadeStep = (f32)s->volume.w / (f * 6.0e1f);
}
