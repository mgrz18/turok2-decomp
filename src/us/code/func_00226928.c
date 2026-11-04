/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0028DC18.c, func_0028DC18.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/world.h"

s32 *func_002017D4(s32 *tbl, s32 idx);
void func_00201818(s32 *tbl, s32 idx);
s32 func_00226928(CWorld *lv, s32 sel, s32 c, s32 b, s32 bit) {
    s32 *t;
    s32 *base;
    u8 *bits;
    s32 m;
    if (sel != 0) {
        base = lv->m_pBitSections;
    } else {
        base = lv->m_pBitSections2;
    }
    t = func_002017D4(func_002017D4(func_002017D4(base, 0), b), c);
    func_002017D4(t, 0);
    func_00201818(t, 1);
    bits = (u8 *)func_002017D4(t, 1);
    m = 1 << (bit & 7);
    return (bits[bit / 8] & m) != 0;
}
