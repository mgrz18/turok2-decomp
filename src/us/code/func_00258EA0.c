#include "common.h"

extern f32 func_0021170C(f32 value, f32 rate);  /* moves value toward zero */
extern void func_0027588C(s32 sound);
extern s32 D_800B6D20;      /* frame counter */
extern f32 D_800B6D28;      /* frame time scale */

#define ABS(x) ((x) < 0.0f ? -(x) : (x))

typedef struct {
    /* 0x000 */ u8 pad0[0x168];
    /* 0x168 */ f32 volume;
} Sound;

typedef struct {
    /* 0x000 */ u8 pad0[0xA50];
    /* 0xA50 */ Sound *sound;
} Owner;

typedef struct {
    /* 0x000 */ u8 pad0[0x1A8];
    /* 0x1A8 */ Owner *owner;
} Actor;

typedef struct {
    /* 0x000 */ u8 pad0[0x34];
    /* 0x034 */ s8 state;
    /* 0x035 */ u8 pad35[0x10C - 0x35];
    /* 0x10C */ f32 angle;
    /* 0x110 */ f32 spin;
} Spinner;

/* Spins down unless held (state 4), turns the angle and drives the spin sound
 * from the spin speed. */
void func_00258EA0(Actor *actor, Spinner *sp) {
    Owner *owner;
    Sound *snd;

    owner = actor->owner;
    if (sp->state != 4) {
        sp->spin = func_0021170C(sp->spin, 0.0065449853f);
    }
    sp->angle += sp->spin * D_800B6D28 * 2.0f;
    snd = owner->sound;
    snd->volume = (ABS(sp->spin) * 1.7904929f > 1.0f) ? 1.0f : ABS(sp->spin) * 1.7904929f;
    if (sp->state != 4 && (D_800B6D20 & 1)) {
        func_0027588C(0x1A7);
        func_0027588C(0x1A5);
    }
}
