/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8043D080_de.c, func_8043D080_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "globals/game.h"

s32 CheatsMenu_SetToggle(struct Func8043D458Arg *arg0, s32 arg1, s32 *arg2, s32 *arg3) {
    if (gEngineApp_m_Settings.unlockedCheats & arg1) arg0->flags |= 0x01000000;
    else arg0->flags &= 0xFEFFFFFF;
    if (gEngineApp_m_Settings.cheatFlags & arg1) arg0->text = arg2;
    else arg0->text = arg3;
    return 0;
}
