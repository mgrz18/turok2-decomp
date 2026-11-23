/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_0043D568.c, func_0043D568.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
#include "common.h"
#include "structs/menu.h"
#include "globals/game.h"

extern char sCheatMenuText_AllWeaponsOn[];
extern char sCheatMenuText_AllWeaponsOff[];
s32 CheatMenu_AllWeaponsText_2(MenuItem *a) {
    if (gEngineApp_m_Settings.unlockedCheats & 4) a->flags |= 0x1000000; else a->flags &= ~0x1000000;
    if (gEngineApp_m_Settings.cheatFlags & 4) a->text = sCheatMenuText_AllWeaponsOn; else a->text = sCheatMenuText_AllWeaponsOff;
    return 0;
}
