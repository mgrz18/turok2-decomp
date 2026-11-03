/* NON-MATCHING: 2 of 20 words differ: FP register allocation of the final add only. Writing the lerp as `b -= a; b *= level;` loads the folded (b - a) straight into b's pseudo, so level*(b - a) lands in that constant's register like the ROM; the ROM does the same for the sum (it lands in the register of the 75.0 constant, $f1), here the 'a' constant gets a fresh pseudo and the sum goes to $f0. Same shape in func_0025F29C/F928/F9FC/F2F8/FCB8. */
#include "common.h"

typedef struct {
    /* 0x000 */ u32 flags;
    /* 0x004 */ u8 pad4[0x34 - 0x4];
    /* 0x034 */ u8 unk34;
    /* 0x035 */ u8 pad35[0x64 - 0x35];
    /* 0x064 */ f32 timer;
    /* 0x068 */ u8 pad68[0xC8 - 0x68];
    /* 0x0C8 */ u8 unkC8;
    /* 0x0C9 */ u8 padC9[0x120 - 0xC9];
    /* 0x120 */ f32 unk120;
} AI;

typedef struct {
    /* 0x00 */ u8 pad0[0xD4];
    /* 0xD4 */ u32 flags;
} Actor;

/* Raises the AI's 0xC8 level by step, at most 255. */
static __inline__ void RaiseLevel(AI *ai, s32 step) {
    s32 v = ai->unkC8;

    v += step;
    if (v >= 0x100) {
        v = 0xFF;
    }
    ai->unkC8 = v;
}

/* func_0025EB50, inlined: from a to b by the AI's 0xC8 level (0..255). */
static __inline__ f32 LevelLerp(Actor *self, AI *ai, f32 a, f32 b) {
    b -= a;
    b *= ai->unkC8 * (1.0f / 255);
    return a + b;
}

/* AI callback: raises the 0xC8 level by 20 and sets the timer from it (0..255 -> 75..60). */
void func_0025FBB8(Actor *self, AI *ai) {
    RaiseLevel(ai, 20);
    ai->timer = LevelLerp(self, ai, 75.0f, 60.0f);
}
