/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/WEAPONS/80225D10_10.CPP, func_8022ADBC_de, with Turok 2's layouts. */

#include "structs/game_object.h"
#include "common.h"
#include "structs/intelligence.h"


extern f32 D_800A70FC;          /* 82.944f: default height */
extern f32 D_800A7100;          /* 0.9f */

f32 Actor_GetEyeHeight(CGameObjectInstance *actor) {
    CIntelligence *def = actor->header.m_pIntelligence;

    if (def != 0) {
        return def->m_Enemy.m_CollisionHeight * D_800A7100;
    }
    return D_800A70FC;
}
