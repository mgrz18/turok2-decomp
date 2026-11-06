#include "common.h"
#include "m2c_macros.h"
#include "globals/game.h"

s32 CheatMenu_ToggleInvincibility(void) {
    gEngineApp_m_Settings_cheatFlags ^= 1;
    return 0;
}
