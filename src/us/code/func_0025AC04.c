#include "common.h"
#include "structs/game_object.h"
#include "globals/ai.h"

s32 AI_PickAnimFromTable(s32, s32, s32, s32);

void EnemyAnim_Move_B0540(CGameObjectInstance *obj, s32 arg1) {
    AI_PickAnimFromTable((s32) obj, arg1, (s32) sEnemyAnimMove_B0540, obj->header.m_pIntelligence->m_Enemy.m_wMoveAnimFlags);
}
