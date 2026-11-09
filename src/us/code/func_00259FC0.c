#include "common.h"
#include "structs/game_object.h"
#include "globals/enemy.h"
#include "protos/ai.h"



void EnemyAnim_TakeCover_AF900(CGameObjectInstance *obj, CAI *ai) {
    AI_PickAnimFromTable(obj, ai, (u8 *) sEnemyAnimTakeCover_AF900, obj->header.m_pIntelligence->m_Enemy.m_wTakeCoverAnimFlags);
}
