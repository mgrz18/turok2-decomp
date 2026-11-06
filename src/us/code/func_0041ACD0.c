/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8043C688_de_11.c, func_8043CD64_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "globals/menu.h"

s32 strlen(u8 *);
s32 EnterCheatMenu_MatchesCode(u8 *arg0) {
    u8 *input = arg0;
    s32 index = 0;
    if (strlen(input) != strlen(gCheatEntryText)) {
        return 0;
    }
    for (; index < strlen(input); index++) {
        if ((input[index] ^ index) != gCheatEntryText[index]) {
            return 0;
        }
    }
    return 1;
}
