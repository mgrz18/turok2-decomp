#include "common.h"

typedef struct {
    /* 0x00 */ s32 type;
    /* 0x04 */ void *obj;
    /* 0x08 */ f32 angle;
    /* 0x0C */ u8 padC[0x1C - 0xC];
    /* 0x1C */ f32 dy;
    /* 0x20 */ u8 pad20[0x40 - 0x20];
    /* 0x40 */ f32 dist;
} VecDst; /* 0x44 */

typedef struct {
    /* 0x00 */ VecDst a;
    /* 0x44 */ VecDst b;
} Targets;

typedef struct {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ f32 pitchRate;
    /* 0x1C */ u8 pad1C[0x40 - 0x1C];
} Steer;

typedef struct {
    /* 0x00 */ u8 pad0[0x1C];
    /* 0x1C */ f32 pitch;
} Self;

typedef struct {
    /* 0x000 */ u8 pad0[0x40];
    /* 0x040 */ f32 timer;
    /* 0x044 */ u8 pad44[0x6C - 0x44];
    /* 0x06C */ f32 wobble[3];
    /* 0x078 */ u8 pad78[0xBC - 0x78];
    /* 0x0BC */ f32 speed;
    /* 0x0C0 */ u8 padC0[0x118 - 0xC0];
    /* 0x118 */ f32 turnRate;
} AI;

void func_00246108(Self *self, AI *ai, Targets *out, s32 f1, s32 f2);
void func_0024599C(Self *self, AI *ai, Steer *out);
f32 func_00211774(f32 cur, f32 target, f32 rate);
f32 func_0021160C(f32 value, f32 target, f32 rate);
void func_00245BAC(Self *self, AI *ai, f32 turnRate, f32 angle);

void func_00261FB0(Self *self, AI *ai) {
    Targets t;
    Steer steer;
    Targets *tp;

    ai->wobble[0] = 0.0f;
    ai->wobble[2] = 0.0f;
    ai->speed = func_00211774(ai->speed, 6.0f, 0.2f);
    ai->turnRate = func_00211774(ai->turnRate, 0.20943953f, 0.5f);
    func_00246108(self, ai, &t, 1, 1);
    func_0024599C(self, ai, &steer);

    tp = &t;
    self->pitch = func_0021160C(self->pitch, tp->b.dy, steer.pitchRate * 2.0f);
    if (self->pitch > 81.92f) {
        self->pitch = 81.92f;
    } else if (self->pitch < -81.92f) {
        self->pitch = -81.92f;
    }
    func_00245BAC(self, ai, ai->turnRate, tp->b.angle);
}
