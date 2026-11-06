/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU_2/80408AC4_42.CPP, func_8043D080_de.
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
extern s32 gEngineApp_m_Settings_unlockedCheats;
s32 CheatsMenu_SetToggle_2(struct Func8043D458Arg *arg0, s32 arg1, s32 *arg2, s32 *arg3) {
    if (gEngineApp_m_Settings_unlockedCheats & arg1) arg0->flags |= 0x01000000;
    else arg0->flags &= 0xFEFFFFFF;
    if (gEngineApp_m_Settings.cheatFlags & arg1) arg0->text = arg2;
    else arg0->text = arg3;
    return 0;
}
