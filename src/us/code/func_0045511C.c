/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/ITEMS/80444030_6.CPP, func_80444424_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/player.h"
#include "structs/menu.h"
#include "globals/game.h"

extern char sOptionsMenuText_AutoAimOff[];
extern char sOptionsMenuText_AutoAimOn[];
s32 OptionsMenu_AutoAimText_2(MenuItem *item, Menu *menu) {
    CPlayerOptions *settings = &gEngineApp_m_Settings.defaultSlot;
    if (menu->player != 0 && menu->player->m_pOptions != 0) {
        settings = menu->player->m_pOptions;
    }
    switch (settings->m_bAutoAim) {
    case 0:
        item->text = sOptionsMenuText_AutoAimOff;
        break;
    case 1:
        item->text = sOptionsMenuText_AutoAimOn;
        break;
    }
    return 0;
}
