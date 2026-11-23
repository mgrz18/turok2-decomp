/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/ITEMS/80444030_6.CPP, func_80444424_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/player.h"
#include "structs/menu.h"
#include "globals/game.h"

extern char sOptionsMenuText_LookSpringOff[];
extern char sOptionsMenuText_LookSpringOn[];
s32 OptionsMenu_LookSpringText_2(MenuItem *item, Menu *menu) {
    CPlayerOptions *settings = &gEngineApp_m_Settings.defaultSlot;
    if (menu->player != 0 && menu->player->m_pOptions != 0) {
        settings = menu->player->m_pOptions;
    }
    switch (settings->m_bLookSpring) {
    case 0:
        item->text = sOptionsMenuText_LookSpringOff;
        break;
    case 1:
        item->text = sOptionsMenuText_LookSpringOn;
        break;
    }
    return 0;
}
