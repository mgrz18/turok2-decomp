#include "common.h"
#include "structs/game_object.h"
#include "globals/enemy.h"

s32 AI_PickAnimFromTable(CGameObjectInstance *obj, CAI *ai, u8 *table, s32 anim);


void EnemyAnim_Alert_AF810(CGameObjectInstance *obj, CAI *ai) {
    AI_PickAnimFromTable(obj, ai, (u8 *) sEnemyAnimAlert_AF810, obj->header.m_pIntelligence->m_Enemy.m_wAlertAnimFlags);
}
