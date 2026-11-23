#include "common.h"
#include "m2c_macros.h"
#include "globals/game.h"

s32 CheatMenu_ToggleUnlimitedAmmo_2(void) {
    gEngineApp_m_Settings_cheatFlags ^= 8;
    return 0;
}
