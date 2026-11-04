/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0025B940.c, func_0025B940.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "structs/sound.h"

void func_00293E60(void *p, s32 id);
s32 func_00293580(void *p);
void func_00293EC0(void *p);
s32 func_00273C5C(SndVoicePool *p, s16 tag, s16 key) {
    s32 i;
    s32 j;
    SndVoice *v = p->voices;
    SndVoice *s;
    SndMgr *m;
    i = 0;
    for (; i < 16; i++) {
        if (v->unk8 != -1 && p->owner->pendingCue != i && v->soundId == tag && (v->flags & 0x40) != 0 && v->vp.channel == key) {
            j = (s16)i;
            (s) = (SndVoice *)((j) * 0xCC + (u32)(p)); (s) = (SndVoice *)((u8 *)(s) + 4);
            m = s->mgr;
            s->stopping = 1;
            s->vp.follow = 0;
            if (s->vp.frame != m->frame) {
                func_00293E60(&m->player, m->handles[s->index]);
                if (func_00293580(&m->player) != 0) {
                    func_00293EC0(&m->player);
                }
                s->sound = -1;
            }
            return (s16)i;
        }
        v++;
    }
    return -1;
}
