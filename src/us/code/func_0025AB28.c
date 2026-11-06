#include "common.h"
#include "structs/game_object.h"
#include "globals/enemy.h"

s32 AI_PickAnimFromTable(s32, s32, s32, s32);

void EnemyAnim_Evade_B0450(CGameObjectInstance *obj, s32 arg1) {
    AI_PickAnimFromTable((s32) obj, arg1, (s32) sEnemyAnimEvade_B0450, obj->header.m_pIntelligence->m_Enemy.m_wEvadeAnimFlags);
}
