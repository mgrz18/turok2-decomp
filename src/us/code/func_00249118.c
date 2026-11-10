#include "common.h"
#include "structs/vector.h"
#include "structs/player.h"
#include "structs/weapon.h"
#include "globals/game.h"
#include "protos/player.h"
#include "protos/game.h"



void CPlayer__Dismount(CPlayer *player, s32 arg1);
void CPlayer__StopLoopingSounds(CPlayer *player);
void GameObject_DropPickupAt(CPlayer *player, CVector3 pos, s32 id, f32 arg);
void Sound__Play2D(s32 sound);

static __inline__ s32 fallingFast(CPlayer *player) {
    return player->m_DuckOffset > 5.12f;
}

static __inline__ s32 inWater(CPlayer *player) {
    return player->m_Mode == 14;
}

/* Kills the player: drops the weapon in multiplayer and plays a death
   animation picked from the cause (`flags`) unless `anim` is given. */
void CPlayer__Kill(CPlayer *player, sptr arg1, s32 flags, s32 anim) {
    s16 weapon;
    s16 mode;

    Controller_SetRumbleOption(player->m_ContState.m_pContState, 0);
    CPlayer__Dismount(player, arg1);
    GameObject_FireTriggers(arg1, 1, arg1);
    CPlayer__StopLoopingSounds(player);
    if (gEngineApp_m_Settings.multiplayer != 0) {
        weapon = player->m_Data.m_CurrentWeapon;
        if (weapon != 32 && weapon != 24 && weapon != 34 && player->m_pOptions->m_TagMode == 0) {
            GameObject_DropPickupAt(player, player->m_CharacterActor.header.m_vPos, gWeaponInfoTable[weapon]->pickupId, 150.0f);
        }
        player->m_ShockedTimer = 0.0f;
        player->m_CharacterActor.m_dwFlags &= ~0x800000;
        if (player->m_pOptions->m_TagMode == 1) {
            Sound__Play2D(0x27D9);
        }
    }
    if (anim != -1) {
        CPlayer__SetState(player, arg1, anim);
    } else if (fallingFast(player)) {
        CPlayer__SetState(player, arg1, 0x19);
    } else {
        mode = player->m_Mode;
        if (inWater(player)) {
            CPlayer__SetState(player, arg1, 0x1E);
        } else if (mode == 8) {
            CPlayer__SetState(player, arg1, 0x1C);
        } else if (mode == 9) {
            CPlayer__SetState(player, arg1, 0x1C);
        } else if (mode == 10) {
            CPlayer__SetState(player, arg1, 0x1C);
        } else if (mode == 11) {
            CPlayer__SetState(player, arg1, 0x1C);
        } else if (flags & 0x2000000) {
            CPlayer__SetState(player, arg1, 0x1C);
        } else if (flags & 0x1000000) {
            CPlayer__SetState(player, arg1, 0x18);
        } else if (flags & 0x400000) {
            CPlayer__SetState(player, arg1, 0x16);
        } else {
            CPlayer__SetState(player, arg1, 0x13);
        }
    }
}
