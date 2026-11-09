#include "common.h"
#include "structs/heap.h"

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
