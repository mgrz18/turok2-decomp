#include "common.h"

/* Weapon barrel tilt: leans toward the strafe direction while the player moves, else settles. */

typedef struct {
    /* 0x000 */ u8 pad0[0xA60];
    /* 0xA60 */ f32 speed;
    /* 0xA64 */ u8 padA64[0xA78 - 0xA64];
    /* 0xA78 */ f32 strafe;
    /* 0xA7C */ u8 padA7C[0xA80 - 0xA7C];
    /* 0xA80 */ f32 unk_A80;
    /* 0xA84 */ u8 padA84[0x1288 - 0xA84];
    /* 0x1288 */ s32 unk_1288;
} WeaponPlayer;

typedef struct {
    /* 0x000 */ u8 pad0[0x34];
    /* 0x034 */ s8 mode;
    /* 0x035 */ u8 pad35[0x10C - 0x35];
    /* 0x10C */ f32 angle;
    /* 0x110 */ f32 tilt;
} WeaponState;

extern f32 func_00211774(f32 value, f32 target, f32 rate);
extern void func_00253DE0(WeaponPlayer *player, s32 sound);
extern void func_00253E68(WeaponPlayer *player);

void func_002593D8(void *obj, WeaponState *state) {
    WeaponPlayer *player = *(WeaponPlayer **)((u8 *)obj + 0x1A8);
    f32 target;
    f32 rate;
    f32 dir;
    f32 tilt;

    if (player->speed > 0.0f && state->mode != 1) {
        target = -0.41887906f;
        if (player->strafe > 0.0f) {
            target = 0.41887906f;
        }
        dir = 1.0f;
        rate = player->unk_A80 * 0.0525f;
        if (player->strafe < 0.0f) {
            dir = -1.0f;
        }
        if (state->tilt < 0.0f ? dir != -1.0f : dir != 1.0f) {
            rate *= 4.0f;
        }
        if (player->unk_1288 == 0) {
            func_00253DE0(player, 0x467);
        }
    } else {
        rate = 0.008726647124f;
        target = -0.017453294f;
        if (state->tilt > 0.0f) {
            target = 0.017453294f;
        }
        if (player->unk_1288 != 0) {
            func_00253E68(player);
        }
    }
    tilt = func_00211774(state->tilt, target, rate);
    state->tilt = tilt;
    state->angle -= tilt;
}
