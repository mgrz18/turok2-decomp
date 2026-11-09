#include "common.h"
#include "structs/game_object.h"
#include "protos/ai.h"

f32 Math_Approach(f32 cur, f32 target, f32 rate);

void EnemySwim_IdleUpdate(CGameObjectInstance *self, CAI *ai) {
    ai->m_AnimSpeed = Math_Approach(ai->m_AnimSpeed, 1.0f, 0.2f);
    self->m_vVelocity.y = Math_Approach(self->m_vVelocity.y, 0.0f, 1.024f);
    if (ai->m_ModeTime >= ai->m_ConditionTimer) {
        CAI__SetMode(self, ai, 6);
    }
}
