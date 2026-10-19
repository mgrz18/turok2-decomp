/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00260F18.c, func_00260F18.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/junk_types.h"

f32 func_00218398(B14FieldRd *f) {
    B14Tmp t1;
    B14Tmp t2;
    u32 addr;
    u32 *p;
    u32 sh;
    u32 v;
    u32 hi;
    u32 n;
    f32 res;
    t1.r = f->r;
    addr = f->addr;
    t2.r = t1.r;
    n = t2.r.n;
    p = (u32 *)((addr & 0xF0000000) | ((addr & 0x0FFFFFE0) >> 3));
    sh = addr & 0x1F;
    v = p[0];
    hi = p[1];
    if (sh != 0) {
        v >>= sh;
        hi <<= 32 - sh;
        v |= hi;
    }
    if (n < 32) {
        v &= (1 << n) - 1;
    }
    res = (f32)v / (f32)((1 << n) - 1) * t2.r.range + t2.r.min;
    f->addr += t1.r.n;
    return res;
}
