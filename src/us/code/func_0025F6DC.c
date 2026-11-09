#include "common.h"
#include "structs/game_object.h"
#include "protos/ai.h"


void EnemyMode_GroundToUnderwater_Update(s32 arg0, CAI *arg1) {
    arg1->m_Enemy.m_BehaviorIndex = 2;
    if (arg1->m_AnimRepeatedCount != 0) {
        CAI__SetMode(arg0, arg1, 4);
    }
}
