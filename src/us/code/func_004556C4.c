/* The same code as OptionsMenu_ToggleWeaponSelect (the other module). */
#include "common.h"
#include "structs/player.h"
#include "structs/menu.h"

extern CPlayerOptions gEngineApp_m_Settings_defaultSlot;
s32 OptionsMenu_ToggleWeaponSelect_2(void *unused, Menu *menu) {
    CPlayerOptions *settings = &gEngineApp_m_Settings_defaultSlot;
    if (menu->player != 0) {
        settings = menu->player->m_pOptions;
    }
    if (settings->m_WeaponSelect == 1) {
        settings->m_WeaponSelect = 0;
    } else {
        settings->m_WeaponSelect = 1;
    }
    return 0;
}
