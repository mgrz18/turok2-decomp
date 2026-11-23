#include "common.h"
#include "structs/game_object.h"
#include "protos/ai.h"


void EnemyMode_AirToGround_Update(CGameObjectInstance *obj, CAI *ai) {
    ai->m_Enemy.m_BehaviorIndex = 0;
    if (ai->m_AnimRepeatedCount != 0) {
        CAI__SetMode(obj, ai, 4);
    }
}
