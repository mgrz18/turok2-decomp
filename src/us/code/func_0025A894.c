#include "common.h"
#include "structs/game_object.h"
#include "globals/enemy.h"
#include "protos/ai.h"


void EnemyAnim_NormalDeath_B0180(CGameObjectInstance *obj, sptr arg1) {
    AI_PickAnimFromTable((sptr) obj, arg1, sEnemyAnimNormalDeath_B0180, obj->header.m_pIntelligence->m_Enemy.m_wNormalDeathAnimFlags);
}
