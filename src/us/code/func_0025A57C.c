#include "common.h"
#include "structs/game_object.h"

s32 AI_PickAnimFromTable(s32, s32, s32, s32);

extern s32 sEnemyAnimComboEnd_AFDF8;

void EnemyAnim_ComboEnd_AFDF8(CGameObjectInstance *arg0, s32 arg1) {
    AI_PickAnimFromTable((s32) arg0, arg1, (s32) &sEnemyAnimComboEnd_AFDF8, (s32) arg0->header.m_pIntelligence->m_Enemy.m_wComboEndAnimFlags);
}
