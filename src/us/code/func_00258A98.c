#include "common.h"
#include "structs/player.h"
#include "structs/weapon.h"
#include "protos/ai.h"

#define P_S32(p, off) (*(s32 *)((u8 *)(p) + (off)))

extern s32 WeaponActor_CheckFire(CGameObjectInstance *weapon, CAI *state);
extern s32 CPlayer__GetWeaponAmmo(CPlayer *player, s32 weapon);

void WeaponMode_Switch_Update(CGameObjectInstance *weapon, CAI *state) {
    CPlayer *player = *(CPlayer **)((u8 *)weapon + 0x1A8);

    P_S32(player, 0xB2C) = 1;
    P_S32(player, 0xB38) = 0;
    P_S32(player, 0xB30) = 0;
    state->m_Weapon.m_BarrelAngle = 0;
    state->m_Weapon.m_BarrelSpeed = 0;
    if (state->m_AnimRepeatedCount != 0) {
        if ((gWeaponInfoTable[player->m_Data.m_CurrentWeapon]->flags & 4) && WeaponActor_CheckFire(weapon, state) &&
            CPlayer__GetWeaponAmmo(player, player->m_Data.m_CurrentWeapon) && (P_S32(player, 0xA64) & 0x2000) &&
            ((CPlayerOptions *)player->m_pOptions)->m_TagMode == 0) {
            CAI__SetMode(weapon, state, 4);
        } else {
            CAI__SetMode(weapon, state, 2);
        }
    }
}
