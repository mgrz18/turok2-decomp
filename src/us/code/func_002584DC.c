#include "common.h"
#include "structs/game_object.h"
#include "structs/player.h"
#include "globals/engine.h"

/* Weapon state for weapon 0x14: drains ammo while firing, every third tick. */


extern s32 CPlayer__GetWeaponAmmo(CPlayer *player, s32 weapon);
extern s32 CAI__SetMode(void *obj, CAI *state, s32 mode);
extern s16 *CPlayer__GetFiringAmmo(CPlayer *player, s32 weapon);
extern void CPlayer__StopWeaponFiringSound(CPlayer *player);
extern s32 CPlayer__FireWeapon(void *player, s32 arg1);

#define USE_AMMO()                                              {                                                               ammo = &player->m_Data.m_AmmoList[*CPlayer__GetFiringAmmo(player, 0x14)];         n = *ammo - 1;                                              if (n < 0) {                                                    n = 0;                                                  }                                                           *ammo = n;                                              }

void WeaponMode_StreamFire_Update(void *obj, CAI *state) {
    CPlayer *player = (CPlayer *) ((CGameObjectInstance *) obj)->m_AI.m_pParent;
    s16 *ammo;
    s32 n;

    if (!(player->m_ContState.m_dwButtons & 0x2000) || CPlayer__GetWeaponAmmo(player, 0x14) == 0) {
        if (state->m_ModeTime < 3.0f) {
            USE_AMMO();
        }
        CPlayer__StopWeaponFiringSound(player);
        CAI__SetMode(obj, state, 2);
        return;
    }
    if (state->m_ModeTime > 30.0f) {
        player->m_FleeTime = 45.0f;
    }
    if (gEvenOdd != 0) {
        state->m_ConditionTimer += 1.5f;
        CPlayer__FireWeapon(((CGameObjectInstance *) obj)->m_AI.m_pParent, 0x14);
    }
    if (gFrameCountGameplay % 3 == 0) {
        USE_AMMO();
    }
}
