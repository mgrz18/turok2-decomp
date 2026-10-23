#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x1C];
    /* 0x1C */ f32 unk1C;
} Actor;

typedef struct {
    /* 0x00 */ u8 pad0[0x40];
    /* 0x40 */ f32 unk40;
    /* 0x44 */ u8 pad44[0x64 - 0x44];
    /* 0x64 */ f32 timer;
    /* 0x68 */ u8 pad68[0xBC - 0x68];
    /* 0xBC */ f32 unkBC;
} AI;

f32 func_00211774(f32 cur, f32 target, f32 rate);
void func_00243414(Actor *self, AI *ai, s32 mode);

void func_00261E44(Actor *self, AI *ai) {
    ai->unkBC = func_00211774(ai->unkBC, 1.0f, 0.2f);
    self->unk1C = func_00211774(self->unk1C, 0.0f, 1.024f);
    if (ai->unk40 >= ai->timer) {
        func_00243414(self, ai, 6);
    }
}
