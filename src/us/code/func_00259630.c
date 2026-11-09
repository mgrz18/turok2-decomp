#include "common.h"
#include "structs/game_object.h"
#include "structs/player.h"
#include "protos/ai.h"


s32 CPlayer__GetWeaponAmmo(CPlayer *player, s32 weapon);

void WeaponMode_AE694_Hold(CGameObjectInstance *actor, void *state) {
    CPlayer *player = ((CPlayer *)actor->m_AI.m_pParent);

    if ((player->m_ContState.m_dwButtons & 0x2000) && CPlayer__GetWeaponAmmo(player, player->m_Data.m_CurrentWeapon)) {
        CAI__SetMode(actor, state, 4);
    } else if (!(player->m_ContState.m_dwButtons & 0x10)) {
        CAI__SetMode(actor, state, 2);
    }
}
