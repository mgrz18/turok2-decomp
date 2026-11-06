#include "common.h"
#include "m2c_macros.h"
#include "globals/game.h"

s32 CheatMenu_ToggleInfiniteLives(void) {
    gEngineApp_m_Settings_cheatFlags ^= 2;
    return 0;
}
