#include "common.h"
#include "m2c_macros.h"

extern s32 gEngineApp_m_Settings; /* local type: GameSettings.cheatFlags; the ROM xors it as a scalar (as the struct, GCC bases the load/store on a register) */

s32 CheatMenu_ToggleAllWeapons_2(void) {
    gEngineApp_m_Settings ^= 4;
    return 0;
}
