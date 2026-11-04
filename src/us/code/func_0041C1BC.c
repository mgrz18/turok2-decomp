/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/ITEMS/80444030_6.CPP, func_80444424_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/player.h"
#include "structs/menu.h"
extern CPlayerOptions D_8011AB18;
extern char D_800B3F78[];
extern char D_800B3F60[];
s32 func_0041C1BC(MenuItem *item, Menu *menu) {
    CPlayerOptions *settings = &D_8011AB18;
    if (menu->player != 0 && menu->player->m_pOptions != 0) {
        settings = menu->player->m_pOptions;
    }
    switch (settings->m_bAutoAim) {
    case 0:
        item->text = D_800B3F78;
        break;
    case 1:
        item->text = D_800B3F60;
        break;
    }
    return 0;
}
