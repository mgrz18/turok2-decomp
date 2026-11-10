#include "common.h"
#include "structs/player.h"
#include "structs/weapon.h"
#include "protos/ai.h"

#ifdef TARGET_PORT
/* the CPlayer words by member on the host (N64 0xB2C m_CrossHairMode, 0xB30 m_CrossHairTimer,
 * 0xB38 m_pCrossHairTarget, cleared as a pointer, 0xA64 m_ContState.m_dwButtons) */
#define PL_CROSSHAIR_MODE(p) ((p)->m_CrossHairMode)
#define PL_CROSSHAIR_TARGET(p) (*(sptr *) &(p)->m_pCrossHairTarget)
#define PL_CROSSHAIR_TIMER(p) ((p)->m_CrossHairTimer)
#define PL_BUTTONS(p) ((p)->m_ContState.m_dwButtons)
#else
#define P_S32(p, off) (*(s32 *)((u8 *)(p) + (off)))
#define PL_CROSSHAIR_MODE(p) P_S32(p, 0xB2C)
#define PL_CROSSHAIR_TARGET(p) P_S32(p, 0xB38)
#define PL_CROSSHAIR_TIMER(p) P_S32(p, 0xB30)
#define PL_BUTTONS(p) P_S32(p, 0xA64)
#endif

extern s32 WeaponActor_CheckFire(CGameObjectInstance *weapon, CAI *state);
extern s32 CPlayer__GetWeaponAmmo(CPlayer *player, s32 weapon);

void WeaponMode_Switch_Update(CGameObjectInstance *weapon, CAI *state) {
    CPlayer *player = (*(CPlayer **)&((CGameObjectInstance *)weapon)->m_AI.m_pParent);

    PL_CROSSHAIR_MODE(player) = 1;
    PL_CROSSHAIR_TARGET(player) = 0;
    PL_CROSSHAIR_TIMER(player) = 0;
    state->m_Weapon.m_BarrelAngle = 0;
    state->m_Weapon.m_BarrelSpeed = 0;
    if (state->m_AnimRepeatedCount != 0) {
        if ((gWeaponInfoTable[player->m_Data.m_CurrentWeapon]->flags & 4) && WeaponActor_CheckFire(weapon, state) &&
            CPlayer__GetWeaponAmmo(player, player->m_Data.m_CurrentWeapon) && (PL_BUTTONS(player) & 0x2000) &&
            ((CPlayerOptions *)player->m_pOptions)->m_TagMode == 0) {
            CAI__SetMode(weapon, state, 4);
        } else {
            CAI__SetMode(weapon, state, 2);
        }
    }
}
