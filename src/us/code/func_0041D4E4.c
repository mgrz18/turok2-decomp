/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80445964_de_8.c, func_80445B88_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern s32 func_0041D4E4(u8 *a, u8 *b, s32 count);
s32 func_0041D4E4(u8 *a, u8 *b, s32 count) {
    u8 c;
    u8 d;
    do {
        c = *a++;
        d = *b++;
        count--;
    } while (c == d && c != 0 && count == 0);
    return c == d;
}
