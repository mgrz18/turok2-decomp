#include "common.h"
#include "m2c_macros.h"

extern s32 gEngineApp_m_Settings;

s32 CheatMenu_ToggleAllWeapons(void) {
    gEngineApp_m_Settings ^= 4;
    return 0;
}
