#include "common.h"
#include "structs/game_object.h"

s32 CPlayer__FireWeapon(s32, s32);

/* Weapon callback: forwards to the owning player (m_AI.m_pParent). */
void WeaponActor_FireCallback(CGameObjectInstance *arg0, sptr arg1, s32 arg2) {
    CPlayer__FireWeapon((sptr)arg0->m_AI.m_pParent, arg2);
}
