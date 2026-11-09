#include "common.h"
#include "structs/player.h"
#include "structs/menu.h"
#include "globals/game.h"
#include "protos/menu.h"


s32 OptionsMenu_StepStickVert(s32 arg0, Menu *arg1) {
    CPlayerOptions *temp_v0_2;
    CPlayerOptions *var_s0;
    CPlayer *temp_v0;

    temp_v0 = arg1->player;
    var_s0 = &gEngineApp_m_Settings.defaultSlot;
    if (temp_v0 != NULL) {
        temp_v0_2 = temp_v0->m_pOptions;
        if (temp_v0_2 != NULL) {
            var_s0 = temp_v0_2;
        }
    }
    var_s0->m_SettingY = Menu_StepValue((s32) arg1, (s32) var_s0->m_SettingY, 8, 8, 0xF8, 0);
    return 0;
}
