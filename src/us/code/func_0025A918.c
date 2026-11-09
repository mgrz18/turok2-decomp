#include "common.h"
#include "structs/game_object.h"
#include "globals/enemy.h"
#include "protos/ai.h"


void EnemyAnim_MovingDeath_B0210(CGameObjectInstance *arg0, s32 arg1) {
    AI_PickAnimFromTable((s32) arg0, arg1, (s32) sEnemyAnimMovingDeath_B0210, (s32) arg0->header.m_pIntelligence->m_Enemy.m_wMovingDeathAnimFlags);
}
