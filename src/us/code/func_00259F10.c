#include "common.h"
#include "structs/game_object.h"
#include "globals/enemy.h"

s32 AI_PickAnimFromTable(s32, s32, s32, s32);


void EnemyAnim_Alert_AF840(CGameObjectInstance *arg0, s32 arg1) {
    AI_PickAnimFromTable((s32) arg0, arg1, (s32) sEnemyAnimAlert_AF840, (s32) arg0->header.m_pIntelligence->m_Enemy.m_wAlertAnimFlags);
}
