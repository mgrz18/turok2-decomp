#include "common.h"
#include "structs/menu.h"
#include "structs/player.h"
#include "protos/menu.h"


s32 PlayerSetupMenu_StepControlType(s32 arg0, Menu *arg1) {
    CPlayerOptions *temp_s0;

    temp_s0 = arg1->player->m_pOptions;
    temp_s0->m_ControlType = Menu_StepValue(arg1, (s32) temp_s0->m_ControlType, 1, 0, 1, 1);
    return 0;
}
