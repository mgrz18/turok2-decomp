#include "common.h"
#include "structs/game_object.h"

s32 CAI__SetMode(CGameObjectInstance *obj, CAI *ai, s32 mode);

void EnemyMode_UnderwaterToAir_Update(CGameObjectInstance *obj, CAI *ai) {
    ai->m_Enemy.m_BehaviorIndex = 1;
    if (ai->m_AnimRepeatedCount != 0) {
        CAI__SetMode(obj, ai, 4);
    }
}
