/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_0043D810.c, func_0043D810.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
#include "common.h"
#include "structs/menu.h"

extern s32 gEngineApp_m_Settings_unlockedCheats;
extern s32 gEngineApp_m_Settings;
extern char D_800B5934[];
extern char D_800B5950[];
s32 CheatMenu_StickModeText_2(MenuItem *a) {
    if (gEngineApp_m_Settings_unlockedCheats & 0x80) a->flags |= 0x1000000; else a->flags &= ~0x1000000;
    if (gEngineApp_m_Settings & 0x80) a->text = D_800B5934; else a->text = D_800B5950;
    return 0;
}
