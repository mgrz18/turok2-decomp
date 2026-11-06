#include "common.h"
#include "structs/game_object.h"


s32 CPlayer__GetWeaponAmmo(void *player, s32 weapon);
void CAI__SetMode(CGameObjectInstance *actor, void *state, s32 mode);

void WeaponMode_Weapon1FFire_Enter(CGameObjectInstance *actor, void *state) {
    if (CPlayer__GetWeaponAmmo(actor->m_AI.m_pParent, 0x1F) == 1) {
        CAI__SetMode(actor, state, 7);
    } else {
        CAI__SetMode(actor, state, 4);
    }
}
