#include "common.h"
#include "m2c_macros.h"

extern s32 gEngineApp_m_Settings_match_over;

s32 PlayerSetupMenu_Poll_2(void) {
    return gEngineApp_m_Settings_match_over;
}
