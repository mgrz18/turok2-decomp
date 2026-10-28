#include "common.h"

/* AI state: turn towards the target with the 10000 animation, the turn rate
 * depending on the stance; release the grabbed actor near the end of it. */

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
    /* 0x000 */ u8 pad0[0x36];
    /* 0x036 */ u8 stance;
    /* 0x037 */ u8 pad37[0x90];
    /* 0x0C7 */ s8 animDone;
    /* 0x0C8 */ u8 padC8[0x4C];
    /* 0x114 */ u8 *held;
} AI;

typedef struct {
    /* 0x00 */ u8 pad0[0xD8];
    /* 0xD8 */ f32 animFrame;
    /* 0xDC */ s16 anim;
    /* 0xDE */ s16 padDE;
    /* 0xE0 */ s16 animLength;
} Self;

s32 func_00219F74(void *self, s32 anim, s32 arg2);
void func_00246108(void *self, AI *ai, Targets *out, s32 f1, s32 f2);
void func_00245BAC(void *self, AI *ai, f32 angle, f32 arg3);
void func_00243414(void *self, void *ai, s32 mode);

void func_0025F3A4(Self *self, AI *ai) {
    Targets t;
    u8 *held = ai->held;
    f32 rate;
    s32 anim;

    anim = func_00219F74(self, 10000, -1);
    if (self->anim == anim) {
        func_00246108(self, ai, &t, 0, 1);
        rate = 0.26179942f;
        if (ai->stance == 1 || ai->stance == 2) {
            rate = 1.0471977f;
        }
        func_00245BAC(self, ai, rate * 1.5f, t.a.unk8);
        if (held != NULL && self->animLength * 0.7f <= self->animFrame) {
            func_00243414(held, held + 0x140, 2);
            ai->held = NULL;
        }
    }
    if (ai->animDone) {
        func_00243414(self, ai, 10);
    }
}
