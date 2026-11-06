#include "common.h"
#include "structs/game_object.h"
#include "globals/engine.h"

f32 Math_DecayToZero(f32, f32);
s32 EnemyAI_GetBehaviorIndex(s32, s32);


void EnemyHover_Advance(s32 arg0, CAI *ai) {
    f32 temp_f0;
    f32 temp_f20;

    if ((EnemyAI_GetBehaviorIndex(arg0, (s32) ai) != 1) || (ai->m_Mode != 4)) {
        temp_f20 = 0.17453295f;
        *(f32 *) &ai->m_VarBuffer[1] = Math_DecayToZero(*(f32 *) &ai->m_VarBuffer[1], temp_f20);
        *(f32 *) &ai->m_VarBuffer[0] = Math_DecayToZero(*(f32 *) &ai->m_VarBuffer[0], temp_f20);
    }
    if (ai->m_Action == 0) {
        temp_f0 = ai->m_ActionTime - gFrameIncrement;
        ai->m_ActionTime = temp_f0;
        if (temp_f0 < 0.0f) {
            ai->m_Action = 1;
        }
    }
}
