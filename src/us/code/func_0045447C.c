/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_0043D788.c, func_0043D788.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
#include "common.h"
#include "structs/menu.h"
#include "globals/game.h"

extern char sCheatMenuText_TinyModeOn[];
extern char sCheatMenuText_TinyModeOff[];
s32 CheatMenu_TinyModeText_2(MenuItem *a) {
    if (gEngineApp_m_Settings.unlockedCheats & 0x40) a->flags |= 0x1000000; else a->flags &= ~0x1000000;
    if (gEngineApp_m_Settings.cheatFlags & 0x40) a->text = sCheatMenuText_TinyModeOn; else a->text = sCheatMenuText_TinyModeOff;
    return 0;
}
