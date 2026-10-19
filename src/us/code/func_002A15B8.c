/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80200800_de.c, func_80200880_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern s32 func_002A1384(s32 arg0);
extern int func_002A1540(int arg0);
int func_002A1540(int arg0);
void func_002A15B8(s32 source, u8 *destination, u32 count) {
    while (count != 0 && (source & 3) != 0) {
        *destination++ = func_002A1540(source++);
        count--;
    }
    while (count >= 4) {
        u32 word = func_002A1384(source);
        *destination++ = word >> 24;
        *destination++ = word >> 16;
        *destination++ = word >> 8;
        *destination++ = word;
        source += 4;
        count -= 4;
    }
    while (count != 0) {
        *destination++ = func_002A1540(source++);
        count--;
    }
}
