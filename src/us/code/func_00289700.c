#include "common.h"

/* Only the field read here: a timer far into this (large) state. */
typedef struct {
    /* 0x00000 */ u8 pad0[0x23FD0];
    /* 0x23FD0 */ f32 timer;
} BigState;

extern s32 func_00288C5C(s32, s32);
extern f32 D_800A9E14[];
extern s32 D_800F5D28[];

void func_00289700(BigState *arg0) {
    if (D_800F5D28[0] != 0) {
        arg0->timer = 0.0f;
    }
    if (arg0->timer > D_800A9E14[0]) {
        func_00288C5C((s32) arg0, 7);
    }
}
