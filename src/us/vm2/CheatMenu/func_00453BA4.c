/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8043C688_de_11.c, func_8043CCD0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "structs/menu.h"
#include "globals/menu.h"

s32 EnterCheatMenu_Backspace_2(void) {
    s32 index;
    s32 next;
    sEnterCheatResult = -1;
    gCheatEntry.timer = 0;
    if (gCheatEntry.length == 0) {
        return 0;
    }
    if (gCheatEntry.cursor == 0) {
        return 0;
    }
    index = gCheatEntry.cursor - 1;
    if (index < 0x17) {
        do {
            next = index + 1;
            gCheatEntry.text[index] = gCheatEntry.text[next];
            index = next;
        } while (index < 0x17);
    }
    gCheatEntry.cursor--;
    gCheatEntry.length--;
    return 0;
}
