#include "common.h"
#include "structs/game_object.h"
#include "globals/enemy.h"
#include "protos/ai.h"



void EnemyAnim_Move_AFCA8(CGameObjectInstance *obj, s32 arg1) {
    AI_PickAnimFromTable((sptr) obj, arg1, sEnemyAnimMove_AFCA8, obj->header.m_pIntelligence->m_Enemy.m_wMoveAnimFlags);
}
