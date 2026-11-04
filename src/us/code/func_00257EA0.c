#include "common.h"
#include "structs/game_object.h"
#include "structs/player.h"

typedef struct {
    /* 0x000 */ u8 pad0[0x1A8];
    /* 0x1A8 */ CPlayer *owner;
} Object;

extern s32 func_0024E700(CPlayer *player, s32 weapon);
extern s16 *func_0025398C(CPlayer *player, s32 weapon);
extern void func_00243414(Object *obj, CAI *ai, s32 mode);

/* Fire check for the first-person weapon: once its cooldown is over and the
 * weapon is usable, fire button 0x2000 starts mode 4 and a tap of 0x10 with
 * weapon 19 starts mode 5. Returns 1 when it fired. */
s32 func_00257EA0(Object *obj, CAI *ai) {
    CPlayer *player;
    s32 tapped;
    s32 mode;

    player = obj->owner;
    if (ai->m_VarF32[3] > 0.0f) {
        return 0;
    }
    if (!func_0024E700(player, player->m_Data.m_CurrentWeapon)) {
        return 0;
    }
    if (player->m_pOptions->m_TagMode == 1) {
        return 0;
    }
    if ((player->m_Data.m_CurrentWeapon == 13 || player->m_Data.m_CurrentWeapon == 29) && player->m_CrossHairMode != 2) {
        return 0;
    }
    if (player->m_Data.m_CurrentWeapon == 23 && ai->m_VarF32[9] > 0.0f) {
        return 0;
    }
    if (player->m_bHeadMovementDisabled != 0) {
        return 0;
    }
    if (player->m_Data.m_CurrentWeapon == 21) {
        CPlayer *owner = obj->owner;
        if (*(owner->m_Data.m_AmmoList + *func_0025398C(owner, 21)) == 0) {
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
    func_00243414(obj, ai, mode);
    ai->m_ModeLastFrame = -1;
    return 1;
}
