#include "common.h"

#define MIN(a, b) ((a) > (b) ? (b) : (a))

typedef struct {
    /* 0x000 */ u8 pad0[0x168];
    /* 0x168 */ f32 rumble;
} Controller;

typedef struct {
    /* 0x0000 */ u8 pad0[0xA50];
    /* 0x0A50 */ Controller *controller;
    /* 0x0A54 */ u8 padA54[0xA64 - 0xA54];
    /* 0x0A64 */ s32 flags;
    /* 0x0A68 */ u8 padA68[0x1294 - 0xA68];
    /* 0x1294 */ f32 unk1294;
} WeaponPlayer;

typedef struct {
    /* 0x000 */ u8 pad0[0x40];
    /* 0x040 */ f32 time;
} WeaponState;

typedef struct {
    /* 0x000 */ u8 pad0[0x1A8];
    /* 0x1A8 */ WeaponPlayer *player;
} Owner;

extern s16 D_800AE9D2;

extern f32 func_002119FC(f32 lo, f32 hi); /* random in range */
extern s32 func_00243414(void *obj, WeaponState *state, s32 mode);

/* Weapon state: on release, charges by the time held (0.1 + t/45, at most 1)
 * and sets the shot count D_800AE9D2 (charge * 3, at least 1), then goes to
 * state 5. */
void func_00259C4C(Owner *obj, WeaponState *state) {
    f32 base = 0.1f;
    WeaponPlayer *player = obj->player;
    f32 charge;
    f32 n;
    f32 t;
    s16 shots;

    player->controller->rumble = func_002119FC(base, 0.3f);
    if (player->flags & 0x2000) {
        return;
    }
    t = state->time * (1.0f / 45) + base;
    charge = MIN(t, 1.0f);
    player->unk1294 = charge;
    n = charge * 3.0f;
    shots = (n < 1.0f) ? 1 : (s32) n;
    D_800AE9D2 = shots;
    func_00243414(obj, state, 5);
    player->controller->rumble = 0.0f;
}
