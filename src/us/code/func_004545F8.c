#include "common.h"
#include "m2c_macros.h"
#include "globals/game.h"

s32 CheatMenu_TogglePenAndInkMode_2(void) {
    gEngineApp_m_Settings_cheatFlags ^= 0x200;
    return 0;
}
