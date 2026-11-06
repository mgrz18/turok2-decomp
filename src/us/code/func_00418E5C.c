#include "common.h"
#include "structs/game.h"
#include "globals/game.h"

/* The multiplayer settings, gEngineApp_m_Settings (GameSettings in PlayerSetupMenu_HandicapLabel_2; 0x184 bytes, see Demo_End). */

s32 Menu_StepValue(s32, s32, s32, s32, s32, s32);

s32 BloodLustMenu_StepCrosshairs(s32 arg0, s32 arg1) {
    GameSettings *settings = &gEngineApp_m_Settings;

    settings->dmCrosshairs = Menu_StepValue(arg1, settings->dmCrosshairs, 1, 0, 1, 1);
    return 0;
}
