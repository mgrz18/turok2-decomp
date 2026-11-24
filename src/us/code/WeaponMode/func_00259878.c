#include "common.h"
#include "structs/game_object.h"
#include "structs/player.h"
#include "structs/weapon.h"
#include "protos/ai.h"
#include "protos/weapon_mode.h"


/* Host body: the State view's 0x110 is m_Weapon.m_BarrelSpeed, 0x118 m_Weapon.m_FireDelay. */
void CPlayer__StopWeaponBackgroundSound(CPlayer *player);

void WeaponMode_AE794_Update(CGameObjectInstance *actor, CAI *state) {
    CPlayer *player = ((CPlayer *)actor->m_AI.m_pParent);

    if (player->m_pLoopingSound_WeaponBackground == 0) {
        CPlayer__StartWeaponBackgroundSound(player, 0x464);
    }
    if (!(player->m_ContState.m_dwButtons & 0x2000)) {
        CPlayer__StopWeaponBackgroundSound(player);
        if (state->m_Weapon.m_BarrelSpeed < 0.27925270795822144f /* 16 deg */ || state->m_Weapon.m_FireDelay > 0.0f) {
            CAI__SetMode(actor, state, 2);
        } else {
            state->m_Weapon.m_FireDelay = gWeaponInfoTable[22]->fireDelay * 15.0f;
            CAI__SetMode(actor, state, 5);
        }
    }
}
