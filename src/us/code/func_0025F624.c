#include "common.h"
#include "structs/game_object.h"
#include "rw/abu_protos.h"

f32 func_002119FC(f32 arg0, f32 arg1);

extern f32 D_800A790C;
extern f32 D_800A7910;

void func_0025F624(s32 arg0, CAI *arg1) {
    f32 temp_f20;
    f32 temp_f21;

    temp_f20 = D_800A790C;
    temp_f21 = D_800A7910;
    arg1->m_Enemy.m_Timer = func_002119FC(temp_f20, temp_f21);
    arg1->m_Enemy.m_Timer2 = func_002119FC(temp_f20, temp_f21);
}
