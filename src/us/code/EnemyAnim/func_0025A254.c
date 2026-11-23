#include "common.h"
#include "structs/game_object.h"
#include "globals/enemy.h"
#include "protos/ai.h"



void EnemyAnim_Move_AFCD8(CGameObjectInstance *obj, CAI *ai) {
    AI_PickAnimFromTable(obj, ai, (u8 *) sEnemyAnimMove_AFCD8, obj->header.m_pIntelligence->m_Enemy.m_wMoveAnimFlags);
}
