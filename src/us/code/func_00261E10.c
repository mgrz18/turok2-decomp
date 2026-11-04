#include "common.h"
#include "structs/game_object.h"

extern f32 D_800A7B3C;
extern f32 func_002119FC(f32, f32);

void func_00261E10(s32 arg0, CAI *arg1) {
    arg1->m_ConditionTimer = func_002119FC(0.0f, D_800A7B3C);
}
