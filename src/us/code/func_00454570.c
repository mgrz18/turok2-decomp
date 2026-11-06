#include "common.h"
#include "m2c_macros.h"
#include "globals/game.h"

s32 CheatMenu_ToggleBigHandsFeetMode_2(void) {
    gEngineApp_m_Settings_cheatFlags ^= 0x100;
    return 0;
}
