#include "common.h"
#include "structs/game_object.h"
#include "globals/enemy.h"
#include "protos/ai.h"


void EnemyAnim_Move_B0570(CGameObjectInstance *obj, CAI *ai) {
    AI_PickAnimFromTable(obj, ai, sEnemyAnimMove_B0570, obj->header.m_pIntelligence->m_Enemy.m_wMoveAnimFlags);
}
