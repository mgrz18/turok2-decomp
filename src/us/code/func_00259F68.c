#include "common.h"
#include "structs/game_object.h"

s32 AI_PickAnimFromTable(s32, s32, s32, s32);

extern s32 sEnemyAnimTakeCover_AF8A0;

void EnemyAnim_TakeCover_AF8A0(CGameObjectInstance *arg0, s32 arg1) {
    AI_PickAnimFromTable((s32) arg0, arg1, (s32) &sEnemyAnimTakeCover_AF8A0, (s32) arg0->header.m_pIntelligence->m_Enemy.m_wTakeCoverAnimFlags);
}
