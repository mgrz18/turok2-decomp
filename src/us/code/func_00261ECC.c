#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0xD4];
    /* 0xD4 */ s32 flags;
} Actor;

typedef struct {
    /* 0x00 */ u8 pad0[0x64];
    /* 0x64 */ f32 timer;
} AI;

f32 func_002119FC(f32 lo, f32 hi);

void func_00261ECC(Actor *self, AI *ai) {
    self->flags &= ~0x20000;
    ai->timer = func_002119FC(30.0f, 60.0f);
}
