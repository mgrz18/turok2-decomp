#include "common.h"
#include "structs/game.h"

/* The multiplayer settings, gEngineApp_m_Settings (GameSettings in PlayerSetupMenu_HandicapLabel_2; 0x184 bytes, see Demo_End). */

s32 Menu_StepValue_2(s32, s32, s32, s32, s32, s32);

s32 func_00451E54(s32 arg0, s32 arg1) {
    GameSettings *settings = &gEngineApp_m_Settings;

    settings->teamTimeLimit = Menu_StepValue_2(arg1, settings->teamTimeLimit, 5, 0, 0x1E, 1);
    return 0;
}
