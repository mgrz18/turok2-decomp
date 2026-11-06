#include "common.h"
#include "structs/game_object.h"
#include "globals/engine.h"


f32 Math_Approach(f32 cur, f32 target, f32 rate);
f32 func_002974C0(f32 x);
f32 Random_Blend(f32 lo, f32 hi);

void EnemySwim_Advance(CGameObjectInstance *self, CAI *ai) {
    f32 pitch;
    f32 base;
    f32 c;

    pitch = -self->m_vVelocity.y * 0.030543264f;
    if (pitch > 1.308997f) {
        pitch = 1.308997f;
    } else if (pitch < -1.308997f) {
        pitch = -1.308997f;
    }
    ai->m_Enemy.m_Pitch = Math_Approach(ai->m_Enemy.m_Pitch, pitch, 0.06981318f);

    if (ai->m_Health != 0) {
        if (ai->m_Mode == 27) {
            c = func_002974C0(ai->m_ModeTime * 0.31415930f);
            base = ai->m_Enemy.m_BaseScale;
            base += (c + 1.0f) * ((base * 1.25f - base) * 0.5f);
            self->m_vScale.x = Math_Approach(self->m_vScale.x, base * 3.0f * 2.0f, 0.1f);
            self->m_vScale.y = Math_Approach(self->m_vScale.y, base * 1.25f * 2.0f, 0.1f);
            self->m_vScale.z = Math_Approach(self->m_vScale.z, base * 1.25f * 2.0f, 0.1f);
        } else {
            self->m_vScale.x = Math_Approach(self->m_vScale.x, ai->m_Enemy.m_BaseScale, 0.1f);
            self->m_vScale.y = Math_Approach(self->m_vScale.y, ai->m_Enemy.m_BaseScale, 0.1f);
            self->m_vScale.z = Math_Approach(self->m_vScale.z, ai->m_Enemy.m_BaseScale, 0.1f);
        }
    }

    ai->m_Enemy.m_WanderTimer -= gFrameIncrement;
    if (ai->m_Enemy.m_WanderTimer <= 0.0f) {
        ai->m_Enemy.m_WanderTimer = Random_Blend(60.0f, 120.0f);
        ai->m_vTargetOffset.x = Random_Blend(-51.199997f, 51.199997f);
        ai->m_vTargetOffset.y = Random_Blend(-51.199997f, 51.199997f);
        ai->m_vTargetOffset.z = Random_Blend(-51.199997f, 51.199997f);
    }
}
