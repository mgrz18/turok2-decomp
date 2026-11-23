#include "common.h"
#include "structs/game_object.h"
#include "globals/enemy.h"
#include "protos/ai.h"



void EnemyAnim_RightArmWound_AF9C0(CGameObjectInstance *obj, sptr arg1) {
    AI_PickAnimFromTable((sptr) obj, arg1, sEnemyAnimRightArmWound_AF9C0, obj->header.m_pIntelligence->m_Enemy.m_wRightArmWoundAnimFlags);
}
