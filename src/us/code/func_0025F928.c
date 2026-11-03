#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x64];
    /* 0x64 */ f32 timer;
    /* 0x68 */ u8 pad68[0xC8 - 0x68];
    /* 0xC8 */ u8 level;
} AI;

/* AI callback: raises the 0xC8 level by 10 (at most 255) and sets the timer
 * from it, 0..255 -> 240..120. The float temporaries (240 passed through two
 * variables) are what gives the ROM its alternating $f0/$f1 allocation. */
void func_0025F928(void *self, AI *ai) {
    s32 level = ai->level;
    f32 t;
    f32 scale;
    f32 v;

    scale = 1.0f / 255.0f;
    level += 10;
    if (level >= 0x100) {
        level = 0xFF;
    }
    ai->level = level;
    v = ai->level * scale;
    t = v;
    t *= -120.0f;
    v = 240.0f;
    scale = v;
    v = t + scale;
    ai->timer = v;
}
