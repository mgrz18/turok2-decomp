#include "common.h"
#include "structs/game_object.h"
#include "globals/enemy.h"
#include "protos/ai.h"


void EnemyAnim_ViolentDeath_B0240(CGameObjectInstance *arg0, s32 arg1) {
    AI_PickAnimFromTable((s32) arg0, arg1, (s32) sEnemyAnimViolentDeath_B0240, (s32) arg0->header.m_pIntelligence->m_Enemy.m_wViolentDeathAnimFlags);
}
