/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8043D4E0_us_rev1.c, func_8043D4E0_us_rev1.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "globals/game.h"

extern s32 gEngineApp_m_Settings_unlockedCheats;
extern s32 D_800B57E4;
extern s32 D_800B5800;
s32 CheatMenu_InfiniteLivesText(struct Func8043D458Arg *arg0) {
    if (gEngineApp_m_Settings_unlockedCheats & 2) {
        arg0->flags |= 0x01000000;
    } else {
        arg0->flags &= 0xFEFFFFFF;
    }
    if (gEngineApp_m_Settings.cheatFlags & 2) {
        arg0->text = &D_800B57E4;
    } else {
        arg0->text = &D_800B5800;
    }
    return 0;
}
