#include "common.h"
#include "structs/game.h"

s32 Menu_StepValue_2(s32, s32, s32, s32, s32, s32);

s32 func_00451F30(s32 arg0, s32 arg1) {
    GameSettings *var_s0;

    var_s0 = &gEngineApp_m_Settings;
    var_s0 = &gEngineApp_m_Settings;
    gEngineApp_m_Settings.teamScoreLimit = Menu_StepValue_2(arg1, (s32) gEngineApp_m_Settings.teamScoreLimit, 1, 0, 0x63, 1);
    return 0;
}
