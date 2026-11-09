#include "common.h"
#include "structs/game_object.h"
#include "globals/engine.h"
#include "protos/ai.h"

/* AI state: bob towards the target height on a sine wave, the vertical speed
 * eased and clamped; once level with it, switch to state 0x3C. */

void EnemyMode_DeathMelt_Update(CGameObjectInstance *self, CAI *ai);
void AI_UpdateTargets(CGameObjectInstance *self, CAI *ai, CAITargets *out, s32 f1, s32 f2);
f32 func_002982F0(f32 x); /* sinf */
f32 Math_Approach(f32 cur, f32 target, f32 rate);

void EnemyAI_BobToTargetHeight(CGameObjectInstance *self, CAI *ai) {
    CAITargets t;
    f32 v;

    if (ai->m_Mode == 0x3C) {
        EnemyMode_DeathMelt_Update(self, ai);
    }
    AI_UpdateTargets(self, ai, &t, 1, 0);
    ai->m_ConditionTimer += gFrameIncrement * 15.0f;
    t.m_Path.m_vDelta.y += func_002982F0(ai->m_ConditionTimer * 0.017453294f) * 61.44f + 51.199997f;
    v = Math_Approach(self->m_vVelocity.y, t.m_Path.m_vDelta.y, 10.24f);
    self->m_vVelocity.y = v;
    if (v > 40.96f) {
        self->m_vVelocity.y = 40.96f;
    } else if (v < -40.96f) {
        self->m_vVelocity.y = -40.96f;
    }
    if (t.m_Path.m_vPos.y <= self->header.m_vPos.y + self->m_vVelocity.y) {
        self->m_vVelocity.y = t.m_Path.m_vDelta.y;
        if (ai->m_Mode != 0x3C) {
            CAI__SetMode(self, ai, 0x3C);
        }
    }
}
