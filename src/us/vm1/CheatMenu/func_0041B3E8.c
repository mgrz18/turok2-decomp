#include "common.h"
#include "m2c_macros.h"
#include "globals/game.h"

s32 CheatMenu_ToggleAllSpecialObjects(void) {
    gEngineApp_m_Settings_cheatFlags ^= 0x10;
    return 0;
}
