#include "common.h"
#include "structs/game_object.h"
#include "globals/enemy.h"

s32 AI_PickAnimFromTable(CGameObjectInstance *obj, CAI *ai, u8 *table, s32 anim);


void EnemyAnim_Move_AFAE0(CGameObjectInstance *obj, CAI *ai) {
    AI_PickAnimFromTable(obj, ai, (u8 *) sEnemyAnimMove_AFAE0, obj->header.m_pIntelligence->m_Enemy.m_wMoveAnimFlags);
}
