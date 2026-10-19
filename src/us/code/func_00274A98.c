/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0025C844.c, func_0025C844.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/junk_types.h"

void func_00293530(void *p, s32 id);
s32 func_00274A98(SndVoices *p, s16 idx) {
    SndVoice *v;
    SndMgr *m;
    SndCore *c;
    (v) = (SndVoice *)((idx) * 0xCC + (u32)(p)); (v) = (SndVoice *)((u8 *)(v) + 4);
    m = v->mgr;
    c = (SndCore *)((u8 *)m + 0x7C);
    func_00293530(m->player, c->snd[v->idx]);
    c->snd[v->idx] = -1;
    v->unk38 = 0;
    v->unkC = -1;
    v->unk8 = -1;
    v->tag = -1;
    v->unk3A = -1;
    v->unkB4 = (SndMgr *)-1;
    return 0;
}
