#include "common.h"
#include "structs/game_object.h"

void AI_UpdateTargets(CGameObjectInstance *self, CAI *ai, CAITargets *out, s32 f1, s32 f2);
void AI_ComputeFrameMotion(CGameObjectInstance *self, CAI *ai, CAIFrameMotion *out);
f32 Math_Approach(f32 cur, f32 target, f32 rate);
f32 Math_StepClamped(f32 value, f32 target, f32 rate);
void AI_TurnClamped(CGameObjectInstance *self, CAI *ai, f32 turnRate, f32 angle);
void CAI__SetMode(CGameObjectInstance *self, CAI *ai, s32 mode);

void EnemySwim_FleeUpdate(CGameObjectInstance *self, CAI *ai) {
    CAITargets t;
    CAIFrameMotion steer;
    CAITargets *tp;

    ai->m_Enemy.m_TurnRate = Math_Approach(ai->m_Enemy.m_TurnRate, 0.20943953f, 0.25f);
    ai->m_AnimSpeed = Math_Approach(ai->m_AnimSpeed, 6.0f, 0.25f);
    AI_UpdateTargets(self, ai, &t, 1, 1);
    AI_ComputeFrameMotion(self, ai, &steer);

    tp = &t;
    self->m_vVelocity.y = Math_StepClamped(self->m_vVelocity.y, tp->m_Path.m_vDelta.y, steer.m_Dist * 2.0f);
    if (self->m_vVelocity.y > 81.92f) {
        self->m_vVelocity.y = 81.92f;
    } else if (self->m_vVelocity.y < -81.92f) {
        self->m_vVelocity.y = -81.92f;
    }
    AI_TurnClamped(self, ai, ai->m_Enemy.m_TurnRate, tp->m_Path.m_Angle);

    if (ai->m_ModeTime > 15.0f && (t.m_Path.m_FlatDist < 81.92f || ai->m_ModeTime > 90.0f)) {
        CAI__SetMode(self, ai, 4);
    }
}
