/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80254D44_de_16.c, func_80255B2C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/heap.h"

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
