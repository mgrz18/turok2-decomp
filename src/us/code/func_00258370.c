#include "common.h"

/* Weapon state: winding a spun-up weapon (barrel spin) while fire is held, else stopping it. */

typedef struct {
    f32 x, y, z;
} Vec3;

typedef struct {
    /* 0x000 */ u8 pad0[0x51C];
    /* 0x51C */ u8 *object;
    /* 0x520 */ u8 pad520[0x996 - 0x520];
    /* 0x996 */ s16 weapon;
    /* 0x998 */ u8 pad998[0xA64 - 0x998];
    /* 0xA64 */ s32 flags;
    /* 0xA68 */ u8 padA68[0x1284 - 0xA68];
    /* 0x1284 */ s32 unk_1284;
    /* 0x1288 */ s32 unk_1288;
    /* 0x128C */ s32 unk_128C;
} WeaponPlayer;

typedef struct {
    /* 0x000 */ u8 pad0[0x64];
    /* 0x064 */ f32 timer;
    /* 0x068 */ u8 pad68[0x110 - 0x68];
    /* 0x110 */ f32 spin;
} WeaponState;

extern f32 D_800B6D28; /* frame time scale */

extern s32 func_0024E700(WeaponPlayer *player, s32 weapon);
extern s32 func_00243414(void *obj, WeaponState *state, s32 mode);
extern s32 func_00275544(s32 sound, Vec3 position, Vec3 *source, s32 arg5);
extern void func_00253F40(WeaponPlayer *player);
extern void func_00253E68(WeaponPlayer *player);
extern s32 func_00253EB8(WeaponPlayer *player, s32 sound);
extern s32 func_00248720(void *player, s32 arg1);
extern void func_002117DC(f32 *value, f32 target, f32 rate);

void func_00258370(void *obj, WeaponState *state) {
    WeaponPlayer *player = *(WeaponPlayer **)((u8 *)obj + 0x1A8);
    Vec3 *pos = (Vec3 *)(player->object + 0x114);

    if (!(player->flags & 0x2000) || player->unk_1284 != 0
        || func_0024E700(player, player->weapon) == 0) {
        func_00243414(obj, state, 2);
        if (player->unk_128C != 0) {
            func_00275544(0x1B5, *pos, pos, -1);
        }
        func_00253F40(player);
        func_00253E68(player);
        func_00275544(0x1A6, *pos, pos, -1);
        return;
    }
    func_002117DC(&state->spin, 0.3490659f, 0.2f);
    if (state->spin < 0.2617994249f) {
        return;
    }
    if (player->unk_128C == 0) {
        func_00253EB8(player, 0x1A7);
    }
    state->timer -= D_800B6D28;
    if (state->timer <= 0.0f) {
        state->timer += 1.875f;
        func_00248720(*(void **)((u8 *)obj + 0x1A8), 0x1A);
    }
}
