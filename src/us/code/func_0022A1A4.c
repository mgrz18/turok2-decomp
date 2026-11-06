#include "common.h"
#include "structs/vector.h"

typedef struct { f32 x, y, z, w; } Vector4f;
typedef struct { f32 m[16]; } MatrixF;

typedef struct {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ u16 accel;
    /* 0x12 */ u16 limit;
} ProjectileMotion;

typedef struct {
    /* 0x00 */ u8 pad0[0x30];
    /* 0x30 */ ProjectileMotion *motion;
} ProjectileDef;

typedef struct {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ CVector3 pos;
} TargetActor;

typedef struct {
    /* 0x000 */ u8 pad0[4];
    /* 0x004 */ CVector3 pos;
    /* 0x010 */ u8 pad10[8];
    /* 0x018 */ CVector3 vel;
    /* 0x024 */ u8 pad24[0xF8 - 0x24];
    /* 0x0F8 */ ProjectileDef *def;
    /* 0x0FC */ u8 padFC[0x114 - 0xFC];
    /* 0x114 */ TargetActor *target;
    /* 0x118 */ u8 pad118[4];
    /* 0x11C */ f32 speed;
    /* 0x120 */ u8 pad120[0x150 - 0x120];
    /* 0x150 */ CVector3 dir;
} Projectile;

extern f32 func_00298470(f32);                              /* sqrtf */
extern f32 func_0026841C(TargetActor *actor);               /* its height */
extern f32 GameObject_GetCollisionHeight(TargetActor *actor);
extern f32 HalfToFloat(s32 value);
extern void CVector3__Subtract(CVector3 *out, CVector3 *a, CVector3 *b);  /* out = a - b */
extern void CVector3__Add(CVector3 *out, CVector3 *a, CVector3 *b);  /* out = a + b */
extern void CVector3__Scale(CVector3 *out, CVector3 *in, f32 s);    /* out = in * s */
extern void CVector3__Normalize(CVector3 *v);                        /* normalize */
extern Vector4f CQuaternion__FromDirection(CVector3 *dir);                  /* quaternion facing dir */
extern void CQuaternion__SlerpFast(Vector4f *out, f32 amount, Vector4f *a, Vector4f *b);
extern void CQuaternion__ToMatrix(Vector4f *q, MatrixF *m);
extern void CMtxF__TransformPoint(MatrixF *m, CVector3 *in, CVector3 *out);
extern void CParticle__Wobble(Projectile *p, f32 scale);

/* Speeds up along the heading, stopping at the definition's limit. */
static __inline__ void accelerate(Projectile *p, f32 accel) {
    CVector3 step;
    CVector3 dir;
    CVector3 *vel;
    f32 speed;
    f32 limit;

    dir = p->dir;
    CVector3__Scale(&step, &dir, accel);
    vel = &p->vel;
    CVector3__Add(vel, vel, &step);
    speed = func_00298470(p->vel.x * p->vel.x + p->vel.y * p->vel.y + p->vel.z * p->vel.z);
    limit = HalfToFloat(p->def->motion->limit);
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
void CParticle__UpdateHoming(Projectile *p) {
    CVector3 aim;
    CVector3 delta;
    CVector3 forward;
    Vector4f from;
    Vector4f to;
    Vector4f turn;
    MatrixF matrix;
    TargetActor *target;
    f32 dist;
    f32 speed;
    f32 amount;
    f32 accel;

    target = p->target;
    if (target != NULL && p->speed > 1.0f) {
        f32 height = func_0026841C(target);

        aim = target->pos;
        aim.y += height;
        aim.y += GameObject_GetCollisionHeight(target) * 0.5f;
        CVector3__Subtract(&delta, &aim, &p->pos);
        dist = func_00298470(delta.x * delta.x + delta.y * delta.y + delta.z * delta.z);
        if (dist <= 51.199997f) {
            p->pos = target->pos;
            return;
        }
        CVector3__Normalize(&delta);
        to = CQuaternion__FromDirection(&delta);
        from = CQuaternion__FromDirection(&p->dir);
        speed = func_00298470(p->vel.x * p->vel.x + p->vel.y * p->vel.y + p->vel.z * p->vel.z);
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
        CMtxF__TransformPoint(&matrix, &forward, &p->dir);
        CVector3__Scale(&p->vel, &p->dir, speed);
    }
    accel = HalfToFloat(p->def->motion->accel);
    if (accel != 0.0f) {
        accelerate(p, accel);
    }
    CParticle__Wobble(p, 1.0f);
}
