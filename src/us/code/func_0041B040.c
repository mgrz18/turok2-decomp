#include "common.h"
#include "m2c_macros.h"

extern s32 gEngineApp_m_Settings[];

s32 CheatMenu_ToggleFlag(s32 arg0, s32 arg1) {
    gEngineApp_m_Settings[0] ^= arg1;
    return 0;
}
