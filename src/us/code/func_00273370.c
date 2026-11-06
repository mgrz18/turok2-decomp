/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0025B054.c, func_0025B054.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "structs/sound.h"
#include "rw/junk_types.h" /* SndCore */

void alSndpSetSound(void *p, s32 id);
s32 alSndpGetState(void *p);
void alSndpStop(void *p);
s32 SndVoicePool_FindSlot(SndVoicePool *p, s32 a, s32 b, s16 pri, s32 c) {
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
    v = p->voices;
    lim = best;
    m = p->owner;
    core = (SndCore *)((u8 *)m + 0x7C);
    for (i = 0; i < 16; i++) {
        if (core->snd[i] == -1) {
            return (s16)i;
        }
        if (v->vp.priority < lim) {
            if (!(best < v->vp.priority) && v->vp.priority != 100) {
                best = v->vp.priority;
                bestIdx = i;
            }
        }
        v++;
    }
    if (bestIdx == -1) {
        return -1;
    }
    if (core->snd[bestIdx] == -1) {
        SndNode *h = (SndNode *)m->queue.active.link.next;
        if ((SndLink *)h == &m->queue.active.link) {
            return (s16)bestIdx;
        }
        if (h->body.vp.priority > pri) {
            return -1;
        }
        return (s16)bestIdx;
    }
    {
        (s) = (SndVoice *)(((s16)bestIdx) * 0xCC + (u32)(p)); (s) = (SndVoice *)((u8 *)(s) + 4);
        m2 = s->mgr;
        s->stopping = 1;
        s->vp.follow = 0;
        if (s->vp.frame != m2->frame) {
            alSndpSetSound(&m2->player, m2->handles[s->index]);
            if (alSndpGetState(&m2->player) != 0) {
                alSndpStop(&m2->player);
            }
            s->sound = -1;
        }
        r = -1;
        if (p->voices[16].id != r) {
            return r;
        }
        return 16;
    }
}
