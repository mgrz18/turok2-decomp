/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80254D44_de_16.c, func_80255780_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/heap.h"

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
