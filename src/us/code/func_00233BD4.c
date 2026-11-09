/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_002A3F94.c, func_002A3F94.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
#include "common.h"
#include "globals/engine.h"
#include "structs/game_object.h"

typedef struct SEnt SEnt;
typedef struct SSlot SSlot;
typedef struct SShape {
               f32 unk0;
               u8 pad4[4];
               f32 unk8;
               s32 unkC;
               u8 pad10[0xC];
               u8 unk1C;
} SShape;
/* structs/swoosh.h SwooshNode, Swoosh: the same layouts. */
struct SEnt {
               SEnt *prev;
               SEnt *next;
               f32 unk8;
               u8 padC[4];
               u8 m10[0xC];
               u8 m1C[0xC];
               u8 m28[2][0x40];
               f32 unkA8;
               f32 unkAC;
               void *unkB0;
               u8 padB4[4];
};
struct SSlot {
               SSlot *prev;
               SSlot *next;
               SShape *shape;
               void *tex0;
               void *tex1;
               s32 texture[2];
               void *owner;
               s32 id;
               f32 unk24;
               f32 unk28;
               f32 unk2C;
               f32 unk30;
               s32 unk34;
               u8 pad38[4];
               u32 flags;
               SEnt *list;
               SEnt *tail;
               s32 unk48;
               f32 unk4C;
               s32 unk50;
};
void SwooshGroup_Remove(sptr a, SSlot *s, SEnt *e);
void func_002368B0(SSlot *s);
s32 Swoosh_Update(sptr a0, SSlot *s) {
    f32 saved, k1, k2;
    SEnt *e, *next;
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
    s->unk24 = s->unk24 - gFrameIncrement;
    if (s->unk24 < 0.0f) {
        s->unk24 = 0.0f;
    }
    e = s->list;
    if (e != 0) {
        k1 = 1.0f;
        k2 = -3.0f;
        do {
            next = e->next;
            if (s->unk24 <= 0.0f && e->unk8 > 0.0f) {
                e->unk8 = 0.0f;
            }
            if (e->unk8 <= 0.0f) {
                e->unk8 = e->unk8 - k1;
                if (e->unk8 < k2) {
                    SwooshGroup_Remove(a0, s, e);
                }
            } else {
                e->unk8 = e->unk8 - gFrameIncrement;
                if (e->unk8 < 0.0f) {
                    e->unk8 = 0.0f;
                }
            }
            e = next;
        } while (e != 0);
    }
    ow = (CGameObjectInstance *)s->owner;
    if (ow != 0 && (s->flags & 1) && !(ow->m_dwFlags & 0x200)) {
        func_002368B0(s);
    }
    gFrameIncrement = saved;
    if (s->unk48 != 0 || (s->unk24 != 0.0f && s->owner != 0)) {
        return 1;
    }
    return 0;
}
