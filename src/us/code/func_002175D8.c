/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00260158.c, func_00260158.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/junk_types.h"

u32 func_00217B64(u32 addr, u32 n);
void func_00217214(B14Trk *t);
f32 func_002175D8(B14Trk *t, f32 x) {
    u32 frame;
    u32 fr;
    u32 start;
    s32 rec;
    f32 d, dt;
    u32 tbl[64];
    if (!(2147483648.0f <= x)) {
        fr = (s32)x;
    } else {
        fr = (s32)(x - 2147483648.0f) | 0x80000000;
    }
    frame = fr;
    start = t->start;
    rec = t->bits * 4 + 6;
    if (frame < start || frame >= start + t->span) {
        s32 idx;
        u32 s4, s0, addr, data;
        u32 *p;
        u32 *tb;
        idx = t->cur;
        s4 = start;
        addr = t->data;
        data = addr;
        addr += idx * rec;
        s0 = s4 + t->span;
        if (frame < start) {
            addr = data;
            idx = 0;
            s4 = 0;
            s0 = func_00217B64(addr, 6);
        }
        tb = tbl;
        p = (u32 *)(idx * 4 + (u32)tb);
        for (;;) {
            if (!(frame < s4 || frame >= s0)) {
                if (idx != t->cur) {
                    t->start = s4;
                    t->cur = idx;
                    func_00217214(t);
                }
                break;
            }
            addr += rec;
            p++;
            idx++;
            s4 = s0;
            *p = func_00217B64(addr, 6);
            s0 += *p;
        }
    }
    d = (f32)t->start;
    *(B14Cubic *)tbl = *(B14Cubic *)&t->c0;
    dt = x - d;
    return ((B14Cubic *)tbl)->c0 + ((B14Cubic *)tbl)->c1 * dt + ((B14Cubic *)tbl)->c2 * dt * dt + ((B14Cubic *)tbl)->c3 * dt * dt * dt;
}
