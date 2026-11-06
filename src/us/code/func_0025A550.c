#include "common.h"
#include "structs/game_object.h"

s32 AI_PickAnimFromTable(CGameObjectInstance *obj, CAI *ai, u8 *table, s32 anim);

extern u8 sEnemyAnimComboEnd_AFDC8[];

void EnemyAnim_ComboEnd_AFDC8(CGameObjectInstance *obj, CAI *ai) {
    AI_PickAnimFromTable(obj, ai, sEnemyAnimComboEnd_AFDC8, obj->header.m_pIntelligence->m_Enemy.m_wComboEndAnimFlags);
}
