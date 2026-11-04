#include "common.h"
#include "structs/player.h"
#include "structs/weapon.h"

#define P_S32(p, off) (*(s32 *)((u8 *)(p) + (off)))

extern u32 D_800B6D20;
extern s32 D_800F7078;
extern u8 D_801198B0[];

extern void func_0027FC2C(void *arg0, void *camera, s32 model);
extern void func_00243414(CGameObjectInstance *weapon, CAI *state, s32 arg2);

/* func_00258668, inlined: switches the weapon actor to its owner's
 * requested weapon. */
static __inline__ void SwitchWeapon(CGameObjectInstance *weapon, CAI *state) {
    CPlayer *player = *(CPlayer **)((u8 *)weapon + 0x1A8);
    s16 requested = *(s16 *)((u8 *)player + 0xB14);

    if (player->m_Data.m_CurrentWeapon != requested || D_800F7078 != 4) {
        player->m_Data.m_CurrentWeapon = *(s16 *)((u8 *)player + 0xB14);
        state->m_pModeTable = D_800AF618[requested]->modeTable;
        state->m_pfEffects = D_800AF618[player->m_Data.m_CurrentWeapon]->effects;
        if (D_800B6D20 >= 5) {
            func_0027FC2C(D_801198B0, player->m_pCamera, D_800AF618[player->m_Data.m_CurrentWeapon]->name);
        }
        state->m_Weapon.m_BarrelAngle = 0;
        state->m_Weapon.m_BarrelSpeed = 0;
        state->m_Weapon.m_nLod = 0;
        state->m_Weapon.m_FireDelay = 0;
    }
}

void func_00258754(CGameObjectInstance *weapon, CAI *state) {
    SwitchWeapon(weapon, state);
    func_00243414(weapon, state, 0);
}
