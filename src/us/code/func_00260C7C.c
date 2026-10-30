#include "common.h"

typedef struct {
    f32 x, y, z;
} Vec3f;

typedef struct {
    f32 m[4][4];
} Mtx44;

typedef struct {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ Vec3f vel;
    /* 0x24 */ u8 pad24[0x50 - 0x24];
    /* 0x50 */ f32 yaw;
} Actor;

typedef struct {
    /* 0x00 */ u8 pad0[0x80];
    /* 0x80 */ void *target;
} AI;

s32 func_002451AC(Actor *self, AI *ai, void *target, f32 angle);
void func_002105A4(Mtx44 *mtx, f32 angle);
void func_0020F85C(Mtx44 *mtx, Vec3f *in, Vec3f *out);
void func_0020EEF8(Vec3f *out, Vec3f *a, Vec3f *b);

/* AI callback: with a target, pushes the actor sideways (by its yaw) by the
 * side func_002451AC reports within 20 degrees; else damps its velocity. */
void func_00260C7C(Actor *self, AI *ai) {
    Vec3f push;
    Vec3f dir;
    Mtx44 mtx;
    s32 side;

    if (ai->target == NULL) {
        return;
    }
    side = func_002451AC(self, ai, ai->target, 0.3490659f);
    if (side != 0) {
        push.x = -(f32) side * 204.79999f;
        push.y = 0.0f;
        push.z = 0.0f;
        func_002105A4(&mtx, self->yaw);
        func_0020F85C(&mtx, &push, &dir);
        func_0020EEF8(&self->vel, &self->vel, &dir);
    } else {
        self->vel.x *= 0.5f;
        self->vel.y *= 0.1f;
        self->vel.z *= 0.5f;
    }
}
