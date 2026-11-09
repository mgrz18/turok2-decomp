#include "common.h"
#include "structs/player.h"
#include "structs/weapon.h"

s32 CPlayer__FireWeapon(s32, s32);
s32 CPlayer__StartWeaponBackgroundSound(s32, s32);
s32 CPlayer__StartWeaponFiringSound(s32, s32);


void WeaponMode_AE634_Enter(CGameObjectInstance *obj, CAI *ai) {
    sptr temp_s1;

    temp_s1 = (sptr) obj->m_AI.m_pParent;
    ai->m_ConditionTimer = 0.0f;
    CPlayer__FireWeapon((sptr) obj->m_AI.m_pParent, 0x12B);
    CPlayer__StartWeaponBackgroundSound(temp_s1, 0x1A5);
    CPlayer__StartWeaponFiringSound(temp_s1, 0x1A7);
    *(f32 *) &ai->m_VarBuffer[3] = gWeaponInfoTable[((CPlayer *) obj->m_AI.m_pParent)->m_Data.m_CurrentWeapon]->fireDelay * 15.0f;
}
