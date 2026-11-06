#include "common.h"
#include "structs/game_object.h"
#include "globals/ai.h"

s32 AI_TurnClamped(s32, void *, f32, f32);       /* extern */
f32 AI_AngleToPoint(s32, CVector3);               /* angle from the actor heading to a point */


void EnemyAI_TurnToPoint(s32 arg0, CAI *arg1) {
    f32 var_f20;

    var_f20 = 0.26179942f;
    if ((u32) ((u8) arg1->m_ModeBefore - 1) < 2U) {
        var_f20 = 1.0471977f;
    }
    AI_TurnClamped(arg0, arg1, var_f20, AI_AngleToPoint(arg0, gEnemyAlertPoint));
}
