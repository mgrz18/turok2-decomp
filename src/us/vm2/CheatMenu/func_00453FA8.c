#include "common.h"
#include "m2c_macros.h"
#include "globals/game.h"


s32 CheatMenu_ToggleFlag_2(s32 arg0, s32 arg1) {
    gEngineApp_m_Settings.cheatFlags ^= arg1;
    return 0;
}
