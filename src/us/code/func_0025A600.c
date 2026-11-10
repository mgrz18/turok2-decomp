#include "common.h"
#include "structs/game_object.h"
#include "globals/enemy.h"
#include "protos/ai.h"



void EnemyAnim_Combat_AFE88(CGameObjectInstance *obj, sptr arg1) {
    AI_PickAnimFromTable((sptr) obj, arg1, sEnemyAnimCombat_AFE88, obj->header.m_pIntelligence->m_Enemy.m_wCombatAnimFlags);
}
