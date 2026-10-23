#include "common.h"

typedef struct {
    /* 0x000 */ u8 pad0[0x24C];
    /* 0x24C */ f32 unk_24C;
} Owner;

typedef struct {
    /* 0x00 */ Owner *owner;
    /* 0x04 */ u8 pad4[0x90 - 0x4];
    /* 0x90 */ s32 mode;
} WeaponState;

extern void func_00210894(void *arg0, f32 arg1);
extern void func_00210968(void *arg0, f32 arg1);

void func_00258D9C(void *arg0, WeaponState *state) {
    f32 value = state->owner->unk_24C;
    f32 half = value * 0.5f;
    f32 more = value * 1.5f;

    switch (state->mode) {
    case 1:
        func_00210894(arg0, half);
        break;
    case 7:
        func_00210968(arg0, more);
        break;
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
        func_00210894(arg0, -more);
        break;
    }
}
