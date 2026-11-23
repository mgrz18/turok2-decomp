#include "common.h"
#include "structs/player.h"


void CPlayer__PickLandingAnim(CPlayer *arg0) {
    f32 temp_f1;
    s32 var_a1;

    var_a1 = 0;
    if (arg0->m_CharAnim == 0x1144) {
        var_a1 = (s8)arg0->m_CharacterActor.m_CurrentAnimation.m_bCycleCompleted == 0;
    }
    if (var_a1 == 0) {
        temp_f1 = arg0->m_Speed;
        if (1.024f <= temp_f1) {
            arg0->m_CharAnim = 0x8A2;
            return;
        }
        if (temp_f1 <= -1.024f) {
            arg0->m_CharAnim = 0x8A7;
            return;
        }
        arg0->m_CharAnim = 0x14;
    }
}
