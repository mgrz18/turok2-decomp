/* NON-MATCHING: 2 of 24 words differ: FP register allocation of the final add only. Writing the lerp as `b -= a; b *= level;` loads the folded (b - a) straight into b's pseudo, so level*(b - a) lands in that constant's register like the ROM; the ROM does the same for the sum (it lands in the register of the 15.0 constant, $f1), here the 'a' constant gets a fresh pseudo and the sum goes to $f0. Same shape in func_0025F29C/F928/F9FC/FBB8/FCB8. */
#include "common.h"

typedef struct {
    /* 0x000 */ u32 flags;
    /* 0x004 */ u8 pad4[0x34 - 0x4];
    /* 0x034 */ u8 unk34;
    /* 0x035 */ u8 pad35[0x64 - 0x35];
    /* 0x064 */ f32 unk64;
    /* 0x068 */ u8 pad68[0xC8 - 0x68];
    /* 0x0C8 */ u8 unkC8;
    /* 0x0C9 */ u8 padC9[0x120 - 0xC9];
    /* 0x120 */ f32 unk120;
} AI;

typedef struct {
    /* 0x00 */ u8 pad0[0xD4];
    /* 0xD4 */ u32 flags;
} Actor;

/* func_0025EB50, inlined: from a to b by the AI's 0xC8 level (0..255). */
static __inline__ f32 LevelLerp(Actor *self, AI *ai, f32 a, f32 b) {
    b -= a;
    b *= ai->unkC8 * (1.0f / 255);
    return a + b;
}

/* AI callback: clears AI flag 0x4000000 and actor flag 0x20000; sets 0x64
 * from the 0xC8 level (0..255 -> 15..45). */
void func_0025F2F8(Actor *self, AI *ai) {
    ai->flags &= ~0x4000000;
    self->flags &= ~0x20000;
    ai->unk64 = LevelLerp(self, ai, 15.0f, 45.0f);
}
