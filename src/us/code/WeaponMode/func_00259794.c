#include "common.h"
#include "structs/game_object.h"
#include "structs/player.h"
#include "structs/weapon.h"
#include "protos/weapon_mode.h"


/* Host body: the State view's 0x64 is m_ConditionTimer (cleared as a word), 0x118 m_Weapon.m_FireDelay. */

void WeaponMode_AE714_Enter(CGameObjectInstance *actor, CAI *state) {
    CPlayer *player = ((CPlayer *)actor->m_AI.m_pParent);

    *(s32 *) &state->m_ConditionTimer = 0;
    state->m_Weapon.m_FireDelay = gWeaponInfoTable[((CPlayer *)actor->m_AI.m_pParent)->m_Data.m_CurrentWeapon]->fireDelay * 15.0f;
    CPlayer__StartWeaponFiringSound(player, 0x1A9);
}
