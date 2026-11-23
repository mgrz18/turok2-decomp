#include "common.h"
#include "structs/game_object.h"

s32 AI_IsAimedAt(CGameObjectInstance *self, CAI *ai, void *target, f32 angle);
void CMtxF__SetRotateY(CMtxF *mtx, f32 angle);
void CMtxF__TransformPoint(CMtxF *mtx, CVector3 *in, CVector3 *out);
void CVector3__Add(CVector3 *out, CVector3 *a, CVector3 *b);

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
