#include "common.h"
#include "structs/game_object.h"


f32 EnemyAI_GetModeTurnRate(s32 arg0, CAI *arg1) {
    f32 var_f0;

    var_f0 = 0.26179942f;
    if ((u32) ((u8) arg1->m_ModeBefore - 1) < 2U) {
        var_f0 = 1.0471977f;
    }
    return var_f0;
}
