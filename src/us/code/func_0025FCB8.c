#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x64];
    /* 0x64 */ f32 reactTime;
    /* 0x68 */ u8 pad68[0xC8 - 0x68];
    /* 0xC8 */ u8 intensity;
} AI;

/* AI reaction (like func_0025CAC8 case 11, but setting the delay): raises the
 * intensity by 20 (at most 255) and sets the reaction delay to 75..60 frames. */
void func_0025FCB8(void *actor, AI *ai) {
    s32 v = ai->intensity;
    f32 level;

    v += 20;
    if (v >= 0x100) {
        v = 0xFF;
    }
    ai->intensity = v;
    level = ai->intensity;
    level *= (1.0f / 255);
    level = level * -15.0f + 75.0f;
    ai->reactTime = level;
}
