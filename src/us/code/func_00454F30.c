#include "common.h"
#include "structs/menu.h"
#include "structs/player.h"
#include "globals/game.h"

s32 Menu_StepValue_2(s32, s32, s32, s32, s32, s32);

s32 OptionsMenu_StepControlSide_2(s32 arg0, Menu *arg1) {
    struct CPlayerOptions *temp_v0_2;
    struct CPlayerOptions *var_s0;
    CPlayer *temp_v0;

    temp_v0 = arg1->player;
    var_s0 = &gEngineApp_m_Settings.defaultSlot;
    if (temp_v0 != NULL) {
        temp_v0_2 = temp_v0->m_pOptions;
        if (temp_v0_2 != NULL) {
            var_s0 = temp_v0_2;
        }
    }
    var_s0->m_ControlSide = Menu_StepValue_2((s32) arg1, (s32) var_s0->m_ControlSide, 1, 0, 1, 1);
    return 0;
}
