/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0025B940.c, func_0025B940.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/junk_types.h"

void func_00293E60(void *p, s32 id);
s32 func_00293580(void *p);
void func_00293EC0(void *p);
s32 func_00273C5C(SndVoices *p, s16 tag, s16 key) {
    s32 i;
    s32 j;
    SndVoice *v = p->v;
    SndVoice *s;
    SndMgr *m;
    i = 0;
    for (; i < 16; i++) {
        if (v->unk8 != -1 && p->owner->unk102 != i && v->tag == tag && (v->unkA4 & 0x40) != 0 && v->unk3A == key) {
            j = (s16)i;
            (s) = (SndVoice *)((j) * 0xCC + (u32)(p)); (s) = (SndVoice *)((u8 *)(s) + 4);
            m = s->mgr;
            s->unkAC = 1;
            s->unk50 = 0;
            if (s->unk10 != m->unk104) {
                func_00293E60(m->player, m->snd[s->idx]);
                if (func_00293580(m->player) != 0) {
                    func_00293EC0(m->player);
                }
                s->unk4 = -1;
            }
            return (s16)i;
        }
        v++;
    }
    return -1;
}
