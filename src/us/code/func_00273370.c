/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0025B054.c, func_0025B054.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/junk_types.h"

void func_00293E60(void *p, s32 id);
s32 func_00293580(void *p);
void func_00293EC0(void *p);
s32 func_00273370(SndVoices *p, s32 a, s32 b, s16 pri, s32 c) {
    SndMgr *m;
    SndCore *core;
    s32 i;
    s32 best;
    s32 bestIdx;
    s16 lim;
    SndVoice *v;
    SndVoice *s;
    SndMgr *m2;
    s32 r;
    best = pri;
    bestIdx = -1;
    v = p->v;
    lim = best;
    m = p->owner;
    core = (SndCore *)((u8 *)m + 0x7C);
    for (i = 0; i < 16; i++) {
        if (core->snd[i] == -1) {
            return (s16)i;
        }
        if (v->unk38 < lim) {
            if (!(best < v->unk38) && v->unk38 != 100) {
                best = v->unk38;
                bestIdx = i;
            }
        }
        v++;
    }
    if (bestIdx == -1) {
        return -1;
    }
    if (core->snd[bestIdx] == -1) {
        SndNode *h = (SndNode *)m->q.active.link.next;
        if ((SndLink *)h == &m->q.active.link) {
            return (s16)bestIdx;
        }
        if (h->body.unk38 > pri) {
            return -1;
        }
        return (s16)bestIdx;
    }
    {
        (s) = (SndVoice *)(((s16)bestIdx) * 0xCC + (u32)(p)); (s) = (SndVoice *)((u8 *)(s) + 4);
        m2 = s->mgr;
        s->unkAC = 1;
        s->unk50 = 0;
        if (s->unk10 != m2->unk104) {
            func_00293E60(m2->player, m2->snd[s->idx]);
            if (func_00293580(m2->player) != 0) {
                func_00293EC0(m2->player);
            }
            s->unk4 = -1;
        }
        r = -1;
        if (p->v[16].unkC != r) {
            return r;
        }
        return 16;
    }
}
