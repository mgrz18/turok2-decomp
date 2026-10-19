/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80200800_de_3.c, func_80200B14_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern s32 func_002A1384(s32 arg0);
extern void func_002A1808(s32 arg0, s32 arg1);
extern int func_002A1540(int arg0);
void func_002A1844(s32 source, s32 destination, u32 count) {
    while (count != 0 && (source & 3) != 0) {
        func_002A1808(destination++, (u8)func_002A1540(source++));
        count--;
    }
    while (count >= 4) {
        u32 word = func_002A1384(source);
        func_002A1808(destination++, word >> 24);
        func_002A1808(destination++, (word >> 16) & 0xFF);
        func_002A1808(destination++, (word >> 8) & 0xFF);
        func_002A1808(destination++, word & 0xFF);
        source += 4;
        count -= 4;
    }
    while (count != 0) {
        func_002A1808(destination++, (u8)func_002A1540(source++));
        count--;
    }
}
