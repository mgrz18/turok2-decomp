#include "common.h"
#include "structs/game_object.h"
#include "structs/intelligence.h"

/* Enemies only: the enemy definition's m_AttackCombatRadius. Round 22 rewrote the m2c draft (two equal arms or a dead
 * do-while, an unmarked fake) as the getters round 21 de-faked: the case that returns the
 * field is tested first. */
f32 GameObject_GetAttackCombatRadius(CGameObjectInstance *obj) {
    CIntelligence *def = obj->header.m_pIntelligence;

    if (def->m_Type == 1) {
        return def->m_Enemy.m_AttackCombatRadius;
    }
    return 0.0f;
}
