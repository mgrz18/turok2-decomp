/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0025DA30.c, func_0025DA30.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/junk_types.h"

extern f32 D_800A81D8;
s32 func_00266D24(void *tab, s32 n, s32 key);
SndNodeRec *func_002017D4(void *node, s32 idx);
f32 func_0026D210(u16 v);
s32 func_00275100(SndSeq *s, s32 key) {
    s32 i;
    SndNodeRec *r;
    SndNodeRec *n;
    s32 res;
    i = func_00266D24(s->owner->unk2B60, s->owner->unk2B64, key);
    if (i != -1) {
        i <<= 1;
        r = func_002017D4(s->owner->node2B50, i | 1);
        n = func_002017D4(s->owner->node2B50, i);
        s->unk24 = n->w0;
        s->unk30 = func_0026D210(n->h4);
        if (s->unk30 <= 0.0f) {
            s->unk30 = D_800A81D8;
        }
        res = (s32)r;
    } else {
        res = 0;
    }
    s->unkC = res;
    return res != 0;
}
