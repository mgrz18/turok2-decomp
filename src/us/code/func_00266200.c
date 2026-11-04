/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0020459C.c, func_0020459C.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/intelligence.h"

typedef struct Actor {
    char pad0[0x14];
    CIntelligence *def; /* 0x14 (0x18 in Rage Wars) */
    char pad18[0xBC];
    u32 flags;     /* 0xD4 (0x100 in Rage Wars) */
} Actor;

s32 func_00266200(Actor *actor) {
    s32 v = actor->def->m_Enemy.unk0C;
    if (v != 0) {
        actor->flags |= 0x2000;
    } else {
        actor->flags &= ~0x2000;
    }
    return v;
}
