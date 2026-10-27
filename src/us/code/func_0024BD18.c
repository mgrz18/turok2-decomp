#include "common.h"

typedef struct WeaponMenu {
    /* 0x000 */ s32 state;
    /* 0x004 */ u8 pad4[4];
    /* 0x008 */ f32 open;
    /* 0x00C */ u8 padC[8];
    /* 0x014 */ f32 glow;
    /* 0x018 */ u8 pad18[0x380 - 0x18];
    /* 0x380 */ s32 stickMode;
} WeaponMenu;

typedef struct PlayerOptions {
    /* 0x00 */ u8 pad0[0x31];
    /* 0x31 */ u8 controlStyle;
    /* 0x32 */ u8 pad32[0x3E - 0x32];
    /* 0x3E */ u8 unk3E;
} PlayerOptions;

typedef struct Player {
    /* 0x000 */ u8 pad0[0x518];
    /* 0x518 */ PlayerOptions *options;
    /* 0x51C */ u8 pad51C[4];
    /* 0x520 */ s32 mode;
    /* 0x524 */ u8 pad524[0x996 - 0x524];
    /* 0x996 */ s16 weapon;
    /* 0x998 */ u8 pad998[0xA1C - 0x998];
    /* 0xA1C */ u32 flags;
    /* 0xA20 */ u8 padA20[0xB14 - 0xA20];
    /* 0xB14 */ s16 nextWeapon;
    /* 0xB16 */ u8 padB16[0x1280 - 0xB16];
    /* 0x1280 */ void *unk1280;
    /* 0x1284 */ s32 unk1284;
} Player;

extern s32 D_80119E2C;
extern f32 D_800B6D28;      /* frame step */

s32 func_00284174(void);
void func_002551A0(WeaponMenu *menu);
void func_0024BAD4(WeaponMenu *menu, Player *player, u32 *input);
void func_0024B5A0(WeaponMenu *menu, Player *player, u32 *input);
void func_0026E9EC(void *, s32);
s32 func_00254F24(WeaponMenu *menu, Player *player);

/* Updates the player's weapon menu: runs it (by stick or by buttons, from
 * the player's control style), then slides it open (state 1), keeps it
 * open (2) until func_00254F24 closes it, and slides it shut (3). */
void func_0024BD18(WeaponMenu *menu, Player *player, u32 *input) {
    if (player->flags & 0x8000) {
        return;
    }
    if (player->mode == 10) {
        return;
    }
    if (player->options->unk3E != 0) {
        return;
    }
    if (func_00284174() != 0) {
        return;
    }
    do {
        menu->stickMode = player->options->controlStyle == 1;
    } while (0);
    if (D_80119E2C != 0 && menu->state != 0) {
        func_002551A0(menu);
        return;
    }
    if (menu->stickMode) {
        func_0024BAD4(menu, player, input);
    } else {
        func_0024B5A0(menu, player, input);
    }
    if (player->nextWeapon != player->weapon && player->unk1280 != NULL) {
        func_0026E9EC(player->unk1280, 0);
        player->unk1280 = NULL;
    }
    switch (menu->state) {
    case 1:
        menu->open += D_800B6D28 * 0.25f;
        if (menu->open >= 1.0f) {
            menu->open = 1.0f;
            menu->state = 2;
        }
        break;
    case 2:
        if (func_00254F24(menu, player) == 0) {
            break;
        }
        menu->state = 3;
        player->unk1284 = 0;
        /* fall through */
    case 3:
        menu->open -= D_800B6D28 * 0.25f;
        if (menu->open <= 0.0f) {
            menu->open = 0.0f;
            menu->state = 0;
        }
        break;
    }
    if (menu->state != 0) {
        menu->glow += D_800B6D28 * 0.52359885f;
    } else {
        player->unk1284 = 0;
    }
}
