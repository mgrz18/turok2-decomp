#include "common.h"
#include "structs/game_object.h"
#include "globals/ai.h"

s32 AI_PickAnimFromTable(s32, s32, s32, s32);

void EnemyAnim_NormalDeath_B03C0(CGameObjectInstance *arg0, s32 arg1) {
    AI_PickAnimFromTable((s32) arg0, arg1, (s32) sEnemyAnimNormalDeath_B03C0, (s32) arg0->header.m_pIntelligence->m_Enemy.m_wNormalDeathAnimFlags);
}
