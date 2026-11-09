#include "common.h"
#include "structs/game_object.h"
#include "globals/enemy.h"
#include "protos/ai.h"



void EnemyAnim_Idle_AF720(CGameObjectInstance *arg0, s32 arg1) {
    AI_PickAnimFromTable((sptr) arg0, arg1, sEnemyAnimIdle_AF720, (s32) arg0->header.m_pIntelligence->m_Enemy.m_wIdleAnimFlags);
}
