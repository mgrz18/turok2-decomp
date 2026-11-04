#include "common.h"
#include "structs/player.h"
#include "structs/weapon.h"

#define P_S32(p, off) (*(s32 *)((u8 *)(p) + (off)))

extern s32 func_00257EA0(CGameObjectInstance *weapon, CAI *state);
extern s32 func_0024E700(CPlayer *player, s32 weapon);
extern void func_00243414(CGameObjectInstance *weapon, CAI *state, s32 arg2);

void func_00258A98(CGameObjectInstance *weapon, CAI *state) {
    CPlayer *player = *(CPlayer **)((u8 *)weapon + 0x1A8);

    P_S32(player, 0xB2C) = 1;
    P_S32(player, 0xB38) = 0;
    P_S32(player, 0xB30) = 0;
    state->m_Weapon.m_BarrelAngle = 0;
    state->m_Weapon.m_BarrelSpeed = 0;
    if (state->m_AnimRepeatedCount != 0) {
        if ((D_800AF618[player->m_Data.m_CurrentWeapon]->flags & 4) && func_00257EA0(weapon, state) &&
            func_0024E700(player, player->m_Data.m_CurrentWeapon) && (P_S32(player, 0xA64) & 0x2000) &&
            ((CPlayerOptions *)player->m_pOptions)->m_TagMode == 0) {
            func_00243414(weapon, state, 4);
        } else {
            func_00243414(weapon, state, 2);
        }
    }
}
