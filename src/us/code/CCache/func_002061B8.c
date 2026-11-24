#include "common.h"
#include "globals/sysrc.h"
#include "structs/cache.h"
#include "protos/cache.h"
#include "protos/heap.h"

/* Drops every lock of the entry (one at a time, as the ROM's loop does) and frees it when nothing
 * else holds it (flags 0x702). The host build used to keep a second, natural form of this; the N64
 * code was a Rage Wars CAI view of the CMemEntry until round 28. */
void CCache__DeallocateInternal(s32 arg0, void *entry) {
    CMemEntry *e = entry;
    s32 n;

    e->flags &= ~2;
    if (e->lockCount != 0) {
loop_1:
        do {
            n = e->lockCount - 1;
            e->lockCount = n;
            if (n != 0) {
                goto loop_1;
            }
            e->flags &= ~MEMENTRY_FLAG_LOCKED;
        } while (e->lockCount != 0);
    }
    if (!(e->flags & 0x702)) {
        CCache__DetachCacheEntry(0, e);
        CHeap__Free(&gHeap, e->data);
        CCache__FreeMemEntry(0, e);
    }
}
