/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/ITEMS/80444030_6.CPP, func_804443C0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/player.h"
#include "structs/menu.h"

extern CPlayerOptions D_8011AB18;
extern s32 func_0041648C(void *, s32, s32, s32, s32, s32);
s32 func_0041C158(void *first, Menu *menu) {
    CPlayerOptions *settings = &D_8011AB18;
    if (menu->player != 0 && menu->player->m_pOptions != 0) {
        settings = menu->player->m_pOptions;
    }
    settings->m_bAutoAim = func_0041648C(menu, settings->m_bAutoAim, 1, 0, 1, 1);
    return 0;
}
