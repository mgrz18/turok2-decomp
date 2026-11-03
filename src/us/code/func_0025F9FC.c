#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x64];
    /* 0x64 */ f32 reactTime;
    /* 0x68 */ u8 pad68[0xC8 - 0x68];
    /* 0xC8 */ u8 intensity;
} AI;

/* AI reaction (same code as func_0025CAC8 case 10): raises the intensity by 10
 * (at most 255) and delays the next reaction by 105..60 frames. */
void func_0025F9FC(void *actor, AI *ai) {
    s32 v = ai->intensity;
    f32 level;

    v += 10;
    if (v >= 0x100) {
        v = 0xFF;
    }
    ai->intensity = v;
    level = ai->intensity;
    level *= (1.0f / 255);
    level = level * -45.0f + 105.0f;
    ai->reactTime += level;
}
