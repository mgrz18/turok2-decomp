#include "common.h"
#include "m2c_macros.h"

extern s32 gEngineApp_m_Settings;

s32 CheatMenu_ToggleZachAttack(void) {
    gEngineApp_m_Settings ^= 0x1000;
    return 0;
}
