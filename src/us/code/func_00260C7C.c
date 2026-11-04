#include "common.h"
#include "structs/game_object.h"

typedef struct {
    f32 m[4][4];
} Mtx44;

s32 func_002451AC(CGameObjectInstance *self, CAI *ai, void *target, f32 angle);
void func_002105A4(Mtx44 *mtx, f32 angle);
void func_0020F85C(Mtx44 *mtx, CVector3 *in, CVector3 *out);
void func_0020EEF8(CVector3 *out, CVector3 *a, CVector3 *b);

/* AI callback: with a target, pushes the actor sideways (by its yaw) by the
 * side func_002451AC reports within 20 degrees; else damps its velocity. */
void func_00260C7C(CGameObjectInstance *self, CAI *ai) {
    CVector3 push;
    CVector3 dir;
    Mtx44 mtx;
    s32 side;

    if (ai->m_pSightTarget == NULL) {
        return;
    }
    side = func_002451AC(self, ai, ai->m_pSightTarget, 0.3490659f);
    if (side != 0) {
        push.x = -(f32) side * 204.79999f;
        push.y = 0.0f;
        push.z = 0.0f;
        func_002105A4(&mtx, self->m_RotY);
        func_0020F85C(&mtx, &push, &dir);
        func_0020EEF8(&self->m_vVelocity, &self->m_vVelocity, &dir);
    } else {
        self->m_vVelocity.x *= 0.5f;
        self->m_vVelocity.y *= 0.1f;
        self->m_vVelocity.z *= 0.5f;
    }
}
