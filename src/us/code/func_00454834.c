/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_0043DB40.c, func_0043DB40.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
#include "common.h"
#include "structs/menu.h"
#include "globals/game.h"

extern s32 gEngineApp_m_Settings_unlockedCheats;
extern char D_800B5A84[];
extern char D_800B5AA0[];
s32 CheatMenu_AllMapText_2(MenuItem *a) {
    if (gEngineApp_m_Settings_unlockedCheats & 0x2000) a->flags |= 0x1000000; else a->flags &= ~0x1000000;
    if (gEngineApp_m_Settings.cheatFlags & 0x2000) a->text = D_800B5A84; else a->text = D_800B5AA0;
    return 0;
}
