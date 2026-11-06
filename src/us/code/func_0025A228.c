#include "common.h"
#include "structs/game_object.h"

s32 AI_PickAnimFromTable(s32, s32, s32, s32);

extern s32 sEnemyAnimMove_AFCA8;

void EnemyAnim_Move_AFCA8(CGameObjectInstance *obj, s32 arg1) {
    AI_PickAnimFromTable((s32) obj, arg1, (s32) &sEnemyAnimMove_AFCA8, obj->header.m_pIntelligence->m_Enemy.m_wMoveAnimFlags);
}
