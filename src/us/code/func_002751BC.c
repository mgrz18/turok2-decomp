/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0025DAEC.c, func_0025DAEC.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "structs/sound.h"

extern f32 D_800A81DC;
extern f32 D_800A81E0;
extern f32 D_800A81E4;
void func_002751BC(SndSeq *s) {
    f32 f;
    s->flags = 2;
    s->unk18 = 0x20;
    s->volume.w = (s32)(s->vol * D_800A81DC);
    f = s->unk30;
    if (!(D_800A81E0 <= f)) {
        f = D_800A81E0;
    }
    s->unk30 = f;
    s->unk28 = 0;
    s->fadeStep = (f32)s->volume.w / (f * D_800A81E4);
}
