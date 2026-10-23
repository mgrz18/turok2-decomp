#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x1C];
    /* 0x1C */ f32 unk1C;
} Actor;

typedef struct {
    /* 0x000 */ u8 pad0[0x118];
    /* 0x118 */ f32 unk118;
} AI;

typedef struct {
    /* 0x00 */ u8 pad0[0x4C];
    /* 0x4C */ f32 unk4C;
    /* 0x50 */ u8 pad50[0x60 - 0x50];
    /* 0x60 */ f32 unk60;
} Arg2;

typedef struct {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ f32 unk18;
} Arg3;

f32 func_0021160C(f32 value, f32 delta, f32 limit);
void func_00245BAC(Actor *self, AI *ai, f32 angle, f32 arg3);

void func_002620B4(Actor *self, AI *ai, Arg2 *a2, Arg3 *a3) {
    self->unk1C = func_0021160C(self->unk1C, a2->unk60, a3->unk18 * 2.0f);
    if (self->unk1C > 81.92f) {
        self->unk1C = 81.92f;
    } else if (self->unk1C < -81.92f) {
        self->unk1C = -81.92f;
    }
    func_00245BAC(self, ai, ai->unk118, a2->unk4C);
}
