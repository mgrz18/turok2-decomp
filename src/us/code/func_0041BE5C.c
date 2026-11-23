/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80444CF8_de_3.c, func_80444314_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "globals/game.h"

extern u8 sOptionsMenuText_DifficultyNormal[];
extern u8 sOptionsMenuText_DifficultyHard[];
extern u8 sOptionsMenuText_DifficultyEasy[];
s32 OptionsMenu_DifficultyText(MenuItemRecAt14 *item) {
    extern u32 gEngineApp_m_Settings_difficulty; /* local type: the field's real type (or the member path) changes the ROM's code here */
    if (gEngineApp_m_Settings.mode == 8) {
        item->flags |= 0x01000000;
    } else {
        item->flags &= ~0x01000000;
    }
    switch (gEngineApp_m_Settings_difficulty) {
    case 8:
        item->table = sOptionsMenuText_DifficultyEasy;
        break;
    case 0x10:
        item->table = sOptionsMenuText_DifficultyNormal;
        break;
    case 0x20:
        item->table = sOptionsMenuText_DifficultyHard;
        break;
    }
    return 0;
}
