#include "common.h"
#include "structs/game_object.h"
#include "globals/enemy.h"
#include "protos/ai.h"


void EnemyAnim_Evade_B0420(CGameObjectInstance *arg0, s32 arg1) {
    AI_PickAnimFromTable((s32) arg0, arg1, (s32) sEnemyAnimEvade_B0420, (s32) arg0->header.m_pIntelligence->m_Enemy.m_wEvadeAnimFlags);
}
