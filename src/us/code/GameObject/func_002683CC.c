#include "common.h"
#include "protos/game_object.h"

/* A dead object's collision height: the enemy block's (players and turrets share it), 0 for the
 * other definitions. Round 28 joined the N64 view and the port's (the definition by member; it is
 * asset data at its N64 byte offsets). */
f32 GameObject_GetDeadCollisionHeight(CGameObjectInstance *actor) {
    CIntelligence *def = actor->header.m_pIntelligence;

    switch (def->m_Type) {
        case INTELLIGENCE_ENEMY:
        case INTELLIGENCE_TURRET:
        case INTELLIGENCE_PLAYER:
            return def->m_Enemy.m_CollisionDeadHeight;
    }
    return 0.0f;
}
