#include "common.h"
#include "structs/game_object.h"

/* AI callback: clears the AI's 0x6C-0x74 words and the actor's flag 0x20000. */
void EnemyStationary_Setup(CGameObjectInstance *obj, CAI *ai) {
    ai->m_vTargetOffset.x = 0;
    ai->m_vTargetOffset.y = 0;
    ai->m_vTargetOffset.z = 0;
    obj->m_dwFlags &= ~0x20000;
}

#include "protos/ai.h"
#include "protos/enemy.h"

f32 EnemyAI_GetModeTurnRate(void *self, CAI *ai);
f32 EnemyAI_GetTypeTurnRate(void *self, CAI *ai);
#ifdef TARGET_PORT
s16 EnemyAI_PickAttack(void *self, CAI *ai, CAITargets *t); /* ABI: returns s16 (the host leaves the upper bits of the return register undefined) */
#else
s32 EnemyAI_PickAttack(void *self, CAI *ai, CAITargets *t);
#endif

void EnemyStationary_Update(void *self, CAI *ai) {
    CAITargets t;
    CAIFrameMotion steer;
    f32 range;
    s32 mode;

    AI_UpdateTargets(self, ai, &t, 1, 1);
    AI_ComputeFrameMotion(self, ai, &steer);
    AI_TurnClamped(self, ai, EnemyAI_GetModeTurnRate(self, ai), t.m_Sight.m_Angle);
    range = EnemyAI_GetTypeTurnRate(self, ai);
    if ((t.m_Sight.m_Angle < 0.0f ? -t.m_Sight.m_Angle : t.m_Sight.m_Angle) <= range) {
        mode = EnemyAI_PickAttack(self, ai, &t);
        if (mode != 0) {
            ai->m_ModeLastFrame = -1;
            CAI__SetMode(self, ai, mode);
        } else {
            CAI__SetMode(self, ai, 10);
        }
    }
}

#include "structs/player.h"

/* Flying AI steering: for index 1, ease the vertical speed towards the wanted
 * one and pitch/roll with it; then turn. Chasing a target in the other
 * behaviour (ground/air), cut the behaviour-switch timer to 0-15 frames. */

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

void EnemyHover_Setup(void *self, CAI *ai) {
    f32 lo = -40.96f;
    f32 hi = 40.96f;

    ai->m_vTargetOffset.x = Random_Blend(lo, hi);
    ai->m_vTargetOffset.y = Random_Blend(lo, hi);
    ai->m_vTargetOffset.z = Random_Blend(lo, hi);
}

#include "globals/engine.h"

f32 Math_DecayToZero(f32, f32);

void EnemyHover_Advance(sptr arg0, CAI *ai) {
    f32 temp_f0;
    f32 temp_f20;

    if ((EnemyAI_GetBehaviorIndex(arg0, (sptr) ai) != 1) || (ai->m_Mode != 4)) {
        temp_f20 = 0.17453295f;
        *(f32 *) &ai->m_VarBuffer[1] = Math_DecayToZero(*(f32 *) &ai->m_VarBuffer[1], temp_f20);
        *(f32 *) &ai->m_VarBuffer[0] = Math_DecayToZero(*(f32 *) &ai->m_VarBuffer[0], temp_f20);
    }
    if (ai->m_Action == 0) {
        temp_f0 = ai->m_ActionTime - gFrameIncrement;
        ai->m_ActionTime = temp_f0;
        if (temp_f0 < 0.0f) {
            ai->m_Action = 1;
        }
    }
}

void EnemyHover_MoveUpdate(void *self, CAI *ai) {
    CAITargets t;
    CAIFrameMotion steer;
    s32 mode;

    AI_UpdateTargets(self, ai, &t, 1, 1);
    AI_ComputeFrameMotion(self, ai, &steer);
    EnemyFly_SteerToPath(self, ai, &t, &steer);
    mode = EnemyAI_PickAttack(self, ai, &t);
    if (mode != 0) {
        ai->m_ModeLastFrame = -1;
        CAI__SetMode(self, ai, mode);
        ai->m_Action = 0;
        ai->m_ActionTime = AI_LerpByAggression(self, ai, 90.0f, 30.0f);
    }
    ai->m_ConditionTimer += gFrameIncrement;
    if (AI_LerpByAggression(self, ai, 60.0f, 120.0f) < ai->m_ConditionTimer) {
        ai->m_ConditionTimer = 0.0f;
        CAI__SetMode(self, ai, 10);
    }
}

#include "m2c_macros.h"

void EnemyHover_IdleEnter(void) {

}

#include "protos/mtxf.h"

/* Flying AI state: keep facing the target; when something is in the way,
 * side-step away from it, otherwise damp the velocity. Fires state 4 when
 * the timer runs out. */

void CVector3__Add(CVector3 *out, CVector3 *a, CVector3 *b);

static __inline__ void sidestep(CGameObjectInstance *self, s32 side) {
    CVector3 push;
    CVector3 dir;
    f32 m[4][4];

    push.x = -(f32)side * 204.79999f;
    push.y = 0.0f;
    push.z = 0.0f;
    CMtxF__SetRotateY(m, self->m_RotY);
    CMtxF__TransformPoint(m, &push, &dir);
    CVector3__Add(&self->m_vVelocity, &self->m_vVelocity, &dir);
}

void EnemyHover_IdleUpdate(CGameObjectInstance *self, CAI *ai) {
    CAITargets t;
    s32 side;

    AI_UpdateTargets(self, ai, &t, 1, 1);
    AI_TurnClamped(self, ai, 0.13962635f, t.m_Sight.m_Angle);
    if (AI_LerpByAggression(self, ai, 60.0f, 30.0f) < ai->m_ModeTime) {
        CAI__SetMode(self, ai, 4);
    }
    if (ai->m_pSightTarget != NULL) {
        side = AI_IsAimedAt(self, ai, ai->m_pSightTarget, 0.3490659f);
        if (side != 0) {
            sidestep(self, side);
        } else {
            self->m_vVelocity.x *= 0.5f;
            self->m_vVelocity.y *= 0.1f;
            self->m_vVelocity.z *= 0.5f;
        }
    }
}

/* AI callback: with a target, pushes the actor sideways (by its yaw) by the
 * side AI_IsAimedAt reports within 20 degrees; else damps its velocity. */
void EnemyAI_Sidestep(CGameObjectInstance *self, CAI *ai) {
    CVector3 push;
    CVector3 dir;
    CMtxF mtx;
    s32 side;

    if (ai->m_pSightTarget == NULL) {
        return;
    }
    side = AI_IsAimedAt(self, ai, ai->m_pSightTarget, 0.3490659f);
    if (side != 0) {
        push.x = -(f32) side * 204.79999f;
        push.y = 0.0f;
        push.z = 0.0f;
        CMtxF__SetRotateY(&mtx, self->m_RotY);
        CMtxF__TransformPoint(&mtx, &push, &dir);
        CVector3__Add(&self->m_vVelocity, &self->m_vVelocity, &dir);
    } else {
        self->m_vVelocity.x *= 0.5f;
        self->m_vVelocity.y *= 0.1f;
        self->m_vVelocity.z *= 0.5f;
    }
}
