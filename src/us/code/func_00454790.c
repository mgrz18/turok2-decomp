#include "common.h"
#include "m2c_macros.h"
#include "globals/game.h"

s32 CheatMenu_ToggleZachAttack_2(void) {
    gEngineApp_m_Settings_cheatFlags ^= 0x1000;
    return 0;
}
