#include "common.h"

/* AI idle-sound callback: while the actor moves fast the timer stays reset;
 * otherwise it counts down and, once idle in state 4, plays the ambient
 * animation (0x26 or 0x28 depending on the mode) every 30-75 frames. */

typedef struct {
    /* 0x00 */ u8 pad0[0x60];
    /* 0x60 */ f32 speed;
} Motion;

typedef struct {
    /* 0x000 */ u8 pad0[0xD4];
    /* 0x0D4 */ u32 flags;
} Actor;

typedef struct {
    /* 0x000 */ u8 pad0[0x4];
    /* 0x004 */ s32 unk4;
    /* 0x008 */ u8 pad8[0x34 - 0x8];
    /* 0x034 */ s8 state;
    /* 0x035 */ u8 pad35[0x3C - 0x35];
    /* 0x03C */ u32 flags;
    /* 0x040 */ u8 pad40[0x12C - 0x40];
    /* 0x12C */ f32 idleTimer;
    /* 0x130 */ s8 mode[3];
    /* 0x133 */ s8 modeIndex;
} CAI;

extern f32 D_800B6D28; /* frame delta */

extern void func_00243414(Actor *actor, CAI *ai, s32 anim);
extern f32 func_002119FC(f32 lo, f32 hi);

void func_0025C1B0(Actor *actor, CAI *ai, Motion *motion) {
    s32 modeIndex = ai->modeIndex;
    f32 speed;

    if (!(ai->flags & 0x10000) && (actor->flags & 0x400)) {
        return;
    }
    speed = motion->speed;
    if (speed < 0.0f ? -speed > 102.399994f : speed > 102.399994f) {
        ai->idleTimer = 30.0f;
        if (ai->mode[1] && modeIndex == 0) {
            func_00243414(actor, ai, 0x26);
        }
        return;
    }
    ai->idleTimer -= D_800B6D28;
    if (ai->idleTimer < 0.0f) {
        ai->idleTimer = 0.0f;
    }
    if (ai->unk4 && ai->state == 4 && ai->idleTimer == 0.0f) {
        ai->idleTimer = func_002119FC(30.0f, 75.0f);
        if (modeIndex == 0) {
            if (ai->mode[1]) {
                func_00243414(actor, ai, 0x26);
            }
        } else if (modeIndex == 1) {
            if (ai->mode[0]) {
                func_00243414(actor, ai, 0x28);
            }
        }
    }
}
