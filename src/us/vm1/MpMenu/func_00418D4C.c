#include "common.h"
#include "structs/game.h"
#include "globals/game.h"
#include "protos/menu.h"


s32 BloodLustMenu_StepTextureSet(sptr arg0, sptr arg1) {
    GameSettings *var_s0;

    var_s0 = &gEngineApp_m_Settings;
    var_s0 = &gEngineApp_m_Settings;
    gEngineApp_m_Settings.dmTextureSet = Menu_StepValue(arg1, (s32) gEngineApp_m_Settings.dmTextureSet, 1, 0, 8, 1);
    return 0;
}
