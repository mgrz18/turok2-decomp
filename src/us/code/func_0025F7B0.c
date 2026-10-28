#include "common.h"

/* AI state: bob towards the target height on a sine wave, the vertical speed
 * eased and clamped; once level with it, switch to state 0x3C. */

typedef struct {
    f32 x, y, z;
} Vec3f;

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ f32 unk8;
    /* 0x0C */ Vec3f a;
    /* 0x18 */ Vec3f b;
    /* 0x24 */ u8 pad24[0x44 - 0x24];
} VecDst;

typedef struct {
    /* 0x00 */ VecDst a;
    /* 0x44 */ VecDst b;
} Targets;

typedef struct {
    /* 0x00 */ u8 pad0[0x34];
    /* 0x34 */ s8 state;
    /* 0x35 */ u8 pad35[0x2F];
    /* 0x64 */ f32 bobPhase;
} AI;

typedef struct {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ Vec3f pos;
    /* 0x10 */ u8 pad10[0xC];
    /* 0x1C */ f32 velY;
} Actor;

extern f32 D_800B6D28;
void func_0025E9BC(Actor *self, AI *ai);
void func_00246108(Actor *self, AI *ai, Targets *out, s32 f1, s32 f2);
f32 func_002982F0(f32 x); /* sinf */
f32 func_00211774(f32 cur, f32 target, f32 rate);
void func_00243414(Actor *self, AI *ai, s32 mode);

void func_0025F7B0(Actor *self, AI *ai) {
    Targets t;
    f32 v;

    if (ai->state == 0x3C) {
        func_0025E9BC(self, ai);
    }
    func_00246108(self, ai, &t, 1, 0);
    ai->bobPhase += D_800B6D28 * 15.0f;
    t.b.b.y += func_002982F0(ai->bobPhase * 0.017453294f) * 61.44f + 51.199997f;
    v = func_00211774(self->velY, t.b.b.y, 10.24f);
    self->velY = v;
    if (v > 40.96f) {
        self->velY = 40.96f;
    } else if (v < -40.96f) {
        self->velY = -40.96f;
    }
    if (t.b.a.y <= self->pos.y + self->velY) {
        self->velY = t.b.b.y;
        if (ai->state != 0x3C) {
            func_00243414(self, ai, 0x3C);
        }
    }
}
