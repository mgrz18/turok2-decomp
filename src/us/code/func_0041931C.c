#include "common.h"
#include "structs/game.h"

s32 Menu_StepValue(s32, s32, s32, s32, s32, s32);

s32 TeamBloodMenu_StepCrosshairs(s32 arg0, s32 arg1) {
    GameSettings *var_s0;

    var_s0 = &gEngineApp_m_Settings;
    var_s0 = &gEngineApp_m_Settings;
    gEngineApp_m_Settings.teamCrosshairs = Menu_StepValue(arg1, (s32) gEngineApp_m_Settings.teamCrosshairs, 1, 0, 1, 1);
    return 0;
}
