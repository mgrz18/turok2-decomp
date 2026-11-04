/* Second VM module copy of func_0042F75C (same source, linked 0xE0 lower). */
#include "common.h"
#include "structs/demo.h"

void func_0042EB50(DemoStream *);

/* Reads count bits, most significant first. */
static __inline__ s32 readBits(DemoStream *r, s32 count) {
    s32 v = 0;
    u32 pos;

    for (count--; count != -1; count--) {
        v <<= 1;
        pos = r->pos++;
        v |= (r->bits[pos >> 3] & (0x80 >> (pos & 7))) != 0;
    }
    return v;
}

void func_0046867C(DemoStream *t) {
    t->value = readBits(t, 8);
    func_0042EB50(t);
}
