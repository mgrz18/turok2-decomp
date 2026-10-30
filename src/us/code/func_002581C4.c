#include "common.h"

#define MIN(a, b) ((a) > (b) ? (b) : (a))
#define MAX(a, b) ((a) < (b) ? (b) : (a))

typedef struct {
    /* 0x000 */ u8 pad0[0x996];
    /* 0x996 */ s16 weapon;
    /* 0x998 */ u8 pad998[0xA64 - 0x998];
    /* 0xA64 */ s32 flags;
    /* 0xA68 */ u8 padA68[0xB8C - 0xA68];
    /* 0xB8C */ s32 unkB8C;
    /* 0xB90 */ u8 padB90[0x1294 - 0xB90];
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

extern f32 D_800AE350;

extern s32 func_0024E700(WeaponPlayer *player, s32 weapon);
extern s32 func_00243414(void *obj, WeaponState *state, s32 mode);

/* Weapon state: sets the fire-rate scale D_800AE350 from the time held for
 * weapons 2-3 (and their 0x1294 ramp) and 12, then goes to state 7 or 5. */
void func_002581C4(Owner *obj, WeaponState *state) {
    WeaponPlayer *player = obj->player;
    f32 t;
    f32 scale;

    if (player->flags & 0x2000) {
        return;
    }
    switch (player->weapon) {
    case 2:
    case 3:
        if (player->unkB8C) {
            D_800AE350 = 1.5f;
        } else {
            scale = MAX(MIN(state->time * (2.0f / 21), 1.5f), 0.5f);
            D_800AE350 = scale;
            t = state->time * (2.0f / 15) + 0.5f;
            if (t > 1.0f) {
                t = 1.0f;
            }
            player->unk1294 = t;
        }
        break;
    case 12:
        D_800AE350 = MAX(MIN(state->time * (2.0f / 21), 2.0f), 0.5f);
        break;
    }
    if (func_0024E700(player, player->weapon) == 1) {
        func_00243414(obj, state, 7);
    } else {
        func_00243414(obj, state, 5);
    }
}
