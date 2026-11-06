#include "common.h"
#include "structs/player.h"
#include "structs/weapon.h"
#include "globals/game.h"


#define gEngineApp_m_Game_m_DebugLines (*(s32 *) ((u8 *) &gEngineApp_m_Settings.multiplayer - 0x51))

void CPlayer__PreferAlternateWeapon(CPlayer *arg0) {
    s16 temp_a1;
    WeaponInfo *temp_v0;

    if ((*(u8 *) &gEngineApp_m_Settings.multiplayer == 0) && ((arg0->m_Barrel.m_bStickWheel == 0) || (arg0->m_Barrel.m_State == 0)) && ((gEngineApp_m_Game_m_DebugLines == 0) || !(arg0->m_ContState.m_dwButtons & 0x20)) && (arg0->m_ContState.m_dwButtonsSingle & 0x200)) {
        temp_v0 = gWeaponInfoTable[arg0->m_RequestedWeapon];
        temp_a1 = temp_v0->alternate;
        if ((temp_a1 != -1) && (arg0->m_Data.m_WeaponList[temp_a1] != 0)) {
            arg0->m_RequestedWeapon = (s16) (u16) temp_v0->alternate;
        }
    }
}
