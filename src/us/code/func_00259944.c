#include "common.h"

/* Weapon spin: turns the barrel and makes the controller rumble with the spin speed. */

typedef struct {
    /* 0x000 */ u8 pad0[0x168];
    /* 0x168 */ f32 rumble;
} Controller;

typedef struct {
    /* 0x000 */ u8 pad0[0xA50];
    /* 0xA50 */ Controller *controller;
} WeaponPlayer;

typedef struct {
    /* 0x000 */ u8 pad0[0x34];
    /* 0x034 */ s8 mode;
    /* 0x035 */ u8 pad35[0x10C - 0x35];
    /* 0x10C */ f32 angle;
    /* 0x110 */ f32 speed;
    /* 0x114 */ u8 pad114[0x120 - 0x114];
    /* 0x120 */ f32 time;
} WeaponState;

extern f32 D_800B6D28; /* frame time scale */

extern f32 func_00211774(f32 value, f32 target, f32 rate);
extern void func_002117DC(f32 *value, f32 target, f32 rate);
extern f32 func_002982F0(f32 x); /* sin */

void func_00259944(void *obj, WeaponState *state) {
    WeaponPlayer *player = *(WeaponPlayer **)((u8 *)obj + 0x1A8);
    f32 t;

    state->angle += state->speed * D_800B6D28;
    if (state->mode == 4) {
        func_002117DC(&state->speed, 0.47123894f, 0.025f);
    } else {
        state->speed = func_00211774(state->speed, 0.08726647496f, 0.008f);
    }
    state->time += D_800B6D28;
    t = state->speed * 2.122065783f;
    player->controller->rumble = t * t * (func_002982F0(state->angle * 1.5f) * 0.4f + 0.4f) + t * 0.05f;
}
