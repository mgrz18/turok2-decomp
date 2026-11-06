#include "common.h"
#include "structs/menu.h"
#include "structs/player.h"
#include "globals/game.h"


s32 *OptionsMenu_GetPlayerOptions(Menu *arg0) {
    s32 *temp_v1_2;
    s32 *var_v0;
    CPlayer *temp_v1;

    temp_v1 = arg0->player;
    var_v0 = &gEngineApp_m_Settings.defaultSlot;
    if (temp_v1 != NULL) {
        temp_v1_2 = (s32 *) temp_v1->m_pOptions;
        if (temp_v1_2 != NULL) {
            var_v0 = temp_v1_2;
        }
    }
    return var_v0;
}
