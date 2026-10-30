/* NON-MATCHING: 8 of 23 words differ: FP register allocation only. The ROM keeps the 0..1 level in $f1, the scaled value in $f0 and the sum in $f1 (each result tied to its constant operand); this C gives the chain one register. Same issue in func_0025F29C/F2F8/F928/F9FC/FBB8/FCB8 (the += in F9FC gets the sum right, 3 words left). */
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
    return a + ai->unkC8 * (1.0f / 255) * (b - a);
}

/* AI callback: in modes 0x11-0x12 sets 0x120 from 0xC8 (0..255 -> 60..15);
 * clears AI flag 0x4000000. */
void func_0025F29C(Actor *self, AI *ai) {
    if (ai->unk34 == 0x11 || ai->unk34 == 0x12) {
        ai->unk120 = LevelLerp(self, ai, 60.0f, 15.0f);
    }
    ai->flags &= ~0x4000000;
}
