#include "common.h"

typedef struct { f32 x, y, z; } Vec3f;
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
    /* 0x04 */ Vec3f pos;
} TargetActor;

typedef struct {
    /* 0x000 */ u8 pad0[4];
    /* 0x004 */ Vec3f pos;
    /* 0x010 */ u8 pad10[8];
    /* 0x018 */ Vec3f vel;
    /* 0x024 */ u8 pad24[0xF8 - 0x24];
    /* 0x0F8 */ ProjectileDef *def;
    /* 0x0FC */ u8 padFC[0x114 - 0xFC];
    /* 0x114 */ TargetActor *target;
    /* 0x118 */ u8 pad118[4];
    /* 0x11C */ f32 speed;
    /* 0x120 */ u8 pad120[0x150 - 0x120];
    /* 0x150 */ Vec3f dir;
} Projectile;

extern f32 func_00298470(f32);                              /* sqrtf */
extern f32 func_0026841C(TargetActor *actor);               /* its height */
extern f32 func_002671B4(TargetActor *actor);
extern f32 func_0026D210(s32 value);
extern void func_0020EF2C(Vec3f *out, Vec3f *a, Vec3f *b);  /* out = a - b */
extern void func_0020EEF8(Vec3f *out, Vec3f *a, Vec3f *b);  /* out = a + b */
extern void func_0020EF60(Vec3f *out, Vec3f *in, f32 s);    /* out = in * s */
extern void func_0020F040(Vec3f *v);                        /* normalize */
extern Vector4f func_0020E810(Vec3f *dir);                  /* quaternion facing dir */
extern void func_0020DCC8(Vector4f *out, f32 amount, Vector4f *a, Vector4f *b);
extern void func_00211114(Vector4f *q, MatrixF *m);
extern void func_0020F85C(MatrixF *m, Vec3f *in, Vec3f *out);
extern void func_00229618(Projectile *p, f32 scale);

/* Speeds up along the heading, stopping at the definition's limit. */
static __inline__ void accelerate(Projectile *p, f32 accel) {
    Vec3f step;
    Vec3f dir;
    Vec3f *vel;
    f32 speed;
    f32 limit;

    dir = p->dir;
    func_0020EF60(&step, &dir, accel);
    vel = &p->vel;
    func_0020EEF8(vel, vel, &step);
    speed = func_00298470(p->vel.x * p->vel.x + p->vel.y * p->vel.y + p->vel.z * p->vel.z);
    limit = func_0026D210(p->def->motion->limit);
    if (accel > 0.0f) {
        if (limit < speed) {
            func_0020EF60(vel, &dir, limit);
        }
    } else {
        if (speed < limit) {
            func_0020EF60(vel, &dir, limit);
        }
    }
}

/* Homing projectile: turns toward the middle of its target, harder the
 * closer it is, then accelerates along its heading up to (or down to) the
 * definition's speed limit, then wobbles. */
void func_0022A1A4(Projectile *p) {
    Vec3f aim;
    Vec3f delta;
    Vec3f forward;
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
        aim.y += func_002671B4(target) * 0.5f;
        func_0020EF2C(&delta, &aim, &p->pos);
        dist = func_00298470(delta.x * delta.x + delta.y * delta.y + delta.z * delta.z);
        if (dist <= 51.199997f) {
            p->pos = target->pos;
            return;
        }
        func_0020F040(&delta);
        to = func_0020E810(&delta);
        from = func_0020E810(&p->dir);
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
        func_0020DCC8(&turn, amount, &from, &to);
        func_00211114(&turn, &matrix);
        forward.x = 0.0f;
        forward.y = 0.0f;
        forward.z = 1.0f;
        func_0020F85C(&matrix, &forward, &p->dir);
        func_0020EF60(&p->vel, &p->dir, speed);
    }
    accel = func_0026D210(p->def->motion->accel);
    if (accel != 0.0f) {
        accelerate(p, accel);
    }
    func_00229618(p, 1.0f);
}
