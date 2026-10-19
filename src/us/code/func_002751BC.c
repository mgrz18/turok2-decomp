/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0025DAEC.c, func_0025DAEC.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/junk_types.h"

extern f32 D_800A81DC;
extern f32 D_800A81E0;
extern f32 D_800A81E4;
void func_002751BC(SndSeq *s) {
    f32 f;
    s->unk1C = 2;
    s->unk18 = 0x20;
    s->unk20 = (s32)(s->vol * D_800A81DC);
    f = s->unk30;
    if (!(D_800A81E0 <= f)) {
        f = D_800A81E0;
    }
    s->unk30 = f;
    s->unk28 = 0;
    s->unk34 = (f32)s->unk20 / (f * D_800A81E4);
}
