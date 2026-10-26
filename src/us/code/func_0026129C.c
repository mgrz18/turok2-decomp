#include "common.h"

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 found;
    /* 0x08 */ u8 pad8[0x1C - 0x8];
    /* 0x1C */ f32 dy;
    /* 0x20 */ u8 pad20[0x40 - 0x20];
    /* 0x40 */ f32 dist;
} VecDst; /* 0x44 */

typedef struct {
    /* 0x00 */ VecDst a;
    /* 0x44 */ VecDst b;
} Targets;

typedef struct {
    /* 0x00 */ u8 pad0[0x40];
} Steer;

typedef struct {
    /* 0x00 */ u8 pad0[0x40];
    /* 0x40 */ f32 unk40;
} AI;

void func_00246108(void *self, AI *ai, Targets *out, s32 f1, s32 f2);
void func_0024599C(void *self, AI *ai, Steer *out);
void func_00260D60(void *self, AI *ai, Targets *t, Steer *steer, f32 speed, f32 arg5);
void func_00243414(void *self, AI *ai, s32 mode);

void func_0026129C(void *self, AI *ai) {
    Targets t;
    Steer steer;
    f32 dist;

    func_00246108(self, ai, &t, 1, 1);
    func_0024599C(self, ai, &steer);
    dist = t.b.dist;
    if (!(dist >= 1024.0f)) {
        dist = 1024.0f;
    }
    func_00260D60(self, ai, &t, &steer, 6.0f - dist * (1.0f / 1024) * 3.0f, 2.0f);
    if ((t.a.found != 0 && (t.a.dy < 0.0f ? -t.a.dy : t.a.dy) < 51.199997f) || ai->unk40 > 90.0f) {
        func_00243414(self, ai, 4);
    }
}
