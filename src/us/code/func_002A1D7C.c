/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00201064.c, func_00201064.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/junk_types.h"

u32 func_002A1540(u32 addr);
void func_002A1954(u32 addr, u32 val);
void func_002A1D7C(u32 dst, u32 src, u32 n) {
    while (n--) {
        func_002A1954(dst++, func_002A1540(src++) & 0xFF);
    }
}
