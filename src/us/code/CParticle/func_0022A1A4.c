#include "common.h"
#include "structs/vector.h"

#include "structs/particle.h"
#include "structs/game_object.h"
typedef struct { f32 x, y, z, w; } Vector4f;

extern f32 func_00298470(f32);                              /* sqrtf */
extern f32 GameObject_GetCollisionYOffset(CGameObjectInstance *actor);               /* its height */
extern f32 GameObject_GetCollisionHeight(CGameObjectInstance *actor);
extern f32 HalfToFloat(s32 value);
extern void CVector3__Subtract(CVector3 *out, CVector3 *a, CVector3 *b);  /* out = a - b */
extern void CVector3__Add(CVector3 *out, CVector3 *a, CVector3 *b);  /* out = a + b */
extern void CVector3__Scale(CVector3 *out, CVector3 *in, f32 s);    /* out = in * s */
extern void CVector3__Normalize(CVector3 *v);                        /* normalize */
#ifdef TARGET_PORT /* ABI (docs/port/ABI.md, abicheck struct_scalar) */
#define CQuaternion__FromDirection CQuaternion__FromDirection_byval
#endif
extern Vector4f CQuaternion__FromDirection(CVector3 *dir);                  /* quaternion facing dir */
extern void CQuaternion__SlerpFast(Vector4f *out, f32 amount, Vector4f *a, Vector4f *b);
extern void CQuaternion__ToMatrix(Vector4f *q, CMtxF *m);
extern void CMtxF__TransformPoint(CMtxF *m, CVector3 *in, CVector3 *out);
extern void CParticle__Wobble(CParticle *p, f32 scale);

/* Speeds up along the heading, stopping at the definition's limit. */
static __inline__ void accelerate(CParticle *p, f32 accel) {
    CVector3 step;
    CVector3 dir;
    CVector3 *vel;
    f32 speed;
    f32 limit;

    dir = p->m_vDirection;
    CVector3__Scale(&step, &dir, accel);
    vel = &p->m_vVelocity;
    CVector3__Add(vel, vel, &step);
    speed = func_00298470(p->m_vVelocity.x * p->m_vVelocity.x + p->m_vVelocity.y * p->m_vVelocity.y + p->m_vVelocity.z * p->m_vVelocity.z);
    limit = HalfToFloat(p->m_pEffect->m_pMotion->m_MaxSpeed);
    if (accel > 0.0f) {
        if (limit < speed) {
            CVector3__Scale(vel, &dir, limit);
        }
    } else {
        if (speed < limit) {
            CVector3__Scale(vel, &dir, limit);
        }
    }
}

/* Homing projectile: turns toward the middle of its target, harder the
 * closer it is, then accelerates along its heading up to (or down to) the
 * definition's speed limit, then wobbles. */
void CParticle__UpdateHoming(CParticle *p) {
    CVector3 aim;
    CVector3 delta;
    CVector3 forward;
    Vector4f from;
    Vector4f to;
    Vector4f turn;
    CMtxF matrix;
    struct CGameObjectInstance *target;
    f32 dist;
    f32 speed;
    f32 amount;
    f32 accel;

    target = p->m_pTarget;
    if (target != NULL && p->m_cFrame > 1.0f) {
        f32 height = GameObject_GetCollisionYOffset(target);

        aim = target->header.m_vPos;
        aim.y += height;
        aim.y += GameObject_GetCollisionHeight(target) * 0.5f;
        CVector3__Subtract(&delta, &aim, &p->header.m_vPos);
        dist = func_00298470(delta.x * delta.x + delta.y * delta.y + delta.z * delta.z);
        if (dist <= 51.199997f) {
            p->header.m_vPos = target->header.m_vPos;
            return;
        }
        CVector3__Normalize(&delta);
        to = CQuaternion__FromDirection(&delta);
        from = CQuaternion__FromDirection(&p->m_vDirection);
        speed = func_00298470(p->m_vVelocity.x * p->m_vVelocity.x + p->m_vVelocity.y * p->m_vVelocity.y + p->m_vVelocity.z * p->m_vVelocity.z);
        if (dist < 1024.0f) {
            amount = dist - 204.79999f;
            if (amount < 0.0f) {
                amount = 0.0f;
            } else {
                amount *= 0.0012207031f;
            }
            amount = 0.9f - amount * 0.85999995f;
        } else {
            amount = 0.04f;
        }
        CQuaternion__SlerpFast(&turn, amount, &from, &to);
        CQuaternion__ToMatrix(&turn, &matrix);
        forward.x = 0.0f;
        forward.y = 0.0f;
        forward.z = 1.0f;
        CMtxF__TransformPoint(&matrix, &forward, &p->m_vDirection);
        CVector3__Scale(&p->m_vVelocity, &p->m_vDirection, speed);
    }
    accel = HalfToFloat(p->m_pEffect->m_pMotion->m_Accel);
    if (accel != 0.0f) {
        accelerate(p, accel);
    }
    CParticle__Wobble(p, 1.0f);
}
