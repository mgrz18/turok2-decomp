#include "common.h"
#include "structs/game_object.h"

/* Flying AI state: keep facing the target; when something is in the way,
 * side-step away from it, otherwise damp the velocity. Fires state 4 when
 * the timer runs out. */

typedef struct {
    f32 m[4][4];
} Mtx4;

void func_00246108(CGameObjectInstance *self, CAI *ai, CAITargets *out, s32 f1, s32 f2);
void func_00245BAC(CGameObjectInstance *self, CAI *ai, f32 angle, f32 arg3);
f32 func_0025EB50(CGameObjectInstance *self, CAI *ai, f32 lo, f32 hi);
void func_00243414(CGameObjectInstance *self, CAI *ai, s32 mode);
s32 func_002451AC(CGameObjectInstance *self, CAI *ai, CGameObjectInstance *target, f32 angle);
void func_002105A4(f32 m[4][4], f32 angle);
void func_0020F85C(f32 m[4][4], CVector3 *in, CVector3 *out);
void func_0020EEF8(CVector3 *out, CVector3 *a, CVector3 *b);

static __inline__ void sidestep(CGameObjectInstance *self, s32 side) {
    CVector3 push;
    CVector3 dir;
    f32 m[4][4];

    push.x = -(f32)side * 204.79999f;
    push.y = 0.0f;
    push.z = 0.0f;
    func_002105A4(m, self->m_RotY);
    func_0020F85C(m, &push, &dir);
    func_0020EEF8(&self->m_vVelocity, &self->m_vVelocity, &dir);
}

void func_00260B40(CGameObjectInstance *self, CAI *ai) {
    CAITargets t;
    s32 side;

    func_00246108(self, ai, &t, 1, 1);
    func_00245BAC(self, ai, 0.13962635f, t.m_Sight.m_Angle);
    if (func_0025EB50(self, ai, 60.0f, 30.0f) < ai->m_ModeTime) {
        func_00243414(self, ai, 4);
    }
    if (ai->m_pSightTarget != NULL) {
        side = func_002451AC(self, ai, ai->m_pSightTarget, 0.3490659f);
        if (side != 0) {
            sidestep(self, side);
        } else {
            self->m_vVelocity.x *= 0.5f;
            self->m_vVelocity.y *= 0.1f;
            self->m_vVelocity.z *= 0.5f;
        }
    }
}
