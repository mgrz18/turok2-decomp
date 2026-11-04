#include "common.h"
#include "structs/menu.h"
#include "structs/player.h"

s32 func_0041648C(s32, s32, s32, s32, s32, s32);

s32 func_00418090(s32 arg0, Menu *arg1) {
    CPlayerOptions *temp_s0;

    temp_s0 = arg1->player->m_pOptions;
    temp_s0->m_ControlType = func_0041648C((s32) arg1, (s32) temp_s0->m_ControlType, 1, 0, 1, 1);
    return 0;
}
