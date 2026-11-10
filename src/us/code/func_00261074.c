#include "common.h"
#include "structs/game_object.h"
#include "globals/engine.h"

/* How this actor uses CAI's m_VarBuffer: two angles turned toward 0. */
typedef struct {
    /* 0x00 */ f32 angle0;
    /* 0x04 */ f32 angle1;
} AITurnVars;

f32 Math_DecayToZero(f32, f32);
s32 EnemyAI_GetBehaviorIndex(sptr, sptr);

void EnemyRam_Advance(sptr arg0, CAI *arg1) {
    f32 temp_f0;
    f32 temp_f20;
    s8 temp_v1;

    arg1->m_vLeashPos.y = (f32) arg1->m_vInitialPos.y;
    if ((EnemyAI_GetBehaviorIndex(arg0, (sptr) arg1) != 1) || ((temp_v1 = arg1->m_Mode, (temp_v1 != 4)) && (temp_v1 != 0xE))) {
        temp_f20 = 0.17453295f;
        ((AITurnVars *) arg1->m_VarBuffer)->angle1 = Math_DecayToZero(((AITurnVars *) arg1->m_VarBuffer)->angle1, temp_f20);
        ((AITurnVars *) arg1->m_VarBuffer)->angle0 = Math_DecayToZero(((AITurnVars *) arg1->m_VarBuffer)->angle0, temp_f20);
    }
    if (arg1->m_Action == 0) {
        temp_f0 = arg1->m_ActionTime - gFrameIncrement;
        arg1->m_ActionTime = temp_f0;
        if (temp_f0 < 0.0f) {
            arg1->m_Action = 1;
        }
    }
}
