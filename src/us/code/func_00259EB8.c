#include "common.h"
#include "structs/game_object.h"
#include "globals/enemy.h"

s32 AI_PickAnimFromTable(s32, s32, s32, s32);


void EnemyAnim_Idle_AF750(CGameObjectInstance *obj, s32 arg1) {
    AI_PickAnimFromTable((s32) obj, arg1, (s32) sEnemyAnimIdle_AF750, obj->header.m_pIntelligence->m_Enemy.m_wIdleAnimFlags);
}
