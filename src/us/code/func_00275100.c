/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0025DA30.c, func_0025DA30.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "structs/sound.h"
#include "rw/junk_types.h" /* SndNodeRec */
#include "protos/iset.h"

s32 BinarySearch(void *tab, s32 n, s32 key);
f32 HalfToFloat(u16 v);
s32 SndSeq_SelectSong(SndSeq *s, s32 key) {
    s32 i;
    SndNodeRec *r;
    SndNodeRec *n;
    sptr res;
    i = BinarySearch(s->owner->table2B60, s->owner->count2B64, key);
    if (i != -1) {
        i <<= 1;
        r = CISet__GetBlock(s->owner->node2B50, i | 1);
        n = CISet__GetBlock(s->owner->node2B50, i);
        s->unk24 = n->w0;
        s->unk30 = HalfToFloat(n->h4);
        if (s->unk30 <= 0.0f) {
            s->unk30 = 0.1f;
        }
        res = (sptr)r;
    } else {
        res = 0;
    }
    s->unkC = res;
    return res != 0;
}
