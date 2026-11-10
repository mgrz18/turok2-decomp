#include "common.h"
#include "structs/game_object.h"
#include "globals/enemy.h"
#include "protos/ai.h"



void EnemyAnim_Idle_AF6F0(CGameObjectInstance *arg0, sptr arg1) {
    AI_PickAnimFromTable((sptr) arg0, arg1, sEnemyAnimIdle_AF6F0, (s32) arg0->header.m_pIntelligence->m_Enemy.m_wIdleAnimFlags);
}
