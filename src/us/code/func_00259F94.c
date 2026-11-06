#include "common.h"
#include "structs/game_object.h"

s32 AI_PickAnimFromTable(s32, s32, s32, s32);

extern s32 sEnemyAnimTakeCover_AF8D0;

void EnemyAnim_TakeCover_AF8D0(CGameObjectInstance *obj, s32 arg1) {
    AI_PickAnimFromTable((s32) obj, arg1, (s32) &sEnemyAnimTakeCover_AF8D0, obj->header.m_pIntelligence->m_Enemy.m_wTakeCoverAnimFlags);
}
