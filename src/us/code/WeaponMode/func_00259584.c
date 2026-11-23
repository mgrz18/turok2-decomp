#include "common.h"
#include "structs/player.h"
#include "structs/game_object.h"
#include "protos/ai.h"

s32 CPlayer__GetWeaponAmmo(void *, s16);                     /* extern */

void WeaponMode_AE694_Update(CGameObjectInstance *arg0, CAI *arg1) {
    CPlayer *temp_s0;

    temp_s0 = (CPlayer *) arg0->m_AI.m_pParent;
    if (CPlayer__GetWeaponAmmo(temp_s0, temp_s0->m_Data.m_CurrentWeapon) == 0) {
        CAI__SetMode(arg0, arg1, 2);
        return;
    }
    if (temp_s0->m_ContState.m_dwButtonsSingle & 0x10) {
        CAI__SetMode(arg0, arg1, 5);
        return;
    }
    if (!(temp_s0->m_ContState.m_dwButtons & 0x2000)) {
        CAI__SetMode(arg0, arg1, 2);
    }
    if (arg1->m_AnimRepeatedCount != 0) {
        arg1->m_ModeLastFrame = -1;
    }
}
