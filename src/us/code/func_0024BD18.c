#include "common.h"
#include "structs/player.h"
#include "structs/weapon.h"

extern s32 gEngineApp_m_Game_m_MenuPool;
extern f32 gFrameIncrement;      /* frame step */

s32 Cinema_IsActive(void);
void CBarrel__Close(CBarrel *menu);
void CBarrel__RunStick(CBarrel *menu, CPlayer *player, u32 *input);
void CBarrel__RunButtons(CBarrel *menu, CPlayer *player, u32 *input);
void func_0026E9EC(void *, s32);
s32 CBarrel__ShouldClose(CBarrel *menu, CPlayer *player);

/* Updates the player's weapon menu: runs it (by stick or by buttons, from
 * the player's control style), then slides it open (state 1), keeps it
 * open (2) until CBarrel__ShouldClose closes it, and slides it shut (3). */
void CBarrel__Update(CBarrel *menu, CPlayer *player, u32 *input) {
    if (player->m_ModeFlags & 0x8000) {
        return;
    }
    if (player->m_Data.m_nDeathmatchCharacter == 10) {
        return;
    }
    if (player->m_pOptions->m_TagMode != 0) {
        return;
    }
    if (Cinema_IsActive() != 0) {
        return;
    }
    do {
        menu->m_bStickWheel = player->m_pOptions->m_WeaponSelect == 1;
    } while (0);
    if (gEngineApp_m_Game_m_MenuPool != 0 && menu->m_State != 0) {
        CBarrel__Close(menu);
        return;
    }
    if (menu->m_bStickWheel) {
        CBarrel__RunStick(menu, player, input);
    } else {
        CBarrel__RunButtons(menu, player, input);
    }
    if (player->m_RequestedWeapon != player->m_Data.m_CurrentWeapon && player->m_pRumble != NULL) {
        func_0026E9EC(player->m_pRumble, 0);
        player->m_pRumble = NULL;
    }
    switch (menu->m_State) {
    case 1:
        menu->m_Open += gFrameIncrement * 0.25f;
        if (menu->m_Open >= 1.0f) {
            menu->m_Open = 1.0f;
            menu->m_State = 2;
        }
        break;
    case 2:
        if (CBarrel__ShouldClose(menu, player) == 0) {
            break;
        }
        menu->m_State = 3;
        player->m_bHeadMovementDisabled = 0;
        /* fall through */
    case 3:
        menu->m_Open -= gFrameIncrement * 0.25f;
        if (menu->m_Open <= 0.0f) {
            menu->m_Open = 0.0f;
            menu->m_State = 0;
        }
        break;
    }
    if (menu->m_State != 0) {
        menu->m_Glow += gFrameIncrement * 0.52359885f;
    } else {
        player->m_bHeadMovementDisabled = 0;
    }
}
