#include "common.h"
#include "structs/game_object.h"
#include "globals/enemy.h"
#include "protos/ai.h"



void EnemyAnim_TakeCover_AF8D0(CGameObjectInstance *obj, sptr arg1) {
    AI_PickAnimFromTable((sptr) obj, arg1, sEnemyAnimTakeCover_AF8D0, obj->header.m_pIntelligence->m_Enemy.m_wTakeCoverAnimFlags);
}
