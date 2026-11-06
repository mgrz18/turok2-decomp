/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_0043DAB8.c, func_0043DAB8.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
#include "common.h"
#include "structs/menu.h"
#include "globals/game.h"

extern s32 gEngineApp_m_Settings_unlockedCheats;
extern char D_800B5A4C[];
extern char D_800B5A68[];
s32 CheatMenu_ZachAttackText_2(MenuItem *a) {
    if (gEngineApp_m_Settings_unlockedCheats & 0x1000) a->flags |= 0x1000000; else a->flags &= ~0x1000000;
    if (gEngineApp_m_Settings.cheatFlags & 0x1000) a->text = D_800B5A4C; else a->text = D_800B5A68;
    return 0;
}
