#include "common.h"
#include "structs/game_object.h"
#include "globals/enemy.h"
#include "protos/ai.h"



void EnemyAnim_Leap_AFEB8(CGameObjectInstance *obj, CAI *ai) {
    AI_PickAnimFromTable(obj, ai, (u8 *) sEnemyAnimLeap_AFEB8, obj->header.m_pIntelligence->m_Enemy.m_wLeapAnimFlags);
}
