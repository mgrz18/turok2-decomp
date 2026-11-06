#include "common.h"
#include "structs/game_object.h"
#include "structs/player.h"


s16 *CPlayer__GetFiringAmmo(CPlayer *player, s32 weapon);
s32 CPlayer__GetWeaponAmmo(CPlayer *player, s32 weapon);
void CAI__SetMode(CGameObjectInstance *actor, void *state, s32 mode);

void WeaponMode_Weapon10Fire_Enter(CGameObjectInstance *actor, void *state) {
    CPlayer *player = ((CPlayer *)actor->m_AI.m_pParent);
    s16 *ammo = &player->m_Data.m_AmmoList[*CPlayer__GetFiringAmmo(player, 0x10)];

    if (*ammo > 0) {
        (*ammo)--;
    }
    if (CPlayer__GetWeaponAmmo(player, 0x10) == 0) {
        CAI__SetMode(actor, state, 7);
    } else {
        CAI__SetMode(actor, state, 4);
    }
}
