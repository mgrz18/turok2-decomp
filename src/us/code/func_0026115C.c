#include "common.h"

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 found;
    /* 0x08 */ f32 angle;
    /* 0x0C */ u8 padC[0x1C - 0xC];
    /* 0x1C */ f32 dy;
    /* 0x20 */ u8 pad20[0x44 - 0x20];
} VecDst; /* 0x44 */

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
    /* 0x37 */ s8 mode;
    /* 0x38 */ u8 pad38[0xB0 - 0x38];
    /* 0xB0 */ f32 targetY;
} AI;

typedef struct {
    /* 0x00 */ u8 pad0[0x8];
    /* 0x08 */ f32 y;
} Self;

void func_00246108(Self *self, AI *ai, Targets *out, s32 f1, s32 f2);
void func_0024599C(Self *self, AI *ai, Steer *out);
void func_00260D60(Self *self, AI *ai, Targets *t, Steer *steer, f32 speed, f32 arg5);
s32 func_0025D030(Self *self, AI *ai, Targets *t);
void func_00243414(Self *self, AI *ai, s32 mode);

void func_0026115C(Self *self, AI *ai) {
    Targets t;
    Steer steer;
    s32 next;

    func_00246108(self, ai, &t, 1, 1);
    func_0024599C(self, ai, &steer);
    t.b.dy = t.a.dy = ai->targetY - self->y;
    if (ai->mode == 1) {
        func_00260D60(self, ai, &t, &steer, 2.0f, 2.0f);
    } else {
        func_00260D60(self, ai, &t, &steer, 1.0f, 1.0f);
    }
    if (t.a.found != 0 && (t.a.angle < 0.0f ? -t.a.angle : t.a.angle) <= 1.5707965f) {
        next = func_0025D030(self, ai, &t);
        if (next != 0) {
            ai->unk35 = -1;
            func_00243414(self, ai, next);
        }
    }
}
