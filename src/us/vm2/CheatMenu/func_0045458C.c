/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_0043D898.c, func_0043D898.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
#include "common.h"
#include "globals/menu_text.h"
#include "structs/menu.h"
#include "globals/game.h"

s32 CheatMenu_BigHandsFeetModeText_2(MenuItem *a) {
    if (gEngineApp_m_Settings.unlockedCheats & 0x100) a->flags |= 0x1000000; else a->flags &= ~0x1000000;
    if (gEngineApp_m_Settings.cheatFlags & 0x100) a->text = sCheatMenuText_BigHandsFeetModeOn; else a->text = sCheatMenuText_BigHandsFeetModeOff;
    return 0;
}
