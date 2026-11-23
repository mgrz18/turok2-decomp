/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_002A3F94.c, func_002A3F94.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py.
 * On structs/swoosh.h's Swoosh and SwooshNode. */
#include "common.h"
#include "globals/engine.h"
#include "structs/game_object.h"
#include "structs/swoosh.h"


void SwooshGroup_Remove(sptr a, Swoosh *s, SwooshNode *e);
void Swoosh_ReleaseOwner(Swoosh *s);
s32 Swoosh_Update(sptr a0, Swoosh *s) {
    f32 saved, k1, k2;
    SwooshNode *e, *next;
    CGameObjectInstance *ow;
    s32 r;
    saved = gFrameIncrement;
    if (s->flags & 2) {
        gFrameIncrement = saved * gParticleTimeScale;
    }
    if (s->flags & 1) {
        gFrameIncrement = gFrameIncrement * gEnemyTimeScale;
    }
    if (s->flags & 8) {
        s->unk50 = s->unk50 + 1;
        if (s->unk50 >= 3) {
            s->owner = 0;
        }
    }
    if (s->unk2C >= 0.0f) {
        s->unk30 = s->unk30 + gFrameIncrement;
    }
    s->time = s->time - gFrameIncrement;
    if (s->time < 0.0f) {
        s->time = 0.0f;
    }
    e = s->nodes.head;
    if (e != 0) {
        k1 = 1.0f;
        k2 = -3.0f;
        do {
            next = e->next;
            if (s->time <= 0.0f && e->life > 0.0f) {
                e->life = 0.0f;
            }
            if (e->life <= 0.0f) {
                e->life = e->life - k1;
                if (e->life < k2) {
                    SwooshGroup_Remove(a0, s, e);
                }
            } else {
                e->life = e->life - gFrameIncrement;
                if (e->life < 0.0f) {
                    e->life = 0.0f;
                }
            }
            e = next;
        } while (e != 0);
    }
    ow = (CGameObjectInstance *)s->owner;
    if (ow != 0 && (s->flags & 1) && !(ow->m_dwFlags & 0x200)) {
        Swoosh_ReleaseOwner(s);
    }
    gFrameIncrement = saved;
    if (s->nodes.count != 0 || (s->time != 0.0f && s->owner != 0)) {
        return 1;
    }
    return 0;
}
