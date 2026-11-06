#include "common.h"
#include "structs/player.h"
#include "structs/game_object.h"

s32 CGameObjectInstance__PickRandomAnim(void *, s32, s32);        /* extern */
s32 CGameObjectInstance__SetAnim(void *, s32, s32);        /* extern */

void WeaponActor_PlayChargeAnim(CGameObjectInstance *obj) {
    CPlayer *temp_s0;

    temp_s0 = (CPlayer *) obj->m_AI.m_pParent;
    if (temp_s0->m_ContState.m_dwButtons & 0x2000) {
        temp_s0->m_CharAnim = 0x1130;
        CGameObjectInstance__SetAnim(temp_s0, CGameObjectInstance__PickRandomAnim(temp_s0, 0x1130, -1), 0);
        temp_s0->m_CharacterActor.m_CurrentAnimation.m_bCycleCompleted = 0;
    }
}
