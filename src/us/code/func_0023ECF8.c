/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/MENU/802ABDBC_11.CPP, func_802ACB18_de (the pickup feedback), with Turok 2's layouts. */
#include "common.h"
#include "structs/vector.h"
#include "structs/player.h"
#include "structs/weapon.h"
#include "structs/pickup.h"

extern char gEngineApp_m_Game_m_CameraPool[];
extern u8 gEngineApp_m_Settings_multiplayer;

s32 CPlayer__AddAmmo(CPlayer *player, s32 index, s32 argument);
void CCamera__StartFlash(void *ui, s32 red, s32 green, s32 blue, s32 alpha, s32 a5, s32 a6, s32 a7);
void CCameraPool__AddText(void *arg0, void *ui, void *resource);
s32 Sound__Play(s32 sound, CVector3 position, s32 arg4, s32 arg5);
void Sound__PlaySpeech(s32 callback);

/* Weapon pickup: gives the weapon, selects it when it is better than the
 * current one, and plays the descriptor's feedback. */
s32 Pickup_GiveWeapon(CPlayer *player, CPickupEntry *entry) {
    WeaponInfo *info;
    void *resource;
    s32 sound;
    s32 callback;
    s32 result;
    s32 had;
    CPlayer *p;

    result = 0;
    if (entry->m_Params.m_Weapon.m_AmmoType != -1 && player->m_Data.m_WeaponList[entry->m_Params.m_Weapon.m_Weapon] != 0 &&
        (result = CPlayer__AddAmmo(player, entry->m_Params.m_Weapon.m_AmmoType, entry->m_Params.m_Weapon.m_AmmoAmount)) == 0 &&
        (!gEngineApp_m_Settings_multiplayer || !player->m_pOptions->m_TagMode)) {
        return 0;
    }
    if (gEngineApp_m_Settings_multiplayer) {
        if (!(gWeaponInfoTable[entry->m_Params.m_Weapon.m_Weapon]->flags & 8)) {
            return 0;
        }
    } else {
        if (gWeaponInfoTable[entry->m_Params.m_Weapon.m_Weapon]->flags & 8) {
            return 0;
        }
    }
    had = player->m_Data.m_WeaponList[entry->m_Params.m_Weapon.m_Weapon];
    p = player; /* a second name for the player: needed for register allocation */
    p->m_Data.m_WeaponList[entry->m_Params.m_Weapon.m_Weapon] = 1;
    info = gWeaponInfoTable[entry->m_Params.m_Weapon.m_Weapon];
    if (p->m_Data.m_CurrentWeapon < entry->m_Params.m_Weapon.m_Weapon) {
        if ((p->m_CharacterActor.m_AI.m_dwFlags & 0x80) ? (info->flags & 2) : (info->flags & 1)) {
            if (!gEngineApp_m_Settings_multiplayer && had == 0) {
                p->m_RequestedWeapon = entry->m_Params.m_Weapon.m_Weapon;
            }
        }
    }
    sound = entry->m_Sound;
    callback = entry->m_Callback;
    resource = entry->m_pMessage;
    CCamera__StartFlash(((void *)p->m_pCamera), 0x80, 0x32, 0x32, 0x4B, 0, 0, 2);
    if (resource != NULL) {
        CCameraPool__AddText(gEngineApp_m_Game_m_CameraPool, ((void *)p->m_pCamera), resource);
    }
    if (sound != 0) {
        Sound__Play(sound, p->m_CharacterActor.header.m_vPos, 0, -1);
    }
    if (callback != 0) {
        Sound__PlaySpeech(callback);
    }
    if (result == 0) {
        CPlayer__AddAmmo(p, entry->m_Params.m_Weapon.m_AmmoType, entry->m_Params.m_Weapon.m_AmmoAmount);
    }
    return 1;
}
