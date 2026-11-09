#include "common.h"
#include "structs/game_object.h"
#include "protos/ai.h"

/* Flying AI steering, scaled by speed: like EnemyFly_SteerToPath, but the pitch and
 * roll ease faster when they have to swing to the other side. */

s32 EnemyAI_GetBehaviorIndex(CGameObjectInstance *self, void *ai);
f32 Math_StepClamped(f32 value, f32 target, f32 step);
f32 Math_Approach(f32 cur, f32 target, f32 rate);

void EnemyFly_SteerToPathScaled(CGameObjectInstance *self, CAI *ai, CAITargets *steer, CAIFrameMotion *limits, f32 speed, f32 turnRate) {
    f32 v;
    f32 max;
    f32 pitch;
    f32 roll;
    f32 pitchSide;
    f32 rollSide;

    AI_TurnClamped(self, ai, turnRate * 0.13962635f, steer->m_Path.m_Angle);
    if (EnemyAI_GetBehaviorIndex(self, ai) == 1) {
        v = Math_StepClamped(self->m_vVelocity.y, steer->m_Path.m_vDelta.y, limits->m_Dist * (speed * 8.0f));
        self->m_vVelocity.y = v;
        max = speed * 102.399994f;
        if (v > max) {
            self->m_vVelocity.y = max;
        } else if (v < -max) {
            self->m_vVelocity.y = -max;
        }
        pitch = self->m_vVelocity.y * 0.008726647f;
        if (pitch > 0.436332345f) {
            pitch = 0.436332345f;
        } else if (pitch < -0.436332345f) {
            pitch = -0.436332345f;
        }
        pitchSide = 1.0f;
        if (pitch < 0.0f) {
            pitchSide = -1.0f;
        }
        if (pitchSide != (ai->m_Enemy.m_Pitch < 0.0f ? -1.0f : 1.0f)) {
            ai->m_Enemy.m_Pitch = Math_Approach(ai->m_Enemy.m_Pitch, pitch, 0.17453295f);
        } else {
            ai->m_Enemy.m_Pitch = Math_Approach(ai->m_Enemy.m_Pitch, pitch, 0.087266475f);
        }
        roll = steer->m_Path.m_Angle * 0.75f;
        if (roll > 0.436332345f) {
            roll = 0.436332345f;
        } else if (roll < -0.436332345f) {
            roll = -0.436332345f;
        }
        rollSide = 1.0f;
        if (roll < 0.0f) {
            rollSide = -1.0f;
        }
        if (rollSide != (ai->m_Enemy.m_Roll < 0.0f ? -1.0f : 1.0f)) {
            ai->m_Enemy.m_Roll = Math_Approach(ai->m_Enemy.m_Roll, roll, 0.3490659f);
        } else {
            ai->m_Enemy.m_Roll = Math_Approach(ai->m_Enemy.m_Roll, roll, 0.17453295f);
        }
    }
}
