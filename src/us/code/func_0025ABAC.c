#include "common.h"
#include "structs/game_object.h"

s32 AI_PickAnimFromTable(s32, s32, s32, s32);

extern s32 sEnemyAnimEvade_B04E0;

void EnemyAnim_Evade_B04E0(CGameObjectInstance *arg0, s32 arg1) {
    AI_PickAnimFromTable((s32) arg0, arg1, (s32) &sEnemyAnimEvade_B04E0, (s32) arg0->header.m_pIntelligence->m_Enemy.m_wEvadeAnimFlags);
}
