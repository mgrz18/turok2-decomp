/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/rodata.func_8043DD2C_us_rev1.800E2330.c, func_8043D6B8_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "globals/game.h"

extern char D_800B596C[];
extern char D_800B5988[];
s32 CheatMenu_BigHandsFeetModeText(struct StateRecAt14 *item) {
    if (gEngineApp_m_Settings.unlockedCheats & 0x100) {
        item->unk8 |= 0x1000000;
    } else {
        item->unk8 &= ~0x1000000;
    }
    if (gEngineApp_m_Settings.cheatFlags & 0x100) {
        item->unk14 = D_800B596C;
    } else {
        item->unk14 = D_800B5988;
    }
    return 0;
}
