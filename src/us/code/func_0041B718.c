#include "common.h"
#include "m2c_macros.h"
#include "globals/game.h"

s32 CheatMenu_ToggleGouraudMode(void) {
    gEngineApp_m_Settings_cheatFlags ^= 0x400;
    return 0;
}
