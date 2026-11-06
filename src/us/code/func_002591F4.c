#include "common.h"

#define ABS(x) ((x) < 0.0f ? -(x) : (x))

/* A spinning part driven by an AI callback (minimal layouts). */
typedef struct {
    /* 0x000 */ u8 pad0[0x34];
    /* 0x034 */ s8 mode;
    /* 0x035 */ u8 pad35[0x10C - 0x35];
    /* 0x10C */ f32 angle;
    /* 0x110 */ f32 spin;
} SpinState;

typedef struct {
    /* 0x000 */ u8 pad0[0x168];
    /* 0x168 */ f32 rate;
} SpinTarget;

typedef struct {
    /* 0x000 */ u8 pad0[0xA50];
    /* 0xA50 */ SpinTarget *target;
} SpinOwner;

typedef struct {
    /* 0x000 */ u8 pad0[0x1A8];
    /* 0x1A8 */ SpinOwner *owner;
} SpinObj;

f32 Math_DecayToZero(f32 value, f32 rate);
void Sound__StopSoundId(s32 sound);

extern u32 D_800B6D20;
extern f32 D_800B6D28;

void WeaponBarrel_Spin_AF324(SpinObj *obj, SpinState *s) {
    SpinOwner *owner = obj->owner;
    SpinTarget *target;

    if (s->mode != 4) {
        s->spin = Math_DecayToZero(s->spin, 0.0065449853f);
    }
    s->angle += s->spin * D_800B6D28 * 2;
    target = owner->target;
    target->rate = (ABS(s->spin) * 1.7904929f > 1.0f) ? 1.0f : ABS(s->spin) * 1.7904929f;
    if (s->mode != 4 && (D_800B6D20 & 1)) {
        Sound__StopSoundId(0x1A7);
        Sound__StopSoundId(0x1A5);
    }
}
