#include "common.h"
#include "structs/game_object.h"
#include "globals/enemy.h"
#include "protos/ai.h"



void EnemyAnim_ComboEnd_AFDC8(CGameObjectInstance *obj, CAI *ai) {
    AI_PickAnimFromTable(obj, ai, (u8 *) sEnemyAnimComboEnd_AFDC8, obj->header.m_pIntelligence->m_Enemy.m_wComboEndAnimFlags);
}
