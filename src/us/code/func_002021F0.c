/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_002555D0.c, func_002555D0.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/heap.h"

u8 *CHeap__AllocAt(CHeap *h, u8 *ptr, u32 size) {
    CHeapBlock *f;
    CHeapBlock *b;
    CHeapBlock *p;
    CHeapBlock *nx;
    CHeapBlock *q;
    u32 need = (size + 0x2F) & ~0xF;
    u32 lo;
    u32 end;
    u32 bend;
    q = h->freeHead;
    if (q) {
        f = q;
        while (f->freeNext && f->freeNext < (CHeapBlock *)ptr) {
            f = f->freeNext;
        }
        lo = (u32)f + f->used;
        end = lo + f->free;
        b = (CHeapBlock *)(ptr - sizeof(CHeapBlock));
        if (!((u32)b < lo)) {
            bend = (u32)b + need;
            if (!(end < bend)) {
                b->used = need;
                b->free = end - bend;
                if (b->free != 0) {
                    b->freeLast = f;
                    p = f->freeNext;
                    b->freeNext = p;
                    if (f->freeNext) {
                        p->freeLast = b;
                    }
                    f->freeNext = b;
                    if (h->freeTail == f) {
                        h->freeTail = b;
                    }
                } else {
                    b->freeLast = 0;
                    b->freeNext = 0;
                }
                f->free = (u32)b - ((u32)f + f->used);
                if (f->free == 0) {
                    if (f->freeLast) {
                        f->freeLast->freeNext = f->freeNext;
                    }
                    if (f->freeNext) {
                        f->freeNext->freeLast = f->freeLast;
                    }
                    if (h->freeHead == f) {
                        h->freeHead = f->freeNext;
                    }
                    if (h->freeTail == f) {
                        if (f->freeNext) {
                            h->freeTail = f->freeNext;
                        } else {
                            h->freeTail = f->freeLast;
                        }
                    }
                    f->freeLast = 0;
                    f->freeNext = 0;
                }
                b->usedLast = f;
                nx = f->usedNext;
                b->usedNext = nx;
                if (f->usedNext) {
                    nx->usedLast = b;
                }
                f->usedNext = b;
                return ptr;
            }
        }
    }
    return 0;
}
