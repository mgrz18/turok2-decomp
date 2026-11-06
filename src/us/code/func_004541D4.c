/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/LEVEL/8043D4E0.CPP, func_8043D4E0_us_rev1.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "globals/game.h"
struct Func8043D458Arg;
struct Func8043D458Arg {
    s8 pad0[8];
    s32 flags;
    s8 padC[8];
    s32 *text;
};
extern s32 D_800B57E4;
extern s32 D_800B5800;
s32 CheatMenu_InfiniteLivesText_2(struct Func8043D458Arg *arg0) {
    if (gEngineApp_m_Settings.unlockedCheats & 2) {
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
