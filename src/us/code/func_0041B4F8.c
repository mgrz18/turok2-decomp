#include "common.h"
#include "m2c_macros.h"
#include "globals/game.h"

s32 CheatMenu_ToggleTinyMode(void) {
    gEngineApp_m_Settings_cheatFlags ^= 0x40;
    return 0;
}
