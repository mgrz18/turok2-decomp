/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_002555D0.c, func_002555D0.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef unsigned int u32;
typedef signed int s32;
typedef struct B12HBlk B12HBlk;
struct B12HBlk {
               B12HBlk *next;
               B12HBlk *prev;
               s32 unk8;
               s32 unkC;
               u32 hdr;
               u32 size;
};
typedef struct B12Heap {
               u32 start;
               u32 end;
               B12HBlk *first;
               B12HBlk *last;
               B12HBlk *cur;
} B12Heap;
u8 *func_002021F0(B12Heap *h, u8 *ptr, u32 size) {
    B12HBlk *f;
    B12HBlk *b;
    B12HBlk *p;
    B12HBlk *nx;
    B12HBlk *q;
    u32 need = (size + 0x2F) & ~0xF;
    u32 lo;
    u32 end;
    u32 bend;
    q = h->first;
    if (q) {
        f = q;
        while (f->prev && f->prev < (B12HBlk *)ptr) {
            f = f->prev;
        }
        lo = (u32)f + f->hdr;
        end = lo + f->size;
        b = (B12HBlk *)(ptr - 0x20);
        if (!((u32)b < lo)) {
            bend = (u32)b + need;
            if (!(end < bend)) {
                b->hdr = need;
                b->size = end - bend;
                if (b->size != 0) {
                    b->next = f;
                    p = f->prev;
                    b->prev = p;
                    if (f->prev) {
                        p->next = b;
                    }
                    f->prev = b;
                    if (h->last == f) {
                        h->last = b;
                    }
                } else {
                    b->next = 0;
                    b->prev = 0;
                }
                f->size = (u32)b - ((u32)f + f->hdr);
                if (f->size == 0) {
                    if (f->next) {
                        f->next->prev = f->prev;
                    }
                    if (f->prev) {
                        f->prev->next = f->next;
                    }
                    if (h->first == f) {
                        h->first = f->prev;
                    }
                    if (h->last == f) {
                        if (f->prev) {
                            h->last = f->prev;
                        } else {
                            h->last = f->next;
                        }
                    }
                    f->next = 0;
                    f->prev = 0;
                }
                b->unk8 = (s32)f;
                nx = (B12HBlk *)f->unkC;
                b->unkC = (s32)nx;
                if (f->unkC) {
                    nx->unk8 = (s32)b;
                }
                f->unkC = (s32)b;
                return ptr;
            }
        }
    }
    return 0;
}
