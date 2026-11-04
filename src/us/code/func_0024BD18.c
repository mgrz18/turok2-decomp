#include "common.h"
#include "structs/player.h"
#include "structs/weapon.h"

extern s32 D_80119E2C;
extern f32 D_800B6D28;      /* frame step */

s32 func_00284174(void);
void func_002551A0(CBarrel *menu);
void func_0024BAD4(CBarrel *menu, CPlayer *player, u32 *input);
void func_0024B5A0(CBarrel *menu, CPlayer *player, u32 *input);
void func_0026E9EC(void *, s32);
s32 func_00254F24(CBarrel *menu, CPlayer *player);

/* Updates the player's weapon menu: runs it (by stick or by buttons, from
 * the player's control style), then slides it open (state 1), keeps it
 * open (2) until func_00254F24 closes it, and slides it shut (3). */
void func_0024BD18(CBarrel *menu, CPlayer *player, u32 *input) {
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
        menu->m_bStickWheel = player->m_pOptions->m_WeaponSelect == 1;
    } while (0);
    if (D_80119E2C != 0 && menu->m_State != 0) {
        func_002551A0(menu);
        return;
    }
    if (menu->m_bStickWheel) {
        func_0024BAD4(menu, player, input);
    } else {
        func_0024B5A0(menu, player, input);
    }
    if (player->m_RequestedWeapon != player->m_Data.m_CurrentWeapon && player->m_pRumble != NULL) {
        func_0026E9EC(player->m_pRumble, 0);
        player->m_pRumble = NULL;
    }
    switch (menu->m_State) {
    case 1:
        menu->m_Open += D_800B6D28 * 0.25f;
        if (menu->m_Open >= 1.0f) {
            menu->m_Open = 1.0f;
            menu->m_State = 2;
        }
        break;
    case 2:
        if (func_00254F24(menu, player) == 0) {
            break;
        }
        menu->m_State = 3;
        player->m_bHeadMovementDisabled = 0;
        /* fall through */
    case 3:
        menu->m_Open -= D_800B6D28 * 0.25f;
        if (menu->m_Open <= 0.0f) {
            menu->m_Open = 0.0f;
            menu->m_State = 0;
        }
        break;
    }
    if (menu->m_State != 0) {
        menu->m_Glow += D_800B6D28 * 0.52359885f;
    } else {
        player->m_bHeadMovementDisabled = 0;
    }
}
