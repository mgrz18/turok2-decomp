#include "common.h"
#include "structs/game_object.h"

/* Flying AI steering: for index 1, ease the vertical speed towards the wanted
 * one and pitch/roll with it; then turn. Chasing a target in the other
 * behaviour (ground/air), cut the behaviour-switch timer to 0-15 frames. */

s32 func_0025E428(CGameObjectInstance *self, void *ai);
f32 func_0021160C(f32 value, f32 target, f32 step);
f32 func_00211774(f32 cur, f32 target, f32 rate);
void func_00245BAC(CGameObjectInstance *self, CAI *ai, f32 angle, f32 turn);
f32 func_002119FC(f32 lo, f32 hi);

void func_00260730(CGameObjectInstance *self, CAI *ai, CAITargets *steer, CAIFrameMotion *limits) {
    CGameObjectInstance *target = steer->m_Path.m_pTarget;
    f32 v;
    f32 pitch;
    f32 roll;

    if (func_0025E428(self, ai) == 1) {
        v = func_0021160C(self->m_vVelocity.y, steer->m_Path.m_vDelta.y, limits->m_Dist * 8.0f);
        self->m_vVelocity.y = v;
        if (v > 81.92f) {
            self->m_vVelocity.y = 81.92f;
        } else if (v < -81.92f) {
            self->m_vVelocity.y = -81.92f;
        }
        pitch = -self->m_vVelocity.y * 0.030543264f;
        if (pitch > 0.436332345f) {
            pitch = 0.436332345f;
        } else if (pitch < -0.436332345f) {
            pitch = -0.436332345f;
        }
        ai->m_Enemy.m_Pitch = func_00211774(ai->m_Enemy.m_Pitch, pitch, 0.087266475f);
        roll = steer->m_Path.m_Angle * 0.75f;
        if (roll > 0.436332345f) {
            roll = 0.436332345f;
        } else if (roll < -0.436332345f) {
            roll = -0.436332345f;
        }
        ai->m_Enemy.m_Roll = func_00211774(ai->m_Enemy.m_Roll, roll, 0.17453295f);
    }
    func_00245BAC(self, ai, 0.13962635f, steer->m_Path.m_Angle);
    if (steer->m_Path.m_Kind == 2 && target != NULL && ai->m_Enemy.m_IdleTimer > 15.0f) {
        if (func_0025E428(self, ai) != func_0025E428(target, (u8 *)target + 0x140)) {
            ai->m_Enemy.m_IdleTimer = func_002119FC(0.0f, 15.0f);
        }
    }
}
