#include "common.h"
#include "structs/game_object.h"

s32 func_00243414(s32, s32, s32);

extern f32 D_800B6D28;

void func_0025F978(s32 arg0, CAI *arg1) {
    f32 temp_f0;

    if (arg1->m_ConditionTimer > 240.0f) {
        arg1->m_ConditionTimer = 240.0f;
    }
    temp_f0 = arg1->m_ConditionTimer - D_800B6D28;
    arg1->m_ConditionTimer = temp_f0;
    if (temp_f0 <= 0.0f) {
        func_00243414(arg0, (s32) arg1, 0x1E);
    }
}
