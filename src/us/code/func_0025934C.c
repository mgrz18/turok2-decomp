#include "common.h"
#include "structs/player.h"
#include "structs/game_object.h"

s32 func_00219F74(void *, s32, s32);        /* extern */
s32 func_0021E9F8(void *, s32, s32);        /* extern */

void func_0025934C(CGameObjectInstance *obj) {
    CPlayer *temp_s0;

    temp_s0 = (CPlayer *) obj->m_AI.m_pParent;
    if (temp_s0->m_ContState.m_dwButtons & 0x2000) {
        temp_s0->m_CharAnim = 0x1130;
        func_0021E9F8(temp_s0, func_00219F74(temp_s0, 0x1130, -1), 0);
        temp_s0->m_CharacterActor.m_CurrentAnimation.m_bCycleCompleted = 0;
    }
}
