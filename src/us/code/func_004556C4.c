/* The same code as func_0041C764 (the other module). */
#include "common.h"
#include "structs/player.h"
#include "structs/menu.h"

extern CPlayerOptions D_8011AB18;
s32 func_004556C4(void *unused, Menu *menu) {
    CPlayerOptions *settings = &D_8011AB18;
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
