/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/LEVEL/8043D4E0.CPP, func_8043D4E0_us_rev1.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "globals/menu_text.h"
#include "globals/game.h"
#include "rw/abu_types.h"
s32 CheatMenu_InfiniteLivesText_2(struct Func8043D458Arg *arg0) {
    if (gEngineApp_m_Settings.unlockedCheats & 2) {
        arg0->flags |= 0x01000000;
    } else {
        arg0->flags &= 0xFEFFFFFF;
    }
    if (gEngineApp_m_Settings.cheatFlags & 2) {
        arg0->text = &sCheatMenuText_InfiniteLivesOn;
    } else {
        arg0->text = &sCheatMenuText_InfiniteLivesOff;
    }
    return 0;
}
