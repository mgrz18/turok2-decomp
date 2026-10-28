#include "common.h"

/* Flying AI state: keep facing the target; when something is in the way,
 * side-step away from it, otherwise damp the velocity. Fires state 4 when
 * the timer runs out. */

typedef struct {
    f32 x, y, z;
} Vec3f;

typedef struct {
    f32 m[4][4];
} Mtx4;

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ f32 unk8;
    /* 0x0C */ u8 padC[0x44 - 0xC];
} VecDst;

typedef struct {
    /* 0x00 */ VecDst a;
    /* 0x44 */ VecDst b;
} Targets;

typedef struct {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ Vec3f vel;
    /* 0x24 */ u8 pad24[0x2C];
    /* 0x50 */ f32 yaw;
} Actor;

typedef struct {
    /* 0x00 */ u8 pad0[0x40];
    /* 0x40 */ f32 timer;
    /* 0x44 */ u8 pad44[0x3C];
    /* 0x80 */ Actor *obstacle;
} AI;

void func_00246108(Actor *self, AI *ai, Targets *out, s32 f1, s32 f2);
void func_00245BAC(Actor *self, AI *ai, f32 angle, f32 arg3);
f32 func_0025EB50(Actor *self, AI *ai, f32 lo, f32 hi);
void func_00243414(Actor *self, AI *ai, s32 mode);
s32 func_002451AC(Actor *self, AI *ai, Actor *target, f32 angle);
void func_002105A4(f32 m[4][4], f32 angle);
void func_0020F85C(f32 m[4][4], Vec3f *in, Vec3f *out);
void func_0020EEF8(Vec3f *out, Vec3f *a, Vec3f *b);

static __inline__ void sidestep(Actor *self, s32 side) {
    Vec3f push;
    Vec3f dir;
    f32 m[4][4];

    push.x = -(f32)side * 204.79999f;
    push.y = 0.0f;
    push.z = 0.0f;
    func_002105A4(m, self->yaw);
    func_0020F85C(m, &push, &dir);
    func_0020EEF8(&self->vel, &self->vel, &dir);
}

void func_00260B40(Actor *self, AI *ai) {
    Targets t;
    s32 side;

    func_00246108(self, ai, &t, 1, 1);
    func_00245BAC(self, ai, 0.13962635f, t.a.unk8);
    if (func_0025EB50(self, ai, 60.0f, 30.0f) < ai->timer) {
        func_00243414(self, ai, 4);
    }
    if (ai->obstacle != NULL) {
        side = func_002451AC(self, ai, ai->obstacle, 0.3490659f);
        if (side != 0) {
            sidestep(self, side);
        } else {
            self->vel.x *= 0.5f;
            self->vel.y *= 0.1f;
            self->vel.z *= 0.5f;
        }
    }
}
