#include "common.h"
#include "m2c_macros.h"
#include "globals/game.h"

s32 CheatMenu_ToggleBlackout(void) {
    gEngineApp_m_Settings_cheatFlags ^= 0x4000;
    return 0;
}
