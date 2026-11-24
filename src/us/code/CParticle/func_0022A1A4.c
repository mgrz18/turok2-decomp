#include "common.h"
#include "structs/vector.h"

#include "structs/particle.h"
#include "structs/game_object.h"

extern f32 func_00298470(f32);                              /* sqrtf */
f32 GameObject_GetCollisionYOffset(CGameObjectInstance *item);
f32 GameObject_GetCollisionHeight(CGameObjectInstance *a);
f32 HalfToFloat(s32 half);
extern void CVector3__Subtract(CVector3 *out, CVector3 *a, CVector3 *b);  /* out = a - b */
extern void CVector3__Add(CVector3 *out, CVector3 *a, CVector3 *b);  /* out = a + b */
extern void CVector3__Scale(CVector3 *out, CVector3 *in, f32 s);    /* out = in * s */
extern void CVector3__Normalize(CVector3 *v);                        /* normalize */
/* The quaternion facing dir, returned by value: the N64 definition's (out, dir) seen as a struct return
 * (the hidden pointer is its first argument); the port calls its by-value shim (ABI.md, struct_scalar). */
#ifdef TARGET_PORT
extern CQuaternion CQuaternion__FromDirection_byval(CVector3 *dir);
#define CQuaternion__FromDirection_r CQuaternion__FromDirection_byval
#else
extern CQuaternion CQuaternion__FromDirection_r(CVector3 *dir) __asm__("CQuaternion__FromDirection");
#endif
void CQuaternion__SlerpFast(CQuaternion *out, f32 t, CQuaternion *a, CQuaternion *b);
void CQuaternion__ToMatrix(CQuaternion *q, void *m);
void CMtxF__TransformPoint(void *m, CVector3 *in, CVector3 *out);
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
    CQuaternion from;
    CQuaternion to;
    CQuaternion turn;
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
        to = CQuaternion__FromDirection_r(&delta);
        from = CQuaternion__FromDirection_r(&p->m_vDirection);
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
