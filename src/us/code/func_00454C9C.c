#include "common.h"
#include "structs/menu.h"
#include "structs/player.h"

extern CPlayerOptions D_8011AB18;

/* The player's options, or the defaults at D_8011AB18. */
CPlayerOptions *OptionsMenu_GetPlayerOptions_2(Menu *arg0) {
    CPlayerOptions *temp_v1_2;
    CPlayerOptions *var_v0;
    CPlayer *temp_v1;

    temp_v1 = arg0->player;
    var_v0 = &D_8011AB18;
    if (temp_v1 != NULL) {
        temp_v1_2 = temp_v1->m_pOptions;
        if (temp_v1_2 != NULL) {
            var_v0 = temp_v1_2;
        }
    }
    return var_v0;
}
