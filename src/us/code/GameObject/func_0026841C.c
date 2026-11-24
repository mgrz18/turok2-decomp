/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/SOUND/8024E130_3.CPP, func_8024E610_de, with Turok 2's layouts. */

#include "common.h"
#include "structs/game_object.h"

/* An object's collision y offset; scenery (definition type 0) keeps it in its definition.
 * Round 28 joined the N64 view and the port's host-offset one on CGameObjectInstance. */
f32 GameObject_GetCollisionYOffset(CGameObjectInstance *item) {
    CIntelligence *def;

    if (item->header.m_Type == 1) {
        return item->m_CollisionYOffset;
    }
    def = item->header.m_pIntelligence;
    if (def->m_Type == INTELLIGENCE_STATIC) {
        return def->m_Static.m_CollisionHeightOffset;
    }
    return 0.0f;
}
