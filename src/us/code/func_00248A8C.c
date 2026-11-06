#include "common.h"
#include "structs/player.h"
#include "structs/weapon.h"

extern u8 gEngineApp_m_Settings_multiplayer;            /* multiplayer */

/* Takes every weapon and all ammo away, then gives the starting kit: in
 * multiplayer by character (10 gets weapon 0x22), otherwise the
 * knife-and-bow set with 20 rounds. */
void CPlayer__GiveStartingWeapons(CPlayer *p) {
    s32 i;

    for (i = 0; i < 0x1F; i++) {
        p->m_Data.m_AmmoList[i] = 0;
    }
    for (i = 0; i < 0x23; i++) {
        p->m_Data.m_WeaponList[i] = 0;
    }
    if (gEngineApp_m_Settings_multiplayer) {
        if (p->m_Data.m_nDeathmatchCharacter == 10) {
            p->m_Data.m_WeaponList[0x22] = 1;
            p->m_Data.m_LastWeapon[0] = 0x22;
            p->m_Data.m_LastWeapon[1] = 0x22;
            p->m_Data.m_LastWeapon[2] = -1;
            p->m_RequestedWeapon = 0x22;
            p->m_Data.m_CurrentWeapon = 0x22;
        } else {
            p->m_Data.m_WeaponList[0x18] = 1;
            p->m_Data.m_WeaponList[0x20] = 1;
            p->m_Data.m_LastWeapon[0] = 0x18;
            p->m_Data.m_LastWeapon[1] = 0x20;
            p->m_Data.m_LastWeapon[2] = -1;
            p->m_RequestedWeapon = 0x18;
            p->m_Data.m_CurrentWeapon = 0x18;
        }
    } else {
        p->m_Data.m_WeaponList[0] = 1;
        p->m_Data.m_WeaponList[2] = 1;
        p->m_Data.m_WeaponList[0x17] = 1;
        p->m_Data.m_AmmoList[0] = 0x14;
        p->m_Data.m_AmmoList[0x15] = 1;
        p->m_Data.m_AmmoList[0x17] = 1;
        p->m_RequestedWeapon = 0;
        p->m_Data.m_CurrentWeapon = 0;
        p->m_Data.m_LastWeapon[0] = 0;
        p->m_Data.m_LastWeapon[1] = 0;
        p->m_Data.m_LastWeapon[2] = 0x13;
    }
    if (gEngineApp_m_Settings_multiplayer) {
        p->m_WeaponActor.m_AI.m_pModeTable = gWeaponInfoTable[p->m_RequestedWeapon]->modeTable;
        p->m_WeaponActor.m_AI.m_pfEffects = gWeaponInfoTable[p->m_Data.m_CurrentWeapon]->effects;
    }
}
