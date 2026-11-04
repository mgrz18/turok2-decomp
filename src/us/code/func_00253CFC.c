/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/WEAPONS/80225D10_10.CPP, func_8022ADBC_de, with Turok 2's layouts. */

#include "common.h"
#include "structs/intelligence.h"

typedef struct Actor {
    char pad0[0x14];
    CIntelligence *def;              /* 0x14 (0x18 in Rage Wars) */
} Actor;

extern f32 D_800A70FC;          /* 82.944f: default height */
extern f32 D_800A7100;          /* 0.9f */

f32 func_00253CFC(Actor *actor) {
    CIntelligence *def = actor->def;

    if (def != 0) {
        return def->m_Enemy.m_CollisionHeight * D_800A7100;
    }
    return D_800A70FC;
}
