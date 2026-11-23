/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8043D4E0_us_rev1.c, func_8043D4E0_us_rev1.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "globals/menu_text.h"
#include "globals/game.h"

#define T2_TEXT(s) ((void *) (s))

#ifdef TARGET_PORT
#else
#endif
#ifdef TARGET_PORT
#else
#endif
s32 CheatMenu_InfiniteLivesText(struct Func8043D458Arg *arg0) {
    if (gEngineApp_m_Settings.unlockedCheats & 2) {
        arg0->flags |= 0x01000000;
    } else {
        arg0->flags &= 0xFEFFFFFF;
    }
    if (gEngineApp_m_Settings.cheatFlags & 2) {
        arg0->text = T2_TEXT(sCheatMenuText_InfiniteLivesOn);
    } else {
        arg0->text = T2_TEXT(sCheatMenuText_InfiniteLivesOff);
    }
    return 0;
}
