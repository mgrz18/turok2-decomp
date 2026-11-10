#include "common.h"
#include "structs/player.h"
#include "structs/game_object.h"
#include "protos/ai.h"

/* Flying AI steering: for index 1, ease the vertical speed towards the wanted
 * one and pitch/roll with it; then turn. Chasing a target in the other
 * behaviour (ground/air), cut the behaviour-switch timer to 0-15 frames. */

#ifdef TARGET_PORT
s8 EnemyAI_GetBehaviorIndex(CGameObjectInstance *self, void *ai); /* ABI: returns s8 (the host leaves the upper bits of the return register undefined) */
#else
s32 EnemyAI_GetBehaviorIndex(CGameObjectInstance *self, void *ai);
#endif
f32 Math_StepClamped(f32 value, f32 target, f32 step);
f32 Math_Approach(f32 cur, f32 target, f32 rate);
f32 Random_Blend(f32 lo, f32 hi);

void EnemyFly_SteerToPath(CGameObjectInstance *self, CAI *ai, CAITargets *steer, CAIFrameMotion *limits) {
    CGameObjectInstance *target = steer->m_Path.m_pTarget;
    f32 v;
    f32 pitch;
    f32 roll;

    if (EnemyAI_GetBehaviorIndex(self, ai) == 1) {
        v = Math_StepClamped(self->m_vVelocity.y, steer->m_Path.m_vDelta.y, limits->m_Dist * 8.0f);
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
        ai->m_Enemy.m_Pitch = Math_Approach(ai->m_Enemy.m_Pitch, pitch, 0.087266475f);
        roll = steer->m_Path.m_Angle * 0.75f;
        if (roll > 0.436332345f) {
            roll = 0.436332345f;
        } else if (roll < -0.436332345f) {
            roll = -0.436332345f;
        }
        ai->m_Enemy.m_Roll = Math_Approach(ai->m_Enemy.m_Roll, roll, 0.17453295f);
    }
    AI_TurnClamped(self, ai, 0.13962635f, steer->m_Path.m_Angle);
    if (steer->m_Path.m_Kind == 2 && target != NULL && ai->m_Enemy.m_IdleTimer > 15.0f) {
        if (EnemyAI_GetBehaviorIndex(self, ai) != EnemyAI_GetBehaviorIndex(target, (u8 *)target + T2_OFFSETOF(CGameObjectInstance, m_AI))) {
            ai->m_Enemy.m_IdleTimer = Random_Blend(0.0f, 15.0f);
        }
    }
}
