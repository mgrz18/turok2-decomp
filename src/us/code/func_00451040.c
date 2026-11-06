#include "common.h"
#include "structs/player.h"
#include "structs/menu.h"

s32 Menu_StepValue(s32, s32, s32, s32, s32, s32);

s32 PlayerSetupMenu_StepControlType_2(s32 arg0, Menu *arg1) {
    CPlayerOptions *temp_s0;

    temp_s0 = arg1->player->m_pOptions;
    temp_s0->m_ControlType = Menu_StepValue((s32) arg1, (s32) temp_s0->m_ControlType, 1, 0, 1, 1);
    return 0;
}
