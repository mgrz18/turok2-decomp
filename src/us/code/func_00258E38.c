#include "common.h"
#include "structs/player.h"
#include "structs/weapon.h"

s32 CPlayer__StartWeaponBackgroundSound(sptr, s32);


void WeaponMode_SpinUp_Enter(CGameObjectInstance *obj, CAI *ai) {
    sptr player;

    player = (sptr) obj->m_AI.m_pParent;
    ai->m_ConditionTimer = 0.0f;
    CPlayer__StartWeaponBackgroundSound(player, 0x1A5);
    *(f32 *) &ai->m_VarBuffer[3] = gWeaponInfoTable[((CPlayer *) obj->m_AI.m_pParent)->m_Data.m_CurrentWeapon]->fireDelay * 15.0f;
}
