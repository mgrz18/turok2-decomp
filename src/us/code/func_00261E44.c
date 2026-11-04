#include "common.h"
#include "structs/game_object.h"

f32 func_00211774(f32 cur, f32 target, f32 rate);
void func_00243414(CGameObjectInstance *self, CAI *ai, s32 mode);

void func_00261E44(CGameObjectInstance *self, CAI *ai) {
    ai->m_AnimSpeed = func_00211774(ai->m_AnimSpeed, 1.0f, 0.2f);
    self->m_vVelocity.y = func_00211774(self->m_vVelocity.y, 0.0f, 1.024f);
    if (ai->m_ModeTime >= ai->m_ConditionTimer) {
        func_00243414(self, ai, 6);
    }
}
