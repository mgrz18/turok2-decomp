#include "common.h"
#include "m2c_macros.h"

extern s32 gEngineApp_m_Settings;

s32 CheatMenu_ToggleInfiniteLives(void) {
    gEngineApp_m_Settings ^= 2;
    return 0;
}
