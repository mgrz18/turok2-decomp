#include "common.h"
#include "structs/player.h"
#include "structs/weapon.h"

#define P_S32(p, off) (*(s32 *)((u8 *)(p) + (off)))

extern u32 D_800B6D20;
extern s32 gScene;
extern u8 D_801198B0[];

extern void CCameraPool__AddText(void *arg0, void *camera, s32 model);
extern void CAI__SetMode(CGameObjectInstance *weapon, CAI *state, s32 arg2);

/* WeaponActor_SwitchToRequested, inlined: switches the weapon actor to its owner's
 * requested weapon. */
static __inline__ void SwitchWeapon(CGameObjectInstance *weapon, CAI *state) {
    CPlayer *player = *(CPlayer **)((u8 *)weapon + 0x1A8);
    s16 requested = *(s16 *)((u8 *)player + 0xB14);

    if (player->m_Data.m_CurrentWeapon != requested || gScene != 4) {
        player->m_Data.m_CurrentWeapon = *(s16 *)((u8 *)player + 0xB14);
        state->m_pModeTable = D_800AF618[requested]->modeTable;
        state->m_pfEffects = D_800AF618[player->m_Data.m_CurrentWeapon]->effects;
        if (D_800B6D20 >= 5) {
            CCameraPool__AddText(D_801198B0, player->m_pCamera, D_800AF618[player->m_Data.m_CurrentWeapon]->name);
        }
        state->m_Weapon.m_BarrelAngle = 0;
        state->m_Weapon.m_BarrelSpeed = 0;
        state->m_Weapon.m_nLod = 0;
        state->m_Weapon.m_FireDelay = 0;
    }
}

void WeaponActor_SwitchToRequestedInline(CGameObjectInstance *weapon, CAI *state) {
    SwitchWeapon(weapon, state);
    CAI__SetMode(weapon, state, 0);
}
