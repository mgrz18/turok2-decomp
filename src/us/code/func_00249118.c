#include "common.h"

typedef struct { f32 x, y, z; } Point3;

typedef struct PlayerOptions {
    /* 0x00 */ u8 pad0[0x3E];
    /* 0x3E */ u8 tagMode;
} PlayerOptions;

typedef struct WeaponDef {
    /* 0x00 */ u8 pad0[0xA];
    /* 0x0A */ u16 pickupId;
} WeaponDef;

typedef struct Player {
    /* 0x0000 */ u8 pad0[0x4];
    /* 0x0004 */ Point3 pos;
    /* 0x0010 */ u8 pad10[0xD4 - 0x10];
    /* 0x00D4 */ s32 flags;
    /* 0x00D8 */ u8 padD8[0x518 - 0xD8];
    /* 0x0518 */ PlayerOptions *options;
    /* 0x051C */ u8 pad51C[0x996 - 0x51C];
    /* 0x0996 */ s16 weapon;
    /* 0x0998 */ u8 pad998[0xA08 - 0x998];
    /* 0x0A08 */ s16 mode;
    /* 0x0A0A */ u8 padA0A[0xA50 - 0xA0A];
    /* 0x0A50 */ s32 unkA50;
    /* 0x0A54 */ u8 padA54[0xAC0 - 0xA54];
    /* 0x0AC0 */ f32 fallSpeed;
    /* 0x0AC4 */ u8 padAC4[0x1298 - 0xAC4];
    /* 0x1298 */ f32 health;
} Player;

extern u8 D_8011AAF1;           /* multiplayer */
extern WeaponDef *D_800AF618[];

void func_00285BD0(s32 arg0, s32 arg1);
void func_002553BC(Player *player, s32 arg1);
void func_0026E110(s32 arg0, s32 arg1, s32 arg2);
void func_0042EB04(Player *player);
void func_00268C48(Player *player, Point3 pos, s32 id, f32 arg);
void func_00275624(s32 sound);
void func_0024E5F0(Player *player, s32 arg1, s32 anim);

static __inline__ s32 fallingFast(Player *player) {
    return player->fallSpeed > 5.12f;
}

static __inline__ s32 inWater(Player *player) {
    return player->mode == 14;
}

/* Kills the player: drops the weapon in multiplayer and plays a death
   animation picked from the cause (`flags`) unless `anim` is given. */
void func_00249118(Player *player, s32 arg1, s32 flags, s32 anim) {
    s16 weapon;
    s16 mode;

    func_00285BD0(player->unkA50, 0);
    func_002553BC(player, arg1);
    func_0026E110(arg1, 1, arg1);
    func_0042EB04(player);
    if (D_8011AAF1 != 0) {
        weapon = player->weapon;
        if (weapon != 32 && weapon != 24 && weapon != 34 && player->options->tagMode == 0) {
            func_00268C48(player, player->pos, D_800AF618[weapon]->pickupId, 150.0f);
        }
        player->health = 0.0f;
        player->flags &= ~0x800000;
        if (player->options->tagMode == 1) {
            func_00275624(0x27D9);
        }
    }
    if (anim != -1) {
        func_0024E5F0(player, arg1, anim);
    } else if (fallingFast(player)) {
        func_0024E5F0(player, arg1, 0x19);
    } else {
        mode = player->mode;
        if (inWater(player)) {
            func_0024E5F0(player, arg1, 0x1E);
        } else if (mode == 8) {
            func_0024E5F0(player, arg1, 0x1C);
        } else if (mode == 9) {
            func_0024E5F0(player, arg1, 0x1C);
        } else if (mode == 10) {
            func_0024E5F0(player, arg1, 0x1C);
        } else if (mode == 11) {
            func_0024E5F0(player, arg1, 0x1C);
        } else if (flags & 0x2000000) {
            func_0024E5F0(player, arg1, 0x1C);
        } else if (flags & 0x1000000) {
            func_0024E5F0(player, arg1, 0x18);
        } else if (flags & 0x400000) {
            func_0024E5F0(player, arg1, 0x16);
        } else {
            func_0024E5F0(player, arg1, 0x13);
        }
    }
}
