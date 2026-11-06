#include "common.h"
#include "structs/game_object.h"
#include "globals/ai.h"

s32 AI_PickAnimFromTable(s32, s32, s32, s32);

void EnemyAnim_Evade_B03F0(CGameObjectInstance *arg0, s32 arg1) {
    AI_PickAnimFromTable((s32) arg0, arg1, (s32) sEnemyAnimEvade_B03F0, (s32) arg0->header.m_pIntelligence->m_Enemy.m_wEvadeAnimFlags);
}
