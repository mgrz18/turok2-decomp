#include "common.h"
#include "structs/game.h"
#include "globals/game.h"

s32 Menu_StepValue(s32, s32, s32, s32, s32, s32);

s32 OptionsMenu_StepOpacity(s32 arg0, s32 arg1) {
    u16 temp_v0;
    u8 var_v1;

    temp_v0 = Menu_StepValue(arg1, gEngineApp_m_Settings.hudAlphaSetting.value, 0x10, 0, 0x100, 0);
    gEngineApp_m_Settings.hudAlphaSetting.value = temp_v0;
    var_v1 = 0xFF;
    if ((u32) (temp_v0 & 0xFFFF) < 0xFFU) {
        var_v1 = gEngineApp_m_Settings.hudAlphaSetting.bytes[1];
    }
    gEngineApp_m_Settings.hudAlpha = var_v1;
    return 0;
}
