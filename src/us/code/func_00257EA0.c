#include "common.h"
#include "structs/game_object.h"
#include "structs/player.h"

extern s32 CPlayer__GetWeaponAmmo(CPlayer *player, s32 weapon);
extern s16 *CPlayer__GetFiringAmmo(CPlayer *player, s32 weapon);
extern void CAI__SetMode(CGameObjectInstance *obj, CAI *ai, s32 mode);

/* Fire check for the first-person weapon: once its cooldown is over and the
 * weapon is usable, fire button 0x2000 starts mode 4 and a tap of 0x10 with
 * weapon 19 starts mode 5. Returns 1 when it fired. */
s32 WeaponActor_CheckFire(CGameObjectInstance *obj, CAI *ai) {
    CPlayer *player;
    s32 tapped;
    s32 mode;

    player = (CPlayer *) obj->m_AI.m_pParent;
    if (ai->m_Weapon.m_FireDelay > 0.0f) {
        return 0;
    }
    if (!CPlayer__GetWeaponAmmo(player, player->m_Data.m_CurrentWeapon)) {
        return 0;
    }
    if (player->m_pOptions->m_TagMode == 1) {
        return 0;
    }
    if ((player->m_Data.m_CurrentWeapon == 13 || player->m_Data.m_CurrentWeapon == 29) && player->m_CrossHairMode != 2) {
        return 0;
    }
    if (player->m_Data.m_CurrentWeapon == 23 && ai->m_Weapon.m_RefireDelay > 0.0f) {
        return 0;
    }
    if (player->m_bHeadMovementDisabled != 0) {
        return 0;
    }
    if (player->m_Data.m_CurrentWeapon == 21) {
        CPlayer *owner = (CPlayer *) obj->m_AI.m_pParent;
        if (*(owner->m_Data.m_AmmoList + *CPlayer__GetFiringAmmo(owner, 21)) == 0) {
            return 0;
        }
    }
    tapped = 0;
    if (player->m_ContState.m_dwButtonsSingle & 0x10) {
        tapped = player->m_Data.m_CurrentWeapon == 19;
    }
    if (player->m_ContState.m_dwButtons & 0x2000) {
        mode = 4;
    } else if (tapped) {
        mode = 5;
    } else {
        return 0;
    }
    CAI__SetMode(obj, ai, mode);
    ai->m_ModeLastFrame = -1;
    return 1;
}
