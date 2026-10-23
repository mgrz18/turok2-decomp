#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x6C];
    /* 0x6C */ f32 unk6C;
    /* 0x70 */ f32 unk70;
    /* 0x74 */ f32 unk74;
} AI;

f32 func_002119FC(f32 lo, f32 hi);

void func_00260920(void *self, AI *ai) {
    f32 lo = -40.96f;
    f32 hi = 40.96f;

    ai->unk6C = func_002119FC(lo, hi);
    ai->unk70 = func_002119FC(lo, hi);
    ai->unk74 = func_002119FC(lo, hi);
}
