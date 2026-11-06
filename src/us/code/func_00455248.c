#include "common.h"
#include "m2c_macros.h"

s32 Menu_StepValue_2(s32, s32, s32, s32, s32, s32);

extern u8 gEngineApp_m_Settings_unk1B[];

s32 OptionsMenu_StepTwoPlayerLayout_2(s32 arg0, s32 arg1) {
    gEngineApp_m_Settings_unk1B[0] = Menu_StepValue_2(arg1, (s32) gEngineApp_m_Settings_unk1B[0], 1, 0, 1, 1);
    return 0;
}
