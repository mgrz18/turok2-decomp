/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_0043DBC8.c, func_0043DBC8.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
#include "common.h"
#include "structs/menu.h"

extern s32 gEngineApp_m_Settings_unlockedCheats;
extern s32 gEngineApp_m_Settings;
extern char D_800B5ABC[];
extern char D_800B5AD8[];
s32 CheatMenu_BlackoutText_2(MenuItem *a) {
    if (gEngineApp_m_Settings_unlockedCheats & 0x4000) a->flags |= 0x1000000; else a->flags &= ~0x1000000;
    if (gEngineApp_m_Settings & 0x4000) a->text = D_800B5ABC; else a->text = D_800B5AD8;
    return 0;
}
