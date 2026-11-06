#include "common.h"
#include "structs/game_object.h"

f32 Math_Approach(f32 cur, f32 target, f32 rate);
void CAI__SetMode(CGameObjectInstance *self, CAI *ai, s32 mode);

void EnemySwim_PuffUpUpdate(CGameObjectInstance *self, CAI *ai) {
    ai->m_AnimSpeed = Math_Approach(ai->m_AnimSpeed, 0.25f, 0.2f);
    self->m_vVelocity.y = Math_Approach(self->m_vVelocity.y, 0.0f, 1.024f);
    if (ai->m_ModeTime >= ai->m_ConditionTimer) {
        self->m_dwFlags |= 0x20000;
        CAI__SetMode(self, ai, 6);
    }
}
