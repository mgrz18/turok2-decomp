/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/ITEMS/80444030.CPP, func_80444C34_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/player.h"
#include "structs/menu.h"
extern CPlayerOptions D_8011AB18;
s32 func_0041C764(void *unused, Menu *menu) {
    CPlayerOptions *settings = &D_8011AB18;
    if (menu->player != 0) {
        settings = menu->player->m_pOptions;
    }
    if (settings->m_ControlStyle == 1) {
        settings->m_ControlStyle = 0;
    } else {
        settings->m_ControlStyle = 1;
    }
    return 0;
}
