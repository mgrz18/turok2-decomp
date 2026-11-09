#include "common.h"
#include "structs/game_object.h"

void CPlayer__StartWeaponBackgroundSound(s32, s32);                       /* extern */

void WeaponMode_AE794_Enter(CGameObjectInstance *arg0) {
    CPlayer__StartWeaponBackgroundSound((sptr) arg0->m_AI.m_pParent, 0x464);
}
