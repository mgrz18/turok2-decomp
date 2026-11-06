#include "common.h"
#include "structs/menu.h"
#include "structs/player.h"
#include "globals/game.h"


/* The player's options, or the defaults at gEngineApp_m_Settings_defaultSlot. */
CPlayerOptions *OptionsMenu_GetPlayerOptions_2(Menu *arg0) {
    CPlayerOptions *temp_v1_2;
    CPlayerOptions *var_v0;
    CPlayer *temp_v1;

    temp_v1 = arg0->player;
    var_v0 = &gEngineApp_m_Settings.defaultSlot;
    if (temp_v1 != NULL) {
        temp_v1_2 = temp_v1->m_pOptions;
        if (temp_v1_2 != NULL) {
            var_v0 = temp_v1_2;
        }
    }
    return var_v0;
}
