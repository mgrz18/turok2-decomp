/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802AFE30_de_16.c, func_802AFEE0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern s32 func_00291034(void *, s16 *, s32);
extern s32 func_00291034(void *, s16 *, s32);
void func_00290E40(s32 arg0, s32 arg1, s8 arg2) {
    Buf16c sp10;
    sp10.count = 2;
    sp10.f8 = (s8)(arg1 | 0xB0);
    sp10.fA = arg2;
    sp10.zero = 0;
    sp10.f9 = 0x5B;
    func_00291034(arg0 + 0x48, &sp10, 0);
}
