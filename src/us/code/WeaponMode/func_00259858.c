#include "common.h"
#include "structs/game_object.h"
#include "protos/weapon_mode.h"


void WeaponMode_AE794_Enter(CGameObjectInstance *arg0) {
    CPlayer__StartWeaponBackgroundSound((sptr) arg0->m_AI.m_pParent, 0x464);
}
