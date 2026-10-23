#include "common.h"

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
    /* 0x00 */ u8 pad0[0x40];
} Steer;

typedef struct {
    /* 0x00 */ u8 pad0[0x35];
    /* 0x35 */ s8 unk35;
} AI;

void func_00246108(void *self, AI *ai, Targets *out, s32 f1, s32 f2);
void func_0024599C(void *self, AI *ai, Steer *out);
f32 func_0025EB24(void *self, AI *ai);
void func_00245BAC(void *self, AI *ai, f32 angle, f32 arg3);
f32 func_0025EAFC(void *self, AI *ai);
s32 func_0025D030(void *self, AI *ai, Targets *t);
void func_00243414(void *self, AI *ai, s32 mode);

void func_00260634(void *self, AI *ai) {
    Targets t;
    Steer steer;
    f32 range;
    s32 mode;

    func_00246108(self, ai, &t, 1, 1);
    func_0024599C(self, ai, &steer);
    func_00245BAC(self, ai, func_0025EB24(self, ai), t.a.unk8);
    range = func_0025EAFC(self, ai);
    if ((t.a.unk8 < 0.0f ? -t.a.unk8 : t.a.unk8) <= range) {
        mode = func_0025D030(self, ai, &t);
        if (mode != 0) {
            ai->unk35 = -1;
            func_00243414(self, ai, mode);
        } else {
            func_00243414(self, ai, 10);
        }
    }
}
