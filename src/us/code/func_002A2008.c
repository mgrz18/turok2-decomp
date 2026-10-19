/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_002012D8.c, func_002012D8.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/junk_types.h"

u32 func_002A1384(u32 addr);
void func_002A1350(u32 addr, u32 val);
void func_002A13BC(s32 n);
void func_002A2008(void) {
    u32 src = func_002A1384(0xB1FFFFF4) & 0xB1FFFFFC;
    u32 len = func_002A1384(0xB1FFFFF8) & 0x1FFFFFC;
    u32 i;
    func_002A1350(0xB1FFFFFC, 0);
    while (func_002A1384(0xB0000010) == 0) {
        func_002A13BC(500);
    }
    for (i = 0; i < (len >> 2); i++) {
        func_002A1350((src & 0xB07FFFFF) + i * 4, func_002A1384(src + i * 4));
    }
    func_002A13BC(2000);
    func_002A1350(0xB1FFFFF4, 0);
}
