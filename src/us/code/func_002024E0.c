/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80254D44_de_16.c, func_80255920_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/heap.h"

void *CHeap__Alloc(CHeap *heap, s32 request) {
    CHeapBlock *block;
    CHeapBlock *split;
    u32 size;
    size = (request + 0x2F) & ~0xF;
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
