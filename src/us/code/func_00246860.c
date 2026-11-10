#include "structs/vector.h"
#include "protos/game.h"

/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_002172D0.c, func_002172D0.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef struct SndSrc {
    u8 type;
    u8 pad1[3];
    CVector3 pos;              /* 0x04 (0x08 in Rage Wars) */
    u8 pad10[0x80 - 0x10];
    void *unk80;            /* 0x80 (0xD0) */
} SndSrc;
typedef struct SndHandle {
    u8 pad0[0xC];
    s32 unkC;
} SndHandle;
typedef struct SndOwner {
    u8 pad0[0xEC];
    SndHandle *sound;       /* 0xEC (0xFC in Rage Wars) */
} SndOwner;
void *LoopSounds_Start(sptr a, s32 b, CVector3 *c, CVector3 *d, void *e);
void LoopSounds_Stop(sptr a, SndHandle *h);
void AI_StartSound(SndSrc *src, SndOwner *owner, s32 id) {
    void *s0 = (void *)-1;
    if (owner->sound != ((void *)0)) {
        if (owner->sound->unkC == id) {
            return;
        }
        LoopSounds_Stop(LoopSounds_Get(), owner->sound);
    }
    switch (src->type) {
    case 1:
    case 2:
        s0 = src;
        break;
    case 0:
        s0 = src->unk80;
        break;
    }
    owner->sound = LoopSounds_Start(LoopSounds_Get(), id, &src->pos, &src->pos, s0);
}
