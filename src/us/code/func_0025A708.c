#include "common.h"
#include "structs/game_object.h"
#include "globals/enemy.h"
#include "protos/ai.h"



void EnemyAnim_DartFallback_AFF18(CGameObjectInstance *obj, CAI *ai) {
    AI_PickAnimFromTable(obj, ai, (u8 *) sEnemyAnimLeap_AFF18, obj->header.m_pIntelligence->m_Enemy.m_wLeapAnimFlags);
}
