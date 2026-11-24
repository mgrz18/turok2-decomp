#include "common.h"
#include "structs/game_object.h"
#include "globals/player.h"
#include "protos/ai.h"
#include "protos/weapon_mode.h"

s32 CPlayer__GetNextUsableWeapon(sptr);
s32 WeaponActor_CheckFire(sptr, sptr);


/* Host body: the parent is the CPlayer (N64 0x996 m_Data.m_CurrentWeapon, 0xA08 m_Mode,
 * 0xB14 m_RequestedWeapon). */
#include "structs/player.h"
void WeaponMode_Ready_Update(CGameObjectInstance *arg0, sptr arg1) {
    s32 temp_s2;
    CPlayer *temp_s0;

    temp_s0 = (CPlayer *)arg0->m_AI.m_pParent;
    temp_s2 = gPlayerStateTable[temp_s0->m_Mode].pose;
    if (CPlayer__GetWeaponAmmo((sptr) temp_s0, (s32) temp_s0->m_Data.m_CurrentWeapon) == 0) {
        temp_s0->m_RequestedWeapon = CPlayer__GetNextUsableWeapon((sptr) temp_s0);
    }
    if ((WeaponActor_CheckFire((sptr) arg0, arg1) == 0) && !(arg0->m_dwFlags & 0x400)) {
        CAI__SetMode(arg0, (CAI *) arg1, (s32) temp_s2);
    }
}
