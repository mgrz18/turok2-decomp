/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0028B964.c, func_0028B964.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/world.h"

typedef struct { u8 pad0[0x16E]; u16 unk16E; } Src;
s32 *func_002017D4(s32 *tbl, s32 idx);
void func_00201818(s32 *tbl, s32 idx);
s32 func_002248D4(CWorld *lv, Src *s) {
    s32 bit = s->unk16E;
    s32 idx = lv->m_nLevel;
    s32 *t = func_002017D4(func_002017D4(func_002017D4(lv->m_pBitSections, 0), idx), 1);
    u8 *bits;
    s32 m;
    func_002017D4(t, 0);
    func_00201818(t, 1);
    bits = (u8 *)func_002017D4(t, 1);
    m = 1 << (bit & 7);
    return (bits[bit / 8] & m) != 0;
}
