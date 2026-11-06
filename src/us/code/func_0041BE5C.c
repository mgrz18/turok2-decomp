/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80444CF8_de_3.c, func_80444314_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "globals/game.h"

extern u8 D_800B3E88[];
extern u8 D_800B3EA0[];
extern u8 D_800B3E70[];
s32 OptionsMenu_DifficultyText(MenuItemRecAt14 *item) {
    extern u32 gEngineApp_m_Settings_difficulty;
    if (gEngineApp_m_Settings.mode == 8) {
        item->flags |= 0x01000000;
    } else {
        item->flags &= ~0x01000000;
    }
    switch (gEngineApp_m_Settings_difficulty) {
    case 8:
        item->table = D_800B3E70;
        break;
    case 0x10:
        item->table = D_800B3E88;
        break;
    case 0x20:
        item->table = D_800B3EA0;
        break;
    }
    return 0;
}
