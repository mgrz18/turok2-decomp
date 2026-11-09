#include "common.h"
#include "structs/game_object.h"
#include "globals/enemy.h"
#include "protos/ai.h"


void EnemyAnim_ExplosiveDeath_B0330(CGameObjectInstance *arg0, s32 arg1) {
    AI_PickAnimFromTable((sptr) arg0, arg1, sEnemyAnimExplosiveDeath_B0330, (s32) arg0->header.m_pIntelligence->m_Enemy.m_wExplosiveDeathAnimFlags);
}
