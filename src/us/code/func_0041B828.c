#include "common.h"
#include "m2c_macros.h"

extern s32 gEngineApp_m_Settings; /* local type: GameSettings.cheatFlags; the ROM xors it as a scalar (as the struct, GCC bases the load/store on a register) */

s32 CheatMenu_ToggleZachAttack(void) {
    gEngineApp_m_Settings ^= 0x1000;
    return 0;
}
