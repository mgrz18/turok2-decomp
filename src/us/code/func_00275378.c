/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0025DCA8.c, func_0025DCA8.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/junk_types.h"

extern f32 D_800A81F4;
extern f32 D_800A81F8;
extern f32 D_800A81FC;
extern f32 D_800A8200;
void func_00290EC0(void *csp, s16 vol);
void func_00275378(SndSeq *s) {
    f32 f;
    if (s->unk38 != 0) {
        f = (f32)s->unk24;
        f *= s->owner->unk2BA4 * D_800A81F4;
        f *= s->unk3C;
    } else {
        f = (f32)s->unk24 * (s->owner->unk2BA4 * D_800A81F8);
        if (s->owner->unk2BB8 != 0) {
            f *= D_800A81FC;
        }
        if (f == s->vol) {
            return;
        }
    }
    func_00290EC0(s->csp, f * D_800A8200);
    s->vol = f;
}
