#include "common.h"
#include "structs/player.h"
#include "structs/game_object.h"

s32 func_00243414(void *, void *, s32);     /* extern */
s32 func_0024E700(void *, s16);                     /* extern */

void func_00259584(CGameObjectInstance *arg0, CAI *arg1) {
    CPlayer *temp_s0;

    temp_s0 = (CPlayer *) arg0->m_AI.m_pParent;
    if (func_0024E700(temp_s0, temp_s0->m_Data.m_CurrentWeapon) == 0) {
        func_00243414(arg0, arg1, 2);
        return;
    }
    if (temp_s0->m_ContState.m_dwButtonsSingle & 0x10) {
        func_00243414(arg0, arg1, 5);
        return;
    }
    if (!(temp_s0->m_ContState.m_dwButtons & 0x2000)) {
        func_00243414(arg0, arg1, 2);
    }
    if (arg1->m_AnimRepeatedCount != 0) {
        arg1->m_ModeLastFrame = -1;
    }
}
