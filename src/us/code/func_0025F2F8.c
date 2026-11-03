#include "common.h"

typedef struct {
    /* 0x000 */ u32 flags;
    /* 0x004 */ u8 pad4[0x64 - 0x4];
    /* 0x064 */ f32 timer;
    /* 0x068 */ u8 pad68[0xC8 - 0x68];
    /* 0x0C8 */ u8 intensity; /* 0..255 */
} AI;

typedef struct {
    /* 0x00 */ u8 pad0[0xD4];
    /* 0xD4 */ u32 flags;
} Actor;

/* AI callback: clears AI flag 0x4000000 and actor flag 0x20000; sets the
 * timer from the intensity (0..255 -> 15..45). */
void func_0025F2F8(Actor *self, AI *ai) {
    f32 level;

    ai->flags &= ~0x4000000;
    self->flags &= ~0x20000;
    level = ai->intensity;
    level *= (1.0f / 255);
    level = level * 30.0f + 15.0f;
    ai->timer = level;
}
