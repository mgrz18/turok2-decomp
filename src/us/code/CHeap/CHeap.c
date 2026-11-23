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
    uptr need = (size + (sizeof(CHeapBlock) + 0xF)) & ~0xF;
    uptr lo;
    uptr end;
    uptr bend;
    q = h->freeHead;
    if (q) {
        f = q;
        while (f->freeNext && f->freeNext < (CHeapBlock *)ptr) {
            f = f->freeNext;
        }
        lo = (uptr)f + f->used;
        end = lo + f->free;
        b = (CHeapBlock *)(ptr - sizeof(CHeapBlock));
        if (!((uptr)b < lo)) {
            bend = (uptr)b + need;
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
                f->free = (uptr)b - ((uptr)f + f->used);
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

void CHeap__FreeAll(CHeap *heap)
{
    CHeapBlock *block;
    CHeapBlock *header;
    CHeapBlock *other;
    CHeapBlock *scan;
    CHeapBlock *next;
    while (heap->usedHead->usedNext != 0) {
        header = heap->usedHead->usedNext;
        block = header->usedLast;
        if (block->free == 0) {
            other = header->freeLast;
            if (other != 0) {
                block->freeLast = other;
            } else {
                scan = block;
                while (scan != 0) {
                    if (scan->free != 0) {
                        break;
                    }
                    scan = scan->usedLast;
                }
                if (scan != 0) {
                    block->freeLast = scan;
                } else {
                    goto insert_head;
                }
                other = scan;
            }
            next = other->freeNext;
            block->freeNext = next;
            if (other->freeNext != 0) {
                next->freeLast = block;
            }
            other->freeNext = block;
            if (heap->freeTail == other) {
                heap->freeTail = block;
            }
            goto merge;
insert_head:
            block->freeNext = heap->freeHead;
            if (heap->freeHead != 0) {
                heap->freeHead->freeLast = block;
            } else {
                heap->freeTail = block;
            }
            heap->freeHead = block;
        }
merge:
        block->free += header->used + header->free;
        if (header->usedNext != 0) {
            header->usedNext->usedLast = header->usedLast;
        }
        header->usedLast->usedNext = header->usedNext;
        if (header->free != 0) {
            if (header->freeLast != 0) {
                header->freeLast->freeNext = header->freeNext;
            }
            if (header->freeNext != 0) {
                header->freeNext->freeLast = header->freeLast;
            }
            if (heap->freeTail == header) {
                heap->freeTail = header->freeLast;
            }
        }
    }
}

void CHeap__Construct(CHeap *heap, u8 *start, sptr size) {
    u8 *end = start + size;
    CHeapBlock *block = (CHeapBlock *)(((uptr)start + 15) & ~15);

    size -= (u8 *)block - start;
    size &= ~15;
    heap->start = start;
    heap->end = end;
    block->freeNext = 0;
    block->freeLast = 0;
    block->usedNext = 0;
    block->usedLast = 0;
    block->used = sizeof(CHeapBlock);
    block->free = size - sizeof(CHeapBlock);
    heap->usedHead = block;
    if (block->free != 0) {
        heap->freeTail = block;
        heap->freeHead = block;
    } else {
        heap->freeTail = NULL;
        heap->freeHead = NULL;
    }
}

void *CHeap__Alloc(CHeap *heap, s32 request) {
    CHeapBlock *block;
    CHeapBlock *split;
    u32 size;
    size = (request + (sizeof(CHeapBlock) + 0xF)) & ~0xF;
    for (block = heap->freeHead; block != 0; block = block->freeNext) {
        if (block->free >= size) {
            { u8 *cursor = (u8 *)block; cursor += block->used; split = (CHeapBlock *)cursor; }
            split->used = size;
            split->free = block->free - size;
            if (split->free != 0) {
                split->freeLast = block;
                split->freeNext = block->freeNext;
                if (block->freeNext != 0) {
                    split->freeNext->freeLast = split;
                }
                block->freeNext = split;
                if (heap->freeTail == block) {
                    heap->freeTail = split;
                }
            } else {
                split->freeLast = 0;
                split->freeNext = 0;
            }
            block->free = 0;
            if (block->freeLast != 0) {
                block->freeLast->freeNext = block->freeNext;
            }
            if (block->freeNext != 0) {
                block->freeNext->freeLast = block->freeLast;
            }
            if (heap->freeHead == block) {
                heap->freeHead = block->freeNext;
            }
            if (heap->freeTail == block) {
                heap->freeTail = block->freeNext != 0 ? block->freeNext : block->freeLast;
            }
            block->freeLast = 0;
            block->freeNext = 0;
            split->usedLast = block;
            split->usedNext = block->usedNext;
            if (block->usedNext != 0) {
                split->usedNext->usedLast = split;
            }
            block->usedNext = split;
            return split + 1;
        }
    }
    return 0;
}

void *CHeap__AllocTop(CHeap *heap, s32 request)
{
    CHeapBlock *block;
    CHeapBlock *split;
    u32 size;
    size = (request + (sizeof(CHeapBlock) + 0xF)) & ~0xF;
    for (block = heap->freeTail; block != 0; block = block->freeLast) {
        if (block->free >= size) {
            { u8 *cursor = (u8 *)block; cursor += block->used; cursor += block->free; cursor -= size; split = (CHeapBlock *)cursor; }
            split->used = size;
            split->free = 0;
            block->free -= size;
            split->freeNext = 0;
            split->freeLast = 0;
            if (block->free == 0) {
                if (block->freeLast != 0) {
                    block->freeLast->freeNext = block->freeNext;
                }
                if (block->freeNext != 0) {
                    block->freeNext->freeLast = block->freeLast;
                }
                if (heap->freeHead == block) {
                    heap->freeHead = block->freeNext;
                }
                if (heap->freeTail == block) {
                    heap->freeTail = block->freeNext != 0 ? block->freeNext : block->freeLast;
                }
                block->freeLast = 0;
                block->freeNext = 0;
            }
            split->usedLast = block;
            split->usedNext = block->usedNext;
            if (block->usedNext != 0) {
                split->usedNext->usedLast = split;
            }
            block->usedNext = split;
            return split + 1;
        }
    }
    return 0;
}

s32 CHeap__Free(CHeap *heap, u8 *data) {
    CHeapBlock *block;
    CHeapBlock *header;
    CHeapBlock *other;
    CHeapBlock *scan;
    uptr next;
    header = (CHeapBlock *)(data - sizeof(CHeapBlock));
    block = header->usedLast;
    if (block->free == 0) {
        other = header->freeLast;
        if (other != 0) {
            block->freeLast = other;
        } else {
            scan = block;
            while (scan != 0) {
                if (scan->free != 0) {
                    break;
                }
                scan = scan->usedLast;
            }
            if (scan != 0) {
                block->freeLast = scan;
            } else {
                goto insert_head;
            }
            other = scan;
        }
        next = (uptr)other->freeNext;
        block->freeNext = (CHeapBlock *)next;
        if (other->freeNext != 0) {
            *(CHeapBlock **)next = block;
        }
        other->freeNext = block;
        if (heap->freeTail == other) {
            heap->freeTail = block;
        }
        goto merge;
insert_head:
        block->freeNext = heap->freeHead;
        if (heap->freeHead != 0) {
            heap->freeHead->freeLast = block;
        } else {
            heap->freeTail = block;
        }
        heap->freeHead = block;
    }
merge:
    block->free += header->used + header->free;
    if (header->usedNext != 0) {
        header->usedNext->usedLast = header->usedLast;
    }
    header->usedLast->usedNext = header->usedNext;
    if (header->free != 0) {
        if (header->freeLast != 0) {
            header->freeLast->freeNext = header->freeNext;
        }
        if (header->freeNext != 0) {
            header->freeNext->freeLast = header->freeLast;
        }
        if (heap->freeTail == header) {
            heap->freeTail = header->freeLast;
        }
    }
    return block->free - sizeof(CHeapBlock);
}

/* Sums the block sizes into *arg1 and keeps the largest in *arg2. */
void CHeap__GetFreeStats(CHeap *heap, s32 *arg1, u32 *arg2) {
    u32 temp_a0;
    u32 var_v1;
    CHeapBlock *var_a3;

    *arg1 = 0;
    *arg2 = 0;
    var_a3 = heap->freeHead;
    if (var_a3 != NULL) {
        do {
            *arg1 += var_a3->free;
            var_v1 = var_a3->free;
            temp_a0 = *arg2;
            if (var_v1 < temp_a0) {
                var_v1 = temp_a0;
            }
            *arg2 = var_v1;
            var_a3 = var_a3->freeNext;
        } while (var_a3 != NULL);
    }
}
