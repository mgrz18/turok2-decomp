#include "common.h"
#include "structs/game_object.h"
#include "globals/enemy.h"
#include "protos/ai.h"



void EnemyAnim_Patrol_AFAB0(CGameObjectInstance *obj, s32 arg1) {
    AI_PickAnimFromTable((sptr) obj, arg1, sEnemyAnimPatrol_AFAB0, obj->header.m_pIntelligence->m_Enemy.m_wPatrolAnimFlags);
}
