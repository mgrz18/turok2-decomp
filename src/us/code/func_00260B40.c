#include "common.h"
#include "structs/game_object.h"

/* Flying AI state: keep facing the target; when something is in the way,
 * side-step away from it, otherwise damp the velocity. Fires state 4 when
 * the timer runs out. */

void AI_UpdateTargets(CGameObjectInstance *self, CAI *ai, CAITargets *out, s32 f1, s32 f2);
void AI_TurnClamped(CGameObjectInstance *self, CAI *ai, f32 angle, f32 arg3);
f32 AI_LerpByAggression(CGameObjectInstance *self, CAI *ai, f32 lo, f32 hi);
void CAI__SetMode(CGameObjectInstance *self, CAI *ai, s32 mode);
s32 AI_IsAimedAt(CGameObjectInstance *self, CAI *ai, CGameObjectInstance *target, f32 angle);
void CMtxF__SetRotateY(f32 m[4][4], f32 angle);
void CMtxF__TransformPoint(f32 m[4][4], CVector3 *in, CVector3 *out);
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
