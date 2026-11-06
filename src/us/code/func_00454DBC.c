/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/ITEMS/80444030_6.CPP, func_80444314_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "globals/game.h"
struct MenuItemRecAt14;
struct MenuItemRecAt14 {
    char pad0[8];
    u32 flags;
    char padC[8];
    void *table;
};
struct MenuItemRecAt14;
typedef struct MenuItemRecAt14 MenuItemRecAt14;
extern u8 D_800B3E88[];
extern u8 D_800B3EA0[];
extern u8 D_800B3E70[];
extern u32 gEngineApp_m_Settings_difficulty[];
s32 OptionsMenu_DifficultyText_2(MenuItemRecAt14 *item) {
    if (gEngineApp_m_Settings.mode == 8) {
        item->flags |= 0x01000000;
    } else {
        item->flags &= ~0x01000000;
    }
    switch (gEngineApp_m_Settings_difficulty[0]) {
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
