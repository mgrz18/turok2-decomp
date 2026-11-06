/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/WEAPONS/80225D10_10.CPP, func_8022ADBC_de, with Turok 2's layouts. */

#include "structs/game_object.h"
#include "common.h"
#include "structs/intelligence.h"



f32 Actor_GetEyeHeight(CGameObjectInstance *actor) {
    CIntelligence *def = actor->header.m_pIntelligence;

    if (def != 0) {
        return def->m_Enemy.m_CollisionHeight * 0.9f;
    }
    return 82.94399f;
}
