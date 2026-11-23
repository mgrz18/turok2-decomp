/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_0043D5F0.c, func_0043D5F0.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
#include "common.h"
#include "structs/menu.h"
#include "globals/game.h"

extern char sCheatMenuText_UnlimitedAmmoOn[];
extern char sCheatMenuText_UnlimitedAmmoOff[];
s32 CheatMenu_UnlimitedAmmoText_2(MenuItem *a) {
    if (gEngineApp_m_Settings.unlockedCheats & 0x8) a->flags |= 0x1000000; else a->flags &= ~0x1000000;
    if (gEngineApp_m_Settings.cheatFlags & 0x8) a->text = sCheatMenuText_UnlimitedAmmoOn; else a->text = sCheatMenuText_UnlimitedAmmoOff;
    return 0;
}
