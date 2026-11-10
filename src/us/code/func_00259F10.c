#include "common.h"
#include "structs/game_object.h"
#include "globals/enemy.h"
#include "protos/ai.h"



void EnemyAnim_Alert_AF840(CGameObjectInstance *arg0, sptr arg1) {
    AI_PickAnimFromTable((sptr) arg0, arg1, sEnemyAnimAlert_AF840, (s32) arg0->header.m_pIntelligence->m_Enemy.m_wAlertAnimFlags);
}
