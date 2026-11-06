#include "common.h"
#include "structs/player.h"
#include "structs/game_object.h"
#include "structs/weapon.h"


void WeaponMode_Fire_Enter(CGameObjectInstance *arg0, CAI *arg1) {
    arg1->m_Weapon.m_FireDelay = (f32) (gWeaponInfoTable[((CPlayer *) arg0->m_AI.m_pParent)->m_Data.m_CurrentWeapon]->fireDelay * 15.0f);
}
