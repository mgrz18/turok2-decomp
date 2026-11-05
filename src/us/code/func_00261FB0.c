#include "common.h"
#include "structs/game_object.h"

void func_00246108(CGameObjectInstance *self, CAI *ai, CAITargets *out, s32 f1, s32 f2);
void func_0024599C(CGameObjectInstance *self, CAI *ai, CAIFrameMotion *out);
f32 func_00211774(f32 cur, f32 target, f32 rate);
f32 func_0021160C(f32 value, f32 target, f32 rate);
void func_00245BAC(CGameObjectInstance *self, CAI *ai, f32 turnRate, f32 angle);

void func_00261FB0(CGameObjectInstance *self, CAI *ai) {
    CAITargets t;
    CAIFrameMotion steer;
    CAITargets *tp;

    ai->m_vTargetOffset.x = 0.0f;
    ai->m_vTargetOffset.z = 0.0f;
    ai->m_AnimSpeed = func_00211774(ai->m_AnimSpeed, 6.0f, 0.2f);
    ai->m_Enemy.m_TurnRate = func_00211774(ai->m_Enemy.m_TurnRate, 0.20943953f, 0.5f);
    func_00246108(self, ai, &t, 1, 1);
    func_0024599C(self, ai, &steer);

    tp = &t;
    self->m_vVelocity.y = func_0021160C(self->m_vVelocity.y, tp->m_Path.m_vDelta.y, steer.m_Dist * 2.0f);
    if (self->m_vVelocity.y > 81.92f) {
        self->m_vVelocity.y = 81.92f;
    } else if (self->m_vVelocity.y < -81.92f) {
        self->m_vVelocity.y = -81.92f;
    }
    func_00245BAC(self, ai, ai->m_Enemy.m_TurnRate, tp->m_Path.m_Angle);
}
