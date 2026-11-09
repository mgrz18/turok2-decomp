#include "common.h"
#include "structs/game_object.h"
#include "globals/enemy.h"
#include "protos/ai.h"



void EnemyAnim_Patrol_AFA50(CGameObjectInstance *arg0, s32 arg1) {
    AI_PickAnimFromTable((sptr) arg0, arg1, sEnemyAnimPatrol_AFA50, (s32) arg0->header.m_pIntelligence->m_Enemy.m_wPatrolAnimFlags);
}
