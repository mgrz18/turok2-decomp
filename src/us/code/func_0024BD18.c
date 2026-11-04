#include "common.h"
#include "structs/player.h"

typedef struct WeaponMenu {
    /* 0x000 */ s32 state;
    /* 0x004 */ u8 pad4[4];
    /* 0x008 */ f32 open;
    /* 0x00C */ u8 padC[8];
    /* 0x014 */ f32 glow;
    /* 0x018 */ u8 pad18[0x380 - 0x18];
    /* 0x380 */ s32 stickMode;
} WeaponMenu;

extern s32 D_80119E2C;
extern f32 D_800B6D28;      /* frame step */

s32 func_00284174(void);
void func_002551A0(WeaponMenu *menu);
void func_0024BAD4(WeaponMenu *menu, CPlayer *player, u32 *input);
void func_0024B5A0(WeaponMenu *menu, CPlayer *player, u32 *input);
void func_0026E9EC(void *, s32);
s32 func_00254F24(WeaponMenu *menu, CPlayer *player);

/* Updates the player's weapon menu: runs it (by stick or by buttons, from
 * the player's control style), then slides it open (state 1), keeps it
 * open (2) until func_00254F24 closes it, and slides it shut (3). */
void func_0024BD18(WeaponMenu *menu, CPlayer *player, u32 *input) {
    if (player->m_ModeFlags & 0x8000) {
        return;
    }
    if (player->m_Data.m_nDeathmatchCharacter == 10) {
        return;
    }
    if (player->m_pOptions->m_TagMode != 0) {
        return;
    }
    if (func_00284174() != 0) {
        return;
    }
    do {
        menu->stickMode = player->m_pOptions->m_WeaponSelect == 1;
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
    if (player->m_RequestedWeapon != player->m_Data.m_CurrentWeapon && player->m_pRumble != NULL) {
        func_0026E9EC(player->m_pRumble, 0);
        player->m_pRumble = NULL;
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
        player->m_bHeadMovementDisabled = 0;
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
        player->m_bHeadMovementDisabled = 0;
    }
}
