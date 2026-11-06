#include "common.h"
#include "structs/game_object.h"

s32 CAI__SetMode(s32, s32, s32);

extern f32 gFrameIncrement;

void EnemyMode_StartDelay_Update(s32 arg0, CAI *arg1) {
    f32 temp_f0;

    temp_f0 = arg1->m_ConditionTimer - gFrameIncrement;
    arg1->m_ConditionTimer = temp_f0;
    if (temp_f0 <= 0.0f) {
        CAI__SetMode(arg0, (s32) arg1, 2);
    }
}
