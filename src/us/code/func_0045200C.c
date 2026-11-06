#include "common.h"
#include "structs/game.h"

/* The multiplayer settings, gEngineApp_m_Settings (GameSettings in PlayerSetupMenu_HandicapLabel_2; 0x184 bytes, see Demo_End). */

s32 Menu_StepValue_2(s32, s32, s32, s32, s32, s32);

s32 func_0045200C(s32 arg0, s32 arg1) {
    GameSettings *settings = &gEngineApp_m_Settings;

    settings->teamArena = Menu_StepValue_2(arg1, settings->teamArena, 1, 0, 0xD, 1);
    return 0;
}
