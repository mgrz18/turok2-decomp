#include "common.h"
#include "structs/game_object.h"
#include "globals/enemy.h"
#include "protos/ai.h"



void EnemyAnim_Idle_AF750(CGameObjectInstance *obj, sptr arg1) {
    AI_PickAnimFromTable((sptr) obj, arg1, sEnemyAnimIdle_AF750, obj->header.m_pIntelligence->m_Enemy.m_wIdleAnimFlags);
}
