#include "common.h"
#include "structs/game.h"
#include "globals/game.h"
#include "protos/menu.h"


s32 FragTagMenu_StepCrosshairs(s32 arg0, s32 arg1) {
    GameSettings *var_s0;

    var_s0 = &gEngineApp_m_Settings;
    var_s0 = &gEngineApp_m_Settings;
    gEngineApp_m_Settings.tagCrosshairs = Menu_StepValue(arg1, (s32) gEngineApp_m_Settings.tagCrosshairs, 1, 0, 1, 1);
    return 0;
}
