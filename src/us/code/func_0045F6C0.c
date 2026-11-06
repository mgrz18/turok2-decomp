/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80403E90_de_6.c, func_80405160_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "structs/pak.h"
#include "globals/pak.h"

int Pak_GetFreePages_2(int slot, int *score) {
    if (gPakState[slot] != 3) {
        return -2;
    }
    if (gPakError[slot] == 0) {
        *score = gPakDirectories[slot].freeBytes / 256;
    } else {
        *score = 0;
    }
    return gPakError[slot];
}
