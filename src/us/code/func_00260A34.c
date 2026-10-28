#include "common.h"

/* Flying AI state: steer towards the target, attack when possible (resetting
 * the attack delay), and fall back to state 10 once the timer runs out. */

typedef struct {
    /* 0x00 */ u8 pad0[0x44];
} VecDst;

typedef struct {
    /* 0x00 */ VecDst a;
    /* 0x44 */ VecDst b;
} Targets;

typedef struct {
    /* 0x00 */ u8 pad0[0x40];
} Steer;

typedef struct {
    /* 0x00 */ u8 pad0[0x35];
    /* 0x35 */ s8 unk35;
    /* 0x36 */ u8 pad36;
    /* 0x37 */ s8 unk37;
    /* 0x38 */ u8 pad38[0xC];
    /* 0x44 */ f32 attackDelay;
    /* 0x48 */ u8 pad48[0x1C];
    /* 0x64 */ f32 timer;
} AI;

extern f32 D_800B6D28;
void func_00246108(void *self, AI *ai, Targets *out, s32 f1, s32 f2);
void func_0024599C(void *self, AI *ai, Steer *out);
void func_00260730(void *self, AI *ai, Targets *t, Steer *steer);
s32 func_0025D030(void *self, AI *ai, Targets *t);
void func_00243414(void *self, AI *ai, s32 mode);
f32 func_0025EB50(void *self, AI *ai, f32 lo, f32 hi);

void func_00260A34(void *self, AI *ai) {
    Targets t;
    Steer steer;
    s32 mode;

    func_00246108(self, ai, &t, 1, 1);
    func_0024599C(self, ai, &steer);
    func_00260730(self, ai, &t, &steer);
    mode = func_0025D030(self, ai, &t);
    if (mode != 0) {
        ai->unk35 = -1;
        func_00243414(self, ai, mode);
        ai->unk37 = 0;
        ai->attackDelay = func_0025EB50(self, ai, 90.0f, 30.0f);
    }
    ai->timer += D_800B6D28;
    if (func_0025EB50(self, ai, 60.0f, 120.0f) < ai->timer) {
        ai->timer = 0.0f;
        func_00243414(self, ai, 10);
    }
}
