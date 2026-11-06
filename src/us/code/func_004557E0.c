#include "common.h"
#include "structs/game.h"
#include "globals/game.h"

/* The multiplayer settings, gEngineApp_m_Settings (GameSettings in PlayerSetupMenu_HandicapLabel_2; 0x184 bytes, see Demo_End). */

s32 Menu_StepValue_2(s32, s32, s32, s32, s32, s32);

s32 OptionsMenu_StepBrightness_2(s32 arg0, s32 arg1) {
    GameSettings *settings = &gEngineApp_m_Settings;

    settings->unkC = Menu_StepValue_2(arg1, settings->unkC, 0x10, 0, 0x80, 0);
    return 0;
}
