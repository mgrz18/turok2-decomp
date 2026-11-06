#include "common.h"
#include "m2c_macros.h"

extern s32 gEngineApp_m_Settings;

s32 CheatMenu_ToggleStickMode_2(void) {
    gEngineApp_m_Settings ^= 0x80;
    return 0;
}
