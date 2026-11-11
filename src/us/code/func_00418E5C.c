#include "common.h"
#include "structs/game.h"
#include "globals/game.h"
#include "protos/menu.h"

/* The multiplayer settings, gEngineApp_m_Settings (GameSettings in PlayerSetupMenu_HandicapLabel_2; 0x184 bytes, see Demo_End). */


s32 BloodLustMenu_StepCrosshairs(sptr arg0, sptr arg1) {
    GameSettings *settings = &gEngineApp_m_Settings;

    settings->dmCrosshairs = Menu_StepValue(arg1, settings->dmCrosshairs, 1, 0, 1, 1);
    return 0;
}
