/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/ITEMS/80444030_6.CPP, func_804443C0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/player.h"
#include "structs/menu.h"
#include "globals/game.h"

extern s32 Menu_StepValue(void *, s32, s32, s32, s32, s32);
s32 OptionsMenu_StepAutoAim(void *first, Menu *menu) {
    CPlayerOptions *settings = &gEngineApp_m_Settings.defaultSlot;
    if (menu->player != 0 && menu->player->m_pOptions != 0) {
        settings = menu->player->m_pOptions;
    }
    settings->m_bAutoAim = Menu_StepValue(menu, settings->m_bAutoAim, 1, 0, 1, 1);
    return 0;
}
