#include "common.h"
#include "structs/game_object.h"

s32 AI_PickAnimFromTable(s32, s32, s32, s32);

extern s32 sEnemyAnimNormalDeath_B0180;

void EnemyAnim_NormalDeath_B0180(CGameObjectInstance *obj, s32 arg1) {
    AI_PickAnimFromTable((s32) obj, arg1, (s32) &sEnemyAnimNormalDeath_B0180, obj->header.m_pIntelligence->m_Enemy.m_wNormalDeathAnimFlags);
}
