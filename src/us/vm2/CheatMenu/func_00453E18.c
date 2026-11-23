/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU_2/80408AC4_8.CPP, func_8043CF44_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "globals/menu.h"
#include "globals/cheat.h"
#include "structs/menu.h"

/* The cheat keyboard's labels (see vm1's EnterCheatMenu_ResultLabel). */
extern u8 D_800C1144[];

s32 EnterCheatMenu_ResultLabel_2(MenuItem *arg0) {
    if ((u32) sEnterCheatResult < 0xCU) {
        arg0->table = (s32 *) sCheatCodes_2[sEnterCheatResult].unk4;
    } else {
        arg0->table = (s32 *) D_800C1144;
    }
    return 0;
}
