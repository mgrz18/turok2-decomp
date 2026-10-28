#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x1C];
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ u8 pad20[0x34 - 0x20];
    /* 0x34 */ f32 scale[3];
} Self;

typedef struct {
    /* 0x000 */ u8 pad0[0x4];
    /* 0x004 */ s32 active;
    /* 0x008 */ u8 pad8[0x34 - 0x8];
    /* 0x034 */ s8 state;
    /* 0x035 */ u8 pad35[0x40 - 0x35];
    /* 0x040 */ f32 timer;
    /* 0x044 */ u8 pad44[0x6C - 0x44];
    /* 0x06C */ f32 wobble[3];
    /* 0x078 */ u8 pad78[0x10C - 0x78];
    /* 0x10C */ f32 roll;
    /* 0x110 */ u8 pad110[0x11C - 0x110];
    /* 0x11C */ f32 baseScale;
    /* 0x120 */ u8 pad120[0x128 - 0x120];
    /* 0x128 */ f32 wobbleTimer;
} AI;

extern f32 D_800B6D28; /* frame time */

f32 func_00211774(f32 cur, f32 target, f32 rate);
f32 func_002974C0(f32 x);
f32 func_002119FC(f32 lo, f32 hi);

void func_00261490(Self *self, AI *ai) {
    f32 roll;
    f32 base;
    f32 c;

    roll = -self->unk1C * 0.030543264f;
    if (roll > 1.308997f) {
        roll = 1.308997f;
    } else if (roll < -1.308997f) {
        roll = -1.308997f;
    }
    ai->roll = func_00211774(ai->roll, roll, 0.06981318f);

    if (ai->active != 0) {
        if (ai->state == 27) {
            c = func_002974C0(ai->timer * 0.31415930f);
            base = ai->baseScale;
            base += (c + 1.0f) * ((base * 1.25f - base) * 0.5f);
            self->scale[0] = func_00211774(self->scale[0], base * 3.0f * 2.0f, 0.1f);
            self->scale[1] = func_00211774(self->scale[1], base * 1.25f * 2.0f, 0.1f);
            self->scale[2] = func_00211774(self->scale[2], base * 1.25f * 2.0f, 0.1f);
        } else {
            self->scale[0] = func_00211774(self->scale[0], ai->baseScale, 0.1f);
            self->scale[1] = func_00211774(self->scale[1], ai->baseScale, 0.1f);
            self->scale[2] = func_00211774(self->scale[2], ai->baseScale, 0.1f);
        }
    }

    ai->wobbleTimer -= D_800B6D28;
    if (ai->wobbleTimer <= 0.0f) {
        ai->wobbleTimer = func_002119FC(60.0f, 120.0f);
        ai->wobble[0] = func_002119FC(-51.199997f, 51.199997f);
        ai->wobble[1] = func_002119FC(-51.199997f, 51.199997f);
        ai->wobble[2] = func_002119FC(-51.199997f, 51.199997f);
    }
}
