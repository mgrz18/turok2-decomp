#include "common.h"
#include "m2c_macros.h"
#include "globals/game.h"


s32 TeamMenu_Poll_2(void) {
    return gEngineApp_m_Settings.match.over;
}
