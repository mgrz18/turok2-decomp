#include "common.h"
#include "m2c_macros.h"
#include "globals/game.h"

s32 CheatMenu_ToggleJuansCheat(void) {
    gEngineApp_m_Settings_cheatFlags ^= 0x800;
    return 0;
}
